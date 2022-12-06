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


#include "IntelligentDecomposer.h"
#include "../../util/Constants.h"

namespace DLV2 {
namespace grounder {

IntelligentDecomposer* IntelligentDecomposer::intelligentDecomposer=0;

bool IntelligentDecomposer::choiceToDecompose(Rule* r){
	if(r->getSizeBody()<=2 || r->containsExternalLits()) return false;
	r->computeJoinVariables();
	if(!r->getRuleInformation().areThereJoinVariables()) return false;
	DecompositionOptimizer opt(r);
	opt.decompose();

	if(!decompositionMapper->isDecomposable(r)){
		return false;
	}
	substituteDomainPredicates(r);
	if(Options::globalOptions()->getDecompositionFormula()==DECOMPOSITION_FORMULA_NONE){
		decompositionMapper->setDecomposed(r);
		return true;
	}

	double originalRuleComplexity=evaluator->evaluate(r);
	double decomposedRulesComplexity=evaluateDecomposition(decompositionMapper->get(r),decompositionMapper->getBegin(r),decompositionMapper->getEnd(r));
	bool decomposed=decomposedRulesComplexity<originalRuleComplexity;
	if(decomposed)
		decompositionMapper->setDecomposed(r);
	return decomposed;
}

bool IntelligentDecomposer::choiceToDecomposeWithThreshold(Rule* r,double threshold){
	if(r->getSizeBody()<=2 || r->containsExternalLits()) return false;
	r->computeJoinVariables();
	if(!r->getRuleInformation().areThereJoinVariables()) return false;
	DecompositionOptimizer opt(r);
	opt.decompose();
	if(!decompositionMapper->isDecomposable(r))
		return false;
	substituteDomainPredicates(r);
	if(Options::globalOptions()->getDecompositionFormula()==DECOMPOSITION_FORMULA_NONE){
		decompositionMapper->setDecomposed(r);
		return true;
	}
	double originalRuleComplexity=evaluator->evaluate(r);
	double decomposedRulesComplexity=evaluateDecomposition(decompositionMapper->get(r),decompositionMapper->getBegin(r),decompositionMapper->getEnd(r));
	bool decomposed=(originalRuleComplexity/decomposedRulesComplexity)>=threshold;
	if(decomposed)
		decompositionMapper->setDecomposed(r);
	return decomposed;
}

void IntelligentDecomposer::substituteDomainPredicates(Rule* r) {
	for(auto it=decompositionMapper->getBegin(r);it!=decompositionMapper->getEnd(r);++it){
		Rule* rule=(*it);
		if(!rule->isChoiceRule() && rule->getSizeBody()==1 && rule->getSizeHead()==1){
			Atom* head=rule->getAtomInHead(0);
			string name=rule->getAtomInHead(0)->getPredicate()->getName();
			if(name.find(DECOMPOSITION_PREDICATES_PREFIX) == 0
					&& name.compare (name.length() - DOMAIN_PRED_SUFFIX.length(), DOMAIN_PRED_SUFFIX.length(), DOMAIN_PRED_SUFFIX) == 0){
				set_term varsInHead=head->getVariable();
				Atom* atomInBody=rule->getAtomInBody(0);
				unsigned minSize=PredicateExtTable::getInstance()->getPredicateExt(atomInBody->getPredicate())->getPredicateExtentionSize();
				Atom* minAtom=nullptr;
				for (auto it2 = r->getBeginBody(); it2 != r->getEndBody(); ++it2) {
					Atom* atom = *it2;
					set_term bindVars;
					if (atom->isClassicalLiteral() && !atom->isNegative()){
						unsigned curSize=PredicateExtTable::getInstance()->getPredicateExt(atom->getPredicate())->getPredicateExtentionSize();
						if(curSize<minSize) {
							for (unsigned i = 0; i < atom->getTermsSize(); ++i) {
								Term* term = atom->getTerm(i);
								if(term->getType()==TermType::VARIABLE)
									bindVars.insert(term);
								if(Utils::isContained(varsInHead,bindVars)){
									minAtom=atom;
									minSize=curSize;
									break;
								}
							}
						}
					}
				}
				if(minAtom!=nullptr && minAtom->getPredicate()->getIndex()!=atomInBody->getPredicate()->getIndex()){
					rule->removeInBody(0);
					delete atomInBody;
					rule->addInBody(minAtom->clone());
				}
			}
		}
	}
}

}}
