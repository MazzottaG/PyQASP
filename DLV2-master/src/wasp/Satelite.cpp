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

#include "Satelite.h"
#include "Solver.h"

bool Satelite::isSubsumed(Clause& clause, Literal literal) {
    for(unsigned i = 0; i < solver.numberOfOccurrences(literal); ++i) {
        Clause* current = solver.getOccurrence(literal, i);
        trace_msg(satelite, 2, "Considering clause " << *current);
        if(&clause != current && subset(*current,clause)) return true;
    }
    return false;
}

bool Satelite::tryToEliminate(Var variable) {
    trace_msg(satelite, 2, "Trying to eliminate variable " << variable);
    if(!solver.isUndefined(variable)) return false;
    
    if(solver.cost(variable) > DLV2::Options::maxCost) return false;    
    Literal pos(variable, POSITIVE);
    Literal neg(variable, NEGATIVE);
    
    if(solver.numberOfOccurrences(pos)>=8 && solver.numberOfOccurrences(neg)>=8) return false;
    
    if(solver.numberOfOccurrences(pos)>=3 && solver.numberOfOccurrences(neg)>=3) {
        unsigned literals=0;
        for(unsigned i=0; i<solver.numberOfOccurrences(pos); ++i) literals+=solver.getOccurrence(pos,i)->size();
        for(unsigned i=0; i<solver.numberOfOccurrences(neg); ++i) literals+=solver.getOccurrence(neg,i)->size();
            
        if(literals > 300) return false;
    }
    
    return tryToEliminateByDefinition(variable);
}

bool Satelite::tryToEliminateByDefinition(Literal lit) {
    assert(POSITIVE == 0 && NEGATIVE == 1);
    Var variable = lit.getVariable();
    Literal oppositeLiteral = lit.getOppositeLiteral();

    Clause negatedDefinition;
    for(unsigned i=0; i<solver.numberOfOccurrences(oppositeLiteral); ++i) {
        Clause* clause = solver.getOccurrence(oppositeLiteral, i);
        if(clause->size() == 2) {
            Literal literal=clause->getAt(0).getVariable()==variable ? clause->getAt(1).getOppositeLiteral() : clause->getAt(0).getOppositeLiteral();
            negatedDefinition.addLiteral(literal);
        }
    }
    for(unsigned i=0; i<negatedDefinition.size(); ++i) {
        if(isSubsumed(negatedDefinition,negatedDefinition.getAt(i))) {
            // DONE: propagate at level 0
            //Propagate the opposite literal
            ok = solver.propagateLiteralAsDeterministicConsequenceSatelite(oppositeLiteral);
            return true;
        }
    }
    
    negatedDefinition.addLiteral(Literal(lit));    
    Clause* result=NULL;
    for(unsigned i=0; i < solver.numberOfOccurrences(lit); ++i) {
        Clause* clause = solver.getOccurrence(lit,i);
        if(subset(*clause,negatedDefinition)) {
            if(result==NULL || result->size()>=clause->size())
                result=clause;
        }
    }
    
    if(result == NULL) return false;
    
    Clause* resultCopy = new Clause();    
    for(unsigned int i = 0; i < result->size(); i++) {
        Literal lit = result->getAt(i);
        if(lit.getVariable() != variable)
            resultCopy->addLiteral(lit);        
    }    
    
    return tryToSubstitute(lit,resultCopy);
}

bool Satelite::tryToSubstitute(Literal lit, Clause* definition) {
    assert(POSITIVE==0 && NEGATIVE==1);
    assert(trueLiterals.empty());
    
    Var variable=lit.getVariable();    
    Literal oppositeLit=lit.getOppositeLiteral();
    trace_msg(satelite, 5, "Trying to substitute " << variable << (lit.getSign()==POSITIVE ? " positive" : " negative" ) << ". Its definition is " << *definition);
    vector<Clause*> newClauses;
    
    for(unsigned i=0; i < definition->size(); ++i) {
        for(unsigned j = 0; j < solver.numberOfOccurrences(lit); ++j) {
            Clause& clause = *solver.getOccurrence(lit,j);
            if(clause.contains(definition->getAt(i))) continue;

            Clause* newClause = new Clause();
            for( unsigned k = 0; k < clause.size(); ++k )
                newClause->addLiteral(clause[k]==lit ? definition->getAt(i).getOppositeLiteral() : clause[k]);
            
            // DONE: remove duplicates and destroy if tautological
            #ifndef NDEBUG
            bool tautological =
            #endif
            newClause->removeDuplicatesAndCheckIfTautological();
            assert_msg(!tautological, "Clause " << *newClause << " is tautological");
            if(newClause->size() == 1) {
                trueLiterals.push_back(newClause->getAt(0));
                delete newClause;
            }
            else
                newClauses.push_back(newClause);
        }
    }
    
    for(unsigned j=0; j < solver.numberOfOccurrences(oppositeLit); ++j) {
        Clause& clause = *solver.getOccurrence(oppositeLit,j);
         
        if(containsAnyComplementOf(clause, *definition)) continue;

        Clause* newClause = new Clause();
        for(unsigned k=0; k<clause.size(); ++k) {
            if(clause[k].getVariable()==variable) {
                for(unsigned i = 0; i < definition->size(); ++i) {
                    if(definition->getAt(i).getVariable()!=variable)
                        newClause->addLiteral(definition->getAt(i));
                }
            }
            else
                newClause->addLiteral(clause[k]);
        }
        
        // DONE: remove duplicates and destroy if tautological
        if(newClause->removeDuplicatesAndCheckIfTautological()) delete newClause;
        else {
            if(newClause->size() == 1) {
                trueLiterals.push_back(newClause->getAt(0));
                delete newClause;
            }
            else
                newClauses.push_back(newClause);
        }
    }

    if(newClauses.size() > solver.numberOfOccurrences(variable)) {
        delete definition;
        for(unsigned int i=0; i<newClauses.size(); ++i)
            delete newClauses[i];
        trueLiterals.clear();
        return false;
    }
    
    substitute(variable, newClauses);
    
    assert_msg(solver.numberOfOccurrences(variable)==0, "Variable " << variable << " has been eliminated but has still " << solver.numberOfOccurrences(variable) << " occurrences" );
    trace_msg(satelite, 2, "Eliminated variable " << variable);
    
    solver.onEliminatingVariable(variable, lit.getSign(), definition);
    ok = propagateTopLevel();
    return true;
}

bool Satelite::tryToEliminateByDefinition(Var variable) {
    Literal pos(variable, POSITIVE);
    Literal neg(variable, NEGATIVE);
    if(solver.numberOfOccurrences(pos)<=1 && solver.numberOfOccurrences(neg)<=1) return false;
    
    if(tryToEliminateByDefinition(pos) || tryToEliminateByDefinition(neg)) return true;    
    return false;
}

bool Satelite::propagateTopLevel() {
    while(!trueLiterals.empty()) {
        Literal lit=trueLiterals.back();
        trueLiterals.pop_back();
        if(!solver.isTrue(lit)) {
            if(!solver.propagateLiteralAsDeterministicConsequenceSatelite(lit)) return false;
        }
    }    
    return true;
}

void Satelite::substitute(Var variable, vector<Clause*>& newClauses) {
    solver.markAllClauses(variable);    
    for(unsigned int i = 0; i < newClauses.size(); i++) {
        Clause& c=*newClauses[i];
        assert(c.size() >= 2);
        onAddingClause(c);
        solver.addClause(newClauses[i]);
    }
}

bool Satelite::simplify() {
    if(!DLV2::Options::simplifications && solver.numberOfClauses() >= DLV2::Options::simplificationsThreshold) return true;
    active=true;
    bool result=simplificationsMinisat();
    active=false;    
    return result;
}


bool Satelite::simplificationsMinisat() {
    assert(solver.callSimplifications()); 
    if(solver.numberOfVariables() > 250000) return true;
    
    for(unsigned int i = 1; i <= solver.numberOfVariables(); i++) {
        assert(touchedVariables.size()==i);
        touchedVariables.push_back(true);
        numberOfTouched++;
        elim_heap.pushNoCheck(i);
    }
    
    assert(numberOfTouched!=0);
    
    while(numberOfTouched>0 && elim_heap.size()>0) {
        gatherTouchedClauses();
        numberOfTouched=0;

        if(!backwardSubsumptionCheck()) return false;
        
        ok=propagateTopLevel();
        if(!ok) return false;
        
        while(!elim_heap.empty()) {
            Var v = elim_heap.removeMin();            
            if(!solver.isUndefined(v) || solver.hasBeenEliminated(v) || solver.isFrozen(v) || !enabledVariableElimination) continue;
            if(!eliminateVariable(v)) return false;
        }
    }

    return true;
}

bool Satelite::backwardSubsumptionCheck() {
    while(!subsumptionQueue.empty()) {
        Clause& clause = *subsumptionQueue.front();
        clause.resetInQueue();
        subsumptionQueue.pop();

        if(clause.hasBeenDeleted()) continue;

        trace_msg(satelite, 1, "Starting subsumption for the clause " << clause);
        Var variable = solver.getVariableWithMinOccurrences(clause);
        assert(variable != 0);
        trace_msg(satelite, 1, "Variable with the min number of occurrences is " << variable);
        
        Literal literal(variable, POSITIVE);
        checkSubsumptionForClause(clause,literal);
        checkSubsumptionForClause(clause,literal.getOppositeLiteral());
                
        ok=propagateTopLevel();
        if(!ok) return false;
    }

    return true;
}

void Satelite::gatherTouchedClauses(Var variable) {
    Literal pos(variable, POSITIVE);
    Literal neg(variable, NEGATIVE);
    for(unsigned int i = 0; i < solver.numberOfOccurrences(pos); i++) {
        assert(solver.getOccurrence(pos,i) != NULL);
        Clause& clause = *solver.getOccurrence(pos,i);
        assert(!clause.hasBeenDeleted() );
        addClauseInSubsumptionQueue(clause);
    }
    
    for(unsigned int i=0; i < solver.numberOfOccurrences(neg); i++) {
        assert(solver.getOccurrence(neg,i) != NULL);
        Clause& clause = *solver.getOccurrence(neg,i);
        assert(!clause.hasBeenDeleted());
        addClauseInSubsumptionQueue(clause);
    }
}

void Satelite::gatherTouchedClauses() {
    for(unsigned int i = 1; i < touchedVariables.size(); i++) {
        if(!touchedVariables[i]) continue;
        gatherTouchedClauses(i);
        touchedVariables[i]=false;
    }
}

void Satelite::checkSubsumptionForClause(Clause& clause, Literal bestLiteral) { solver.checkSubsumptionForClause(clause, bestLiteral); }

void Satelite::touchVariablesInClause(Clause& clause) {
    assert(!clause.hasBeenDeleted());
    for(unsigned int j=0; j<clause.size(); j++) {
        Var variable=clause[j].getVariable();        
        if(solver.isUndefined(variable) && !solver.hasBeenEliminated(variable) && solver.numberOfOccurrences(variable)>0) {
            if(!touchedVariables[variable]) {
                numberOfTouched++;
                if(elim_heap.inHeap(variable)) elim_heap.increase(variable);
            }
        }
    }
}

unsigned int Satelite::cost(Var v) const { return solver.cost(v); }

SubsumptionData Satelite::subsumes(Clause& c1, Clause& c2) {
    if(c1.size() < c2.size() && (c1.getSignature() & ~c2.getSignature())!=0) return NO_SUBSUMPTION;
    SubsumptionData ret = SUBSUMPTION;
    unsigned int position = MAXUNSIGNEDINT;    
    for(unsigned int i=0; i<c1.size(); i++) {
        for(unsigned int j=0; j<c2.size(); j++) {
            if(c1[i]==c2[j]) goto ok;
            else if(ret==SUBSUMPTION && c1[i]==c2[j].getOppositeLiteral()) {
                ret=SELFSUBSUMPTION;
                position=j;
                goto ok;
            }
        }
        
        return NO_SUBSUMPTION;
        ok:;
    }
    
    if(ret==SELFSUBSUMPTION) {
        assert_msg(position < c2.size(), "Position is " << position << " while the size of the clause is " << c2.size());
        c2.swapLiterals(position, c2.size()-1);
    }
    return ret;
}

bool Satelite::containsAnyComplementOf(Clause& c1, Clause& c2) {
    for(unsigned i=0; i < c2.size(); ++i)
        if(c1.contains(c2[i].getOppositeLiteral()))
            return true;
    return false;
}
