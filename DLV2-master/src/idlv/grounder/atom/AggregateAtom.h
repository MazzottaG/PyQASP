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
 * AggregateAtom.h
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#ifndef AGGREGATEATOM_H_
#define AGGREGATEATOM_H_

#include <vector>
#include "Atom.h"
#include <limits.h>
#include "../statement/Rule.h"
#include "AggregateUtility/CombsGenerator.h"

using namespace std;

namespace DLV2{

namespace grounder{

///Specify the type of the element in the aggregate.
enum ElementType{
	IDLV_POSITIVE,IDLV_NEGATIVE,IDLV_MIXED,IDLV_EMPTY
};


/**
 * This class implements an aggregate atom extending atom
 */

class AggregateAtom: public Atom {
public:

	///Check if the binopGuard is equal to binop and check the value of true atom if checkUndef is false otherwise the getCheckValue, between
	/// the constant value of term with the operator op
	static bool checkOperatorValue(Term* term,Binop binopGuard,Binop binop, Binop op,Term* value);

	///Default constructor
	AggregateAtom(): firstBinop(Binop::NONE_OP), secondBinop(Binop::NONE_OP), aggregateFunction(AggregateFunction::NONE), negative(false),
		partialEvaluation(TermTable::getInstance()->term_zero), undefAtomEvaluation(TermTable::getInstance()->term_zero), firstGuard(nullptr), secondGuard(nullptr), assignment(false),
	    combinationGenerator(nullptr){};

	/** Constructor
		 * @param f set the first term of comparison
		 * @param fB set the first binary operation
		 * @param s set the second term of comparison
		 * @param sB set the second binary operation
		 * @param aF set the aggregate function type
		 * @param aE set the vector of aggregate elements
		 * @param negative set whether the atom is negated with negation as failure
		 */
	AggregateAtom(Term* f, Binop fB, Term* s, Binop sB, AggregateFunction aF, vector<AggregateElement*> aE, bool n):
		firstBinop(fB), secondBinop(sB), aggregateElements(move(aE)), negative(n), partialEvaluation(TermTable::getInstance()->term_zero), undefAtomEvaluation(TermTable::getInstance()->term_zero), firstGuard(f), secondGuard(s), assignment(false) {
		setAggregateFunction(aF);
	}

	AggregateAtom(Term* f, Binop fB, Term* s, Binop sB, AggregateFunction aF, bool n):
		firstBinop(fB), secondBinop(sB), negative(n),partialEvaluation(TermTable::getInstance()->term_zero), undefAtomEvaluation(TermTable::getInstance()->term_zero), firstGuard(f), secondGuard(s), assignment(false) {
		setAggregateFunction(aF);
	}
	AggregateAtom(Term* f, Binop fB, Term* s, Binop sB, bool n) :
			firstBinop(fB), secondBinop(sB),aggregateFunction(AggregateFunction::NONE), negative(n), partialEvaluation(TermTable::getInstance()->term_zero), undefAtomEvaluation(TermTable::getInstance()->term_zero), firstGuard(
					f), secondGuard(s), assignment(false) {
		aggregateFunction = AggregateFunction::NONE;
	}
	Atom* clone() {
		AggregateAtom* atom = new AggregateAtom(firstGuard, firstBinop, secondGuard, secondBinop, negative);
		atom->setAssignment(assignment);
		atom->setTerms(this->terms);
		for(auto agg_element:aggregateElements)
			atom->addAggregateElement(agg_element->clone());
		atom->aggregateFunction = this->aggregateFunction;
		atom->partialEvaluation=this->partialEvaluation;
		atom->undefAtomEvaluation=this->undefAtomEvaluation;
		atom->combinationGenerator = this->combinationGenerator->clone();
		for(auto term:possibleUndefValue)
			atom->possibleUndefValue.insert(term);
		return atom;
	};


	///This method compute the resulting hash a classical atom using its terms
	size_t hash() ;
	///This method compute the resulting hash for an aggregateElement TODO
	size_t getHash() const {return 0;};

	bool operator==(const Atom& a)const;

	virtual set_term getVariable(bool guard=true) const;
	virtual void getVariable(set_term& variables, bool guard=true) const;

	virtual void getVariablesInAggregationTerms(set_term& vars) const;

	virtual set_term getGuardVariable() const;

	virtual set_predicate getPredicates()const;

	///Getter method for the aggregate elements
//	const vector<AggregateElement*>& getAggregateElements() const {return aggregateElements;};
	///Getter method for the i-th aggregate element
	AggregateElement* getAggregateElement(unsigned i) const{return aggregateElements[i];}
	///Setter method for the aggregate elements
	void setAggregateElements(const vector<AggregateElement*>& aggregateElements) {this->aggregateElements = aggregateElements;};
	///Getter method for the aggregate elements size
	virtual unsigned getAggregateElementsSize() const{return aggregateElements.size();};
	/// Add aggregate element
	void addAggregateElement(AggregateElement *element){aggregateElements.push_back(element);};

	void setFirstGuard(Term* lower){firstGuard=lower;};
	void setSecondGuard(Term* upper){secondGuard=upper;};

	virtual bool isAggregateAtom()const{return true;}


	Term* getFirstGuard()const{return firstGuard;};
	Term* getSecondGuard()const{return secondGuard;};

	///Getter method for the aggregate function
	AggregateFunction getAggregateFunction() const {return aggregateFunction;};
	///Setter method for the aggregate function
	void setAggregateFunction(AggregateFunction aggregateFunction) {
		if(aggregateFunction==MIN){partialEvaluation=undefAtomEvaluation=TermTable::getInstance()->term_max;}
		if(aggregateFunction==MAX){partialEvaluation=undefAtomEvaluation=TermTable::getInstance()->term_min;}
		if( combinationGenerator != nullptr ) delete combinationGenerator;
		combinationGenerator = CombsGenerator::makeGenerator( aggregateFunction );
		this->aggregateFunction = aggregateFunction;
	};
	///Getter method for the first binary operation
	Binop getFirstBinop() const {return firstBinop;};
	///Setter method for the first binary operation
	void setFirstBinop(Binop firstBinop) {this->firstBinop = firstBinop;};
	///Getter method for the second binary operation
	Binop getSecondBinop() const {return secondBinop;};
	///Setter method for the second binary operation
	void setSecondBinop(Binop secondBinop) {this->secondBinop = secondBinop;};
	///Returns true if the atom is negated with negation as failure
	bool isNegative() const {return negative;};
	///Set whether the atom is negated with negation as failure
	void setNegative(bool negative) {this->negative = negative;};

	/// Substitute the term with constant term and calculate the arithmetic terms
	/// The subclasses have to implement the substitute method for create correct type class of Atom
	/// Similiar to ground(map_term_term& substritutionTerm) but not create new atom
	virtual void ground(var_assignment& substritutionTerm,Atom*& templateAtom);

	///Update the evaluation of the aggregate with the last aggregate element
	/// Return the result of the evaluation
	ResultEvaluation partialEvaluate();

	 /// Return the result of the evaluation of the aggregate
	 /// Call before partialEvaluate for update the partial evaluation
	 ResultEvaluation finalEvaluate();

	 Term* getPartialEvaluation(){return partialEvaluation;};

	 Term* getUndefEvaluation(){return undefAtomEvaluation;}

	 ///Check if a aggregate is a count or sum aggregate and have in the guard term different respect number. Because this aggregate
	 ///can return only number we can simplify the aggregate without evaluating.
	 ///The function return true if we can simplify and set alwaysTrue true if the aggregate is always true else false if is always false
	 bool checkAggregateSumCountStringGuard(bool& alwaysTrue) const;

	 ///Check if a aggregate is a count aggregate and have all the aggregate term shared. In this case we can simplify
	 /// the aggregate without evaluate.
	 ///The function return true if we can simplify and set alwaysTrue true if the aggregate is always true else false if is always false
	 bool checkAggregateAllAggTermShared(vector<Atom*>::iterator begin,vector<Atom*>::iterator end,bool& alwaysTrue) ;

	 ///Check if is count aggregate and if have negative guard simplify the aggregate
	 ///If the aggregate have to be > of -1 the aggregate is always true. If the aggregate have to be < -1 the aggregate is always false
	 ///The function return true if we can simplify and set alwaysTrue true if the aggregate is always true else false if is always false
	 bool checkAggregateCountNegativeGuard(bool& alwaysTrue)const;

	///return true if one guard is an equal
	bool isAnAssigment(){return ((firstBinop==Binop::EQUAL && !firstGuard->isGround()) || (secondBinop==Binop::EQUAL && !secondGuard->isGround() ));}

	bool invalideGuards(){
		if(firstBinop!=NONE_OP && secondBinop!=NONE_OP && firstGuard->isGround() && secondGuard->isGround() && firstGuard->getConstantValue()>secondGuard->getConstantValue())
			return true;
		return false;
	}

	/// Set the aggregate atom to standard format:
	/// - if it has an EQUAL binop, its first binop is set to EQUAL
	/// - else the first binop is LESS_EQUAL and the second LESS
	void changeInStandardFormat();

	virtual Term* generateNextCombination(bool& finish);

	///Printer method
	void print(ostream& stream=cout) const;

	virtual void deleteAtoms() {for(auto& aggregateElem:aggregateElements) aggregateElem->deleteAtoms();};

	virtual bool isAssignment(){return assignment;};
	virtual void setAssignment(bool assignment){this->assignment=assignment;};

	virtual set_term getSharedVariable(vector<Atom*>::iterator begin,vector<Atom*>::iterator end);
	virtual vector< Term* > getVectorGlobalSharedVariable( Rule* rewriting, Rule* replacement );
	virtual void getUnsolvedPredicateVariable(set_term& vars);

	//Copy the guard of the atom
	virtual void copyGuard(Atom *atom);

	//Check if two atoms are completely equal (checking also negation as failure, and strong negation)
	virtual bool equal(const Atom& atom) const;

	virtual bool isAllAggregateTermShared(vector<Atom*>::iterator begin,vector<Atom*>::iterator end);

	virtual ~AggregateAtom(){
		for ( auto& aggregateElem : aggregateElements )
			delete aggregateElem;
		delete combinationGenerator;
	}

private:
	///First binary operation
	Binop firstBinop;
	///Second binary operation
	Binop secondBinop;
	///Aggregate function @see AggregateFunction
	AggregateFunction aggregateFunction;
	///Vector of the aggregate elements
	vector<AggregateElement*> aggregateElements;
	///Negated with negation as failure
	bool negative;
	///Evaluation of true atoms
	Term* partialEvaluation;
	///Evaluation of undef atoms
	Term* undefAtomEvaluation;
	///The first (or lower) guard)
	Term* firstGuard;
	///The second (or upper) guard)
	Term* secondGuard;
	//Is true if the aggregate atom assigns value to a term, false if it is a comparison (like ==)
	bool assignment;
	//the generator of combinations in assignment
	CombsGenerator* combinationGenerator = nullptr;
	///Return the evaluation if the aggregate is not negated else invert the evaluation
	ResultEvaluation returnEvaluation(ResultEvaluation evaluation);
	///Partial evaluate the count aggregate
	ResultEvaluation partialEvaluateCount();
	///Partial evaluate the max aggregate
	ResultEvaluation partialEvaluateMax();
	///Partial evaluate the min aggregate
	ResultEvaluation partialEvaluateMin();
	///Partial evaluate the sum aggregate
	ResultEvaluation partialEvaluateSum();
	///Final evaluate the count aggregate
	ResultEvaluation finalEvaluateCount();
	///Final evaluate the max aggregate
	ResultEvaluation finalEvaluateMax();
	///Final evaluate the min aggregate
	ResultEvaluation finalEvaluateMin();
	///Final evaluate the sum aggregate
	ResultEvaluation finalEvaluateSum();

	set_term possibleUndefValue;
	void findUndefAtoms();
	void applayAggregateOperator(Term*& n1, Term* n2);
	void computeNextCombination();

	///Return true if the aggregate is an undefined assignment
	virtual bool isUndefAssignment(){
		if( combinationGenerator->size() > 0 )
			return true;
		return false;
	}

	void getVectorSharedVariable( vector< Atom* >::iterator begin, vector< Atom* >::iterator end, const set_term & terms, vector< Term* >& toReturn, set_term&sharedTerm );

	///Utility method: change the guards according to the standard format
	Term* changeInStandardFormatGuard(Term* guard);

	///Check if the binopGuard is equal to binop and check the value of true atom if checkUndef is false otherwise the getCheckValue, between
	/// the constant value of term with the operator op
	bool checkOperator(Term* term,Binop binopGuard,Binop binop, Binop op, bool checkUndef);

	///Get the sum of undef and true evaluation with sum and count, the maximum between undef and true with max,the minimun between undef and true with min
	Term* getCheckValue();

	///Contains information used for possible simplification during the evaluation of the sum aggregate
	struct SumCheckInfo{

		SumCheckInfo():elemType(IDLV_EMPTY),findUndefInSum(false),negativeSumValue(0){}

		///Used for the final evaluation of the aggregate sum. Is the type of the element undefined in the aggregate
		ElementType elemType;

		/// True if in the sum aggregate there are an a undefined aggregate element
		bool findUndefInSum;

		///Sum of the negative weight of the elements in the sum aggregate
		int negativeSumValue;
	} sumCheckInfo;

};

}}

#endif /* AGGREGATEATOM_H_ */
