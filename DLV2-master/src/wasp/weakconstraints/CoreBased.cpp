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

#include "CoreBased.h"
#include "../WaspFacade.h"

unsigned int CoreBased::run(bool unweighted) {
    waspFacade.getSolver().setComputeUnsatCores(true);
    waspFacade.getSolver().turnOffSimplifications();
    if(DLV2::Options::computeFirstModel) {
        waspFacade.setBudget(BUDGET_TIME, DLV2::Options::budget);
        assumptions.clear();
        vector<Literal> conflict;
        unsigned int result = waspFacade.solve(assumptions, conflict);
        waspFacade.budgetOff();
        if(result==INCOHERENT) return result;
    }
    if(DLV2::Options::disjCoresPreprocessing && !disjointCorePreprocessing()) return INCOHERENT;

    if(ub()==lb()) return OPTIMUM_FOUND;

    return (DLV2::Options::stratification && !unweighted) ? runWeighted() : runUnweighted();
}

unsigned int CoreBased::runUnweighted() {
    trace_msg(optimization, 1, "Starting algorithm based on unsatisfiable core (unweighted)");    
    
    vector<Literal> conflict;
    while(true) {
        computeAssumptions(false, optLiterals.size());
        if(waspFacade.solve(assumptions, conflict)!=INCOHERENT) break;
        if(!foundUnsat_(conflict)) return INCOHERENT;
        if(!hardening()) break;
    }
    assert_msg(lb() == ub(), lb() << " != " << ub());
    return OPTIMUM_FOUND;
}

unsigned int CoreBased::runWeighted() {
    trace_msg(optimization, 1, "Starting algorithm based on unsatisfiable core (weighted)");

    preprocessingWeights();
    changeWeight();
    computeAssumptions(true, optLiterals.size());

    while(true) {
        vector<Literal> conflict;
        if(waspFacade.solve(assumptions, conflict)!=INCOHERENT) {
            if(!changeWeight()) break;
        }
        else if(!foundUnsat_(conflict)) return INCOHERENT;

        computeAssumptions(true, optLiterals.size());
        if(lb()==ub()) break;
        if(!hardening()) break;
    }

    assert_msg(lb()==ub(), lb() << " != " << ub());
    return OPTIMUM_FOUND;
}

void CoreBased::preprocessingWeights() {
    for(unsigned int i=0; i < optLiterals.size(); i++)
        weights.push_back(weight(optLiterals[i]));    
    stable_sort(weights.rbegin(), weights.rend());
    vector<uint64_t>::iterator it=unique(weights.begin(),weights.end());
    weights.erase(it,weights.end());
}

void CoreBased::computeAssumptions(bool stratification, unsigned int size) {
    assumptions.clear();
    uint64_t limit_ = stratification ? limit : 0;
    for(unsigned int i = 0; i < size; i++) {
        uint64_t w = weight(optLiterals[i]);
        if(w==0 || w < limit_) continue;
        assumptions.push_back(optLiterals[i].getOppositeLiteral());
    }
    trace_action(optimization,2, {
        trace_tag(cerr,optimization,2);
        cerr << "Assumptions: [";
        for(unsigned int i=0; i < assumptions.size(); i++)
            cerr << " " << assumptions[i];
        cerr << " ]" << endl;
    });
}

bool CoreBased::changeWeight() {
    if(limit==0) return false;
    uint64_t m=0;
    for(unsigned int i=0; i < optLiterals.size(); i++) {
        uint64_t cw = weight(optLiterals[i]);
        if(cw >= limit || cw > ub()) continue;
        m=max(m,cw);
    }

    limit=m;
    return limit!=0;
}

uint64_t CoreBased::computeMinWeight(const vector<Literal>& unsatCore) {
    uint64_t minWeight = UINT64_MAX;
    for(unsigned int i = 0; i < unsatCore.size(); i++)
        minWeight = min(minWeight, weight(unsatCore[i].getOppositeLiteral()));
    assert(minWeight!=UINT64_MAX);
    trace_msg(optimization, 2, "Min weight " << minWeight);
    return minWeight;
}

bool CoreBased::disjointCorePreprocessing() {
    unsigned int size = optLiterals.size();
    computeAssumptions(false, size);    
    vector<Literal> conflict;
    while(true) {
        unsigned int result = waspFacade.solve(assumptions, conflict);
        if(result != INCOHERENT) break;        
        if(!foundUnsat_(conflict)) return false;
        computeAssumptions(false, size);
    }

    return true;
}

bool CoreBased::foundUnsat_(vector<Literal>& unsatCore) {
    trace_action(optimization,2, {
        trace_tag(cerr,optimization,2);
        cerr << "Unsat core: [";
        for(unsigned int i=0; i < unsatCore.size(); i++)
            cerr << " " << unsatCore[i];
        cerr << " ]" << endl;
    });
    //The incoherence does not depend on weak constraints
    if(unsatCore.empty()) return false;
    
    uint64_t minWeight = computeMinWeight(unsatCore);
    for(unsigned int i = 0; i < unsatCore.size(); i++) {
        Literal lit = unsatCore[i].getOppositeLiteral();
        trace_msg(optimization, 3, "Considering literal " << lit << " - weight " << weight(lit));
        removeWeight(lit, minWeight);
    }
    
    incrementLb(minWeight);
    waspFacade.getSolver().foundLowerBound(lb());

    if(unsatCore.size()==1) return true;
    return foundUnsat(unsatCore, minWeight);
}
