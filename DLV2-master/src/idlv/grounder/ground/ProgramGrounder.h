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
 * ProgramGrounder.h
 *
 *  Created on: 10/apr/2014
 *      Author: Davide
 */

#ifndef PROGRAMGROUNDER_H_
#define PROGRAMGROUNDER_H_

#include <vector>

#include "../table/PredicateTable.h"
#include "../table/PredicateExtension.h"
#include "../table/TermTable.h"
#include "StatementDependency.h"
#include "NonGroundSimplifier.h"
#include "../output/NumericOutputBuilder.h"
#include "../exception/ConstrainException.h"
#include "../statement/RuleStatistics.h"
#include "../statement/InputRewriter.h"
#include "PropagatorsHeuristicsHolder.h"


using namespace std;

namespace DLV2{

namespace grounder{

/**
 * @brief This class manages and executes the grounding process.
 *
 * The grounding of rule process is a template method.
 */
class ProgramGrounder {

public:
	/** Constructor
	 *	@param pt A pointer to the predicate table
	 *	@param it A pointer to the instances table
	 *	@param st A pointer to the statement dependency table
	 *	Since the term table is a singleton, it is not needed to give it as a parameter.
	 */
	ProgramGrounder() :
		predicateTable(PredicateTable::getInstance()), predicateExtTable(PredicateExtTable::getInstance()),
		statementDependency(StatementDependency::getInstance()), termsMap(TermTable::getInstance()),outputBuilder(OutputBuilder::getInstance()),iteration(0),
		rstats(RuleStatistics::getInstance()),printRuleTime(Options::globalOptions()->getRuleTime()),printStats(Options::globalOptions()->getPrintGroundStats()!=0){
		Options::globalOptions()->splitOption();
	};

	/// This method executes the overall grounding process
	virtual void ground();

	/// This method executes the grounding of a rule
	/// @param r The rule to be grounded
	/// @parm isRecursive If the rule is recursive
	/// @param firstIteraction If it is the first iteration or not (useful for recursive rules)
	virtual bool groundRule(Rule* r, unordered_set<index_object>* componentPredicateInHead=nullptr);

	///Printer method
	void print() {	statementDependency->print();};

	/// Printer method for facts
	void printFact() {	predicateExtTable->print(FACT);};

	/// Return the InstanceTable
	PredicateExtTable* getInstanceTable() {	return predicateExtTable;};

	/// Return the StatementDependency
	StatementDependency* getStatementDependency() {	return statementDependency;};

	/// Return the PredicateTable
	PredicateTable* getPredicateTable() {return predicateTable;};

	bool isNotEmptyPredExt(Predicate* pred,unsigned table);
	bool isNotEmptyPredExt(Predicate* pred,unsigned table,SearchType type);



	///Destructor
	virtual ~ProgramGrounder();

protected:
	///A pointer to the predicate table
	PredicateTable* predicateTable;
	///A pointer to the instances table
	PredicateExtTable* predicateExtTable;
	///A pointer to the statement table
	StatementDependency* statementDependency;
	///A pointer to the terms table
	TermTable* termsMap;
	///Output builder
	OutputBuilder *outputBuilder;


//	///The set of grounder rules
//	GroundedRules groundedRule;
//	/// Manage the output and simplification
//	ProgramEvaluator evaluator;


	/// For each predicate in the current rule this vector stores the tables of insert for the atom in head and
	/// searching table for the predicate in the body
	vector<vector<pair<unsigned,SearchType>>> predicate_searchInsert_table;

	/// For each predicate in the current rule this vector stores the atom searchers of insertion and look-up for head atoms and
	/// the atom searchers of look-up for body atoms
	vector<vector<vector<IndexingStructure*>>> predicate_searchInsert_atomSearcher;

	///The NonGroundSimplifier object
	NonGroundSimplifier nonGroundSimplificator;

	void swapInDelta(Rule* r,set_predicate &predicateEvaluated);

	/* Initialize the vector of predicate_searchInsert_table with:
	 * 		Atom in head: if is recursive predicate DELTA table else NOFACT table
	 * 		Atom in body: FACT and NOFACT table
	 * 		Returns true if there is at least one atom in the body with all tables to search empty.
	 * 		In this case the rule has not to be grounded.
	*/
	bool inizializeSearchInsertPredicate(Rule* rule,unordered_set<index_object>& componentPredicateInHead);
	bool inizializeSearchInsertPredicate(Rule* rule);
	/// Based on the sequence of searching table set the table to search and insert for grounding process
	bool nextSearchInsertPredicate(Rule* rule,unordered_set<index_object>& componentPredicateInHead,unsigned token,const vector<unsigned>& originalOrderBody);

	/// Initialization of grounding rule r
	virtual void inizialize(Rule* rule, unordered_set<index_object>* componentPredicateInHead) = 0;
	/// Return true if exist a match with the current atom and current assignment
	virtual bool match() = 0;
	/// Next atom in the rule, if is last return false else true
	virtual bool next() = 0;
	/// Found an assignment for the rule, return true if is grounded new knowledge
	virtual bool foundAssignment() = 0;
	/// Previous atom in the rule, if not exist return false
	virtual bool back() = 0;

	/// A vector of ground rules containing in their bodies atoms that can potentially be undef atoms.
	vector<Rule*> rulesWithPossibleUndefAtoms;
	/// For each rule in rulesWithPossibleUndefAtoms, this vector store a vector
	/// containing the positions of the atoms in that rule that can potentially be undef atoms.
	vector<vector<unsigned>> atomsPossibleUndefPositions;

	///This method iterates the rulesWithPossibleUndefAtoms, in order to see whether the atoms possible undef have been
	///derived or not:
	/// 	-If it was derived, then its index is set to the index of the derived atom
	/// 	-If it was derived as true, then (since is negative) the atom is false so the rule can be simplified
	///		-If it was not derived, then (since is false) the atom is true, so it can be simplified
	void substituteIndicesInRulesWithPossibleUndefAtoms();

	virtual bool isCartesianProductRule(Rule *r){return false;}
	virtual bool groundCartesian(Rule *r,unordered_set<index_object>* componentPredicateInHead)=0;

	///This method creates a default atom searcher for each predicate occurring in the head and the body of the current rule.
	///In particular it creates an atom searcher on FACT and NOFACT tables for every predicate,
	///and for recursive predicates it creates an atom searcher also for DELTA and NEXTDELTA tables.
	void setDefaultAtomSearchers(Rule* r, unordered_set<index_object>* componentPredicateInHead);

	///Utility method for setDefaultAtomSearchers
	virtual void createAtomSearchersForPredicateBody(unsigned position, unsigned atomPos, Predicate* predicate, unordered_set<index_object>* componentPredicateInHead){};
	///Utility method for setDefaultAtomSearchers
	virtual void createAtomSearchersForPredicateHead(unsigned position, unsigned choiceElementPos, Predicate* predicate, unordered_set<index_object>* componentPredicateInHead, bool firstAtom){};

	///For each recursive predicate in the set update the indices corresponding delta and nf table
	///This function must be called at the beginning of iteration
	void updateIndiciesHistoryTable(unordered_set<unsigned> recursivePredicate){
		trace_action_tag(recursion,1,cerr<<"ITERATION "<<iteration<<endl;);

		for(auto p:recursivePredicate){
			auto predExt=predicateExtTable->getPredicateExt(p);
			trace_action_tag(recursion,1,cerr<<"Update predicate "<<predExt->getPredicate()->getName()<<endl;);
			predExt->updateIndexesTable(NOFACT,iteration);
			predExt->updateIndexesTable(FACT,iteration);
		}
	}

	void addTrueNegationConstraints();

	//Iteration of the current instantiation
	unsigned iteration;


	///Statistic for each rule grounded
	RuleStatistics* rstats;

	///Print the time spent to ground each single rule
	bool printRuleTime;
	///Print the statistics of each rule grounded
	bool printStats;

private:
	///Print the program rule
	void printProgram(const vector<vector<Rule*> >& exitRules,const vector<vector<Rule*> >& recursiveRules);
	bool inizializeSearchInsertPredicateBody(Rule* rule);
	void findRecursivePredicatesInComponentRules(const unordered_set<index_object>& componentPredicateInHead, vector<unsigned>& recursivePredicatesPositions, Rule* rule, vector<unsigned >& orderedBody);
	void orderPositiveAtomsBody(vector<unsigned>& originalOrderBody,Rule* rule, unordered_set<index_object>* componentPredicateInHead);
	bool orderPositiveAtomsBody(Rule* rule);
	void projectAtomsInRule(vector<Rule*>& exitRules,vector<Rule*>& recursiveRule,vector<Rule*>& constraint,unordered_set<index_object>& recursivePred,BaseInputRewriter& rewriter);
	bool performDecompositionOfExitRule(Rule* rule);
	bool checkIfDecompositionIsConvenient(Rule* r);
	void groundExitRule(Rule* r);
	void applyDecompositionRewriting(const vector<Rule*>& rules, vector<Rule*>& newRules, vector<Rule*>& changedRules);

	void createPropagatorForWasp();
	void expandForWasp(wasp_prop& wp);
};

};

};

#endif /* PROGRAMGROUNDER_H_ */
