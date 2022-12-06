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
 * OrderRuleGroundable.cpp
 *
 *  Created on: Nov 25, 2015
 *      Author: jessica
 */

#include "OrderRuleGroundable.h"
#include <list>
#include "../../../common/Options.h"
#include "../../../common/Trace.h"
#include "../../util/Utils.h"
#include "../atom/BuiltInAtom.h"

namespace DLV2 {
namespace grounder {

void OrderRuleGroundable::applyBinderSplittingRewriting() {
	set_term variablesWeak;
	if(rule->isWeakConstraint()){
		rule->getLevel()->getVariable(variablesWeak);
		rule->getWeight()->getVariable(variablesWeak);
		for(auto termInLabel:rule->getLabel()){
			termInLabel->getVariable(variablesWeak);
		}
	}
	for (unsigned j = 0; j < rule->getSizeBody(); ++j) {
		Atom* atom = rule->getAtomInBody(j);
		if (atom->isClassicalLiteral() && atom->getPredicate()->isSolved()) {
			for (unsigned t = 0; t < atom->getTermsSize(); ++t) {
				Term* term = atom->getTerm(t);
				if (term->getType() == VARIABLE) {
					if (rule->isAnOutputVariable(term))
						continue;
					if(variablesWeak.count(term))
						continue;

					bool found = false;
					for (unsigned i = 0; i < rule->getSizeBody(); ++i) {
						if(j != i && rule->getAtomInBody(i)->isExternalAtom() && rule->getAtomInBody(i)->getVariable().count(term)){
							found = true;
							break;
						}
						else if (j != i && atomsVariables[i].count(term)) {
							found = true;
							break;
						}
					}
					if (found)
						continue;

					for (unsigned t1 = 0; t1 < atom->getTermsSize(); ++t1) {
						Term* term1 = atom->getTerm(t1);
						if (t1 != t && term1->containsVariable(term)) {
							found = true;
							break;
						}
					}
					if (found)
						continue;

					atom->setTerm(t, TermTable::getInstance()->getTermAnonymous());
				}
			}
		}
	}
}

/****************************************** OrderRuleGroundable ***********************************************/

vector<unsigned> OrderRuleGroundable::order(vector<vector<pair<unsigned,SearchType>>>& predicate_searchInsert_table, vector<vector<vector<IndexingStructure*>>>& predicate_searchInsert_atomSearcher, unordered_set<index_object>* componentPredicateInHead) {
	this->predicate_searchInsert_table=predicate_searchInsert_table;
	unsigned sizeBody=rule->getSizeBody();
	atomsVariables.resize(sizeBody);
	unsigned numPosClassLits=0;

	this->componentPredicateInHead=componentPredicateInHead;

	bool justVariablesAndConstants=true;
	bool justPositiveClassicalOrArith0=true;
	for(unsigned i=0;i<sizeBody;i++){
		Atom* atom=rule->getAtomInBody(i);
		if(atom->getPredicate()==nullptr || atom->isNegative() || !atom->isClassicalLiteral())
			justPositiveClassicalOrArith0=false;
		if(atom->isAggregateAtom()){
			atomsVariables[i]=atom->getSharedVariable(rule->getBeginBody(),rule->getEndBody());
			set_term guards=atom->getGuardVariable();
			atomsVariables[i].insert(guards.begin(),guards.end());
		}
		else if(atom->isExternalAtom() && !atom->isNegative())
			atomsVariables[i] = atom->getVariable();
		else
			atomsVariables[i]=atom->getVariable();
		if(atom->isClassicalLiteral() && !atom->isNegative()){
			if(atom->getPredicate()->getArity()>0)
				numPosClassLits++;
			for(auto v:atom->getTerms()){
				if(!(v->getType()==TermType::VARIABLE || v->getType()==TermType::NUMERIC_CONSTANT
					|| v->getType()==TermType::STRING_CONSTANT || v->getType()==TermType::SYMBOLIC_CONSTANT))
					justVariablesAndConstants=false;
				v->getVariablesInArith(mapPositiveAtomsBoundVariables[i]);
			}
		}
		atomsToInsert.push_back(i);
	}

	vector<Atom*> orderedBody;
	orderedBody.reserve(sizeBody);

	vector<unsigned> orderedPositions;
	orderedPositions.reserve(sizeBody);

	vector<vector<pair<unsigned,SearchType>>> orderdedPredicateSearchInsertTable;
	orderdedPredicateSearchInsertTable.reserve(predicate_searchInsert_table.size());

	orderdedPredicateSearchInsertAtomSearcher.resize(rule->getSizeBody()+rule->getSizeHead());

	unsigned sizeHead=predicate_searchInsert_table.size()-sizeBody;

	for(unsigned i=0;i<sizeHead;++i)
		orderdedPredicateSearchInsertTable.push_back(predicate_searchInsert_table[i]);

//	trace_action_tag(grounding,1,
//		cerr<<"Before Ordering Body and Search/Insert tables are: ";
//		for(unsigned i=0;i<rule->getSizeBody();++i){
//			rule->getAtomInBody(i)->print(cerr);
//			for(auto j:predicate_searchInsert_table[i+sizeHead])
//				cerr<<" "<<j;
//			cerr<<"  ";
//		}
//		cerr<<endl;
//	);

	rule->clearDictionaryIntersection();
	rule->setDictionaryIntersectionSize(rule->getVariablesSize()+2);

	if(rule->getSizeBody()>1)
		GroundingPreferences::getGroundingPreferences()->applyRulePartialOrder(rule);
	bool specialCase=false;
	if(numPosClassLits==2 && justVariablesAndConstants && justPositiveClassicalOrArith0){
		vector<pair<Atom*,unsigned>> classicalLits;
		for(unsigned i=0;i<sizeBody;++i){
			Atom* atom=rule->getAtomInBody(i);
			if(atom->getPredicate()->getArity()>0)
				classicalLits.push_back({atom,i});
		}
		Atom* atom0=classicalLits[0].first;
		unsigned pos0=classicalLits[0].second;
		Atom* atom1=classicalLits[1].first;
		unsigned pos1=classicalLits[1].second;
		if(componentPredicateInHead==nullptr || (componentPredicateInHead!=nullptr && !componentPredicateInHead->count(atom0->getPredicate()->getIndex())
				&& !componentPredicateInHead->count(atom1->getPredicate()->getIndex()))
			){
			if(computePredicateExtensionSize(pos0,atom0->getPredicate())<computePredicateExtensionSize(pos1,atom1->getPredicate())){
				vector<unsigned> indexingTerm;
				for(unsigned i=0;i<atom1->getTermsSize();++i){
					Term* t=atom1->getTerm(i);
					if((t->getType()==TermType::VARIABLE && atomsVariables[pos0].count(t)) || t->getType()==TermType::NUMERIC_CONSTANT
							|| t->getType()==TermType::STRING_CONSTANT || t->getType()==TermType::SYMBOLIC_CONSTANT)
						indexingTerm.push_back(i);
				}
				if(indexingTerm.size()==1 && !predicateExtTable->getPredicateExt(atom1->getPredicate())->isAlreadyCreatedAtomSearcher(MAP,&indexingTerm,false)){
					specialCase=true;
					for(unsigned i=0;i<sizeBody;++i){
						Atom* atom=rule->getAtomInBody(i);
						if(atom->getPredicate()->getArity()==0){
							orderdedPredicateSearchInsertTable.push_back(predicate_searchInsert_table[sizeHead+i]);
							setAtomSearcher(atom,i,orderedBody.size());
							orderedBody.push_back(atom);
							orderedPositions.push_back(i);
							if(Options::globalOptions()->isEnabledDictionaryIntersection() || GroundingPreferences::getGroundingPreferences()->getAlignSubstitutions(rule))
								computeDictionaryIntersection(atom);
						}
					}
					orderdedPredicateSearchInsertTable.push_back(predicate_searchInsert_table[sizeHead+pos1]);
					setAtomSearcher(atom1,pos1,orderedBody.size());
					orderedBody.push_back(atom1);
					orderedPositions.push_back(pos1);
					if(Options::globalOptions()->isEnabledDictionaryIntersection() || GroundingPreferences::getGroundingPreferences()->getAlignSubstitutions(rule))
						computeDictionaryIntersection(atom1);
					orderdedPredicateSearchInsertTable.push_back(predicate_searchInsert_table[sizeHead+pos0]);
					setAtomSearcher(atom0,pos0,orderedBody.size());
					orderedBody.push_back(atom0);
					orderedPositions.push_back(pos0);
					if(Options::globalOptions()->isEnabledDictionaryIntersection() || GroundingPreferences::getGroundingPreferences()->getAlignSubstitutions(rule))
						computeDictionaryIntersection(atom0);
				}
			}
			else if(computePredicateExtensionSize(pos0,atom0->getPredicate())>computePredicateExtensionSize(pos1,atom1->getPredicate())){
					vector<unsigned> indexingTerm;
					for(unsigned i=0;i<atom0->getTermsSize();++i){
						Term* t=atom0->getTerm(i);
						if((t->getType()==TermType::VARIABLE && atomsVariables[pos1].count(t)) || t->getType()==TermType::NUMERIC_CONSTANT
								|| t->getType()==TermType::STRING_CONSTANT || t->getType()==TermType::SYMBOLIC_CONSTANT)
							indexingTerm.push_back(i);
					}
					if(indexingTerm.size()==1 && !predicateExtTable->getPredicateExt(atom0->getPredicate())->isAlreadyCreatedAtomSearcher(MAP,&indexingTerm,false)){
						specialCase=true;
						for(unsigned i=0;i<sizeBody;++i){
							Atom* atom=rule->getAtomInBody(i);
							if(atom->getPredicate()->getArity()==0){
								orderdedPredicateSearchInsertTable.push_back(predicate_searchInsert_table[sizeHead+i]);
								setAtomSearcher(atom,i,orderedBody.size());
								orderedBody.push_back(atom);
								orderedPositions.push_back(i);
								if(Options::globalOptions()->isEnabledDictionaryIntersection() || GroundingPreferences::getGroundingPreferences()->getAlignSubstitutions(rule))
									computeDictionaryIntersection(atom);
							}
						}
						orderdedPredicateSearchInsertTable.push_back(predicate_searchInsert_table[sizeHead+pos0]);
						setAtomSearcher(atom0,pos0,orderedBody.size());
						orderedBody.push_back(atom0);
						orderedPositions.push_back(pos0);
						if(Options::globalOptions()->isEnabledDictionaryIntersection() || GroundingPreferences::getGroundingPreferences()->getAlignSubstitutions(rule))
							computeDictionaryIntersection(atom0);
						orderdedPredicateSearchInsertTable.push_back(predicate_searchInsert_table[sizeHead+pos1]);
						setAtomSearcher(atom1,pos1,orderedBody.size());
						orderedBody.push_back(atom1);
						orderedPositions.push_back(pos1);
						if(Options::globalOptions()->isEnabledDictionaryIntersection() || GroundingPreferences::getGroundingPreferences()->getAlignSubstitutions(rule))
							computeDictionaryIntersection(atom1);
					}
			}
		}
	}

	if(!specialCase){
		while(!atomsToInsert.empty()){
			list<unsigned>::iterator bestAtom=assignWeights();
			Atom* atom=rule->getAtomInBody((*bestAtom));
			orderdedPredicateSearchInsertTable.push_back(predicate_searchInsert_table[sizeHead+*bestAtom]);
			if(atom->isClassicalLiteral())
				setAtomSearcher(atom,*bestAtom,orderedBody.size());
			orderedBody.push_back(atom);
			orderedPositions.push_back(*bestAtom);
			if((Options::globalOptions()->isEnabledDictionaryIntersection() || GroundingPreferences::getGroundingPreferences()->getAlignSubstitutions(rule)) && atom->isClassicalLiteral() && !atom->isNegative())
				computeDictionaryIntersection(atom);
			variablesInTheBody.insert(atomsVariables[*bestAtom].begin(),atomsVariables[*bestAtom].end());
			atomInserted.push_back(*bestAtom);
			atomsToInsert.erase(bestAtom);
		}
	}

	if(!Options::globalOptions()->isDisabledAnonymousFilter())
		applyBinderSplittingRewriting();

	rule->setBody(orderedBody);
	predicate_searchInsert_table=orderdedPredicateSearchInsertTable;
	predicate_searchInsert_atomSearcher=orderdedPredicateSearchInsertAtomSearcher;

//	trace_action_tag(grounding,1,
//		cerr<<"After Ordering Body and Search/Insert tables are: ";
//		for(unsigned i=0;i<rule->getSizeBody();++i){
//			rule->getAtomInBody(i)->print(cerr);
//			for(auto j:orderdedPredicateSearchInsertTable[i+sizeHead])
//				cerr<<" "<<j;
//			cerr<<"  ";
//		}
//		cerr<<endl;
//	);

	return orderedPositions;
}

void OrderRuleGroundable::order(vector<vector<pair<unsigned,SearchType>> >& predicate_searchInsert_table, vector<vector<vector<IndexingStructure*>>>& predicate_searchInsert_atomSearcher, vector<unsigned>& originalOrderBody, unordered_set<index_object>* componentPredicateInHead) {
	vector<unsigned> orderedPositions=order(predicate_searchInsert_table,predicate_searchInsert_atomSearcher,componentPredicateInHead);
	vector<unsigned> newOriginalOrderBody;
	newOriginalOrderBody.resize(originalOrderBody.size());
	for(unsigned i=0;i<orderedPositions.size();++i){
		unsigned pos=orderedPositions[i];
		newOriginalOrderBody[i]=originalOrderBody[pos];
	}
	originalOrderBody=newOriginalOrderBody;
}

bool OrderRuleGroundable::isBound(Atom* atom, unsigned orginalPosition, const set_term& terms) {
	if(atom->isBuiltIn() && atom->getBinop()==Binop::EQUAL){

		if(atom->plusMinusBuiltin()){
			//If the builtin is an = and contain only + and - operators, then we count only the total variable
			// of the builtin and if only 1 variable is free we can put this atom because even if the free variable is in
			// arithmetic operators we can calculate like an equation
			set_term variablesInAtom=atom->getVariable();
			unsigned count=0;
			for(auto var:variablesInAtom){
				if(!terms.count(var))
					count++;
				if(count>1)break;
			}
			if(count<=1){
				atom->setAssignment(count==1);
				return true;
			}
		}else{
			// If the builtin contain also * or / operator, because ww don't have float constant we cannot solve the equation and then
			// the free variable must be alone in first or second term in the atom
			set_term varsFirst;
			atom->getTerm(0)->getVariable(varsFirst);
			if(Utils::isContained(varsFirst,terms) && atom->getTerm(1)->getType()==VARIABLE && !terms.count(atom->getTerm(1))){
				atom->setAssignment(true);
				return true;
			}
			set_term varSecond;
			atom->getTerm(1)->getVariable(varSecond);
			if(Utils::isContained(varSecond,terms) && atom->getTerm(0)->getType()==VARIABLE && !terms.count(atom->getTerm(0))){
				atom->setAssignment(true);

				return true;
			}
			if(Utils::isContained(atomsVariables[orginalPosition],terms)){
				atom->setAssignment(false);
				return true;
			}
		}

		return false;
	}
	else if(atom->isAggregateAtom()){
		set_term variables=atom->getSharedVariable(rule->getBeginBody(),rule->getEndBody());
		set_term guards=atom->getGuardVariable();
		if(!atom->isAssignment())
			variables.insert(guards.begin(),guards.end());
		else
			variables.erase(atom->getFirstGuard());
		return Utils::isContained(variables,terms);
	}
	else if(mapPositiveAtomsBoundVariables[orginalPosition].size()>0){
		return !(atom->containsAnonymous()) && Utils::isContained(mapPositiveAtomsBoundVariables[orginalPosition],terms);
	}
	return !(atom->containsAnonymous()) && Utils::isContained(atomsVariables[orginalPosition],terms);
}

void OrderRuleGroundable::computeDictionaryIntersection(Atom* atom) {
	for(unsigned i=0;i<atom->getTermsSize();++i){
		Term* var=atom->getTerm(i);
		if(var->getType()==TermType::VARIABLE && rule->getRuleInformation().isAJoinVariable(var)){
			index_object localIndex=var->getLocalVariableIndex();
			if(variablesInTheBody.count(var)){
				for(auto it=rule->getDictionaryIntersectionBegin(localIndex);it!=rule->getDictionaryIntersectionEnd(localIndex);){
					if(predicateExtTable->getPredicateExt(atom->getPredicate())->getPredicateInformation()->isPresent(i,*it))
						it++;
					else
						rule->removeInDictionaryIntersection(it,localIndex);
				}
			}
			else{
				const set_term& set=predicateExtTable->getPredicateExt(atom->getPredicate())->getPredicateInformation()->getDictionary(i);
				rule->insertInDictionaryIntersection(localIndex,set);
			}
		}
	}
}


/****************************************** AllOrderRuleGroundable ***********************************************/

unsigned OrderRuleGroundable::computePredicateExtensionSize(unsigned atomPosition, Predicate* p) {
	if(p==0) return 0;
	unsigned extensionSize = 0;
	for (auto j : predicate_searchInsert_table[atomPosition + rule->getSizeHead()])
		if(componentPredicateInHead!=nullptr && componentPredicateInHead->count(p->getIndex()) && !rule->getAtomInBody(atomPosition)->isNegative())
			extensionSize += predicateExtTable->getInstance()->getPredicateExt(p)->getPredicateExtentionSize(j.first,j.second);
		else
			extensionSize += predicateExtTable->getInstance()->getPredicateExt(p)->getPredicateExtentionSize(j.first);
	return extensionSize;
}


list<unsigned>::iterator AllOrderRuleGroundable::assignWeights() {
	double bestWeight=LLONG_MAX;
	list<unsigned>::iterator bestAtomIt=atomsToInsert.begin();
	unsigned bestAtomExtensionSize=0;
	for(list<unsigned>::iterator it=atomsToInsert.begin();it!=atomsToInsert.end();++it){
		if(!GroundingPreferences::getGroundingPreferences()->checkPartialOrder(rule,*it,atomsToInsert) )
			continue;
		double weight=INT_MAX;
		Atom* atom=rule->getAtomInBody(*it);
		trace_action_tag(grounding,2,
			atom->print(cerr);
		);

		bool bound=false;
		if(atom->isExternalAtom() && Utils::isContained(atomsVariables[*it],variablesInTheBody))
			weight=assignWeightExternalAtom(atom,*it);
		else{
			bool bound=isBound(atom,*it,variablesInTheBody);
			if(atom->isClassicalLiteral() && !atom->isNegative() && ((!bound && mapPositiveAtomsBoundVariables[*it].empty()) || (bound && mapPositiveAtomsBoundVariables[*it].size()>0))){
				// If in a positive classical literal all variables that must be bound are bound then it can be safely added to the new body
				weight=assignWeightPositiveClassicalLit(atom,*it);
			}
			else if(bound){
				if(atom->isClassicalLiteral()){
					if(atom->isNegative())
						weight=assignWeightNegativeClassicalLit(atom,*it);
					else
						weight=assignWeightBoundPositiveClassicalLit(atom,*it);
				}
				else if(atom->isBuiltIn()){
					weight=assignWeightBuiltInAtom(atom,*it);
				}
				else if(atom->isAggregateAtom()){
					weight=assignWeightAggregateAtom(atom,*it);
				}
			}
			else
				continue;
		}

			trace_action_tag(grounding,2,
				cerr<<" Weight: "<<weight<<endl;
			);

		if(weight<bestWeight){
			bestWeight=weight;
			bestAtomIt=it;
			if(!bound)
				bestAtomExtensionSize = manageEqualWeights(*bestAtomIt); //computePredicateExtensionSize(*bestAtomIt, p);
		}
//		If two atoms have the same weight we prefer the one with the lower extension size
		else if(!bound && ckeckSimilarity(weight,bestWeight)){
			double secondaryWeight=manageEqualWeights(*it);
			if(secondaryWeight<bestAtomExtensionSize){
				bestWeight=weight;
				bestAtomIt=it;
				bestAtomExtensionSize=secondaryWeight;
			}
		}
	}
	//TODO Add all bound atoms all together and avoid the weigh update if no bind atom has been added
	update(rule->getAtomInBody(*bestAtomIt),*bestAtomIt);
	trace_action_tag(grounding,2,
		cerr<<"Chosen atom: ";rule->getAtomInBody(*bestAtomIt)->print(cerr);cerr<<" "<<bestWeight<<endl;
	);

	return bestAtomIt;
}

double AllOrderRuleGroundable::manageEqualWeights(unsigned originalPosition) {
	Atom* atom=rule->getAtomInBody(originalPosition);
	Predicate* predicate=atom->getPredicate();
	if(predicate!=nullptr)
		return computePredicateExtensionSize(originalPosition, predicate);
	return 0;
}

/****************************************** CombinedCriterion ***********************************************/

void CombinedCriterion::computeVariablesDomains() {
	for(auto it=rule->getBeginBody();it!=rule->getEndBody();++it){
		Atom* atom=*it;
		if(atom->isClassicalLiteral() && !atom->isNegative()){
			for(unsigned i=0;i<atom->getTermsSize();++i){
				Term* term=atom->getTerm(i);
				unsigned selectivity=0;
				if(term->getType()==TermType::VARIABLE)
					selectivity=predicateExtTable->getPredicateExt(atom->getPredicate())->getPredicateInformation()->getSelectivity(i);
//					else if(term==TermType::FUNCTION){ //TODO
//
//					}
				auto it=variablesDomains.find(term);
				if(it!=variablesDomains.end()){
					if(selectivity>(*it).second)
						variablesDomains[term]=selectivity;
				}
				else
					variablesDomains[term]=selectivity;
			}
		}
	}
}

int estimateSelection(unsigned orginalSelectivity, int n, int max, int min){
	return ((n+abs(min))/(abs(max)+abs(min)))*orginalSelectivity;
}

void CombinedCriterion::updateVariableSelectivity(Atom* atomAdded, unsigned atomPos) {
	if(atomAdded->isClassicalLiteral() && !atomAdded->isNegative()){
		PredicateInformation* predInfo=predicateExtTable->getPredicateExt(atomAdded->getPredicate())->getPredicateInformation();
		for(unsigned i=0;i<atomAdded->getTermsSize();++i){
			Term* term=atomAdded->getTerm(i);
			if(term->getType()==TermType::VARIABLE){
				unsigned selectivity=predInfo->getSelectivity(i);
//				for(auto it=rule->getBeginBody();it!=rule->getEndBody();++it){
//					if((*it)->isBuiltIn() && (*it)->isComparisonBuiltIn()){
//						if((*it)->getTerm(0)->getIndex()==term->getIndex()){
//							selectivity=estimateSelection(selectivity,(*it)->getTerm(1)->getConstantValue(),predInfo->getMax(i),predInfo->getMin(i));
//							cout<<"Selectivity: "<<selectivity<<endl;
//						}
//						else if((*it)->getTerm(1)->getIndex()==term->getIndex()){
//							selectivity=estimateSelection(selectivity,(*it)->getTerm(0)->getConstantValue(),predInfo->getMax(i),predInfo->getMin(i));
//							cout<<"Selectivity: "<<selectivity<<endl;
//						}
//					}
//				}
				if(variablesInTheBody.count(term) && variablesDomains[term]>0)
					variablesSelectivities[term]*=selectivity/variablesDomains[term];
				else
					variablesSelectivities[term]=selectivity;
			}
//			else if(term==TermType::FUNCTION){ //TODO
//
//			}
		}
	}
}

double CombinedCriterion::assignWeightPositiveClassicalLit(Atom* atom, unsigned originalPosition, int size) {
	if(variablesDomains.empty())
		computeVariablesDomains();

	if(size==-1)
		size=computePredicateExtensionSize(originalPosition,atom->getPredicate());

	long double prodSelectivity_a=1;
	long unsigned prodDomains_a=1;
	long double prodSelectivity_b=1;
	long unsigned prodDomains_b=1;
	set_term variablesFound;
	for(unsigned i=0;i<atom->getTermsSize();++i){
		Term* var=atom->getTerm(i);
		if(var->getType()==TermType::VARIABLE){
			if(!variablesFound.insert(var).second) continue;
			if(variablesInTheBody.count(var)){
				prodSelectivity_a*=variablesSelectivities[var]/variablesDomains[var];
				prodDomains_a*=variablesDomains[var];
				prodSelectivity_b*=predicateExtTable->getPredicateExt(atom->getPredicate())->getPredicateInformation()->getSelectivity(i);
				prodDomains_b*=variablesDomains[var]*variablesDomains[var];
			}
			else{
				for(unsigned j=0;j<rule->getSizeBody();++j){
					if(j!=originalPosition && atomsVariables[j].count(var)){
						prodDomains_a*=variablesDomains[var];
						break;
					}
				}

			}
		}
//			else if(term==TermType::FUNCTION){ //TODO
//
//			}
	}

	double sel_a=size*prodSelectivity_a/prodDomains_a;
	double sel_b=prodSelectivity_b/prodDomains_b;

//	atom->print();cout<<" ";
//	cout<<sel_a<<" "<<sel_b<<" "<<endl;
	return sel_a*sel_b;

}

double CombinedCriterion::computeBestIndexingTerms(Atom* atom, unsigned originalPosition) {
	unsigned sizeTablesToSearch=computePredicateExtensionSize(originalPosition,atom->getPredicate());

	double max=0;
	double secondMax=0;
	double bestIndex=1;
	unsigned bestTerm=0;
	unsigned secondBestTerm=0;
	positiveAtomsIndexingTerms[originalPosition].clear();
//	atom->print();cout<<endl;
	for(unsigned i=0;i<atom->getTermsSize();++i){
		if(!variablesInTerms[originalPosition][i].empty() && Utils::isContained(variablesInTerms[originalPosition][i],variablesInTheBody)){
//			atom->getTerm(i)->print();cout<<" "<<boundArgumentsSelectivities[originalPosition][i]<<endl;
			if(boundArgumentsSelectivities[originalPosition][i]>max){
				secondBestTerm=bestTerm;
				bestTerm=i;
				secondMax=max;
				max=boundArgumentsSelectivities[originalPosition][i];
			}
			else if(boundArgumentsSelectivities[originalPosition][i]==max || boundArgumentsSelectivities[originalPosition][i]>secondMax){
				secondMax=boundArgumentsSelectivities[originalPosition][i];
				secondBestTerm=i;
			}
		}
	}
	if(max>0 && secondMax>0){ // && (1-max/sizeTablesToSearch)<DOUBLE_INDEX_THRESHOLD){
		bestIndex=(1-((max*secondMax)/sizeTablesToSearch));
		positiveAtomsIndexingTerms[originalPosition].push_back(bestTerm);
		positiveAtomsIndexingTerms[originalPosition].push_back(secondBestTerm);
	}
	else if(max>0){
		bestIndex=(1-max/sizeTablesToSearch);
		positiveAtomsIndexingTerms[originalPosition].push_back(bestTerm);
	}
	return bestIndex;
}

double CombinedCriterion::computeBoundAtoms(Atom* atom, unsigned originalPosition) {
	unsigned numBoundAtoms=0;
	set_term set=variablesInTheBody;
	set.insert(atomsVariables[originalPosition].begin(),atomsVariables[originalPosition].end());
	for(auto a:atomsToInsert){
		if(isBound(rule->getAtomInBody(a),a,set)){
			numBoundAtoms++;
		}
	}
	return (1-numBoundAtoms/atomsToInsert.size());
}

double CombinedCriterion::computeOutputVariablesBounded(Atom* atom, unsigned originalPosition){
	double prod_selectivity_output=1;
	double prod_domains_output=1;
	set_term variablesFound;
	for(unsigned i=0;i<atom->getTermsSize();++i){
		Term* var=atom->getTerm(i);
		set_term variables;
		atom->getTerm(i)->getVariable(variables);
		for(auto v:variables){
			if(!variablesFound.insert(var).second) continue;
			if(!rule->isAnOutputVariable(v)) continue;
			prod_selectivity_output*=predicateExtTable->getPredicateExt(atom->getPredicate())->getPredicateInformation()->getSelectivity(i);
			prod_domains_output*=pow(variablesDomains[var],2);
		}
	}
	return (prod_selectivity_output/prod_domains_output);
}

double CombinedCriterion1::assignWeightPositiveClassicalLit(Atom* atom, unsigned originalPosition, int s) {
	if(variablesDomains.empty())
		computeVariablesDomains();

	unsigned sizeTablesToSearch=computePredicateExtensionSize(originalPosition,atom->getPredicate());

	long double num1=1;
	long unsigned  den1=1;
	long unsigned den2=1;
	for(unsigned i=0;i<atom->getTermsSize();++i){
		Term* var=atom->getTerm(i);
		if(var->getType()==TermType::VARIABLE){
			if(variablesInTheBody.count(var)){
				num1*=variablesSelectivities[var]*predicateExtTable->getPredicateExt(atom->getPredicate())->getPredicateInformation()->getSelectivity(i);
				den1=pow(variablesDomains[var],4);
			}
			else{
				for(unsigned j=0;j<rule->getSizeBody();++j){
					if(j!=originalPosition && atomsVariables[j].count(var)){
						den2=pow(variablesDomains[var],2);
						break;
					}
				}

			}
		}
	}
	return sizeTablesToSearch*(num1/den1)*(1/den2);
}

double CombinedCriterion3::assignWeightPositiveClassicalLit(Atom* atom, unsigned originalPosition, int s){
	double sel_c=CombinedCriterion::assignWeightPositiveClassicalLit(atom,originalPosition);
	long unsigned sel_f=1;
	for(unsigned i=0;i<atom->getTermsSize();++i){
		Term* t=atom->getTerm(i);
		if(t->getType()==TermType::FUNCTION){
			set_term variablesInFunction;
			t->getVariable(variablesInFunction);
			for(auto v:variablesInFunction){
				if(!variablesInTheBody.count(v)){
					if(variablesDomains.count(v))
						sel_f*=variablesDomains[v];
					else
						sel_f*=predicateExtTable->getPredicateExt(atom->getPredicate())->getPredicateInformation()->getSelectivity(i);
				}
			}
		}
	}
	return sel_c*sel_f;
}

double CombinedCriterion4::assignWeightPositiveClassicalLit(Atom* atom, unsigned originalPosition, int s) {
	double sel_c=CombinedCriterion::assignWeightPositiveClassicalLit(atom,originalPosition);
	long unsigned sel_f=0;
	for(unsigned i=0;i<atom->getTermsSize();++i){
		Term* t=atom->getTerm(i);
		if(t->getType()==TermType::FUNCTION){
			set_term variablesInFunction;
			t->getVariable(variablesInFunction);
			for(auto v:variablesInFunction){
				if(!variablesInTheBody.count(v)){
					if(variablesDomains.count(v))
						sel_f+=variablesDomains[v];
					else
						sel_f+=predicateExtTable->getPredicateExt(atom->getPredicate())->getPredicateInformation()->getSelectivity(i);
				}
			}
		}
	}
	if(sel_f>0)
		return sel_c*sel_f;
	return sel_c;
}

double CombinedCriterion5::manageEqualWeights(unsigned originalPosition){
	Atom* atom=rule->getAtomInBody(originalPosition);
	unsigned numVar=this->rule->getVariablesSize();
	double size=CombinedCriterion::manageEqualWeights(originalPosition);
	double similarity=estimateSizeSimilarity(originalPosition);
	set_term variablesShared;
	unsigned occurencies=countVariablesOccurencies(originalPosition,variablesShared);
//	for(auto v:variablesShared){
//		v->print(cerr);
//		cerr<<" ";
//	}
//	cerr<<endl;

	if(atomsToInsert.size()==rule->getSizeBody() && similarity>=SIZE_SIMILARITY_THRESHOLD){
		double indexBestGoodness=0;
		for(unsigned i=0;i<atom->getTermsSize();++i){
			Term* t=atom->getTerm(i);
			if(t->getType()==VARIABLE && variablesShared.count(t)){
				unsigned sel=predicateExtTable->getPredicateExt(atom->getPredicate())->getPredicateInformation()->getSelectivity(i);
//				t->print(cerr);
//				cerr<<" "<<sel<<endl;
				double indexGoodness=sel/size;
				if(indexGoodness>indexBestGoodness)
					indexBestGoodness=indexGoodness;
			}
		}
//		cerr<<" Best Index weight: "<<indexBestGoodness<<endl;
//		cerr<<" Weight: "<<((numVar-occurencies)*maximumSize*indexBestGoodness)<<endl;
		return ((numVar-occurencies)*maximumSize*indexBestGoodness);
	}
	return (numVar-occurencies)*size;
}

unsigned CombinedCriterion5::countVariablesOccurencies(unsigned originalPosition, set_term& variablesShared) {
	Atom* atom=rule->getAtomInBody(originalPosition);
	unsigned numOccurrencies=0;
	if(atom->isClassicalLiteral() && !atom->isNegative()){
		set_term variables=atom->getVariable();
		for(auto var:variables){
			for(auto a:this->atomsToInsert){
				if(originalPosition==a)continue;
				for(auto v:atomsVariables[a])
					if(v->getIndex()==var->getIndex()){
						variablesShared.insert(var);
						numOccurrencies++;
					}
			}
		}
	}
//	atom->print();cout<<" "<<numOccurrencies<<endl;
	return numOccurrencies;
}

double CombinedCriterion5::estimateSizeSimilarity(unsigned originalPosition) {
	Atom* atom=rule->getAtomInBody(originalPosition);
	if(atom->isClassicalLiteral() && !atom->isNegative()){
		unsigned size=computePredicateExtensionSize(originalPosition,atom->getPredicate());
		return double(size)/maximumSize;
	}
	return 0;
}

double CombinedCriterion5::assignWeightPositiveClassicalLit(Atom* atom, unsigned originalPosition, int s) {
//	cerr<<"Atom: ";atom->print(cerr);cerr<<endl;
	if(atomsToInsert.size()==rule->getSizeBody()){
		if(maximumSize==0 && minimumSize==INT_MAX){
			unsigned i=0;
			for(auto it=rule->getBeginBody();it!=rule->getEndBody();++it,++i){
				Predicate* p=(*it)->getPredicate();
				if(p!=nullptr){
					unsigned size=computePredicateExtensionSize(i,p);
					if(size>maximumSize){
						posMax=i;
						maximumSize=size;
					}
					if(size<minimumSize){
						posMin=i;
						minimumSize=size;
					}
				}
			}
//			cerr<<"MAX: "<<maximumSize<<" "<<posMax<<endl;
//			cerr<<"MIN: "<<minimumSize<<" "<<posMin<<endl;
		}
//		if(originalPosition!=posMax && originalPosition!=posMin){
			double similarity=estimateSizeSimilarity(originalPosition);
			if(similarity>=SIZE_SIMILARITY_THRESHOLD){
				double sel_c=CombinedCriterion::assignWeightPositiveClassicalLit(atom,originalPosition,maximumSize);
//				cerr<<"SIMIL: "<<similarity<<" "<<sel_c<<endl;
				return sel_c;
			}
//		}
	}
	double sel_c=CombinedCriterion::assignWeightPositiveClassicalLit(atom,originalPosition);
//	cerr<<"NO: "<<sel_c<<endl;
	return sel_c;

}




//double IndexingArgumentsOrderRuleGroundable::manageEqualWeights(unsigned originalPosition) {
//	Atom* atom=rule->getAtomInBody(originalPosition);
//	Predicate* predicate=atom->getPredicate();
//	if(predicate==nullptr)
//		return 0;
//
//	PredicateExtension* predicateExtension=PredicateExtTable::getInstance()->getPredicateExt(atom->getPredicate());
//	double forwardWeight = predicateExtension->getPredicateExtentionSize();
//	for (auto j : atomsToInsert) {
//		Atom* a = rule->getAtomInBody(j);
//		if (j == originalPosition
//				|| !(a->isClassicalLiteral() && !a->isNegative())
//				|| a->getPredicate()->getArity() <= 1)
//			continue;
//
//		PredicateExtension* predicateExtension =
//				PredicateExtTable::getInstance()->getPredicateExt(
//						a->getPredicate());
//		unsigned s = predicateExtension->getPredicateExtentionSize();
//		double maxOther = 0;
//		double secondMaxOther = 0;
//		double bestIndex = 0;
//		for (unsigned i = 0; i < a->getTermsSize(); ++i) {
//			bool bound = true;
//			for (auto v : variablesInTerms[j][i]) {
//				if (!variablesInTheBody.count(v)) {
//					if (!atomsVariables[originalPosition].count(v))
//						bound = false;
//				}
//			}
//			if (bound && boundArgumentsSelectivities[j][i] > maxOther) {
//				secondMaxOther = maxOther;
//				maxOther = boundArgumentsSelectivities[j][i];
//			} else if (boundArgumentsSelectivities[j][i] == maxOther) {
//				secondMaxOther = maxOther;
//			}
//		}
//		if (maxOther > 0 && secondMaxOther > 0
//				&& (maxOther / s) < DOUBLE_INDEX_THRESHOLD) {
//			bestIndex = (1 - ((maxOther * secondMaxOther) / s));
//		} else
//			bestIndex = (1 - maxOther / s);
//
//		forwardWeight *= bestIndex;
//	}
//
//	return forwardWeight;
//}

double IndexingArgumentsOrderRuleGroundable::assignWeightPositiveClassicalLit(Atom* atom, unsigned originalPosition, int s) {
	if(boundArgumentsSelectivities.empty())
		computeBoundArgumentsSelectivities();

//	cout<<"-->";atom->print();cout<<endl;
	unsigned size=computePredicateExtensionSize(originalPosition,atom->getPredicate());
	double max=0;
	double secondMax=0;
	double backwardWeight=currentJoinSize;
	if(currentJoinSize==1)
		backwardWeight=size;
	else{
		for(unsigned i=0;i<atom->getTermsSize();++i){
			if(Utils::isContained(variablesInTerms[originalPosition][i],variablesInTheBody)){
				if(boundArgumentsSelectivities[originalPosition][i]>max){
					secondMax=max;
					max=boundArgumentsSelectivities[originalPosition][i];
				}
				else if(boundArgumentsSelectivities[originalPosition][i]==max){
					secondMax=max;
				}
			}
		}
		if(max>0 && secondMax>0 && (max/size)<DOUBLE_INDEX_THRESHOLD){
			backwardWeight*=(((max*secondMax)/size));
		}
		else
			backwardWeight*=(max/size);
	}

	backwardWeights[originalPosition]=backwardWeight;

//	cout<<"BACK: "<<backwardWeights[originalPosition]<<endl;

	double forwardWeight = 1; //backwardWeights[originalPosition]; //predicateExtension->getPredicateExtentionSize();
	for (auto j : atomsToInsert) {
		Atom* a = rule->getAtomInBody(j);
		if (j == originalPosition
				|| !(a->isClassicalLiteral() && !a->isNegative())
				|| a->getPredicate()->getArity() <= 1)
			continue;

		unsigned s = computePredicateExtensionSize(originalPosition,atom->getPredicate());
		double maxOther = 0;
		double secondMaxOther = 0;
		double bestIndex = 0;
		for (unsigned i = 0; i < a->getTermsSize(); ++i) {
			bool bound = true;
			for (auto v : variablesInTerms[j][i]) {
				if (!variablesInTheBody.count(v)) {
					if (!atomsVariables[originalPosition].count(v))
						bound = false;
				}
			}
			if (bound && boundArgumentsSelectivities[j][i] > maxOther) {
				secondMaxOther = maxOther;
				maxOther = boundArgumentsSelectivities[j][i];
			} else if (boundArgumentsSelectivities[j][i] == maxOther) {
				secondMaxOther = maxOther;
			}
		}
		if (maxOther > 0 && secondMaxOther > 0
				&& (maxOther / s) < DOUBLE_INDEX_THRESHOLD) {
			bestIndex = (((maxOther * secondMaxOther) / s));
		} else
			bestIndex = (maxOther / s);

//		cout<<"MAX: "<<maxOther<<endl;

		forwardWeight *= bestIndex;
	}


//	cout<<"FORWARD: "<<forwardWeight<<endl;
//	cout<<"FINAL: "<<(backwardWeight*forwardWeight)<<endl;

	return backwardWeight*forwardWeight;



}

void IndexingArgumentsOrderRuleGroundable::update(Atom* atomAdded, unsigned originalPosition) {
	if(atomAdded->isClassicalLiteral() && !atomAdded->isNegative() && originalPosition<backwardWeights.size())
		currentJoinSize=backwardWeights[originalPosition];
}

void IndexingArgumentsOrderRuleGroundable::computeBoundArgumentsSelectivities() {
	unsigned sizeBody=rule->getSizeBody();
	boundArgumentsSelectivities.resize(sizeBody);
	variablesInTerms.resize(sizeBody);
	backwardWeights.resize(sizeBody,1);
	unsigned atom_pos=0;
	for(auto it=rule->getBeginBody();it!=rule->getEndBody();++it,++atom_pos){
		Atom* atom=*it;
		if(!(atom->isClassicalLiteral() && !atom->isNegative())) continue;
		PredicateExtension* predicateExtension=PredicateExtTable::getInstance()->getPredicateExt(atom->getPredicate());
		unsigned termSize=atom->getTermsSize();
		variablesInTerms[atom_pos].resize(termSize);
		for(unsigned i=0;i<termSize;++i){
			Term* term=atom->getTerm(i);
			set_term variables;
			term->getVariable(variables);
			bool boundTerm=true;
			for(auto var:variables){
				bool bound=false;
				for(unsigned j=0;j<sizeBody;++j){
					if(j!=atom_pos && atomsVariables[j].count(var)){
						bound=true;
						break;
					}
				}
				if(!bound){
					if(boundArgumentsSelectivities.empty())
						computeBoundArgumentsSelectivities();
					boundTerm=false;
				}
				else
					variablesInTerms[atom_pos][i].insert(var);
			}
			if(boundTerm){
				double selectivity=predicateExtension->getPredicateInformation()->getSelectivity(i);
				boundArgumentsSelectivities[atom_pos].insert({i,(selectivity)});
			}
		}
	}

//	for(unsigned i=0;i<boundArgumentsSelectivities.size();++i){
//		rule->getAtomInBody(i)->print();
//		cout<<endl;
//		for(auto p:boundArgumentsSelectivities[i])
//			cout<<p.first<<" "<<p.second<<" ";
//		cout<<endl;
//	}
//
//	for(unsigned i=0;i<variablesInTerms.size();++i){
//		rule->getAtomInBody(i)->print();
//		cout<<endl;
//		for(auto p:variablesInTerms[i])
//			for(auto s:p){
//				s->print();cout<<" ";
//			}
//		cout<<endl;
//	}


}

/******************************************************* SemiJoinIndexingArgumentsOrderRuleGroundable************************************************************************/

double SemiJoinIndexingArgumentsOrderRuleGroundable::assignWeightPositiveClassicalLit(Atom* atom, unsigned originalPosition, int s) {
	if(variablesDomains.empty())
		computeVariablesDomains();

	if(boundArgumentsSelectivities.empty())
		computeBoundArgumentsSelectivities();

//	cout<<"-->";atom->print();cout<<endl;

	unsigned sizeTablesToSearch=computePredicateExtensionSize(originalPosition,atom->getPredicate());

	long double prod=1;
	for(unsigned i=0;i<atom->getTermsSize();++i){
		Term* var=atom->getTerm(i);
		if(var->getType()==TermType::VARIABLE){
			if(variablesInTheBody.count(var)){
				prod*=variablesSelectivities[var]/variablesDomains[var];
			}
		}
	}
	double semiJoinSize=sizeTablesToSearch*prod;
//	cout<<"SemiJoin: "<<semiJoinSize<<endl;
	double bestIndex=CombinedCriterion::computeBestIndexingTerms(atom,originalPosition);
//	cout<<"Best: "<<bestIndex<<endl;

	return semiJoinSize*bestIndex;
}

void CombinedCriterion::computeBoundArgumentsSelectivities() {
	unsigned sizeBody=rule->getSizeBody();
	boundArgumentsSelectivities.resize(sizeBody);
	variablesInTerms.resize(sizeBody);
	unsigned atom_pos=0;
	for(auto it=rule->getBeginBody();it!=rule->getEndBody();++it,++atom_pos){
		Atom* atom=*it;
		if(!(atom->isClassicalLiteral() && !atom->isNegative())) continue;
		PredicateExtension* predicateExtension=PredicateExtTable::getInstance()->getPredicateExt(atom->getPredicate());
		unsigned termSize=atom->getTermsSize();
		variablesInTerms[atom_pos].resize(termSize);
		for(unsigned i=0;i<termSize;++i){
			Term* term=atom->getTerm(i);
			set_term variables;
			term->getVariable(variables);
			bool boundTerm=true;
			if(term->contain(TermType::ANONYMOUS)) continue;
			for(auto var:variables){
				bool bound=false;
				for(unsigned j=0;j<sizeBody;++j){
					if(j!=atom_pos && atomsVariables[j].count(var)){
						bound=true;
						break;
					}
				}
				if(!bound){
					boundTerm=false;
					break;
				}
				else
					variablesInTerms[atom_pos][i].insert(var);
			}
			if(boundTerm){
				double selectivity=predicateExtension->getPredicateInformation()->getSelectivity(i);
				boundArgumentsSelectivities[atom_pos].insert({i,(selectivity)});
			}
		}
	}
}

double SemiJoinIndexingArgumentsOrderRuleGroundable2::assignWeightPositiveClassicalLit(Atom* atom, unsigned originalPosition, int s){
	if(variablesDomains.empty())
		computeVariablesDomains();

	if(boundArgumentsSelectivities.empty())
		computeBoundArgumentsSelectivities();

//	cout<<"-->";atom->print();cout<<endl;
	unsigned sizeTablesToSearch=computePredicateExtensionSize(originalPosition,atom->getPredicate());

	long double prod=1;
	for(unsigned i=0;i<atom->getTermsSize();++i){
		Term* var=atom->getTerm(i);
		if(var->getType()==TermType::VARIABLE){
			if(variablesInTheBody.count(var)){
				prod*=variablesSelectivities[var]/variablesDomains[var];
			}
		}
	}
	double semiJoinSize=sizeTablesToSearch*prod;
//	cout<<"SemiJoin: "<<semiJoinSize<<endl;

	return semiJoinSize;
}

double SemiJoinIndexingArgumentsOrderRuleGroundable2::manageEqualWeights(unsigned originalPosition) {
	Atom* atom=rule->getAtomInBody(originalPosition);
	unsigned sizeTablesToSearch=computePredicateExtensionSize(originalPosition,atom->getPredicate());

	double max=0;
	double secondMax=0;
	double bestIndex=1;
	for(unsigned i=0;i<atom->getTermsSize();++i){
		if(Utils::isContained(variablesInTerms[originalPosition][i],variablesInTheBody)){
			if(boundArgumentsSelectivities[originalPosition][i]>max){
				secondMax=max;
				max=boundArgumentsSelectivities[originalPosition][i];
			}
			else if(boundArgumentsSelectivities[originalPosition][i]==max){
				secondMax=max;
			}
		}
	}
	if(max>0 && secondMax>0 && (1-max/sizeTablesToSearch)<DOUBLE_INDEX_THRESHOLD){
		bestIndex=(1-((max*secondMax)/sizeTablesToSearch));
	}
	else if(max>0){
		bestIndex=(1-max/sizeTablesToSearch);
	}
	return bestIndex;
}

bool SemiJoinIndexingArgumentsOrderRuleGroundable2::ckeckSimilarity(double weight1, double weight2) {
	if(weight1>weight2)
		return (weight2/weight1)>SIMILARITY_THRESHOLD;
	return (weight1/weight2)>SIMILARITY_THRESHOLD;
}

double CombinedCriterionIndexingArgumentsOrderRuleGroundable::assignWeightPositiveClassicalLit(Atom* atom, unsigned originalPosition, int s) {
	if(boundArgumentsSelectivities.empty())
		computeBoundArgumentsSelectivities();

	double combinedCriterion=CombinedCriterion::assignWeightPositiveClassicalLit(atom,originalPosition);
	double bestIndex=CombinedCriterion::computeBestIndexingTerms(atom,originalPosition);
	return combinedCriterion*bestIndex;
}

double BindersOrderRuleGroundable::assignWeightPositiveClassicalLit(Atom* atom, unsigned originalPosition, int s) {
	unsigned numBinders=0;
	unsigned arity=atom->getPredicate()->getArity();
	if(atom->getPredicate()->isSolved()){
		for(unsigned i=0;i<atom->getTermsSize();++i){
			set_term variables;
			atom->getTerm(i)->getVariable(variables);
			bool isBinder=false;
			bool boundAll=true;
			for(auto v:variables){
				if(!rule->isAnOutputVariable(v))
					continue;
				if(!variablesInTheBody.count(v)){
					if(!atomsVariables[originalPosition].count(v))
						boundAll=false;
					isBinder=true;
				}
			}
			if(isBinder && boundAll && !Utils::isDisjoint(variables,rule->getOutputVariables())){
//				atom->getTerm(i)->print(cerr);cerr<<" ";
				numBinders++;
			}
		}
//		cerr<<"Predicate: "<<atom->getPredicate()->getName()<<" "<<numBinders<<endl;
	}
	else{
		numBinders=arity;
//		cerr<<"Predicate: "<<atom->getPredicate()->getName()<<" "<<numBinders<<endl;
	}

	return (1-(numBinders/arity));
}

double CombinedCriterionBindersOrderRuleGroundable::assignWeightPositiveClassicalLit(Atom* atom, unsigned originalPosition, int s) {
	double combinedCriterion=CombinedCriterion::assignWeightPositiveClassicalLit(atom,originalPosition);
	return combinedCriterion*computeOutputVariablesBounded(atom,originalPosition);
//	if(boundArgumentsSelectivities.empty())
//		computeBoundArgumentsSelectivities();
//	double bestIndex=CombinedCriterion::computeBestIndexingTerms(atom,originalPosition);
//	return combinedCriterion*bestIndex*computeOutputVariablesBounded(atom,originalPosition);
}

double CombinedCriterionAdvanced::assignWeightPositiveClassicalLit(Atom* atom,unsigned originalPosition, int s) {
	if(boundArgumentsSelectivities.empty())
		computeBoundArgumentsSelectivities();

	double combinedCriterion=CombinedCriterion::assignWeightPositiveClassicalLit(atom,originalPosition);
	double bestIndex=CombinedCriterion::computeBestIndexingTerms(atom,originalPosition);
//	double boundAtoms=CombinedCriterion::computeBoundAtoms(atom,originalPosition);
	double outputVariablesBound=CombinedCriterion::computeOutputVariablesBounded(atom,originalPosition);
	return combinedCriterion*bestIndex*outputVariablesBound;
}

bool CombinedCriterion::setAtomSearcher(Atom* atom, unsigned orginalPosition,unsigned newPosition) {
	Predicate* predicate=atom->getPredicate();
//	PredicateExtension* predicateExtension = predicateExtTable->getPredicateExt(predicate);
//	if(!positiveAtomsIndexingTerms[orginalPosition].empty()){
//		for(auto tablePair:predicate_searchInsert_table[orginalPosition]){
//			unsigned table=tablePair.first;
//			IndexingStructure* atomSearcher;
//			if (componentPredicateInHead!=nullptr && componentPredicateInHead->count(predicate->getIndex()))
//				atomSearcher=predicateExtension->addAtomSearcher(table,DEFAULT_RECURSIVE,nullptr,true);
//			else
//				atomSearcher=predicateExtension->addAtomSearcher(table,DEFAULT,nullptr,false);
//			predicate_searchInsert_atomSearcher[orginalPosition+rule->getSizeHead()].push_back(atomSearcher);
//		}
//	}
//	else if(positiveAtomsIndexingTerms[orginalPosition].size()==predicate->getArity()){
//		for(auto tablePair:predicate_searchInsert_table[orginalPosition]){
//			unsigned table=tablePair.first;
//			IndexingStructure* atomSearcher;
//			auto atomSearcherMAP=predicateExtension->getIndexingStructure(table,MAP);
//			auto atomSearcherHASH=predicateExtension->getIndexingStructure(table,HASHSET);
//			if(atomSearcherMAP!=nullptr)
//				atomSearcher=atomSearcherMAP;
//			else if(atomSearcherHASH!=nullptr)
//				atomSearcher=atomSearcherHASH;
//			else{
//				vector<unsigned> terms(1,0);
//				atomSearcher=predicateExtension->addAtomSearcher(table,MAP,&terms);
//			}
//			predicate_searchInsert_atomSearcher[orginalPosition+rule->getSizeHead()].push_back(atomSearcher);
//		}
//	}
	if(!positiveAtomsIndexingTerms[orginalPosition].empty() && positiveAtomsIndexingTerms[orginalPosition].size()<predicate->getArity()){
		Predicate* predicate=atom->getPredicate();
		PredicateExtension* predicateExtension = predicateExtTable->getPredicateExt(predicate);
		for(auto tablePair:predicate_searchInsert_table[orginalPosition+rule->getSizeHead()]){
			unsigned table=tablePair.first;
			IndexingStructure* atomSearcher;
			vector<unsigned> indexingTerm=positiveAtomsIndexingTerms[orginalPosition];
//			cout<<"---> Predicate: "<<predicate->getName()<<endl;
//			for(auto e: indexingTerm)
//				cout<<e<<" ";
//			cout<<endl;
	//		For FULL INDEXING ON EACH SINGLE ARGUMENT:
	//		atomSearcher=predicateExtension->addFullIndexAtomSearcher(table,(componentPredicateInHead!=nullptr && componentPredicateInHead->count(predicate->getIndex())));
			if (componentPredicateInHead!=nullptr && componentPredicateInHead->count(predicate->getIndex())){
				if(indexingTerm.size()>1)
					atomSearcher=predicateExtension->addAtomSearcher(table, MAP_PAIR_HISTORY_VECTOR, &indexingTerm);
				else
					atomSearcher=predicateExtension->addAtomSearcher(table, MAP_HISTORY_VECTOR, &indexingTerm, true);
			}
			else{
				if(indexingTerm.size()>1)
					atomSearcher=predicateExtension->addAtomSearcher(table, DOUBLEMAP, &indexingTerm);
				else
					atomSearcher=predicateExtension->addAtomSearcher(table, &indexingTerm);
			}
			orderdedPredicateSearchInsertAtomSearcher[newPosition+rule->getSizeHead()].push_back(vector<IndexingStructure*>());
			orderdedPredicateSearchInsertAtomSearcher[newPosition+rule->getSizeHead()][0].push_back(atomSearcher);
		//	indexingArguments[position-currentRule->getSizeHead()][atomPos]=bestArg;
		}
		return true;
	}
	return false;
}

/******************************************************* CombinedCriterionSelections ************************************************************************/

double CombinedCriterionComparisonsEstimation::basicFormulaLinearInterpolation(double numValues, double constant, double max, double min){
//	return 1+((numValues-1)*(constant-min)/(max-min));
	return (numValues-1)*(constant-min)/(max-min);
}

double CombinedCriterionComparisonsEstimation::computeSelectivityLessComparison(Atom* atom, unsigned termPos, int constant, Atom* builtIn){
	PredicateInformation* pF=predicateExtTable->getPredicateExt(atom->getPredicate())->getPredicateInformation();
	if(pF->getMax(termPos)<constant)
		return pF->getSelectivity(termPos);
	if(pF->getMin(termPos)>=constant)
		return 0;
	return basicFormulaLinearInterpolation(pF->getSelectivity(termPos),constant,pF->getMax(termPos),pF->getMin(termPos));
}

double CombinedCriterionComparisonsEstimation::computeSelectivityGreaterComparison(Atom* atom, unsigned termPos, int constant, Atom* builtIn){
	PredicateInformation* pF=predicateExtTable->getPredicateExt(atom->getPredicate())->getPredicateInformation();
	if(pF->getMax(termPos)<=constant)
		return 0;
	if(pF->getMin(termPos)>constant)
		return pF->getSelectivity(termPos);
	double lessSelectivity=basicFormulaLinearInterpolation(pF->getSelectivity(termPos),constant,pF->getMax(termPos),pF->getMin(termPos));
	return pF->getSelectivity(termPos)-lessSelectivity;
}

double CombinedCriterionComparisonsEstimation::computeSelectivityEqualConstantComparison(Atom* atom, unsigned termPos, int constant){
	PredicateInformation* pF=predicateExtTable->getPredicateExt(atom->getPredicate())->getPredicateInformation();
	return 1/pF->getSelectivity(termPos);
}

double CombinedCriterionComparisonsEstimation::computeSelectivityUnequalConstantComparison(Atom* atom, unsigned termPos, int constant){
	PredicateInformation* pF=predicateExtTable->getPredicateExt(atom->getPredicate())->getPredicateInformation();
	return pF->getSelectivity(termPos)-computeSelectivityEqualConstantComparison(atom,termPos,constant);
}

double CombinedCriterionComparisonsEstimation::computeSelectivityEqualVariableComparison(Atom* atom1, Atom* atom2, unsigned termPos1, unsigned termPos2){
	PredicateInformation* pF1=predicateExtTable->getPredicateExt(atom1->getPredicate())->getPredicateInformation();
	PredicateInformation* pF2=predicateExtTable->getPredicateExt(atom2->getPredicate())->getPredicateInformation();
	return 1/max(pF1->getSelectivity(termPos1),pF2->getSelectivity(termPos2));
}

void CombinedCriterionComparisonsEstimation::findTheBindingPredicate(Term* variable, pair<Predicate*,unsigned>& pair){
	for(unsigned atom_pos:atomInserted){
		if(atomsVariables[atom_pos].count(variable)){
			Atom* atom=rule->getAtomInBody(atom_pos);
			for(unsigned i=0;i<atom->getTermsSize();++i){
				Term* t=atom->getTerm(i);
				if(t->getType()==VARIABLE && t->getIndex()==variable->getIndex()){
					pair.first=atom->getPredicate();
					pair.second=i;
				}
			}
		}
	}
	pair.first=0;
	pair.second=0;
}

double CombinedCriterionComparisonsEstimation::estimateIntersection(Predicate* p1, unsigned pos1, Predicate* p2, unsigned pos2, bool max){
//	PredicateInformation* pF1=predicateExtTable->getPredicateExt(p1)->getPredicateInformation();
	PredicateInformation* pF2=predicateExtTable->getPredicateExt(p2)->getPredicateInformation();
//	return basicFormulaLinearInterpolation(pF1->getSelectivity(pos1),pF2->getMax(pos2),pF1->getMax(pos1),pF1->getMin(pos1));
	if(max)
		return pF2->getMax(pos2);
	return pF2->getMin(pos2);
}

double CombinedCriterionComparisonsEstimation::evaluateComparisonSelectivity(Atom* atom, unsigned originalPosition, unsigned i, Term* var, bool begin){
		double selectivityComparison=-1;
		auto pairIt=atomsBuiltInsComparisonsDependencies.equal_range(originalPosition);
		for(auto it=pairIt.first;it!=pairIt.second;++it){
			if(!atomsVariables[it->second].count(var)) continue;
			BuiltInAtom* builtIn=(dynamic_cast< BuiltInAtom* >(rule->getAtomInBody(it->second)));
//			builtIn->print(cerr);cerr<<endl;
			int value=0;
			pair<Predicate*, unsigned> bindingPredicate={0,0};
			bool evaluableFirstTerm=false;
			bool evaluableSecondTerm=false;
			if(builtIn->getTerm(0)->getType()==VARIABLE && builtIn->getTerm(1)->getType()==VARIABLE){
				if(begin){
					unsigned atom_pos=0;
					for(auto it=rule->getBeginBody();it!=rule->getEndBody()&&atom_pos<originalPosition;++it,++atom_pos){
						if(atomsVariables[atom_pos].count(builtIn->getTerm(0))){
							Atom* atom=rule->getAtomInBody(atom_pos);
							for(unsigned i=0;i<atom->getTermsSize();++i){
								Term* t=atom->getTerm(i);
								if(t->getType()==VARIABLE && t->getIndex()==builtIn->getTerm(0)->getIndex()){
									bindingPredicate.first=atom->getPredicate();
									bindingPredicate.second=i;
									evaluableFirstTerm=true;
									break;
								}
							}
						}
						else if(atomsVariables[atom_pos].count(builtIn->getTerm(1))){
							Atom* atom=rule->getAtomInBody(atom_pos);
							for(unsigned i=0;i<atom->getTermsSize();++i){
								Term* t=atom->getTerm(i);
								if(t->getType()==VARIABLE && t->getIndex()==builtIn->getTerm(1)->getIndex()){
									bindingPredicate.first=atom->getPredicate();
									bindingPredicate.second=i;
									evaluableSecondTerm=true;
									break;
								}
							}
						}
					}
				}
				else{
					if(variablesInTheBody.count(builtIn->getTerm(0))){
						findTheBindingPredicate(builtIn->getTerm(0),bindingPredicate);
						evaluableFirstTerm=true;
					}
					else if(variablesInTheBody.count(builtIn->getTerm(1))){
						findTheBindingPredicate(builtIn->getTerm(1),bindingPredicate);
						evaluableSecondTerm=true;
					}
				}
			}
			else if(builtIn->getTerm(0)->getType()==VARIABLE && builtIn->getTerm(1)->getType()==NUMERIC_CONSTANT){
				value=builtIn->getTerm(1)->getConstantValue();
			}
			else if(builtIn->getTerm(1)->getType()==VARIABLE && builtIn->getTerm(0)->getType()==NUMERIC_CONSTANT){
				value=builtIn->getTerm(0)->getConstantValue();
			}
			if(builtIn->isVariableLessConstant() || builtIn->isConstantGreaterVariable()
					|| (builtIn->isVariableLessVariable() && evaluableSecondTerm)  || (builtIn->isVariableGreaterVariable() && evaluableFirstTerm) ){
				if(bindingPredicate.first!=0 && (evaluableFirstTerm || evaluableSecondTerm))
					value=estimateIntersection(atom->getPredicate(),i,bindingPredicate.first,bindingPredicate.second,true);
				selectivityComparison=computeSelectivityLessComparison(atom,i,value,builtIn);
			}
			else if(builtIn->isVariableLessOrEqConstant() || builtIn->isConstantGreaterOrEqVariable()
					|| (builtIn->isVariableLessOrEqVariable() && evaluableSecondTerm)  || (builtIn->isVariableGreaterOrEqVariable() && evaluableFirstTerm) ){
				if(bindingPredicate.first!=0 && (evaluableFirstTerm || evaluableSecondTerm))
					value=estimateIntersection(atom->getPredicate(),i,bindingPredicate.first,bindingPredicate.second,true);
				selectivityComparison=computeSelectivityLessComparison(atom,i,value,builtIn);
			}
			else if(builtIn->isVariableGreaterConstant() || builtIn->isConstantLessVariable()
					|| (builtIn->isVariableGreaterVariable() && evaluableSecondTerm)  || (builtIn->isVariableLessVariable() && evaluableFirstTerm) ){
				if(bindingPredicate.first!=0 && (evaluableFirstTerm || evaluableSecondTerm))
					value=estimateIntersection(bindingPredicate.first,bindingPredicate.second,atom->getPredicate(),i,false);
				selectivityComparison=computeSelectivityGreaterComparison(atom,i,value,builtIn);
			}
			else if(builtIn->isVariableGreaterOrEqConstant() || builtIn->isConstantLessOrEqVariable()
					|| (builtIn->isVariableGreaterOrEqVariable() && evaluableSecondTerm)  || (builtIn->isVariableLessOrEqVariable() && evaluableFirstTerm) ){
				if(bindingPredicate.first!=0 && (evaluableFirstTerm || evaluableSecondTerm))
					value=estimateIntersection(bindingPredicate.first,bindingPredicate.second,atom->getPredicate(),i,false);
				selectivityComparison=computeSelectivityGreaterComparison(atom,i,value,builtIn);
			}
			else if(builtIn->isVariableEqualConstantNotAssignment() || builtIn->isConstantEqualVariableNotAssignment()
					|| (builtIn->isVariableEqualVariableNotAssignment() && (evaluableSecondTerm || evaluableFirstTerm)))
				selectivityComparison=computeSelectivityEqualConstantComparison(atom,i,value);
			else if(builtIn->isVariableUnequalConstant() || builtIn->isConstantUnequalVariable()
					|| (builtIn->isVariableUnequalVariable() && (evaluableSecondTerm || evaluableFirstTerm)))
				selectivityComparison=computeSelectivityUnequalConstantComparison(atom,i,value);
//					else if(builtIn->isVariableEqualVariableNotAssignment()){
//						selectivityComparison=computeSelectivityEqualVariableComparison(atom,i);
//						break;
//					}
		}
//		cerr<<"Sel Comp: "<<selectivityComparison<<endl;
		return selectivityComparison;
}


double CombinedCriterionComparisonsEstimation::assignWeightPositiveClassicalLit(Atom* atom, unsigned originalPosition, int size){
	if(variablesDomains.empty()){
		computeBuiltInsComparisonDependencies();
		computeVariablesDomains();
	}

	if(size==-1)
		size=computePredicateExtensionSize(originalPosition,atom->getPredicate());

	long double prodSelectivity_a=1;
	long unsigned prodDomains_a=1;
	long double prodSelectivity_b=1;
	long unsigned prodDomains_b=1;
	set_term variablesFound;
	double selectivity_product=1;
	for(unsigned i=0;i<atom->getTermsSize();++i){
		Term* var=atom->getTerm(i);
		if(var->getType()==TermType::VARIABLE){
			if(!variablesFound.insert(var).second) continue;
			double selectivityComparison=evaluateComparisonSelectivity(atom,originalPosition,i,var);
			if(selectivityComparison>=0)
				selectivity_product*=(selectivityComparison/predicateExtTable->getPredicateExt(atom->getPredicate())->getPredicateInformation()->getSelectivity(i));
			if(variablesInTheBody.count(var)){
				prodSelectivity_a*=variablesSelectivities[var]/variablesDomains[var];
				prodDomains_a*=variablesDomains[var];
				if(selectivityComparison>=0){
					variablesComparisonsSelectivities[var]=selectivityComparison;
					prodSelectivity_b*=selectivityComparison;
				}
				else
					prodSelectivity_b*=predicateExtTable->getPredicateExt(atom->getPredicate())->getPredicateInformation()->getSelectivity(i);
				prodDomains_b*=variablesDomains[var]*variablesDomains[var];
			}
			else{
				for(unsigned j=0;j<rule->getSizeBody();++j){
					if(j!=originalPosition && atomsVariables[j].count(var)){
						prodDomains_a*=variablesDomains[var];
						break;
					}
				}

			}
		}
	}

	double sel_a=size*prodSelectivity_a/prodDomains_a;
	double sel_b=prodSelectivity_b/prodDomains_b;

//	atom->print(cerr);cerr<<endl;
//	cerr<<sel_a*sel_b*selectivity_product<<endl;
	return sel_a*sel_b*selectivity_product;
}

void CombinedCriterionComparisonsEstimation::computeBuiltInsComparisonDependencies() {
	unsigned k=0,j=0;
	for(auto it=rule->getBeginBody();it!=rule->getEndBody();++it,++k){
		Atom* atom=*it;
		if(atom->isClassicalLiteral() && !atom->isNegative()){
			for(unsigned i=0;i<atom->getTermsSize();++i){
				Term* term=atom->getTerm(i);
				if(term->getType()==TermType::VARIABLE){
					j=0;
					for(auto it2=rule->getBeginBody();it2!=rule->getEndBody();++it2,++j){
						Atom* atom2=*it2;
						if(atom2->isBuiltIn()){
							if(atom2->isComparisonBuiltIn() && (atom2->getTerm(0)->getIndex()==term->getIndex() || atom2->getTerm(1)->getIndex()==term->getIndex())){
								atomsBuiltInsComparisonsDependencies.insert({k,j});
							}
						}
					}
				}
			}
		}
	}

//	for(auto e:atomsBuiltInsComparisonsDependencies){
//		rule->getAtomInBody(e.first)->print(cerr);
//		cerr<<" ";
//		rule->getAtomInBody(e.second)->print(cerr);
//		cerr<<endl;
//	}

}

void CombinedCriterionComparisonsEstimation::updateVariableSelectivity(Atom* atomAdded, unsigned atomPos) {
	if(atomAdded->isClassicalLiteral() && !atomAdded->isNegative()){
		PredicateInformation* predInfo=predicateExtTable->getPredicateExt(atomAdded->getPredicate())->getPredicateInformation();
		for(unsigned i=0;i<atomAdded->getTermsSize();++i){
			Term* term=atomAdded->getTerm(i);
			if(term->getType()==TermType::VARIABLE){
				unsigned selectivity=0;
				if(variablesComparisonsSelectivities.count(term))
					selectivity=variablesComparisonsSelectivities[term];
				else
					selectivity=predInfo->getSelectivity(i);
				if(variablesInTheBody.count(term) && variablesDomains[term]>0)
					variablesSelectivities[term]*=selectivity/variablesDomains[term];
				else
					variablesSelectivities[term]=selectivity;
			}
//			else if(term==TermType::FUNCTION){ //TODO
//
//			}
		}
		variablesComparisonsSelectivities.clear();
	}
}

}}

