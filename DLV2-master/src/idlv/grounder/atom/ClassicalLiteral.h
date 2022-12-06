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
 * ClassicalLiteral.h
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#ifndef CLASSICALLITERAL_H_
#define CLASSICALLITERAL_H_

#include "Atom.h"
#include "../table/TermTable.h"

using namespace std;

namespace DLV2{

namespace grounder{

/**
 * This class implements a classical atom extending atom
 */

class ClassicalLiteral : public Atom {
public:

	/** Constructor
	 * @param predicate set the predicate index
	 * @param terms set the terms vector
	 * @param hasMinus set whether the atom is negated with true negation
	 * @param negative set whether the atom is negated with negation as failure
	 */
	ClassicalLiteral(Predicate* predicate, vector<Term*> &terms, bool hasMinus, bool negative): Atom(terms), predicate(predicate), hasMinus(hasMinus), negative(negative), fact(false) {};

	ClassicalLiteral(Predicate* predicate, bool hasMinus, bool negative, bool fact): Atom(), predicate(predicate), hasMinus(hasMinus), negative(negative), fact(fact) {};

	ClassicalLiteral(Predicate* predicate, bool hasMinus, bool negative): Atom(), predicate(predicate), hasMinus(hasMinus), negative(negative), fact(false) {};

	Atom* clone() {
		Atom* atom = new ClassicalLiteral(this->predicate,this->hasMinus,this->negative,this->fact);
		atom->setTerms(this->terms);
		atom->setIndex(this->getIndex());
		return atom;
	};

	/** Getter method for the predicate
	 */
	Predicate* getPredicate() const {return predicate;};
	set_predicate getPredicates() const {return set_predicate({predicate});};
	virtual unordered_set<index_object> getPredicatesIndices() const {return unordered_set<index_object>({predicate->getIndex()});};

	virtual bool isClassicalLiteral()const{return true;};

	///Setter method for the predicate
	void setPredicate(Predicate* predicate) {this->predicate=predicate;};
	///Returns true if the atom is negated with true negation
	bool isHasMinus() const {return hasMinus;};
	///Set whether the atom is negated with true negation
	void setHasMinus(bool hasMinus) {this->hasMinus=hasMinus;};
	///Returns true if the atom is negated with negation as failure
	bool isNegative() const {return negative;};
	///Set whether the atom is negated with negation as failure
	void setNegative(bool negative) {this->negative=negative;};

	///This method compute the resulting hash a classical atom using its terms
	size_t hash() ;

	/** @brief Equal-to operator for classical atom
	 * 	@details{Two classical atoms are compared by the predicate and the terms.
	 * 	It is needed in order to compare facts. }
	 */
	bool operator==(const Atom& a) const;

	///This method is used in the printer method @see print()
	//It is static since it is used in Instance class, where just terms are stored instead of the whole classical atom
	static void print(Predicate* predicate,const vector<Term*>& terms,bool negative,bool hasMinus,ostream& stream);

	///Printer Method
	void print(ostream& stream=cout) const;

	/// Substitute the term and return a new Atom with term substituted
	virtual void substitute(var_assignment& substritutionTerm,Atom*& templateAtom);

	virtual bool isFact() const { return fact; }
	virtual void setFact(bool isFact) { this->fact=isFact; }

	//Check if two atoms are completely equal (checking also negation as failure, and strong negation)
	virtual bool equal(const Atom& atom) const;

	///Destructor
	virtual ~ClassicalLiteral() {};

	virtual void substituteVarWithAnonymous(Term* t){
		for(unsigned i=0;i<terms.size();++i){
			if(t->getType()==VARIABLE && t->getIndex()==terms[i]->getIndex())
				setTerm(i,TermTable::getInstance()->getTermAnonymous());
			else if(terms[i]->contain(TermType::FUNCTION))
				terms[i]->substitute(t,TermTable::getInstance()->getTermAnonymous());
		}
	}

private:
	///Index of the predicate
	Predicate* predicate;
	///Negated with true negation
	bool hasMinus;
	///Negated with naf
	bool negative;
	//A boolean to set whether it is a true fact or is undefined
	bool fact;
};

};

};

#endif /* CLASSICALLITERAL_H_ */
