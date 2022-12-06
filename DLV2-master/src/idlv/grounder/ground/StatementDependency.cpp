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
 * StatementDependency.cpp
 *
 *  Created on: 03/apr/2014
 *      Author: Davide
 */

#include "StatementDependency.h"

#include <algorithm>

#include "../../util/Utils.h"

#include "../table/PredicateExtension.h"
#include "../statement/InputRewriter.h"
#include "../../input/InMemoryInputBuilder.h"
#include "MagicSets.h"
#include "RewriteMagic.h"
#include "RewriteMagicExternalSources.h"
#include "../../directives/DirectivesHolder.h"
#include "../../subsumption/SubsumptionChecker.h"
#include "DecompositionMapper.h"
namespace DLV2{


namespace grounder{

StatementDependency* StatementDependency::statementDependency = nullptr;

void DependencyGraph::addInDependency(Rule* r) {
	// Temporary set of predicates
	unordered_set<index_object> head_predicateVisited;
	unordered_set<index_object> body_predicateVisited_positive;
	unordered_set<index_object> body_predicateVisited_negative;


	for (auto head_it = r->getBeginHead(); head_it != r->getEndHead(); head_it++) {
		set_predicate predicates_head=(*head_it)->getPredicates();

		for(auto pred_head:predicates_head){

			// Check if the predicate in the head has been visited
			if ( !head_predicateVisited.count(pred_head->getIndex())) {

				// Set this predicate as visited
				head_predicateVisited.insert(pred_head->getIndex());

	//			// If the rule has no positive predicate in its body, is added a dummy edge in order to not lose this rule
	//			// when the components graph is created
	//			if(r->getPositivePredicateInBody().size()==0)
	//				addEdge(pred_head->getIndex(), pred_head->getIndex(),1);

				bool addPositiveEdge=false;

				for (auto body_it = r->getBeginBody(); body_it != r->getEndBody(); body_it++) {
					// Check if the predicate is positive, otherwise skip it
					if (!(*body_it)->isNegative()) {

						for(auto pred_body:(*body_it)->getPredicates()){
							// Check if the predicate in the head has been visited
							if(!body_predicateVisited_positive.count(pred_body->getIndex()) && !pred_body->isEdb()){
								body_predicateVisited_positive.insert(pred_body->getIndex());
								addEdge(pred_body, pred_head,1);
								addPositiveEdge=true;
							}
						}

					}
					else{

						for(auto pred_body:(*body_it)->getPredicates()){
							// Check if the predicate in the head has been visited
							if(!body_predicateVisited_negative.count(pred_body->getIndex()) && !pred_body->isEdb()){
								body_predicateVisited_negative.insert(pred_body->getIndex());
								if(!(*body_it)->isAggregateAtom())
									addEdge(pred_body, pred_head,-1);
								else{
									//TODO Add edge -1 with aggregate??????
									addEdge(pred_body, pred_head,1);
									addPositiveEdge=true;
								}
							}
						}

					}

				}

				// If the rule has no positive predicate in its body, is added a dummy edge in order to not lose this rule
				// when the components graph is created
				if(!addPositiveEdge){
					labelPredicate.insert({pred_head->getNameArity(),pred_head});
					depGraph.addAVertex(pred_head->getNameArity());
//					addEdge(pred_head, pred_head,1);
				}
			}
			//Set all predicate in the body as unvisited, and then continue with the next atom
			body_predicateVisited_negative.clear();
			body_predicateVisited_positive.clear();
		}
	}

	//Add for each choice element the dependency with the atom in the right of :
	if(r->getSizeHead()>0){
		auto head_atom=r->getAtomInHead(0);
		if( head_atom->isChoice()){
			for(unsigned i=0;i<head_atom->getChoiceElementsSize();i++){
				for(auto pred_body:head_atom->getChoiceElement(i)->getPredicatePositiveInNaf()){
					addEdge(pred_body, head_atom->getChoiceElement(i)->getFirstAtom()->getPredicate(),1);
				}
			}
		}
	}

}



void DependencyGraph::addEdge(Predicate* pred_body, Predicate* pred_head, int weight) {
	auto v1=pred_body->getNameArity();
	auto v2=pred_head->getNameArity();

	labelPredicate.insert({v1,pred_body});
	labelPredicate.insert({v2,pred_head});

	// If add an edge positive with the same predicate not add in stratified
	// because is only for component, to add rule with not positive atom in body
	if(!(pred_body==pred_head && weight>0))
		stratifiedGraph.addEdge(v1,v2,weight);

	if(weight>0)
		depGraph.addEdge(v1,v2,0);

}

void DependencyGraph::calculateStrongComponent(unordered_map<string, unsigned int> &componentDepependency) {

	vector<vector<string>> components=depGraph.calculateStrongComponent();

	for(unsigned i=0;i<components.size();i++)
		for(auto& p:components[i])
			componentDepependency[p]=i;

}


void DependencyGraph::calculateUnstratifiedPredicate(unordered_set<index_object>& predicateUnstratified){

	if(stratifiedGraph.getNumVertices()==0)return ;

	//Calculate the cycle in stratifiedGraph and verify thath not exist recursion and negation, if exist
	// then all the predicate in that cycle are not stratified


	auto edges=stratifiedGraph.getEdges();

	// check recursion in the same rule
	for(auto& e :edges){
		if(get<0>(e)==get<1>(e) && get<2>(e)<0){
			predicateUnstratified.insert(labelPredicate[get<0>(e)]->getIndex());
			labelPredicate[get<0>(e)]->setSolved(false);
		}
	}

	// First build a map that contains for each predicate name the component. Then for each edges in the graph
	// if the vertices in the edge belong to the same component and the weith of the edge is negative, then exist
	// a unstrat negation, then all the predicate in this component are unstratified.
	auto strong=stratifiedGraph.calculateStrongComponent();
	std::unordered_map<string,unsigned> lc;
	for(unsigned i=0;i<strong.size();i++)
		for(auto label:strong[i])
			lc.insert({label,i});

	std::unordered_set<unsigned> compProcessed;
	for(auto& e :edges){
		unsigned c1=lc[get<0>(e)];
		unsigned c2=lc[get<1>(e)];
		if(c1==c2 && !compProcessed.count(c1) && get<2>(e)<0){
			compProcessed.insert(c1);
			for(auto v:strong[c1]){
				predicateUnstratified.insert(labelPredicate[v]->getIndex());
				labelPredicate[v]->setSolved(false);
			}
		}

	}

	//  Set as unstratified predicates all the predicates reachable from other unstratified predicates
	//	for(auto& e :edges){
	//		if(get<2>(e)<0 && !labelPredicate[get<1>(e)]->isEdb()){
	//			predicateUnstratified.insert(labelPredicate[get<1>(e)]->getIndex());
	//			list<index_object> predicatesToConsider;
	//			unordered_set<index_object> predicatesConsidered;
	//			predicatesToConsider.push_back(labelPredicate[get<1>(e)]->getIndex());
	//			while(!predicatesToConsider.empty()){
	//				index_object p=predicatesToConsider.back();
	//				predicatesConsidered.insert(p);
	//				predicatesToConsider.pop_back();
	//				for(auto& e2 :edges){
	//					if(p==labelPredicate[get<0>(e2)]->getIndex()){
	//						predicateUnstratified.insert(labelPredicate[get<1>(e2)]->getIndex());
	//						labelPredicate[get<1>(e2)]->setSolved(false);
	//						if(!predicatesConsidered.count(labelPredicate[get<1>(e2)]->getIndex()) && !labelPredicate[get<1>(e2)]->isEdb())
	//							predicatesToConsider.push_back(labelPredicate[get<1>(e2)]->getIndex());
	//					}
	//				}
	//			}
	//		}
	//	}
}

/*
 *
 *
 *
 *
 * 	END DependencyGraph
 *
 *
 *
 *
 *
 */

void ComponentGraph::addEdge(Predicate* pred_body, Predicate* pred_head, int weight) {
	if (componentDependency[pred_body->getNameArity()] != componentDependency[pred_head->getNameArity()])
		compGraph.addEdge(componentDependency[pred_body->getNameArity()],componentDependency[pred_head->getNameArity()],weight);
}

void ComponentGraph::createComponent(DependencyGraph &depGraph,
		StatementAtomMapping &statementAtomMapping) {
	depGraph.calculateStrongComponent(componentDependency);
	depGraph.calculateUnstratifiedPredicate(predicateUnstratified);
	labelPredicate=depGraph.getLabelPredicate();

	// For each component create a vertex in a graph
	// because when adding a edge the vertex have to be created and if exist one component
	// with a body in a rule with all fact are not skipped

	unordered_set<unsigned int> component_insert;
	for (auto it : componentDependency)
		if(component_insert.insert(it.second).second)
			compGraph.addAVertex(it.second);


	vector<Rule*> rules;
	for (auto it : componentDependency) {

		Predicate* predHead = labelPredicate[it.first];
		index_object pred_head=predHead->getIndex();

		statementAtomMapping.getRuleInHead(pred_head, rules);

		for (Rule *r : rules) {

			for (auto body_it = r->getBeginBody(); body_it != r->getEndBody(); body_it++) {

				bool isPositive = !(*body_it)->isNegative();

				for(auto pred_body:(*body_it)->getPredicates()){
					index_object index_pred_body = pred_body->getIndex();
					// Check if the predicate appear in the head also
					if (statementAtomMapping.isInHead(index_pred_body)) {
						int weight = isPositive;
						if (!isPositive)
							weight = -1;
						addEdge(pred_body,  predHead, weight);
					}
				}


			}

			if(r->getSizeHead()>0){
				auto head_atom=r->getAtomInHead(0);
				if( head_atom->isChoice()){
					for(unsigned i=0;i<head_atom->getChoiceElementsSize();i++){
						for(auto pred_body:head_atom->getChoiceElement(i)->getPredicatePositiveInNaf()){
							index_object index_pred_body = pred_body->getIndex();
							if (statementAtomMapping.isInHead(index_pred_body)) {
								addEdge(pred_body, predHead, 1);

							}
						}
						for(auto pred_body:head_atom->getChoiceElement(i)->getPredicateNegativeInNaf()){
							index_object index_pred_body = pred_body->getIndex();
							if (statementAtomMapping.isInHead(index_pred_body)) {
								addEdge(pred_body, predHead, -1);

							}
						}
					}
				}
			}

		}
		rules.clear();
	}

}



void ComponentGraph::computeAnOrdering(list<unsigned>& componentsOrdering) {

	// If the component graph is not connected, then if there are some components any ordering is valid
	if(compGraph.getNumVertices()>=0 && compGraph.getNumEdges()==0 && componentDependency.size()>0){
		unordered_set<unsigned int> components;
		for(auto pair:componentDependency)
			components.insert(pair.second);
		for(auto it:components)
			componentsOrdering.push_back(it);

		return;
	}

	// If the component graph is a-cyclic is suffices to compute a topological sort to get a valid components ordering
	try {
		vector<unsigned> tpsort=compGraph.calculateTopologicalSort();
		componentsOrdering.insert(componentsOrdering.end(),tpsort.begin(),tpsort.end());
	// Otherwise an not_a_dag exception is thrown, and it is computed an ordering taking into account that the graph is cyclic
	} catch (Graph::topological_exception& e) {
		this->recursive_sort(componentsOrdering);
	}

}

void ComponentGraph::printComponentsOrdering(list<unsigned int>& componentsOrdering){
	cout<<"ORDER:"<<endl;
	for(auto comp:componentsOrdering){
		cout<<"Component "<<comp<<" ";
		for(auto it:componentDependency)
			if(it.second==comp)
				cout<<it.first;
		cout<<endl;
	}
}

void ComponentGraph::recursive_sort(list<unsigned int>& componentsOrdering) {

	// Recursive method:
	// If exist a cycle detect the component of each cycle and for each cycle delete one
	// negative edge that connect two vertices of the same cycle, if topological sort don't report
	// error then exit else redo all the operation until all the cycle are deleted


	auto strongcomp = compGraph.calculateStrongComponent();
	std::unordered_map<unsigned,unsigned> lc;
	for(unsigned i=0;i<strongcomp.size();i++)
		for(auto label:strongcomp[i])
			lc.insert({label,i});

	auto edges=compGraph.getEdges();
	std::unordered_set<unsigned> compProcessed;
	for(auto& e:edges){
		unsigned c1=lc[get<0>(e)];
		unsigned c2=lc[get<1>(e)];
		if(c1==c2 && get<2>(e)<0 && compProcessed.insert(c1).second)
			compGraph.removeEdge(get<0>(e),get<1>(e));
	}

	try{
		vector<unsigned> tpsort=compGraph.calculateTopologicalSort();
		componentsOrdering.insert(componentsOrdering.end(),tpsort.begin(),tpsort.end());
	} catch (Graph::topological_exception& e) {
		componentsOrdering.clear();
		this->recursive_sort(componentsOrdering);
	}


}


void ComponentGraph::computeAllPossibleOrdering(vector<vector<unsigned int>>& componentsOrderings) {
	//TODO
}

void StatementDependency::assignRuleIndex(Rule* r) {
	unsigned index = constraints.size() + weak.size() + rules.size();
	r->setIndex(index);
}

/*
 *
 *
 *
 * END ComponentGraph
 *
 *
 *
 *
 *
 */

void StatementDependency::addRuleMapping(Rule* r) {
	assignRuleIndex(r);
	if(r->isAStrongConstraint()){
		constraints.push_back(r);
		statementAtomMapping.addRule(r);
	}else if(r->isWeakConstraint()){
		weak.push_back(r);
		statementAtomMapping.addRule(r);
	}else{
		addAtomMappingAndSetEdb(r);
		rules.push_back(r);
	}
}

bool StatementDependency::isRelevant(Atom* atom) {
	if(appliedMagicRewriting){
		if(usefullPredicateForQueryAnswering.count(atom->getPredicate()))
			return true;
		return false;
	}
	return true;
}

bool StatementDependency::isRelevant(Predicate* p) {
	if(appliedMagicRewriting){
		if(usefullPredicateForQueryAnswering.count(p))
			return true;
		return false;
	}
	return true;
}

bool StatementDependency::magic() {
	string errmsg;
	unsigned queryConstraints = 0;

	//If the option evaluate query is enabled but the program is not stratified,contains choice , disjunction
	//or not contain a query then we enable the printing and print all the fact
	if(Options::globalOptions()->isEvaluateQuery()){
		if(!isQueryEvaluable() || constraints.size() > queryConstraints){
			if(hasChoice || hasDisjunction || compGraph.hasPredicateNotStratified())
				cerr<<"--> Warning: The input program is not stratified or contains disjunction, choice."<<endl;
			if(query.empty())
				cerr<<"--> Warning: No query supplied."<<endl;
			if(constraints.size() > queryConstraints)
				cerr<<"--> Warning: The input program contains integrity constraints."<<endl;
			cerr<<"--> Warning: The evaluation of the query is not applied."<<endl;
			OutputBuilder::getInstance()->setEnablePrint(true);
//			PredicateExtTable::getInstance()->printFacts();
		}
	}

	if(!Options::globalOptions()->rewriteMagic()) return true;
	HandleQuery(queryConstraints);

	if (query.empty()) {
		errmsg = "--> Warning: No query supplied.";
	} else if (constraints.size() > queryConstraints) {
		errmsg = "--> Warning: The input program contains integrity constraints.";
	} else if (weak.size() > 0) {
		errmsg = "--> Warning: The input program contains weak constraints.";
	} else if (hasAggregate) {
		errmsg = "--> Warning: The input program contains aggregate literals.";
//	} else if (Options::globalOptions()->getOptionFrontend() != FRONTEND_CAUTIOUS
//	&& Options::globalOptions()->getOptionFrontend() != FRONTEND_BRAVE) {
//		errmsg = "Neither brave nor cautious reasoning was specified.";
//		return false;
	} else if (hasExternalLiterals) {
		errmsg = "--> Warning: The input program contains external literals.";
	} else if (rules.empty()) {
		errmsg = "--> Warning: IDB is empty or has become empty due to optimizations.";
	} else if(hasNegativeAtom)
		cerr<< "--> Warning: The input program contains negative literals.\nThe correctness of Magic Sets is only guaranteed for super-coherent programs."<<endl;

	if (errmsg.empty()) {
		bool isGroundQuery = true;
		for (auto atom : query)
			if (!atom->isGround()) {
				isGroundQuery = false;
				break;
			}
//		RewriteMagic rewriteMagic(rules, constraints, weak, &query,
//				isGroundQuery);
//		rewriteMagic.rewrite(isGroundQuery);
        MAGIC_SETS magicSets(rules, constraints, weak, &query, isGroundQuery, statementAtomMapping);
        magicSets.rewrite();

		simplifyMagicRules();

	}
	else{
		cerr<<errmsg<<endl;
		cerr<<"--> Warning: Magic Sets technique cannot be applied."<<endl;
		return true;
	}
	return true;
}

void StatementDependency::simplifyMagicRules(){

	//Check if the magic rewriting create a fact rule
	for(unsigned i=0;i<rules.size();i++){
		Rule *rule=rules[i];
		if(rule->isAFact()){
			Atom *fact=rule->getAtomInHead(0);

			fact->setFact(true);
			Predicate* predicate = fact->getPredicate();
//			IndexingStructure* atomSearcher=nullptr;
//			PredicateExtTable::getInstance()->getPredicateExt(predicate)->getAtomSearcher(FACT);
//			if(Options::globalOptions()->getCheckFactDuplicate())
//				atomSearcher=instancesTable->getPredicateExt(predicate)->addAtomSearcher(FACT,HASHSET,nullptr);
//			if(atomSearcher==nullptr || atomSearcher->find(fact)==nullptr){
			PredicateExtTable::getInstance()->getPredicateExt(predicate)->addAtom(fact,FACT);
			if (!Options::globalOptions()->isNofacts() && !Options::globalOptions()->isEvaluateQuery()) {
				OutputBuilder::getInstance()->onFact(fact);
			}
			delete rule;
			rules.erase(rules.begin()+i);
		}
	}

	if(query.size()==1){
		 bool atLeastAGroundTerm = false;
		 for(unsigned i=0;i<query[0]->getTermsSize();++i)
			 if(query[0]->getTerm(i)->isGround()){
				 atLeastAGroundTerm = true;
				 break;
			 }
		if(!atLeastAGroundTerm){
			for(int i=rules.size()-1;i>=0;--i){
				Rule* r=rules[i];
				if(r->getSizeHead()==1 && !r->isChoiceRule() && r->getAtomInHead(0)->getPredicate()->getName()==PREDNAME_QUERY){
					query[0]->getPredicate()->setHiddenForPrinting(false);
					PredicateTable::getInstance()->setQueryPredicate(query[0]->getPredicate());
					r->free();
					delete r;
					rules.erase(rules.begin()+i);
					break;
				}
			}
		}

	}

//	//Check duplicate rule
//	for(unsigned i=0;i<rules.size();i++){
//		for(unsigned j=i+1;j<rules.size();j++){
//			if(*rules[i]==*rules[j]){
//				rules[j]->free();
//				delete rules[j];
//				rules.erase(rules.begin()+j);
//				j--;
//			}
//		}
//	}
	statementAtomMapping.clear();
	PredicateTable::getInstance()->setAllPredicateEDB();
	for(auto r:rules)
		addAtomMappingAndSetEdb(r);
	for(auto r:constraints)
		addAtomMappingAndSetEdb(r);
	for(auto r:weak)
		addAtomMappingAndSetEdb(r);

	//Rule subsumption check
	SubsumptionChecker sc;
	RuleHashBuilder* rhb = new RuleHashBuilder();
	sc.setRuleHashBuilder(rhb);
	sc.run(rules);
	delete rhb;

}

void StatementDependency::addAtomMappingAndSetEdb(Rule *r){
	if(!r->isAStrongConstraint() && !r->isWeakConstraint()){
		set_predicate pred_head=r->getPredicateInHead();
		for(auto p:pred_head)p->setIdb();
	}
	statementAtomMapping.addRule(r);
}

void StatementDependency::createDependencyGraph() {

	if(createdDepedencyGraph) return;

	if(!query.empty() || Options::globalOptions()->isEvaluateQuery()){
		appliedMagicRewriting=magic();

		if(appliedMagicRewriting){
			statementAtomMapping.clear();

			for(unsigned i=0;i<constraints.size();i++){
				constraints[i]->setIndex(rules.size()+i);
				addAtomMappingAndSetEdb(constraints[i]);
			}
		}
	}


	for(unsigned i=0;i<rules.size();i++){
		Rule * rule=rules[i];
		if(appliedMagicRewriting){
			set_predicate pB=rules[i]->getPredicateInBody();
			set_predicate pH=rules[i]->getPredicateInHead();
			usefullPredicateForQueryAnswering.insert(pB.begin(),pB.end());
			usefullPredicateForQueryAnswering.insert(pH.begin(),pH.end());
			addAtomMappingAndSetEdb(rule);
			rule->setIndex(i);
		}
		depGraph.addInDependency(rule);
	}

//	if(Options::globalOptions()->getPrintStatistics() && !usefullPredicateForQueryAnswering.empty()){
//		cerr<<"List of Relevant Predicates: "<<endl;
//		for(auto p:usefullPredicateForQueryAnswering){
//			cerr<<p->getNameArity()<<endl;
//		}
//	}

	//Execute the SQL import directives
	DirectivesHolder::getInstance()->importAll();
	DirectivesHolder::getInstance()->importAllSparql();

	if(appliedMagicRewriting)
		RewriteMagicExternalSources::getInstance()->applyMagicToExternalSources();

	createdDepedencyGraph=true;
}

void StatementDependency::createComponentGraph() {
	compGraph.createComponent(depGraph, statementAtomMapping);
}

// An utility function that specifies how to sort the rules of a component
bool sortRules (Rule* r1,Rule* r2) {
	set_predicate headSecondRule=r2->getPredicateInHead();
	for(auto it=r1->getBeginBody();it!=r1->getEndBody();it++){
		Predicate* pred=(*it)->getPredicate();
		if(pred!=nullptr && headSecondRule.count(pred))
			return false;
	}
	return true;
}

void StatementDependency::substituteRules(const vector<Rule*>& rules, vector<Rule*>& rulesContainer){
	for(auto rule:rules){
		for(unsigned i=0;i<rulesContainer.size();++i){
			Rule* origRule=rulesContainer[i];
			if(rule->getIndex()==origRule->getIndex()){
				statementAtomMapping.removeRule(origRule);
				rulesContainer[i]=rule;
				statementAtomMapping.addRule(rule);
				break;
			}
		}
	}
}
void StatementDependency::substituteRule(Rule* newRule, Rule* oldRule) {
	statementAtomMapping.removeRule(oldRule);
	statementAtomMapping.addRule(newRule);
}
void StatementDependency::addAdditionalExitRulesOrConstraints(const vector<Rule*>& newRules) {
	for (auto r : newRules) {
		set_predicate newPredicatesInHead = r->getPredicateInHead();
		for (auto p : newPredicatesInHead)
			p->setIdb();
		statementAtomMapping.addRule(r);
	}
}

void StatementDependency::addAdditionalPotentiallyRecursiveRules(unsigned compId, const vector<Rule*>& newRules,
		vector<Rule*>& exitRules, vector<Rule*>& recursiveRules, unordered_set<index_object>& componentPredicateInHead) {
	unordered_map<index_object, unsigned int> components=compGraph.getComponent();
	vector<Rule*> componentsRules;
	unordered_set<unsigned> addedRules;

	for (auto r : newRules) {
		set_predicate newPredicatesInHead = r->getPredicateInHead();
		for (auto p : newPredicatesInHead) {
			p->setIdb();
			components.insert( { p->getIndex(), compId });
		}
		statementAtomMapping.addRule(r);
	}

	/// For each rule classify it as exit or recursive
	for(Rule* r: newRules){
		if(addedRules.insert(r->getIndex()).second){
			if(checkIfExitRule(compId,r,components))
				exitRules.push_back( r );
			else{
				recursiveRules.push_back(r);
				for(auto p:r->getPredicateInHead()){
					if(componentPredicateInHead.insert(p->getIndex()).second){
						//If a predicate is recursive add delta and next delta tables
	//								cerr<<"add to table 2"<<endl;
						PredicateExtTable::getInstance()->getPredicateExt(p->getIndex())->addTables(2);
					}
				}
			}
		}

	}

	if( recursiveRules.size() >= 2 )
		stable_sort( recursiveRules.begin(), recursiveRules.end(), sortRules );
}

void StatementDependency::createComponentGraphAndComputeAnOrdering(vector<vector<Rule*>>& exitRules, vector<vector<Rule*>>& recursiveRules,
		vector<unordered_set<index_object>>& componentPredicateInHead,vector<vector<Rule*>>& constraintRules,vector<Rule*>& remainedConstraint,vector<unsigned>& compID) {
	/// Create the component graph
	compGraph.createComponent(depGraph, statementAtomMapping);
	if(Options::globalOptions()->getPrintDepGraph()){
		depGraph.print();
		compGraph.print();
	}

	/// Compute a possible ordering among components
	list<unsigned int> ordering;
	compGraph.computeAnOrdering(ordering);

	/// Declaration of some temporary variables
	vector<Rule*> componentsRules;
	int i=0;

	unordered_map<index_object, unsigned int> components=compGraph.getComponent();
	unordered_set<unsigned> addedRules;

	//For each constraint put the IDB predicates that appear in the constraint.For each component delete the predicates of this component in
	//the predicates of constraints and if all predicate are deleted a constraint can be grounded in this component
	using set_int = unordered_set<index_object>;
	//List of constraint and a set of IDB predicates that appear in the constraint
	using list_rule_setInt = list<pair<Rule*,set_int>>;
	list_rule_setInt constraint_rule;

	for(unsigned j=0;j<constraints.size();j++){
		set_predicate predicates_in_rule=constraints[j]->getPredicateInBody();
		set_int predicate_in_constraint;
		for(auto p:predicates_in_rule)if(!p->isEdb())predicate_in_constraint.insert(p->getIndex());
		constraint_rule.push_back({constraints[j],predicate_in_constraint});
	}


	for(auto comp: ordering){
		compID.push_back(comp);
		exitRules.push_back(vector<Rule*>());
		recursiveRules.push_back(vector<Rule*>());
		componentPredicateInHead.push_back(unordered_set<index_object>());
		constraintRules.push_back(vector<Rule*>());
		for (auto pair: components)
			if(pair.second==comp){
				/// Get all the rules for the current component
				index_object predicate=pair.first;
				statementAtomMapping.getRuleInHead(predicate,componentsRules);
				/// For each rule classify it as exit or recursive
				for(Rule* r: componentsRules){
					if(addedRules.insert(r->getIndex()).second){
						if(checkIfExitRule(comp,r,components)){
							exitRules[i].push_back(r);
						}

						else{
							recursiveRules[i].push_back(r);
							for(auto p:r->getPredicateInHead()){
								if(componentPredicateInHead[i].insert(p->getIndex()).second){
									//If a predicate is recursive add delta and next delta tables
									PredicateExtTable::getInstance()->getPredicateExt(p->getIndex())->addTables(2);
								}
							}
						}
					}
				}
				componentsRules.clear();
				vector<list_rule_setInt::iterator> to_delete;
				for(auto it=constraint_rule.begin();it!=constraint_rule.end();it++){
					if(it->second.count(predicate))
						it->second.erase(predicate);
					if(it->second.size()==0){
						constraintRules[i].push_back(it->first);
						to_delete.push_back(it);
					}
				}
				for(auto it:to_delete)constraint_rule.erase(it);
			}
		i++;
	}

	for(unsigned int i=0;i<recursiveRules.size();i++){
		if(recursiveRules[i].size()>=2)
			stable_sort(recursiveRules[i].begin(),recursiveRules[i].end(),sortRules);
	}

	for(auto it=constraint_rule.begin();it!=constraint_rule.end();it++){
		remainedConstraint.push_back(it->first);
	}
}


bool StatementDependency::checkIfExitRule(unsigned int component, Rule* rule,unordered_map<index_object, unsigned int>& components){
	unordered_set<index_object> positivePredicates=rule->getPositivePredicateIndexInBody();

	for (auto pair: components){
		if(pair.second==component && positivePredicates.count(pair.first))
			return false;
	}
	return true;
}

void StatementDependency::print() {
//	string fileDGraph = Config::getInstance()->getFileGraph() + "DG";
//	string fileCGraph = Config::getInstance()->getFileGraph() + "CG";
//
//	if (Config::getInstance()->isDependency()) {
//		if (strcmp(fileDGraph.c_str(), "DG") == 0)
//			depGraph.print();
//		else
//			depGraph.printFile(fileDGraph);
//	}
//	if (Config::getInstance()->isComponent()) {
//		if (strcmp(fileCGraph.c_str(), "C
//	G") == 0)
//			compGraph.print();
//		else
//			compGraph.printFile(fileCGraph);
//
//	}
//	if (Config::getInstance()->isPrintRules())
//		for (Rule*r : rules)
//			r->print();

}

StatementDependency* StatementDependency::getInstance(){
	if(statementDependency==nullptr)
		statementDependency = new StatementDependency;

	return statementDependency;
}


/** creates appropriate constraints for implementing queries efficiently
  * Note: Error messages are not terminated by newline, since it is expected
  *       that some other message (indicating how the program will proceed)
  *       is printed immediately afterwards.
  * @param queryConstraints the number of constraints introduced by this query
  * @return false if some error occurred
  */
 bool StatementDependency::HandleQuery(unsigned &queryConstraints)
     {
	 bool queryNotDefined=false;
	 bool queryNotInHead=true;
     for(auto atom:query){
    	 Predicate* p=atom->getPredicate();
    	 if(!statementAtomMapping.isInHead(p->getIndex())){
    		 if(PredicateExtTable::getInstance()->getPredicateExt(p)->getPredicateExtentionSize()==0){
    			 cerr<<"Warning: "<<p->getName()<<" is not defined."<<endl;
    		 	 queryNotDefined=true;
    		 }else if(!atom->isGround())
    			 queryNotInHead=false;
    	 }else
    		 queryNotInHead=false;
     }
     if(queryNotDefined || queryNotInHead){
    	 for(auto r:rules)
    		 delete r;
    	 rules.clear();
    	 for(auto w:weak)
    		 delete w;
    	 weak.clear();
    	 for(auto c:constraints)
    		 delete c;
    	 constraints.clear();
    	 return false;
     }

     if( query.empty() )
         {
         // No query has been specified, but one is required when brave
         // or cautious reasoning are requested.
    	 if( Options::globalOptions()->getOptionFrontend() == FRONTEND_BRAVE
             || Options::globalOptions()->getOptionFrontend() == FRONTEND_CAUTIOUS )
             {
             cerr << "No query supplied. Cannot continue." << endl;
             return false;
             }
         else
             return true;
         }


//        if( TraceLevel >= 1 )
//            cdebug << "Handling Query " << *Query << "?" << endl;

     set_term variables;

     // Make one pass through the query, checking whether it is ground
     // or not and looking for the maximum variable index.
     for( vector<Atom*>::const_iterator i=query.begin();
          i != query.end();
          i++ )
         {
         if( ! (*i)->isGround() )
             {
             isQueryGround=false;

             set_term vars=(*i)->getVariable();
             variables.insert(vars.begin(),vars.end());
//                for( vector<Term*>::const_iterator j=(*i)->getTerms().begin();
//                     j != (*i)->getTerms().end();
//                     j++ )
//                    if( (*j)->getType()==VARIABLE )
//                        maxVar=max(maxVar,j->getVar());
//                    else if( j->isComplex() )
//                        {
//                        set<TERM> vars;
//                        j->getComplexTerm().getComplexVars(vars);
//                        for( set<TERM>::const_iterator v = vars.begin();
//                             v != vars.end(); v++ )
//                            maxVar=max(maxVar,v->getVar());
//                        }
                }
         }

     if( Options::globalOptions()->getOptionFrontend() == FRONTEND_CAUTIOUS )
         {

         if( isQueryGround )
             {
             // We do cautious reasoning and have a ground query.

             // The query is treated as an internal constraint.
             // Example: a, not b ?
             //  becomes :- a, not b.
             //
             // If a model is computed, this means that this model did not
             // violate the constraint, i.e. at least one of the query's
             // literals is false in this model.  This model can be seen as
             // a witness that the query is not true in all possible models.
             //
             // If no model is computed, the query is cautiously true.

             // Create a constraint containing exactly the query
             // conjunction and mark it as internal.
         	Rule *c=new Rule;
         	for(auto atom:query)
         		c->addInBody(atom->clone());
//         	c->setBody(query);
            constraints.push_back(c);
            queryConstraints++;
            PredicateTable::getInstance()->setQueryPredicate(query[0]->getPredicate()); //FIX ME IN CASE MULTIPLE QUERY ATOMS

//             if( TraceLevel >= 1 )
//                 cdebug << "                " << c << endl;
             }
         }
     else if( Options::globalOptions()->getOptionFrontend() == FRONTEND_BRAVE )
         {
         // We either do brave reasoning, or just have a query without
         // reasoning (possibly coming from a frontend).  For now, we
         // forbid the latter for non-ground queries.
         if( Options::globalOptions()->getOptionFrontend() != FRONTEND_BRAVE  &&  ! isQueryGround )
             {
             cerr << "Non-ground queries are only supported with brave and "
                     "cautious reasoning." << endl;
             return false;
             }

         // Each ground literal in the query is transformed into a constraint,
         // which we add to the program (marked as internal).
         // Example: a, not b? becomes :- not a.
         //                            :- b.
         for( vector<Atom*>::const_iterator i=query.begin();
              i != query.end();
              i++ )
             if( (*i)->isGround() )
                 {
                 Rule *c=new Rule;
                 Atom *newAtom=(*i)->clone();
                 newAtom->setNegative(!((*i)->isNegative()));
                 c->addInBody(newAtom);
                 constraints.push_back(c);
                 queryConstraints++;

//                 if( TraceLevel >= 1 )
//                     cdebug << "                " << c << endl;
                 }
         }
     	 PredicateTable::getInstance()->setQueryPredicate(query[0]->getPredicate()); //FIX ME IN CASE MULTIPLE QUERY ATOMS


     // If the query is non-ground, we need to create a new "query rule" with
     // the query in the body, and initialize the interpretation that will
     // hold the union or intersection, respectively, of all models found.
     if( ! isQueryGround )
         {
//         TERMS t;
//         for(unsigned i=0; i <= maxVar; i++)
//             t.push_back( TERM(i) );

    	 Rule *newRule=new Rule;
//    	 for(auto q:query)
//    		 newRule->addInBody(q->clone());
    	 for(auto atom:query)
    		 newRule->addInBody(atom->clone());
//    	newRule->setBody(query);
    	string name=PREDNAME_QUERY;
		Predicate *newPred=new Predicate(name,variables.size(),false);
		PredicateTable::getInstance()->insertPredicate(newPred);
		PredicateExtTable::getInstance()->addPredicateExt(newPred);
		newPred->setHiddenForPrinting(false);
		vector<Term*> terms;
		for(auto t:variables)terms.push_back(t);
    		newRule->addInHead(new ClassicalLiteral(newPred,terms,false,false));

//         DISJUNCTION head;
//         head.add( ATOM(PREDNAME_QUERY,&t,PREDICATE_NAMES::typeQuery) );
//         IDB.push_back( RULE(&head,Query) );

//         if( TraceLevel >= 1 )
//             cdebug << "Adding rule for non-ground query: "
//                    << IDB.back() << endl;
//         assert( IDB.back().isSafe() );
		 rules.push_back(newRule);
		 PredicateTable::getInstance()->setQueryPredicate(newPred);
         }
     return true;
     }

 StatementDependency::~StatementDependency() {
		if(Options::globalOptions()->isPrintRewrittenProgram()){
			cerr<<"----------    BEGIN REWRITTEN PROGRAM      ----------"<<endl;
			DecompositionMapper* decomp=DecompositionMapper::getInstance();
			for(auto r:rules){
				if(decomp->isDecomposed(r)){
					for(auto it=decomp->getBegin(r);it!=decomp->getEnd(r);++it)
						(*it)->print(cerr);
					decomp->get(r)->print(cerr);
				}
				else
					r->print(cerr);
			}
			for(auto r:constraints)
				if(decomp->isDecomposed(r)){
					for(auto it=decomp->getBegin(r);it!=decomp->getEnd(r);++it)
						(*it)->print(cerr);
					decomp->get(r)->print(cerr);
				}
				else
					r->print(cerr);

			for(auto r:weak)
				if(decomp->isDecomposed(r)){
					for(auto it=decomp->getBegin(r);it!=decomp->getEnd(r);++it)
						(*it)->print(cerr);
					decomp->get(r)->print(cerr);
				}
				else
					r->print(cerr);
			cerr<<"----------      END REWRITTEN PROGRAM       ----------"<<endl;
		}
		for(Rule* r:rules)
			clearRule(r);

		for(Rule* r:constraints)
			clearRule(r);

		for(Rule* r:weak)
			clearRule(r);

		for(auto a:query)
			delete a;
	}


}
}
