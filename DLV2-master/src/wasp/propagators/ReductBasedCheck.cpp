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

#include "ReductBasedCheck.h"
#include "../Solver.h"
#include "../../common/Trace.h"
#include "../../common/Options.h"
#include "../WaspFacade.h"

#include <vector>

ReductBasedCheck::ReductBasedCheck(
    vector< GUSData* >& gusData_,
    Solver& s,
    unsigned numberOfInputAtoms) : HCComponent(gusData_, s),
        numberOfAtoms(numberOfInputAtoms), assumptionLiteral(Literal::null), numberOfExternalLiterals(0),
        numberOfInternalVariables(0), numberOfZeroLevel(0), removedHCVars(0), literalToAdd(Literal::null)
{   
    inUnfoundedSet.push_back(0);
    generatorToCheckerId.push_back(UINT_MAX);
    while(checker.numberOfVariables() < numberOfInputAtoms) {
        inUnfoundedSet.push_back(0);
        checker.addVariable(false);
        generatorToCheckerId.push_back(UINT_MAX);
    }
        
    assert_msg(checker.numberOfVariables() == numberOfInputAtoms, checker.numberOfVariables() << " != " << numberOfInputAtoms);
    assert(checker.numberOfVariables() == inUnfoundedSet.size()-1);
    initChecker();
}

bool
ReductBasedCheck::addExternalLiteral(
    Literal lit)  {
    if(lit.isPositive()) {
        if(inUnfoundedSet[lit.getVariable()] & 1) return false;
        inUnfoundedSet[lit.getVariable()] |= 1;
    }
    else {
        assert(lit.isNegative());
        if(inUnfoundedSet[lit.getVariable()] & 2) return false;
        inUnfoundedSet[lit.getVariable()] |= 2;
        generatorToCheckerId[lit.getVariable()] = addFreshVariable();    
    }
    externalLiterals.push_back(lit);
    numberOfExternalLiterals++;
    return true;
}

bool
ReductBasedCheck::addExternalLiteralForInternalVariable(
    Literal lit )
{
    if(inUnfoundedSet[lit.getVariable()] & 2) return false;
    inUnfoundedSet[lit.getVariable()] |= 2;
    generatorToCheckerId[lit.getVariable()] = addFreshVariable();
    externalLiterals.push_back(lit);
    numberOfExternalLiterals++;
    return true;
}

bool
ReductBasedCheck::onLiteralFalse(
    Literal literal )
{    
    if(solver.getCurrentDecisionLevel() > 0) trail.push_back( literal );
    else numberOfZeroLevel++;
    
    if(trail.size() + numberOfZeroLevel == ( numberOfInternalVariables + numberOfExternalLiterals ) ) { hasToTestModel = true; return true; }    
    return false;
}

void
ReductBasedCheck::testModel()
{
    trace_msg(modelchecker, 1, "Check component " << *this);
    if(numberOfCalls++ == 0) initDataStructures();    
    
    //The checker will return always unsat
    if(isConflictual) return;

    vector<Literal> assumptions;
    computeAssumptions(assumptions);
    //The checker will return always unsat
    if(isConflictual) return;    
    if(unfoundedSetCandidates.empty()) return;
    
    checkModel(assumptions);
    
    clearUnfoundedSetCandidates();        

    assert(!checker.getSolver().conflictDetected());
    if(assumptionLiteral != Literal::null) checker.addClause(assumptionLiteral.getOppositeLiteral());
    assert( !checker.getSolver().conflictDetected() );    
}

void
ReductBasedCheck::computeAssumptions(
    vector< Literal >& assumptions )
{
    trace_msg(modelchecker, 1, "Computing assumptions");
    assert_msg(unfoundedSetCandidates.size() == removedHCVars, unfoundedSetCandidates.size() << "!=" << removedHCVars);
    iterationInternalLiterals(assumptions);
    iterationExternalLiterals(assumptions);
}

void
ReductBasedCheck::iterationInternalLiterals(
    vector<Literal>& assumptions)
{
    trace_msg(modelchecker, 2, "Iteration on internal literals");
    bool hasToAddClause = true;
    vector<Literal> clause;
    for(unsigned int i = 0; i < hcVariables.size(); i++) {
        Literal lit = Literal(hcVariables[i], NEGATIVE);
        trace_msg(modelchecker, 3, "Considering var " << Literal(hcVariables[i], POSITIVE) << " which is " << (solver.isFalse(hcVariables[i]) ? "false" : "true/undefined"));
        if(solver.isFalse(hcVariables[i])) {
            trace_msg(modelchecker, 4, "Adding in assumptions");
            assumptions.push_back(lit);            
        }
        else {
            trace_msg(modelchecker, 4, "Adding in candidates");
            unfoundedSetCandidates.push_back(lit);
            if(!hasToAddClause) continue;
            hasToAddClause = addLiteralInClause(lit, clause);
        }
    }
    
    if(hasToAddClause && literalToAdd != Literal::null) hasToAddClause = addLiteralInClause(literalToAdd, clause);
    
    if(!hasToAddClause) return;
    
    assumptionLiteral = Literal(addFreshVariable(), POSITIVE);
    clause.push_back(assumptionLiteral.getOppositeLiteral());
    assumptions.push_back(assumptionLiteral);

    trace_action(modelchecker, 5, { printVector( clause, "Adding clause" ); });
    #ifndef NDEBUG
    bool result =
    #endif
    checker.addClause(clause);    
    assert(result);
    statistics(&checker.getSolver(), assumptionsOR(clause.size()));
    trace_msg(modelchecker, 6, "Adding " << assumptionLiteral << " as assumption");
}
void

ReductBasedCheck::iterationExternalLiterals(
    vector<Literal>& assumptions)
{
    trace_msg(modelchecker, 2, "Iteration on external literals");
    int j = 0;
    for(unsigned int i=0; i < externalLiterals.size(); i++) {
        Literal lit = externalLiterals[j] = externalLiterals[i];
        trace_msg(modelchecker, 3, "Considering literal " << lit);
        assert(getCheckerVarFromExternalLiteral(lit) != UINT_MAX);
        if(solver.getDecisionLevel(lit) > 0 || solver.isUndefined(lit)) {
            trace_msg(modelchecker, 4, "Adding assumption " << Literal(getCheckerVarFromExternalLiteral(lit), solver.isTrue(lit) ? POSITIVE : NEGATIVE));
            assumptions.push_back(Literal(getCheckerVarFromExternalLiteral(lit), solver.isTrue(lit) ? POSITIVE : NEGATIVE));
            j++;
        }
        else {
            isConflictual = !checker.addClause(Literal(getCheckerVarFromExternalLiteral(lit), solver.isTrue(lit) ? POSITIVE : NEGATIVE));
            if( isConflictual ) return;
        }
    }
    externalLiterals.resize(j);
    
    statistics(&checker.getSolver(), assumptions(assumptions.size()));
}

void
ReductBasedCheck::addClauseToChecker(
    vector<Literal>& clause,
    unsigned int headSize)
{
    trace_action(modelchecker, 2, { printVector( clause, "Original clause" ); });
    for(unsigned int i = 0; i < clause.size(); i++) {
        Var v = clause[i].getVariable();
        if(!sameComponent(v)) {
            Var newVar = getCheckerVarFromExternalLiteral(clause[i]);            
            if(newVar != v) { clause[i].setVariable(newVar); clause[i].setPositive(); }
        }
        else if(i >= headSize && clause[i].isPositive()) {
            assert(getCheckerVarFromExternalLiteral(clause[i]) != UINT_MAX);
            clause[i].setVariable(getCheckerVarFromExternalLiteral(clause[i]));
        }
    }
    trace_action(modelchecker, 3, { printVector( clause, "Added clause" ); });
    checker.addClause(clause);    
}

void
ReductBasedCheck::createInitialClauseAndSimplifyHCVars()
{
    trace_msg( modelchecker, 1, "Simplifying Head Cycle variables" );
    vector<Literal> clause;

    bool satisfied = false;
    unsigned int j = 0;
    for(unsigned int i = 0; i < hcVariables.size(); i++) {
        Var v = hcVariables[j] = hcVariables[i];
        if(solver.isTrue(v) && solver.getDecisionLevel(v) == 0) {
            Literal lit = Literal(v, NEGATIVE);
            unfoundedSetCandidates.push_back(lit);            
            removedHCVars++;
            trace_msg( modelchecker, 2, "Variable " << Literal( v ) << " is true at level 0: removed" );
            if( !satisfied ) satisfied = !addLiteralInClause( lit, clause );
        }
        else
            j++;
    }
    hcVariables.resize(j);        
    
    trace_action(modelchecker, 2, { printVector(clause, "Clause before adding a fresh variable "); });
    trace_msg(modelchecker, 3, "clause is " << (satisfied ? "" : "not") << " satisfied");
    if(clause.empty() || satisfied) { trace_msg(modelchecker, 3, "Removed"); }
    else {
        statistics(&checker.getSolver(), setTrueAtLevelZero(removedHCVars));
        literalToAdd = Literal(addFreshVariable(), NEGATIVE);
        clause.push_back(literalToAdd.getOppositeLiteral());
        
        trace_action(modelchecker, 2, { printVector(clause, "Clause to add"); });
        if(!isConflictual) isConflictual=!checker.addClause(clause);
    }
    
    assert(removedHCVars == unfoundedSetCandidates.size());
}

void
ReductBasedCheck::initDataStructures()
{
    trace_msg( modelchecker, 2, "First call. Removing unused variables" );
    for(unsigned i=1; i < inUnfoundedSet.size(); ++i) {
        if(inUnfoundedSet[i]==0) checker.addClause(Literal(i,POSITIVE));
        else inUnfoundedSet[i]=0;        
    }
//    #ifndef NDEBUG
//    bool result = 
//    #endif  
//    checker.preprocessing();
//    assert(result);
//
//    checker.turnOffSimplifications();
    createInitialClauseAndSimplifyHCVars();
}

void ReductBasedCheck::checkModel(
    vector<Literal>& assumptions)
{    
    trace_action(modelchecker, 2, { printVector(assumptions, "Assumptions"); } );
    trace_action(modelchecker, 2, { printVector(unfoundedSetCandidates, "Unfounded set candidates"); } );
    statistics(&checker.getSolver(), startCheckerInvocation(trail.size() != (hcVariables.size()+externalLiterals.size()), time(0)));
    
    vector<Literal> conflict;    
    if(checker.solve(assumptions, conflict)==COHERENT) {
        trace_msg(modelchecker, 1, "SATISFIABLE: the model is not stable.");
        for(unsigned int i = 0; i < unfoundedSetCandidates.size(); i++)
            if(checker.isTrue(unfoundedSetCandidates[i]))
                setInUnfoundedSet(unfoundedSetCandidates[i].getVariable());
        trace_action(modelchecker, 2, { printVector(unfoundedSet, "Unfounded set"); });
        statistics(&checker.getSolver(), foundUS(trail.size() != (hcVariables.size()+externalLiterals.size()), unfoundedSet.size()));
        assert(!unfoundedSet.empty());        
    }
    else { trace_msg(modelchecker, 1, "UNSATISFIABLE: the model is stable."); }    
    statistics(&checker.getSolver(), endCheckerInvocation(time(0)));    
}

void ReductBasedCheck::addHCVariableProtected(
    Var v)
{
    inUnfoundedSet[v] |= 4;
    numberOfInternalVariables++;
    attachLiterals(Literal(v,POSITIVE));
    solver.setComponent(v, NULL);
    solver.setHCComponent(v, this);
}

void ReductBasedCheck::processRule(
    int,
    const vector<int>& headRec,
    const vector<int>& headNonRec,
    const vector<int>& bodyRec,
    const vector<int>& bodyNonRec)
{
    vector<Literal> clause;
    for(unsigned int i = 0; i < headRec.size(); i++) {
        unsigned int headAtom = headRec[i];
        Literal headAtomLit = Literal::int2Lit(headAtom);
        clause.push_back(headAtomLit); 
    }
    for(unsigned int i = 0; i < headNonRec.size(); i++) {
        unsigned int headAtom = headNonRec[i];
        Literal headAtomLit = Literal::int2Lit(headAtom);
        clause.push_back(headAtomLit); 
        if(addExternalLiteral(headAtomLit)) attachLiterals(headAtomLit);
    }
    for(unsigned int i = 0; i < bodyRec.size(); i++) {
        int body = bodyRec[i];
        Literal bodyLit = Literal::int2Lit(-body);
        clause.push_back(bodyLit);
        if(body < 0 && addExternalLiteralForInternalVariable(bodyLit)) attachLiterals(bodyLit); 
    }
    
    for(unsigned int i = 0; i < bodyNonRec.size(); i++) {
        int body = bodyNonRec[i];
        Literal bodyLit = Literal::int2Lit(-body);
        clause.push_back(bodyLit);
        if(body < 0 && sameComponent(-body)) { if(addExternalLiteralForInternalVariable(bodyLit)) attachLiterals(bodyLit); continue; }
        assert(!sameComponent(bodyLit.getVariable()));
        if(addExternalLiteral(bodyLit)) attachLiterals(bodyLit);
    }

    addClauseToChecker(clause, headRec.size()+headNonRec.size());
    createDefiningRules(headRec, headNonRec, bodyRec, bodyNonRec);
}

void ReductBasedCheck::processComponentBeforeStarting()
{
    for(unsigned int j=0; j < size(); j++) {
        Var v = getVariable(j);
        if(solver.isTrue(v)) onLiteralFalse(Literal(v, NEGATIVE));
        else if(solver.isFalse(v)) onLiteralFalse(Literal(v, POSITIVE));
    }

    for(unsigned int j=0; j < externalLiteralsSize(); j++) {
        Literal lit = getExternalLiteral(j);
        if(solver.isTrue(lit)) onLiteralFalse(lit.getOppositeLiteral());
        else if(solver.isFalse(lit)) onLiteralFalse(lit);
    }
}

