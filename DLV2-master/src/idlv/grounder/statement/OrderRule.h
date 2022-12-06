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
 * OrderRule.h
 *
 *  Created on: May 28, 2015
 *      Author: jessica
 */

#ifndef SRC_GROUNDER_STATEMENT_ORDERRULE_H_
#define SRC_GROUNDER_STATEMENT_ORDERRULE_H_

#include "../atom/AggregateAtom.h"
#include <list>

namespace DLV2 {
namespace grounder {

class PartialOrders {
public:
	PartialOrders(Rule* rule): rule(rule){};
	void checkIfPresentPartialOrders();
private:
	Rule* rule;
	void containsFunctionalTerms(Atom* atom1,Atom* atom2);
	void containsExtAtom(Atom* atom1, Atom* atom2);
	void containsABuiltIAssignmentVariable(Atom* atom1,Atom* atom2);
};

class OrderRule {
private:
	Rule* rule;
	set_term safeVariables;
	list<unsigned> builtInAtoms;
	list<unsigned> negativeAtoms;
	list<unsigned> positiveAtoms;
	list<unsigned> aggregatesAtoms;
	list<unsigned> positiveAtomsToBeBound;
	list<unsigned> extAtoms;
	unordered_map<unsigned,set_term> mapAtomsVariables;
	/// A map in which for each positive classical literal are stored variables that must be bound
	/// (for example variables appearing in arith terms)
	unordered_map<unsigned,set_term> mapPositiveAtomsBoundVariables;
	vector<Atom*> orderedBody;
	map_term<unsigned> mapVariablesAtoms;
	vector<unordered_set<unsigned>> bindAtomsDependency;
	set_term variablesInRule;
	vector<Atom*> rewBuiltins;

	//Utility methods
	void computeAtomsVariables();
 	void addSafeVariablesInAtom(Atom* atom, unsigned pos);
 	void addSafeVariablesInAtom(set_term& variables, unsigned pos,unsigned bodypos);
	void unlockAtoms(list<unsigned>& atoms);
	bool lookForVariablesUnsafe(set_term& variables,Atom* atom, list<unsigned>::iterator it, vector<list<unsigned>::iterator>& atomsUnlocked);
	void checkBuiltInSafety(bool& firstSafe, Term* firstTerm,Term*& bindVariable);
	void foundAnAssigment(Atom* atom, Term* bindVariable, unsigned pos);
	bool checkHeadSafety();
	bool checkWeakSafety();
	vector<Atom*> rewriteArith(Atom* current_atom, unordered_map<Term*, Term*, IndexForTable<Term>, IndexForTable<Term> >& arithRewrited);
	bool unlockAtomWithArith(list<unsigned>& atoms);

public:
	OrderRule(Rule* r);
	bool order();
	const vector<Atom*>& getRewBuiltins(){return rewBuiltins;}
	vector<pair<unsigned int, Atom*>> getAtomsFromWhichDepends(unsigned atom_position) const;

	const set_term& getSafeVariables() const {
		return safeVariables;
	}
};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_STATEMENT_ORDERRULE_H_ */
