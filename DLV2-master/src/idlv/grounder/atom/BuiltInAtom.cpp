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
 * BuiltInAtom.cpp
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#include "BuiltInAtom.h"

#include <iostream>

#include "../table/TermTable.h"
#include "../hash/HashVecInt.h"

namespace DLV2{

namespace grounder{

bool BuiltInAtom::calculateVariableInAssignment(Term* firstTerm, Term* secondTerm, var_assignment& substitutionTerm) {
	if(firstTerm==nullptr || secondTerm==nullptr) return false;
	// If there is equal and variable assign that value
	if(firstTerm->getType()==TermType::VARIABLE){
		Term* t=secondTerm->calculate();
		if(t==nullptr) return false;
		substitutionTerm[firstTerm->getLocalVariableIndex()]=t;
		return true;
	}
	if(secondTerm->getType()==TermType::VARIABLE){
		Term* t=firstTerm->calculate();
		if(t==nullptr) return false;
		substitutionTerm[secondTerm->getLocalVariableIndex()]=t;
		return true;
	}
	LINE A = firstTerm->transformToLineEq(), B = secondTerm->transformToLineEq();
	VAR x = A.evaluate( B );

	//if the result is a float then the assignment must return false.
	//With a truncation, the equation may not hold
	//		Ex: 33=(Z*2), Z=16.5 so 33=33
	//	        but with a truncation Z=16 so 33=32 -> Wrong!
	if( std::fmod( x.i, 1.0 ) != 0 )
		return false;

	int result = x.i;

	Term *constantTerm = new NumericConstantTerm( result < 0, result );
	TermTable::getInstance()->addTerm( constantTerm );
	set_term variables;
	firstTerm->getVariablesInArith( variables );
	secondTerm->getVariablesInArith( variables );
	substitutionTerm[(*variables.begin())->getLocalVariableIndex()] = constantTerm;

	return true;
}

//TODO - This method have to be removed, use only groundAndEvaluate
bool BuiltInAtom::evaluate(var_assignment& substitutionTerm){
	Term* firstTerm=terms[0];
	Term* secondTerm=terms[1];

	// If there is equal and variable assign that value
	if(assignment){
		return calculateVariableInAssignment(firstTerm,	secondTerm, substitutionTerm) ;
	}

	if(binop==Binop::EQUAL)
		return firstTerm->getIndex()==secondTerm->getIndex();
	if(binop==Binop::UNEQUAL)
		return firstTerm->getIndex()!=secondTerm->getIndex();
	if(binop==Binop::LESS)
		return *firstTerm<*secondTerm;
	if(binop==Binop::LESS_OR_EQ)
		return *firstTerm<=*secondTerm;
	if(binop==Binop::GREATER)
		return *firstTerm>*secondTerm;
	if(binop==Binop::GREATER_OR_EQ)
		return *firstTerm>=*secondTerm;

	return false;
}

bool BuiltInAtom::groundAndEvaluate(var_assignment& substitutionTerm){
	// If there is equal and variable assign that value
	if(assignment){
		Term* firstTerm=terms[0]->substitute(substitutionTerm);
		Term* secondTerm=terms[1]->substitute(substitutionTerm);
		return calculateVariableInAssignment(firstTerm,	secondTerm, substitutionTerm) ;
	}
	Term* firstTerm=nullptr;
	Term* secondTerm=nullptr;
	bool deleteFirst=false;
	bool deleteSecond=false;
	bool ok1=true;
	bool ok2=true;
	if(terms[0]->getType()!=ARITH){
		firstTerm=terms[0]->substitute(substitutionTerm);
		firstTerm=firstTerm->calculate();
	}else{
		ok1=true;
		firstTerm=new NumericConstantTerm(false,terms[0]->substituteAndCalculate(substitutionTerm,ok1));
		deleteFirst=true;
	}
	if(terms[1]->getType()!=ARITH){
		secondTerm=terms[1]->substitute(substitutionTerm);
		secondTerm=secondTerm->calculate();
	}else{
		ok2=true;
		secondTerm=new NumericConstantTerm(false,terms[1]->substituteAndCalculate(substitutionTerm,ok2));
		deleteSecond=true;
	}

	bool eval=false;
	if(binop==Binop::EQUAL)
		eval= *firstTerm==*secondTerm;
	else if(binop==Binop::UNEQUAL)
		eval= !(*firstTerm==*secondTerm);
	else if(binop==Binop::LESS)
		eval= *firstTerm<*secondTerm;
	else if(binop==Binop::LESS_OR_EQ)
		eval= *firstTerm<=*secondTerm;
	else if(binop==Binop::GREATER)
		eval= *firstTerm>*secondTerm;
	else if(binop==Binop::GREATER_OR_EQ)
		eval= *firstTerm>=*secondTerm;

	if(deleteFirst)delete firstTerm;
	if(deleteSecond)delete secondTerm;

	if(!ok1 || !ok2) return false;

	return eval;
}

size_t BuiltInAtom::hash(){
	return HashVecInt::getHashVecInt()->computeHashTerm(terms);
}

bool BuiltInAtom::operator==(const Atom& a) const {

	if(!a.isBuiltIn())return false;
	if(binop != a.getBinop()) return false;
	if(terms[0]->getIndex() != a.getTerm(0)->getIndex()) return false;
	if(terms[1]->getIndex() != a.getTerm(1)->getIndex()) return false;

	return true;
}

void BuiltInAtom::print(ostream& stream) const {
	Term* firstTerm=terms[0];
	Term* secondTerm=terms[1];

	firstTerm->print(stream);
	if(binop==Binop::EQUAL)
		stream<<"=";
	if(binop==Binop::UNEQUAL)
		stream<<"!=";
	if(binop==Binop::LESS)
		stream<<"<";
	if(binop==Binop::LESS_OR_EQ)
		stream<<"<=";
	if(binop==Binop::GREATER)
		stream<<">";
	if(binop==Binop::GREATER_OR_EQ)
		stream<<">=";
	secondTerm->print(stream);
}


void BuiltInAtom::substitute(var_assignment& substitutionTerm,Atom*& templateAtom){
	if(templateAtom==nullptr){
		vector<Term*> terms_substitute(terms.size());
		for(unsigned int i=0;i<terms.size();i++){
			terms_substitute[i]=terms[i]->substitute(substitutionTerm) ;
		}
		templateAtom=new BuiltInAtom(binop,negative,terms_substitute,assignment);
	}
	else
		for(unsigned int i=0;i<terms.size();i++){
			templateAtom->setTerm(i,terms[i]->substitute(substitutionTerm));
		}
};

};

};
