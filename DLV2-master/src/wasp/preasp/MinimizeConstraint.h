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

#ifndef MINIMIZECONSTRAINT_H
#define	MINIMIZECONSTRAINT_H

#include <vector>
#include <cassert>
#include <iostream>
#include <unordered_map>
using namespace std;

#include "Literal.h"

namespace preasp {

class AtomData;
class Interpretation;

class MinimizeConstraint
{
    public:
        inline MinimizeConstraint(vector<Literal>& lits, vector<unsigned int>& w) { literals.swap(lits); weights.swap(w); assert(literals.size() == weights.size()); }
        inline MinimizeConstraint() {}

        void simplifyAndRemap(const Interpretation&, const AtomData&);
        void printGringo(ostream& out) const;
        
        unsigned int getWeight(unsigned int pos) const { assert(pos < weights.size()); return weights[pos]; }
        Literal getLiteral(unsigned int pos) const { assert(pos < literals.size()); return literals[pos]; }
        unsigned int size() const { return literals.size(); }
        
        void copyFrom(const MinimizeConstraint& minimize) {
            literals = minimize.literals;
            weights = minimize.weights;
        }
        
        void replaceLiterals(unordered_map<int,int>& replacements);
        
    private:
        MinimizeConstraint( const MinimizeConstraint& orig );
        vector<Literal> literals;
        vector<unsigned int> weights;
};

}

#endif
