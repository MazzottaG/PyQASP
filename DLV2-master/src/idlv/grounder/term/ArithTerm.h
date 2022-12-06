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
 * ArithTerm.h
 *
 *  Created on: 09/mar/2014
 *      Author: Davide
 */

#ifndef ARITHTERM_H_
#define ARITHTERM_H_

#include <vector>

#include "Term.h"
#include "../table/TermTable.h"

namespace DLV2{

namespace grounder{


/**
 *  ArithTerm is an arithmetic with the list of a terms
 *  and the relative operator (+,-,*,/,\)
 *
 *  Contains vector of id terms and vectors of relative operators
 *
 */


class ArithTerm: public Term {
public:
	ArithTerm():Term(){};
	ArithTerm(bool negative,const vector<OperandOperator>& operators,vector<Term*>& terms):Term(negative),operators(operators),terms(terms){};
	ArithTerm(bool negative,Term *t1,Term *t2,Operator op):Term(negative){
		addTerm(t1);
		addTerm(t2);
		addOperator(op);
	};
	ArithTerm(const ArithTerm& arith):Term(arith.negative),operators(arith.operators),terms(arith.terms){};


	///Add an operator in the vector
	virtual void addOperator(Operator op){operators.push_back({op,true});};
	virtual void addTerm(Term* term){terms.push_back(term);operators.push_back({terms.size()-1,false});};

	virtual unsigned getTermsSize()const{return terms.size();}
	virtual Term* getTerm(unsigned i)const{return terms[i];}

	///Return the size of the operators in arith term
	virtual unsigned int getSizeOperator()const{return operators.size();}
	virtual OperandOperator getOperator(int i) const {return operators[i];}

	virtual void getVariable(set_term& variables){
		for(auto term:terms)
			term->getVariable(variables);
	};

	virtual void getVariable(vector<Term*>& variables){
		for(auto term:terms)
			term->getVariable(variables);
	};

	virtual void getGroundTerm(set_term& variables){
		for(auto term:terms)
			term->getGroundTerm(variables);
	};

	///Calculate the value of the expression with the value in the terms vector
	int calculateValue(vector<Term*>& vt, bool&);
	Term* calculate(){
		bool ok=true;
		int result=calculateValue(terms,ok);
		if(!ok) return nullptr;
		Term *constantTerm=new NumericConstantTerm( result<0,result);
		TermTable::getInstance()->addTerm(constantTerm);
		return constantTerm;
	}


	virtual void popTerm(){terms.pop_back();};
	virtual Term* substitute(var_assignment& substritutionTerm);
	virtual int substituteAndCalculate(var_assignment& substritutionTerm, bool&);

	virtual TermType getType()const{return TermType::ARITH;};
	virtual bool contain(TermType type){
		if(type==TermType::ARITH) return true;
		for(auto term:terms)
			if(term->contain(type))
				return true;
		return false;
	}
	virtual bool isGround(){
		for(auto term:terms)
			if(!term->isGround())
				return false;
		return true;
	}

	virtual bool checkArithIsMinusOne(){
		unsigned count=0;
		Operator oper=PLUS;
		for(auto op:operators)
			if(op.second){
				count++;
				oper=unsignedToOperator(op.first);
			}

		if(count==1 && oper==TIMES){
			if(getTermsSize()==2 && (getTerm(0)==TermTable::getInstance()->term_minus_one || getTerm(1)==TermTable::getInstance()->term_minus_one ))
				return true;
		}
		return false;

	};

	vector<Term*>& getTerms(){return terms;};


	virtual bool operator==(const Term& term)const;

	virtual void getVariablesInArith(set_term& vars){this->getVariable(vars);}

	/// Return the string composed by the concatenation of terms and operators
	virtual size_t hash();
	virtual void print(ostream& stream=cout)const;
	/// Return the string of enum operator
	static string getNameOperator(Operator op);
	static Operator getOperatorName(char op);

	virtual LINE transformToLineEq();

	///Merge the first term, that shuld be an arith term, and a second term in the first term
	///Then if the second term isn't an arith term add the term and the operator in the first term.
	//If the second term is an arithmetic term, then add all the term and the operator of second term in the first term
	static void addArithTerm(Term* arith,Term* term,Operator op);

	virtual ~ArithTerm() {}

	virtual const vector<OperandOperator>& getOperators() const {return operators;}

	virtual string toString() const;

private:
	/*
	 * The operators
	 */
	vector<OperandOperator> operators;
	/**
	 *  All the index of the terms to calculate the operation
	 */
	vector<Term*> terms;

	static Operator unsignedToOperator(unsigned op);
};

};

};

#endif /* ARITHTERM_H_ */

