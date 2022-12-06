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
 * BackTrackingGrounder.h
 *
 *  Created on: Jan 27, 2015
 *      Author: davide
 */

#ifndef SRC_GROUNDER_GROUND_BACKTRACKINGGROUNDER_H_
#define SRC_GROUNDER_GROUND_BACKTRACKINGGROUNDER_H_

#include "ProgramGrounder.h"
#include "../table/AdvancedArray.h"

namespace DLV2 {
namespace grounder {

/// If the firstMatch on table has not been called
#define NO_MATCH 0
#define MATCH 1

///Information used in the function ground cartesian product. For each atom
struct AtomCartesianInfo{

	struct TableInfo{
		TableInfo(AtomHistoryVector* table,	unsigned start,unsigned end):table(table),start(start),end(end){}
		AtomHistoryVector* table;
		unsigned start;
		unsigned end;
		~TableInfo(){}
	};
	AtomCartesianInfo():table(0),indexTable(0){}

	void addTable(AtomHistoryVector* vec,unsigned start,unsigned end){
		tables.emplace_back(vec,start,end);
	}

	//vector of iterators  search
	vector<TableInfo> tables;
	// Index of tables that specifies the current table
	unsigned table;
	// Index in tables[table] that specifies the current atom in the table
	unsigned indexTable;

	~AtomCartesianInfo(){}
};

/*
 *  Simple backtrack algorithm to ground a rule
*/
class BackTrackingGrounder : public ProgramGrounder {
public:
	BackTrackingGrounder():ProgramGrounder(),currentRule(0),index_current_atom(0),ground_rule(0),callFoundAssignment(0),direction(1){};
	virtual ~BackTrackingGrounder() {
		for(auto atom:templateSetAtom) {if(atom!=nullptr){atom->deleteAtoms(); delete atom;}}
		for(auto atom:groundTemplateAtomHead) delete atom;
		if(ground_rule!=0)
			ground_rule->deleteGroundRule();

	};

	bool findGroundMatch();

protected:
	virtual bool match();
	virtual bool next();
	virtual bool back();
	virtual void inizialize(Rule* rule, unordered_set<index_object>* componentPredicateInHead);
	virtual bool foundAssignment();

	/// Call the first match with the current atom
	virtual bool firstMatch();

	/// Call the next match with the current atom
	virtual bool nextMatch();

	///Ground an aggregate atom and return true if math false otherwise
	virtual bool groundAggregate();

	///Ground a choice in the head of the rule
	virtual void groundChoice(bool& find_new_true_atom,bool& ground_new_atom);

	///Ground a choice in the head of the rule
	virtual void groundChoiceNatively(bool& find_new_true_atom, bool& ground_new_atom);

	///Find the bind variable on rule
	void findBindVariablesRule();

	///Determine the table to search for each atom in the body
	void findSearchTables();

	/// Remove the variable in the current assignment which are in bind_variables
	virtual void removeBindValueFromAssignment(const vector<index_object>& bind_variables);

	/// Generate the template atom of current atom (substitute at the current atom the current assignment)
	inline void generateTemplateAtom(){
		currentRule->getAtomInBody(index_current_atom)->ground(current_assignment,templateSetAtom[index_current_atom]);
	}

	//Delete the atom at the given position and substitute it with the given atom at that position
	void substiteInGroundRule(unsigned position, Atom* new_atom) {
		Atom* atom=(ground_rule->getAtomInBody(position));
		if(atom!=nullptr && ((atom->isClassicalLiteral() && atom->isNegative()) || atom->isAggregateAtom()))
			delete atom;
		ground_rule->setAtomInBody(position,new_atom);
	}

	virtual bool isCartesianProductRule(Rule *r);
	virtual bool groundCartesian(Rule *r,unordered_set<index_object>* componentPredicateInHead);

	///Find the builtin that can be evaluated while match an atom
	virtual void findBuiltinFastEvaluated();

	///Find all match information for each atom in the body of the rule
	void setAtomsMatch();

	/// Current rule to be grounded
	Rule* currentRule;
	/// Position of the atom currently under evaluation
	int index_current_atom;
	/// Ground rule built by the grounding process
	Rule* ground_rule;

	/// Is 1 if a valid assignment has been found
	bool callFoundAssignment;
	/// The direction of the grounding process: 1 if forward, 0 if backward
	bool direction;

	/// Current assignment produced
	var_assignment current_assignment;

	/// Vector of current template atoms: partially ground atoms, in which bound variables have been substituted according to the current assignment
	vector<Atom*> templateSetAtom;

	/// Current id of first match for grounding rule
	/// Vector of the atom and vector of pair : table to search and id of firstMatch (if is NOMATCH call first else next)
	vector<vector<pair<unsigned,bool>>> current_id_match;
	/// Map of id of the atom and table of current searching (iterator of vector in current_id_match)
	vector<unsigned> current_id_match_iterator;

	/// Bind variables for each atom in the current rule
	vector<vector<index_object>> atoms_bind_variables;

	vector<vector<vector<index_object>>> boundTermsInAtoms;

	/// Positions of bound atoms
	vector<bool> is_bound_atom;

	/// The size is >0 if the current ground rule has at least a negative atom that an potentially be an undef atom (an atom without a valid index)
	vector<unsigned> atomsPossibleUndef;

	///Map each variable to its binder atom
	AdvancedArray<int,ARRAY_SIZE> variablesBinder;

	///Vector of the size of the body. If the position of this vector is true then the builtin
	/// in the rule with this position is already evaluated
	vector<bool> builtAlreadyEvaluated;

	vector<vector<Atom*>> matchBuiltin;

	///This vector contains a kind of template atoms for head atoms
	vector<Atom*> groundTemplateAtomHead;

	vector<vector<unsigned>> indexingArguments;

	/// Output variables in each atom
	/// (if its predicate is solved in the corresponding vector are present the positions of the output variable)
	/// (if either the predicate is solved or all the terms are of output the corresponding vector is empty)
	vector<vector<unsigned>> outputVariablesInAtoms;

	virtual void createAtomSearchersForPredicateBody(unsigned position, unsigned atomPos, Predicate* predicate, unordered_set<index_object>* componentPredicateInHead);
	virtual void createAtomSearchersForPredicateHead(unsigned position, unsigned choiceElementPos, Predicate* predicate, unordered_set<index_object>* componentPredicateInHead,bool firstAtom);

	void groundChoiceFirstAtom(Atom* firstAtom, unsigned i, bool& ground_new_atom, Atom* ground_choice);

#ifdef TRACE_ON
	/// Print the current assignment
	void printAssignment();
#endif

        void findVariablesMustList();

	vector<bool> headAtomsWithTheSamePredicate;
	vector<bool> newAtomsInHead;


        ///Contains the local index of variable that must be a list
        vector<unsigned> variableMustList;

private:
	void findBoundTerms(unsigned int index_current_atom, unsigned position, Atom* current_atom);
	void setIndexingStructureInHeadAndBody(unsigned position, unsigned atomPos, PredicateExtension* predicateExtension, Predicate* predicate, unordered_set<index_object>* componentPredicateInHead, unsigned table);
	///Utility function for creating atoms Match information of atoms
	void createMatchInfoClassicalLiteral(unsigned int index_current_atom, Atom* current_atom);
	void onGroundRuleToPrint(const vector<bool>& newAtomsInHead);
};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_GROUND_BACKTRACKINGGROUNDER_H_ */
