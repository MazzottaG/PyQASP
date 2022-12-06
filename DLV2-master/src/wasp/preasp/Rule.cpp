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

#include "Rule.h"
#include "Interpretation.h"
#include "AtomData.h"

namespace preasp {

unsigned int Rule::simplify(Interpretation& interpretation) {
    unsigned int countTrue = 0;
    unsigned int trueVar = 0;
    unsigned int j = 0;
    unsigned int headS = headSize();
    unsigned int newHeadSize = headS;
    unsigned int trueBody = 0;
    for(unsigned int i = 0; i < literals.size(); i++) {
        literals[j] = literals[i];
        unsigned int var = literals[i].var();
        if(i < headS) {
            if(interpretation.isTrueSupported(var) && !isChoiceRule()) { remove(); return SIMPLIFIED; }
            if(interpretation.isTrueSupported(var)) { --newHeadSize; continue; }
            if(interpretation.isFalse(var)) { --newHeadSize; continue; }
            if(interpretation.isTrue(var)) { trueVar = var; countTrue++; }
        }
        else {
            if(interpretation.isFalse(literals[i])) { remove(); return SIMPLIFIED; }
            if(interpretation.isTrueSupported(var)) { continue; }
            if(interpretation.isTrue(literals[i])) {
                if(literals[i].sign()) continue;
                trueBody++;
            }
        }
        j++;
    }
    updateHead(newHeadSize);    
    literals.resize(j);
    if(newHeadSize == 0 && trueBody==j) literals.clear(); //Violation: head is false, all literals in body are true
    if(countTrue == 0) return NO_TRUE;
    if(countTrue == 1) return trueVar;    
    return MORE_TRUE;
}

void Rule::removeLiteralFromBody(Literal lit) {
    assert(literals.size() > 1);
    for(unsigned int i = headSize(); i < literals.size(); i++) {
        Literal l = literals[i];
        if(lit != l) continue;
        literals[i] = literals.back();
        literals.pop_back();
        return;
    }
}

void Rule::removeVarFromHead(unsigned int var) {
    unsigned int j = 0;
    for(unsigned int i = 0; i < literals.size(); i++) {
        literals[j] = literals[i];
        if(i >= headSize() || literals[j].var() != var) j++;
    }
    literals.resize(j);
    updateHead(headSize()-1);
}

Literal Rule::getUndefinedBodyLiteral(const Interpretation& interpretation, const AtomData& atomData) const {
    for(unsigned int i = 0; i < bodySize(); i++) {
        Literal l = getBodyLiteral(i);
        if(interpretation.isUndefined(l) || !atomData.isPropagated(l.var()))
            return l;
    }
    assert(0);
    return Literal(0, false);
}

//unsigned int Rule::getUndefinedHeadAtom(const Interpretation& interpretation, const AtomData& atomData) const {
//    for(unsigned int i = 0; i < headSize(); i++) {
//        unsigned int var = getHeadAtom(i);
//        if(interpretation.isUndefined(var) || !atomData.isPropagated(var))
//            return var;
//    }
//    assert(0);
//    return 0;
//}

void Rule::remap(const AtomData& atomData) {
    for(unsigned int i = 0; i < literals.size(); i++) {
        unsigned int newVar = atomData.getNewVar(literals[i].var());
        if(newVar == 0) {
            cerr << "Failing to remap literal " << literals[i].toInt() << ": something went wrong during initial propagation" << endl;
            cerr << "Failing rule: "; printGringo(cerr);            
            exit(5);
        }
        literals[i].setVar(newVar);
    }
}

void Rule::printGringo(ostream& out) const {
    if(isRemoved()) return;

    if(isChoiceRule()) out << "3 " << headSize();            
    else if(isDisjunctive()) out << "8 " << headSize();            
    else out << "1" << (isConstraint() ? " 1" : "");

    vector<unsigned int> negatives;
    vector<unsigned int> positives;
    unsigned int i = 0;
    for(; i < headSize(); i++) out << " " << literals[i].var();
    for(; i < literals.size(); i++) literals[i].sign() ? negatives.push_back(literals[i].var()) : positives.push_back(literals[i].var());    

    out << " " << bodySize() << " " << negatives.size();
    for(unsigned int i = 0; i < negatives.size(); i++) out << " " << negatives[i];
    for(unsigned int i = 0; i < positives.size(); i++) out << " " << positives[i];
    out << endl;
}

void Rule::replaceLiterals(unordered_map<int,int>& replacements) {
    for(unsigned int i = 0; i < literals.size(); i++) {
        if(replacements.find(literals[i].toInt()) != replacements.end()) literals[i] = Literal(replacements[literals[i].toInt()]);
        if(replacements.find(-literals[i].toInt()) != replacements.end()) literals[i] = Literal(-replacements[-literals[i].toInt()]); 
    }
}

}
