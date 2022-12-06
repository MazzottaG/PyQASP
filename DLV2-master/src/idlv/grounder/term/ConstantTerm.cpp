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
 * ConstantTerm.cpp
 *
 *  Created on: 27/feb/2014
 *      Author: Davide
 */

#include "ConstantTerm.h"
#include "../table/TermTable.h"

namespace DLV2{

namespace grounder{

bool NumericConstantTerm::operator>(const Term& term)const{
	int checkMaxMin=checkMaxAndMinTerm(term,false);
	if(checkMaxMin!= 2) return checkMaxMin;
	if(TermType::NUMERIC_CONSTANT==term.getType())
		return numeric_constant>term.getConstantValue();

	return false;
};

bool NumericConstantTerm::operator>=(const Term& term)const{
	int checkMaxMin=checkMaxAndMinTerm(term,true);
	if(checkMaxMin!= 2) return checkMaxMin;
	if(TermType::NUMERIC_CONSTANT==term.getType())
		return numeric_constant>=term.getConstantValue();

	return false;
}

 bool NumericConstantTerm::operator<(const Term& term)const {
	int checkMaxMin=checkMaxAndMinTerm(term,false);
	if(checkMaxMin!= 2) return !checkMaxMin;
	if(TermType::NUMERIC_CONSTANT==term.getType())
		return numeric_constant<term.getConstantValue();

	return true;
};

bool NumericConstantTerm::operator<=(const Term& term)const{
	int checkMaxMin=checkMaxAndMinTerm(term,true);
	if(checkMaxMin!= 2) return !checkMaxMin;
	if(TermType::NUMERIC_CONSTANT==term.getType())
		return numeric_constant<=term.getConstantValue();

	return true;
};

Term* NumericConstantTerm::sum(Term* t){
	if(t->getType()==NUMERIC_CONSTANT){
		Term* term=new NumericConstantTerm(false,t->getConstantValue()+numeric_constant);
		TermTable::getInstance()->addTerm(term);
		return term;
	}
	return 0;
};

Term* NumericConstantTerm::increment(){
	Term* term=new NumericConstantTerm(false,numeric_constant+1);
	TermTable::getInstance()->addTerm(term);
	return term;
};

 bool StringConstantTerm::operator>(const Term& term)const{
	int checkMaxMin=checkMaxAndMinTerm(term,false);
	if(checkMaxMin!=2) return checkMaxMin;
	if(term.getType()==NUMERIC_CONSTANT)return true;

	if(getType()==STRING_CONSTANT && term.getType()==SYMBOLIC_CONSTANT)
		return true;

	if(term.getType()==getType())
		return string_constant>term.getName();

	return false;
};
 bool StringConstantTerm::operator>=(const Term& term)const{
	int checkMaxMin=checkMaxAndMinTerm(term,true);
	if(checkMaxMin!=2) return checkMaxMin;
	if(term.getType()==NUMERIC_CONSTANT)return true;

	if(getType()==STRING_CONSTANT && term.getType()==SYMBOLIC_CONSTANT)
		return true;

	if(term.getType()==getType())
		return string_constant>=term.getName();

	return false;
}

 bool StringConstantTerm::operator<(const Term& term)const {
	int checkMaxMin=checkMaxAndMinTerm(term,false);
	if(checkMaxMin!=2) return !checkMaxMin;
	if(term.getType()==NUMERIC_CONSTANT)return false;

	if(getType()==STRING_CONSTANT && term.getType()==SYMBOLIC_CONSTANT)
		return false;

	if(term.getType()==getType())
		return string_constant<term.getName();

	return true;
};

 bool StringConstantTerm::operator<=(const Term& term)const{
	int checkMaxMin=checkMaxAndMinTerm(term,true);
	if(checkMaxMin!=2) return !checkMaxMin;
	if(term.getType()==NUMERIC_CONSTANT)return false;

	if(getType()==STRING_CONSTANT && term.getType()==SYMBOLIC_CONSTANT)
		return false;

	if(term.getType()==getType())
		return string_constant<=term.getName();

	return true;
};

};

};
