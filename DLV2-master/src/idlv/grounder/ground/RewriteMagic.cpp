/*******************************************************************************
 * Copyright 2016 Francesco Calimeri, Davide Fusca', Simona Perri and Jessica Zangari
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/
/*
 * RewriteMagic.cpp
 *
 *  Created on: Mar 30, 2016
 *      Author: davide
 */

#include "RewriteMagic.h"

namespace DLV2 {
namespace grounder {

RewriteMagic::RewriteMagic(vector<Rule*>& idb,
		const vector<Rule*>& constraints, const vector<Rule*>& wconstraints,
		vector<Atom*>* query, bool isqueryground): IDB_OLD(idb),
		          firstRule(0),
		          IsQueryGround(isqueryground),
		          AdPredsAndTargs(),
		          EDBAdornedPredicates(),
		          magicNames(),
		          Sources()        {
	for(auto rule:idb)
		IDB.emplace_back(rule);
	for(auto atom:*query)
		Query.emplace_back(atom->clone());
//    assert( ! IDB.empty() );
    size_t fact = IDB.size() - 1;
    while( true )
        {
        while( firstRule <= fact && IDB[ firstRule ].getSizeHead() == 1 && IDB[ firstRule ].getSizeBody() == 0 )
            firstRule++;

        while( firstRule < fact && ( IDB[ fact ].getSizeHead() > 1 || IDB[ fact ].getSizeBody() > 0 ) )
            fact--;

        if( firstRule < fact )
            {
            RULE tmp = IDB[ fact ];
            IDB[ fact ] = IDB[ firstRule ];
            IDB[ firstRule ] = tmp;
            }
        else
            break;
        }

    for( size_t i = firstRule; i < IDB.size(); i++ )
        {
        const vector<SHAREDATOM> &head = IDB[ i ].getHead();
        assert( head.size() > 1 || IDB[ i ].getSizeBody() > 0  );

        for( vector<SHAREDATOM>::const_iterator j = head.begin();
                     j != head.end();
                     j++ )
            {
            PREDICATES_RULES_INDEX::iterator k =
                PredicatesRulesIndex.find( (*j)->getPredicate()->getName());
            if( k == PredicatesRulesIndex.end() )
                {
                vector<size_t> tmp;
                tmp.push_back( i );
                PredicatesRulesIndex.insert(
                    PREDICATES_RULES_INDEX::value_type((*j)->getPredicate()->getName(),
                                                       tmp));
                }
            else
                {
                k->second.push_back( i );
                }
            }
        }

    OptionOptimizedDisjunctiveMagicSets=false;
    OptionMagicSetsExplicit=true;
    OptionSubsumptionCheckingAfterDMS=false;
    }

void RewriteMagic::normalizeMagicPredicateNames()
    {
	unordered_set<string> names;
	PredicateTable::getInstance()->getPredicateName(names);

    for( set_predicate::iterator it = magicNames.begin(); it != magicNames.end(); it++ )
        {
        // Normalize predicate names of magic predicates (of the
        // form "magic_predicateName_adornment[_suffix]) until a really new
        // name (not already internally defined or occurring in
        // the input program) is found.

        char str[256];
        strcpy( str, (*it)->getName().c_str() );
        for(unsigned i = 0; i < strlen(str); i++)
            {
            if( str[i] == '#' )
                str[i] = '_';
            }


		char str2[256];
        bool find=true;
        while(find){
			unsigned count=IdGenerator::getInstance()->getNewId();
			sprintf( str2, "%s_%u", str, count );
			find=names.count(str2);
        }
        strcpy( str, str2 );
//        pair<index_object, bool> res =
//            ATOM::Predicates.find( str );
//        if( res.second )
//            {
//            int count = 0;
//            do
//                {
//                sprintf( str2, "%s_%u", str, count );
//                res = ATOM::Prediticates.find( str2 );
//                count++;
////                if( TraceLevel >= 3 )
////                   cdebug << "Trying to normalize " << *it << " to "
////                          << str2 << endl;
//                }
//            while(res.second);
//            strcpy( str, str2 );
//            }

//        if( TraceLevel >= 3 )
//           cdebug << "Normalizing " << *it
//                  << " to " << str << endl;



//        res = ATOM::Predicates.find( it->c_str() );
//        assert( res.second);
//        PREDICATE_NAMES::index_t idx = res.first;
//
//        // Use the result only for assertion checking at the moment.
//#ifndef NDEBUG
//        res =
//#endif
//        ATOM::Predicates.substitute(str,
//                                    ATOM::Predicates.getArity(idx),
//                                    ATOM::Predicates.getType(idx),
//                                    idx);
//
//        // Make sure we do the substitution only once; the value is
//        // false if the predicate name already existed.
//        assert( res.second );
        (*it)->setName(str);
        }
    }

} /* namespace grounder */
} /* namespace DLV2 */

