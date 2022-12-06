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

#ifndef WASP_SATELITE_H
#define WASP_SATELITE_H

#include <iostream>
#include <queue>
using namespace std;

#include "../common/Assert.h"
#include "../common/Options.h"
#include "../common/Trace.h"
#include "Clause.h"
#include "stl/UnorderedSet.h"
#include "stl/Heap.h"

class Satelite;

enum SubsumptionData { NO_SUBSUMPTION = 0, SUBSUMPTION = 1, SELFSUBSUMPTION = 2 };

struct EliminationComparator {
    const Satelite& satelite;
    inline bool operator()(Var x, Var y) const;
    EliminationComparator(const Satelite& s) : satelite(s) {}
};

class Satelite {
    public:
        inline Satelite(Solver& s);
        inline ~Satelite() {}
        bool simplify();
        inline void onStrengtheningClause(Clause& clause);
        inline void onDeletingClause(Clause& clause);
        inline unsigned getSubsumptionLimit() const { return subsumptionLimit; }
        inline void addTrueLiteral(Literal l) { trueLiterals.push_back( l ); }
        unsigned int cost(Var v) const;
        inline void disableVariableElimination() { enabledVariableElimination = false; }
        SubsumptionData subsumes(Clause& c1, Clause& c2);
        
    private:
        bool simplificationsMinisat();
        inline void onAddingClause(Clause& clause);
        inline bool subset(const Clause& c1, const Clause& c2);
        bool isSubsumed(Clause& clause, Literal literal);
        bool tryToEliminate(Var variable);
        bool tryToEliminateByDefinition(Var variable);
        bool tryToEliminateByDefinition(Literal lit);
        bool tryToSubstitute(Literal lit, Clause* result);
        void substitute(Var variable, vector< Clause* >& newClauses);
        
        inline bool eliminateVariable( Var variable );
        bool backwardSubsumptionCheck();
        
        void touchVariablesInClause(Clause& clause);
        void gatherTouchedClauses();
        void gatherTouchedClauses(Var variable);
        queue<Clause*> subsumptionQueue;
        
        bool propagateTopLevel();        
        inline void addClauseInSubsumptionQueue(Clause& clause);
        void checkSubsumptionForClause(Clause& clause, Literal bestLiteral);
        bool containsAnyComplementOf(Clause& c1, Clause& c2);

        Solver& solver;
        
        vector<bool> touchedVariables;
        vector<Literal> trueLiterals;
        
        bool ok;
        bool active;
        unsigned int numberOfTouched;        
        //unsigned int clauseLimit; //A variable is not eliminated if it produces a resolvent with a length above this limit.
        unsigned int subsumptionLimit; //Do not check if subsumption against a clause larger than this.        
        bool enabledVariableElimination;
        Heap<EliminationComparator> elim_heap;
};

Satelite::Satelite(Solver& s) : solver(s), ok(true), active(false), numberOfTouched(0),
    subsumptionLimit(1000), enabledVariableElimination(true), elim_heap(EliminationComparator(*this)) {
    touchedVariables.push_back( false );
}

bool EliminationComparator::operator()(Var x, Var y) const { return satelite.cost(x) < satelite.cost(y); }

bool Satelite::subset(const Clause& c1, const Clause& c2) {
    if(c1.size()>c2.size()) return false;    
    if((c1.getSignature() & ~c2.getSignature())!=0) return false;
    
    for(unsigned int i=0; i<c1.size(); i++) {
        bool found=false;
        for(unsigned int j=0; j<c2.size(); j++) {
            if(c1[i]!=c2[j]) continue;
            found=true;
            break;
        }
        if(!found) return false;
    }
    return true;
}

void Satelite::onStrengtheningClause(Clause& clause) {
    if(!active) return;
    trace_msg(satelite, 3, "Strengthening clause " << clause);        
    addClauseInSubsumptionQueue(clause);
    touchVariablesInClause(clause);    
}

void Satelite::onDeletingClause(Clause& clause) {
    if(!active) return;
    trace_msg(satelite, 3, "Deleting clause " << clause);
    touchVariablesInClause(clause);
}

void Satelite::onAddingClause(Clause& clause) {
    assert(active);
    trace_msg(satelite, 3, "Adding clause " << clause);
    addClauseInSubsumptionQueue(clause);
    touchVariablesInClause(clause);
}

bool Satelite::eliminateVariable(Var variable) {
    if(!tryToEliminate(variable)) return true;    
    if(!ok) return false;
    return backwardSubsumptionCheck();
}

void Satelite::addClauseInSubsumptionQueue(Clause& clause) {
    assert(!clause.hasBeenDeleted());
    if(clause.isInQueue()) return;
    clause.setInQueue();
    subsumptionQueue.push(&clause);
}

#endif
