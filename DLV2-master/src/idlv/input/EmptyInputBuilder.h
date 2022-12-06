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
 * File:   EmptyInputBuilder.h
 * Author: cesco
 *
 * Created on 23 maggio 2014, 17.54
 */

#ifndef EMPTYINPUTBUILDER_H
#define	EMPTYINPUTBUILDER_H

namespace DLV2
{
 
class EmptyInputBuilder: public InputBuilder {
	public:
		EmptyInputBuilder() { }
		virtual ~EmptyInputBuilder() { }

		virtual void onDirective( char* directiveName, char* directiveValue ) { }
		virtual void onRule() { }
		virtual void onConstraint() { }
		virtual void onWeakConstraint() { }
		virtual void onQuery() { }
		virtual void onHeadAtom() { }
		virtual void onHead() { }
		virtual void onBodyLiteral() { }
		virtual void onBody() { }
		virtual void onNafLiteral( bool naf = false ) { }
		virtual void onAtom( bool isStrongNeg = false ) { }
		virtual void onExistentialAtom() { }
		virtual void onEqualOperator() { }
		virtual void onUnequalOperator() { }
		virtual void onLessOperator() { }
		virtual void onLessOrEqualOperator() { }
		virtual void onGreaterOperator() { }
		virtual void onGreaterOrEqualOperator() { }
		virtual void onPredicateName( char* name ) { }
		virtual void onExistentialVariable( char* var ) { }
		virtual void onTerm( char* value ) { }
		virtual void onTerm( int value ) { }
		virtual void onUnknownVariable() { }
		virtual void onFunction( char* functionSymbol, int nTerms ) { }
		virtual void onTermDash() { }
		virtual void onTermParams() { }
		virtual void onTermRange( char* lowerBound, char* upperBound ) { }
		virtual void onArithmeticOperation( char arithOperator ) { }
		virtual void onWeightAtLevels( int nWeight, int nLevel, int nTerm ) { }
		virtual void onChoiceLowerGuard() { }
		virtual void onChoiceUpperGuard() { }
		virtual void onChoiceElementAtom() { }
		virtual void onChoiceElementLiteral() { }
		virtual void onChoiceElement() { }
		virtual void onChoiceAtom() { }
		virtual void onBuiltinAtom() { }
		virtual void onAggregateLowerGuard() { }
		virtual void onAggregateUpperGuard() { }
		virtual void onAggregateFunction( char* functionSymbol ) { }
		virtual void onAggregateGroundTerm( char* value, bool dash = false ) { }
		virtual void onAggregateVariableTerm( char* value ) { }
		virtual void onAggregateFunctionalTerm( char* value, int nTerms ) {};
		virtual void onAggregateUnknownVariable() { }
		virtual void onAggregateNafLiteral() { }
		virtual void onAggregateElement() { }
		virtual void onAggregate( bool naf = false ) { }

		//Grounder Annotations
		virtual void onAnnotationPartialOrdering(bool global=false){};
		virtual void onAnnotationRuleOrdering(char* annotation){};
		virtual void onAnnotationRuleAtomIndexedArgument(char* annotation){};
		virtual void onAnnotationRuleAtomIndexedLiteral(bool naf = false){};
		virtual void onAnnotationRulePartialOrderingBefore(bool naf = false){};
		virtual void onAnnotationRulePartialOrderingAfter(bool naf = false){};
		virtual void onAnnotationAggregateRulePartialOrderingAfter(bool naf = false){};
		virtual void onAnnotationAggregateRulePartialOrderingBefore(bool naf = false){};
		virtual void onAnnotationGlobalOrdering(char* annotation){};
		virtual void onAnnotationGlobalAtomIndexedArgument(char* annotation){};
		virtual void onAnnotationGlobalAtomIndexedLiteral(bool naf = false){};
		virtual void onAnnotationGlobalPartialOrderingAfter(bool naf = false){};
		virtual void onAnnotationGlobalPartialOrderingBefore(bool naf = false){};
		virtual void onAnnotationAggregateGlobalPartialOrderingAfter(bool naf = false){};
		virtual void onAnnotationAggregateGlobalPartialOrderingBefore(bool naf = false){};
		virtual void onAnnotationGlobalExternalPredicateName(char* name){};
		virtual void onAnnotationGlobalExternalType(char* type){};
		virtual void onAnnotationGlobalExternalTypeQC(){};
		virtual void onAnnotationGlobalExternalTypeC(){};
		virtual void onAnnotationGlobalExternalTypeU(){};
		virtual void onAnnotationGlobalExternalTypeUT(){};
		virtual void onAnnotationGlobalExternalTypeUR(){};
		virtual void onAnnotationGlobalExternalTypeT(){};
		virtual void onAnnotationGlobalExternalTypeR(){};
		virtual void onAnnotationRuleProjection(char* annotation){};
		virtual void onAnnotationRuleRewArith(){};
		virtual void onAnnotationRuleLookAhead(){};
		virtual void onAnnotationRuleAlignSubstitutions(){};
		virtual void onAnnotationGlobalExternal(){};
		virtual void onAnnotationRuleToDecompose(){};
		virtual void onAnnotationRuleToNotDecompose(){};


		//Solver Annotations
		virtual void onAnnotationGlobalWaspAggregateElement(){};
		virtual void onAnnotationGlobalWaspLiteral(){};
		virtual void onAnnotationGlobalWaspPropagatorBegin(){};
		virtual void onAnnotationGlobalWaspPropagatorEnd(){};
		virtual void onAnnotationGlobalWaspHeuristicsBegin(){};
		virtual void onAnnotationGlobalWaspHeuristicsEnd(){};
		virtual void onAnnotationGlobalWaspPyFile(char* name){};


        	virtual void onEnd(){};

 		virtual void onHeadTailList(){};
        	virtual void onListTerm(int nTerms){};
	};

};

#endif	/* EMPTYINPUTBUILDER_H */

