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

#ifndef PROGRAM_H
#define	PROGRAM_H

#include "Rule.h"
#include "Aggregate.h"
#include "MinimizeConstraint.h"
#include "AtomData.h"
#include "DependencyGraph.h"
#include "Options.h"
#include "BinaryConstraint.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
using namespace std;

namespace preasp {

class Program {

    public:
        Program() : numberOfVars(0) {}
        ~Program() { clearAndDeleteRules(); clearAndDeleteAggregates(); clearAndDeleteMinimizeConstraints(); clearAndDeleteBinaryConstraints(); }
        inline void deepClean() {
            clearAndDeleteRules(); clearAndDeleteAggregates(); clearAndDeleteMinimizeConstraints(); clearAndDeleteBinaryConstraints();
            dependencyGraph.deepClean();
            clearVarNames();
        }
        inline void setVarName(unsigned int var, const string& name) { varNames[var] = name; }        
        inline bool isHidden(unsigned int var) const { return varNames.find( var ) == varNames.end(); }
        inline const string& getName(unsigned int var) const { return varNames.at(var); }
        inline void addName(unsigned int var, const string& name) { atomsToPrint_.push_back(var); atomNamesToPrint_.push_back(name); }
        inline unsigned int numberOfAtomsToPrint() const { return atomsToPrint_.size(); }
        inline const string& getNameAtomToPrint(unsigned int pos) const { assert(pos < atomNamesToPrint_.size()); return atomNamesToPrint_[pos]; }
        inline unsigned int getAtomToPrint(unsigned int pos) const { assert(pos < atomsToPrint_.size()); return atomsToPrint_[pos]; }
        
        inline void clearVarNames() {
            unordered_map<unsigned int, string> tmp;
            varNames.swap(tmp);
            vector<unsigned int> tmp2;
            atomsToPrint_.swap(tmp2);
            vector<string> tmp3;
            atomNamesToPrint_.swap(tmp3);
        }
        
        void remapAtomTable(const AtomData& atomData) {
            for(unsigned int i = 0; i <= numberOfVars; i++) {
                unsigned int newVar = atomData.getNewVar(i);
                if(newVar == 0) continue;
                
                if(isHidden(i)) varNames.erase(newVar);
                else setVarName(newVar, getName(i));
                
            }
            unsigned int j = 0;
            for(unsigned int i = 0; i < atomsToPrint_.size(); i++) {
                atomsToPrint_[j] = atomsToPrint_[i];
                atomNamesToPrint_[j] = atomNamesToPrint_[i];
                if(atomData.isEliminated(atomsToPrint_[i])) continue;
                unsigned int newVar = atomData.getNewVar(atomsToPrint_[i]);
                if(newVar == 0) continue;
                atomsToPrint_[j]=newVar;
                j++;
            }
            atomsToPrint_.resize(j);
            atomNamesToPrint_.resize(j);            
        }
        
        inline bool tight() const { return dependencyGraph.numberOfCyclicComponents() == 0; }        
        inline bool hcf() const { return dependencyGraph.hcf(); }
        
        void checkHCF() {
            if(tight()) return;
            for(unsigned int i = 0; i < rules.size(); i++) {
                if(rules[i]->isChoiceRule()) continue;
                unsigned headSize = rules[i]->headSize();                
                if(headSize == 0) continue;
                unordered_set<unsigned int> componentsUsed;
                for(unsigned int j = 0; j < headSize; j++) {
                    unsigned int atom = rules[i]->getHeadAtom(j);
                    if(!dependencyGraph.isInCyclicComponent(atom)) continue;
                    if(!componentsUsed.insert(dependencyGraph.getCyclicComponent(atom)).second)
                        dependencyGraph.setNonHcf(dependencyGraph.getCyclicComponent(atom));
                }
            }
        }
        
        vector<Rule*> rules;
        vector<Aggregate*> aggregates;
        vector<MinimizeConstraint*> minimizeConstraints;
        vector<BinaryConstraint*> binaryConstraints;
        vector<unsigned int> facts;
        vector<unsigned int> falseAtoms;
        vector<unsigned int> trueAtoms;        
        unsigned int numberOfVars;
        DependencyGraph dependencyGraph;
        Options options;
        
    private:
        unordered_map<unsigned int, string> varNames;
        vector<unsigned int> atomsToPrint_;
        vector<string> atomNamesToPrint_;
        
        inline void clearAndDeleteRules() {
            for(unsigned int i = 0; i < rules.size(); i++) delete rules[i];
            vector<Rule*> tmp; tmp.swap(rules);
        }
        
        inline void clearAndDeleteAggregates() {
            for(unsigned int i = 0; i < aggregates.size(); i++) delete aggregates[i];
            vector<Aggregate*> tmp; tmp.swap(aggregates);
        }
        
        inline void clearAndDeleteMinimizeConstraints() {
            for(unsigned int i = 0; i < minimizeConstraints.size(); i++) delete minimizeConstraints[i];
            vector<MinimizeConstraint*> tmp; tmp.swap(minimizeConstraints);
        }        
        
        inline void clearAndDeleteBinaryConstraints () {
            for(unsigned int i = 0; i < binaryConstraints.size(); i++) delete binaryConstraints[i];
            vector<BinaryConstraint*> tmp; tmp.swap(binaryConstraints);
        }
};

}

#endif
