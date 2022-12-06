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

#ifndef GRINGOOUTPUT_H
#define	GRINGOOUTPUT_H

#include <iostream>
#include <utility>
#include "../Program.h"
using namespace std;

namespace preasp {

class GringoOutput {
    
    public:
        inline GringoOutput(bool compressLevel_ = false) : compressLevel(compressLevel_) {}
        
        inline void printCoherentProgram(const Program& program, ostream& out) const {
            for(unsigned int i = 0; i < program.facts.size(); i++) out << "1 " << program.facts[i] << " 0 0" << endl;
            for(unsigned int i = 0; i < program.trueAtoms.size(); i++) out << "1 1 1 1 " << program.trueAtoms[i] << endl;            
            for(unsigned int i = 0; i < program.falseAtoms.size(); i++) out << "1 1 1 0 " << program.falseAtoms[i] << endl;            
            for(unsigned int i = 0; i < program.rules.size(); i++) { program.rules[i]->printGringo(out); }
            for(unsigned int i = 0; i < program.binaryConstraints.size(); i++) { program.binaryConstraints[i]->printGringo(out); }
            for(unsigned int i = 0; i < program.aggregates.size(); i++) { program.aggregates[i]->printGringo(out); }
            for(unsigned int i = 0; i < program.minimizeConstraints.size(); i++) { program.minimizeConstraints[i]->printGringo(out); }
            out << "0" << endl;
            for(unsigned int i = 0; i < program.numberOfAtomsToPrint(); i++) out << program.getAtomToPrint(i) << " " << program.getNameAtomToPrint(i) << endl;
            out << "0" << endl;
            out << "B+\n0\nB-\n1\n0\n1" << endl;
        }
        
        inline void printCoherentProgramAndClean(Program& program, ostream& out) {
            for(unsigned int i = 0; i < program.facts.size(); i++) out << "1 " << program.facts[i] << " 0 0" << endl;
            deepClean(program.facts);
            for(unsigned int i = 0; i < program.trueAtoms.size(); i++) out << "1 1 1 1 " << program.trueAtoms[i] << endl;            
            deepClean(program.trueAtoms);
            for(unsigned int i = 0; i < program.falseAtoms.size(); i++) out << "1 1 1 0 " << program.falseAtoms[i] << endl;            
            deepClean(program.falseAtoms);
            for(unsigned int i = 0; i < program.rules.size(); i++) { program.rules[i]->printGringo(out); delete program.rules[i]; }
            deepClean(program.rules);
            for(unsigned int i = 0; i < program.binaryConstraints.size(); i++) { program.binaryConstraints[i]->printGringo(out); delete program.binaryConstraints[i]; }
            deepClean(program.binaryConstraints);
            for(unsigned int i = 0; i < program.aggregates.size(); i++) { program.aggregates[i]->printGringo(out); delete program.aggregates[i]; }
            deepClean(program.aggregates);
            if(compressLevel) {
                vector<Literal> lits;
                vector<unsigned int> weights;
                unsigned int currentLevel = 0;
                uint64_t sumOfWeights = 0;
                while(currentLevel < program.minimizeConstraints.size()) {  
                    unsigned int weight = sumOfWeights;
                    for(unsigned int i = 0; i < program.minimizeConstraints[currentLevel]->size(); i++) {
                        lits.push_back(program.minimizeConstraints[currentLevel]->getLiteral(i));
                        weights.push_back(program.minimizeConstraints[currentLevel]->getWeight(i)+weight);
                        sumOfWeights += program.minimizeConstraints[currentLevel]->getWeight(i)+weight;
                    }
                    delete program.minimizeConstraints[currentLevel];
                    currentLevel++;
                }
                if(sumOfWeights > UINT_MAX) {
                    cerr << "Compress level error: Overflow of weights during conversion" << endl;
                    exit(100);
                }                

                MinimizeConstraint m(lits, weights);
                m.printGringo(out);
            }
            else {
                for(unsigned int i = 0; i < program.minimizeConstraints.size(); i++) { program.minimizeConstraints[i]->printGringo(out); delete program.minimizeConstraints[i]; }
            }
            deepClean(program.minimizeConstraints);
            out << "0" << endl;
            for(unsigned int i = 0; i < program.numberOfAtomsToPrint(); i++) out << program.getAtomToPrint(i) << " " << program.getNameAtomToPrint(i) << endl;
            out << "0" << endl;
            out << "B+\n0\nB-\n1\n0\n1" << endl;
            program.deepClean();            
        }
        
        inline void printIncoherentProgram(ostream& out) const { out << "1 1 0 0\n0\n0\nB+\n0\nB-\n1\n0\n1" << endl; }
    
    private:
        template<class T>
        inline void deepClean(vector<T>& vec) { vector<T> tmp; tmp.swap(vec); }
        bool compressLevel;
};

}

#endif
