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

#ifndef VIRTUALCLAUSECREATOR_H
#define	VIRTUALCLAUSECREATOR_H

#include <vector>
#include <string>
using namespace std;
#include "../Aggregate.h"
#include "../MinimizeConstraint.h"
#include "../Rule.h"
#include "../Program.h"

namespace preasp {
    
class VirtualClauseCreator {
    public:
        inline VirtualClauseCreator() {}
        virtual ~VirtualClauseCreator() {}
        virtual void init() = 0;
        virtual void printClause(int l) = 0;
        virtual void printClause(int l1, int l2) = 0;
        virtual void printClause(int l1, int l2, int l3) = 0;
        virtual void printClause(const vector<int>& lits) = 0;
        virtual void printRuleAsClause(const Rule& rule, int aux) = 0;
        virtual void printComponent(const Program& program, const Rule& rule, unsigned int head, int bodyLit) = 0;
        virtual void printHCComponent(const Program& program, const Rule& rule, int bodyLit, vector<int>& headRec, vector<int>& headNonRec, vector<int>& bodyRec, vector<int>& bodyNonRec, unsigned int component) = 0;
        virtual void printMinimize(const MinimizeConstraint& minimize, unsigned int level) = 0;
        virtual void printEndProgram(unsigned int n) = 0;
        virtual void printVisibleAtom(unsigned int atomId, const string& name) = 0;
        virtual void newVar() = 0;        
        virtual void printAggregate(const Aggregate& aggregate) = 0;
        virtual void printDisjunctivePropagator(vector<unsigned int>& headAtoms, vector<unsigned int>& auxAtoms, int bodyLit) = 0;
};

}

#endif
