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
 * Atom.cpp
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#include "Atom.h"

#include "../term/ConstantTerm.h"
#include "../table/TermTable.h"


namespace DLV2{

namespace grounder{


void Atom::ground(var_assignment& substritutionTerm,Atom*& templateAtom){
	substitute(substritutionTerm,templateAtom);

	for(unsigned int i=0;i<templateAtom->getTermsSize();i++){
		Term* term=templateAtom->getTerm(i);
		/// Calculate the value of arithmetic term and add in terms table
		if(term!=nullptr && term->contain(TermType::ARITH) && term->isGround())
			templateAtom->setTerm(i,term->calculate());
	}

};

set_term Atom::getVariable(bool guard) const {
	set_term variables;
	for(auto term:terms)
		term->getVariable(variables);
	return variables;
}

void Atom::getVariable(set_term& variables, bool guard) const {
	for(auto term:terms)
		term->getVariable(variables);
}

vector<Term*> Atom::getVectorVariable(){
	vector<Term*> variables;
	for(auto term:getTerms())
		term->getVariable(variables);
	return variables;
}

string Atom::getBinopToStrng(Binop binop){
	switch (binop) {
		case EQUAL:
			return "=";
			break;
		case UNEQUAL:
			return "!=";
			break;
		case LESS:
			return "<";
			break;
		case LESS_OR_EQ:
			return "<=";
			break;
		case GREATER:
			return ">";
			break;
		case GREATER_OR_EQ:
			return ">=";
			break;
		default:
			return "";
			break;
	}
}

}}
