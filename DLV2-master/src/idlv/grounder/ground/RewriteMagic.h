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
 * RewriteMagic.h
 *
 *  Created on: Mar 30, 2016
 *      Author: davide
 */

#ifndef SRC_GROUNDER_GROUND_REWRITEMAGIC_H_
#define SRC_GROUNDER_GROUND_REWRITEMAGIC_H_

#include <memory>

#include "../statement/Rule.h"
#include "../table/PredicateExtension.h"


namespace DLV2 {
namespace grounder {

#define MAGIC_PREFIX "magic#"

//bool subsumption(const Rule*, vector<Rule*>&);

using SHAREDATOM = shared_ptr<Atom>;

class RULE{
public:
	RULE(){};

	RULE(Rule *rule){
		for(auto atom:rule->getHead()){
			shared_ptr<Atom> shared(atom->clone());
			head.push_back(shared);
		}
		for(auto atom:rule->getBody()){
			shared_ptr<Atom> shared(atom->clone());
			body.push_back(shared);
		}
	}

	RULE(const RULE& r){
		for(auto atom:r.head)
			head.emplace_back(atom->clone());
		for(auto atom:r.body)
			body.emplace_back(atom->clone());
	}

	RULE& operator=(const RULE& r){
		head.clear();
		body.clear();
		for(auto atom:r.head)
			head.emplace_back(atom->clone());
		for(auto atom:r.body)
			body.emplace_back(atom->clone());
		return *this;
	}

	bool isChoiceRule() const{ return (head.size()==1 && head[0]->isChoice());}

	SHAREDATOM getAtomInHead(unsigned i)const {return head[i];}

	const vector<SHAREDATOM>& getHead()const {return head;}

	const vector<SHAREDATOM>& getBody()const{return body;}

	vector<SHAREDATOM> getClonedBody()const{
		vector<SHAREDATOM> newBody;
		for(auto atom:body)
			newBody.emplace_back(atom->clone());
		return newBody;
	}

	vector<SHAREDATOM> getClonedHead()const{
		vector<SHAREDATOM> newBody;
		for(auto atom:head)
			newBody.emplace_back(atom->clone());
		return newBody;
	}

	void setHead(vector<SHAREDATOM>& head){
		this->head=head;
	}

	void setBody(vector<SHAREDATOM>& body){
			this->body=body;
	}

	///This method adds an atom in the head
	void addInHead(SHAREDATOM a){head.push_back(a);};
	///This method adds an atom in the body
	void addInBody(SHAREDATOM a){body.push_back(a);};

	///Insert an element in the body at specified position
	void insertInBody(SHAREDATOM atom,unsigned position){body.insert(body.begin()+position,atom);};

	///This method returns an iterator that points to the first atom in the body
	vector<SHAREDATOM>::iterator getBeginBody(){return body.begin();};
	vector<SHAREDATOM>::const_iterator getBeginBody()const{return body.begin();};
	///This method returns an iterator that points to the last atom in the body
	vector<SHAREDATOM>::const_iterator getEndBody()const{return body.end();};
	vector<SHAREDATOM>::iterator getEndBody(){return body.end();};
	///This method returns an iterator that points to the first atom in the head
	vector<SHAREDATOM>::const_iterator getBeginHead()const{return head.begin();};
	///This method returns an iterator that points to the last atom in the head
	vector<SHAREDATOM>::const_iterator getEndHead()const{return head.end();};

	unsigned getSizeHead()const {return head.size();}

	unsigned getSizeBody()const {return body.size();}


private:
	vector<SHAREDATOM> head;
	vector<SHAREDATOM> body;
};


// when an adorned predicate is stored, we have to specify the
// target of the adornment
enum TARGET { undefined = 0, all, allButSource, onlySource };

typedef pair<string,TARGET> NAMESITEM_TARGET;


ostream & operator<<(ostream &out, const TARGET &target);

class RewriteMagic {
private:
Predicate* createPredicate(string newName,Predicate *oldPredicate){
	Predicate *newPred=new Predicate(newName,oldPredicate->getArity(),oldPredicate->isEdb());
	if(newName!=PREDNAME_QUERY)
		newPred->setHiddenForPrinting(true);
	else
		newPred->setHiddenForPrinting(false);
	PredicateTable::getInstance()->insertPredicate(newPred);
	PredicateExtTable::getInstance()->addPredicateExt(newPred);
	return newPred;
}
public:
	RewriteMagic(vector<Rule*> &idb,
            const vector<Rule*> &constraints,
            const vector<Rule*> &wconstraints,
			vector<Atom*> *query,
            bool isqueryground );

	void normalizeMagicPredicateNames();

private:
    vector<RULE> IDB;
    vector<Rule*>& IDB_OLD;
    size_t firstRule;
    vector<SHAREDATOM> Query;
    bool IsQueryGround;
    bool OptionOptimizedDisjunctiveMagicSets;
    bool OptionMagicSetsExplicit;
    bool OptionSubsumptionCheckingAfterDMS;

    // store the set of bound variables for each rule
    typedef set_term SetofBoundVars;

    vector<NAMESITEM_TARGET> AdPredsAndTargs;

    typedef unordered_set<string> SetOfAdornedPredicates;

    // FIXME: this name should be changed, since this set includes also
    // built-ins
    SetOfAdornedPredicates EDBAdornedPredicates;


//    set<string> magicNames;
    set_predicate magicNames;

    // Given a predicate name, generate the corresponding magic predicate
    // name and record it in the structure magicNames.
    Predicate* createMagicPredicateName(Predicate* oldPredicate)
        {
        string magicName(MAGIC_PREFIX + oldPredicate->getName());


        // Insert the new magic name into the structure holding all magic
        // names, ignoring the return value (we don't care whether it has
        // been generated already).
        Predicate *magicPredicate=createPredicate(magicName,oldPredicate);
        magicNames.insert(magicPredicate);
        return magicPredicate;
        }

    bool isMagic(Predicate* name)
        {
        return magicNames.find(name) != magicNames.end();
        }


    bool isMagic(const SHAREDATOM atom)
        {
    	assert(atom->hasPredicate());
        return isMagic(atom->getPredicate());
        }


    ////////////// ELEMENTS OF THE SOURCES TABLE ///////////////////////
    //
    // one adorned predicate can be generated during the adornment of a
    // rule, or a constraint or the query
    union ADORNMENT_GENERATOR
        {
        vector<SHAREDATOM> *iQuery;
        RULE *iRule;
        RULE *iConstr;
        WeakConstraint *iWConstr;
        };

    // when we deal with pointers and iterators, we need to specify the
    // type of object referred by them
    enum AG_TYPE { typeQuery,
                   typeNormalRule,
                   typeDisjunctive,
                   typeConstraint,
                   typeWConstraint };

    // the source of an adornment is constituted by:
    // the type of the pointer to the source (a rule or a constraint, or
    // the query),
    // the pointer to the source,
    // the atom for which the adornment has been generated,
    // a flag indicating whether the adornment is in progress.
    struct SOURCE
        {
        AG_TYPE ag_type;
        ADORNMENT_GENERATOR ag;
        size_t atom_pos;
        bool toBeAdorned;
        };


    // Maps - depending on availability use unordered_map, hash_map,
    // or ordinary map templates.
    // SOURCES_TABLE associates sources to adornments
    // PREDICATES_RULES_INDEX associates rules (by means of their
    // indices) to predicates

    struct eqstr
        {
        bool operator()(const char* s1, const char* s2) const
            {
            return strcmp(s1, s2) == 0;
            }
        };

//#ifdef HAVE_UNORDEREDMAP
    typedef unordered_map<string, SOURCE>
        SOURCES_TABLE;
    typedef unordered_map<string, vector<size_t> >
	PREDICATES_RULES_INDEX;
//#else
//#ifdef HAVE_HASHMAP
//    typedef hash_map<const char*, SOURCE, hash<const char*>, eqstr>
//        SOURCES_TABLE;
//    typedef hash_map<string, vector<size_t> >, hash<const char*>, eqstr>
//	PREDICATES_RULES_INDEX;
//#else
//    typedef map<const char*, SOURCE, hash<const char*>, eqstr>
//        SOURCES_TABLE;
//    typedef map<const char*, vector<size_t> >
//	PREDICATES_RULES_INDEX;
//#endif
//#endif

    SOURCES_TABLE Sources;

    PREDICATES_RULES_INDEX PredicatesRulesIndex;

    /////////////////////////////////////////////////////////
    // return true if a predicate is adorned, false otherwise
    // a predicate is adorned if its name contains the character '#'
    bool isAdorned(const string &nameAtom)
        {
        const char *name = nameAtom.c_str();
        // even the special predicate "#query" can be adorned
        if( strstr(nameAtom.c_str(),"#query") )
            return ( strcmp(strchr(name,'#'),strrchr(name,'#')) != 0 );
        else
            return ( *name != '#' && strchr(nameAtom.c_str(),'#') != 0 );
        }

    // get the adornment from the name of an adorned atom
    const char* getAdornment(const string &nameAtom)
        {
        assert( isAdorned(nameAtom) );
        char *adornment = new char[strlen(strrchr(nameAtom.c_str(),'#'))];
        strcpy(adornment,strrchr(nameAtom.c_str(),'#')+1);
        return adornment;
        }


    // receive the name of an adorned atom and get the value of the
    // adornment corresponding to a particular position pos
    char getAdornment(const string &nameAtom, unsigned pos)
        {
        assert( isAdorned(nameAtom) );
        const char *adornment = getAdornment(nameAtom);
        assert( pos < strlen(adornment) );
        char binding = adornment[pos];
        delete[] adornment;
        return binding;
        }


    // receive the name of an atom and get the name of the atom
    // without adornment (if it is adorned)
    char* getUnadornedName(const string &nameAdAtom)
        {
        // If the atom is propositional return the name itself.
        char *name;
        if( !isAdorned(nameAdAtom) )
            {
            name = new char[strlen(nameAdAtom.c_str())+1];
            strcpy(name,nameAdAtom.c_str());
            }
        else
            {
            const char *adornment = getAdornment(nameAdAtom);
            unsigned len = strlen(nameAdAtom.c_str()) -
                strlen(adornment)-1;
            delete[] adornment;
            name = new char[len+1];
            strncpy(name,nameAdAtom.c_str(),len);
            name[len] ='\0';
            }

        return name;
        }

    // receive an atom and an adornment and builds a new adorned atom
    SHAREDATOM buildAdornedAtom(SHAREDATOM a,const char *adornment)
        {
        // it is possible to adorn an atom only if it is IDB
    	assert(a->hasPredicate());
        char *name = new char[strlen(a->getPredicate()->getName().c_str())+a->getPredicate()->getArity()+2];
        strcpy(name,a->getPredicate()->getName().c_str());
        strcat(strcat(name,"#"),adornment);
        Predicate *newPred=createPredicate(name,a->getPredicate());

        SHAREDATOM aa(a->clone());
        aa->setPredicate(newPred);
        delete[] name;
        return aa;
        }

    // receive an atom and an adorned name and build a new adorned atom
    // with the same name of adornedName
    SHAREDATOM buildAdornedAtom(SHAREDATOM a,const string &adornedName)
        {
        // it is possible to adorn an atom only if it is IDB and not
        // propositional
//        assert( a.isIDB() && !a.isPropositional() );
    	assert(a->hasPredicate());
    	Predicate *newPred=createPredicate(adornedName,a->getPredicate());
        SHAREDATOM aa(a->clone());
        aa->setPredicate(newPred);
        return aa;
        }

    // receive a single query atom and build the adorned version
    SHAREDATOM adornQueryAtom(SHAREDATOM query_atom)
        {

        const vector<Term*>& params = query_atom->getTerms();
    	assert(query_atom->hasPredicate());
        char *adornment= new char[query_atom->getPredicate()->getArity()+1];

        // initialize the string
        strcpy(adornment,"");
        for( vector<Term*>::const_iterator i=params.begin();
             i != params.end();
             i++ )
            if( (*i)->getType()==VARIABLE || (*i)->getType()==ANONYMOUS || ( (*i)->getType()==FUNCTION && !(*i)->isGround() ) )
                {
                strcat(adornment,"f");
                }
            else
                {
//                assert( i->isInt() || i->getType() == TERM::String ||
//                    ( i->isComplex() && i->getComplexTerm().isConstantTerm() ) );
                strcat(adornment,"b");
                }
        SHAREDATOM a=buildAdornedAtom(query_atom,adornment);
        delete[] adornment;
        return a;
        }

    // get the set of bound variables of one adorned atom and insert
    // them into the set of bound variables of the rule
    // @param adorned_atom, the atom whose bound variables must be added
    //        to the set of bound variables BoundVars
    // @param BoundVars, the set of bound variables at the current
    //        position of the considered rule
    void getBoundVars(SHAREDATOM &adorned_atom,
                      SetofBoundVars &BoundVars)
        {
//        assert( adorned_atom.isIDB() );
        if ( !adorned_atom->getTermsSize()==0 )
            {
            vector<Term*> &params = adorned_atom->getTerms();
//            assert( params != 0 );
            assert(adorned_atom->hasPredicate());
            const char *adornment = getAdornment(adorned_atom->getPredicate()->getName());
            const char *adornmentpos = adornment;

            assert( strlen(adornment)==(params.size()) );
            for( vector<Term*> ::const_iterator i=params.begin();
                 i != params.end() ;
                 i++,adornmentpos++ )
                {
//                assert( !(*i).isNull() );
                if( ((*i)->getType()==VARIABLE ) && (*adornmentpos == 'b') )
                    BoundVars.insert(*i);
                }

            delete[] adornment;
            }
        }

    // adorn a literal according to the set of bound variables collected
    // so far
    // @param L, the literal to be adorned
    // @param BoundVars, the set of bound variables at the current
    //        position of the considered rule
    SHAREDATOM adornLiteral(SHAREDATOM L,const SetofBoundVars &BoundVars)
        {
//        assert( L.isIDB() && !L.isPropositional() );
        assert(L->hasPredicate());
        char *adornment= new char[L->getPredicate()->getArity()+1];
        strcpy(adornment,"");
        const vector<Term*>& params = L->getTerms();
        for( vector<Term*>::const_iterator i=params.begin();
             i != params.end();
             i++ )
            if( (*i)->getType()==VARIABLE || (*i)->getType()==ANONYMOUS)
                {
                // if it is contained into the SetofBoundVars put it to
                // bound,else put it to false
                if( BoundVars.find(*i) != BoundVars.end() )
                    strcat(adornment,"b");
                else
                    strcat(adornment,"f");
                }
            else if( (*i)->getType()==FUNCTION && !(*i)->isGround() )
                // complex terms with variables are free
                // FIXME: we should reconsider these terms
                {
                strcat(adornment,"f");
                }
            else
                // the term is neither a variable nor a constant complex term:
                // it is a constant
                {
//                assert( i->isInt() || i->getType() == TERM::String ||
//                    ( i->isComplex() && i->getComplexTerm().isConstantTerm() ) );
                strcat(adornment,"b");
                }

        SHAREDATOM L1=buildAdornedAtom(L,adornment);
        L1->setNegative(L->isNegative());
        delete[] adornment;
        return L1;
        }

    // check if an atom has at least one constant term.
    // @param a, the atom to be checked if it has bound terms
    bool hasConstants(SHAREDATOM a)
        {
//        assert( !a.isPropositional() );
        const vector<Term*> &params = a->getTerms();
        for( vector<Term*>::const_iterator k=params.begin();
             k != params.end();
             k++ )
            // FIXME: We would need a method isConstant() here.
            if( (*k)->isGround() )
                return true;
        return false;
        }

    // check if an atom has at least one bound term w.r.t. the current set
    // of bound variables or if it has at least one constant term.
    // @param a, the atom to be checked if it has bound terms
    // @param BV, the set of bound variables at the current position of
    //        the considered rule
    bool hasBoundTerms(SHAREDATOM a,const SetofBoundVars &BV)
        {
//        assert( !a.isPropositional() );
        //check if the literal contains at least a bound term
        const vector<Term*> &params = a->getTerms();
        for( vector<Term*>::const_iterator k=params.begin();
             k != params.end();
             k++ )
            if( ( (*k)->getType()==VARIABLE   && BV.find(*k) != BV.end())
                // FIXME: We would need a method isConstant() here.
                || (*k)->isGround() )
                return true;
        return false;
        }

    // receive an adorned IDB atom and return true if it contains at least
    // a bound term
    // @param a, the adorned IDB atom to be checked if it has bound terms
    bool hasBoundTermsIDB(const SHAREDATOM ad)
        {
//        assert( ad.isIDB() );
        if ( ad->getTermsSize()==0 )
            return false;
        assert(ad->hasPredicate());
        const char *adornment = getAdornment(ad->getPredicate()->getName());
        bool bound = ( strchr(adornment,'b') != 0 );
        delete[] adornment;
        return bound;
        }

    // calculate the number of bound terms in atom a w.r.t. the
    // current set of bound variables
    // @param a, the atom whose number of bound terms must be calculated
    // @param BV, the set of bound variables at the current position of
    //        the considered rule
    size_t getNumberBoundTerms(SHAREDATOM a,const SetofBoundVars &BV)
        {
        size_t count=0;
        const vector<Term*> &params = a->getTerms();
        for( vector<Term*>::const_iterator i=params.begin();
             i != params.end();
             i++ )
            if( ( (*i)->getType()==VARIABLE && BV.find(*i) != BV.end() )
                // FIXME: We would need a method isConstant() here.
                ||  (*i)->isGround() )
                count++;
        return count;
        }

    // update the set of bound variables with all the variables of a
    // literal
    // @param BV, the set of bound variables at the current position of
    //        the considered rule
    // @param L, the literal whose variables must be added to the set of
    //        bound variables BV
    void updateBoundVars(SetofBoundVars &BV,SHAREDATOM L)
        {
//        assert( !L.isPropositional() );
        const vector<Term*> &params = L->getTerms();
        for( vector<Term*>::const_iterator i=params.begin();
             i != params.end();
             i++ )
            if( (*i)->getType()==VARIABLE && BV.find(*i) == BV.end() )
                BV.insert(*i);
        }


    //////////////////////////////////////ADORN///////////////////////

    // return true if a literal is a comparison built-in (except the
    // equality built-in)
    // FIXME: this function should probably be relocated
    // @param L, the builtin literal to be checked if it a comparison
    //        builtin, (except the equality built-in)
    bool isComparison(const SHAREDATOM L)
        {
//        assert( L->isBuiltIn() && (L->getTerm(0)->contain(ARITH) || L->getTerm(1)->contain(ARITH) ) );
        if(L->getBinop()==EQUAL)
        	return false;

        return true;

//        switch( L->getBinop() )
//            {
//            case BUILTIN_TYPE::BuiltinInequality:
//            case BUILTIN_TYPE::BuiltinLess:
//            case BUILTIN_TYPE::BuiltinGreater:
//            case BUILTIN_TYPE::BuiltinLessEqual:
//            case BUILTIN_TYPE::BuiltinGreaterEqual:
//                return true;
//            default: return false;
//            }
        }

    // return true if the variables of a literal are all bound w.r.t.
    // the current set of bound variables
    // @param L, the literal to be checked
    // @param BV, the set of bound variables at the current position of
    //        the considered rule
    bool fullyBound(SHAREDATOM L,const SetofBoundVars &BV)
        {
//        assert( !L.isPropositional() );
        const vector<Term*> &params = L->getTerms();
        for( vector<Term*> ::const_iterator i=params.begin();
             i != params.end();
             i++ )
            if( (*i)->getType()==VARIABLE && BV.find(*i) == BV.end() )
                return false;
        return true;
        }

    // return true if a literal is permissible w.r.t. the current set of
    // bound variables
    // the condition of permissibility depends on the type
    // of the literal and on the number of bound terms
    // intuitively, a literal is permissible, if the number of tuples it
    // represents is considerably restricted
    // @param L, the literal to be checked
    // @param BV, the set of bound variables at the current position of
    //        the considered rule
    bool isPermissible(SHAREDATOM L,const SetofBoundVars &BV)
        {
        // a propositional literal is permissible
        if ( L->getTermsSize()==0 )
            return true;
        if( ( L->isNegative() || (L->isBuiltIn() && isComparison(L)) )
            && fullyBound(L,BV) )
            return true;

        // FIXME: binding through builtins disabled for the moment.
        //if( L.isBuiltin() && ( L.getBuiltinID()==BUILTIN_TYPE::BuiltinEquality
        //                       && hasBoundTerms(L,BV) ) )
        //    return true;

        // FIXME: to decide how many variables are requested to be bound
        // for math operators
        // if ( L.isBuiltin() && ( L.getBuiltinID()==BUILTIN_TYPE::BuiltinAddition
        //                         || L.getBuiltinID()==BUILTIN_TYPE::BuiltinMultiplication )
        //                    && hasBoundTerms(L,BV) )
        //     return true;

        if ( !L->isNegative() && !L->isBuiltIn() )
            return true;

        return false;
        }


    // return true if the first literal is preferable compared to the
    // second literal w.r.t. the current set of bound variables
    // the condition of preferability depends on the ability of a literal
    // to propagate the binding
    // @param L1, the literal to be compared
    // @param L2, the literal to be compared
    // @param BV, the set of bound variables at the current position of
    //        the considered rule
    bool isPreferable( SHAREDATOM L1,
                       SHAREDATOM L2,
                      const SetofBoundVars &BV)
        {
        // an undefined propositional literal is always preferable
//        if( L1->getTermsSize()==0 && L1.isUndef() )
//            return true;
        // an EDB propositional literal is always preferable

        if(L1->hasPredicate() &&  L1->getPredicate()->isEdb() && L1->getTermsSize()==0 )
            return true;
        if(L2->hasPredicate() && L2->getPredicate()->isEdb()&& L2->getTermsSize()==0 )
            return false;
        // an IDB propositional literal is never preferable
        if(L1->hasPredicate() && !L1->getPredicate()->isEdb() && L1->getTermsSize()==0 )
            return false;
        if(L2->hasPredicate() && !L2->getPredicate()->isEdb() && L2->getTermsSize()==0)
            return true;

        // if the first literal has bound terms and the second has not, it
        // is preferable
        if( hasBoundTerms(L1,BV) && !hasBoundTerms(L2,BV) )
            return true;
        // if both literals have bound terms...
        if( hasBoundTerms(L1,BV) && hasBoundTerms(L2,BV) )
            {
            // ... an EDB or a builtin is preferable
            if(L1->isBuiltIn() || (L1->hasPredicate() && L1->getPredicate()->isEdb() ))
                return true;
            else // L1 is IDB
                if(!L2->hasPredicate() || !L2->getPredicate()->isEdb() )
                    // ... and if both literals are IDB, the one with
                    // fully bound is preferable
                    {
                    if( fullyBound(L1,BV) )
                        return true;
                    else
                        if( fullyBound(L2,BV) )
                            return false;
                        else // if both are not fully bound, the one with
                            // the greater number of bound variables is
                            // preferable
                            return getNumberBoundTerms(L1,BV)
                                >= getNumberBoundTerms(L2,BV);
                    }
            }

        // if both literals are completely free, choose the one which is
        // EDB or a builtin; if even this does not decide, try to keep the
        // original ordering
        if( !hasBoundTerms(L1,BV) && !hasBoundTerms(L2,BV)
            && ( L1->getPredicate()->isEdb() || L1->isBuiltIn()|| !L2->getPredicate()->isEdb() ) )
            return true;
        return false;
        }

//    ////////////////////////////////////////////////////////////////////////
//    // receive an adorned atom aa and return a magic atom with the same
//    // predicate name beginning with underscore and with only the bound
//    // variables of aa as parameters
//        static LOCAL_VARIABLES localVariables;

    //
    // @param aa, the atom to transform
    SHAREDATOM magic(SHAREDATOM aa)
        {
        assert( !aa->getPredicate()->isEdb());

        vector<Term*> params;

        if( ! aa->getTermsSize()==0)
            {
            const  vector<Term*>& oldparams = aa->getTerms();
//            assert( oldparams != 0 );
            const char *adornment = getAdornment(aa->getPredicate()->getName());
            const char *adornmentpos = adornment;
            for( vector<Term*> ::const_iterator i=oldparams.begin();
                 i != oldparams.end();
                 i++,adornmentpos++ )
                if( *adornmentpos == 'b' )
                    params.push_back(*i);
            delete[] adornment;
            }

        string name=aa->getPredicate()->getName();
        Predicate *oldPred=new Predicate(name,params.size(),aa->getPredicate()->isEdb());
        Predicate* p=createMagicPredicateName(oldPred);
        delete oldPred;
        SHAREDATOM m(aa->clone());
        m->setPredicate(p);
        m->setTerms(params);

        return m;
        }


    // Check if t1 and t2 are the same terms
    //
    // @param t1, the terms of one atom
    // @param t2, the terms of another atom
    bool sameTerms(const vector<Term*> &t1, const vector<Term*>& t2)
        {
//        if( t1 == 0 && t2 == 0 )
//            return true;
//        if( t1 == 0 || t2 == 0 )
//            return false;
        if( t1.size() != t2.size() )
            return false;
        else
            {
            vector<Term*>::const_iterator i1,i2;
            for(i1=t1.begin(),i2=t2.begin();
                i1!=t1.end();
                i1++,i2++)
                if( (*i1) != (*i2) )
                    return false;
            return true;
            }
        assert(0);
        return true;
        }


    // return true if s is exactly the source corresponding to the
    // parameters ag, type and a.
    //
    // @param s, the source of the current adornment
    // @param ag, the pointer to the source of the adornment (normal or
    //         disjunctive rule, strong or weak constraint, the query)
    // @param type, the type of the source of the adornment (normal or
    //         disjunctive rule, strong or weak constraint, query
    bool sameSource(const SOURCE &s, const ADORNMENT_GENERATOR &ag,
                    const AG_TYPE &ag_type)
        {
        if( s.ag_type != ag_type )
            return false;
        switch(s.ag_type)
            {
            case typeNormalRule:
            case typeDisjunctive:
                return (s.ag.iRule == ag.iRule);
            case typeQuery:
                return (s.ag.iQuery == ag.iQuery);
            default:
                assert(0);
                break;
            }
        assert(0);
        return false;
        }


    // return true if s1 and s2 represent the same source
    //
    // @param s1, a source of an adorned predicate (a normal or
    //        disjunctive rule, a strong or weak constraint,
    //        the query)
    // @param s2, another source of an adorned predicate (a normal or
    //        disjunctive rule, a strong or weak constraint,
    //        the query)
    bool sameSource(const SOURCE &s1,const SOURCE &s2)
        {
        if( sameSource(s1,s2.ag,s2.ag_type) &&
            s1.atom_pos != s2.atom_pos )
            return true;
        return false;
        }


    // Return true if the parameters ag, type and a represent exactly source s.
    // A source s is different from the one represented by ag, ag_type and
    // a if:
    // - 'type' is different from s.ag_type, or
    // - the source referred by 'ag' is not the same source referred by s.ag, or
    // - the terms of atom 'a' are not the same terms as s.t.
    //
    // @param s, a source of an adornment, (a normal or
    //        disjunctive rule, a strong or weak constraint,
    //        the query)
    // @param ag, pointer to the source of an adornment (normal or
    //         disjunctive rule, strong or weak constraint, the query)
    // @param type, the type of the source of the adornment referred by ag
    //       (normal or disjunctive rule, strong ////////////////////////////////////////
    //    // receive an adorned atom aa and return a magic atom with the same
    //    // predicate name beginning with underscore and with only the bound
    //    // variables of aa as parameters
    //        static LOCAL_VARIABLES localVariables;or weak constraint, query
    // @param pos, the pos to the atom for which the adornment has
    //       been generated
    bool sameSource(const SOURCE &s,
                    const ADORNMENT_GENERATOR &ag,
                    const AG_TYPE type,
                    const size_t &pos)
        {
        if( sameSource(s,ag,type) &&
            s.atom_pos == pos )
            return true;
        return false;
        }

    // get position of literal L in the body of a rule
    //
    // @param L, the literal to look for
    // @param body, the body of the rule to check
    size_t getPosFromBody(const SHAREDATOM L,
                          const vector<SHAREDATOM> &body)
        {
    	for(unsigned i=0;i<body.size();i++)
    		if(*body[i]==*L)
    			return i+1;

    	return body.end() -body.begin()+1;
        }

    // get position of literal L in a rule or in a constraint
    //
    // @param L, the literal to look for
    // @param ag, the pointer to the rule or constraint to check
    size_t getLiteralPosition(SHAREDATOM L,
                              const ADORNMENT_GENERATOR &ag,
                              const AG_TYPE type)
        {
        size_t pos = 0;
        if( type == typeNormalRule )
            {
            assert( ag.iRule->getHead().size() == 1 );
            SHAREDATOM headAtom=ag.iRule->getAtomInHead(0);
            if( *headAtom == *L )
                pos = 1;
            else
                pos = getPosFromBody(L,ag.iRule->getBody())
                    + ag.iRule->getHead().size();
            }
        else
            if( type == typeDisjunctive )
                {
                assert( ag.iRule->getHead().size() > 1 );
                if( find(ag.iRule->getHead().begin(),
                         ag.iRule->getHead().end(),L)
                    != ag.iRule->getHead().end() )
                    pos =  find(ag.iRule->getHead().begin(),
                                ag.iRule->getHead().end(),L)
                        - ag.iRule->getHead().begin() + 1;
                else
                    pos = getPosFromBody(L,(ag.iRule->getBody(),
                                             ag.iRule->getBody()))
                        + ag.iRule->getHead().size();
                }

        return pos;
        }

    // check whether an adorned predicate is in the head of a rule.
    //
    // @param ad_pred, the adorned predicate which we are looking for
    // @param head, the head which should include ad_pred.
    bool predIsInHead(const string &item,const vector<SHAREDATOM> &head)
        {
        const char *name = getUnadornedName(item);
        for( vector<SHAREDATOM>::const_iterator j = head.begin();
             j != head.end();
             j++ )
        {
            // if predicate with name "name" is in the head of the rule,
            // stop the for and return true
        	assert((*j)->hasPredicate());
            if( strcmp( name,(*j)->getPredicate()->getName().c_str() ) == 0 )
                {
                delete [] name;
                return true;
                }
        }
        delete [] name;
        return false;
        }


    void insertPredicate(const SHAREDATOM aL,
                         const size_t pos,
                         const AG_TYPE &ag_type,
                         const ADORNMENT_GENERATOR &ag)
        {
    	assert(aL->hasPredicate());
        const char *name = aL->getPredicate()->getName().c_str();
        string nameStr=aL->getPredicate()->getName();
        const string item = aL->getPredicate()->getName();

        SOURCES_TABLE::iterator k = Sources.find(aL->getPredicate()->getName());
        vector<NAMESITEM_TARGET>::iterator h =
            find( AdPredsAndTargs.begin(),
                  AdPredsAndTargs.end(),
                  NAMESITEM_TARGET(item,TARGET()));

        // If a is not in the sources table, it cannot be in the
        // vector of the adorned predicates either and the program has
        // never been adorned by aL.
        if(  k == Sources.end() )
            {
            assert( h == AdPredsAndTargs.end() );

            // Create the corresponding entry in table Sources.
            // A source is made up of:
            // - a pointer to the rule or constraint where the new adorned
            //   predicate has been generated.
            // - the type of the source ( query, normal or disjunctive
            //   rule, strong or weak constraint ).
            // - the terms of the new adorned atom.
            // - the type of the atom ( EDB, IDB, etc. ).
            // - the flag 'toBeAdorned', to check if the new adorned
            //   predicate is currently adorning the program, or not.
            SOURCE s;
            s.ag = ag;
            s.ag_type = ag_type;
            s.atom_pos = pos;

            // Build the corresponding NAMESITEM_TARGET object with
            // undefined target ...
            NAMESITEM_TARGET at(item,TARGET());

            // ... and then assign the target according to the type of
            // the source of aL.
            // If the source of aL is the query or a normal rule (without
            // disjunction), at must adorn the whole program. So, the
            // target is 'all' and 'toBeAdorned' is true.
            if( ! OptionOptimizedDisjunctiveMagicSets ||
                ag_type == typeQuery || ag_type == typeNormalRule )
                {
                at.second = all;
                s.toBeAdorned = true;
                }
            else // If the source of aL is a constraint or a disjunctive
                // rule, at can adorn the whole program except its source,
                // so the target is 'allButSource'.
                {
                at.second = allButSource;
                s.toBeAdorned = false;
                }

            // In any case, insert the adorned predicate in the
            // vector of the adorned predicates and its
            // source in the Sources table.
            AdPredsAndTargs.push_back(at);
            Sources.insert({nameStr,s});
            }
        else
            // at is already in the Sources table.
            {
            SOURCE &s = k->second;
            // at is also in the vector of adorned predicates: at has
            // never adorned the program yet.
            if( h != AdPredsAndTargs.end() )
                {
                assert( (k->first) == (h->first) );
                NAMESITEM_TARGET &at = *h;
                // If the adorned predicate corresponding to aL is already
                // in the vector (at) with target 'allButSource' and the
                // source of aL is different from the one in the source
                // table, the target must be changed to 'all' and the
                // source must be adorned by the new instance of the
                // adorned predicate provided by aL.
                //
                // The source s of the adorned predicate already present
                // in the table, is different from the current source of
                // aL, if:
                // - the type of the current source of aL is different
                //   from s.ag_type, or
                // - the current source of aL is not the same source
                //   referred by s.ag, or
                // - aL is not the atom in position s.atom_pos.
                if( at.second == allButSource
                    && !sameSource(s,ag,ag_type,pos) )
                    {
                    at.second = all;
                    s.toBeAdorned = true;
                    }
                }
            else // at is not in the vector: it has already adorned the
                // program, but aL is a new instance of at for
                // which the binding should be propagated.
                {
                assert( h == AdPredsAndTargs.end() );

                // If at had target 'allButSource', the new instance
                // of at must be inserted in the vector of adorned
                // predicates with target 'onlySource'.
                if( !s.toBeAdorned  && !sameSource(s,ag,ag_type) &&
                    predIsInHead(name,(s.ag.iRule)->getHead()) )
                    {
                    // 'toBeAdorned' set to true: the source must be
                    // adorned by the new instance of at.
                    s.toBeAdorned=true;
                    NAMESITEM_TARGET NewAt(item,onlySource);
                    AdPredsAndTargs.push_back(NewAt);

                    }
                }
            }

       }


    // if a literal is IDB and not propositional adorn it and add it to
    // the body of the rule to be adorned, otherwise simply add it
    //
    // @param L, the literal to be added
    // @param body, the conjunction to which the literal is to be added
    // @param ag_type, the type of the source of the adornment (normal or
    //           disjunctive rule, strong or weak constraint, query
    // @param ag, the pointer to the source of the adornment (normal or
    //           disjunctive rule, strong or weak constraint, query
    // @param BV, the set of bound variables at the current position of
    //           the considered rule
    //
    void adornAndAddLiteralToBody(SHAREDATOM L,
                                  vector<SHAREDATOM> &body,
                                  const AG_TYPE &ag_type,
                                  const ADORNMENT_GENERATOR &ag,
                                  SetofBoundVars &BV)
        {

        size_t pos = getLiteralPosition(L,ag,ag_type);
        if( L->hasPredicate() &&  !L->getPredicate()->isEdb())
            {
            if( !L->getTermsSize()==0 )
                {
                SHAREDATOM aL(adornLiteral(L,BV));
                // insert the adorned literal aL to the body of the rule
                insertPredicate(aL,pos,ag_type,ag);
                body.push_back(aL);
                }
            else
                {
                insertPredicate(L,pos,ag_type,ag);
                body.emplace_back(L->clone());
                }
            }
        else // EDB or built-in literal
            {
            // if L is EDB or built-in and it has never been involved in
            // any adornment, insert it into the set of EDB "adorned"
            // predicates

            if( (L->isBuiltIn() || (L->hasPredicate() && L->getPredicate()->isEdb())  ))
            {
            	string name=(L->isBuiltIn())
            			?L->getBinopToStrng(L->getBinop())
						:L->getPredicate()->getName();
            	if( EDBAdornedPredicates.find(name) == EDBAdornedPredicates.end() )
					EDBAdornedPredicates.insert(name);
            }
            // In any case (EDB or built-in), the literal must be added to
            // the body of the adorned rule
            // FIXME: The separation between positive and negative
            // literals is explicitly ignored in order to ensure the
            // intended ordering of literals.
            body.emplace_back(L->clone());
            }
        }


    // receive an adorned predicate aa and a rule r with the unadorned
    // version of aa in the head and return the rule r1, which is the
    // adorned version of r w.r.t. the adornment of aa
    //
    // @param r, the rule to be adorned
    // @param aa, the predicate name for which the binding is propagated
    // @param ag_type, the type of the source of the adornment (normal or
    //           disjunctive rule, strong or weak constraint, query
    // @param ag, the pointer to the source of the adornment (normal or
    //           disjunctive rule, strong or weak constraint, query
    RULE adornRule(const RULE& r,
                   const string &aa,
                   const AG_TYPE &ag_type,
				   const ADORNMENT_GENERATOR &ag)
        {
trace_action(grounding,2,
        cout << "adorning rule: ";
        for( unsigned i=0; i<r.getSizeHead(); i++ ) {
            r.getAtomInHead(i)->print(cout);
            cout << ",";
        }
        cout << ":-";
        for( unsigned i=0; i<r.getSizeBody(); i++ ) {
            r.getBody().at(i)->print(cout);
            cout << ",";
        }
        cout << endl;
        );
        assert( r.getSizeHead()==1 );
        vector<SHAREDATOM> newRuleHead;
        SHAREDATOM head = r.getAtomInHead(0);
        // the rules which are IDB facts, e.g. p(1,2)., matching with the
        // unadorned version of aa, must be adorned w.r.t. the adornment
        // of aa.
        // for instance if the adornment of aa is '#bb' the new
        // adorned IDB fact will be p#bb(1,2).
        if( r.getSizeBody()==0 )
            {
            assert( ag_type != typeQuery );
            if( !head->getTermsSize()==0 )
                {
#ifndef NDEBUG
                const char *name = getUnadornedName(aa);
                assert( strcmp( head->getPredicate()->getName().c_str(), name ) == 0 );
                delete[] name;
#endif
                newRuleHead.push_back(buildAdornedAtom(head,aa));
                RULE newRule;
                newRule.setHead(newRuleHead);
                return newRule;
                }
            else
                return RULE(r);

            }
        else
            // the rule to be adorned has a body
            {
            if( !head->getTermsSize()==0 )
                {
#ifndef NDEBUG
                const char *name = getUnadornedName(aa);
                assert( strcmp( head->getPredicate()->getName().c_str(), name ) == 0 );
                delete[] name;
#endif
                // the new rule must have in the head a copy of the
                // literal aa with the same adornment
                newRuleHead.push_back(buildAdornedAtom(head,aa));
                }
            else
                newRuleHead.emplace_back(head->clone());

            vector<SHAREDATOM> newRuleBody;

            //initialize BV with the set of bound variables of the adorned head
            SetofBoundVars BV;
            assert( newRuleHead.size() == 1 );
            getBoundVars(*(newRuleHead.begin()),BV);

            // store all the body literals of r in notYetConsideredLiterals
            vector<SHAREDATOM> notYetConsideredLiterals=r.getClonedBody();

            // add the permissible literals of the original body to the
            // new body according to the order defined by the preferential
            // criterion.
            bool foundPermissible = true;
            while( !notYetConsideredLiterals.empty() && foundPermissible )
                {


                foundPermissible=false;
                vector<SHAREDATOM>::iterator i =
                    notYetConsideredLiterals.begin();
                // selection of the first permissible LITERAL to propagate
                // bindings

		trace_action(grounding,2,cout << "atom "; (*i)->print(cout); cout << endl; );
                while( i != notYetConsideredLiterals.end()
                       && !foundPermissible )
                    {
                    if( isPermissible(*i,BV) )
                        // first permissible literal found
                        foundPermissible=true;
                    else
                        // not yet found a permissible literal
                        i++;
                    }


                if( i == notYetConsideredLiterals.end() )
                    // No permissible literal found.
                    foundPermissible=false;
                else
                    {
                    // selection of the best permissible LITERAL to
                    // propagate bindings, through the comparison of the
                    // chosen permissible body literal with the remaining
                    // permissible body literals
                    if( i != notYetConsideredLiterals.end()-1 )
                        {
                        vector<SHAREDATOM>::iterator j = i+1;
                        while( i != notYetConsideredLiterals.end()-1
                               && j != notYetConsideredLiterals.end() )
                            {
                            while( j!=notYetConsideredLiterals.end()
                                   && !isPermissible(*j,BV) )
                                j++;
                            if( j != notYetConsideredLiterals.end() )
                                {
                                // another permissible literal found;
                                // choose the preferable one
                                i = (isPreferable(*i,*j,BV))?i:j;
                                j++;
                                }
                            }
                        }

                    // if a literal is IDB and not propositional adorn it
                    // and add it to the body of the rule to be adorned,
                    // otherwise simply add it to the body of the rule
                    adornAndAddLiteralToBody(*i,
                                             newRuleBody,
                                             ag_type,
                                             ag,
                                             BV);


		    // update the set of bound variables of the current
		    // rule adding all of the variables of the just chosen
		    // literal
		    if( !(*i)->getTermsSize()==0 && hasBoundTerms(*i,BV) )
                        updateBoundVars(BV,*i);
		    // we have considered the literal *i
		    notYetConsideredLiterals.erase(i);
                    }
                }
            // if there are remaining literals it means that they are not
            // permissible and we do not have to propagate the adornment
            // through them, but they must be added to the body of the new
            // rule in any case
            for( vector<SHAREDATOM>::iterator k =
                     notYetConsideredLiterals.begin();
                 k != notYetConsideredLiterals.end();
                 k++ )
                adornAndAddLiteralToBody(*k,
                                         newRuleBody,
                                         ag_type,
                                         ag,
                                         BV);


            // finally create the adorned rule
            RULE r1;
            r1.setBody(newRuleBody);
            r1.setHead(newRuleHead);
trace_action(grounding,2,
        cout << "adorned rule: ";
        for( unsigned i=0; i<r1.getSizeHead(); i++ ) {
            r1.getAtomInHead(i)->print(cout);
            cout << ",";
        }
        cout << ":-";
        for( unsigned i=0; i<r1.getSizeBody(); i++ ) {
            r1.getBody().at(i)->print(cout);
            cout << ",";
        }
        cout << endl;
        );
            return r1;
            }

        // This point should not be reached in any execution path, but some
        // compilers do not recognise this.
        assert(0);
        return nullptr;
        }

    //////////////////////////////////////////////////////////////////////////////////////
    // given a (disjunctive) rule containing an atom a in the head, build a normal rule with
    // only a in the head, and shift the remaining head literals to the
    // body.
    // return the new rule
    // For instance, given a rule
    // r: p(t) V  p1(t) V... V pn(t) :-q1(t),...,qm(t).
    // and an adorned atom p(t), build a normal rule by shifting the head
    // literals p1(t)... pn(t) to the body with negation.
    // the resulting rule will be:
    // p(t) :- q1(t),...,qm(t), not p1(t),...,not pn(t)
    //
    // @param a, the istance of the adorned predicate at for which the
    //        binding is propagated
    // @param r,
    RULE shiftHead( SHAREDATOM a,RULE &r)
        {
        assert( r.getSizeHead()>0 );
        assert( r.getSizeHead() >= 1 );
        assert( find_if(r.getBeginHead(),r.getEndHead(),[&a](SHAREDATOM a1){return *a1==*a;}) != r.getEndHead() );
        if( r.getSizeHead() == 1)
            return RULE(r);

        vector<SHAREDATOM> newHead;
        newHead.push_back(a);
        // build the body of the new rule with all the predicates
        // already present in the original body and then shift the other
        // head atoms different from a
        vector<SHAREDATOM> newBody;
        if( r.getSizeBody()>0)
            newBody = r.getClonedBody();
        for( vector<SHAREDATOM>::const_iterator j= r.getBeginHead();
             j != r.getEndHead();
             j++ )
            if (!(**j == *a)){
            	SHAREDATOM cloneAtom((*j)->clone());
            	cloneAtom->setNegative(true);
            	newBody.push_back(cloneAtom);
            }

        RULE sr;
        sr.setBody(newBody);
        sr.setHead(newHead);
        return sr;
        }


    // check if two literals have the same terms
    //
    // @param L1, one literal to compare.
    // @param L1, another literal to compare.
    bool sameTerms(SHAREDATOM L1,SHAREDATOM L2)
        {
        if( L1->getTermsSize()==0 &&  L2->getTermsSize()==0 )
            return true;
        else
            if ( !L1->getTermsSize()==0 &&  !L2->getTermsSize()==0 )
                {
                vector<Term*> &params1 = L1->getTerms();
                vector<Term*> &params2 = L2->getTerms();
                return sameTerms(params1,params2);
                }
            else
                return false;
        }


    // given an adorned normal rule
    // r:  p#...#(t) :- q1#...#(t),...,not qm#...#(t), not p1#b...b#(t),...,not pn#b...b#(t)
    // and a head h=p(t),p1(t) v ... v pn(t) (with n>=1)
    // build a (disjunctive) rule by shifting the adorned literals
    // not p1#b...b#(t),...,not pn#b...b#(t) from the body to the head,
    // without negation.
    // the resulting rule will be:
    // p#...#(t) v p1#b...b#(t) v ... v pn#b...b#(t) :- q1#...#(t), ..., qm#...#(t)
    //
    // @param r, the rule whose head should be shifted back;
    // @param originalHead, the original head of r in case it was a disjunctive rule.
    void shiftBackHead(RULE&r,const vector<SHAREDATOM> &originalHead)
        {


        assert( r.getSizeHead() == 1 );
        // modify the normal rule r by shifting, from its body to its
        // head, the positive versions of the literals occurring with
        // negation in the body of r and occurring also in the original
        // unadorned head originalHead
        if( originalHead.size() == 1 ) //the original rule was
            //normal: no body literal
            //has to be shifted back
            return;
        else
            {
            const SHAREDATOM a=r.getAtomInHead(0);
            assert(!r.isChoiceRule());
            const char *nameHead = getUnadornedName(a->getPredicate()->getName());
            const vector<SHAREDATOM>& body = r.getBody();
            vector<SHAREDATOM> newBody;
            for( vector<SHAREDATOM>::const_iterator i = body.begin();
                 i != body.end();
                 i++ )
                {
                // Find atoms of the original head in the body of rule r.
                assert((*i)->hasPredicate());
                const char *name = getUnadornedName((*i)->getPredicate()->getName());
                bool found=false;
                for( vector<SHAREDATOM>::const_iterator j = originalHead.begin();
                     j != originalHead.end() && !found;
                     j++ )
                    {
                    // *j is the current atom of the original head
                    found=false;
                    if( strcmp( (*j)->getPredicate()->getName().c_str(),name ) == 0 && (*i)->isNegative() && sameTerms(*i,*j) )
                        {
                        // Found shifted atom: make it positive and shift
                        // it back to the head.
                        found=true;
                        SHAREDATOM newAtom((*i)->clone());
                        newAtom->setNegative(false);
                        r.addInHead(newAtom);
                        }

                    }
                if( !found )
                    newBody.emplace_back((*i)->clone());
                delete[] name;
                }
            delete[] nameHead;
            r.setBody(newBody);
            }
        }


    // check if an adorned predicate name aa, occurring in the head of a
    // disjunctive rule, could generate a new redundant adorned rule.
    // It could happen if the set of (bound?) variables of the atom a, matching
    // with the predicate name aa, is the same of another atom p of the
    // head already adorned.
    // In fact, in this case, aa could propagate the same bindings already
    // propagated by p from the head to the body and therefore it could be
    // built an adorned rule which, after the modify step, could be the
    // same of the one already obtained from that rule adorned w.r.t. the
    // atom p.
    //
    // @param head, the disjunction data structure storing the head of a rule
    // @param a, the istance of the adorned predicate at for which the
    //        binding is propagated
    // @param aa, the predicate name matching with a
    bool redundancy(const vector<SHAREDATOM> &head,
                     SHAREDATOM a,
                    const string &aa)
        {
        assert( OptionOptimizedDisjunctiveMagicSets );

        for( vector<SHAREDATOM>::const_iterator j = head.begin();
             j != head.end();
             j++ )
            {
            if( *j != a  && sameTerms(*j,a) )
                // check if another head atom *j has been already adorned
                // with the same bindings for the same variables of the
                // adorned version of a
                {
                if( !(*j)->getTermsSize()==0)
                    {
                    SHAREDATOM L = buildAdornedAtom(*j,getAdornment(aa));
                    if( Sources.find(L->getPredicate()->getName()) != Sources.end() )
                        {
                        // if L has already adorned the program, i.e. it
                        // is not in the vector of the adorned predicates
                        // anymore, return true
                        if( find(AdPredsAndTargs.begin(),AdPredsAndTargs.end(),
                                 NAMESITEM_TARGET(L->getPredicate()->getName(),TARGET())) ==
                            AdPredsAndTargs.end() )
                            {

                            return true;
                            }
                        else // if L is still in the vector, i.e. it has
                            // not adorned the program yet, return false
                            {

                            return false;
                            }
                        }
                    }
                }
            }
        return false;
        }


    // Get the atom with pos 'pos' from a rule or a constraint.
    SHAREDATOM getAtomByPos( const vector<SHAREDATOM> &head,
                       const vector<SHAREDATOM> &body,
                      const size_t &pos)
        {
        // FIXME: this assert can be avoided when constraints will be
        // considered.
        assert( head.size() > 0 || body.size() > 0 );

        assert( pos > 0 );
        size_t pos2 = 1;

        for( vector<SHAREDATOM>::const_iterator j = head.begin();
             j != head.end();
             j++, pos2++ )
            if( pos2 == pos )
                return SHAREDATOM((*j)->clone());

        for( vector<SHAREDATOM>::const_iterator j=body.begin();
             j != body.end();
             j++, pos2++ )
            if( pos2 == pos )
                return SHAREDATOM((*j)->clone());

        assert(0);
        return nullptr;
        }

    // Get the atom with pos 'pos' from a rule or a constraint.
    SHAREDATOM getAtomByPos( const vector<SHAREDATOM> &head,
                       const vector<Atom*> &body,
                      const size_t &pos)
        {
        // FIXME: this assert can be avoided when constraints will be
        // considered.
        assert( head.size() > 0 || body.size() > 0 );

        assert( pos > 0 );
        size_t pos2 = 1;

        for( vector<SHAREDATOM>::const_iterator j = head.begin();
             j != head.end();
             j++, pos2++ )
            if( pos2 == pos )
                return SHAREDATOM((*j)->clone());

        for( vector<Atom*>::const_iterator j=body.begin();
             j != body.end();
             j++, pos2++ )
            if( pos2 == pos )
                return SHAREDATOM((*j)->clone());

        assert(0);
        return nullptr;
        }

    //
    SHAREDATOM getAtomByPos(const ADORNMENT_GENERATOR &ag,const AG_TYPE &type,const size_t &pos)
        {
        if( type == typeNormalRule || type == typeDisjunctive )
            {
            if( ag.iRule->getSizeBody()>0 )
                return getAtomByPos(ag.iRule->getHead(),ag.iRule->getBody(),pos);
            else
                return getAtomByPos(ag.iRule->getHead(),vector<SHAREDATOM>(),pos);
            }
        else
            {
        	vector<SHAREDATOM> emptyHead;
            if( type == typeConstraint )
                return getAtomByPos(emptyHead,(ag.iConstr)->getBody(),pos);
            else
                if( type == typeWConstraint ){
                    return getAtomByPos(emptyHead,(ag.iWConstr)->getBody(),pos);
                }
            }
        assert(0);
        return nullptr;
        }

    // adorn the source ( rule or constraint ) that generated the adorned
    // predicate at, w.r.t. another instance of the same adorned predicate
    //
    // @param at, the predicate name for which the binding is propagated
    //        and its target
    // @param onlyForMagic, set to true if the modified constraint is
    //           redundant and so only the corresponding magic rule must be
    //           constructed
    pair<bool,RULE> adornSource(const NAMESITEM_TARGET &at,
                                const SOURCE &s,
                                bool &onlyForMagic)
        {
        assert( at.second == onlySource );
        assert( s.ag_type == typeNormalRule || s.ag_type == typeDisjunctive );
        assert( predIsInHead(at.first,(s.ag.iRule)->getHead()) );
        SHAREDATOM L(getAtomByPos(s.ag,s.ag_type,s.atom_pos));


        RULE shiftedRule(shiftHead(L,*(s.ag.iRule)));
        if( predIsInHead(at.first,shiftedRule.getHead()) )
            {
            const RULE &r = adornRule(shiftedRule,
                                      at.first,
                                      s.ag_type,
                                      s.ag);

            onlyForMagic = OptionOptimizedDisjunctiveMagicSets &&
                redundancy(s.ag.iRule->getHead(),L,at.first);

            return pair<bool,RULE>(true,r);
            }
        else
            return pair<bool,RULE>(false,*(s.ag.iRule));
        }

    // Prevent the generation of redundant rules. The propagation of the
    // binding through atom a, could generate redundancy if the source of
    // at, the predicate for wich the adornment is propagated, is exactly
    // the rule or the constraint referred by pointer ag and the
    // occurrence of at on which the adornment has been generated, is
    // exactly atom a.
    //
    // @param ag, the pointer to the source of the adornment (normal or
    //         disjunctive rule, strong or weak constraint, the query)
    // @param type, the type of the source of the adornment (normal or
    //         disjunctive rule, strong or weak constraint, query
    // @param at, the predicate name for which the binding is propagated
    //        and its target
    // @param pos, the pos of the atom for which the binding should
    //        be propagated
    bool generateRedundancy(const ADORNMENT_GENERATOR &ag,
                            const AG_TYPE &type,
                            const NAMESITEM_TARGET &at,
                            const size_t &pos)
        {
        assert( Sources.find(at.first) != Sources.end() );
        SOURCE &s = (Sources.find(at.first))->second;
        if( type == typeNormalRule )
            return false;
        else
            switch( at.second )
                {
                case onlySource:
                    if( !sameSource(s,ag,type,pos) )
                        return true;
                    else
                        return false;
                    break;

                case allButSource:
                    if( sameSource(s,ag,type,pos) )
                        return true;
                    else
                        return false;
                    break;

                case all:
                    return false;
                    break;

                default:
                    assert(0);
                    return false;
                    break;
                }
        assert(0);
        return false;
        }

    // Given an adorned predicate at and its target, propagate the binding
    // to the rules whose heads match with at or to the constraints
    // containing at least one occurrence of at, creating new adorned
    // predicates; for each new adorned predicate repeat the adornment and
    // propagate the binding to the rules and to the constraints.
    // At the end return the adorned rules.
    //
    // @param at, the adorned predicate for which the binding is
    //        propagated and its target
    // @param ag, the pointer to the source of the adornment (normal or
    //         disjunctive rule, strong or weak constraint, the query)
    // @param type, the type of the source of the adornment (normal or
    //         disjunctive rule, strong or weak constraint, query
    // @param a, the istance of the adorned predicate at for which the
    //        binding is propagated
    // @param onlyForMagic, set to true if the modified constraint is
    //           redundant and so only the corresponding magic rule must be
    //           constructed
    RULE adorn(const NAMESITEM_TARGET &at,
               const ADORNMENT_GENERATOR &ag,
               const AG_TYPE type,
               SHAREDATOM L,
               bool &onlyForMagic)
        {

        // adorn the rule
        assert( type == typeNormalRule || type == typeDisjunctive );
        const RULE& r = adornRule(shiftHead(SHAREDATOM(L->clone()),*(ag.iRule)),
                                  at.first,
                                  type,
                                  ag);
        onlyForMagic = OptionOptimizedDisjunctiveMagicSets &&
            redundancy(ag.iRule->getHead(),L,at.first);

        return r;
        }

    ///////////////////////////////////////////////////////////////

    bool queryRule(const RULE &r)
        {
        const vector<SHAREDATOM>& head=r.getHead();
        assert(!r.isChoiceRule());
        return ( head.size() == 1 && strcmp( head[0]->getPredicate()->getName().c_str(),PREDNAME_QUERY) == 0 )
            || strstr(head[0]->getPredicate()->getName().c_str(),"#query");
        }
    //DA GUARDARE
    ////////////////////////////////GENERATE///////////////////////
    //
    // Receive an adorned rule and return the corresponding generated rules.
    // Build a new magic rule for each IDB literal L in the body which is:
    // adorned (the adornment can be either bound or free) or
    // propositional.
    // The head of the magic rule consists of the magic version of L while
    // the body consists of the magic version of the original head and the
    // literals preceding L in the adorned rule, which were useful to
    // propagate some new binding.
    //
    // @param adornedRule, the adorned rule for which the magic rules are
    //              generated
    vector<RULE> generate(RULE &adornedRule)
        {

        assert( adornedRule.getSizeHead() == 1 );

        vector<RULE> MagicRules;
        bool queryR = queryRule(adornedRule);
        SHAREDATOM head(adornedRule.getAtomInHead(0));
        assert(!adornedRule.isChoiceRule());
        assert( !head->getPredicate()->isEdb());
trace_action(grounding,2,
        cout << "generating magic rule of: ";
        for( unsigned i=0; i<adornedRule.getSizeHead(); i++ ) {
            adornedRule.getAtomInHead(i)->print(cout);
            cout << ",";
        }
        cout << ":-";
        for( unsigned i=0; i<adornedRule.getSizeBody(); i++ ) {
            adornedRule.getBody().at(i)->print(cout);
            cout << ",";
        }
        cout << endl;
        );
        // When a magic rule has to be generated for a literal p, it must
        // include in the body of the magic rule only the literals
        // useful for propagating bindings from the head of the rule to p.
        // An unuseful literal (EDB or IDB) is included only if it is
        // propositional or undefined.
        if( adornedRule.getSizeBody()>0 )
            {
            // for each adorned predicate of the body with bound
            // arguments build a magic rule
            for( vector<SHAREDATOM>::const_iterator i=
                     adornedRule.getBeginBody();
                 i != adornedRule.getEndBody();
                 i++ )
                {
                if( (*i)->hasPredicate() && !(*i)->getPredicate()->isEdb() && ( isAdorned((*i)->getPredicate()->getName()) || (*i)->getTermsSize()==0 )
                    // to avoid generating a useless "reflexive"
                    // magic rule of the form
                    // "magic#p#bf(V):- a(Y,V),magic#p#bf(V)", it is
                    // necessary to check if the literal of the
                    // body to be "switched" with the head of the
                    // adorned rule is not equal.
                    && head != *i )
                    {
                    SetofBoundVars BV;
                    if( !(*i)->getTermsSize()==0 && hasBoundTermsIDB((*i)) ){
                    	SHAREDATOM atom=*i;
                        getBoundVars(atom,BV);
                    }
trace_action(grounding,2,cout<<"w.r.t. ";(*i)->print(cout);cout<<endl;);
                    // the new rule must have in the head the magic
                    // version of the current body literal ...
                    SHAREDATOM magicA = magic(*i);
                    magicA->setNegative(false);
                    SHAREDATOM magicL = magic(head);
                    if( ! (*magicA == *magicL)  )
                        {
                    	vector<SHAREDATOM> newMagicRuleHead;
                    	vector<SHAREDATOM> newMagicRuleBody;
                        newMagicRuleHead.push_back(magicA);
                        // ... and the magic version of the head
                        // literal in the body, except if it is the
                        // query rule built from non ground queries
                        // (see comment to function "rewriteQuery" ).
                        if ( !queryR )
                            newMagicRuleBody.push_back(magicL);

                        // add to the new magic rule all the literals
                        // useful to propagate binding to the considered
                        // body literal *i and preceding it in the body
                        if( i > adornedRule.getBeginBody() )
                            for( vector<SHAREDATOM>::const_iterator j = i-1;
                                 j >= adornedRule.getBeginBody();
                                 j-- )
                                {
trace_action(grounding,2,cout << "atom "; (*j)->print(cout); cout << endl; );
                                bool useful = false;
                                // EDB propositional literal or undefined
                                // propositional literal are considered
                                // useful to be inserted in the magic rule,
                                // even if they are not properly useful to
                                // propagate any binding.
                                if( (*j)->getTermsSize()==0 )
                                    {
                                	//TODO check j->isUndef() ||
                                    if(  ((*j)->hasPredicate() && (*j)->getPredicate()->isEdb()))
{
                                        trace_action(grounding,2,cout << "useful1" << endl; );
                                        useful = true;
}
                                    else useful = false;
                                    }
                                else
                                    {
                                    const vector<Term*> &jTerms=(*j)->getTerms();
                                    for( SetofBoundVars::const_iterator k = BV.begin();
                                         k != BV.end() && !useful;
                                         k++)
                                        {
                                    	vector<Term*>::const_iterator it = find(jTerms.begin(),jTerms.end(),*k);
                                        size_t pos = it - jTerms.begin();
                                    	//TODO check j->isUndef() ||

                                        if( ((*j)->hasPredicate() && (*j)->getPredicate()->isEdb()) || (*j)->isBuiltIn() )
                                            {
                                            if( find(jTerms.begin(),jTerms.end(),*k) != jTerms.end())
                                                {
trace_action(grounding,2,cout << "useful2" << endl; );
                                                useful=true;
                                                updateBoundVars(BV,*j);
                                                }

                                            }
                                        else
                                        {
                                        	assert((*j)->hasPredicate());
                                            if( ( isPermissible(*j,BV) &&
                                                  it != jTerms.end() &&
                                                  ( !isAdorned((*j)->getPredicate()->getName()) ||
                                                    getAdornment((*j)->getPredicate()->getName(),pos) =='f')) )
                                                {
trace_action(grounding,2,cout << "useful3" << endl; );
                                                useful =true;
                                                BV.erase(find(BV.begin(),BV.end(),*k));
                                                if( (*j)->isBuiltIn() )
                                                    updateBoundVars(BV,*j);
                                                else{
                                                	SHAREDATOM atomNonConst=*j;
                                                    getBoundVars(atomNonConst,BV);
                                                }
                                                break;
                                                }
                                        }
                                        }
                                    //TODO check j->isUndef()
                                    if( useful )
                                        newMagicRuleBody.emplace_back((*j)->clone());
                                    }
                                }

                        // build the new magic rule
                        RULE mRule;
                        mRule.setHead(newMagicRuleHead);
                        mRule.setBody(newMagicRuleBody);
trace_action(grounding,2,
        cout << "generated magic rule of: ";
        for( unsigned i=0; i<mRule.getSizeHead(); i++ ) {
            mRule.getAtomInHead(i)->print(cout);
            cout << ",";
        }
        cout << ":-";
        for( unsigned i=0; i<mRule.getSizeBody(); i++ ) {
            mRule.getBody().at(i)->print(cout);
            cout << ",";
        }
        cout << endl;
        );
                        //TODO verificare se aggiungere funzione || ! subsumption(mRule,MagicRules)
                        if( ! OptionOptimizedDisjunctiveMagicSets )
                            {

                            MagicRules.push_back(mRule);

                            }
                        }
                    }
                }
            }
        return MagicRules;
        }


    //////////////////////////MODIFY////////////////////////////////////
    //
    // receive an adorned rule and the corresponding original head and
    // return the modified rule, obtained by adding to the body of each
    // adorned rule the magic version of the head predicate, if it has at
    // least one bound term

    ///////////////////DISJUNCTION////////////////////////////
    //
    // Check if the original head of the rule from which the adorn step
    // generated the current adorned rule: if it is disjunctive, first rebuild
    // the disjunctive head of the adorned rule and then insert for each
    // head literal a magic literal in the body.

    // given an adorned normal rule
    // r: p#...#(t):-q1#...#(t),...,not qm#...#(t),not p1#b...b#(t),...,not pn#b...b#(t)
    // and a head h=p(t),p1(t)V...Vpn(t) (with n>=1)
    // by shifting back the head, rebuild the disjunctive rule:
    // p#...#(t) V p1#b...b#(t) V ... V pn#b...b#(t) :- q1#...#(t), ..., qm#...#(t)
    // and finally modify it by adding the magic version of head literals
    // to the body:
    // p#...#(t) V p1#b...b#(t) V ... V pn#b...b#(t) :-
    // m_p#...#(t),m_p1#b...b#(t),...,m_pn#b...b#(t),q1#...#(t),...,qm#...#(t)

    ////////////////////////////////////////////////////////////

    RULE modify(RULE &adornedRule,const vector<SHAREDATOM> &h)
        {

        // h->size() > 1 means that the original rules was
        // disjunctive; in this case all the literals of the original
        // head are shift back in the head of the modified rule.
        // If the rule was not disjunctive, no literal is shifted
        // back.
        shiftBackHead(adornedRule,h);

       const  vector<SHAREDATOM> &head = (adornedRule).getHead();
        assert( &head );
        assert( head.size() >= 1 );

        // for each adorned literal of the head create the
        // corresponding magic literal and put it into the body of the
        // modified rule
        for( vector<SHAREDATOM>::const_iterator k = head.begin();
             k != head.end();
             k++ )
            {
            SHAREDATOM mL(magic(*k));
            if( adornedRule.getSizeBody()>0)
            	adornedRule.insertInBody(mL,0);

//                (*(adornedRule.getBodyForModification())).
//                    insert((*(adornedRule.getBodyForModification()))
//                           .pos_begin(),mL);
            else // it is a fact
                {
                assert( ! (adornedRule.getSizeBody()>0) );
                vector<SHAREDATOM> body;
                body.insert(body.begin(),mL);
                adornedRule.setBody(body);
                }
            }

        return adornedRule; // modified!!!
        }


    ////////////////////////////// QUERY REWRITING ////////////////////////////////////
    //
    // for each adorned query atom build new rules with the original (not
    // adorned) atom in the head and the adorned atom in the body.
    // For instance, if we have the adorned query atom 'q#bb(2,5)',
    // the new  rule will be: q(2,5):- q#bb(2,5).
    // This is because it is necessary to derive the original query atoms
    // from the rewritten rules where the IDB atoms are magic and adorned.
    //
    // @param adornedQuery, the adorned query to be processed
    vector<RULE> buildQueryRules(vector<SHAREDATOM> &adornedQuery)
        {
    	vector<RULE> queryRules;
        for( vector<SHAREDATOM>::iterator j=adornedQuery.begin();
             j != adornedQuery.end();
             j++ ){
        	assert((*j)->hasPredicate());
            if( !(*j)->getTermsSize()==0 && !(*j)->getPredicate()->isEdb())
                {
            	vector<SHAREDATOM> headQueryRule;
                char *name = getUnadornedName((*j)->getPredicate()->getName());
                SHAREDATOM newAtom((*j)->clone());
                newAtom->setPredicate(createPredicate(name,newAtom->getPredicate()));
                headQueryRule.push_back(newAtom);
                vector<SHAREDATOM> bodyQueryRule;
                bodyQueryRule.push_back(*j);
                RULE qr;
                qr.setHead(headQueryRule);
                qr.setBody(bodyQueryRule);
                queryRules.push_back(qr);
                delete[] name;
                }
        }
        return queryRules;
        }


    // If the query is bound, for each adorned atom of the adorned query
    // build a magic fact for instance, if we have the adorned query atom
    // 'q#bf(2,X)', the new magic fact will be: magic#q#bf(2), that is a
    // relevant fact for answering the original query.
    //
    // @param AdornedQueryRule, the adorned query rule.
    vector<RULE> buildMagicQueryRules(RULE &AdornedQueryRule)
        {
        assert( AdornedQueryRule.getSizeBody()>0 );
        const vector<SHAREDATOM> &AdornedQuery = (AdornedQueryRule.getBody());
        vector<RULE> magicQueryRules;
        bool canBuildMagicRules =false;
        for( vector<SHAREDATOM>::const_iterator i=AdornedQuery.begin();
             i!= AdornedQuery.end();
             i++ )
        {
        	assert((*i)->hasPredicate());
            if( !(*i)->getPredicate()->isEdb() )
                {
                vector<SHAREDATOM> head;
                head.push_back(magic(*i));
                RULE qf;
                qf.setHead(head);
                // if the magic literal is completely ground, build a
                // magic query fact
                if( (qf.getAtomInHead(0))->isGround() )
                    {
                    magicQueryRules.push_back(qf);
                    }
                else
                	{// if the magic literal is completely or partially
                    // non ground a magic rule must be generated. If the
                    // adorned query rule is:
                    //    #query(X,Y):- a(3,X), p#bf(X,Y).mL
                    // the magic query rule will be:
                    //    magic#p#bf(X) :- a(3,X).
                    canBuildMagicRules = true;
                	}
                }
        }
        if( canBuildMagicRules )
            {
            assert( AdornedQueryRule.getSizeHead()>0 && queryRule(AdornedQueryRule) );
            vector<RULE> magicQuery = generate(AdornedQueryRule);
            magicQueryRules.insert(magicQueryRules.end(),magicQuery.begin(),magicQuery.end());
            }

        return magicQueryRules;
        }

    // FIXME!
    // necessary only for non-ground queries (at the moment)
    bool hasBoundLiterals(const RULE& r)
        {
        for(  vector<SHAREDATOM>::const_iterator i=r.getBeginBody();
             i!=r.getEndBody();
             i++ ){
        	assert((*i)->hasPredicate());
            if( !(*i)->getPredicate()->isEdb() && hasBoundTermsIDB(*i) )
                return true;
        }
        return false;
        }

    ////////////////////////////////////////////////////////////////////////////////////////
    // Adorn the original query:
    // 1) for ground queries (represented by a conjunction of literals):
    //    - adorn each literal
    //    - build the query rules
    //    - build the magic facts.
    // 2) for non-ground queries (represented by a rule whose head
    //    predicate is "#query"):
    //    - build the adorned rule.
    //    - generate the magic rule for each IDB atom of the query as
    //      in the following examples (the query predicate in the head
    //      is not made magic and is not inverted with the magic atom
    //      of the body).
    //    - don't generate any modified rule.
    //
    // Ex1:
    // p(2,3),q(1)?
    // adorn the literals:
    // p#bb, q#b
    // build the query rules:
    // p(2,3) :- p#bb(2,3).
    // q(1) :- q#b(1).
    // build the magic facts:
    // magic#p#bb(2,3).
    // magic#q#b(1).
    //
    // Ex2:
    // p(2,X), q(X,Y)?
    // corresponding to the rule:
    // #query(X,Y) :- p(2,X), q(X,Y).
    // adorn the rule:
    // #query(X,Y) :- p#bf(2,X),q#bf(X,Y).
    // build the magic rules:
    // magic#p#bf(2).
    // magic#q#bf(X) :- p#bf(2,X).
    //
    // Ex3:
    // p(X,Y)?
    // corresponding to the rule:
    // #query(X,Y) :- p(X,Y).
    // no need to apply magic sets!
    // the query is completely non-ground: no bindings among variables.
    //
    bool rewriteQuery(const bool groundQuery,
                      RULE& AdornedQueryRule,
                      vector<RULE> &magicQueryRules)
        {
        bool rewrittenQuery = false;
        if( !groundQuery )
            {
            //            bool foundQuery = false;
            // the query rule should be the last rule of the vector IDB;
            // if this is true don't check any other rule.
            //            for( RULES::iterator i = IDB.end()-1;
            //                 i != IDB.begin() && !foundQuery;
            //                 i-- )
            //                {
            //                assert( &(*i).getHead() );
                // if the current rule is exactly the query rule, adorn it.
            //                if( queryRule(*i) )
            //      {

            //      foundQuery = true;
            PREDICATES_RULES_INDEX::const_iterator qr = PredicatesRulesIndex.find("#query");
            assert( qr != PredicatesRulesIndex.end() );
            assert( qr->second.size() == 1 );
            RULE queryRule = IDB[qr->second.front()];
			const vector<SHAREDATOM> &head = queryRule.getHead();
			ADORNMENT_GENERATOR ag;
			ag.iRule=&(IDB[qr->second.front()]);
			assert( head.size() == 1 && !head[0]->getPredicate()->isEdb() && !head[0]->getTermsSize()==0 );
			SHAREDATOM aa=adornQueryAtom(*(head.begin()));
			assert(aa->hasPredicate());
			AdornedQueryRule = adornRule(queryRule,aa->getPredicate()->getName(),typeQuery,ag);
			// if there is at least one literal in the query rule
			// that can propagate a binding (i.e. a literal with
			// some bound argument), generate the magic rules and
			// adorn the program.
			if( OptionMagicSetsExplicit || hasBoundLiterals(AdornedQueryRule) )
				{
				for( vector<SHAREDATOM>::const_iterator j = AdornedQueryRule.getBeginBody();
					 j != AdornedQueryRule.getEndBody();
					 j++ )
					insertPredicate(*j,getLiteralPosition(*j,ag,typeQuery),typeQuery,ag);

				magicQueryRules = buildMagicQueryRules(AdornedQueryRule);

				rewrittenQuery=true;
				}
			else
				{
				rewrittenQuery=false;
				}
			//                    }
			//                }
            }
        else
            {
            // If the query is completely ground it is always possible to
            // adorn it.
            size_t pos;
            vector<SHAREDATOM> adornedQuery;
            for( vector<SHAREDATOM>::iterator i = Query.begin();
                 i != Query.end();
                 i++ )
                {
                pos = i - Query.begin();
                assert((*i)->hasPredicate());
                if ( !(*i)->getPredicate()->isEdb() && !(*i)->getTermsSize()==0 )
                    {
                    SHAREDATOM aa=adornQueryAtom(*i);
                    adornedQuery.push_back(aa);
                    assert(aa->hasPredicate());
                    if( Sources.find(aa->getPredicate()->getName()) == Sources.end() )
                        {
                        ADORNMENT_GENERATOR ag;
                        ag.iQuery = &Query;
                        insertPredicate(aa,pos,typeQuery,ag);
                        }
                    }
                else
                    if( (*i)->getTermsSize()==0 )
                        {
                        if( Sources.find((*i)->getPredicate()->getName()) == Sources.end() )
                            {
                            adornedQuery.emplace_back((*i)->clone());
                            ADORNMENT_GENERATOR ag;
                            ag.iQuery = &Query;
                            insertPredicate((*i),pos,typeQuery,ag);
                            }
                        }
                }
            AdornedQueryRule=RULE();
            AdornedQueryRule.setBody(adornedQuery);
            magicQueryRules = buildMagicQueryRules(AdornedQueryRule);
            rewrittenQuery=true;
            }
        return rewrittenQuery;
        }


    ///////////////////////////////////////////////////////////////////////////////


    // Reorder the body of a rule w.r.t. the separation between positive
    // and negative literals expected for ordinary rules (and previously
    // ignored).
    //
    // @param body, the body to be reordered.
    vector<SHAREDATOM> reorderBody(const vector<SHAREDATOM> &body)
        {
    	vector<SHAREDATOM> body1;
        for( vector<SHAREDATOM>::const_iterator j =
                 body.begin();
             j != body.end();
             j++ )
            body1.emplace_back((*j)->clone());
        return body1;
        }

    // add to the global IDB the modified rules and at the same time
    // implicitly [via the functionality of add()] reorder the literals of
    // the body of each rule w.r.t. positive and negative literals, as
    // required by the grounding step, since in the adornRule procedure
    // they were created without respecting the separation between
    // positive and negative literals
    void addReorderedRules(const vector<RULE> &rules)
        {
        for( vector<RULE>::const_iterator i=rules.begin();
             i!=rules.end();
             i++)
            {
            vector<SHAREDATOM> Head=(*i).getClonedHead();
            vector<SHAREDATOM> Body;
            if( (*i).getSizeBody()>0)
                Body = reorderBody(((*i).getBody()));
            RULE r;
            r.setBody(Body);
            r.setHead(Head);
            IDB.push_back(r);
            }
        }

    // receive an (adorned) atom a and build a new unadorned atom
    // with the same name of a
    SHAREDATOM buildUnadornedAtom(SHAREDATOM a)
        {
        // it is possible to unadorn an atom only if it is IDB and not
        // propositional
        assert(!isMagic(a) && (a->hasPredicate() && !a->getPredicate()->isEdb()) && !a->getTermsSize()==0 && !a->isBuiltIn());
        assert(a->hasPredicate());
        char *name = getUnadornedName((a)->getPredicate()->getName());
        SHAREDATOM unadornedAtom(a->clone());
        unadornedAtom->setPredicate(createPredicate(name,unadornedAtom->getPredicate()));
        delete[] name;
        return unadornedAtom;
        }


    typedef vector<string> ListOfAdornedPredicates;

    // Eliminate the adornments of predicates in the list
    // AdornedPredicates from each rule of program rules containing them.
    // return the unadorned program.
    //
    // FIXME: at the moment we use this function in strippOffAdornments
    // using as parameter the list of all the adorned predicate generated
    // during the adornment. In the future we would choose these adorned
    // predicates, with an appropriate strategy, in order to reduce the
    // stripping of adornments from the program.
    //
    // @param AdornedPredicates, the list of adorned predicates
    //        w.r.t. stripp off the adornments from the program
    // @param rules, the rules of the adorned program.
    vector<RULE> stripOffAdornments(const ListOfAdornedPredicates &AdornedPredicates,
    		vector<RULE> &rules)
        {
    	vector<RULE> unadornedRules;
        for( vector<RULE>::iterator j = rules.begin() + firstRule;
             j != rules.end();
             j++)
            {
            vector<SHAREDATOM> newHead;
            vector<SHAREDATOM> newBody;
            // strip off adornment of adorned predicate *i from head of rule *j
            const vector<SHAREDATOM> &head = (*j).getHead();

            for( vector<SHAREDATOM>::const_iterator k = head.begin();
                 k != head.end();
                 k++ )
                {
            	//TODO check k->isUndef()
            	assert((*k)->hasPredicate());
                if( (*k)->isBuiltIn() || isMagic(*k) || ((*k)->hasPredicate() && (*k)->getPredicate()->isEdb()) || (*k)->getTermsSize()==0
                		 || !isAdorned((*k)->getPredicate()->getName()) )
                    newHead.emplace_back((*k)->clone());
                else
                    if( find(AdornedPredicates.begin(),AdornedPredicates.end(),
                             (*k)->getPredicate()->getName()) != AdornedPredicates.end() ||
                        strstr((*k)->getPredicate()->getName().c_str(),"#query") )
                        {
                        SHAREDATOM unadornedAtom = buildUnadornedAtom(*k);
                        newHead.push_back(unadornedAtom);
                        }
                }
            if( (*j).getSizeBody()>0 )
                // strip off adornment of adorned predicate *i from body of rule *j
                {
                const vector<SHAREDATOM> &body = ((*j).getBody());
                for(  vector<SHAREDATOM>::const_iterator k = body.begin();
                     k != body.end();
                     k++ )
                    {
                	//TODO check k->isUndef()
                    if( (*k)->isBuiltIn()  || isMagic(*k) || ((*k)->hasPredicate() && (*k)->getPredicate()->isEdb()) || (*k)->getTermsSize()==0 )
                        newBody.emplace_back((*k)->clone());
                    else
                        {
                        SHAREDATOM unadornedAtom = buildUnadornedAtom(*k);
                        unadornedAtom->setNegative((*k)->isNegative());
                        newBody.push_back(unadornedAtom);
                        }
                    }
                RULE newrule;
                newrule.setHead(newHead);
                newrule.setBody(newBody);
                unadornedRules.push_back(newrule);
                }
            else{
                RULE newrule;
                newrule.setHead(newHead);
                unadornedRules.push_back(newrule);
            }
            }
        return unadornedRules;
        }


    // get the predicate from the source table s and insert them in a list
    // return the list of predicates
    ListOfAdornedPredicates getPredicates()
        {
        ListOfAdornedPredicates l;
        for(SOURCES_TABLE::iterator x = Sources.begin();
            x != Sources.end();
            x++)
            {
            l.push_back((x->first));
            }
        return l;
        }


public:
    // perform the Magic Sets Optimization Technique
    void rewrite(const bool groundQuery)
         {
         assert( !Query.empty() );
//         if( TraceLevel >= 1 )
//             {
//             cdebug << "-------    Magic Sets Rewriting   -------"
//                    << endl << endl;
//             }

         if( ! groundQuery && PredicatesRulesIndex.find("#query") == PredicatesRulesIndex.end() )
             {
//             if( TraceLevel >= 1 )
//                 {
//                 cdebug << "skip magic sets rewriting because #query rule was removed." << endl;
//                 }
             return;
             }

//         if( TraceLevel >= 1 )
//             {
//             if( OptionOptimizedDisjunctiveMagicSets )
//                 cdebug<<"Prevention of redundancy of modified rules will be performed." << endl;
//             else
//                 cdebug<<"Prevention of redundancy of modified rules will be not performed." << endl;
//             }

//         if( TraceLevel >= 1 )
//             {
//             cdebug<<"Original Rules"<<endl;
//             print_list(cdebug,IDB,"\n");
//             cdebug<<endl;
//             print_list(cdebug,Constraints,"\n");
//             cdebug<<endl;
//             print_list(cdebug,WConstraints,"\n");
//             cdebug<<endl;
//             }

         RULE AdornedQueryRule;
         vector<RULE> AdornedRules,queryRules,magicQueryRules,MagicRules,ModifiedRules,magicRulesPerAdornedRule;

         // ADORN STEP

         // rewrite the query
         if( ! rewriteQuery(groundQuery,AdornedQueryRule,magicQueryRules) )
             {
//             if( TraceLevel >= 2 )
//                 cdebug << "Not rewriting the query. Magic Sets are not applied!"
//                        << endl << endl;
        	 return;
             }

         // adorn the rules and the constraints
         while( !AdPredsAndTargs.empty() )
             {
             NAMESITEM_TARGET at(AdPredsAndTargs.back());
             AdPredsAndTargs.pop_back();

             ADORNMENT_GENERATOR ag;
             AG_TYPE type;

             // indicator whether only magic rules or also modified rules
             // should be generated.
             bool onlyForMagic = false;
             const char *name = getUnadornedName(at.first);

//             if( TraceLevel >= 2)
//                 cdebug << "*** Adorning Rules w.r.t. Predicate: " << at.first
//                        << " and target: " << at.second << endl;

             if( at.second == onlySource )
                 {
                 // if the adorning predicate name at must adorn the
                 // source of its adornment, then
                 // 1) search the source of at and adorn it,
                 // 2) generate the corresponding magic rules,
                 // 3) modify the adorned rule
                 assert( Sources.find(at.first) != Sources.end() );
                 SOURCE s((Sources.find(at.first))->second);
                 // 1) search the source of at and adorn it,
                 pair<bool,RULE> adornedSource(adornSource(at,s,onlyForMagic));

                 if( adornedSource.first )
                     {
                	 //TODO verificare mettere || ! subsumption(adornedSource.second,AdornedRules)
                     if( ! OptionOptimizedDisjunctiveMagicSets )
                         {
//                         if( TraceLevel >= 2 &&
//                                 OptionOptimizedDisjunctiveMagicSets )
//                             {
//                             cdebug << "No subsumption of rule\n   "
//                                    << adornedSource.second
//                                    << "\n w.r.t. Adorned Rules" << endl;
//                             print_list(cdebug,AdornedRules,"\n");
//                             cdebug << endl;
//                             }

                         AdornedRules.push_back(adornedSource.second);
                         }
                     // 2) generate the corresponding magic rules,
                     magicRulesPerAdornedRule=generate(adornedSource.second);
                     if( OptionOptimizedDisjunctiveMagicSets )
                         {
                         for( vector<RULE>::iterator i = magicRulesPerAdornedRule.begin();
                              i != magicRulesPerAdornedRule.end();
                              i++ )
                             {
                        	 //TODO verificare ! subsumption(*i,MagicRules)
//                             if( ! subsumption(*i,MagicRules) )
//                                 {
//                                 if( TraceLevel >= 2 )
//                                     {
//                                     cdebug << "No subsumption of rule\n   " << *i
//                                            << "\n w.r.t. Magic Rules" <<endl;
//                                     print_list(cdebug,MagicRules,"\n");
//                                     cdebug << endl;
//                                     }
                                 MagicRules.push_back(*i);
//                                 }
                             }
                         }
                     else
                         MagicRules.insert(MagicRules.end(),
                                           magicRulesPerAdornedRule.begin(),
                                           magicRulesPerAdornedRule.end());
                     // 3) modify the adorned rule
                     if( ! onlyForMagic )
                         {
                         const vector<SHAREDATOM> &head = (s.ag.iRule)->getHead();
                         RULE modifiedRule(modify(adornedSource.second,head));
                         //TODO verificare || ! subsumption(modifiedRule,ModifiedRules)
                         if( ! OptionOptimizedDisjunctiveMagicSets  )
                             {
//                             if( TraceLevel >= 2 &&
//                                     OptionOptimizedDisjunctiveMagicSets )
//                                 {
//                                 cdebug << "No subsumption of rule\n   "
//                                        << modifiedRule
//                                        << "\n w.r.t. Modified Rules" << endl;
//                                 print_list(cdebug,ModifiedRules,"\n");
//                                 cdebug << endl;
//                                 }

                             ModifiedRules.push_back(modifiedRule);
                             }
                         }
                     }
                 }
             else
                 {

                 //                for( RULES::const_iterator i=IDB.begin();
                 //                     i != IDB.end();
                 //                     i++ )
                 //                    {
                 //                    assert( &(*i).getHead() );
                 //                    DISJUNCTION &head = i->getHeadForModification();
                 //                    ag.iRule = &(*i);


                 PREDICATES_RULES_INDEX::const_iterator headrules = PredicatesRulesIndex.find(name);

 //                assert( headrules != PredicatesRulesIndex.end() );
                 if( headrules != PredicatesRulesIndex.end() )
                     {
             //            assert( headrules->second.size() == 1 );

                     for( vector<size_t>::const_iterator headrulei = headrules->second.begin();
                          headrulei != headrules->second.end();
                          headrulei++ )
                         {
                         RULE &headrule = IDB[*headrulei];

                         assert( headrule.getSizeHead()>0 );
                         const vector<SHAREDATOM> &head = headrule.getHead();
                         ag.iRule = &headrule;
                         if( headrule.getSizeHead()>1 )
                             type=typeDisjunctive;
                         else
                             type=typeNormalRule;

                         for( vector<SHAREDATOM>::const_iterator j=head.begin();
                              j != head.end();
                              j++ )
                             {
                             // if the current name atom ,*j, matches with the adorning
                             // predicate name (unadorned), then
                             // 1) build the rule and adorn it,
                             // 2) generate the corresponding magic rules,
                             // 3) modify the adorned rule.
                        	 assert((*j)->hasPredicate());
                             if( strcmp( (*j)->getPredicate()->getName().c_str(),name ) == 0 )
                                 {


                                 onlyForMagic = false;
                                 // if the target is onlySource, there is no need to check
                                 // redundancy prevention
                                 if( !generateRedundancy(ag,type,at,getLiteralPosition(*j,ag,type)) )
                                     {
                                     // 1) build the rule and adorn it
                                     RULE adornedRule(adorn(at,ag,type,*j,onlyForMagic));
                                     //TODO verificare || ! subsumption(adornedRule,AdornedRules)
                                     if( ! OptionOptimizedDisjunctiveMagicSets  )
                                         {
//                                         if( TraceLevel >= 2 &&
//                                                 OptionOptimizedDisjunctiveMagicSets )
//                                             {
//                                             cdebug << "No subsumption of rule\n   " << adornedRule
//                                                    << "\n w.r.t. Adorned Rules" << endl;
//                                             print_list(cdebug,AdornedRules,"\n");
//                                             cdebug << endl;
//                                             }

                                         AdornedRules.push_back(adornedRule);
                                         }
                                     // 2) generate the corresponding magic rules
                                     magicRulesPerAdornedRule=generate(adornedRule);
                                     if( OptionOptimizedDisjunctiveMagicSets )
                                         {
                                         for(vector<RULE>::iterator i = magicRulesPerAdornedRule.begin();
                                             i != magicRulesPerAdornedRule.end();
                                             i++)
                                        	 //TODO verificare ! subsumption(*i,MagicRules)
//                                             if( ! subsumption(*i,MagicRules) )
//                                                 {
//                                                 if( TraceLevel >= 2 )
//                                                     {
//                                                     cdebug << "No subsumption of rule\n   " << *i
//                                                            << "\n w.r.t. Magic Rules" << endl;
//                                                     print_list(cdebug,MagicRules,"\n");
//                                                     cdebug << endl;
//                                                     }
                                                 MagicRules.push_back(*i);
//                                                 }
                                         }
                                     else
                                         MagicRules.insert(MagicRules.end(),
                                                           magicRulesPerAdornedRule.begin(),
                                                           magicRulesPerAdornedRule.end());
                                     // 3) modify the adorned rule
                                     if( ! onlyForMagic )
                                         {
                                         RULE modifiedRule(modify(adornedRule,head));
                                         //TODO verificare || ! subsumption( modifiedRule,ModifiedRules)
                                         if( ! OptionOptimizedDisjunctiveMagicSets  )
                                             {
//                                             if( TraceLevel >= 2 &&
//                                                     OptionOptimizedDisjunctiveMagicSets )
//                                                 {
//                                                 cdebug << "No subsumption of rule\n   " << modifiedRule
//                                                        << "\n w.r.t. Modified Rules" << endl;
//                                                 print_list(cdebug,ModifiedRules,"\n");
//                                                 cdebug << endl;
//                                                 }
                                             ModifiedRules.push_back(modifiedRule);
                                             }
                                         }
                                     }
                                 }
                                        //                        else
                                        //                            if( TraceLevel >= 2 )
                                        //                                cdebug << " No matching between " << j->getName()
                                        //                                       << "and " << at.first << endl;
                             }
                         }
                     }
                 }
             delete [] name;
             }
//         if( TraceLevel >= 1 )
//             {
//             cdebug << endl << endl << " Adorned Query" << endl;
//             cdebug << AdornedQueryRule << endl;
//             cdebug << endl << endl << " Adorned Rules" << endl;
//             print_list(cdebug,AdornedR)a(X);b(X):-c(X).

//             cdebug << endl;
//             }
//
//         if( TraceLevel >= 1 )
//             {
//             cdebug << endl << endl << " Magic Rules" << endl;
//             print_list(cdebug,MagicRules,"\n");
//             }

         if( magicQueryRules.size() != 0 )
             {
//             if( TraceLevel >= 1 )
//                 {
//                 cdebug << endl << endl << " Magic Query Rules" << endl;
//                 print_list(cdebug,magicQueryRules,"\n");
//                 cdebug<<endl;
//                 }
             MagicRules.insert(MagicRules.end(),magicQueryRules.begin(),
                               magicQueryRules.end());
             }

//         if( TraceLevel >= 1 )
//             {
//             cdebug << endl << endl <<" Modified Rules" << endl;
//             print_list(cdebug,ModifiedRules,"\n");
//             }

         // REWRITING THE ORIGINAL RULES
         // substitute the rewritten rules for the original ones
//         for_each( IDB.begin() + firstRule, IDB.end(),[](RULE r){
//     		r->deleteBody([](SHAREDATOM atom){
//     			return 2;
//     		});
//     		if(!r->isAStrongConstraint())
//     			r->deleteHead([](SHAREDATOM atom){
//     				return 2;
//     			});
//
//     		delete r;
//         } );

         IDB.erase( IDB.begin() + firstRule, IDB.end() );

         addReorderedRules(ModifiedRules);
         IDB.insert(IDB.end(),MagicRules.begin(),MagicRules.end());

         if( !groundQuery){
             IDB.push_back(AdornedQueryRule);
         }

         // add the query rules
         IDB.insert(IDB.end(),queryRules.begin(),queryRules.end());

         // print the magic program
//         if( TraceLevel >= 1 )
//             {
//             cdebug << endl << endl <<" Rewritten program" << endl;
//             print_list(cdebug,IDB,"\n");
//             cdebug << endl <<"( Totally: "<< IDB.size() << " rewritten rules )\n";
//             cdebug << endl <<endl;
//             }

         // stripp off adornments from the rewritten program in order to
         // preserve the correct semantics


         ListOfAdornedPredicates list = getPredicates();
         const vector<RULE> &UnadornedRules = stripOffAdornments(list,IDB);


         IDB.erase( IDB.begin() + firstRule, IDB.end() );
         IDB.insert( IDB.end(), UnadornedRules.begin(), UnadornedRules.end() );

         // print the magic unadorned program
//         if( TraceLevel >= 1 )
//             {
//             cdebug << endl << endl <<" Rewritten and **** Unadorned **** program" << endl;
//             print_list(cdebug,IDB,"\n");
//             cdebug << endl << endl << endl;
//             }

         // check subsumption
         if( OptionSubsumptionCheckingAfterDMS ||
                 OptionOptimizedDisjunctiveMagicSets )
             {
//             if( TraceLevel >= 1 )
//                 {
//                 cdebug << endl << endl
//                        << " Subsumption checking of rewritten"
//                        << " and unadorned IDB rules will be performed" << endl;
//                 }

        	 //TODO verificare possiamo evitarla
//             SubsumptionCheckingRewriting(IDB, firstRule);
             }

         for(auto r:IDB_OLD){
        	 r->free();
        	 delete r;
         }
         IDB_OLD.clear();

         for(auto& rule:IDB){
        	 Rule *r=new Rule;
        	 for(auto headAtom:rule.getHead())
        		 r->addInHead(headAtom->clone());
        	 for(auto bodyAtom:rule.getBody())
        		 r->addInBody(bodyAtom->clone());
        	 IDB_OLD.push_back(r);

         }

         }



//////////////////////////////////////////////////////////////

// Local Variables:
// c-file-style: "dl"
// End:

};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_GROUND_REWRITEMAGIC_H_ */
