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

#include "UnfoundedBasedCheck.h"
#include "../Solver.h"
#include "../../common/Trace.h"
#include "../../common/Options.h"
#include "../WaspFacade.h"

UnfoundedBasedCheck::UnfoundedBasedCheck(
    vector< GUSData* >& gusData_,
    Solver& s,
    unsigned numberOfInputAtoms ) : HCComponent( gusData_, s )
{   
    varsAtLevelZero = 0; numberOfAttachedVars = 0; inUnfoundedSet.push_back( 0 );
    while(checker.numberOfVariables() < numberOfInputAtoms) { checker.addVariable(false); inUnfoundedSet.push_back(0); }
    assert_msg(checker.numberOfVariables() == numberOfInputAtoms, checker.numberOfVariables() << " != " << numberOfInputAtoms);
    assert(checker.numberOfVariables() == inUnfoundedSet.size()-1);    
    initChecker();
}

bool
UnfoundedBasedCheck::onLiteralFalse(
    Literal literal )
{    
    if( solver.getCurrentDecisionLevel() > 0 ) trail.push_back( literal );
    else varsAtLevelZero++;
    
    if(trail.size()+varsAtLevelZero==(numberOfAttachedVars)) { hasToTestModel=true; return true; }
    
    return false;
}

void
UnfoundedBasedCheck::testModel()
{
    trace_msg(modelchecker, 1, "Check component " << *this);
    if(numberOfCalls++ == 0) initDataStructures();    
    
    //The checker will return always unsat
    if(isConflictual) return;

    vector<Literal> assumptions;
    computeAssumptions(assumptions);
    //The checker will return always unsat
    if(isConflictual) return;

    checkModel(assumptions);
}

void
UnfoundedBasedCheck::computeAssumptions(
    vector< Literal >& assumptions )
{
    trace_msg(modelchecker, 1, "Computing assumptions");
    iterationInternalLiterals(assumptions);
    iterationExternalLiterals(assumptions);
    statistics(&checker.getSolver(), assumptions(assumptions.size()));
}

void
UnfoundedBasedCheck::iterationInternalLiterals(
    vector< Literal >& assumptions )
{
    trace_msg(modelchecker, 2, "Iteration on " << hcVariablesNotTrueAtLevelZero.size() << " internal variables");
    for(unsigned int i=0; i < hcVariablesNotTrueAtLevelZero.size(); i++) {
        Var v = hcVariablesNotTrueAtLevelZero[i];
        if(solver.isUndefined(v)) continue;
        
        if(solver.isTrue(v)) assumptions.push_back(Literal(v, POSITIVE));
        else if(solver.isFalse(v)) {
            assumptions.push_back(Literal(v, NEGATIVE));
            assumptions.push_back(Literal(getGUSData(v).unfoundedVarForHCC, NEGATIVE));
        }
    }
}
void

UnfoundedBasedCheck::iterationExternalLiterals(
    vector<Literal>& assumptions) {
    trace_msg(modelchecker, 2, "Iteration on " << externalVars.size() << " external variables");
    unsigned int j=0;
    for(unsigned int i = 0; i < externalVars.size(); i++) {
        Var v = externalVars[j] = externalVars[i];
        if(solver.isUndefined(v)) { j++; continue; }

        if(solver.getDecisionLevel(v) > 0) { assumptions.push_back(Literal(v, solver.isTrue(v) ? POSITIVE : NEGATIVE)); j++; }
        else {
            isConflictual = !checker.addClause(Literal(v, solver.isTrue(v) ? POSITIVE : NEGATIVE));
            if(isConflictual) return;
        }
    }
    externalVars.resize(j);
}

void
UnfoundedBasedCheck::addClausesForHCAtoms() {
    trace_msg(modelchecker, 1, "Simplifying Head Cycle variables");
    if(isConflictual) return;
    
    vector<Literal> clause;
    clause.reserve(hcVariables.size());
    for(unsigned int i=0; i < hcVariables.size(); i++) {
        Var v = hcVariables[i];
        Var uv = getGUSData(v).unfoundedVarForHCC;
        Var hv = getGUSData(v).headVarForHCC;
        
        clause.push_back(Literal(uv, POSITIVE));
        
        vector<Literal> c;
        c.push_back(Literal(uv, POSITIVE));
        c.push_back(Literal(hv, POSITIVE));
        c.push_back(Literal(v, NEGATIVE));        

        isConflictual = 
            !checker.addClause(Literal(uv, NEGATIVE), Literal(hv, NEGATIVE))
        ||  !checker.addClause(Literal(v, POSITIVE), Literal(hv, NEGATIVE))
        ||  !checker.addClause(c);

        trace_msg(modelchecker, 2, "Adding clause: " << Literal(uv, POSITIVE) << ", " << Literal(hv, POSITIVE) << ", " << Literal(v, NEGATIVE));
        trace_msg(modelchecker, 2, "Adding clause: " << Literal(uv, NEGATIVE) << ", " << Literal(hv, NEGATIVE));
        trace_msg(modelchecker, 2, "Adding clause: " << Literal(v, POSITIVE) << ", " << Literal(hv, NEGATIVE));

        if(isConflictual) return;
        if(!solver.isUndefined(v) && solver.getDecisionLevel(v)==0) {
            if(solver.isTrue(v)) isConflictual=!checker.addClause(Literal(v,POSITIVE));
            else isConflictual=!checker.addClause(Literal(v,NEGATIVE)) || !checker.addClause(Literal(getGUSData(v).unfoundedVarForHCC, NEGATIVE));

            if(isConflictual) return;
        }
        else hcVariablesNotTrueAtLevelZero.push_back(v);
    }

    trace_action(modelchecker, 2, {
        trace_tag(cerr, modelchecker, 2);
        if(hcVariables.size()==0) cerr << "No HC variables: adding empty clause." << endl;
        else {
            cerr << "Adding clause: " << Literal(hcVariables[0],POSITIVE);
            for(unsigned int i=1; i < hcVariables.size();i++) cerr << ", " << Literal( hcVariables[i],POSITIVE);
            cerr << endl;
        }
    });
    isConflictual=!checker.addClause(clause);
}

void
UnfoundedBasedCheck::initDataStructures() {
    trace_msg(modelchecker, 2, "First call. Removing unused variables");
    if(isConflictual) return;
    
    addClausesForHCAtoms();
}

void
UnfoundedBasedCheck::checkModel(
    vector< Literal >& assumptions ) {    
    trace_action(modelchecker, 2, { printVector(assumptions, "Assumptions"); });
    statistics(&checker.getSolver(), startCheckerInvocation(trail.size() != (hcVariables.size()+externalVars.size()), time(0)));
    
    vector<Literal> conflict;
    if(checker.solve(assumptions, conflict)==COHERENT) {
        trace_msg(modelchecker, 1, "SATISFIABLE: the model is not stable.");
        for(unsigned int i = 0; i < hcVariables.size(); i++)
            if(checker.isTrue(getGUSData(hcVariables[i]).unfoundedVarForHCC)) setInUnfoundedSet(hcVariables[i]);
        
        trace_action(modelchecker, 2, { printVector(unfoundedSet, "Unfounded set"); });
        statistics(&checker.getSolver(), foundUS(trail.size()!=(hcVariables.size()+externalVars.size()), unfoundedSet.size()));
        assert(!unfoundedSet.empty());
    }
    else { trace_msg(modelchecker, 1, "UNSATISFIABLE: the model is stable."); }
    
    statistics(&checker.getSolver(), endCheckerInvocation(time(0)));
}

void
UnfoundedBasedCheck::processRule(
    int, const vector<int>& headRec,
    const vector<int>& headNonRec, const vector<int>& bodyRec,
    const vector<int>& bodyNonRec) {
    vector<Literal> c;
    if(headRec.size() > 1) for(unsigned int i=0; i < headRec.size(); i++) c.push_back(Literal(getGUSData(headRec[i]).headVarForHCC, POSITIVE));
    for(unsigned int i=0; i < headNonRec.size(); i++) { addExternalVariable(headNonRec[i]); c.push_back(Literal::int2Lit(headNonRec[i])); }
    for(unsigned int i=0; i < bodyNonRec.size(); i++) { addExternalVariable(abs(bodyNonRec[i])); c.push_back(Literal::int2Lit(-bodyNonRec[i])); }
    for(unsigned int i=0; i < bodyRec.size(); i++) { c.push_back(Literal(getGUSData(bodyRec[i]).unfoundedVarForHCC, POSITIVE)); c.push_back(Literal::int2Lit(-bodyRec[i])); }
    
    if(headRec.size() == 1) { c.push_back(Literal(getGUSData(headRec[0]).unfoundedVarForHCC, NEGATIVE)); }
    else {
        Var freshVar = addFreshVariable();
        c.push_back(Literal(freshVar, NEGATIVE));
        for(unsigned int i=0; i < headRec.size(); i++) {
            trace_msg(modelchecker, 2, "Adding clause " << Literal(getGUSData(headRec[i]).unfoundedVarForHCC, NEGATIVE) << ", " << Literal(freshVar, POSITIVE));
            checker.addClause(Literal(getGUSData(headRec[i]).unfoundedVarForHCC, NEGATIVE), Literal(freshVar, POSITIVE));            
        }
    }
    trace_action(modelchecker, 2, { printVector(c, "Created clause"); });
    checker.addClause(c);
    trace_action(modelchecker, 2, { printVector(c, "Added clause"); });
    createDefiningRules(headRec, headNonRec, bodyRec, bodyNonRec);    
}

void
UnfoundedBasedCheck::processComponentBeforeStarting() {
    for(unsigned int j=0; j < hcVariables.size(); j++) {
        Var v = hcVariables[j];
        if(solver.isTrue(v)) onLiteralFalse(Literal(v, NEGATIVE));
        else if(solver.isFalse(v)) onLiteralFalse(Literal(v, POSITIVE));
    }

    for(unsigned int j=0; j < externalVars.size(); j++) {
        Var v = externalVars[j];
        if(solver.isTrue(v)) onLiteralFalse(Literal(v, NEGATIVE));
        else if(solver.isFalse(v)) onLiteralFalse(Literal(v, POSITIVE));
    }
}