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
 * ChoiceElement.cpp
 *
 *  Created on: 04/ago/2014
 *      Author: tesi2
 */

#include "ChoiceElement.h"

namespace DLV2{

namespace grounder{

set_predicate ChoiceElement::getPredicates() const {
	set_predicate predicates;
	for(unsigned i=0;i<choiceElement.size();i++){
		Predicate* predicate=choiceElement[i]->getPredicate();
		predicates.insert(predicate);
	}

	return predicates;
}

set_predicate ChoiceElement::getPredicateInNaf() const {
	set_predicate predicates;
	for(unsigned i=1;i<choiceElement.size();i++){
		Predicate* predicate=choiceElement[i]->getPredicate();
		predicates.insert(predicate);
	}

	return predicates;
}

set_predicate ChoiceElement::getPredicatePositiveInNaf() const {
	set_predicate predicates;
	for(unsigned i=1;i<choiceElement.size();i++)
		if(!choiceElement[i]->isNegative() && choiceElement[i]->isClassicalLiteral()){
			Predicate* predicate=choiceElement[i]->getPredicate();
			predicates.insert(predicate);
		}

	return predicates;
}

set_predicate ChoiceElement::getPredicateNegativeInNaf() const {
	set_predicate predicates;
	for(unsigned i=1;i<choiceElement.size();i++)
		if(choiceElement[i]->isNegative()){
			Predicate* predicate=choiceElement[i]->getPredicate();
			predicates.insert(predicate);
		}

	return predicates;
}

bool ChoiceElement::haveOnlyEqualBuiltin() const {
	return choiceElement.size()==2 && choiceElement[1]->isBuiltIn() && choiceElement[1]->getBinop()==EQUAL;
}

bool ChoiceElement::isBodyChoiceIsSolved() const {
	for(unsigned i=1;i<choiceElement.size();i++){
		if(choiceElement[i]->isClassicalLiteral() &&  !choiceElement[i]->getPredicate()->isSolved())
			return false;
	}
	return true;
}

void ChoiceElement::getVariableInNaf(set_term& terms) const {
	for(unsigned i=1;i<choiceElement.size();++i)
		choiceElement[i]->getVariable(terms);
}

void ChoiceElement::deleteAtoms() {
	for(auto& atom:choiceElement)
		delete atom;
}

bool ChoiceElement::operator ==(const ChoiceElement& element) const {
	if(choiceElement.size()!=element.choiceElement.size())return false;
	for(unsigned i=0;i<choiceElement.size();++i)
		if(!(choiceElement[i]==element.choiceElement[i]))
			return false;
	return true;
}


bool ChoiceElement::hasNegativeAtoms() const {
	for(auto a:choiceElement)
		if(a->isNegative())
			return true;
	return false;
}

ChoiceElement* ChoiceElement::clone(){
	ChoiceElement* ce=new ChoiceElement;
	for(auto a:choiceElement)
		ce->add(a->clone());
	return ce;
}
        
}
}

