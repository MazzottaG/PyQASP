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
 * Atom.h
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 *
 */

#ifndef ATOM_H_
#define ATOM_H_



#include <vector>
#include <string>
#include <unordered_map>

#include "../table/HistoryStructure.h"
#include "AggregateElement.h"
#include "ChoiceElement.h"
#include "../table/TermTable.h"
#include "Predicate.h"
#include "../../util/Utils.h"

using namespace std;

namespace DLV2{

namespace grounder{
class Rule;
class AggregateElement;
class ChoiceElement;

/// Aggregate Function Types
enum AggregateFunction {NONE=0,COUNT,MAX,MIN,SUM};

/// Binary Operation Types
enum Binop {NONE_OP=0,EQUAL,UNEQUAL,LESS,GREATER,LESS_OR_EQ,GREATER_OR_EQ};

enum ResultEvaluation{
	SATISFY=0, UNSATISFY, UNDEF
};



/**      The hierarchy is this:
*      									Atom
*      	|					|				|					|
*   	ClassicalLiteral	BuiltInAtom		AggregateAtom		Choice
*
*	Atom acts like an interface for its subclasses.
*/

class Atom: public Hashable, public Indexable {
public:


	///Default constructor
	Atom():Indexable(0) {};

	/** Constructor
	 * @param termsVec set the terms vector
	 */
	Atom(vector<Term*>& termsVec) : terms(move(termsVec)) {};

	/** Constructor
	 * @param negative set whether the atom is negated with negation as failure
	 */
	Atom(bool negative) {this->setNegative(negative);};

	virtual Atom* clone() = 0;

	///Equal-to operator for atoms
	/// Not check the predicate of the atoms
	virtual bool operator==(const Atom& a)const = 0;

	///Return the term of variable present in the Atom
	///If the guard is true add also the variable in the guard
	virtual set_term getVariable(bool guard=true) const;
	virtual void getVariable(set_term& variables, bool guard=true) const;

	///Return the term of variable present in the Atom in a vector
	virtual vector<Term*> getVectorVariable();


	/// Return true if is ground, each term is constant term
	virtual bool isGround(){
		for(auto term:terms)
			if(term!=nullptr && !term->isGround())return false;
		return true;
	}

	/// Return true if contains a range term
	virtual bool containsRangeTerms(){
		for(auto term:terms)
			if(term->contain(TermType::RANGE)) return true;
		return false;
	};

	/// Return true if contains an anonymus term
	virtual bool containsAnonymous(){
		for(auto term:terms)
			if(term->contain(TermType::ANONYMOUS))return true;
		return false;
	};

	/// Return true if contains an anonymus term
	virtual bool containsVariablesOrAnonymous(){
		for(auto term:terms)
			if(term->contain(TermType::ANONYMOUS) || term->contain(TermType::VARIABLE))return true;
		return false;
	};

	virtual bool containsFunctionalTerms(){
		for(auto term:terms)
			if(term->contain(TermType::FUNCTION))return true;
		return false;
	}

	bool hasPredicate()const{
		return getPredicate()!=nullptr;
	}

	/******** Methods useful for ClassicalLiteral ********/
	/**
	 * Getter method for the predicate
	 */
	virtual Predicate* getPredicate() const {return nullptr;};
	virtual set_predicate getPredicates() const {return set_predicate();};
	virtual set_predicate getFirstAtomsPredicates() const {return set_predicate();};
	virtual set_predicate getNafLiteralsPredicates() const {return set_predicate();};
	virtual unordered_set<index_object> getFirstAtomsPredicatesIndices() const {return unordered_set<index_object>();};

	///Return true if is a Classical Literal
	virtual bool isClassicalLiteral()const {return false;};

	///Setter method for the predicate
	virtual void setPredicate(Predicate* predicate) {};
	///This method returns the size of the terms' vector
	const unsigned int getTermsSize() const {return terms.size();};

	/// Returns the term in the position i
	vector<Term*>& getTerms(){return terms;};
	const vector<Term*>& getTerms() const {return terms;};
	inline Term* getTerm(unsigned int i) const {return terms[i];};
	inline Term* operator[](unsigned i) const {return terms[i];};
	inline Term*& operator[](unsigned i) {return terms[i];};

	///Substitute the term in terms at position i
	void setTerm(unsigned int i,Term* term) {terms[i]=term;};
	/// Setter for terms
	void setTerms(const vector<Term*>& terms) {this->terms=terms;};
	///Returns true if the atom is negated with true negation
	virtual bool isHasMinus() const {return 0;};
	///Set whether the atom is negated with true negation
	virtual void setHasMinus(bool hasMinus) {};
	///Returns true if the atom is negated with negation as failure
	virtual bool isNegative() const {return 0;};
	///Set whether the atom is negated with negation as failure
	virtual void setNegative(bool negative) {};
	/*****************************************************/

	/******** Methods useful for BuiltInAtom ********/
	///Getter method for the binary operation
	virtual Binop getBinop() const {return Binop::NONE_OP;};
	///Setter method for the binary operation
	virtual void setBinop(Binop binop) {};
	/// Return true if is BuiltInAtom
	virtual bool isBuiltIn()const{return false;};
	///This method evaluate the truth value of the built-in atom, if there is bind variable
	/// and equal then assign that value for the bind variable
	virtual bool evaluate(var_assignment& substritutionTerm ){return false;};
	///This method is like the evaluate but first substitute the variable in the builtin
	virtual bool groundAndEvaluate(var_assignment& substitutionTerm){return false;}
	///Return true if the built in assign value in term, else compare the variable like ==
	virtual bool isAssignment(){return false;};
	virtual void setAssignment(bool assigment){};
	//Return true if the builtin not contains in the operator of Airth term a DIV or a TIMES and not contain function
	virtual bool plusMinusBuiltin(){return true;};
	virtual bool isComparisonBuiltIn() const{return false;}
	/*****************************************************/

	/******** Methods useful for AggregateAtom ********/
	///Getter method for the aggregate elements
//	virtual const vector<AggregateElement*>& getAggregateElements() const {return vector<AggregateElement*>(0);};
	///Getter method for the i-th aggregate element
	virtual AggregateElement* getAggregateElement(unsigned i) const{return 0;}
	///Setter method for the aggregate elements
	virtual void setAggregateElements(const vector<AggregateElement*>& aggregateElements) {};
	///Getter method for the aggregate elements size
	virtual unsigned getAggregateElementsSize() const{return 0;};
	/// Return true if is an aggregate atom
	virtual bool isAggregateAtom()const{return false;}
	///Setter method for the i-th aggregate element
	virtual void setAggregateElement(unsigned i, const vector<Atom*>& nafLits, const vector<Term*>& terms) {}
	///Setter method for the i-th aggregate element
	virtual void setAggregateElement(unsigned i, const vector<Atom*>& nafLits) {}
	///Getter method for the aggregate function
	virtual AggregateFunction getAggregateFunction() const {return AggregateFunction::NONE;};
	///Set first guard of aggregate
	virtual void setFirstGuard(Term* lower){};
	///Set second guard of aggregate
	virtual void setSecondGuard(Term* upper){};
	///Get lower guard of aggregate
	virtual Term* getFirstGuard()const{return nullptr;};
	///Get upper guard of aggregate
	virtual Term* getSecondGuard()const{return nullptr;};
	///Setter method for the aggregate function
	virtual void setAggregateFunction(AggregateFunction aggregateFunction) {};
	///Getter method for the first binary operation
	virtual Binop getFirstBinop() const {return Binop::NONE_OP;};
	///Setter method for the first binary operation
	virtual void setFirstBinop(Binop firstBinop) {};
	///Getter method for the second binary operation
	virtual Binop getSecondBinop() const {return Binop::NONE_OP;};
	///Setter method for the second binary operation
	virtual void setSecondBinop(Binop secondBinop) {};
	/// Add aggregate Element
	virtual void addAggregateElement(AggregateElement* element){};

	virtual ResultEvaluation partialEvaluate(){return ResultEvaluation::SATISFY;};

	/// Return the result of the evaluation of the aggregate
	virtual ResultEvaluation finalEvaluate(){return ResultEvaluation::SATISFY;};

	///Return the value of evaluation
	virtual Term* getPartialEvaluation(){return 0;};

	virtual Term* getUndefEvaluation(){return 0;};

	//Return the variable in the guard of an aggregate
	virtual set_term getGuardVariable() const {return set_term();};

	virtual void getVariablesInAggregationTerms(set_term& vars) const {};

	///return true if one guard is an equal
	virtual bool isAnAssigment(){return false;};

	virtual Term* generateNextCombination(bool& finish) {return 0;};

	virtual bool isUndefAssignment() {return false;}

	//Copy the guard of the atom
	virtual void copyGuard(Atom *atom){};

	 ///Check if a aggregate is a count or sum aggregate and have in the guard term different respect number. Because this aggregate
	 ///can return only number we can simplify the aggregate without evaluating.
	 ///The function return true if we can simplify and set alwaysTrue true if the aggregate is always true else false if is always false
	virtual  bool checkAggregateSumCountStringGuard(bool& alwaysTrue) const{return false;};
	///Check if a aggregate is a count aggregate and have all the aggregate term shared. In this case we can simplify
	/// the aggregate without evaluate.
	///The function return true if we can simplify and set alwaysTrue true if the aggregate is always true else false if is always false
	virtual bool checkAggregateAllAggTermShared(vector<Atom*>::iterator begin,vector<Atom*>::iterator end,bool& alwaysTrue) {return false;};

	///Check if is count aggregate and if have negative guard simplify the aggregate.
	///If the aggregate have to be > of -1 the aggregate is always true. If the aggregate have to be < -1 the aggregate is always false
	 ///The function return true if we can simplify and set alwaysTrue true if the aggregate is always true else false if is always false
	virtual bool checkAggregateCountNegativeGuard(bool& alwaysTrue)const{return false;};

	virtual set_term getSharedVariable(vector<Atom*>::iterator begin,vector<Atom*>::iterator end){return set_term();};
	virtual vector< Term* > getVectorGlobalSharedVariable( Rule* rewriting, Rule* replacement ){
		return vector<Term*>();
	}

	/// Set the aggregate atom to standard format:
	/// - if it has an EQUAL binop, its first binop is set to EQUAL
	/// - else the first binop is LESS_EQUAL and the second LESS
	virtual void changeInStandardFormat(){};

	///Get variables of unsolved predicates appearing in the aggregate
	virtual void getUnsolvedPredicateVariable(set_term& vars){};

	/*****************************************************/

	/******** Methods useful for Choice ********/
	///Getter method for the choice elements
//	virtual const vector<ChoiceElement*>& getChoiceElements() const {return vector<ChoiceElement*>(0);};
	///Setter method for the choice elements
	virtual void setChoiceElements(const vector<ChoiceElement*>& choiceElements) {};
	///Returns the choice elements size
	virtual unsigned getChoiceElementsSize() const {return 0;}
	///Returns the i-th choice element
	virtual ChoiceElement* getChoiceElement(unsigned i) const {return 0;}
	///Return true if is a choice atom
	virtual bool isChoice()const{return false;}
	///Add a choice element
	virtual void addChoiceElement(ChoiceElement* choiceElement){}
	virtual void addSingleChoiceElement(Atom* atom){}
	///Return true if the choice atom have the default guard ({}>=0)
	virtual bool isDefaultGuard(){return false;}
	/*****************************************************/

	/******** Methods useful for Generic Atom ********/
	virtual bool isFact() const { return false; }
	virtual void setFact(bool isFact) {  }
	/*****************************************************/

	static string getBinopToStrng(Binop binop);


	/// Substitute the terms in the atom with the given terms and return the atom with terms sobstitute
	/// @param substritutionTerm map of index_object. The first index is the ID of term to substitute and second the value
	/// Similiar to substitute(map_term_term& substitutionTerm) but not create new atom
	virtual void substitute(var_assignment& substitutionTerm,Atom*& templateAtom){};


	/// Substitute the term with constant term and calculate the arithmetic terms
	/// The subclasses have to implement the substitute method for create correct type class of Atom
	/// Similiar to ground(map_term_term& substritutionTerm) but not create new atom
	virtual void ground(var_assignment& substritutionTerm,Atom*& templateAtom);
	///Printer method
	virtual void print(ostream& stream=cout) const = 0;

	virtual void deleteAtoms() {};

	///Destructor
	virtual ~Atom() {};

	//Check if two atoms are completely equal (checking also negation as failure, and strong negation)
	virtual bool equal(const Atom& atom) const {return (*this)==atom;}

	virtual void substituteVarWithAnonymous(Term* t){}

	virtual bool containsArithTerms(){
		for(auto term:terms)
			if(term->contain(TermType::ARITH))return true;
		return false;
	}

	/******** Methods useful for External Atom ********/
	virtual bool isExternalAtom()const { return false; }
	virtual bool match(var_assignment& assignement, bool direction) { return false; }
	virtual unsigned getSeparator() const { return 0; }
	virtual const set_term getInputVariables() const {return set_term();}
	virtual const set_term getOutputVariables() const { return set_term(); }
	virtual void setTypeConvs(vector<TypeConv>& typeConvs) {}
	/*************************************************/

	virtual string toString() const {ostringstream s; print(s); return s.str();}

protected:
	vector<Term*> terms;
};

using AtomTable = hashSet<Atom> ;
///Vector of Atom
//using AtomVector = vector<Atom*>;
///Vector used for the recursion
using AtomHistoryVector = HistoryVector<Atom*>;


///An unordered multimap of Atom
using Multimap_Atom = unordered_multimap<index_object, Atom*>;



}}

#endif /* ATOM_H_ */

