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
 * ArithTerm.cpp
 *
 *  Created on: 09/mar/2014
 *      Author: Davide
 */

#include "ArithTerm.h"
#include "../term/ConstantTerm.h"

#include "../hash/HashString.h"
#include "../hash/HashVecInt.h"
#include <stack>
#include "../../util/VecStack.h"
#include "../../util/WarningMessage.h"
#include <sstream>

namespace DLV2{

namespace grounder{

int ArithTerm::calculateValue(vector<Term*>& terms, bool& ok) {
	ok=true;
	VecStack<int> s;
	s.reserve(operators.size());
	for(auto op:operators){
		if(!op.second){
			if(terms[op.first]->getType()!=NUMERIC_CONSTANT){
				ok=false;
				WarningMessage::warningGeneric("Undefined Operation "+this->toString());
				return 0;
			}
			int constant=terms[op.first]->getConstantValue();
			s.push(constant);
		}else{
			int c1=s.pop();
			int c2=s.pop();
			Operator operat=unsignedToOperator(op.first);
			int result=0;
			if (operat == Operator::PLUS)
				result = c2+c1;
			else if (operat == Operator::MINUS)
				result = c2-c1;
			else if (operat == Operator::DIV){
				//FIXME For not stop the program division by 0 is 0
				if(c1 != 0)
					result = c2/c1;
				else{
					ok = false;
					result = 0;
				    }
			}else if (operat == Operator::TIMES)
				result = c2*c1;
			else if (operat == Operator::MODULO)
				result = c2%c1;
			s.push(result);
		}
	}
	int result = s.pop();
	return result;
}


LINE ArithTerm::transformToLineEq(){
	stack<LINE> s;
	for(auto op:operators){
		if(!op.second){
			s.push(terms[op.first]->transformToLineEq());
		}else{
			LINE c1=s.top();
			s.pop();
			LINE c2=s.top();
			s.pop();
			Operator operat=unsignedToOperator(op.first);
			LINE result;
			if (operat == Operator::PLUS)
				result = c2+c1;
			else if (operat == Operator::MINUS)
				result =c2-c1;
			else if (operat == Operator::DIV){
				//FIXME For not stop the program division by 0 is 0
				result = c2/c1;
			}else if (operat == Operator::TIMES)
				result = c2*c1;
			s.push(result);
		}
	}
	return s.top();
}

size_t ArithTerm::hash() {
	vector<size_t> hashVec(terms.size()+(operators.size()*2));
	hashVec[0]=terms[0]->getIndex();
	for(auto o:operators){
		if(!o.second)
			hashVec.push_back(terms[o.first]->getIndex());
		else
			hashVec.push_back(o.first);
	}
	return HashVecInt::getHashVecInt()->computeHashSize_T(hashVec);
}

void ArithTerm::print(ostream& stream) const{
	stack<string> expression;
	for(auto op:operators){
		if(!op.second){
			stringstream ss;
			bool addParenthesis=terms[op.first]->getType()==NUMERIC_CONSTANT && terms[op.first]->getConstantValue()<0;
			if(addParenthesis)
				ss<<"(";
			terms[op.first]->print(ss);
			if(addParenthesis)
				ss<<")";
			expression.push(ss.str());
		}else{
			string s1=expression.top();
			expression.pop();
			string s2=expression.top();
			expression.pop();
			Operator operat=unsignedToOperator(op.first);
			expression.push("("+s2+getNameOperator(operat)+s1+")");
		}
	}

	stream<<expression.top();
}

string ArithTerm::getNameOperator(Operator op) {
	if (op == Operator::PLUS)
		return "+";
	if (op == Operator::MINUS)
		return "-";
	if (op == Operator::TIMES)
		return "*";
	if (op == Operator::DIV)
		return "/";
	if (op == Operator::MODULO)
		return "\\";
	return "";
}

Operator ArithTerm::getOperatorName(char op) {
	if (op == '+')
		return Operator::PLUS;
	if (op == '-')
		return Operator::MINUS;
	if (op == '*')
		return Operator::TIMES;
	if (op == '/')
		return Operator::DIV;
	if (op == '\\')
		return Operator::MODULO;
	return Operator::DIV;
}

 bool ArithTerm::operator==(const Term& term)const{
	if(getType()!=term.getType())return false;
	if(operators.size()!=term.getSizeOperator())return false;
	if(terms.size()!=term.getTermsSize())return false;
	for(unsigned int i=0;i<operators.size();i++)
		if(operators[i].first!=term.getOperator(i).first || operators[i].second!=term.getOperator(i).second)
			return false;
	for(unsigned int i=0;i<terms.size();++i)
		if(terms[i]!=term.getTerm(i))
			return false;

	return true;
}

Term* ArithTerm::substitute(var_assignment& substritutionTerm) {
	// Create a new arithmetic term replacing the term in a vector
	// Recursively call substitute for nested function
	// At the end add a new term in a table and return index

	TermTable *termTable=TermTable::getInstance();
	vector<Term*> subTerms(terms.size());
	bool findVariable=false;
	for(unsigned int i=0;i<terms.size();++i){
		Term* sub_term=terms[i]->substitute(substritutionTerm);
		if(!findVariable && !sub_term->isGround())
			findVariable=true;
		subTerms[i]=sub_term;
	}
	if(!findVariable){
		bool ok=true;
		int result=calculateValue(subTerms,ok);
		if(!ok) return nullptr;
		Term *constantTerm=new NumericConstantTerm( result<0,result);
		TermTable::getInstance()->addTerm(constantTerm);
		return constantTerm;
	}
	Term *newTerm=new ArithTerm(negative,operators,subTerms);
	termTable->addTerm(newTerm);
	return  newTerm;
}

int ArithTerm::substituteAndCalculate(var_assignment& substritutionTerm, bool& ok){
	//Substitute the variable in the Arith term and calculate the value
	//The term returned is not added in the table Term

	vector<Term*> subTerms(terms.size());
	for(unsigned int i=0;i<terms.size();++i)
		subTerms[i]=terms[i]->substitute(substritutionTerm);
	int result=calculateValue(subTerms,ok);
	return result;
}

void ArithTerm::addArithTerm(Term* arith,Term* term,Operator op){
	if(term->getType()!=ARITH){
		arith->addTerm(term);
		arith->addOperator(op);
		return;
	}
	for(unsigned i=0;i<term->getSizeOperator();++i){
		auto oop=term->getOperator(i);
		if(!oop.second)
			arith->addTerm(term->getTerm(oop.first));
		else
			arith->addOperator(unsignedToOperator(oop.first));
	}
	arith->addOperator(op);

}

string ArithTerm::toString() const {
	ostringstream s;
	print(s);
	return s.str();
}

Operator ArithTerm::unsignedToOperator(unsigned op){
	switch (op) {
		case 0:
			return PLUS;
			break;
		case 1:
			return MINUS;
			break;
		case 2:
			return DIV;
			break;
		case 3:
			return TIMES;
			break;
		case 4:
			return MODULO;
			break;
		default:
			return PLUS;
			break;
	}
}

};

};
