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
#include "PropositionalGrounder.h"
#include "../atom/Choice.h"
#include "../../directives/DirectivesHolder.h"

namespace DLV2{

namespace grounder{
        
void PropositionalGrounder::ground() {
	//Execute the SQL import directives
	bool printOnlyQuery=Options::globalOptions()->isEvaluateQuery();
	bool printedQueryResult=false;
	Predicate *queryPredicate = predicateTable->getQueryPredicate();

	DirectivesHolder::getInstance()->importAll();
	DirectivesHolder::getInstance()->importAllSparql();
	for (unsigned i=0; i < statementDependency->getRulesSize(); i++) {
			if(printOnlyQuery){
				if(statementDependency->getRule(i)->isQueryRule(queryPredicate) && !statementDependency->queryGround()){
					printedQueryResult=true;
					outputBuilder->setEnablePrint(true);
				}
				else
					outputBuilder->setEnablePrint(false);
			}
			groundRule(statementDependency->getRule(i));
	}

	for (unsigned i = 0; i < statementDependency->getConstraintSize(); i++) {
			groundRule(statementDependency->getConstraint(i));
	}

	for (unsigned i = 0; i < statementDependency->getWeakContraintsSize(); i++) {
			groundRule(statementDependency->getWeakContraint(i));
	}

	addTrueNegationConstraints();

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
//		if(queryPredicate!=nullptr && queryPredicate->getName()!=PREDNAME_QUERY && !statementDependency->queryGround()){
//			predicateExtTable->getPredicateExt(queryPredicate)->printQueryFactTable();
//		}
	}
//	outputBuilder->onEnd();
}

bool PropositionalGrounder::groundRule(Rule* r, unordered_set<index_object>* componentPredicateInHead){
	GroundRuleGrounder g;
	return g.groundRule(r,componentPredicateInHead);
}

Atom* GroundRuleGrounder::getAtomToInsert(Atom* atom, bool setFact) {
        auto extTable =PredicateExtTable::getInstance()->getPredicateExt(atom->getPredicate());
        Atom *groundAtom = extTable->getGroundAtom(atom);
        Atom* atomToInsert=nullptr;
        if(groundAtom!=nullptr){
                if(setFact)groundAtom->setFact(true);
                atomToInsert = groundAtom;
        }else{
                atomToInsert = atom->clone();
                atomToInsert->setFact(setFact);
                atomToInsert->setNegative(false);
                extTable->addAtom(atomToInsert, NOFACT);
        }
        
        if(atom->isNegative()){
                atomToInsert = atomToInsert->clone();
                atomToInsert->setNegative(true);
        }
        return atomToInsert;
}

ResultEvaluation GroundRuleGrounder::groundAggregate(DLV2::grounder::Atom *atom, DLV2::grounder::Atom *&gAtom){
        ResultEvaluation result=UNDEF;
        auto fg = atom->getFirstGuard();
        auto sg = atom->getSecondGuard();
        gAtom = new AggregateAtom((fg!=nullptr)?fg->calculate():fg,atom->getFirstBinop(),(sg!=nullptr)?sg->calculate():sg,atom->getSecondBinop(),atom->getAggregateFunction(),atom->isNegative());
        
        
        for (unsigned j = 0; j < atom->getAggregateElementsSize(); j++) {
                auto ae = atom->getAggregateElement(j);
                AggregateElement* gae = new AggregateElement;
                gae->setTerms(ae->getTerms());
                for (auto& naf : ae->getNafLiterals()) {
                        auto gnaf = getAtomToInsert(naf);
                        gae->addNafLiterals(gnaf);
                }
                gAtom->addAggregateElement(gae);
                result = gAtom->partialEvaluate();
                if(result==SATISFY)break;
        }
        if(result==SATISFY)return SATISFY;

        result = gAtom->finalEvaluate();
        return result;
};

void GroundRuleGrounder::groundChoice(Atom* atom, Atom*& gAtom) {
        vector<ChoiceElement*> ces;
        for (unsigned i= 0; i < atom->getChoiceElementsSize(); i++) {
                auto ce = atom->getChoiceElement(i);
                ChoiceElement* gce = new ChoiceElement();
                for (unsigned j = 0; j < ce->getSize(); j++) {
                        auto at = getAtomToInsert(ce->getAtom(j));
                        gce->add(at);
                }
                ces.push_back(gce);
        }
        gAtom = new DLV2::grounder::Choice();
        gAtom->setChoiceElements(ces);           
};


}
}
