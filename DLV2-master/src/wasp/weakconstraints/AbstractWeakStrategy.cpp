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

#include "AbstractWeakStrategy.h"
#include "../WaspFacade.h"
#include "WeakFacade.h"

void AbstractWeakStrategy::sortOptimizationLiterals() {
    stable_sort(optLiterals.begin(), optLiterals.end(), [this](Literal l1, Literal l2) {return weight(l1) > weight(l2); });
}

AbstractWeakStrategy::AbstractWeakStrategy(WaspFacade& w) : waspFacade(w), weakFacade(w.getWeakFacade()) {}

unsigned int AbstractWeakStrategy::solve() {
    waspFacade.attachAnswerSetListener(this);
    for(unsigned int i = 0; i < weakFacade.numberOfLevels();i++) { lb_.push_back(0); ub_.push_back(UINT64_MAX); }    

    unsigned int res = OPTIMUM_FOUND;
    for(int i=weakFacade.numberOfLevels()-1; i >= 0; i--) {
        currentLevel=i;
        trace_msg(optimization, 1, "Solving level " << currentLevel << ": lb=" << lb() << ", ub=" << ub());
        bool unweighted = createOptLiterals();
        if(lb()==ub()) {
            if(!hardening()) return OPTIMUM_FOUND;
            continue;
        }
        res=run(unweighted);
        if(res==INCOHERENT) return res;
        else if(res==OPTIMUM_FOUND_STOP) return OPTIMUM_FOUND;

        assumptions.clear();
        trace_msg(optimization, 1, "End level " << currentLevel << ": lb=" << lb() << ", ub=" << ub());
        if(!hardening()) return res;
    }

    waspFacade.removeAnswerSetListener(this);
    return res;
}

bool AbstractWeakStrategy::hardening() {
    trace_msg(optimization, 2, "Starting hardening: lb=" << lb() << ", ub=" << ub());
    for(unsigned int i = 0; i < optLiterals.size(); i++) {
        trace_msg(optimization, 3, "Considering " << optLiterals[i] << ", weight: " << weight(optLiterals[i]));
        if(lb()+weight(optLiterals[i]) > ub()) {
            trace_msg(optimization, 4, "...adding " << optLiterals[i].getOppositeLiteral());
            if(!waspFacade.addClause(optLiterals[i].getOppositeLiteral())) return false;
        }
    }
    return true;
}

void AbstractWeakStrategy::foundAnswerSet() {
    uint64_t cost = costOfAnswerSet(currentLevel);
    if(cost >= ub()) return;

    ub_[currentLevel]=cost;
    vector<uint64_t> costs;
    for(int i=currentLevel-1; i >= 0; i--) ub_[i] = costOfAnswerSet(i);
    for(unsigned int i = 0; i < weakFacade.numberOfLevels(); i++) costs.push_back(ub_[i]);
    waspFacade.printAnswerSet();
    waspFacade.getSolver().foundUpperBound(ub());    
    waspFacade.printOptimizationValue(costs);
    return;
}

uint64_t AbstractWeakStrategy::costOfAnswerSet(unsigned int level) {
    trace_msg(optimization, 2, "Computing cost of level " << level);
    uint64_t cost = 0;
    for(unsigned int j = 0; j < weakFacade.numberOfOptLiterals(level); j++) {
        Literal lit = weakFacade.getOptLiteral(j, level).lit();
        uint64_t weight = weakFacade.getOptLiteral(j, level).weight();
        trace_msg(optimization, 3, "Considering literal " << lit << " ");
        if(!waspFacade.isTrue(lit)) { trace_msg(optimization, 4, "...not true: skip!"); continue; }
        
        trace_msg(optimization, 4, "adding weight " << weight);
        cost+=weight;
    }
    trace_msg(optimization, 2, "Cost: " << cost);
    return cost;
}

bool AbstractWeakStrategy::createOptLiterals() {
    if(waspFacade.getSolver().getCurrentDecisionLevel()!=0) resetSolver();
    assert(waspFacade.getSolver().getCurrentDecisionLevel()==0);
    bool unweighted=true;    
    optLiterals.clear();
    soft.clear();
    trace_msg(optimization, 2, "Creating opt literals");
    for(unsigned int i=0; i < weakFacade.numberOfOptLiterals(currentLevel); i++) {
        Literal lit = weakFacade.getOptLiteral(i, currentLevel).lit();
        uint64_t weight = weakFacade.getOptLiteral(i, currentLevel).weight();
        assert(!waspFacade.getSolver().hasBeenEliminated(lit.getVariable()));
        if(waspFacade.isTrue(lit)) { trace_msg(optimization, 3, "Skip " << lit); lb_[currentLevel]+=weight; continue; }
        if(weight > 1) unweighted = false;
        trace_msg(optimization, 3, "Adding " << lit << " whose weight is " << weight);
        addOptimizationLiteral(lit, weight);
    }
    return unweighted;    
}

Var AbstractWeakStrategy::addAuxVariable() { return waspFacade.addVariable(true); }

void AbstractWeakStrategy::resetSolver() { waspFacade.getSolver().unrollToZero(); waspFacade.getSolver().clearConflictStatus(); }