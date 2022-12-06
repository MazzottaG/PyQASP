/*
 *  Copyright (C) 2017  Carmine Dodaro (carminedodaro@gmail.com)
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

#include "MinimizeConstraint.h"
#include "Literal.h"
#include "Interpretation.h"
#include "AtomData.h"

#include <iostream>
using namespace std;

namespace preasp {

void MinimizeConstraint::simplifyAndRemap(const Interpretation& interpretation, const AtomData& atomData) {
    assert(literals.size() == weights.size());
    unsigned int j = 0;
    unsigned int sumTrue = 0;
    for(unsigned int i = 0; i < literals.size(); i++) {
        literals[j] = literals[i];
        weights[j] = weights[i];
        if(weights[i]==0) continue;
        if(interpretation.isFalse(literals[i])) continue;
        if(interpretation.isTrue(literals[i])) { sumTrue += weights[i]; continue; }
        unsigned int newVar = atomData.getNewVar(literals[i].var());
        if(newVar == 0) {
            cerr << "Failing to remap literal " << literals[i].toInt() << ": something went wrong during initial propagation" << endl;
            exit(5);
        }        
        literals[j].setVar(newVar);
        j++;
    }
    
    literals.resize(j);
    weights.resize(j);
    
    if(sumTrue>0) {
        literals.push_back(Literal(1, true));
        weights.push_back(sumTrue);
    } else if(literals.empty()) {
        literals.push_back(Literal(1, false));
        weights.push_back(0);
    }
}

void MinimizeConstraint::printGringo(ostream& out) const {
    assert(literals.size() == weights.size());
    
    vector<unsigned int> negative;
    vector<unsigned int> wNeg;
    vector<unsigned int> positive;
    vector<unsigned int> wPos;
    for(unsigned int i = 0; i < literals.size(); i++) {
        if(literals[i].sign()) { negative.push_back(literals[i].var()); wNeg.push_back(weights[i]); }
        else { positive.push_back(literals[i].var()); wPos.push_back(weights[i]); }
    }
    
    out << "6 0";    
    out << " " << literals.size() << " " << negative.size();
    for(unsigned int i = 0; i < negative.size(); i++) out << " " << negative[i];    
    for(unsigned int i = 0; i < positive.size(); i++) out << " " << positive[i];
    
    for(unsigned int i = 0; i < wNeg.size(); i++) out << " " << wNeg[i];    
    for(unsigned int i = 0; i < wPos.size(); i++) out << " " << wPos[i];
    out << endl;
}

void MinimizeConstraint::replaceLiterals(unordered_map<int,int>& replacements) {
    for(unsigned int i = 0; i < literals.size(); i++) {
        if(replacements.find(literals[i].toInt()) != replacements.end()) literals[i] = Literal(replacements[literals[i].toInt()]);
        if(replacements.find(-literals[i].toInt()) != replacements.end()) literals[i] = Literal(-replacements[-literals[i].toInt()]); 
    }
}

}
