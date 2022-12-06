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
 * GroundingPreferences.cpp
 *
 *  Created on: Mar 10, 2016
 *      Author: jessica
 */

#include "GroundingPreferences.h"
#include "../ground/StatementDependency.h"
#include "OrderRuleGroundable.h"
#include "../ground/DecompositionMapper.h"

namespace DLV2 {
namespace grounder {

GroundingPreferences* GroundingPreferences::groundingPreferences=0;

bool GroundingPreferences::addRuleOrderingType(Rule* rule,unsigned orderingType) {
	if(OrderRuleGroundableFactory::isAValidOrderingType(orderingType)
		&& !rulesOrderingTypes.count(rule->getIndex())){
		rulesOrderingTypes.insert({rule->getIndex(),orderingType});
		return true;
	}
	return false;
}

AnnotationsError GroundingPreferences::addRuleAtomIndexingSetting(Rule* rule, Atom* atom, vector<unsigned>& arguments) {
	vector<unsigned> positions;
	checkIfAtomIsPresentInRule(rule,atom,positions);
	if(positions.empty())
		return ATOM_NOT_PRESENT;
	for(auto atomPos:positions)
		rulesAtomsIndexed[rule->getIndex()].insert({rule->getAtomInBody(atomPos),arguments});
	return OK;
}

AnnotationsError GroundingPreferences::addRulePartialOrderAtom(Rule* rule, Atom* atom) {
	vector<unsigned> positions;
	checkIfAtomIsPresentInRule(rule,atom,positions);
	if(positions.empty())
		return ATOM_NOT_PRESENT;
	rulesPartialOrdersAtoms[rule->getIndex()].back().push_back(rule->getAtomInBody(positions.front()));
	return OK;
}

void GroundingPreferences::clearRulePartialOrder(Rule* rule) {
	unsigned ruleIndex=rule->getIndex();
	rulesPartialOrders.erase(ruleIndex);
	rulesPartialOrdersAtoms.erase(ruleIndex);
}
void GroundingPreferences::swapRulePartialOrder(Rule* originalRule, Rule* newRule) {
	clearRulePartialOrder(originalRule);
	unsigned newRuleIndex=newRule->getIndex();
	unsigned originalRuleIndex=originalRule->getIndex();
	if (rulesPartialOrders.count(newRuleIndex))
		rulesPartialOrders.insert( { originalRuleIndex, rulesPartialOrders.at(newRuleIndex) });
	if (rulesPartialOrdersAtoms.count(newRuleIndex))
		rulesPartialOrdersAtoms.insert({originalRuleIndex,rulesPartialOrdersAtoms.at(newRuleIndex)});
	clearRulePartialOrder(newRule);
}
bool GroundingPreferences::addGlobalOrderingType(unsigned orderingType) {
	if(globalOrderingType==-1 && OrderRuleGroundableFactory::isAValidOrderingType(orderingType)){
		globalOrderingType=orderingType;
		return true;
	}
	return false;
}

bool GroundingPreferences::applyGlobalAtomIndexingSetting() {
	for(unsigned i=0;i<StatementDependency::getInstance()->getRulesSize();++i){
		Rule* rule=StatementDependency::getInstance()->getRule(i);
		for(auto pair:globalAtomsIndexed)
			addRuleAtomIndexingSetting(rule,pair.first,pair.second);
	}
	for(unsigned i=0;i<StatementDependency::getInstance()->getConstraintSize();++i){
		Rule* rule=StatementDependency::getInstance()->getConstraint(i);
		for(auto pair:globalAtomsIndexed)
			addRuleAtomIndexingSetting(rule,pair.first,pair.second);
	}
	for(unsigned i=0;i<StatementDependency::getInstance()->getWeakContraintsSize();++i){
		Rule* rule=StatementDependency::getInstance()->getWeakContraint(i);
		for(auto pair:globalAtomsIndexed)
			addRuleAtomIndexingSetting(rule,pair.first,pair.second);
	}
	return true;
}

void GroundingPreferences::addGlobalAtomIndexingSetting(Atom* atom, vector<unsigned>& arguments) {
	globalAtomsIndexed.insert({atom,arguments});
}

void GroundingPreferences::addGlobalPartialOrderAtomStart(Atom* atom) {
	globalPartialOrdersAtoms.back().push_front(atom);
}

void GroundingPreferences::addGlobalPartialOrderAtomEnd(Atom* atom) {
	globalPartialOrdersAtoms.back().push_back(atom);
}


bool GroundingPreferences::applyGlobalPartialOrder() {
	for(unsigned i=0;i<StatementDependency::getInstance()->getRulesSize();++i){
		Rule* rule=StatementDependency::getInstance()->getRule(i);
		for(auto order:globalPartialOrdersAtoms){
			addRulePartialOrder(rule);
			for(auto atom:order)
				addRulePartialOrderAtom(rule,atom);
			checkRulePartialOrderConflicts(rule);
		}
	}
	for(unsigned i=0;i<StatementDependency::getInstance()->getConstraintSize();++i){
		Rule* rule=StatementDependency::getInstance()->getConstraint(i);
		for(auto order:globalPartialOrdersAtoms){
			addRulePartialOrder(rule);
			for(auto atom:order)
				addRulePartialOrderAtom(rule,atom);
			checkRulePartialOrderConflicts(rule);
		}
	}
	for(unsigned i=0;i<StatementDependency::getInstance()->getWeakContraintsSize();++i){
		Rule* rule=StatementDependency::getInstance()->getWeakContraint(i);
		for(auto order:globalPartialOrdersAtoms){
			addRulePartialOrder(rule);
			for(auto atom:order)
				addRulePartialOrderAtom(rule,atom);
			checkRulePartialOrderConflicts(rule);
		}
	}
	return true;
}

AnnotationsError GroundingPreferences::checkRulePartialOrderConflicts(Rule* rule) {
	rulesPartialOrders[rule->getIndex()].resize(rule->getSizeBody());
	for(unsigned i=0;i<rule->getSizeBody();++i)
		rulesPartialOrders[rule->getIndex()][i].resize(rule->getSizeBody(),false);

	for(unsigned k=0;k<rulesPartialOrdersAtoms[rule->getIndex()].size();++k){
		vector<vector<unsigned>> atomsPositions;
		for(unsigned j=0;j<rulesPartialOrdersAtoms[rule->getIndex()][k].size();++j){
			atomsPositions.emplace_back();
			atomsPositions.back().reserve(rule->getIndex());
			checkIfAtomIsPresentInRule(rule,rulesPartialOrdersAtoms[rule->getIndex()][k][j],atomsPositions.back());
		}

		for(unsigned i=0;i<atomsPositions.size();++i){
			for(auto pB:atomsPositions[i])
				for(unsigned j=i+1;j<atomsPositions.size();++j){
					for(auto pA:atomsPositions[j]){
						if(rulesPartialOrders[rule->getIndex()][pB][pA]){
							rulesPartialOrders[rule->getIndex()].erase(rulesPartialOrders[rule->getIndex()].begin()+j);
							return CONFLICT_FOUND;
						}
					}
				}
		}
	}
	return OK;
}

AnnotationsError DLV2::grounder::GroundingPreferences::applyRulePartialOrder(Rule* rule) {
	if(DecompositionMapper::getInstance()->isDecomposed(rule)) return OK;
	rulesPartialOrders[rule->getIndex()].resize(rule->getSizeBody());
	for(unsigned i=0;i<rule->getSizeBody();++i)
		rulesPartialOrders[rule->getIndex()][i].resize(rule->getSizeBody(),false);
	for(unsigned k=0;k<rulesPartialOrdersAtoms[rule->getIndex()].size();++k){

		vector<vector<unsigned>> atomsPositions;
		for(unsigned j=0;j<rulesPartialOrdersAtoms[rule->getIndex()][k].size();++j){
			atomsPositions.emplace_back();
			atomsPositions.back().reserve(rule->getIndex());
			checkIfAtomIsPresentInRule(rule,rulesPartialOrdersAtoms[rule->getIndex()][k][j],atomsPositions.back());
		}

		for(unsigned i=0;i<atomsPositions.size();++i){
			for(auto pB:atomsPositions[i])
				for(unsigned j=i+1;j<atomsPositions.size();++j)
					for(auto pA:atomsPositions[j]){
						rulesPartialOrders[rule->getIndex()][pA][pB]=true;
					}
		}
	}
	return OK;
}

void GroundingPreferences::setGlobalAnnotations(){
	if(!applayedGlobalAnnotations){
		applyGlobalAtomIndexingSetting();
		for(auto pair:globalAtomsIndexed)
			delete pair.first;
		applyGlobalPartialOrder();
		for(auto order:globalPartialOrdersAtoms)
			for(auto atom:order)
				delete atom;
		applayedGlobalAnnotations=true;
	}
}

int GroundingPreferences::getOrderingType(Rule* r) {
	setGlobalAnnotations();
	int ruleIndex=r->getIndex();
	if(rulesOrderingTypes.count(ruleIndex) && !DecompositionMapper::getInstance()->isDecomposed(r))
		return rulesOrderingTypes.at(ruleIndex);
	return globalOrderingType;
}

bool GroundingPreferences::checkPartialOrder(Rule* rule, unsigned atomPosition, const list<unsigned>& atoms) {
	setGlobalAnnotations();
	unsigned ruleIndex=rule->getIndex();
	if(rulesPartialOrders.count(ruleIndex) && !DecompositionMapper::getInstance()->isDecomposed(rule)){
		const vector<bool>& partialOrder=rulesPartialOrders.at(ruleIndex).at(atomPosition);
		for(unsigned i=0;i<partialOrder.size();++i){
			if(partialOrder[i]){
				bool found=false;
				for(auto j:atoms){
					if(j==i){
						found=true;
						break;
					}
				}
				if(found)
					return false;
			}
		}
		return true;
	}
	return true;
}

bool GroundingPreferences::checkAtomIndexed(Rule* rule, Atom* atom, const vector<unsigned>& possibileArgs,vector<unsigned>& idxTerms) {
	unsigned ruleIndex=rule->getIndex();
	setGlobalAnnotations();
	if(rulesAtomsIndexed.count(ruleIndex) && !DecompositionMapper::getInstance()->isDecomposed(rule)){
		auto it=rulesAtomsIndexed.at(ruleIndex).find(atom);
		if(it==rulesAtomsIndexed.at(ruleIndex).end())
			return false;
		auto arguments=it->second;
		for(unsigned i=0;i<arguments.size();++i){
			bool found=false;
			for(unsigned j=0;j<possibileArgs.size();++j){
				if(possibileArgs[j]==arguments[i]){
					found=true;
					break;
				}
			}
			if(!found){
				return false;
			}
		}
		idxTerms=arguments;
		return true;
	}
	vector<unsigned> commandLineOtp=Options::globalOptions()->getPredicateIndexTerm(atom->getPredicate());
	if(commandLineOtp.empty())
		return false;
	for(unsigned i=0;i<commandLineOtp.size();++i){
		bool found=false;
		for(unsigned j=0;j<possibileArgs.size();++j){
			if(possibileArgs[j]==commandLineOtp[i]){
				found=true;
				break;
			}
		}
		if(!found){
//			cerr<<commandLineOtp[i]<<endl;
			return false;
		}
	}
	idxTerms=commandLineOtp;
	return true;
}

void GroundingPreferences::checkIfAtomIsPresentInRule(Rule* rule, Atom* atom, vector<unsigned>& positions) {
	unsigned i=0;
	for(auto atomIt=rule->getBeginBody();atomIt!=rule->getEndBody();++atomIt,++i){
		if(*(*atomIt)==(*atom))
			positions.push_back(i);
	}
}

void GroundingPreferences::print(Rule* rule) const {
	cout<<"Rule --> ";rule->print();
	if(rulesOrderingTypes.count(rule->getIndex()))
		cout<<"Ordering: "<<rulesOrderingTypes.at(rule->getIndex())<<endl;
	else if(globalOrderingType!=-1)
		cout<<"Global Ordering: "<<globalOrderingType<<endl;
	if(rulesAtomsIndexed.count(rule->getIndex())){
		auto v=rulesAtomsIndexed.at(rule->getIndex());
		for(auto p:v){
			cout<<"Atom: ";p.first->print();cout<<" Indexing Arguments: ";
			for(auto a:p.second){
				cout<<a<<" ";
			}
			cout<<endl;
		}
	}
	cout<<"Partial Order: "<<endl;
	if(rulesPartialOrders.count(rule->getIndex())){
		vector<vector<bool>> b=rulesPartialOrders.at(rule->getIndex());
		for(auto a:b){
			for(auto c:a)
				cout<<c<<" ";
			cout<<endl;
		}
	}
}

} /* namespace grounder */
} /* namespace DLV2 */

