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

#include "Opt.h"
#include "../WaspFacade.h"
#include "../propagators/Aggregate.h"

unsigned int Opt::run(bool unweighted) {
    assumptions.clear();
    varId = addAuxVariable();
    assumptions.push_back(Literal(varId,NEGATIVE));
    trace_msg(optimization, 1, "Starting algorithm " << (disableprefchoices_ ? "BASIC" : "OPT"));
    if(!unweighted) sortOptimizationLiterals();
    if(!disableprefchoices_) addPreferredChoicesFromOptimizationLiterals();
    numberOfModels=0;
    while(waspFacade.getSolver().solve(assumptions)==COHERENT) {
        numberOfModels++;
        currentCost = costOfOptimizationLiterals();
        trace_msg(optimization, 2, "Cost of optimization literals: " << currentCost);
        trace_msg(optimization, 2, "Decision level of solver: " << waspFacade.getSolver().getCurrentDecisionLevel());
        if(currentCost==0 || waspFacade.getSolver().getCurrentDecisionLevel()==0) break;
        if(!updateOptimizationAggregate()) { trace_msg(optimization, 3, "Failed updating of optimization aggregate: return"); break; }
        trace_msg(optimization, 2, "Calling solver...");
    }   
    
    if(numberOfModels > 0) return completedLevel() ? OPTIMUM_FOUND : OPTIMUM_FOUND_STOP;
    return INCOHERENT;
}

void Opt::createOptimizationAggregate() {
    assert_msg(aggregate == NULL, "Aggregate has been created");
    trace_msg(optimization, 4, "Creating optimization aggregate...");
    
    vector<Literal> literals;
    vector<uint64_t> weights;    
    for(unsigned int i = 0; i < optLiterals.size(); i++) {
        literals.push_back(optLiterals[i]);
        weights.push_back(weight(optLiterals[i]));
    }
    
    Var aggrId = addAuxVariable();
    waspFacade.addClause(Literal(varId,POSITIVE),Literal(aggrId,NEGATIVE));
    aggregate = createAggregate(aggrId, literals, weights);
    processAndAddAggregate(aggregate, currentCost);
    trace_msg(optimization, 5, "...done! Aggregate: " << *aggregate);
}

bool Opt::updateOptimizationAggregate() {
    trace_msg(optimization, 2, "Updating bound of optimization aggregate. Model cost: " << currentCost);
    resetSolver();
    if(aggregate == NULL) {
        trace_msg(optimization, 3, "Optimization aggregate is null: need to be created");
        createOptimizationAggregate();
        return true;
    }    
        
    return aggregate->updateBound(waspFacade.getSolver(), currentCost);        
}

bool Opt::completedLevel() {
    resetSolver();    
    waspFacade.getSolver().removePrefChoices();
    aggregate = NULL;
    varId = 0;
    return createAggregateFromOptimizationLiterals();    
}

bool Opt::createAggregateFromOptimizationLiterals() {
    assert(waspFacade.getSolver().getCurrentDecisionLevel()==0);
    assert(!waspFacade.getSolver().conflictDetected());
    trace_msg(optimization, 3, "Completed level: creating aggregate from optimization literals");
    vector<Literal> literals;
    vector<uint64_t> weights;
    for(unsigned int i=0; i < optLiterals.size(); i++) {
        literals.push_back(optLiterals[i]);
        weights.push_back(weight(optLiterals[i]));
    }

    return createFalseAggregate(literals, weights, currentCost+1);
}

void Opt::addPreferredChoicesFromOptimizationLiterals() {
    assert(waspFacade.getSolver().getCurrentDecisionLevel() == 0);
    vector<Literal> prefChoices;
    for(unsigned int i=0; i < optLiterals.size(); i++) {
        if(waspFacade.isUndefined(optLiterals[i]))
            prefChoices.push_back(optLiterals[i].getOppositeLiteral());
    }
    waspFacade.setPreferredChoices(prefChoices);
}

bool Opt::createFalseAggregate(const vector<Literal>& literals, const vector<uint64_t>& weights, uint64_t bound) {
    assert(literals.size() == weights.size());
    assert(waspFacade.getSolver().getCurrentDecisionLevel() == 0);
    assert(!waspFacade.getSolver().conflictDetected());

    Var aggregateVar = addAuxVariable();
    Literal aggregateLit(aggregateVar, POSITIVE);
    Aggregate* aggregate = createAggregate(aggregateVar, literals, weights);
    trace_msg(optimization, 4, "Created aggregate " << *aggregate << " - with bound " << bound);
    assert(aggregate->size() >= 1);
    #ifndef NDEBUG
    bool res =
    #endif
    waspFacade.addClause(aggregateLit.getOppositeLiteral());
    assert(res);

    //aggregate is already false: return
    if(!aggregate->updateBound(waspFacade.getSolver(),bound)) return true;

    waspFacade.getSolver().attachAggregate(*aggregate);
    waspFacade.getSolver().addAggregate(aggregate);

    assert(waspFacade.isFalse(aggregateLit));
    aggregate->onLiteralFalse(waspFacade.getSolver(),aggregateLit,-1);

    if(waspFacade.getSolver().conflictDetected()) return false;

    return true;
}

Aggregate* Opt::createAggregate(Var aggrId, const vector<Literal>& literals, const vector<uint64_t>& weights) {
    assert(literals.size() == weights.size());

    Aggregate* aggregate = new Aggregate();
    aggregate->addLiteral(Literal(aggrId,NEGATIVE), 0);

    for(unsigned int i = 0; i < literals.size(); i++)
        aggregate->addLiteral(literals[i], weights[i]);
    return aggregate;
}

bool Opt::processAndAddAggregate(Aggregate* aggregate, uint64_t bound) {
    trace_msg(optimization, 4, "Trying to add the aggregate " << *aggregate << " - with bound " << bound);

    #ifndef NDEBUG
    bool res = true;
    #endif
    if(!aggregate->updateBound(waspFacade.getSolver(),bound)) {
        trace_msg( optimization, 5, "Aggregate is already false" );
        #ifndef NDEBUG
        res =
        #endif
        waspFacade.addClause(aggregate->getLiteral(1));
        assert(waspFacade.isFalse(aggregate->getLiteral(1).getVariable()));
    }
    else {
        if(aggregate->isTrue()) {
            trace_msg(optimization, 5, "Aggregate is true");
            #ifndef NDEBUG
            res =
            #endif
            waspFacade.addClause(aggregate->getLiteral(1).getOppositeLiteral());
        }
        waspFacade.getSolver().attachAggregate( *aggregate );
    }

    assert(res);
    waspFacade.getSolver().addAggregate( aggregate );

    trace_msg(optimization, 5, "Adding aggregate " << *aggregate);
    assert(!waspFacade.getSolver().conflictDetected());
    return true;
}

uint64_t Opt::costOfOptimizationLiterals() {    
    uint64_t cost = 0;
    trace_msg(optimization, 2, "Computing cost of optimization literals");
    for(unsigned int j = 0; j < optLiterals.size(); j++) {
        trace_msg(optimization, 3, "Considering " << optLiterals[j]);        
        if(!waspFacade.isTrue(optLiterals[j])) { trace_msg(optimization, 4, "...skip!") continue; }        
        cost+=weight(optLiterals[j]);
        trace_msg(optimization, 4, "...added its weight: " << weight(optLiterals[j]));
    }
    return cost;
}
