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

#ifndef COMPLETION_H
#define	COMPLETION_H

#include <vector>
#include <iostream>
#include "Program.h"
#include "Rule.h"
#include "Aggregate.h"
#include "Trie.h"
#include "output/VirtualClauseCreator.h"
#include <algorithm>
using namespace std;

namespace preasp {

class Completion
{
    public:
        inline Completion(VirtualClauseCreator* c) : clauseCreator(c) { if(c == NULL) { fprintf(stderr, "COMPLETION ERROR! Clause creator is null\n"); exit(4);}}
        inline ~Completion() { delete clauseCreator; }
        void execute(Program& program, bool clean);        
        
    private:
        vector< vector<int> > supportForAtoms;
        unsigned int nextAux;        
        Trie usedBody;
        VirtualClauseCreator* clauseCreator;        
        
        inline bool supported(unsigned int at) const {
            assert(at < supportForAtoms.size());
            if(supportForAtoms[at].empty()) return false;
            return supportForAtoms[at][0] == -1;
        }
        
        inline void setSupported(unsigned int at) {
            assert(at < supportForAtoms.size());
            if(supported(at)) return; 
            supportForAtoms[at].push_back(-1);
            swapFirstLast(at);
        }
        
        inline void addSupportingLiteral(unsigned int at, int lit) { assert(at < supportForAtoms.size()); supportForAtoms[at].push_back(lit); }
        
        inline bool removeDuplicatesTautology(vector<int>& supp) {
            unordered_set<int> used;
            unsigned int j=0;
            for(unsigned int i = 0; i < supp.size(); i++) {
                supp[j] = supp[i];
                if(used.find(-supp[i]) != used.end()) { supp.clear(); return true; }
                if(used.insert(supp[i]).second) j++;                
            }
            supp.resize(j);
            return false;
        }
        
        inline void swapFirstLast(unsigned int at) {
            assert(at < supportForAtoms.size());
            int tmp = supportForAtoms[at][0];
            supportForAtoms[at][0] = supportForAtoms[at].back();
            supportForAtoms[at].back() = tmp;
        }
        
        inline Completion(const Completion& orig);
                
        int addAuxForBody(const Rule& rule, unsigned int headAtom);
        inline unsigned int newAuxAtom() { clauseCreator->newVar(); return nextAux++; }
        void createDataStructures(unsigned int nbOfVars) { nextAux = nbOfVars+1; usedBody.setNumberOfLabels(nbOfVars); while(supportForAtoms.size() <= nbOfVars) { clauseCreator->newVar(); supportForAtoms.push_back(vector<int>()); } }
        
        inline void printImplication(int lit1, int lit2)  { clauseCreator->printClause(-lit1, lit2); }
        inline void printEquivalence(int lit1, int lit2)  { printImplication(lit1, lit2); printImplication(lit2, lit1); }
        
        /*lit <-> l1 ^ l2*/ inline void litEqTwoAnd(int lit, int l1, int l2) { printImplication(lit, l1); printImplication(lit, l2); clauseCreator->printClause(lit, -l1, -l2); }
        /*lit <-> l1 v l2*/ inline void litEqTwoOr(int lit, int l1, int l2) { printImplication(l1, lit); printImplication(l2, lit); clauseCreator->printClause(-lit, l1, l2); }
        /*lit <-> l1 v ... v ln*/
        inline void litEqOr(int lit, const vector<int>& v ) {
            vector<int> c; c.push_back(-lit);
            for(unsigned int i = 0; i < v.size(); i++) { clauseCreator->printClause(lit, -v[i]); c.push_back(v[i]); }                
            clauseCreator->printClause(c);
        }
        
        void processRule(const Program& program, const Rule& rule);
        void normalizeRule(const Program& program, const Rule& rule, int bodyLit);
        void shift(const Program& program, const Rule& rule);
        
        template<class T>
        inline void deepClean(vector<T>& vec) { vector<T> tmp; tmp.swap(vec); }
        
        inline void printComponent(const Program& program, const Rule& rule, unsigned int head, int bodyLit) {
            if(!program.dependencyGraph.isInCyclicComponent(head)) return;
            if(program.dependencyGraph.hcf(program.dependencyGraph.getCyclicComponent(head)))
                clauseCreator->printComponent(program, rule, head, bodyLit);            
        }
        
        inline void printHCComponent(const Program& program, const Rule& rule, int bodyLit) {
            if(program.hcf()) return;
            vector<unsigned int> components;
            for(unsigned int j = 0; j < rule.headSize(); j++) {
                unsigned int h = rule.getHeadAtom(j);
                if(!program.dependencyGraph.isInCyclicComponent(h)) continue;
                unsigned int component = program.dependencyGraph.getCyclicComponent(h);
                //This check should not be problematic
                if(!program.dependencyGraph.hcf(component) && find(components.begin(), components.end(), component) == components.end())
                    components.push_back(component);                
            }            
            if(components.empty()) return;
            
            if(!rule.isChoiceRule()) {
                for(unsigned int i = 0; i < components.size(); i++) {
                    vector<int> headRec;
                    vector<int> headNonRec;
                    for(unsigned int j = 0; j < rule.headSize(); j++) {
                        unsigned int h = rule.getHeadAtom(j);
                        !program.dependencyGraph.isInCyclicComponent(h) || program.dependencyGraph.getCyclicComponent(h) != components[i] ? headNonRec.push_back(h) : headRec.push_back(h);                    
                    }
                    handleBody(program, rule, bodyLit, headRec, headNonRec, components[i]);                    
                }
            }
            else {
                vector<int> headRec;                        
                vector<int> headNonRec;                        
                for(unsigned int i = 0; i < components.size(); i++) {                    
                    for(unsigned int j = 0; j < rule.headSize(); j++) {
                        unsigned int h = rule.getHeadAtom(j);
                        if(program.dependencyGraph.isInCyclicComponent(h) && program.dependencyGraph.getCyclicComponent(h) == components[i]) {
                            headRec.clear();
                            headRec.push_back(h);
                            handleBody(program, rule, bodyLit, headRec, headNonRec, components[i]);
                        }
                    }
                }
            }
        }
        
        inline void handleBody(const Program& program, const Rule& rule, int bodyLit, vector<int>& headRec, vector<int>& headNonRec, unsigned int component) {
            vector<int> bodyRec;
            vector<int> bodyNonRec;                
            for(unsigned int j = 0; j < rule.bodySize(); j++) {
                Literal lit = rule.getBodyLiteral(j);
                if(lit.sign() || !program.dependencyGraph.isInCyclicComponent(lit.var()) || program.dependencyGraph.getCyclicComponent(lit.var()) != component) {
                    bodyNonRec.push_back(lit.toInt());                        
                }
                else
                    bodyRec.push_back(lit.toInt());                
            }                        
            clauseCreator->printHCComponent(program, rule, bodyLit, headRec, headNonRec, bodyRec, bodyNonRec, program.dependencyGraph.id(component));            
        }
};

}

#endif
