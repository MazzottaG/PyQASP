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

#ifndef BUILDER_H
#define BUILDER_H

#include "Trie.h"
#include "Literal.h"
#include "Program.h"
#include "../util/Statistics.h"

namespace preasp {

class Builder
{
    public:
        enum RuleType { NORMAL_RULE, DISJUNCTIVE_RULE, CHOICE_RULE };
        inline Builder(Program& p) : program(p), used(false) {}        
        
        inline unsigned int nextFreeVar() const { return program.numberOfVars + 1; }
        inline void addVariable(unsigned int var) { if (var > program.numberOfVars) { program.numberOfVars = var; statistics( NULL, setNumberOfAtoms(var) ); } }
        inline bool existVariable(unsigned int var) const { return var < program.numberOfVars; }
        inline void setVarName(unsigned int var, const string& name) { program.setVarName(var,name); program.addName(var,name); }
        void addRule(vector<Literal>& lits, unsigned int headSize, RuleType r);
        void addAggregate(unsigned int id, vector<Literal>& lits, vector<unsigned int>& weights, unsigned int bound);
        void addMinimizeConstraint(vector<Literal>& lits, vector<unsigned int>& weights);
        void endParse();
        
        bool simplify();
        void computeSCC();
        
        inline Program& getProgram() { return program; }
        void printWellFoundedModel();
        
    private:
        inline Builder(const Builder& orig);        
                
        bool isNormalRuleUseless(vector<Literal>& lits, bool isConstraint);
        bool checkDuplicatesAndRemoveFalse(unsigned int id, vector<Literal>& lits, vector<unsigned int>& weights, unsigned int bound, bool isMinimization);
        bool checkRule(vector<Literal>& lits, unsigned int& headSize, bool isChoiceRule);
        void aggregateIsCountGT1(unsigned int id, const vector<Literal>& lits);
        void aggregateIsCountGT2(unsigned int id, const vector<Literal>& lits);
        
        Program& program;
        Trie aggregatesTrie;
        Trie constraintsTrie;
        Trie rulesTrie;
        vector< pair<Literal,Literal> > aggregateEquivalences;
        vector<Literal> aggregatesIds;
        bool used;
};

}

#endif
