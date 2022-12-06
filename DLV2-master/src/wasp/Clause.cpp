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

#include "Clause.h"
#include "Solver.h"

#include <vector>
using namespace std;

ostream& operator<<(ostream & out, const Clause & clause) { return clause.print(out); }

ostream& Clause::print(ostream& out) const {
    if(size()==0) return out << "[]";
    if(hasBeenDeleted()) return out << "-";
    out << "[ " << literals[0];
    for(unsigned int i=1; i<literals.size(); i++) out << " | " << literals[i];
    return out << " ]";
}

void Clause::onLearning(const Solver& solver, Learning* strategy, Literal) {
    assert("LearningStrategy is not initialized." && strategy != NULL);
    //Navigating all literals in the clause.
    assert_msg(solver.getDecisionLevel(literals[1])!=0, "Literal " << literals[1] << " is in position 1 and it has been inferred at level 0. Clause: " << *this);
    strategy->onNavigatingLiteral(literals[1]);
    for(unsigned int i=2; i < literals.size();) {
        Literal literal = literals[i];
        if(solver.getDecisionLevel(literal) != 0) {
            strategy->onNavigatingLiteral(literal);
            i++;
        }
        else {
            assert_msg(solver.isFalse(literal), "Literal " << literal << " is not false.");
            swapLiterals(i,literals.size()-1);
            literals.pop_back();
        }
    }
}

bool Clause::onNavigatingLiteralForAllMarked(const Solver&, Learning* strategy, Literal) {
    assert("LearningStrategy is not initialized." && strategy != NULL);
    for(unsigned int i = 1; i < literals.size(); i++) {
        trace_msg(learning, 5, "Considering literal " << literals[i] << " in position " << i);
        if(!strategy->onNavigatingLiteralForAllMarked(literals[i])) return false;
    }
    return true;
}

void Clause::onNavigatingForUnsatCore(const Solver& solver, vector< unsigned int >& visited, unsigned int numberOfCalls, Literal) {
    //Navigating all literals in the clause.    
    //It was 0 but position 0 contains the assigned literal
    for(unsigned int i=1; i < literals.size(); i++) {
        Var v = literals[i].getVariable();
        if(solver.getDecisionLevel(v) > 0)
            visited[v] = numberOfCalls;
    }
}

bool Clause::removeDuplicatesAndFalseAndCheckIfTautological(Solver& solver) {
    assert(size() > 0);
    literals.sort(literalComparator);
    Literal previousLiteral=literals[0];
    unsigned int j=1;
    for(unsigned int i=1; i < size(); i++) {
        literals[j]=literals[i];
        if(previousLiteral==literals[i].getOppositeLiteral()) return true; //TAUTOLOGICAL
        if(previousLiteral==literals[i]) continue; //DUPLICATE
        if(solver.isTrue(literals[i])) return true;
        if(solver.isFalse(literals[i])) continue;
        previousLiteral=literals[i];
        j++;
    }
    
    if(j!=size()) {
        shrink(j);
        recomputeSignature();
    }
    return false;
}