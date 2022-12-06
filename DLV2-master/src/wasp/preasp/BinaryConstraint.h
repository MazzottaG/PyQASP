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

#ifndef BINARY_CONSTRAINT_H
#define BINARY_CONSTRAINT_H

#include "Literal.h"
#include <iostream>
#include <cassert>
#include <unordered_map>
using namespace std;

namespace preasp {

class AtomData;
class Interpretation;

#define CONSTRAINT_UNDEFINED 2
#define CONSTRAINT_SATISFIED 3
#define CONSTRAINT_VIOLATED 4

class BinaryConstraint
{
    public:
        inline BinaryConstraint(Literal l1, Literal l2) { lits[0] = l1; lits[1] = l2; }
        inline Literal getLiteral(unsigned int pos) const { assert(pos <= 1); return lits[pos]; }
        unsigned int simplify(Interpretation&);
        bool isSatisfied(Interpretation&) const;
        bool isViolated(Interpretation&) const;
        void remap(const AtomData& atomData);
        void printGringo(ostream& o) const;
        void replaceLiterals(unordered_map<int,int>& replacements);

    private:
        Literal lits[2];
};

}

#endif
