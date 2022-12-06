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
 * StatementDependency.h
 *
 *  Created on: 03/apr/2014
 *      Author: Davide
 */

#ifndef STATEMENTDEPENDENCY_H_
#define STATEMENTDEPENDENCY_H_

#include <unordered_map>
#include <unordered_set>
#include <list>
#include <climits>
#include <fstream>


#include "../statement/Rule.h"
#include "../table/PredicateTable.h"
#include "../../util/Graph.h"
#include "../../../common/Options.h"
#include "../table/IdGenerator.h"
#include "StatementAtomMapping.h"

using namespace std;
namespace DLV2{

namespace grounder{

/*
 *  This class represents the dependency graph of the program
 */
class DependencyGraph{
public:
	DependencyGraph():stratifiedGraph(false){};

	/// This method adds the atoms in the rule in the dependency graph
	void addInDependency(Rule* rule);

	/// This method computes the strong components of the dependency graph.
	/// It also put for each predicate the relative component in the map of components.
	void calculateStrongComponent(unordered_map<string, unsigned int> &componentDepependency);

	// It  calculate the predicate with negation recursive not stratified
	void calculateUnstratifiedPredicate(unordered_set<index_object>& predicateUnstratified);

	/// This method adds an edge in the dependency graph between the two predicate given
	void addEdge(Predicate* pred_body, Predicate* pred_head, int weight);



	///Printer method (on standard output)
	void print(){
		stringstream ss;
		depGraph.printDot("DependencyGraph",ss);
		ofstream myfile;
		myfile.open ("Dependency-Graph.dot");
		myfile << ss.str();
		myfile.close();
	}

	const unordered_map<string, Predicate*>& getLabelPredicate() const {
		return labelPredicate;
	}


private:
	/// The Dependency Graph
	UntypedGraph<string> depGraph;
	/// The Stratified Graph
	UntypedGraph<string> stratifiedGraph;
	///Map of label used in the graph and predicate object
	unordered_map<string,Predicate*> labelPredicate;
};

/*
 *   This class represents the component graph of the program
 */
class ComponentGraph{
public:
	ComponentGraph(){};

	/// Create component graph
	///@param depGraph The dependency graph
	///@param statementAtomMapping The dependency correlation between predicates and rules
	void createComponent(DependencyGraph &depGraph,StatementAtomMapping &statementAtomMapping);

	/// This method adds an edge with the specified weight in the component graph between the two predicate given
	void addEdge(Predicate* pred_body,Predicate* pred_head,int weight);
	/// This method compute a single possible ordering among components
	void computeAnOrdering(list<unsigned int>& componentsOrdering);
	/// Print the ordering of component
	void printComponentsOrdering(list<unsigned>& componentsOrdering);
	/// This method compute all possible orderings among components
	void computeAllPossibleOrdering(vector<vector<unsigned>>& componentsOrderings);


	///Getter for the components mapping
	unordered_map<index_object, unsigned int> getComponent() {
		unordered_map<index_object, unsigned int> predCompMap;

		for(auto it:componentDependency)
			predCompMap.insert({this->labelPredicate[it.first]->getIndex(),it.second});
		return predCompMap;
	}

	///Return true is the program is not stratified
	bool hasPredicateNotStratified(){return predicateUnstratified.size()>=1;}

	///Return true if the predicate is not stratified
	bool isPredicateNotStratified(index_object predicate){
		return predicateUnstratified.count(predicate);
	}

	void print(){
		auto getNameComp=[](unsigned c,unordered_map<string,unsigned>& cd){
			string name="";
			for(auto it:cd)
					name+=it.first+" ";

			return name;
		};
		UntypedGraph<string> stringGraph;
		auto edges=compGraph.getEdges();
		for(auto& e:edges){
			stringGraph.addEdge(getNameComp(get<0>(e),componentDependency),getNameComp(get<1>(e),componentDependency),get<2>(e));
		}
		stringstream ss;
		stringGraph.printDot("ComponentGraph",ss);
		ofstream myfile;
		myfile.open ("Component-Graph.dot");
		myfile << ss.str();
		myfile.close();
	}

private:
	/// The Component Graph
	UntypedGraph<unsigned> compGraph;
	///This unordered map maps each predicate to its relative component
	unordered_map<string,unsigned> componentDependency;

	unordered_set<index_object> predicateUnstratified;

	///Map of label used in the graph and predicate object
	unordered_map<string,Predicate*> labelPredicate;

	///This method computes an ordering among components if the component graph is cyclic
	void recursive_sort(list<unsigned int>& componentsOrdering);

};

/**
 * 	This class manages the overall creation of the dependency graph
 * 	and the component graph
 */
class StatementDependency {
public:


	/// This method determines the predicates appearing in the rule,
	/// stores their mapping, and updates the dependency graph properly according this mapping
	void addRuleMapping(Rule *r);

	void addAtomMappingAndSetEdb(Rule *r);

	void simplifyMagicRules();


	/// This method creates the dependency graph
	void createDependencyGraph();
	/// This method creates the components graph
	void createComponentGraph();

	/// This method creates the components graph and compute an ordering for the components.
	/// It also classifies the rules for each components as exit or recursive.
	/// An rule occurring in a component is recursive if there is a predicate belonging
	/// to the component in its positive body, otherwise it is said to be an exit rule.
	/// @param exitRules A vector containing at the ith position a vector containing the exit rules for the ith component
	/// @param recursiveRules A vector containing at the ith position a vector containing the recursive rules for the ith component
	/// @param componentPredicateInHead A vector containing at the ith position an unordered set containing
	/// @param stratifiedPred The set of predicate appearing as not statified negated
	/// the predicates in the head of the recursive rules of the ith component
	/// @param constraintRules A vector containing at the ith position a vector of constraints for the ith component
	/// @param remainedConstraint A vector of constraint that not appear in any component
	void createComponentGraphAndComputeAnOrdering(vector<vector<Rule*>>& exitRules, vector<vector<Rule*>>& recursiveRules,vector<unordered_set<index_object>>& componentPredicateInHead,
			vector<vector<Rule*>>& constraintRules,vector<Rule*>& remainedConstraint,vector<unsigned>& compID);

	void addAdditionalPotentiallyRecursiveRules(unsigned compId,const vector<Rule*>& newRules, vector<Rule*>& exitRules, vector<Rule*>& recursiveRules,
			unordered_set<index_object>& componentPredicateInHead);

	void addAdditionalExitRulesOrConstraints(const vector<Rule*>& newRules);

	void substituteRules(const vector<Rule*>& newRules, vector<Rule*>& rulesContainer);
	void substituteRule(Rule* newRule, Rule* oldRule);

	/// Return true if the predicate is recursive with negation not stratified
	bool isPredicateNotStratified(index_object predicate){return compGraph.isPredicateNotStratified(predicate);};


	/// This method returns the number of rules in the program
	unsigned int getRulesSize(){return rules.size();}
	/// Getter for the ith rule
	Rule* getRule(int i) {return rules[i];};

        const vector<Rule*>& getRules(){return rules;};
        const vector<Rule*>& getConstraints(){return constraints;};
        const vector<Rule*>& getWeakCs(){return weak;};

	/// This method returns the number of constraints in the program
	unsigned int getConstraintSize(){return constraints.size();}
	/// Getter for the ith constraint
	Rule* getConstraint(int i) {return constraints[i];}

	vector<Rule*>::iterator getBeginRules(){return rules.begin();}
	vector<Rule*>::iterator getEndRules(){return rules.end();}
	void eraseRule( vector<Rule*>::iterator itR ){ clearRule(*itR); rules.erase(itR); }
	vector<Rule*>::iterator getBeginConstraints(){return constraints.begin();}
	vector<Rule*>::iterator getEndConstraints(){return constraints.end();}

	const vector<Rule*>& getWeakContraints(){return weak;}

	Rule* getWeakContraint(unsigned i){return weak[i];}
	unsigned getWeakContraintsSize(){return weak.size();}

	bool isOnlyInHead(index_object predicate){
		return (!statementAtomMapping.isInBody(predicate));
	}

	bool isOnlyInBody(index_object predicate){
		return (!statementAtomMapping.isInHead(predicate) && statementAtomMapping.isInBody(predicate));
	}

	void addQueryAtom(Atom* query, bool ground){
		if(ground)
			query->setIndex(IdGenerator::getInstance()->getTheAtomReservedId());
		this->query.push_back(query);
	}

	///Printer method
	void print();

	virtual ~StatementDependency();

	static void freeInstance(){
		if(statementDependency != nullptr){
			delete statementDependency;
			statementDependency = nullptr;
		}
	}

	void clearRule(Rule *r){
		r->free();
		delete r;
	}

	void aggregateFound(){
		hasAggregate=true;
	}

	void choiceFound(){
		hasChoice=true;
	}

	void negativeAtomFound(){
		hasNegativeAtom=true;
	}

	void disjunctionFound(){
		hasDisjunction=true;
	}

	void externalLiteralFound(){
		hasExternalLiterals=true;
	}

	vector<Atom*>& getQueryAtoms(){
		return query;
	}

	bool queryGround(){
		return isQueryGround;
	}

	bool isQueryEvaluable(){
		return !(hasChoice || hasDisjunction || compGraph.hasPredicateNotStratified() || query.empty());
	}

	static StatementDependency* getInstance();

	unsigned getStatementsSize() const{
		return constraints.size()+weak.size()+rules.size();
	}

	bool isAppliedMagicRewriting() const {
		return appliedMagicRewriting;
	}

	void assignRuleIndex(Rule* r);

	///Return true is the program is stratified and not disjunctive
	bool isStratifiedAndNotDisjuctive(){
		return !hasDisjunction && !compGraph.hasPredicateNotStratified();
	}

	Atom* getGroundQueryAtom(){
		if(!isQueryGround || query.empty() || query.size()>1)
			return nullptr;
		return query.front();
	}

	StatementAtomMapping& getStatementAtomMapping() {
		return statementAtomMapping;
	}

	bool isRelevant(Atom* atom);
	bool isRelevant(Predicate* p);

private:

	static StatementDependency* statementDependency;

	StatementDependency() :
			hasAggregate(false), hasChoice(false), hasNegativeAtom(false),
			hasExternalLiterals(false), isQueryGround(true), hasDisjunction(false),
			createdDepedencyGraph(false),appliedMagicRewriting(false) {}

	/// The Dependency Graph
	DependencyGraph depGraph;
	/// The Component Graph
	ComponentGraph compGraph;
	/// The dependency correlation between predicates and rules
	StatementAtomMapping statementAtomMapping;
	/// The vector of rules composing the program
	vector<Rule*> rules;
	/// The vector of contraints composing the program
	vector<Rule*> constraints;
	/// The vector of weak contraints composing the program
	vector<Rule*> weak;
	///Vector of query atoms
	vector<Atom*> query;

	bool hasAggregate;

	bool hasChoice;

	bool hasNegativeAtom;

	bool hasExternalLiterals;

	bool isQueryGround;

	bool hasDisjunction;

	bool createdDepedencyGraph;

	/// This method checks if a rule is a recursive rule or an exit rule
	/// An rule occurring in a component is recursive if there is a predicate belonging
	/// to the component in its positive body, otherwise it is said to be an exit rule.
	/// @param rule A rule
	/// @param component The component to which the rule belongs
	/// @retval true If the rule is an exit rule
	/// @retval false If the rule is a recursive rule
	bool checkIfExitRule(unsigned int component, Rule* rule,unordered_map<index_object, unsigned int>& components);

	bool HandleQuery(unsigned &queryConstraints);
	bool magic();


	bool appliedMagicRewriting;

	set_predicate usefullPredicateForQueryAnswering;
};

}

}

#endif /* STATEMENTDEPENDENCY_H_ */
