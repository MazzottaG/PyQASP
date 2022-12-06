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
 * RuleComplexityEvaluator.h
 *
 *  Created on: Mar 28, 2017
 *      Author: jessica
 */

#ifndef SRC_GROUNDER_GROUND_RULECOMPLEXITYEVALUATOR_H_
#define SRC_GROUNDER_GROUND_RULECOMPLEXITYEVALUATOR_H_

#include "../statement/Rule.h"
#include "../table/PredicateExtension.h"
#include <algorithm>

namespace DLV2 {
namespace grounder {

class RuleComplexityEvaluator {
public:
	virtual double evaluate(Rule* rule)=0;
	virtual ~RuleComplexityEvaluator() {}
protected:
	//Utility Methods
	double determineSelectivity(unsigned i, Atom* atom);
	double determineSize(Atom* atom);
	void setTempPredicatesStats(double value, Rule* rule);
	bool isAuxiliaryPred(Predicate* p);
};

class RuleComplexityEvaluatorCombined : public RuleComplexityEvaluator {
public:
	virtual double evaluate(Rule* rule);

protected:
	void combinedEstimationsOfJoinSizeAndJoinCost(Rule* rule,double& s,double& c);
};

class RuleComplexityEvaluatorSizeJoinUllmann : public RuleComplexityEvaluator {
public:
	virtual double evaluate(Rule* rule){
		rule->computeJoinVariables();
		double estimation=1,sels=1;
		unordered_map<Term*,list<double>> valuesJoinVariables;
		for (auto it = rule->getBeginBody(); it != rule->getEndBody(); ++it) {
			if((*it)->getPredicate()!=nullptr){
				estimation*=determineSize(*it);
				for(unsigned i=0;i<(*it)->getTermsSize();++i){
					if((*it)->getTerm(i)->getType()==VARIABLE && rule->getRuleInformation().isAJoinVariable((*it)->getTerm(i)))
						valuesJoinVariables[(*it)->getTerm(i)].push_back(determineSelectivity(i,*it));
				}
			}
		}
		for(auto e:valuesJoinVariables)
		{
			e.second.sort();
			e.second.pop_front();
			for(auto d:e.second)
				sels*=d;
		}
		double joinsize=estimation/sels;
		setTempPredicatesStats(joinsize, rule);
		return joinsize;
	}
};

class RuleComplexityEvaluatorCombinedJoinOnly : public RuleComplexityEvaluatorCombined {
public:
	virtual double evaluate(Rule* rule){
		double joinsize = 1;
		double cost = 1;
		combinedEstimationsOfJoinSizeAndJoinCost(rule,joinsize,cost);
		setTempPredicatesStats(joinsize, rule);
		return joinsize;
	}
};

class RuleComplexityEvaluatorSelS : public RuleComplexityEvaluator {
public:
	virtual double evaluate(Rule* rule);
};

class RuleComplexityEvaluatorJoinCost: public RuleComplexityEvaluator {
public:
	virtual double evaluate(Rule* rule);
};

class RuleComplexityEvaluatorJoinCostCombined: public RuleComplexityEvaluatorCombined {
public:
	virtual double evaluate(Rule* rule);
};

class RuleComplexityEvaluatorJoinCostParallel: public RuleComplexityEvaluatorCombined {
public:
	virtual double evaluate(Rule* rule);

private:
	double evaluateJoinCost(vector<Atom*>::const_iterator begin,vector<Atom*>::const_iterator end);
};

class RuleComplexityEvaluatorJoinParallelCombined: public RuleComplexityEvaluatorCombined {
public:
	virtual double evaluate(Rule* rule);
};

class RuleComplexityEvaluatorCombinedVariant : public RuleComplexityEvaluator {
public:
	virtual double evaluate(Rule* rule);

private:
	void combinedEstimationsOfJoinSizeAndJoinCost(Rule* rule,double& s,double& c);
};

class RuleComplexityEvaluatorFactory {
public:
	static RuleComplexityEvaluator* getDefaultEvaluator();
	static void freeInstance() {
		delete evaluator;
	}
protected:
	static RuleComplexityEvaluator* evaluator;
};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_GROUND_RULECOMPLEXITYEVALUATOR_H_ */
