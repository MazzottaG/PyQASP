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

#ifndef RULE_H
#define RULE_H

#include "Literal.h"
#include "RuleData.h"
#include <vector>
#include <iostream>
#include <cassert>
#include <climits>
#include <unordered_map>
using namespace std;

namespace preasp {

class Interpretation;
class AtomData;

#define NO_TRUE 0
#define MORE_TRUE UINT_MAX
#define SIMPLIFIED UINT_MAX-1

class Rule
{
    public:
        inline Rule(Literal head, Literal body, bool isChoice) { literals.push_back(head); literals.push_back(body); data = RuleData(1, isChoice); }
        inline Rule(Literal head, Literal body1, Literal body2, bool isChoice) { literals.push_back(head); literals.push_back(body1); literals.push_back(body2); data = RuleData(1, isChoice); }
        inline Rule(vector<Literal>& lits, unsigned int headSize, bool isChoice) { literals.swap(lits); data = RuleData(headSize, isChoice); }
        inline Rule(const Rule& r) { literals = r.literals; data = r.data; }        
        
        inline bool isDisjunctive() const { return headSize() >= 2 && !isChoiceRule(); }
        inline bool isConstraint() const { return headSize() == 0 && !isChoiceRule(); }
        inline bool isChoiceRule() const { return data.isChoice(); }
        inline bool isRemoved() const { return data.removed(); }        
        inline void remove() { data.remove(); }        
        inline bool canSupportHead() const { return data.canSupportHead(); }
        inline void disableSupportForHead() { data.disableSupportForHead(); }
        
        inline unsigned int headSize() const { return data.headSize(); }
        inline unsigned int getHeadAtom(unsigned int pos) const { assert(pos < headSize() && pos < literals.size()); return literals[pos].var();}
        inline unsigned int bodySize() const { return literals.size() - headSize(); }
        inline Literal getBodyLiteral(unsigned int pos) const { assert(pos < bodySize() && headSize() + pos < literals.size()); return literals[headSize() + pos];}
        
        unsigned int simplify(Interpretation&);
        void removeLiteralFromBody(Literal lit);
        void removeVarFromHead(unsigned int var);
        Literal getUndefinedBodyLiteral(const Interpretation&, const AtomData& atomData) const;
//        unsigned int getUndefinedHeadAtom(const Interpretation&, const AtomData& atomData) const;
        
        void remap(const AtomData& atomData);
        void printGringo(ostream& o) const;
        void replaceLiterals(unordered_map<int,int>&);        
        inline void addBodyLiteral(Literal lit) { literals.push_back(lit); }
                
    private:
        vector<Literal> literals;
        RuleData data;
        inline void updateHead(unsigned int head) { RuleData l(head, isChoiceRule()); data = l; }
};

}

#endif
