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
 * AggregateElement.cpp
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#include "AggregateElement.h"
#include "../../util/Utils.h"


namespace DLV2{

namespace grounder{

	AggregateElement* AggregateElement::clone(){
		AggregateElement *element=new AggregateElement;
		element->setTerms(terms);
		for(auto atom:nafLiterals)
			element->addNafLiterals(atom->clone());
		return element;
	}

	void AggregateElement::clearNafLiterals() {
		for(auto atom:nafLiterals)
			delete atom;
		nafLiterals.clear();
	}

	void AggregateElement::deleteAtoms() {
		for(auto atom:nafLiterals)
			delete atom;
	}

	set_term AggregateElement::getSafeVariable() {
		set_term safeVars;
		for(auto atom:nafLiterals)
			if(!atom->isNegative() && ! atom->isBuiltIn())
				atom->getVariable(safeVars);
		return safeVars;
	}

	set_term AggregateElement::getUnsafeVariable() {
		set_term safeVars=getSafeVariable();
		set_term unsafeVars;
		for(auto atom:nafLiterals){
			set_term variableInArith;
			for(auto t:atom->getTerms())t->getVariablesInArith(variableInArith);
			if(variableInArith.size()>0)unsafeVars.insert(variableInArith.begin(),variableInArith.end());
			if(atom->isNegative() || atom->isBuiltIn()){
				for(auto variable:atom->getVariable())
					if(!safeVars.count(variable))
						unsafeVars.insert(variable);
			}else if(atom->isExternalAtom()){
				for(auto t:atom->getInputVariables())
					unsafeVars.insert(t);
			}
		}

		return unsafeVars;
	}

	void AggregateElement::getPredicates(set_predicate& predicates)const{
		for(auto atom:nafLiterals){
			if(atom->isClassicalLiteral())
				predicates.insert(atom->getPredicate());
		}
	}

	bool AggregateElement::areAggregationTermsSafe() const {
		set_term variables;
		for(auto term: terms)
			term->getVariable(variables);
		for(auto term: variables){
			if(term->isGround())continue;
			bool ok=false;
			for(auto atom:nafLiterals){
				if(((atom->isClassicalLiteral() && !atom->isNegative()) || atom->getBinop()==Binop::EQUAL)  && atom->getVariable().count(term)){
					ok=true;
				}
				if(atom->isExternalAtom() && atom->getOutputVariables().count(term))
					ok = true;
			}
			if(!ok)
				return false;
		}
		return true;
	}

	bool AggregateElement::operator ==(const AggregateElement& element) const {
		if(terms.size()!=element.terms.size())return false;
		if(nafLiterals.size()!=element.nafLiterals.size())return false;
		for(unsigned i=0;i<terms.size();i++)
			if(terms[i]->getIndex()!=element.terms[i]->getIndex())
				return false;
		for(unsigned i=0;i<nafLiterals.size();i++)
			if(!(*nafLiterals[i]==*element.nafLiterals[i]))
				return false;


		return true;
}



bool AggregateElement::isAllAggregateTermShared(set_term& shared_variable)const{
	for(auto term:terms){
		set_term variable_term;
		term->getVariable(variable_term);
		if(!Utils::isContained(variable_term,shared_variable))
			return false;
	}
	return true;
}

void AggregateElement::getUnsolvedPredicateVariable(set_term& vars) {
	for(auto atom:nafLiterals){
		if(atom->isClassicalLiteral() && !atom->getPredicate()->isSolved()){
			atom->getVariable(vars);
		}
	}
}

void AggregateElement::getVariableAggregationTerms(set_term& vars) const {
	for(auto t:terms)
		t->getVariable(vars);
}

void AggregateElement::getVariableInNaf(set_term& vars) const {
	for(auto n:nafLiterals)
		n->getVariable(vars);
}
vector< Term* > AggregateElement::getVectorVariableInNaf() const{
	vector< Term* > toReturn;
	for ( auto n : nafLiterals ){
		vector< Term* > tmp( n->getVectorVariable() );
		toReturn.insert( toReturn.end(), tmp.begin(), tmp.end() );
	}
	return toReturn;
}
AggregateElement::~AggregateElement() {}

}
}


