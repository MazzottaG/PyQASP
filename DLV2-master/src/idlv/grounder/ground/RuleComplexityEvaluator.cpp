/*******************************************************************************
 *   Copyright 2016 Francesco Calimeri, Davide Fusca', Simona Perri and Jessica Zangari
 *  
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *  
 *        http://www.apache.org/licenses/LICENSE-2.0
 *  
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *******************************************************************************/
/*
 * RuleComplexityEvaluator.cpp
 *
 *  Created on: Mar 28, 2017
 *      Author: jessica
 */

#include "RuleComplexityEvaluator.h"
#include "IntelligentDecomposer.h"

namespace DLV2 {
namespace grounder {

RuleComplexityEvaluator* RuleComplexityEvaluatorFactory::evaluator=0;

bool RuleComplexityEvaluator::isAuxiliaryPred(Predicate* p){
	return p!=nullptr &&
			(p->getName().find(DECOMPOSITION_PREDICATES_PREFIX) == 0
			|| p->getName().find(AUXILIARY_PROJ_DECOMP) == 0);
}

double RuleComplexityEvaluator::determineSelectivity(unsigned i,Atom* atom) {
	double selectivity = 1;
		if (isAuxiliaryPred(atom->getPredicate()))
			selectivity = IntelligentDecomposer::getInstance()->getTempPredicatesArgSel(atom->getPredicate(), i);
		else
			selectivity = PredicateExtTable::getInstance()->getPredicateExt(
					atom->getPredicate())->getPredicateInformation()->getSelectivity(i);
	if(selectivity==0) return 1;
	return selectivity;
}

double RuleComplexityEvaluator::determineSize(Atom* atom) {
	double size = 1;
	if (isAuxiliaryPred(atom->getPredicate()))
		size = IntelligentDecomposer::getInstance()->getTempPredicatesSize(
				atom->getPredicate());
	else
		size = (double) (PredicateExtTable::getInstance()->getPredicateExt(
				atom->getPredicate())->getPredicateExtentionSize());
	if(size==0) return 1;
	return size;
}

void RuleComplexityEvaluator::setTempPredicatesStats(double value,
		Rule* rule) {
	for (auto it = rule->getBeginHead(); it != rule->getEndHead(); ++it) {
		Atom* atom = *it;
		if (atom->isClassicalLiteral() && !atom->isNegative()
				&& isAuxiliaryPred(atom->getPredicate())) {
			if(atom->getPredicate()->getArity()==1){
				unsigned pos=0;
				unsigned count=0;
				Predicate* p=0;
				for(auto it=rule->getBeginBody();it!=rule->getEndBody();++it){
					Atom* a=*it;
					if(a->isClassicalLiteral() && !a->isNegative()){
						for(unsigned j=0;j<a->getTermsSize();++j){
							if(a->getTerm(j)->getIndex()==atom->getTerm(0)->getIndex()){
								count++;
								pos=j;
								p=a->getPredicate();
							}
						}
					}
					if(count>=2)
						break;
				}
				if(count==1 && p!=0){
					double s=PredicateExtTable::getInstance()->getPredicateExt(p)->getPredicateInformation()->getSelectivity(pos);
					IntelligentDecomposer::getInstance()->setTempPredicatesSize(
							atom->getPredicate(), s);
					IntelligentDecomposer::getInstance()->setTempPredicatesArgSel(
						atom->getPredicate(), 0, s);
					return;
				}
			}
			IntelligentDecomposer::getInstance()->setTempPredicatesSize(
					atom->getPredicate(), value);
			double uniformSel = pow(value,
					1.0 / atom->getPredicate()->getArity());
			for (unsigned i = 0; i < atom->getTermsSize(); ++i)
				IntelligentDecomposer::getInstance()->setTempPredicatesArgSel(
						atom->getPredicate(), i, uniformSel);
		}
	}
}

void RuleComplexityEvaluatorCombined::combinedEstimationsOfJoinSizeAndJoinCost(Rule* rule, double& joinsize, double& cost) {

	vector<Atom*> classicalLits;
	for (auto it = rule->getBeginBody(); it != rule->getEndBody(); ++it) {
		if((*it)->isClassicalLiteral() && !(*it)->isNegative())
			classicalLits.push_back((*it));
	}

	map_term<unsigned> variablesDomains;
	map_term<double> variablesSelectivities;
	set_term boundVars;
	for (auto atom:classicalLits) {
		for (unsigned i = 0; i < atom->getTermsSize(); ++i) {
			Term* term = atom->getTerm(i);
			unsigned selectivity = 0;
			selectivity = determineSelectivity(i, atom);
			auto it = variablesDomains.find(term);
			if (it != variablesDomains.end()) {
				if (selectivity > (*it).second)
					variablesDomains[term] = selectivity;
			} else
				variablesDomains[term] = selectivity;
		}
	}
	for (auto atom:classicalLits) {
		double prod = 1;
		double sel = 1;
		set_term newBound;
		set_term terms = atom->getVariable();
		list<unsigned> sels;
		bool allBound=true;
		for (unsigned i = 0; i < atom->getTermsSize(); ++i) {
			Term* term = atom->getTerm(i);
			if (term->getType() == TermType::VARIABLE
					&& boundVars.count(term)) {
				double selectivity = 1;
				selectivity = determineSelectivity(i, atom);
				sels.push_back(selectivity);
				prod *= variablesSelectivities[term]
						/ variablesDomains[term];
			}
			else
				allBound=false;
		}
		sels.sort();
		if (sels.size() == 1)
			sel *= sels.front();
		else if (sels.size() >= 1) {
			auto it = sels.begin();
			sel *= (*it);
			++it;
			sel *= (*it);
		}
		double size = determineSize(atom);
		double n = (size / sel);
		joinsize  *= (size * prod);
		if(!allBound)
			cost *= (n * prod);
		for (unsigned i = 0; i < atom->getTermsSize(); ++i) {
			Term* term = atom->getTerm(i);
			if (term->getType() == TermType::VARIABLE) {
				unsigned selectivity = 1;
				selectivity = determineSelectivity(i, atom);
				if(boundVars.count(term) && variablesDomains[term]>0){
					double x = (double(selectivity) / variablesDomains[term]);
					variablesSelectivities[term] *= x;
				}
				else{
					variablesSelectivities[term] = double(selectivity);
				}
				newBound.insert(term);
			}
		}
		boundVars.insert(newBound.begin(),newBound.end());
	}
}

double RuleComplexityEvaluatorCombined::evaluate(Rule* rule){
	double joinsize = 1;
	double cost = 1;
	combinedEstimationsOfJoinSizeAndJoinCost(rule,joinsize,cost);
	setTempPredicatesStats(joinsize, rule);
//	if (rule->getSizeHead()==1 && rule->getAtomInHead(0)->isClassicalLiteral() && !rule->getAtomInHead(0)->isNegative()
//			&& rule->getAtomInHead(0)->getPredicate()->getName().find(LpoptInputBuilder::DECOMPOSED_PRED_NAME) == 0) {
//		return IntelligentDecomposer::getInstance()->getTempPredicatesSize(rule->getAtomInHead(0)->getPredicate());
//	}
	return cost;
}

double RuleComplexityEvaluatorSelS::evaluate(Rule* rule){

	map_term<unsigned> variablesDomains;
	map_term<double> variablesSelectivities;
	set_term boundVars;
	double value=1;

	for(auto it=rule->getBeginBody();it!=rule->getEndBody();++it){
		Atom* atom=*it;
		if(atom->isClassicalLiteral() && !atom->isNegative()){
			for(unsigned i=0;i<atom->getTermsSize();++i){
				Term* term=atom->getTerm(i);
				unsigned selectivity = determineSelectivity(i, atom);
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

	for(auto it=rule->getBeginBody();it!=rule->getEndBody();++it){
		Atom* atom=(*it);
		if(atom->isClassicalLiteral() && !atom->isNegative()){
			double prod=1;
			double boundDom=1;
			set_term terms=atom->getVariable();
			for(auto t:terms)
				if(boundVars.count(t)){
					prod*=variablesSelectivities[t]/variablesDomains[t];
					boundDom*=variablesDomains[t];
				}
			double size = determineSize(atom);
			double n=prod*size;
			value=(value*n)/boundDom;
			for(unsigned i=0;i<atom->getTermsSize();++i){
				Term* term=atom->getTerm(i);
				if(term->getType()==TermType::VARIABLE){
					double selectivity = determineSelectivity(i, atom);
					if(boundVars.count(term) && variablesDomains[term]>0)
						variablesSelectivities[term]*=selectivity/variablesDomains[term];
					else
						variablesSelectivities[term]=selectivity;
				}
			}
			boundVars.insert(terms.begin(),terms.end());
		}
	}
	setTempPredicatesStats(value, rule);
	return value;
}

double RuleComplexityEvaluatorJoinCost::evaluate(Rule* rule){
	double value=1;
	double join=1;
	double cost=1;

	map_term<double> variablesSelectivities;
	set_term boundVars;

	for(auto it=rule->getBeginBody();it!=rule->getEndBody();++it){
		Atom* atom=(*it);
		if(atom->isClassicalLiteral() && !atom->isNegative()){
			double prod=1;
			map_term<double> selectivities;
			for(unsigned i=0;i<atom->getTermsSize();++i){
				Term* t=atom->getTerm(i);
				if(t->getType()==VARIABLE && boundVars.count(t)){
					double selectivity = determineSelectivity(i, atom);
					double sel=max(variablesSelectivities[t],selectivity);
					prod*=sel;
					selectivities.insert({t,sel});
				}
			}
			double size=determineSize(atom);
			value=(value*size)/prod;
			for(auto el:selectivities){
				variablesSelectivities[el.first]=el.second;
				boundVars.insert(el.first);
			}
		}
	}

	set_term variablesInRule;
	rule->getVariables(variablesInRule);
	for(auto v:variablesInRule){
		int nLitsInWhichAppears=0;
		double prod=1;
		for(auto it=rule->getBeginBody();it!=rule->getEndBody();++it){
			Atom* atom=(*it);
			if(atom->isClassicalLiteral() && !atom->isNegative()){
				bool found=false;
				for(unsigned i=0;i<atom->getTermsSize();++i){
					Term* t=atom->getTerm(i);
					if(t->getType()==VARIABLE && t->getIndex()==v->getIndex()){
						found=true;
						double selectivity = determineSelectivity(i, atom);
						prod*=selectivity;
					}
				}
				if(found)
					nLitsInWhichAppears++;
			}
			if(nLitsInWhichAppears>=2)
				cost+=prod;
		}
	}

	value=join*cost;
	setTempPredicatesStats(join, rule);
	return value;
}

double RuleComplexityEvaluatorJoinCostCombined::evaluate(Rule* rule){
	double value=1;
	double join=RuleComplexityEvaluatorCombined::evaluate(rule);
	double comparisons=1;

	set_term variablesInRule;
	rule->getVariables(variablesInRule);
	for(auto v:variablesInRule){
		int nLitsInWhichAppears=0;
		double prod=1;
		for(auto it=rule->getBeginBody();it!=rule->getEndBody();++it){
			Atom* atom=(*it);
			if(atom->isClassicalLiteral() && !atom->isNegative()){
				bool found=false;
				for(unsigned i=0;i<atom->getTermsSize();++i){
					Term* t=atom->getTerm(i);
					if(t->getType()==VARIABLE && t->getIndex()==v->getIndex()){
						found=true;
						double selectivity = determineSelectivity(i, atom);
						prod*=selectivity;
					}
				}
				if(found)
					nLitsInWhichAppears++;
			}
			if(nLitsInWhichAppears>=2)
				comparisons+=prod;
		}
	}

	value=join*comparisons;
//	cerr<<" "<<join<<" "<<comparisons<<endl;
//	setTempPredicatesStats(value, rule);
	return value;
}

double RuleComplexityEvaluatorJoinCostParallel::evaluateJoinCost(vector<Atom*>::const_iterator begin,vector<Atom*>::const_iterator end) {
	double value = 1;
	map_term<double> variablesSelectivities;
	set_term boundVars;
	for (auto it = begin; it != end; ++it) {
		Atom* atom = (*it);
		if (atom->isClassicalLiteral() && !atom->isNegative()) {
			double prod = 1;
			map_term<double> selectivities;
			for (unsigned i = 0; i < atom->getTermsSize(); ++i) {
				Term* t = atom->getTerm(i);
				if (t->getType() == VARIABLE && boundVars.count(t)) {
					double selectivity = determineSelectivity(i, atom);
					double sel = max(variablesSelectivities[t], selectivity);
					prod *= sel;
					selectivities.insert( { t, sel });
				}
			}
			double size = determineSize(atom);
			value = (value * size) / prod;
			for (auto el : selectivities) {
				variablesSelectivities[el.first] = el.second;
				boundVars.insert(el.first);
			}
		}
	}
	return value;
}

double RuleComplexityEvaluatorJoinCostParallel::evaluate(Rule* rule){
	double cost=0;
	if(rule->getSizeBody()>0){
		PredicateExtTable* predExtTable=PredicateExtTable::getInstance();
		vector<Atom*> classicalLits;
		for (auto it = rule->getBeginBody(); it != rule->getEndBody(); ++it) {
			if((*it)->isClassicalLiteral() && !(*it)->isNegative())
				classicalLits.push_back((*it));
		}
		if(classicalLits.size()==1)
			cost=predExtTable->getPredicateExt(classicalLits[0]->getPredicate())->getPredicateExtentionSize();
		else if(classicalLits.size()==2)
			cost=predExtTable->getPredicateExt(classicalLits[0]->getPredicate())->getPredicateExtentionSize()*predExtTable->getPredicateExt(classicalLits[1]->getPredicate())->getPredicateExtentionSize();
		else{
			cost=predExtTable->getPredicateExt(classicalLits[0]->getPredicate())->getPredicateExtentionSize();
			vector<Atom*> otherClassical;
			otherClassical.push_back(classicalLits[0]);
			for(unsigned i=1;i<classicalLits.size();++i){
				cost+=evaluateJoinCost(otherClassical.begin(),otherClassical.end())*predExtTable->getPredicateExt(classicalLits[i]->getPredicate())->getPredicateExtentionSize();;
				otherClassical.push_back(classicalLits[i]);
			}
		}
	}

	setTempPredicatesStats(cost, rule);
	return cost;
}

void RuleComplexityEvaluatorCombinedVariant::combinedEstimationsOfJoinSizeAndJoinCost(
		Rule* rule, double& joinsize, double& cost) {

	vector<Atom*> classicalLits;
	for (auto it = rule->getBeginBody(); it != rule->getEndBody(); ++it) {
		if((*it)->isClassicalLiteral() && !(*it)->isNegative())
			classicalLits.push_back((*it));
	}

	map_term<unsigned> variablesDomains;
	map_term<double> variablesSelectivities;
	set_term boundVars;
	for (auto atom:classicalLits) {
		for (unsigned i = 0; i < atom->getTermsSize(); ++i) {
			Term* term = atom->getTerm(i);
			unsigned selectivity = 0;
			selectivity = determineSelectivity(i, atom);
			auto it = variablesDomains.find(term);
			if (it != variablesDomains.end()) {
				if (selectivity > (*it).second)
					variablesDomains[term] = selectivity;
			} else
				variablesDomains[term] = selectivity;
		}
	}
	for (auto atom:classicalLits) {
		double prod = 1;
		double sel = 1;
		set_term newBound;
		set_term terms = atom->getVariable();
		list<unsigned> sels;
		bool allBound=true;
		for (unsigned i = 0; i < atom->getTermsSize(); ++i) {
			Term* term = atom->getTerm(i);
			if (term->getType() == TermType::VARIABLE
					&& boundVars.count(term)) {
				double selectivity = 1;
				selectivity = determineSelectivity(i, atom);
				sels.push_back(selectivity);
				prod *= variablesSelectivities[term]
						/ variablesDomains[term];
			}
			else
				allBound=false;
		}
		sels.sort();
		if (sels.size() == 1)
			sel *= sels.front();
		else if (sels.size() >= 1) {
			auto it = sels.begin();
			sel *= (*it);
			++it;
			sel *= (*it);
		}
		double size = determineSize(atom);
		double n = (size / sel);
		joinsize  *= (size * prod);
		if(!allBound)
			cost *= n;
		for (unsigned i = 0; i < atom->getTermsSize(); ++i) {
			Term* term = atom->getTerm(i);
			if (term->getType() == TermType::VARIABLE) {
				unsigned selectivity = 1;
				selectivity = determineSelectivity(i, atom);
				if(boundVars.count(term) && variablesDomains[term]>0){
					double x = (double(selectivity) / variablesDomains[term]);
					variablesSelectivities[term] *= x;
				}
				else{
					variablesSelectivities[term] = double(selectivity);
				}
				newBound.insert(term);
			}
		}
		boundVars.insert(newBound.begin(),newBound.end());
	}
}

double RuleComplexityEvaluatorCombinedVariant::evaluate(Rule* rule){
	double joinsize = 1;
	double cost = 1;
	combinedEstimationsOfJoinSizeAndJoinCost(rule,joinsize,cost);
	setTempPredicatesStats(joinsize, rule);
	return cost;
}

RuleComplexityEvaluator* RuleComplexityEvaluatorFactory::getDefaultEvaluator() {
	if(evaluator==0){
		switch(Options::globalOptions()->getDecompositionFormula()){
			case DECOMPOSITION_FORMULA_COMBINED:
				evaluator=new RuleComplexityEvaluatorCombined;
				break;
			case DECOMPOSITION_FORMULA_COMBINED2:
				evaluator=new RuleComplexityEvaluatorSelS;
				break;
			case DECOMPOSITION_FORMULA_JOIN_TIME:
				evaluator=new RuleComplexityEvaluatorJoinCost;
				break;
			case DECOMPOSITION_FORMULA_JOIN_TIME_COMBINED:
				evaluator=new RuleComplexityEvaluatorJoinCostCombined;
				break;
			case DECOMPOSITION_FORMULA_COMBINED_VARIANT:
				evaluator=new RuleComplexityEvaluatorCombinedVariant;
				break;
			case DECOMPOSITION_FORMULA_JOIN_TIME_PARALLEL:
				evaluator=new RuleComplexityEvaluatorJoinCostParallel;
				break;
			case DECOMPOSITION_FORMULA_COMBINED_ORIGINAL:
				evaluator=new RuleComplexityEvaluatorCombinedJoinOnly;
				break;
			case DECOMPOSITION_FORMULA_JOIN_ULLMAN:
				evaluator=new RuleComplexityEvaluatorSizeJoinUllmann;
				break;
			default:
				evaluator=new RuleComplexityEvaluatorCombined;
				break;
		}
	}
	return evaluator;
}

} /* namespace grounder */
} /* namespace DLV2 */
