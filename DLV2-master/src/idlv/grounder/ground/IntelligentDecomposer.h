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
 * IntelligentDecomposer.h
 *
 *  Created on: Mar 28, 2017
 *      Author: jessica
 */

#ifndef SRC_GROUNDER_GROUND_INTELLIGENTDECOMPOSER_H_
#define SRC_GROUNDER_GROUND_INTELLIGENTDECOMPOSER_H_

#include "DecompositionMapper.h"
#include "RuleComplexityEvaluator.h"
#include "DecompositionOptimizer.h"

namespace DLV2 {
namespace grounder {

class IntelligentDecomposer {
public:
	~IntelligentDecomposer(){};
	double evaluateDecomposition(Rule* r,vector<Rule*>::const_iterator begin,vector<Rule*>::const_iterator end){
		double decomposedRulesComplexity=0;
		for(auto it=begin;it!=end;++it)
			decomposedRulesComplexity+=evaluator->evaluate(*it);;
		decomposedRulesComplexity+=evaluator->evaluate(r);
		return decomposedRulesComplexity;
	}

	bool choiceToDecompose(Rule* r);
	bool choiceToDecomposeWithThreshold(Rule* r,double threshold);

	unsigned getTempPredicatesArgSel(Predicate* p,unsigned arg) const {
		if(tempPredicatesArgSel.count(p))
			return tempPredicatesArgSel.find(p)->second[arg];
		return 0;
	}

	void setTempPredicatesArgSel(Predicate* p,unsigned arg, unsigned sel) {
		if(!tempPredicatesArgSel.count(p))
			this->tempPredicatesArgSel[p].resize(p->getArity(),0);
		this->tempPredicatesArgSel[p][arg] = sel;
	}

	unsigned getTempPredicatesSize(Predicate* p) const {
		if(tempPredicatesSize.count(p))
			return tempPredicatesSize.find(p)->second;
		return 0;
	}

	void setTempPredicatesSize(Predicate* p, unsigned size) {
		this->tempPredicatesSize.insert({p,size});
	}

	void cleanTempPredicatesStatistics(Predicate* p){
		tempPredicatesSize.erase(p);
		tempPredicatesArgSel.erase(p);
	}

	static IntelligentDecomposer* getInstance() {
		if(intelligentDecomposer==0)
			intelligentDecomposer=new IntelligentDecomposer;
		return intelligentDecomposer;
	}

	static void freeInstance() {
		delete intelligentDecomposer;
	}

private:
	IntelligentDecomposer():decompositionMapper(DecompositionMapper::getInstance()),evaluator(RuleComplexityEvaluatorFactory::getDefaultEvaluator()){}
	static IntelligentDecomposer* intelligentDecomposer;
	DecompositionMapper* decompositionMapper;
	RuleComplexityEvaluator* evaluator;
	unordered_map<Predicate*, unsigned, IndexForTable<Predicate>,IndexForTable<Predicate>> tempPredicatesSize;
	unordered_map<Predicate*, vector<unsigned>, IndexForTable<Predicate>,IndexForTable<Predicate>> tempPredicatesArgSel;

	void substituteDomainPredicates(Rule* r);
};

} /* namespace grounder */
} /* namespace DLV2 */



#endif /* SRC_GROUNDER_GROUND_INTELLIGENTDECOMPOSER_H_ */
