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
 * AggregateAtom.cpp
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#include "AggregateAtom.h"
#include "../../../common/Assert.h"
#include "../table/PredicateExtension.h"
#include "../term/ConstantTerm.h"
#include "../term/ArithTerm.h"
#include <algorithm>

using namespace std;

namespace DLV2{
namespace grounder{

void AggregateAtom::getVariablesInAggregationTerms(set_term& vars) const {
	for(auto element:aggregateElements)
		element->getVariableAggregationTerms(vars);
}

set_term AggregateAtom::getVariable(bool guard) const{
	set_term variables;
	if(guard)
		variables=getGuardVariable();
	for(auto element:aggregateElements)
		element->getVariableInNaf(variables);
	return variables;
}

void AggregateAtom::getVariable(set_term& variables,bool guard) const {
	if(guard)
		variables=getGuardVariable();
	for(auto element:aggregateElements)
		element->getVariableInNaf(variables);
}

set_predicate AggregateAtom::getPredicates() const{
	set_predicate predicates;
	for(auto element:aggregateElements)
		element->getPredicates(predicates);
	return predicates;
}

set_term AggregateAtom::getGuardVariable() const{
	set_term variables;
	//if(firstBinop!= NONE_OP && firstGuard->getType()==TermType::VARIABLE)
	//	variables.insert(firstGuard);
//	if(secondBinop != NONE_OP && secondGuard->getType()==TermType::VARIABLE)
//		variables.insert(secondGuard);
	if(firstBinop!= NONE_OP)
		firstGuard->getVariable(variables);
	if(secondBinop!= NONE_OP)
		secondGuard->getVariable(variables);
	return variables;
}

size_t AggregateAtom::hash() {
	return 0; //TODO
}

bool AggregateAtom::operator ==(const Atom& a) const {
	if(!a.isAggregateAtom())return false;
	if(aggregateFunction!=a.getAggregateFunction())return false;
	if(firstBinop!=a.getFirstBinop())return false;
	if(secondBinop!=a.getSecondBinop())return false;
	if(firstBinop!=NONE_OP && firstGuard->getIndex()!=a.getFirstGuard()->getIndex())return false;
	if(secondBinop!=NONE_OP && secondGuard->getIndex()!=a.getSecondGuard()->getIndex())return false;
	if(aggregateElements.size()!=a.getAggregateElementsSize())return false;
	for(unsigned i=0;i<aggregateElements.size();i++)
		if(!(*aggregateElements[i]==*a.getAggregateElement(i)))
			return false;

	return true;

}


void AggregateAtom::ground(var_assignment& substritutionTerm, Atom*& templateAtom) {
	//Ground guard
	Term* template_lowerGuard=nullptr,*template_upperGuard=nullptr;
	if(firstBinop!=NONE_OP){
		template_lowerGuard=firstGuard->substitute(substritutionTerm);
		if(template_lowerGuard->isGround() && firstGuard->contain(TermType::ARITH))
			template_lowerGuard=template_lowerGuard->calculate();
	}
	if(secondBinop!=NONE_OP){
		template_upperGuard=secondGuard->substitute(substritutionTerm);
		if(template_upperGuard->isGround() && secondGuard->contain(TermType::ARITH))
			template_upperGuard=template_upperGuard->calculate();
	}
	assert_msg(((firstBinop==NONE_OP || firstBinop==EQUAL ||template_lowerGuard->isGround()  )  && (secondBinop==NONE_OP || secondBinop==EQUAL || template_upperGuard->isGround())),"Arith term not safe");
	if(templateAtom==nullptr){
		//Ground Aggregate Element
		vector<AggregateElement*> template_aggregate_element;
		for(auto& agg_element:aggregateElements){
			AggregateElement *newElement=new AggregateElement;
			//Set all terms of aggregate in the new aggregate
			newElement->setTerms(agg_element->getTerms());
			//For each aggregate element ground the naf of the aggregate element and push
			//in the new aggregate element
			for(auto atom_agg_element:agg_element->getNafLiterals()){
				Atom *groundAtom=nullptr;
				atom_agg_element->ground(substritutionTerm,groundAtom);
				newElement->addNafLiterals(groundAtom);
			}
			template_aggregate_element.push_back(newElement);
		}
		templateAtom=new AggregateAtom(template_lowerGuard,firstBinop,template_upperGuard,secondBinop,aggregateFunction,template_aggregate_element,negative);

	}else{
		templateAtom->setFirstGuard(template_lowerGuard);
		templateAtom->setSecondGuard(template_upperGuard);
		//Ground Aggregate Element
		for(unsigned i=0;i<templateAtom->getAggregateElementsSize();i++){
			//Set all terms of aggregate in the new aggregate
			templateAtom->getAggregateElement(i)->setTerms(aggregateElements[i]->getTerms());
			//clear all the naf literal
			templateAtom->getAggregateElement(i)->clearNafLiterals();
			//For each aggregate element ground the naf of the aggregate element and push
			//in the new aggregate element
			for(auto atom_agg_element:aggregateElements[i]->getNafLiterals()){
				Atom *groundAtom=nullptr;
				atom_agg_element->ground(substritutionTerm,groundAtom);
				templateAtom->getAggregateElement(i)->addNafLiterals(groundAtom);
			}
		}
	}
}

ResultEvaluation AggregateAtom::partialEvaluate() {
	if(invalideGuards())return UNSATISFY;
	switch (aggregateFunction) {
		case COUNT:
			return returnEvaluation(partialEvaluateCount());
		case MIN:
			return returnEvaluation(partialEvaluateMin());
		case MAX:
			return returnEvaluation(partialEvaluateMax());
		case SUM:
			return returnEvaluation(partialEvaluateSum());
		default:
			return UNDEF;
	}
}

ResultEvaluation AggregateAtom::finalEvaluate() {
	switch (aggregateFunction) {
		case COUNT:
			return returnEvaluation(finalEvaluateCount());
		case MIN:
			return returnEvaluation(finalEvaluateMin());
		case MAX:
			return returnEvaluation(finalEvaluateMax());
		case SUM:
			return returnEvaluation(finalEvaluateSum());
		default:
			return UNDEF;
	}
}

ResultEvaluation AggregateAtom::returnEvaluation(ResultEvaluation evaluation){
	if(!negative)return evaluation;
	if(evaluation==SATISFY)return UNSATISFY;
	if(evaluation==UNSATISFY)return SATISFY;
	return evaluation;
}

ResultEvaluation AggregateAtom::partialEvaluateCount() {

	Atom *lastAtom=aggregateElements.back()->getNafLiteral(0);
	if(!lastAtom->isFact()){
		undefAtomEvaluation=undefAtomEvaluation->increment();
		return UNDEF;
	}

	partialEvaluation=partialEvaluation->increment();

	if(checkOperator(firstGuard,firstBinop,EQUAL,GREATER,false))
		return UNSATISFY;

	if(checkOperator(secondGuard,secondBinop,LESS,GREATER_OR_EQ,false))
		return UNSATISFY;
	if(checkOperator(secondGuard,secondBinop,LESS_OR_EQ,GREATER,false))
		return UNSATISFY;

	if(secondBinop==NONE_OP && checkOperator(firstGuard,firstBinop,LESS_OR_EQ,GREATER_OR_EQ,false))
		return SATISFY;
	if(secondBinop==NONE_OP && checkOperator(firstGuard,firstBinop,LESS,GREATER,false))
		return SATISFY;

	return UNDEF;
}

ResultEvaluation AggregateAtom::partialEvaluateMax() {
	Atom *lastAtom=aggregateElements.back()->getNafLiteral(0);
	if(!lastAtom->isFact()){
		Term* value=aggregateElements.back()->getTerms().front();
		if(undefAtomEvaluation->getIndex()==TermTable::getInstance()->term_min->getIndex() || *undefAtomEvaluation<*value)
			undefAtomEvaluation=value;
		return UNDEF;
	}
	Term* value=aggregateElements.back()->getTerms().front();
	if(partialEvaluation->getIndex()==TermTable::getInstance()->term_min->getIndex() || *partialEvaluation<*value)
		partialEvaluation=value;

	if(checkOperator(firstGuard,firstBinop,EQUAL,GREATER,false))
		return UNSATISFY;

	if(checkOperator(secondGuard,secondBinop,LESS,GREATER_OR_EQ,false))
		return UNSATISFY;
	if(checkOperator(secondGuard,secondBinop,LESS_OR_EQ,GREATER,false))
		return UNSATISFY;

	if(secondBinop==NONE_OP && checkOperator(firstGuard,firstBinop,LESS_OR_EQ,GREATER_OR_EQ,false))
		return SATISFY;
	if(secondBinop==NONE_OP && checkOperator(firstGuard,firstBinop,LESS,GREATER,false))
		return SATISFY;

	return UNDEF;

}

ResultEvaluation AggregateAtom::partialEvaluateMin() {
	Atom *lastAtom=aggregateElements.back()->getNafLiteral(0);
	if(!lastAtom->isFact()){
		Term* value=aggregateElements.back()->getTerms().front();
		if(undefAtomEvaluation->getIndex()==TermTable::getInstance()->term_max->getIndex() || *undefAtomEvaluation>*value)
			undefAtomEvaluation=value;
		return UNDEF;
	}
	Term* value=aggregateElements.back()->getTerms().front();

	if(partialEvaluation->getIndex()==TermTable::getInstance()->term_max->getIndex() || *partialEvaluation>*value )
		partialEvaluation=value;

	if(checkOperator(firstGuard,firstBinop,EQUAL,LESS,false))
		return UNSATISFY;

	if(checkOperator(firstGuard,firstBinop,LESS_OR_EQ,LESS,false))
		return UNSATISFY;
	if(checkOperator(firstGuard,firstBinop,LESS,LESS_OR_EQ,false))
		return UNSATISFY;

	if(firstBinop==NONE_OP && checkOperator(secondGuard,secondBinop,LESS,LESS,false))
		return SATISFY;
	if(firstBinop==NONE_OP && checkOperator(secondGuard,secondBinop,LESS_OR_EQ,LESS_OR_EQ,false))
		return SATISFY;

	return UNDEF;
}

ResultEvaluation AggregateAtom::partialEvaluateSum() {
	Atom *lastAtom=aggregateElements.back()->getNafLiteral(0);
	Term* term=aggregateElements.back()->getTerms().front();
	bool isNumeric=term->getType()==NUMERIC_CONSTANT;

	if(isNumeric){

		bool positive=(term->getConstantValue())>=0;
		if(!positive)
			sumCheckInfo.negativeSumValue+=term->getConstantValue();

		if(!lastAtom->isFact()){
			if(sumCheckInfo.elemType!=IDLV_MIXED){
				ElementType& elemType=sumCheckInfo.elemType;
				if(elemType==IDLV_EMPTY)
					elemType=(positive)?IDLV_POSITIVE:IDLV_NEGATIVE;
				else if(elemType==IDLV_POSITIVE && !positive)
					elemType=IDLV_MIXED;
				else if(elemType==IDLV_NEGATIVE && positive)
					elemType=IDLV_MIXED;
			}


			undefAtomEvaluation=undefAtomEvaluation->sum(term);
			sumCheckInfo.findUndefInSum=true;
			return UNDEF;
		}

		partialEvaluation=partialEvaluation->sum(term);
	}


	if(PredicateExtTable::getInstance()->getPredicateExt(lastAtom->getPredicate())->getPredicateInformation()->isOnlyPositive()){
		if(checkOperator(firstGuard,firstBinop,EQUAL,GREATER,false))
			return UNSATISFY;

		if(secondBinop==NONE_OP && checkOperator(firstGuard,firstBinop,LESS_OR_EQ,GREATER,false))
			return SATISFY;
		if(secondBinop==NONE_OP && checkOperator(firstGuard,firstBinop,LESS,GREATER,false))
			return SATISFY;

		if(checkOperator(secondGuard,secondBinop,LESS,GREATER_OR_EQ,false))
			return UNSATISFY;
		if(checkOperator(secondGuard,secondBinop,LESS_OR_EQ,GREATER,false))
			return UNSATISFY;
	}

	if(PredicateExtTable::getInstance()->getPredicateExt(lastAtom->getPredicate())->getPredicateInformation()->isOnlyNegative()){
		if(checkOperator(firstGuard,firstBinop,EQUAL,LESS,false))
			return UNSATISFY;

		if(checkOperator(firstGuard,firstBinop,LESS_OR_EQ,LESS,false))
			return UNSATISFY;
		if(checkOperator(firstGuard,firstBinop,LESS,LESS_OR_EQ,false))
			return UNSATISFY;

		if(firstBinop==NONE_OP && checkOperator(secondGuard,secondBinop,LESS,LESS,false))
			return SATISFY;
		if(firstBinop==NONE_OP && checkOperator(secondGuard,secondBinop,LESS_OR_EQ,LESS_OR_EQ,false))
			return SATISFY;
	}

	return UNDEF;
}

ResultEvaluation AggregateAtom::finalEvaluateCount() {
	if(aggregateElements.empty() && undefAtomEvaluation->getIndex()==TermTable::getInstance()->term_zero->getIndex()){
		if(firstBinop==NONE_OP && checkOperator(secondGuard,secondBinop,LESS,EQUAL,true))
			return UNSATISFY;
		if(secondBinop==NONE_OP && checkOperator(firstGuard,firstBinop,LESS,EQUAL,true))
			return UNSATISFY;
	}

	if(checkOperator(firstGuard,firstBinop,EQUAL,LESS,true))
		return UNSATISFY;

	if(undefAtomEvaluation->getIndex()==TermTable::getInstance()->term_zero->getIndex() && checkOperator(firstGuard,firstBinop,EQUAL,EQUAL,false))
		return SATISFY;

	if(checkOperator(firstGuard,firstBinop,LESS_OR_EQ,LESS,true))
		return UNSATISFY;
	if(checkOperator(firstGuard,firstBinop,LESS,LESS_OR_EQ,true))
		return UNSATISFY;

	if(firstBinop==NONE_OP && checkOperator(secondGuard,secondBinop,LESS,LESS,true))
		return SATISFY;
	if(firstBinop==NONE_OP && checkOperator(secondGuard,secondBinop,LESS_OR_EQ,LESS_OR_EQ,true))
		return SATISFY;

	if(checkOperator(secondGuard,secondBinop,LESS,LESS,true) && checkOperator(firstGuard,firstBinop,LESS_OR_EQ,GREATER_OR_EQ,false))
		return SATISFY;
	if(checkOperator(secondGuard,secondBinop,LESS_OR_EQ,LESS_OR_EQ,true) && checkOperator(firstGuard,firstBinop,LESS_OR_EQ,GREATER_OR_EQ,false))
		return SATISFY;
	if(checkOperator(secondGuard,secondBinop,LESS,LESS,true) && checkOperator(firstGuard,firstBinop,LESS,GREATER,false))
		return SATISFY;
	if(checkOperator(secondGuard,secondBinop,LESS_OR_EQ,LESS_OR_EQ,true) && checkOperator(firstGuard,firstBinop,LESS,GREATER,false))
		return SATISFY;

	findUndefAtoms();
	return UNDEF;
}

ResultEvaluation AggregateAtom::finalEvaluateMax() {
	if(checkOperator(firstGuard,firstBinop,EQUAL,LESS,true))
		return UNSATISFY;

	if(undefAtomEvaluation->getIndex()==TermTable::getInstance()->term_min->getIndex()){
		if(checkOperator(firstGuard,firstBinop,EQUAL,EQUAL,false))
			return SATISFY;
		if(checkOperator(firstGuard,firstBinop,EQUAL,LESS,false))
			return UNSATISFY;
	}

	if(checkOperator(firstGuard,firstBinop,LESS_OR_EQ,LESS,true))
		return UNSATISFY;
	if(checkOperator(firstGuard,firstBinop,LESS,LESS_OR_EQ,true))
		return UNSATISFY;

	if(firstBinop==NONE_OP && checkOperator(secondGuard,secondBinop,LESS,LESS,true))
		return SATISFY;
	if(firstBinop==NONE_OP && checkOperator(secondGuard,secondBinop,LESS_OR_EQ,LESS_OR_EQ,true))
		return SATISFY;

	if(checkOperator(secondGuard,secondBinop,LESS,LESS,true) && checkOperator(firstGuard,firstBinop,LESS_OR_EQ,GREATER_OR_EQ,false))
		return SATISFY;
	if(checkOperator(secondGuard,secondBinop,LESS_OR_EQ,LESS_OR_EQ,true) && checkOperator(firstGuard,firstBinop,LESS_OR_EQ,GREATER_OR_EQ,false))
		return SATISFY;
	if(checkOperator(secondGuard,secondBinop,LESS,LESS,true) && checkOperator(firstGuard,firstBinop,LESS,GREATER,false))
		return SATISFY;
	if(checkOperator(secondGuard,secondBinop,LESS_OR_EQ,LESS_OR_EQ,true) && checkOperator(firstGuard,firstBinop,LESS,GREATER,false))
		return SATISFY;

	if(isAnAssigment()) findUndefAtoms();
	return UNDEF;
}

ResultEvaluation AggregateAtom::finalEvaluateMin() {
//	if(aggregateElements.empty()){
//		if(checkOperator(firstGuard,firstBinop,LESS_OR_EQ,GREATER_OR_EQ,false) && secondBinop==NONE_OP)
//			return SATISFY;
//		if(checkOperator(firstGuard,firstBinop,EQUAL,EQUAL,false) && secondBinop==NONE_OP)
//			return SATISFY;
//		if(checkOperator(firstGuard,firstBinop,LESS,GREATER,false) && secondBinop==NONE_OP)
//			return SATISFY;
//		return UNSATISFY;
//	}

	if(undefAtomEvaluation->getIndex() == TermTable::getInstance()->term_max->getIndex()){
		if(checkOperator(firstGuard,firstBinop,EQUAL,EQUAL,false))
			return SATISFY;
		if(checkOperator(firstGuard,firstBinop,EQUAL,GREATER,false))
			return UNSATISFY;
	}

	if(secondBinop==NONE_OP && checkOperator(firstGuard,firstBinop,LESS_OR_EQ,GREATER_OR_EQ,true))
		return SATISFY;
	if(secondBinop==NONE_OP && checkOperator(firstGuard,firstBinop,LESS,GREATER,true))
		return SATISFY;

	if(checkOperator(secondGuard,secondBinop,LESS,GREATER_OR_EQ,true))
		return UNSATISFY;
	if(checkOperator(secondGuard,secondBinop,LESS_OR_EQ,GREATER,true))
		return UNSATISFY;

	if(checkOperator(secondGuard,secondBinop,LESS,LESS,false) && checkOperator(firstGuard,firstBinop,LESS_OR_EQ,GREATER_OR_EQ,true))
		return SATISFY;
	if(checkOperator(secondGuard,secondBinop,LESS_OR_EQ,LESS_OR_EQ,false) && checkOperator(firstGuard,firstBinop,LESS_OR_EQ,GREATER_OR_EQ,true))
		return SATISFY;
	if(checkOperator(secondGuard,secondBinop,LESS,LESS,false) && checkOperator(firstGuard,firstBinop,LESS,GREATER,true))
		return SATISFY;
	if(checkOperator(secondGuard,secondBinop,LESS_OR_EQ,LESS_OR_EQ,false) && checkOperator(firstGuard,firstBinop,LESS,GREATER,true))
		return SATISFY;

	if(isAnAssigment()) findUndefAtoms();
	return UNDEF;
}

ResultEvaluation AggregateAtom::finalEvaluateSum() {

	//If the aggregate is defined or contain only positive atoms
	//If the aggregate is defined we can check the value also if the aggregate contain negative number because we know the
	//exact value.
	if(!sumCheckInfo.findUndefInSum || sumCheckInfo.elemType==IDLV_POSITIVE){

		if(checkOperator(firstGuard,firstBinop,EQUAL,LESS,true) || checkOperator(firstGuard,firstBinop,EQUAL,GREATER,false))
			return UNSATISFY;

		if(!sumCheckInfo.findUndefInSum && checkOperator(firstGuard,firstBinop,EQUAL,EQUAL,false))
			return SATISFY;

		if(checkOperator(firstGuard,firstBinop,LESS_OR_EQ,LESS,true))
			return UNSATISFY;
		if(checkOperator(firstGuard,firstBinop,LESS,LESS_OR_EQ,true))
			return UNSATISFY;

		if(checkOperator(secondGuard,secondBinop,LESS,GREATER_OR_EQ,false))
			return UNSATISFY;
		if(checkOperator(secondGuard,secondBinop,LESS_OR_EQ,GREATER,false))
			return UNSATISFY;

		if(secondBinop==NONE_OP && checkOperator(firstGuard,firstBinop,LESS_OR_EQ,GREATER_OR_EQ,false))
			return SATISFY;
		if(secondBinop==NONE_OP && checkOperator(firstGuard,firstBinop,LESS,GREATER,false))
			return SATISFY;

		if(firstBinop==NONE_OP && checkOperator(secondGuard,secondBinop,LESS,LESS,true))
			return SATISFY;
		if(firstBinop==NONE_OP && checkOperator(secondGuard,secondBinop,LESS_OR_EQ,LESS_OR_EQ,true))
			return SATISFY;

		if(checkOperator(secondGuard,secondBinop,LESS,LESS,true) && checkOperator(firstGuard,firstBinop,LESS_OR_EQ,GREATER_OR_EQ,false))
			return SATISFY;
		if(checkOperator(secondGuard,secondBinop,LESS_OR_EQ,LESS_OR_EQ,true) && checkOperator(firstGuard,firstBinop,LESS_OR_EQ,GREATER_OR_EQ,false))
			return SATISFY;
		if(checkOperator(secondGuard,secondBinop,LESS,LESS,true) && checkOperator(firstGuard,firstBinop,LESS,GREATER,false))
			return SATISFY;
		if(checkOperator(secondGuard,secondBinop,LESS_OR_EQ,LESS_OR_EQ,true) && checkOperator(firstGuard,firstBinop,LESS,GREATER,false))
			return SATISFY;


	}else if(sumCheckInfo.elemType==IDLV_NEGATIVE){

		if(checkOperator(firstGuard,firstBinop,EQUAL,LESS,false))
			return UNSATISFY;

		if(checkOperator(firstGuard,firstBinop,LESS_OR_EQ,LESS,false))
			return UNSATISFY;
		if(checkOperator(firstGuard,firstBinop,LESS,LESS_OR_EQ,false))
			return UNSATISFY;

		if(secondBinop==NONE_OP && checkOperator(firstGuard,firstBinop,LESS_OR_EQ,GREATER_OR_EQ,true))
			return SATISFY;
		if(secondBinop==NONE_OP && checkOperator(firstGuard,firstBinop,LESS,GREATER,true))
			return SATISFY;

		if(firstBinop==NONE_OP && checkOperator(secondGuard,secondBinop,LESS,LESS,false))
			return SATISFY;
		if(firstBinop==NONE_OP && checkOperator(secondGuard,secondBinop,LESS_OR_EQ,LESS_OR_EQ,false))
			return SATISFY;

		if(firstBinop==NONE_OP && checkOperator(secondGuard,secondBinop,LESS,GREATER_OR_EQ,true))
			return UNSATISFY;
		if(firstBinop==NONE_OP && checkOperator(secondGuard,secondBinop,LESS_OR_EQ,GREATER,true))
			return UNSATISFY;

		if(checkOperator(secondGuard,secondBinop,LESS,LESS,false) && checkOperator(firstGuard,firstBinop,LESS_OR_EQ,GREATER_OR_EQ,true))
			return SATISFY;
		if(checkOperator(secondGuard,secondBinop,LESS_OR_EQ,LESS_OR_EQ,false) && checkOperator(firstGuard,firstBinop,LESS_OR_EQ,GREATER_OR_EQ,true))
			return SATISFY;
		if(checkOperator(secondGuard,secondBinop,LESS,LESS,false) && checkOperator(firstGuard,firstBinop,LESS,GREATER,true))
			return SATISFY;
		if(checkOperator(secondGuard,secondBinop,LESS_OR_EQ,LESS_OR_EQ,false) && checkOperator(firstGuard,firstBinop,LESS,GREATER,true))
			return SATISFY;
	}

	//Avoid negative guard if the sum of the negative weight in the aggregate are less of the guard
	if(firstBinop!=NONE_OP)
		if(firstGuard->getConstantValue()<0 && ((firstGuard->getConstantValue())-(sumCheckInfo.negativeSumValue))<0){
			if(secondBinop==NONE_OP)
				return SATISFY;
			else{
				firstBinop=NONE_OP;
				firstGuard=nullptr;
			}
		}
	if(secondBinop!=NONE_OP)
		if(secondGuard->getConstantValue()<0 && ((secondGuard->getConstantValue())-(sumCheckInfo.negativeSumValue))<0)
			return UNSATISFY;


	if(isAnAssigment()) findUndefAtoms();
	return UNDEF;

}

void AggregateAtom::print(ostream& stream) const{
	if(negative)
		stream<<"not ";
	if(firstGuard!=nullptr){
		firstGuard->print(stream);
		stream<<getBinopToStrng(firstBinop);
	}
	stream<<"#";
	if(aggregateFunction==AggregateFunction::COUNT){
		stream<<"count";
	}else if(aggregateFunction==AggregateFunction::MIN){
		stream<<"min";
	}else if(aggregateFunction==AggregateFunction::MAX){
		stream<<"max";
	}else if(aggregateFunction==AggregateFunction::SUM){
		stream<<"sum";
	}
	stream<<"{";
	bool first=true;
	bool firstElement=true;
	for(auto& element:aggregateElements){
		if(!firstElement)stream<<";";else firstElement=false;
		for(auto& term:element->getTerms()){
			if(!first)stream<<",";else first=false;
			term->print(stream);
		}
		stream<<":";first=true;
		for(auto& naf:element->getNafLiterals()){
			if(!first)stream<<",";else first=false;
			naf->print(stream);
		}
		first=true;
	}
	stream<<"}";
	if(secondGuard!=nullptr){
		stream<<getBinopToStrng(secondBinop);
		secondGuard->print(stream);
	}
}

Term* AggregateAtom::changeInStandardFormatGuard(Term* guard) {
	Term* t = nullptr;
	if (guard->getType() == TermType::NUMERIC_CONSTANT) {
		t=new NumericConstantTerm(false, guard->getConstantValue() + 1);
		TermTable::getInstance()->addTerm(t);
	}
	if (guard->getType() == TermType::VARIABLE) {
		Term* one_term = new NumericConstantTerm(false, 1);
		TermTable::getInstance()->addTerm(one_term);

		t = new ArithTerm(false, guard,one_term,Operator::PLUS);
		TermTable::getInstance()->addTerm(t);
	}
	return t;
}

void AggregateAtom::changeInStandardFormat() {

	// If one of the binop is UNEQUAL
	if(!this->negative && secondBinop==NONE_OP && firstBinop==Binop::UNEQUAL){
		firstBinop=EQUAL;
		this->negative=true;
	}
	if(this->negative && secondBinop==NONE_OP && firstBinop==Binop::UNEQUAL){
		firstBinop=EQUAL;
		this->negative=false;
	}
	if(!this->negative && firstBinop==NONE_OP && secondBinop==Binop::UNEQUAL){
		firstBinop=EQUAL;
		secondBinop=NONE_OP;
		firstGuard=secondGuard;
		secondGuard=nullptr;
		this->negative=true;
	}
	if(this->negative && firstBinop==NONE_OP && secondBinop==Binop::UNEQUAL){
		firstBinop=EQUAL;
		secondBinop=NONE_OP;
		firstGuard=secondGuard;
		secondGuard=nullptr;
		this->negative=false;
	}
	// If the second binop is an EQUAL
	if(secondBinop==Binop::EQUAL){
		firstBinop=Binop::EQUAL;
		firstGuard=secondGuard;
		secondBinop=Binop::NONE_OP;
		secondGuard=nullptr;
	}

	// One of the following cases hold:
	// Case: 1 > x > 3 or 1 >= x > 3 or 1 > x >= 3 or 1 >= x >= 3
	// Case: x >= 3 or x > 3
	// Case: 1 > x or 1 >= x
	if((firstBinop==Binop::GREATER || firstBinop==Binop::GREATER_OR_EQ) || (secondBinop==Binop::GREATER || secondBinop==Binop::GREATER_OR_EQ)){
		Binop tmpB=firstBinop;
		firstBinop=secondBinop;
		secondBinop=tmpB;
		Term* tmpT=firstGuard;
		firstGuard=secondGuard;
		secondGuard=tmpT;
		if(firstBinop==Binop::GREATER)
			firstBinop=Binop::LESS;
		if(secondBinop==Binop::GREATER)
			secondBinop=Binop::LESS;
		if(firstBinop==Binop::GREATER_OR_EQ)
			firstBinop=Binop::LESS_OR_EQ;
		if(secondBinop==Binop::GREATER_OR_EQ)
			secondBinop=Binop::LESS_OR_EQ;
	}

//	// Case: 1 < x
//	if(firstBinop==Binop::LESS && firstGuard!=0){
//		firstGuard=changeInStandardFormatGuard(firstGuard);
//		firstBinop=Binop::LESS_OR_EQ;
//	}
//
//	// Case: x >= 1
//	if(secondBinop==Binop::LESS_OR_EQ && secondGuard!=0){
//		secondGuard=changeInStandardFormatGuard(secondGuard);
//		secondBinop=Binop::LESS;
//	}
}

set_term AggregateAtom::getSharedVariable(vector<Atom*>::iterator begin,vector<Atom*>::iterator end) {
	//TODO Check if guards are considered
	set_term sharedTerms;
//	if(alsoGuards && (negative || firstBinop!=EQUAL))
//		sharedTerms=getGuardVariable();
	set_term terms=getVariable();
	for(auto atom=begin;atom!=end;++atom){
		Atom* current_atom=*atom;
		set_term variables;
		if(current_atom!=this){
			if(current_atom->isAggregateAtom()){
				if(current_atom->getFirstBinop()==EQUAL)
					variables=current_atom->getGuardVariable();
			}else if((current_atom->isClassicalLiteral() && !current_atom->isNegative() ) || (current_atom->isBuiltIn() && current_atom->getBinop()==EQUAL))
				variables=current_atom->getVariable();
			else
				continue;
			for(auto variable:variables)
				if(terms.count(variable))
					sharedTerms.insert(variable);
		}
	}

	return sharedTerms;
}

void AggregateAtom::getVectorSharedVariable( vector< Atom* >::iterator begin, vector< Atom* >::iterator end, const set_term & terms, vector< Term* >& toReturn,
        set_term& sharedTerms ){
	for ( auto atom = begin; atom != end; ++atom ){
		Atom* current_atom = *atom;
		set_term variables;
		if( current_atom->isAggregateAtom() && !((*current_atom) == (*this)) ){
				if( current_atom->getFirstBinop() == EQUAL )
					variables = current_atom->getGuardVariable();
			} else if( (current_atom->isClassicalLiteral() && !current_atom->isNegative()) || (current_atom->isBuiltIn() && current_atom->getBinop() == EQUAL) )
				variables = current_atom->getVariable();
			else
				continue;
			for ( auto variable : variables )
				if( terms.count( variable ) ){
					if( !sharedTerms.count( variable ) ){
						sharedTerms.insert( variable );
						toReturn.push_back( variable );
					}

			}
	}
}
vector< Term* > AggregateAtom::getVectorGlobalSharedVariable( Rule* rewriting, Rule* replacement ){
	set_term guards = getGuardVariable();
	vector< Term* > toReturn( guards.begin(), guards.end() );
	set_term sharedTerms;
	set_term terms = getVariable();
	getVectorSharedVariable( rewriting->getBeginBody(), rewriting->getEndBody(), terms, toReturn, sharedTerms );
	//getVectorSharedVariable( replacement->getBeginBody(), replacement->getEndBody(), terms, toReturn, sharedTerms );
	return toReturn;
}

bool AggregateAtom::isAllAggregateTermShared(vector<Atom*>::iterator begin,vector<Atom*>::iterator end) {
	set_term shared_variable=getSharedVariable(begin,end);
	if(negative || firstBinop!=EQUAL){
		set_term guards=getGuardVariable();
		shared_variable.insert(guards.begin(),guards.end());
	}
	for(auto element:aggregateElements){
		if(!element->isAllAggregateTermShared(shared_variable))
			return false;
	}

	return true;
}

bool AggregateAtom::checkAggregateCountNegativeGuard(bool& alwaysTrue) const{
	if(aggregateFunction==COUNT) {
		if(firstBinop!=NONE_OP && secondBinop==NONE_OP && firstGuard->getType()==NUMERIC_CONSTANT && firstGuard->getConstantValue()<0){
			alwaysTrue= !negative;
			return true;
		}
		if(secondBinop!=NONE_OP && secondGuard->getType()==NUMERIC_CONSTANT && secondGuard->getConstantValue()<0){
			alwaysTrue= negative;
			return true;
		}
	}
	return false;
}

Term* AggregateAtom::getCheckValue() {
	Term* value = 0;
	switch (aggregateFunction) {
	case MAX:
		if(*partialEvaluation>*undefAtomEvaluation)
			value=partialEvaluation;
		else
			value=undefAtomEvaluation;
		break;
	case MIN:
		if(*partialEvaluation<*undefAtomEvaluation)
			value=partialEvaluation;
		else
			value=undefAtomEvaluation;
		break;
	default:
		value=partialEvaluation->sum(undefAtomEvaluation);
		break;
	}
	return value;
}




bool AggregateAtom::checkOperatorValue(Term* term,Binop binopGuard,Binop binop, Binop op,Term* value){
	if(binopGuard!=binop)return false;
	switch (op) {
		case LESS:
			if(*value<*term)
				return true;
			break;
		case LESS_OR_EQ:
			if(*value<=*term) return true;
			break;
		case GREATER:
			if(*value>*term) return true;
			break;
		case GREATER_OR_EQ:
			if(*value>=*term) return true;
			break;
		case EQUAL:
			if(value->getIndex()==term->getIndex())	return true;
			break;
		default:
			return false;
			break;
	}
	return false;
}

bool AggregateAtom::checkOperator(Term* term,Binop binopGuard,Binop binop, Binop op, bool checkUndef) {
	Term* value=partialEvaluation;
	if(checkUndef)value=getCheckValue();
	if( (binop == binopGuard) && binopGuard == EQUAL && !term->isGround() )
		return false;
	return checkOperatorValue(term,binopGuard,binop,op,value);
}

void AggregateAtom::findUndefAtoms(){
	//if an aggregate element is undefined then add its term to the chain of combination generators
	for ( unsigned i = 0; i < this->aggregateElements.size(); i++ ){
		Atom* aggElementAtom = this->aggregateElements[i]->getNafLiteral( 0 );
		if( aggElementAtom->getTermsSize()>0 && !aggElementAtom->isFact() ){
			Term * t = this->aggregateElements[i]->getTerm( 0 );
			combinationGenerator->addTerm( t );
		}
	}
}

Term* AggregateAtom::generateNextCombination(bool& finish){
	Term* evaluation = partialEvaluation;
	do{
		if( !combinationGenerator->nextCombination() ){
			finish = true;
			return 0;
		}
		finish = false;
		evaluation = partialEvaluation;
		Term *currentEvaluation = combinationGenerator->getCurrentEvaluation();
		if( currentEvaluation != nullptr )
			applayAggregateOperator( evaluation, currentEvaluation );
	} while ( possibleUndefValue.count( evaluation ) );
	possibleUndefValue.insert( evaluation );
	return evaluation;
}

void AggregateAtom::applayAggregateOperator(Term*& n1, Term* n2){
	switch (aggregateFunction) {
		case MIN:
			if(*n1>*n2)
				n1=n2;
			break;
		case MAX:
			if(*n1<*n2)
				n1=n2;
			break;
			/*case COUNT:
			n1=n1->increment();
			break;*/
		default:
			if(n1->getType()==NUMERIC_CONSTANT && n2->getType()==NUMERIC_CONSTANT)
				n1=n1->sum(n2);
			break;
	}
}

void AggregateAtom::copyGuard(Atom* atom) {
	if( ! (atom->isAggregateAtom() || atom->isChoice()))return ;
	if(atom->getFirstBinop()!=NONE_OP){
		firstBinop=atom->getFirstBinop();
		firstGuard=atom->getFirstGuard();
	}
	if(atom->getSecondBinop()!=NONE_OP){
		secondBinop=atom->getSecondBinop();
		secondGuard=atom->getSecondGuard();
	}
}

bool AggregateAtom::equal(const Atom& atom) const {
	if(!atom.isAggregateAtom()) return false;
	if(negative!=atom.isNegative()) return false;
	return (*this)==atom;
}

bool AggregateAtom::checkAggregateSumCountStringGuard(bool& alwaysTrue) const {
	if(aggregateFunction==SUM || aggregateFunction==COUNT) {
		if((firstBinop==LESS_OR_EQ || firstBinop==EQUAL || firstBinop==LESS)
		&& firstGuard->getType()!=VARIABLE && firstGuard->getType()!=NUMERIC_CONSTANT){
			alwaysTrue= isNegative();
			return true;
		}
		if((secondBinop==LESS || secondBinop==LESS_OR_EQ) && firstBinop==NONE_OP
			&& secondGuard->getType()!=VARIABLE && secondGuard->getType()!=NUMERIC_CONSTANT){
			alwaysTrue= !isNegative();
			return true;
		}
	}
	return false;
}

bool AggregateAtom::checkAggregateAllAggTermShared(vector<Atom*>::iterator begin,vector<Atom*>::iterator end, bool& alwaysTrue)  {
	bool returnValue=false;
	if(aggregateFunction==COUNT  && isAllAggregateTermShared(begin,end) &&
		 (	(firstBinop==NONE_OP || firstGuard->getType()==NUMERIC_CONSTANT) && ((secondBinop==NONE_OP || secondGuard->getType()==NUMERIC_CONSTANT)) )){
		Term* max_evaluation=new NumericConstantTerm(false,aggregateElements.size());

		if(firstBinop==NONE_OP && checkOperatorValue(secondGuard,secondBinop,LESS,LESS,max_evaluation)){
			alwaysTrue=true;
			returnValue= true;
		}else
		if(firstBinop==NONE_OP && checkOperatorValue(secondGuard,secondBinop,LESS_OR_EQ,LESS_OR_EQ,max_evaluation)){
			alwaysTrue=true;
			returnValue= true;
		}else
		if(checkOperatorValue(firstGuard,firstBinop,LESS,LESS_OR_EQ,max_evaluation)){
			alwaysTrue=false;
			returnValue= true;
		}else
		if(checkOperatorValue(firstGuard,firstBinop,LESS_OR_EQ,LESS,max_evaluation)){
			alwaysTrue=false;
			returnValue= true;
		}else
		if( checkOperatorValue(firstGuard,firstBinop,EQUAL,LESS,max_evaluation)){
			alwaysTrue=false;
			returnValue= true;
		}else
		if(checkOperatorValue(firstGuard,firstBinop,LESS,GREATER,TermTable::getInstance()->term_zero) || checkOperatorValue(firstGuard,firstBinop,LESS_OR_EQ,GREATER_OR_EQ,TermTable::getInstance()->term_zero))
		{
			if(checkOperatorValue(secondGuard,secondBinop,LESS,LESS,max_evaluation) || checkOperatorValue(secondGuard,secondBinop,LESS_OR_EQ,LESS_OR_EQ,max_evaluation)){
				alwaysTrue=true;
				returnValue= true;
			}
		}

		delete max_evaluation;

	}

	if(isNegative())
		alwaysTrue=!alwaysTrue;

	return returnValue;
}

void AggregateAtom::getUnsolvedPredicateVariable(set_term& vars) {
	for(auto element:aggregateElements){
		element->getUnsolvedPredicateVariable(vars);
	}
}

}
}

