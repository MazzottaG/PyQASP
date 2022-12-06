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
 * ProgramGrounder.cpp
 *
 *  Created on: 10/apr/2014
 *      Author: Davide
 */



#include <list>

#include "ProgramGrounder.h"
#include "../atom/ClassicalLiteral.h"
#include "../../util/Timer.h"
#include "../../util/Utils.h"
#include "../../../common/Options.h"
#include "../../../common/Trace.h"
#include "../statement/OrderRuleGroundable.h"
#include "../statement/GroundingPreferences.h"
#include "../../input/ExtAtomFactory.h"
#include "../exception/ConstrainException.h"
#include "../exception/ExtAtomException.h"
#include "../../directives/DirectivesHolder.h"
#include "IntelligentDecomposer.h"
#include "PropositionalGrounder.h"
#include "../output/PreaspOutputBuilder.h"

namespace DLV2{

namespace grounder{

void ProgramGrounder::printProgram(const vector<vector<Rule*> >& exitRules,	const vector<vector<Rule*> >& recursiveRules) {
	for (unsigned int component = 0; component < exitRules.size();
			component++) {
		for (Rule* r : exitRules[component])
			r->print();
		for (unsigned int i = 0; i < recursiveRules[component].size(); i++)
			recursiveRules[component][i]->print();
	}
	for (unsigned int i = 0; i < statementDependency->getConstraintSize(); i++)
		if (statementDependency->getConstraint(i)->getSizeBody() > 0)
			statementDependency->getConstraint(i)->print();


}

#ifdef TRACE_ON
void printTableInRule(Rule *rule,vector<vector<pair<unsigned,SearchType>>>& predicate_searchInsert_table,bool body=true){
	cerr<<endl;
	PredicateExtTable *predicateExtTable=PredicateExtTable::getInstance();
	if(body){
		for(unsigned i=0;i<rule->getSizeBody();i++){
			Predicate* p=rule->getAtomInBody(i)->getPredicate();
			if(p==nullptr) continue;
			cerr<<"---> ATOM: ";rule->getAtomInBody(i)->print(cerr);cerr<<endl;
			for(auto tableToSearch:predicate_searchInsert_table[i+rule->getSizeHead()]){
				cerr<<"TABLE: "<<tableToSearch.first<<" --> ";
				predicateExtTable->getPredicateExt(p)->getTable(tableToSearch.first)->print(cerr);
				cerr<<endl;
			}
		}
	}
	else{
		for(unsigned i=0;i<rule->getSizeHead();i++){
			Predicate* p=rule->getAtomInHead(i)->getPredicate();
			if(p==nullptr) continue;
			cerr<<"---> ATOM: ";rule->getAtomInHead(i)->print(cerr);cerr<<endl;
			for(auto tableToSearch:predicate_searchInsert_table[i]){
				cerr<<"TABLE: "<<tableToSearch.first<<" --> ";
				predicateExtTable->getPredicateExt(p)->getTable(tableToSearch.first)->print(cerr);
				cerr<<endl;
			}
		}
	}
}
#endif

void ProgramGrounder::findRecursivePredicatesInComponentRules(const unordered_set<index_object>& componentPredicateInHead, vector<unsigned>& recursivePredicatesPositions, Rule* rule, vector<unsigned >& orderedBody) {
	for (unsigned j = 0; j < rule->getSizeBody(); ++j) {
		Predicate* pred = rule->getAtomInBody(j)->getPredicate();
		if (pred != nullptr	&& componentPredicateInHead.count(pred->getIndex()) && !rule->getAtomInBody(j)->isNegative())
			recursivePredicatesPositions.push_back(j);
		orderedBody.push_back(j);
	}
}

void ProgramGrounder::orderPositiveAtomsBody(vector<unsigned>& originalOrderBody,Rule* rule, unordered_set<index_object>* componentPredicateInHead) {
	rule->computeVariablesLocalIndices();
	unsigned sizeRule=rule->getSizeBody();
	predicate_searchInsert_atomSearcher.clear();
	predicate_searchInsert_atomSearcher.resize(rule->getSizeHead()+sizeRule);
	OrderRuleGroundable* orderRuleGroundable = OrderRuleGroundableFactory::getInstance(rule);
	if (orderRuleGroundable != nullptr) {
		orderRuleGroundable->order(predicate_searchInsert_table,predicate_searchInsert_atomSearcher,originalOrderBody,componentPredicateInHead);
		delete orderRuleGroundable;
	}
}

void ProgramGrounder::projectAtomsInRule(vector<Rule*>& exitRules,vector<Rule*>& recursiveRule, vector<Rule*>& constraint,unordered_set<index_object>& recursivePred,BaseInputRewriter& rewriter) {

	for(unsigned i=0;i<exitRules.size();i++){
		vector<Rule*> rules;
		rewriter.projectAtoms(exitRules[i],rules);
		unsigned start=i;
		for(unsigned j=0;j<rules.size();j++){
			exitRules.insert(exitRules.begin()+start+j,rules[j]);
			++i;
		}
	}

	for(unsigned i=0;i<recursiveRule.size();i++){
		vector<Rule*> rules;
		rewriter.projectAtoms(recursiveRule[i],rules);
		unsigned start=i;
		for(unsigned j=0;j<rules.size();j++){
			Predicate *predicateInBody=rules[j]->getAtomInBody(0)->getPredicate();
			if(recursivePred.count(predicateInBody->getIndex())){
				recursivePred.insert(rules[j]->getAtomInHead(0)->getPredicate()->getIndex());
				recursiveRule.insert(recursiveRule.begin()+start+j,rules[j]);
				++i;
			}else
				exitRules.push_back(rules[j]);

		}
	}

	for(unsigned i=0;i<constraint.size();i++){
		vector<Rule*> rules;
		rewriter.projectAtoms(constraint[i],rules);
		unsigned start=i;
		for(unsigned j=0;j<rules.size();j++){
			constraint.insert(constraint.begin()+start+j,rules[j]);
			++i;
		}
	}

//	cout<<"AFTER PROJ"<<endl;
//	cout<<"EXIT"<<endl;
//	for(auto r:exitRules)
//		r->print();
//	cout<<"RECURSIVE"<<endl;
//	for(auto r:recursiveRule)
//		r->print();
//	cout<<"CONSTRAINT"<<endl;
//	for(auto r:constraint)
//		r->print();
//
//	cout<<"END"<<endl;

}

bool ProgramGrounder::orderPositiveAtomsBody(Rule* rule) {
	rule->computeVariablesLocalIndices();
	predicate_searchInsert_atomSearcher.clear();
	predicate_searchInsert_atomSearcher.resize(rule->getSizeHead()+rule->getSizeBody());
	OrderRuleGroundable* orderRuleGroundable = OrderRuleGroundableFactory::getInstance(rule);
	if (orderRuleGroundable != nullptr && rule->getSizeBody()<ATOMS_IN_RULE) {
		orderRuleGroundable->order(predicate_searchInsert_table,predicate_searchInsert_atomSearcher);
		delete orderRuleGroundable;
	}
	if(!Options::globalOptions()->isEnabledDictionaryIntersection() && !GroundingPreferences::getGroundingPreferences()->getAlignSubstitutions(rule)) return false;
	for(auto it=rule->getBeginBody();it!=rule->getEndBody();it++){
		Atom* atom=*it;
		if(atom->isClassicalLiteral() && !atom->isNegative()){
		for(unsigned j=0;j<atom->getTermsSize();++j){
			Term* t=atom->getTerm(j);
			if(t->getType()==VARIABLE  && rule->getRuleInformation().isCreatedDictionaryIntersection(t->getLocalVariableIndex()) && rule->getRuleInformation().isAJoinVariable(t) && rule->getRuleInformation().getDictionaryIntersectionSize(t->getLocalVariableIndex())==0)
				return true;
			}
		}
	}
	return false;
}

void ProgramGrounder::applyDecompositionRewriting(const vector<Rule*>& rules, vector<Rule*>& newRules, vector<Rule*>& changedRules) {
	for (Rule* rule : rules) {
		if (checkIfDecompositionIsConvenient(rule)) {
			DecompositionMapper* decompositionMapper = DecompositionMapper::getInstance();
			for (auto it = decompositionMapper->getBegin(rule); it != decompositionMapper->getEnd(rule); ++it)
				newRules.push_back(*it);
			changedRules.push_back(decompositionMapper->get(rule));
		}
	}
}

void ProgramGrounder::ground() {
	//Create the dependency graph
	statementDependency->createDependencyGraph();
	bool foundEmptyConstraint=false;
//	bool projectAtomsInRules=Options::globalOptions()->getRewProject()==2;
	BaseInputRewriter rewriter;
	// Create the component graph and compute an ordering among components.
	// Components' rules are classified as exit or recursive.
	// An rule occurring in a component is recursive if there is a predicate belonging
	// to the component in its positive body, otherwise it is said to be an exit rule.
	vector<vector<Rule*>> exitRules;
	vector<vector<Rule*>> recursiveRules;
	vector<vector<Rule*>> constraintRules;
	vector<Rule*> remainedConstraint;
	vector<unordered_set<index_object>> componentPredicateInHead;
	vector<unsigned> compID;
	statementDependency->createComponentGraphAndComputeAnOrdering(exitRules, recursiveRules, componentPredicateInHead,constraintRules,remainedConstraint,compID);

	trace_msg(grounding,1,"Grounding Starts Now.");


	try {
	bool printOnlyQuery=Options::globalOptions()->isEvaluateQuery();
	bool printedQueryResult=false;
	Predicate *queryPredicate = predicateTable->getQueryPredicate();

	// Ground each module according to the ordering:
	// For each component, each rule is either recursive or exit,
	// Exit rules are grounded just once, while recursive rules are grounded until no more knowledge is derived
	for (unsigned int component = 0; component < exitRules.size()&&!foundEmptyConstraint; ++component) {

		trace_msg(grounding,1,"Component: "<<component);
		trace_msg(grounding,1,"Exit rules: "<<exitRules[component].size());
		trace_msg(grounding,1,"Recursive rules: "<<recursiveRules[component].size());

//		if(projectAtomsInRules){
//			projectAtomsInRule(exitRules[component],recursiveRules[component],constraintRules[component],componentPredicateInHead[component],rewriter);
//		}

		// Decomposition of recursive rules.
		if (Options::globalOptions()->isEnabledDecomposition()){
			vector<Rule*> newRules;
			vector<Rule*> changedRules;
			applyDecompositionRewriting(recursiveRules[component], newRules, changedRules);
			if(!changedRules.empty()){
				statementDependency->substituteRules(changedRules,recursiveRules[component]);
				statementDependency->addAdditionalPotentiallyRecursiveRules(compID[component],newRules,
					exitRules[component],recursiveRules[component],componentPredicateInHead[component]);
			}

//			newRules.clear();
//			changedRules.clear();
//			applyDecompositionRewriting(exitRules[component], newRules, changedRules);
//			exitRules[component].insert(exitRules[component].begin(),newRules.begin(),newRules.end());
//			statementDependency->substituteRules(changedRules,exitRules[component]);
//			statementDependency->addAdditionalExitRulesOrConstraints(newRules);
//
//			newRules.clear();
//			changedRules.clear();
//			applyDecompositionRewriting(constraintRules[component], newRules, changedRules);
//			constraintRules[component].insert(constraintRules[component].begin(),newRules.begin(),newRules.end());
//			statementDependency->substituteRules(changedRules,constraintRules[component]);
//			statementDependency->addAdditionalExitRulesOrConstraints(newRules);
		}

		iteration=0;
		// Ground exit rules
		for (Rule* rule : exitRules[component]){
//			if(rule->isAnInputGroundRule()){
//				if(inizializeSearchInsertPredicate(rule))
//					continue;
//				GroundRuleGrounder g;
//				g.groundRule(rule,&componentPredicateInHead[component]);
//				g.addHeadToPredicateExtensions(predicate_searchInsert_table,predicate_searchInsert_atomSearcher);
//				continue;
//			}
			if(nonGroundSimplificator.simplifyRule(rule))
				continue;
			trace_action_tag(grounding,1,cerr<<"Grounding Exit Rule: ";rule->print(cerr););
			//If we have to print only the query we have to enable the output builder
			//if the rule is a query rule (constains the answer to the query)
			if(printOnlyQuery){
				if(rule->isQueryRule(queryPredicate) && !statementDependency->queryGround()){
					printedQueryResult=true;
					outputBuilder->setEnablePrint(true);
				}
				else
					outputBuilder->setEnablePrint(false);
			}
			if(performDecompositionOfExitRule(rule))
				continue;
			if(inizializeSearchInsertPredicate(rule))
				continue;
			orderPositiveAtomsBody(rule);
			groundRule(rule);

#ifdef DEBUG_RULE_TIME
			Timer::getInstance()->print();
#endif
		}

		if (!recursiveRules[component].empty()){
			// Ground recursive rules
			unsigned int n_rules = recursiveRules[component].size();
			bool found_something = false;

			vector<vector<unsigned>> recursivePredicatesPositions;
			recursivePredicatesPositions.resize(n_rules);

			// A vector containing for each atom in the body (according to the current order) their original position (in the original body)
			vector<vector<unsigned>> originalOrderBody;
			originalOrderBody.resize(n_rules);

			iteration++;
			updateIndiciesHistoryTable(componentPredicateInHead[component]);

			// First iteration
			for (unsigned int i = 0; i < n_rules; ++i) {
				Rule *rule=recursiveRules[component][i];
				trace_action_tag(grounding,1,cerr<<"Grounding Recursive Rule: ";rule->print(cerr););
//				cerr<<"Recursive Exit Rule: ";rule->print(cerr);
				if(nonGroundSimplificator.simplifyRule(rule) || inizializeSearchInsertPredicate(rule)){
					if(n_rules>1) findRecursivePredicatesInComponentRules(componentPredicateInHead[component], recursivePredicatesPositions[i], rule, originalOrderBody[i]);
					trace_msg(grounding,1,"Rule simplified: no grounding is needed.");
					continue;
				}
				//trace_msg(grounding,1,"At first iteration the tables to search in  are: ");
				//trace_action_tag(grounding,2,printTableInRule(rule,predicate_searchInsert_table););

				findRecursivePredicatesInComponentRules(componentPredicateInHead[component], recursivePredicatesPositions[i], rule, originalOrderBody[i]);
				orderPositiveAtomsBody(originalOrderBody[i], rule, &componentPredicateInHead[component]);
				if(printOnlyQuery){
					if(rule->isQueryRule(queryPredicate) && !statementDependency->queryGround()){
						printedQueryResult=true;
						outputBuilder->setEnablePrint(true);
					}
					else
						outputBuilder->setEnablePrint(false);
				}
				if(groundRule(rule,&componentPredicateInHead[component]))
					found_something=true;

				trace_action_tag(grounding,1,cerr<<"Found New Knowledge: "<<found_something<<endl;);
				//trace_msg(grounding,1,"After the first iteration the tables to insert in  are: ");
				//trace_action_tag(grounding,2,printTableInRule(rule,predicate_searchInsert_table););

			}
			trace_action_tag(grounding,1,cerr<<"Iteration Number: "<<iteration<<endl;);

			//Further Iterations
			while (found_something) {

				++iteration;
				updateIndiciesHistoryTable(componentPredicateInHead[component]);

//				trace_msg(grounding,1,"Further Iterations");
				found_something = false;

				// Since in the first iteration search is performed in facts and no facts tables,
				// while in the next iteration search is performed in the delta table, it is needed
				// to keep track if the current iteration is the first or not.
				for (unsigned int i = 0; i < n_rules; ++i) {
					Rule* rule = recursiveRules[component][i];
					trace_action_tag(grounding,1,cerr<<"Grounding Recursive Rule: ";rule->print(cerr););
//					cerr<<"Recursive Exit Rule Again: ";rule->print(cerr);
					//If no more knowledge is derived the grounding of this component can stop
					for(auto token: recursivePredicatesPositions[i]){
						if(nextSearchInsertPredicate(rule,componentPredicateInHead[component],token,originalOrderBody[i])){
							continue;
						}
						//trace_msg(grounding,1,"At this iteration the tables to search in  are: ");
						//trace_action_tag(grounding,2,printTableInRule(rule,predicate_searchInsert_table););

						orderPositiveAtomsBody(originalOrderBody[i],rule,&componentPredicateInHead[component]);
						if(printOnlyQuery){
							if(rule->isQueryRule(queryPredicate) && !statementDependency->queryGround()){
								printedQueryResult=true;
								outputBuilder->setEnablePrint(true);
							}
							else
								outputBuilder->setEnablePrint(false);
						}
						if (groundRule(rule,&componentPredicateInHead[component])){
							found_something = true;
						}

						trace_action_tag(grounding,1,cerr<<"Found New Knowledge: "<<found_something<<endl;);
						//trace_msg(grounding,1,"After the first iteration the tables to insert in  are: ");
						//trace_action_tag(grounding,2,printTableInRule(rule,predicate_searchInsert_table););
					}
				}

				trace_action_tag(grounding,1,cerr<<"Iteration Number: "<<iteration<<endl;);


//				// If more rule share a recursive predicate, only the first rule
//				// have to swap the delta, then we need a set of predicate evaluated
//				set_predicate predicateEvaluated;
//				for (unsigned int i = 0; i < n_rules; i++){
//					// Move the content of the delta table in the no fact table,
//					// and fill delta with the content of the next delta table.
//					swapInDelta(recursiveRules[component][i],predicateEvaluated);
//					trace_action_tag(grounding,1,cerr<<"Swap Delta Rule: ";recursiveRules[component][i]->print(cerr););
//					trace_action_tag(grounding,2,printTableInRule(recursiveRules[component][i],predicate_searchInsert_table););
//				}


			}
		}

		// Ground constraint rules
		for (Rule* rule : constraintRules[component]){
			if(rule->isAnInputGroundRule()){
				GroundRuleGrounder g;
				g.groundRule(rule,&componentPredicateInHead[component]);
				continue;
			}
			if (rule->getSizeBody() == 0) continue;
			if(nonGroundSimplificator.simplifyRule(rule) || inizializeSearchInsertPredicate(rule))
				continue;
			orderPositiveAtomsBody(rule);
			trace_action_tag(grounding,1,cerr<<"Grounding Constraint Rule: ";rule->print(cerr););
			if(performDecompositionOfExitRule(rule))
				continue;
			groundRule(rule);
		}
	}


	// Remained Constraints are grounded at the end
	if(!foundEmptyConstraint)
	{

//		// Decomposition of remained constraint rules.
//		if (Options::globalOptions()->isEnabledDecomp()){
//			vector<Rule*> newRules;
//			vector<Rule*> changedRules;
//			applyDecompositionRewriting(remainedConstraint, newRules, changedRules);
//			remainedConstraint.insert(remainedConstraint.begin(), newRules.begin(),newRules.end());
//			statementDependency->substituteRules(changedRules,remainedConstraint);
//			statementDependency->addAdditionalExitRulesOrConstraints(newRules);
//		}

		for (auto rule:remainedConstraint){
			if (rule->getSizeBody() > 0){
				if(rule->isAnInputGroundRule()){
					GroundRuleGrounder g;
					g.groundRule(rule);
					continue;
				}
				if(nonGroundSimplificator.simplifyRule(rule) || inizializeSearchInsertPredicate(rule))
					continue;
				orderPositiveAtomsBody(rule);
				trace_action_tag(grounding,1,cerr<<"Grounding Constraint Rule: ";rule->print(cerr););
				if(performDecompositionOfExitRule(rule))
					continue;
				groundRule(rule);
			}
		}
		substituteIndicesInRulesWithPossibleUndefAtoms();
		addTrueNegationConstraints();

		vector<Rule*> weakConstraints=statementDependency->getWeakContraints();
//		// Decomposition of remained constraint rules.
//		if (Options::globalOptions()->isEnabledDecomp()){
//			vector<Rule*> newRules;
//			vector<Rule*> changedRules;
//			applyDecompositionRewriting(weakConstraints, newRules, changedRules);
//			weakConstraints.insert(weakConstraints.begin(), newRules.begin(),newRules.end());
//			statementDependency->substituteRules(changedRules,weakConstraints);
//			statementDependency->addAdditionalExitRulesOrConstraints(newRules);
//		}

		//Ground weak constraint
		for(auto weak:weakConstraints){
			if(weak->isAnInputGroundRule()){
				GroundRuleGrounder g;
				g.groundRule(weak);
				continue;
			}
			if(nonGroundSimplificator.simplifyRule(weak) || inizializeSearchInsertPredicate(weak)){
				continue;
			}
			orderPositiveAtomsBody(weak);
			trace_action_tag(grounding,1,cerr<<"Grounding Constraint Rule: ";weak->print(cerr););
			if(performDecompositionOfExitRule(weak))
				continue;
			groundRule(weak);
		}
	}

	//If we have to print only the result of the query but the query is ground, then we have to search
	// in the predicate extension if the ground query is true
	if(printOnlyQuery && statementDependency->isQueryEvaluable()){
		if(!printedQueryResult && statementDependency->queryGround()){
			bool answer=true;
			for(auto &atom:statementDependency->getQueryAtoms()){
				if(predicateExtTable->getPredicateExt(atom->getPredicate())->getGroundAtom(atom)==nullptr){
					answer=false;
					break;
				}
			}
			if(answer){
				for(auto &atom:statementDependency->getQueryAtoms()){
					atom->print();cout<<"."<<endl;
				}
			}
		}
	}
//	if(queryPredicate!=nullptr && queryPredicate->getName()!=PREDNAME_QUERY && !statementDependency->queryGround()){
//		predicateExtTable->getPredicateExt(queryPredicate)->printQueryFactTable();
//	}

	if(printStats)
		rstats->printStats();

	createPropagatorForWasp();

//	Timer::getInstance()->printSumTime(cerr);

	//Execute the SQL export directives
	DirectivesHolder::getInstance()->exportAll();
	//Close the SQL connections
	DirectivesHolder::getInstance()->disconnectAll();

	//Print and simplify the rule
//	evaluator.printAndSimplify(predicateExtTable);
	} catch (ConstrainException& e) {
	} catch (ExtAtomException& e) {
	}
}

void ProgramGrounder::addTrueNegationConstraints() {

        bool foundEmptyConstraint = false;

        Rule *constraintRule=new Rule(true,0,2);
	//Add constraint for true negation
	for(auto predicate:predicateTable->getPredicateTrueNegated()){
		if(foundEmptyConstraint)break;
		//For each predicate true negated for table FACT and NOFACT try to find an a equal atom
		// but with the predicate not true negated. If exist print the constraint
		string predName=predicate->getName();
		Predicate *predNotTrueNegated=new Predicate(predName,predicate->getArity());
		predicateTable->insertPredicate(predNotTrueNegated);
		auto predExt= predicateExtTable->getPredicateExt(predicate);
		auto predExtNotTrueNegated= predicateExtTable->getPredicateExt(predNotTrueNegated);
		for(unsigned i=0;i<2;i++){
			if(predExt->getPredicateExtentionSize(i)==0)continue;
			for(auto atom:*predExt->getTable(i)){
				atom->setPredicate(predNotTrueNegated);
				Atom* atomNotTrueNegated=predExtNotTrueNegated->getGroundAtom(atom);
				atom->setPredicate(predicate);
				if(atomNotTrueNegated!=nullptr){
					constraintRule->setAtomInBody(0,atom);
					constraintRule->setAtomInBody(1,atomNotTrueNegated);
					constraintRule->setAtomToSimplifyInBody(0,atom->isFact());
					constraintRule->setAtomToSimplifyInBody(1,atomNotTrueNegated->isFact());
					outputBuilder->onRule(constraintRule);
					if(atom->isFact() && atomNotTrueNegated->isFact()){
						foundEmptyConstraint=true;
						break;
					}
				}
			}
		}
	}
	delete constraintRule;
}

bool ProgramGrounder::isNotEmptyPredExt(Predicate* pred,unsigned table) {
	return PredicateExtTable::getInstance()->getPredicateExt(pred)->getPredicateExtentionSize(
			table) > 0;
}

bool ProgramGrounder::isNotEmptyPredExt(Predicate* pred,unsigned table,SearchType type) {
	return PredicateExtTable::getInstance()->getPredicateExt(pred)->getPredicateExtentionSize(table,type) > 0;
}

bool ProgramGrounder::inizializeSearchInsertPredicateBody(Rule* rule) {
	for (auto atom = rule->getBeginBody(); atom != rule->getEndBody(); atom++) {
		vector<pair<unsigned,SearchType>> tableToInsert;
		Predicate* pred = (*atom)->getPredicate();

		//Add fact if or the predicate is not nullptr and or the atom is not negative and then the table is not empty or the atom is negative
		// If the predicate is nullptr add FACT
		if((pred != nullptr && (*atom)->isClassicalLiteral() && ( (!(*atom)->isNegative()&&isNotEmptyPredExt(pred,FACT)) || (*atom)->isNegative()) )
				|| pred == nullptr)
			tableToInsert.push_back({FACT,ALL});

		//Add nofact if the atom is not nullptr and or the atom is not negative is not EDB and the table is not empty, or if the atom is negative
		// If the pred is nullptr check if is an aggregate
		if((pred != nullptr && (*atom)->isClassicalLiteral() && ( (!(*atom)->isNegative()&&!pred->isEdb()&&isNotEmptyPredExt(pred,NOFACT)) || (*atom)->isNegative()) )
				|| (pred == nullptr && (*atom)->isAggregateAtom()))
			tableToInsert.push_back({NOFACT,ALL});

		if((*atom)->isClassicalLiteral() && !(*atom)->isNegative() && tableToInsert.empty())
			return true;
		predicate_searchInsert_table.push_back(tableToInsert);
	}
	return false;
}


bool ProgramGrounder::inizializeSearchInsertPredicate(Rule* rule) {
	predicate_searchInsert_table.clear();
	for(auto atom=rule->getBeginHead();atom!=rule->getEndHead();atom++){
		vector<pair<unsigned,SearchType>> tableToInsert(1,{NOFACT,ALL});
		predicate_searchInsert_table.push_back(tableToInsert);
	}

	return inizializeSearchInsertPredicateBody(rule);
}

bool ProgramGrounder::nextSearchInsertPredicate(Rule* rule,unordered_set<index_object>& componentPredicateInHead,unsigned token,const vector<unsigned>& originalOrderBody) {
	predicate_searchInsert_table.clear();
	for(auto atom=rule->getBeginHead();atom!=rule->getEndHead();++atom){
		vector<pair<unsigned,SearchType>> tableToInsert(1,{NOFACT,ALL});
		predicate_searchInsert_table.push_back(tableToInsert);
	}
	unsigned i=0;
	for(auto atom=rule->getBeginBody();atom!=rule->getEndBody();++atom,++i){
		Predicate* pred=(*atom)->getPredicate();
		vector<pair<unsigned,SearchType>> tableToInsert;

		if(pred!=nullptr){
			//Init the type of ALL (for the non recursive predicate and for recursive predicate > than a toke)
			SearchType typeNF=ALL;
			SearchType typeF=ALL;

			bool isRecursive=componentPredicateInHead.count(pred->getIndex()) && !(*atom)->isNegative();

			if(isRecursive){
				if(originalOrderBody[i]<token)
					typeNF=OLD;
				else if(originalOrderBody[i]==token)
					typeNF=NEW;
			}

			if(isNotEmptyPredExt(pred,FACT) && typeNF!=NEW)
				tableToInsert.push_back({FACT,typeF});
			if(isRecursive && isNotEmptyPredExt(pred,NOFACT,typeNF))
				tableToInsert.push_back({NOFACT,typeNF});
			if(!isRecursive && isNotEmptyPredExt(pred,NOFACT))
				tableToInsert.push_back({NOFACT,ALL});

		}
		else if((*atom)->isAggregateAtom()){
			tableToInsert.push_back({FACT,ALL});
			tableToInsert.push_back({NOFACT,ALL});
		}
		else
			tableToInsert.push_back({FACT,ALL});

		if((*atom)->isClassicalLiteral() && !(*atom)->isNegative() && tableToInsert.empty())
			return true;

		predicate_searchInsert_table.push_back(tableToInsert);
	}

	return false;
}


void ProgramGrounder::swapInDelta(Rule *rule,set_predicate &predicateEvaluated){
	for (auto it = rule->getBeginHead(); it != rule->getEndHead(); it++) {
		set_predicate predicates=(*it)->getPredicates();
		for(auto predicate:predicates){
			PredicateExtension* predicateExt = predicateExtTable->getPredicateExt(predicate);
			if (predicateExt != nullptr && !predicateEvaluated.count(predicate)){
				predicateExt->swapTables(DELTA,NOFACT);
//				predicateExt->swapTables(NEXTDELTA,DELTA);
				predicateExt->swapPointersTables(NEXTDELTA,DELTA);
				predicateEvaluated.insert(predicate);
			}
		}
	}
}

inline void statsGroundRule(bool printStats,bool printRuleTime,Rule *rule,clock_t start,RuleStatistics* rstats){
	if(printStats || printRuleTime){
		clock_t end=Timer::getInstance()->getClock();
		if(printRuleTime){
//			{cerr<<endl<<"RULE ORDERED: \t";rule->print(cerr);}
			Timer::printTimeElapsed(end-start,cerr);
		}if(printStats)
			rstats->setTime(rule->getIndex(),((end-start)/(double) CLOCKS_PER_SEC));
	}
}

bool ProgramGrounder::groundRule(Rule* rule, unordered_set<index_object>* componentPredicateInHead) {
	if(printStats)
		rstats->prepareStats(rule);


	clock_t start=0;

	if(printStats || printRuleTime){
		start=Timer::getInstance()->getClock();
		if(printRuleTime){cerr<<"RULE: ";rule->print(cerr);}
	}

	inizialize(rule,componentPredicateInHead);

	if(rule->getSizeBody()==0){
		foundAssignment();
		statsGroundRule(printStats,printRuleTime,rule,start,rstats);
		return true;
	}

	bool find_assignment=false;

	if(isCartesianProductRule(rule)){
		find_assignment=groundCartesian(rule,componentPredicateInHead);
		statsGroundRule(printStats,printRuleTime,rule,start,rstats);
		return find_assignment;
	}

	bool finish=false;

	while(!finish){

		if(match()){
			if(!next()) {
				if(foundAssignment()){
					find_assignment=true;
				}
				if(!back())
					finish=true;
			}
		}else
			{
				if(!back())
					finish=true;
			}

	}

	statsGroundRule(printStats,printRuleTime,rule,start,rstats);
	return find_assignment;
}

void ProgramGrounder::substituteIndicesInRulesWithPossibleUndefAtoms(){
	for(unsigned i=0;i<rulesWithPossibleUndefAtoms.size();i++){
		Rule* rule=rulesWithPossibleUndefAtoms[i];
		bool ruleSimplified=false;
		for(unsigned possibleUndef:atomsPossibleUndefPositions[i]){
			Atom* atom=rule->getAtomInBody(possibleUndef);
			Atom* atomFound=predicateExtTable->getPredicateExt(atom->getPredicate())->getGroundAtom(atom);
			if(atomFound!=nullptr){
				atom->setIndex(atomFound->getIndex());
				//Simplification now can be done
				//TODO Move in the ground simplification or elsewhere
				if(atomFound->isFact()){
					ruleSimplified=true;
					break;
				}
			}
			else
				rule->setAtomToSimplifyInBody(possibleUndef);
		}
		if(!ruleSimplified)
			outputBuilder->onRule(rule);
		rule->deleteGroundRule();
	}
}

ProgramGrounder::~ProgramGrounder() {
	RuleComplexityEvaluatorFactory::freeInstance();
	IntelligentDecomposer::freeInstance();
	RuleStatistics::freeInstance();
	GroundingPreferences::freeInstance();
	ExtAtomFactory::freeInstance();
	DirectivesHolder::freeInstance();
	SparqlConnection::freeInstance();
	StatementDependency::freeInstance();
	DecompositionMapper::freeInstance();
	outputBuilder->onEnd();
	PredicateExtTable::freeInstance();
	outputBuilder->onPrintAtomTable();
	HashString::freeInstance();
	HashVecInt::freeInstance();
	TermTable::freeInstance();
	PredicateTable::freeInstance();
	IdGenerator::freeInstance();
}

//void ProgramGrounder::createAtomSearchersForPredicateBody(unsigned position, Predicate* predicate, unsigned sizeRule) {
//	PredicateExtension* predicateExtension = predicateExtTable->getPredicateExt(predicate);
//	for(auto table:predicate_searchInsert_table[position]){
//
//		auto atomSearcher=predicateExtension->addAtomSearcher(table,MAP_VECTOR,);
//		predicateExtension->getAtomSearcher(table)->setSizeResultVector(sizeRule);
//		predicate_searchInsert_atomSearcher[position].push_back(atomSearcher);
//	}
//}

void ProgramGrounder::setDefaultAtomSearchers(Rule* rule, unordered_set<index_object>* componentPredicateInHead) {
	unsigned atomIndex=rule->getSizeHead();
	for(auto atom=rule->getBeginBody();atom!=rule->getEndBody();++atom,++atomIndex){
		if((*atom)->isClassicalLiteral()){
			Predicate *predicate=(*atom)->getPredicate();
			predicate_searchInsert_atomSearcher[atomIndex].push_back(vector<IndexingStructure*>());
			createAtomSearchersForPredicateBody(atomIndex, 0, predicate, componentPredicateInHead);
		}
		else if((*atom)->isAggregateAtom()){
			for(unsigned i=0;i<(*atom)->getAggregateElementsSize();++i){
				Predicate *predicate=(*atom)->getAggregateElement(i)->getNafLiteral(0)->getPredicate();
				if(predicate!=nullptr){
					predicate_searchInsert_atomSearcher[atomIndex].push_back(vector<IndexingStructure*>());
					createAtomSearchersForPredicateBody(atomIndex, i, predicate, componentPredicateInHead);
				}
			}
		}
	}
	atomIndex=0;
	for(auto atom=rule->getBeginHead();atom!=rule->getEndHead();++atom,++atomIndex){
		if((*atom)->isClassicalLiteral()){
			Predicate *predicate=(*atom)->getPredicate();
			predicate_searchInsert_atomSearcher[atomIndex].push_back(vector<IndexingStructure*>());
			createAtomSearchersForPredicateHead(atomIndex, 0,  predicate, componentPredicateInHead, true);
		}
		else if((*atom)->isChoice()){
			for(unsigned i=0;i<(*atom)->getChoiceElementsSize();++i){
				Predicate *predicate=(*atom)->getChoiceElement(i)->getFirstAtom()->getPredicate();
				predicate_searchInsert_atomSearcher[atomIndex].push_back(vector<IndexingStructure*>());
				if(predicate!=nullptr)
					createAtomSearchersForPredicateHead(atomIndex, i, predicate, componentPredicateInHead, true);
				if(Options::globalOptions()->getRewritingType()==COMPACT_NATIVE_CHOICE){
					Atom* secondAtomInTheChoice=(*atom)->getChoiceElement(i)->getAtom(1);
					if(secondAtomInTheChoice!=nullptr && secondAtomInTheChoice->getPredicate()!=nullptr){
						createAtomSearchersForPredicateHead(atomIndex, i, secondAtomInTheChoice->getPredicate(), componentPredicateInHead, false);
					}
				}
			}
		}
	}

//	rule->print();
//	cout<<"Size = "<<predicate_searchInsert_atomSearcher.size()<<endl;
//	for(unsigned atom=0;atom<predicate_searchInsert_table.size();atom++){
//		cout<<"Atom: "<<atom<<" --> ";
//		for(auto i:predicate_searchInsert_table[atom]){
//			cout<<i.first<<" ";
//		}
//		cout<<endl;
//	}
//	cout<<endl;
//	for(unsigned atom=0;atom<predicate_searchInsert_atomSearcher.size();atom++){
//		cout<<"Atom: "<<atom<<" --> ";
//		for(auto i:predicate_searchInsert_atomSearcher[atom]){
//			for(auto j:i)
//				if(j!=nullptr)
//					cout<<j->getType()<<" ";
//		}
//		cout<<endl;i
//	}
//	cout<<endl;


}

void ProgramGrounder::groundExitRule(Rule* r) {
	if (nonGroundSimplificator.simplifyRule(r) || inizializeSearchInsertPredicate(r))
		return;
	orderPositiveAtomsBody(r);
	groundRule(r);
}

bool ProgramGrounder::checkIfDecompositionIsConvenient(Rule* r){
	if(!Options::globalOptions()->isEnabledDecomposition()) return false;
	IntelligentDecomposer* decomposer=IntelligentDecomposer::getInstance();
	bool choiceToDecompose=false;
	if(Options::globalOptions()->getDecompositionThreshold()>=0)
		choiceToDecompose=decomposer->choiceToDecomposeWithThreshold(r,Options::globalOptions()->getDecompositionThreshold());
	else
		choiceToDecompose=decomposer->choiceToDecompose(r);
	return choiceToDecompose;
}

bool ProgramGrounder::performDecompositionOfExitRule(Rule* r){
	bool choiceToDecompose=checkIfDecompositionIsConvenient(r);
	DecompositionMapper* decompositionMapper=DecompositionMapper::getInstance();
	if(choiceToDecompose){
		for (auto it = decompositionMapper->getBegin(r);it != decompositionMapper->getEnd(r); ++it)
			groundExitRule(*it);
		groundExitRule(decompositionMapper->get(r));
		return true;
	}
	return false;
}

void ProgramGrounder::expandForWasp(wasp_prop& wp) {
	OutputBuilder* o1 = OutputBuilder::getInstance();
	PreaspOutputBuilder* o = (PreaspOutputBuilder*)(o1);

	for (unsigned i = 0; i < wp.first->getAggregateElementsSize(); i++) {
		AggregateElement* ag = wp.first->getAggregateElement(i);
		if (ag->getTerms().size() > 0) {
			vector<Term*> terms = ag->getTerms();
			set_term set_variables;
			for (auto t : terms)
				t->getVariable(set_variables);
			if (Utils::isContained(set_variables,
					ag->getNafLiteral(0)->getVariable())) {
				map_term<unsigned> positions;
				for (auto t1 : ag->getTerms())
					for (unsigned i = 0;
							i < ag->getNafLiteral(0)->getTermsSize(); ++i)
						if (t1->getIndex()
								== ag->getNafLiteral(0)->getTerm(i)->getIndex())
							positions.insert( { t1, i });
				for (auto it = predicateExtTable->getPredicateExt(
						ag->getNafLiteral(0)->getPredicate())->getTableBegin(
						FACT);
						it
								!= predicateExtTable->getPredicateExt(
										ag->getNafLiteral(0)->getPredicate())->getTableEnd(
										FACT); it++) {
//					writer.appendVariablesToFreeze((*it)->getIndex());
//					writer.appendLiteral((*it)->getIndex());
					vector<Term*> groundTerms;
					for (auto t1 : ag->getTerms()) {
						auto termType = t1->getType();
						if (termType == NUMERIC_CONSTANT
								|| termType == STRING_CONSTANT
								|| termType == SYMBOLIC_CONSTANT)
							groundTerms.push_back(t1);
						else
							groundTerms.push_back(
									(*it)->getTerm(positions[t1]));
					}
					o->appendToDictionary(groundTerms, (*it)->getIndex());
				}
				for (auto it = predicateExtTable->getPredicateExt(
						ag->getNafLiteral(0)->getPredicate())->getTableBegin(
						NOFACT);
						it
								!= predicateExtTable->getPredicateExt(
										ag->getNafLiteral(0)->getPredicate())->getTableEnd(
										NOFACT); it++) {
//					writer.appendVariablesToFreeze((*it)->getIndex());
//					writer.appendLiteral((*it)->getIndex());
					vector<Term*> groundTerms;
					for (auto t1 : ag->getTerms()) {
						auto termType = t1->getType();
						if (termType == NUMERIC_CONSTANT
								|| termType == STRING_CONSTANT
								|| termType == SYMBOLIC_CONSTANT)
							groundTerms.push_back(t1);
						else
							groundTerms.push_back(
									(*it)->getTerm(positions[t1]));
						o->appendToDictionary(groundTerms,
								(*it)->getIndex());
					}
				}
				//				Rule* aux_rule = new Rule;
				//				unsigned id=IdGenerator::getInstance()->getNewId();
				//				string predicate_name=InputRewriter::AUXILIARY+InputRewriter::SEPARATOR+to_string(id);
				//				if(Utils::isContained(set_variables,ag->getNafLiteral(0)->getVariable())){
				//					Atom *atomHead=InputRewriter::generateNewAuxiliaryAtom(predicate_name,terms);
				//					aux_rule->addInHead(atomHead);
				//					aux_rule->addInBody(ag->getNafLiteral(0));
				//					aux_rule->print(cerr);
				//					inizializeSearchInsertPredicate(aux_rule);
				//					orderPositiveAtomsBody(aux_rule);
				//					inizialize(aux_rule,0);
				//					groundCartesian(aux_rule,0);
				//				}
			}
		} else {
			for (auto it = predicateExtTable->getPredicateExt(
					ag->getNafLiteral(0)->getPredicate())->getTableBegin(FACT);
					it
							!= predicateExtTable->getPredicateExt(
									ag->getNafLiteral(0)->getPredicate())->getTableEnd(
									FACT); it++) {
//				writer.appendVariablesToFreeze((*it)->getIndex());
//				writer.appendLiteral((*it)->getIndex());
				o->appendToDictionary((*it)->getTerms(), (*it)->getIndex());
			}
			for (auto it = predicateExtTable->getPredicateExt(
					ag->getNafLiteral(0)->getPredicate())->getTableBegin(
					NOFACT);
					it
							!= predicateExtTable->getPredicateExt(
									ag->getNafLiteral(0)->getPredicate())->getTableEnd(
									NOFACT); it++) {
//				writer.appendVariablesToFreeze((*it)->getIndex());
//				writer.appendLiteral((*it)->getIndex());
				o->appendToDictionary((*it)->getTerms(), (*it)->getIndex());
			}
		}
	}
}


void ProgramGrounder::createPropagatorForWasp(){
	PropagatorsHeuristicsHolder* pwh=PropagatorsHeuristicsHolder::getInstance();
	pwh->setWaspOptions();
	for(auto wp:pwh->getWaspHeuristics())
		expandForWasp(wp);
	for(auto wp:pwh->getWaspAtomsPropagator())
		expandForWasp(wp);
}

}}


