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

#include "BinaryConstraint.h"
#include "Interpretation.h"
#include "AtomData.h"

namespace preasp {

unsigned int BinaryConstraint::simplify(Interpretation& interpretation) {
    if(isSatisfied(interpretation)) return CONSTRAINT_SATISFIED;
    if(interpretation.isTrue(lits[0])) return interpretation.isTrue(lits[1]) ? CONSTRAINT_VIOLATED : 1;    
    else if(interpretation.isTrue(lits[1])) return 0;
    return CONSTRAINT_UNDEFINED;    
}

bool BinaryConstraint::isSatisfied(Interpretation& interpretation) const {
    return interpretation.isFalse(lits[0]) || interpretation.isFalse(lits[1]);
}

bool BinaryConstraint::isViolated(Interpretation& interpretation) const {
    return interpretation.isTrue(lits[0]) && interpretation.isTrue(lits[1]);
}

void BinaryConstraint::remap(const AtomData& atomData) {
    for(unsigned int i = 0; i < 2; i++) {
        unsigned int newVar = atomData.getNewVar(lits[i].var());
        if(newVar == 0) {
            cerr << "Failing to remap literal " << lits[i].toInt() << ": something went wrong during initial propagation" << endl;
            exit(5);
        }
        lits[i].setVar(newVar);
    }
}

void BinaryConstraint::printGringo(ostream& out) const {
    out << "1 1 2 ";
    
    if(lits[0].sign() && lits[1].sign()) out << "2 " << lits[0].var() << " " << lits[1].var();
    else if(lits[0].sign() && !lits[1].sign()) out << "1 " << lits[0].var() << " " << lits[1].var();
    else if(!lits[0].sign() && lits[1].sign()) out << "1 " << lits[1].var() << " " << lits[0].var();    
    else out << "0 " << lits[0].var() << " " << lits[1].var();
    out << endl;
}

void BinaryConstraint::replaceLiterals(unordered_map<int,int>& replacements) {
    for(unsigned int i = 0; i <= 1; i++) {
        if(replacements.find(lits[i].toInt()) != replacements.end()) lits[i] = Literal(replacements[lits[i].toInt()]);
        if(replacements.find(-lits[i].toInt()) != replacements.end()) lits[i] = Literal(-replacements[-lits[i].toInt()]);        
    }
}

}
