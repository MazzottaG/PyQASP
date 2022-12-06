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
 * ChoiceElement.h
 *
 *  Created on: 04/ago/2014
 *      Author: tesi2
 */

#ifndef CHOICEELEMENT_H_
#define CHOICEELEMENT_H_


#include <vector>
#include "Atom.h"
#include "Predicate.h"
#include "algorithm"

using namespace std;

namespace DLV2{

namespace grounder{

class Atom;


/// This class represents a choice element

class ChoiceElement {
public:
	//Default constructor
	ChoiceElement() {};

	///Getter for choice element
	const vector<Atom*>& getChoiceElement() const {return choiceElement;}
	///Setter for choice element
	void setChoiceElement(const vector<Atom*>& choiceElement) {this->choiceElement = choiceElement;}
	///This method returns the size of the vector of atoms
	unsigned int getSize() const {return choiceElement.size();}
	/** This method returns the atom in the position i
	 * @retval a if the position i is valid, and a is the atom in that position
	 * @retval null if the position i is not valid
	 */
	Atom* getAtom(unsigned int i) const {if(i<choiceElement.size()) return choiceElement[i]; return 0;};
	///Get the classical atom
	Atom* getFirstAtom() const {return choiceElement[0];}
	///Get the naf atoms and insert in atoms
	void getNafAtoms(vector<Atom*>& atoms){
		if(choiceElement.size()<2) return ;
		atoms.insert(atoms.end(),choiceElement.begin()+1,choiceElement.end());
	}
	//Add an atom to the choice element
	void add(Atom* atom){choiceElement.push_back(atom);}
	void add(const vector<Atom*>& atoms) {choiceElement.insert(choiceElement.end(),atoms.begin(),atoms.end());}

	///Return the predicates of the first atom and the naf literals in the choice element
	set_predicate getPredicates() const;

	///Return the predicates of naf literal in choice element
	set_predicate getPredicateInNaf() const;

	///Return the positive predicates of naf literal in choice element
	set_predicate getPredicatePositiveInNaf() const;

	///Return the negative predicates of naf literal in choice element
	set_predicate getPredicateNegativeInNaf() const;

	///Return the variable in naf literal
	void getVariableInNaf(set_term& terms) const;

	bool haveOnlyEqualBuiltin() const;

	bool isBodyChoiceIsSolved() const;

	bool operator==(const ChoiceElement& choiceElement)const;

	bool hasNegativeAtoms() const;
        
	void deleteAtoms();
	///Destructor
	~ChoiceElement(){};

	ChoiceElement* clone();

private:

	///Vector containing the atoms that compose the choice element
	/*
	 * Notice that the first atom must be a classical atom, while the remaining ones must be naf literals,
	 * so either classical literals or built-in atoms FIXME
	 */
	vector<Atom*> choiceElement;
};

};

};

#endif /* CHOICEELEMENT_H_ */
