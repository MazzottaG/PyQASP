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
 * ClassicalLiteral.cpp
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#include "ClassicalLiteral.h"

#include <iostream>

#include "../hash/HashVecInt.h"
#include "../table/TermTable.h"

namespace DLV2{

namespace grounder{



size_t ClassicalLiteral::hash(){
	return HashVecInt::getHashVecInt()->computeHashTerm(terms);
}


void ClassicalLiteral::print(ostream& stream) const {
	print(predicate,terms,negative,hasMinus,stream);
}

void ClassicalLiteral::print(Predicate* predicate,const vector<Term*>& terms,bool negative,bool hasMinus,ostream& stream){
	if(negative)
		stream<<"not ";
	if(predicate->isTrueNegated())
		stream<<"-";
	stream<<predicate->getName();
	for (unsigned int i = 0; i < terms.size(); ++i){
		if(i==0)
			stream<<"(";
		terms[i]->print(stream);
		if(i!=terms.size()-1)
			stream<<",";
		else
			stream<<")";
	}
}



bool ClassicalLiteral::operator==(const Atom& a)const {
	if(a.getPredicate()==nullptr) return false;
	if(predicate->getIndex()!=a.getPredicate()->getIndex())return false;
	if(terms.size()!=a.getTermsSize())return false;
	for(unsigned int i=0;i<terms.size();i++)
		if(terms[i]->getIndex()!=a.getTerm(i)->getIndex())
			return false;

	return true;
}

void ClassicalLiteral::substitute(var_assignment& substitutionTerm,Atom*& templateAtom){
	if(templateAtom==nullptr){
		vector<Term*> terms_substitute(terms.size());
		for(unsigned int i=0;i<terms.size();++i){
			terms_substitute[i]=terms[i]->substitute(substitutionTerm) ;
		}
		templateAtom=new ClassicalLiteral(predicate,terms_substitute,hasMinus,negative);
	}
	else
		for(unsigned int i=0;i<terms.size();++i)
			templateAtom->setTerm(i,terms[i]->substitute(substitutionTerm));
}


bool ClassicalLiteral::equal(const Atom& atom) const {
	if(!atom.isClassicalLiteral()) return false;
	if(negative!=atom.isNegative()) return false;
	if(hasMinus!=atom.isHasMinus()) return false;
	return (*this)==atom;
}

}}
