/*
*
*  Copyright 2013 Mario Alviano, Carmine Dodaro, and Francesco Ricca.
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
*/

#include "HCComponent.h"

#include "../Learning.h"
#include "../Clause.h"
#include "../WaspFacade.h"
#include <iostream>
using namespace std;

ostream& operator<<(ostream& out, const HCComponent& component)
{
    out << component.id << ": [ ";
    for(unsigned i = 0; i < component.hcVariables.size(); ++i)
        out << Literal::int2Lit(component.hcVariables[i]) << " ";
    return out << "]";
}

void
HCComponent::computeReasonForUnfoundedAtom(
    Var v,
    Learning& learning)
{
    trace_msg(modelchecker, 2, "Processing variable " << Literal::int2Lit(v));
    vector<unsigned int>& dR = getGUSData(v).definingRulesForNonHCFAtom;
    for(unsigned int i = 0; i < dR.size(); i++) {
        bool skipRule = false;
        Literal l = Literal::null;
        const vector<Var>& recursiveBody = getDefiningRule(dR[i]).recursiveBody();        
        trace_msg(modelchecker, 3, "Recursive body size " << recursiveBody.size());
        for(unsigned int j=0; j < recursiveBody.size(); j++) {
            trace_msg(modelchecker, 4, "Recursive body: " << Literal(recursiveBody[j], POSITIVE));
            if(isInUnfoundedSet(recursiveBody[j])) {
                trace_msg(modelchecker, 5, "Skip rule because of an unfounded positive body literal: " << Literal(recursiveBody[j], POSITIVE));
                skipRule = true;
                break;
            }
            if(solver.isFalse(recursiveBody[j])) l = Literal(recursiveBody[j], POSITIVE);
        }
        if(skipRule) continue;
        const vector<Literal>& supporting = getDefiningRule(dR[i]).supporting();
        trace_msg(modelchecker, 3, "Supporting " << supporting.size());
        for(unsigned int j=0; j < supporting.size(); j++) {
            trace_msg(modelchecker, 4, "Supporting " << supporting[j] << " which is " << (solver.isTrue(supporting[j]) ? "true" : "false/undefined"));
            if(solver.isFalse(supporting[j])) { l = supporting[j]; break; }
        }
        
        if(l == Literal::null) {
            const vector<Var>& recHead = getDefiningRule(dR[i]).recursiveHead();
            trace_msg(modelchecker, 3, "Recursive head size " << recHead.size());
            for(unsigned int j=0; j < recHead.size(); j++) {
                unsigned int headAtom = recHead[j];
                trace_msg(modelchecker, 4, "Recursive head " << headAtom << " which is " << (solver.isTrue(headAtom) ? "true" : "false/undefined"));
                if(!isInUnfoundedSet(headAtom) && solver.isTrue(headAtom)) { l = Literal(headAtom, NEGATIVE); break; }
            }
        }
        
        assert(l != Literal::null);
        assert(!solver.isUndefined(l));
        if(solver.getDecisionLevel(l) > 0) learning.onNavigatingLiteralForUnfoundedSetLearning(l);       
    }
}

HCComponent::~HCComponent()
{
    for(unsigned int i = 0; i < definingRules.size(); i++) delete definingRules[i];    
    checker.getSolver().enableStatistics();
    statistics(&checker.getSolver(), onDeletingChecker(id));    
}

Clause*
HCComponent::getClauseToPropagate(
    Learning& learning)
{
    assert(unfoundedSet.empty());
    if(hasToTestModel) testModel();

    hasToTestModel = false;
    if(!unfoundedSet.empty()) {
        trace_msg(modelchecker, 1, "Learning unfounded set rule for component " << *this);
        Clause* loopFormula = learning.learnClausesFromDisjunctiveUnfoundedSet(unfoundedSet);
        trace_msg(modelchecker, 1, "Adding loop formula: " << *loopFormula);
        unfoundedSet.clear();
        solver.onLearningALoopFormulaFromModelChecker();
        return loopFormula;
    }
    return NULL;
}

void
HCComponent::reset()
{    
    while(!trail.empty() && solver.isUndefined(trail.back())) { hasToTestModel = false; trail.pop_back(); }
    unfoundedSet.clear();
}

void
HCComponent::createDefiningRules(
    const vector<int>& headRec,
    const vector<int>& headNonRec,
    const vector<int>& bodyRec,
    const vector<int>& bodyNonRec)
{   
    DefiningRule* def = new DefiningRule();
    for(unsigned int i = 0; i < bodyNonRec.size(); i++) { assert(bodyNonRec[i] != 0); def->addSupporting(Literal::int2Lit(bodyNonRec[i])); }
    for(unsigned int i = 0; i < headNonRec.size(); i++) { assert(headNonRec[i] != 0); def->addSupporting(Literal::int2Lit(-headNonRec[i])); }
    for(unsigned int i = 0; i < bodyRec.size(); i++) { assert(bodyRec[i] != 0); def->addRecursiveBody(bodyRec[i]); }
    for(unsigned int i = 0; i < headRec.size(); i++) { assert(headRec[i] != 0); def->addRecursiveHead(headRec[i]); }
    for(unsigned int i = 0; i < headRec.size(); i++) getGUSData(headRec[i]).definingRulesForNonHCFAtom.push_back(definingRules.size());
    definingRules.push_back(def);    
}