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
 * File:   SelectorBuilder.h
 * Author: cesco
 *
 * Created on 7 marzo 2014, 10.53
 */

#ifndef SELECTORBUILDER_H
#define	SELECTORBUILDER_H

#include "../input/InputBuilder.h"
#include <vector>
#include <unordered_map>
#include <string>

namespace DLV2
{

    class SelectorBuilder : public InputBuilder {
    public:
        SelectorBuilder();
        virtual ~SelectorBuilder();

        virtual void onDirective( char*, char* ) { }
        virtual void onRule();
        virtual void onConstraint();
        virtual void onWeakConstraint();
        virtual void onQuery();
        virtual void onHeadAtom();    
        virtual void onHead() { }
        virtual void onBodyLiteral();
        virtual void onBody() { }
        virtual void onNafLiteral( bool naf = false );
        virtual void onAtom( bool isStrongNeg = false );
        virtual void onExistentialAtom();
        virtual void onPredicateName( char* );
        virtual void onExistentialVariable( char* );
        virtual void onEqualOperator();
        virtual void onUnequalOperator();
        virtual void onLessOperator();
        virtual void onLessOrEqualOperator();
        virtual void onGreaterOperator();
        virtual void onGreaterOrEqualOperator();
        virtual void onTerm( char* );
        virtual void onTerm( int );
        virtual void onUnknownVariable();
        virtual void onFunction( char*, int );
        virtual void onTermDash();
        virtual void onTermParams();
        virtual void onTermRange( char*, char* ) { }
        virtual void onArithmeticOperation( char );
        virtual void onWeightAtLevels( int, int, int );
        virtual void onChoiceLowerGuard();
        virtual void onChoiceUpperGuard();
        virtual void onChoiceElementAtom();
        virtual void onChoiceElementLiteral();
        virtual void onChoiceElement();
        virtual void onChoiceAtom();
        virtual void onBuiltinAtom();
        virtual void onAggregateLowerGuard();
        virtual void onAggregateUpperGuard();
        virtual void onAggregateFunction( char* );
        virtual void onAggregateGroundTerm( char*, bool dash = false );
        virtual void onAggregateVariableTerm( char* );
        virtual void onAggregateUnknownVariable();
        virtual void onAggregateNafLiteral();
        virtual void onAggregateElement();
        virtual void onAggregate( bool naf = false );

        unsigned getSolverToCall();

    private:
        bool foundQuery;
        bool foundChoice;

        std::string predName;
        //bool inBody;
        std::vector<std::string> head;
        struct Literal { std::string predname; bool sign; };
        std::vector<Literal> body;
        void cleanTemp();


        std::unordered_map<std::string,unsigned> pred2nodeId;
        unsigned addPredicateInHash(std::string predname);

        std::vector<std::vector<unsigned>> heads;
        bool reaches(unsigned, std::vector<unsigned>);

        //unordered_map<unsigned,vector<vector<unsigned>>> headsOf;

        std::unordered_map<unsigned,std::vector<unsigned>> grafo;
        void addArc(unsigned from, unsigned to);

        bool HCF();


    };

};

#endif	/* SELECTORBUILDER_H */

