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
 * Term.h
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#ifndef TERM_H_
#define TERM_H_


#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "../hash/Hashable.h"
#include "../../util/Constants.h"
#include "../table/AdvancedArray.h"
#include "../../util/Equation.h"
#include <cstring>

using namespace std;


namespace DLV2{

namespace grounder{


/// Operator of and arithmetic term
enum Operator {
	PLUS=0, MINUS=1, DIV=2, TIMES=3, MODULO=4
};

using OperandOperator = pair<unsigned,bool>;

enum TermType{
	NUMERIC_CONSTANT = 0, STRING_CONSTANT, SYMBOLIC_CONSTANT, VARIABLE, ARITH, FUNCTION, ANONYMOUS, RANGE, LIST, LIST_HEAD_TAIL
};

class Term;

template<typename T>
using map_term = unordered_map<Term*, T, IndexForTable<Term>, IndexForTable<Term>>;
using set_term = hashIndexSet<Term>;
using var_assignment = AdvancedArray<Term*,ARRAY_SIZE>;



/**
 *  Term is an abstract class that represent one general Term.
 *  All the term have one index that represent our id.
 *
 *  The structure of a Term is based on the Composite pattern
 */
class Term: public Indexable,public Hashable {
public:

	/// Empty constructor
	Term():Indexable(),negative(0){};
	//Constructor only if is negative, set id in second time
	Term(bool negative):Indexable(),negative(negative){};
	//Constructor with negation and index
	Term(bool negative,index_object index):Indexable(index),negative(negative){};

	/// Return the string used to hash the term
	virtual size_t hash(){return 0;};

	virtual bool isRange() const {return false;}

	///Return the negation of the term
	bool isNegative(){return negative;};
	///Set the negation of the term
	virtual void setNegative(bool n){negative=n;};
	///Return the numeric term constant
	virtual int getConstantValue() const {return 0;};
	///Return the name of the term
	virtual string getName()const{return "";};
	///Set the name of the term
	virtual void setName(string& name){};
	/// Return the size of the terms (Function and Arith)
	virtual unsigned getTermsSize()const{return 0;}
	///Return the term in terms
	virtual Term* getTerm(unsigned i)const{return nullptr;}
	///Return the size of the operators in arith term
	virtual unsigned int getSizeOperator()const{return 0;}
	///Return the operator with index i
	virtual OperandOperator getOperator(int i)const{return {0,0};}
	///Return true if the arith term is minus one
	virtual bool checkArithIsMinusOne(){return false;};

	///Return the type of term
	virtual TermType getType() const {return TermType::NUMERIC_CONSTANT;};
	///Return true if contain a term of the given type
	virtual bool contain(TermType type){return false;};
	///Return true if is ground
	virtual bool isGround(){return false;}

	///This method is used for indexing term
	///TODO change the name to equal?
	virtual bool operator==(const Term& term)const{
		if(getType()!=term.getType())return false;
		if(getName()!=term.getName())return false;
		return true;
	};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
	virtual vector<Term*>& getTerms(){};
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
	virtual const vector<OperandOperator>& getOperators() const {};
#pragma GCC diagnostic pop


	virtual bool operator>(const Term& term)const{return false;};
	virtual bool operator>=(const Term& term)const{return false;};

	virtual bool operator<(const Term& term)const{return false;};
	virtual bool operator<=(const Term& term)const{return false;};

	virtual Term* increment(){return 0;};
	virtual Term* sum(Term* t1){return 0;};

	bool isTermMax() const;
	bool isTermMin() const;
	bool isTermZero() const;

	virtual void getVariablesInArith(set_term& vars){};

	///Check if one or both terms are term max or term min and behaves as the operator >
	///If checkEqual is true it behaves as the operator >=
	///Returns 1 or 0 if one of the two terms is the max or the min (according to the comparison)
	///Returns 2 otherwise
	int checkMaxAndMinTerm(const Term& term, bool checkEqual) const{
		if(isTermMax() && !term.isTermMax())
			return 1;
		if(term.isTermMax())
			return checkEqual;
		if(term.isTermMin() && isTermMin() && checkEqual)
			return 1;
		if(isTermMin() && !term.isTermMin())
			return 0;
		if(!isTermMin() && term.isTermMin())
			return 1;
		return 2;
	}

	virtual int substituteAndCalculate(var_assignment& substritutionTerm, bool&){return 0;}

	///Add the index of a composite term
	virtual void addTerm(Term* termIndex){};
	///Remove last index of a term
	virtual void popTerm(){};
	/// Set operator for arithmetic Term
	virtual void addOperator(Operator op){};
	/// Calculate the value for arithmetic term and return the ID of the result term
	virtual Term* calculate(){return this;};
	/// If the term is variable insert the variables in the set
	virtual void getVariable(set_term& variables){};
	/// If the term is variable insert the variables in the vector of variable
	virtual void getVariable(vector<Term*>& variables){};
	/// If the term is ground insert the term in the setr
	virtual void getGroundTerm(set_term& variables){void(0);};
	/// Substitute the term with the given terms and return the term of substitute term
	/// @param substritutionTerm map of index_object. The first index is the ID of term to substitute and second the value
	virtual Term* substitute(var_assignment& substritutionTerm){return nullptr;};
	/// Print with the cout the term
	virtual void print(ostream& stream=cout)const{};

	///Return the local index of the variable in the rule
	inline virtual index_object getLocalVariableIndex()const{return 0;};
	///Set the local index of the variable in the rule
	inline virtual void setLocalVariableIndex(index_object index){};
	//Transform the arith term in LINE struct for the evaluation of the equation
	virtual LINE transformToLineEq(){return LINE();};

	virtual bool containsVariable(Term* t){
		set_term variables;
		getVariable(variables);
		return variables.count(t);
	}

	virtual void substitute(Term* t,Term* t1){}

	virtual string toString()const{return "";}
	///Transform an Head Tail list term in normal List term
	virtual Term* toList(){return this;}
	///If the term is head tail list return a pair of the Head and the Tail of the list 
	virtual pair< Term*, Term* > getHeadTailList(){	return{nullptr,nullptr};}
	///If the list contains an Head Tail list with Head a variable, insert the variable in variables
	///because the variable must be a list
	virtual void getVariableMustBeList( vector< Term* >& variables ){};

	///If the term not is a list then this method returns it. Otherwise, it
	///creates a new list term that contains its terms in reverse order and
	///calls "getReverseTerms" on each term.
	virtual Term* getReverseTerms(){return this;};
	///If the term is different from element returns it.
	///If the term is a list then it calls "deleteAllOccurrence" on each term.
	virtual Term* deleteAllOccurrence( Term * element ){
		if( this != element )
			return this;
		return nullptr;
	}
	///If a term has already been removed, this method returns it.
	///Otherwise it only returns it if the term is different from element.
	///If the term is a list then it calls "deleteFirstOccurrence" on each term.
	virtual Term* deleteFirstOccurrence( Term * elem, bool& dlt ){
		if( this != elem || !dlt )
			return this;
		else if( this == elem && dlt )
			dlt = false;
		return nullptr;
	}

	virtual Term* getReverseList(){return nullptr;}
	virtual void flatten( vector< Term* >& termsF ){termsF.push_back( this );}
	virtual ~Term(){};
protected:
	/*
	 *  True if the term is negated, else false
	 */
	bool negative;
};



};

};

#endif /* TERM_H_ */
