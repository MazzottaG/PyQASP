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
 * File:   InputBuilder.h
 * Author: cesco
 *
 * Created on 6 marzo 2014, 23.03
 */

#ifndef INPUTBUILDER_H
#define	INPUTBUILDER_H

#include <iostream>
using namespace std;

namespace DLV2
{

    class InputBuilder {
    public:
        InputBuilder() { }
        virtual ~InputBuilder() { }

        virtual void onDirective( char* directiveName, char* directiveValue ) = 0;
        virtual void onRule() = 0;
        virtual void onConstraint() = 0;
        virtual void onWeakConstraint() = 0;
        virtual void onQuery() = 0;
        virtual void onHeadAtom() = 0;
        virtual void onHead() = 0;
        virtual void onBodyLiteral() = 0;
        virtual void onBody() = 0;
        virtual void onNafLiteral( bool naf = false ) = 0;
        virtual void onAtom( bool isStrongNeg = false ) = 0;
        virtual void onExistentialAtom() = 0;
        virtual void onPredicateName( char* name ) = 0;
        virtual void onExistentialVariable( char* var ) = 0;
        virtual void onEqualOperator() = 0;
        virtual void onUnequalOperator() = 0;
        virtual void onLessOperator() = 0;
        virtual void onLessOrEqualOperator() = 0;
        virtual void onGreaterOperator() = 0;
        virtual void onGreaterOrEqualOperator() = 0;
        virtual void onTerm( char* value ) = 0;
        virtual void onTerm( int value ) = 0;
        virtual void onUnknownVariable() = 0;
        virtual void onFunction( char* functionSymbol, int nTerms ) = 0;
        virtual void onHeadTailList()=0;
        virtual void onListTerm(int nTerms)=0;
            
        virtual void onTermDash() = 0;
        virtual void onTermParams() = 0;
        virtual void onTermRange( char* lowerBound, char* upperBound ) = 0;
        virtual void onArithmeticOperation( char arithOperator ) = 0;
        virtual void onWeightAtLevels( int nWeight, int nLevel, int nTerm ) = 0;
        virtual void onChoiceLowerGuard() = 0;
        virtual void onChoiceUpperGuard() = 0;
        virtual void onChoiceElementAtom() = 0;
        virtual void onChoiceElementLiteral() = 0;
        virtual void onChoiceElement() = 0;
        virtual void onChoiceAtom() = 0;
        virtual void onBuiltinAtom() = 0;
        virtual void onAggregateLowerGuard() = 0;
        virtual void onAggregateUpperGuard() = 0;
        virtual void onAggregateFunction( char* functionSymbol ) = 0;
        virtual void onAggregateGroundTerm( char* value, bool dash = false ) = 0;
        virtual void onAggregateVariableTerm( char* value ) = 0;
        virtual void onAggregateFunctionalTerm( char* value, int nTerms ) = 0;
        virtual void onAggregateUnknownVariable() = 0;
        virtual void onAggregateNafLiteral() = 0;
        virtual void onAggregateElement() = 0;
        virtual void onAggregate( bool naf = false ) = 0;

        virtual void onAnnotationPartialOrdering(bool global=false) = 0;
        virtual void onAnnotationRuleOrdering(char* annotation) = 0;
        virtual void onAnnotationRuleProjection(char* annotation) = 0;
        virtual void onAnnotationRuleRewArith() = 0;
        virtual void onAnnotationRuleLookAhead() = 0;
        virtual void onAnnotationRuleAlignSubstitutions() = 0;
        virtual void onAnnotationRuleAtomIndexedArgument(char* annotation) = 0;
        virtual void onAnnotationRuleAtomIndexedLiteral(bool naf = false) = 0;
        virtual void onAnnotationRulePartialOrderingBefore(bool naf = false) = 0;
        virtual void onAnnotationRulePartialOrderingAfter(bool naf = false) = 0;
        virtual void onAnnotationAggregateRulePartialOrderingAfter(bool naf = false) = 0;
        virtual void onAnnotationAggregateRulePartialOrderingBefore(bool naf = false) = 0;
        virtual void onAnnotationGlobalOrdering(char* annotation) = 0;
        virtual void onAnnotationGlobalAtomIndexedArgument(char* annotation) = 0;
        virtual void onAnnotationGlobalAtomIndexedLiteral(bool naf = false) = 0;
        virtual void onAnnotationGlobalPartialOrderingAfter(bool naf = false) = 0;
        virtual void onAnnotationGlobalPartialOrderingBefore(bool naf = false) = 0;
        virtual void onAnnotationAggregateGlobalPartialOrderingAfter(bool naf = false) = 0;
        virtual void onAnnotationAggregateGlobalPartialOrderingBefore(bool naf = false) = 0;
        virtual void onAnnotationGlobalExternalPredicateName(char* name)= 0;
        virtual void onAnnotationGlobalExternalType(char* type) = 0;
        virtual void onAnnotationGlobalExternalTypeQC() = 0;
        virtual void onAnnotationGlobalExternalTypeC()=0;
        virtual void onAnnotationGlobalExternalTypeU()=0;
        virtual void onAnnotationGlobalExternalTypeUT()=0;
        virtual void onAnnotationGlobalExternalTypeUR()=0;
        virtual void onAnnotationGlobalExternalTypeT()=0;
        virtual void onAnnotationGlobalExternalTypeR()=0;
        virtual void onAnnotationGlobalExternal()=0;

        virtual void onAnnotationGlobalWaspAggregateElement()=0;
        virtual void onAnnotationGlobalWaspLiteral()=0;
        virtual void onAnnotationGlobalWaspPropagatorBegin()=0;
        virtual void onAnnotationGlobalWaspPropagatorEnd()=0;
        virtual void onAnnotationGlobalWaspHeuristicsBegin()=0;
        virtual void onAnnotationGlobalWaspHeuristicsEnd()=0;
        virtual void onAnnotationGlobalWaspPyFile(char* name)=0;

        virtual void onEnd()=0;

        virtual void onAnnotationRuleToDecompose()=0;
        virtual void onAnnotationRuleToNotDecompose()=0;

		/*************** useful for ExtAtom ******************/
		virtual void onExtAtom(bool naf = false){};
		virtual void onSemicolon(){};
		virtual void onExternalPredicateName(char* name){};
		/***************************************************/

    };
    
};

#endif	/* INPUTBUILDER_H */

