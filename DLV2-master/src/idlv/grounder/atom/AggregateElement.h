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
 * AggregateElement.h
 *
 *  Created on: 08/mar/2014
 *      Author: Jessica
 */

#ifndef AGGREGATEELEMENT_H_
#define AGGREGATEELEMENT_H_


#include <vector>
#include "Atom.h"
#include "../../util/Constants.h"
#include "../term/Term.h"
#include "../atom/Predicate.h"


using namespace std;

namespace DLV2{

namespace grounder{

class Atom;

/// This class represents an aggregate element
class AggregateElement {

	public:
		//Default constructor
		AggregateElement() {}

		AggregateElement(Atom * atom,vector<Term*>& terms):terms(terms) {nafLiterals.push_back(atom);}

		AggregateElement* clone();

		///Getter for terms
		const vector<Term*>& getTerms() const {return terms;}
		///Setter for terms
		void setTerms(const vector<Term*>& terms) {this->terms = terms;}
		///Get the i-th term
		Term* getTerm(unsigned i) const {return terms[i];}
		///Getter for naf literals
		const vector<Atom*>& getNafLiterals() const {return nafLiterals;}
		///Getter for naf literal
		Atom* getNafLiteral(unsigned i) const {return nafLiterals[i];}
		///Setter for naf literals
		void setNafLiterals(const vector<Atom*>& nafLiterals) {this->nafLiterals = nafLiterals;}
		///Add term in terms
		void addTerm(Term* term){terms.push_back(term);}
		///Add Naf literal in nafLiterals
		void addNafLiterals(Atom* atom){nafLiterals.push_back(atom);}
		/// Remove all Naf literals
		void clearNafLiterals();
		/// Remove all terms
		void clearTerms(){terms.clear();}

		///Return true if all the aggregate term in the element compare in the set shared_variable
		bool isAllAggregateTermShared(set_term& shared_variable)const;

		void getPredicates(set_predicate& predicates)const;

		void deleteAtoms();

		set_term getSafeVariable();

		set_term getUnsafeVariable();

		unsigned getNafLiteralsSize() const {return nafLiterals.size();}

		//Destructor
		~AggregateElement();

		bool areAggregationTermsSafe()const;

		bool operator==(const AggregateElement& element)const;

		void getUnsolvedPredicateVariable(set_term& vars);

		void getVariableInNaf(set_term& vars) const;
		vector< Term* > getVectorVariableInNaf() const;
		void getVariableAggregationTerms(set_term& vars) const;

	private:
		///Vector of terms
		vector<Term*> terms;
		///Vector of naf literals
		// Notice that the atoms must be naf literals, so either classical literals or built-in atoms
		vector<Atom*> nafLiterals;
};

};

};

#endif /* AGGREGATEELEMENT_H_ */
