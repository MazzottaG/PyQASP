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

#ifndef WBOOUTPUT_H
#define	WBOOUTPUT_H

#include <iostream>
#include <sstream>
#include "../Program.h"
using namespace std;

namespace preasp {

class WBOOutput : public VirtualClauseCreator {
    
    public:
        inline WBOOutput(ostream& o) : VirtualClauseCreator(), output(o), nConstr(0), isOpt(0), levels(0) {}
        
        inline void init() {}
        
        inline void printClause(int lit) {
            printLit(lit, 1);
            out << ">= " << (lit > 0 ? "1" : "0") << ";" << endl;
            nConstr++;
        }
        
        inline void printClause(int lit1, int lit2) {
            vector<int> clause;
            clause.push_back(lit1);
            clause.push_back(lit2);
            printClause(clause);
        }
        
        inline void printClause(int lit1, int lit2, int lit3) {
            vector<int> clause;
            clause.push_back(lit1);
            clause.push_back(lit2);
            clause.push_back(lit3);
            printClause(clause);
        }
        inline void printClause(const vector<int>& lits) {
            int countNeg = 0;
            for(unsigned int i = 0; i < lits.size(); i++) {
                if(lits[i] < 0) ++countNeg;
                printLit(lits[i], 1);
            }            
            out << ">= " << (1-countNeg) << ";" << endl;
            nConstr++;
        }
        
        inline void printRuleAsClause(const Rule& rule, int aux) {
            vector<int> clause;
            for(unsigned int i = 0; i < rule.headSize(); i++) clause.push_back(rule.getHeadAtom(i));
            if(aux==0) for(unsigned int i = 0; i < rule.bodySize(); i++) clause.push_back(-rule.getBodyLiteral(i).toInt());
            else clause.push_back(-aux);
            printClause(clause);            
        }                
        
        inline void printAggregate(const Aggregate& aggregate) {
            for(unsigned int i = 0; i < aggregate.numberOfIdBounds(); i++) {                
                if(aggregate.isIdTrue(i)) { printTrueAggregate(aggregate, 0, aggregate.getBound(i)); }
                else if(aggregate.isIdFalse(i)) { printFalseAggregate(aggregate, 0, aggregate.getBound(i)); }
                else {
                    printTrueAggregate(aggregate, aggregate.getId(i), aggregate.getBound(i));
                    printFalseAggregate(aggregate, aggregate.getId(i), aggregate.getBound(i));
                }
            }
        }
        
        inline void printTrueAggregate(const Aggregate& aggregate, int id, unsigned int bound)  {
            int countNeg = 0;
            for(unsigned int i = 0; i < aggregate.size(); i++) {
                if(aggregate.getLiteral(i).toInt() < 0) countNeg+=aggregate.getWeight(i);
                printLit(aggregate.getLiteral(i).toInt(), aggregate.getWeight(i));
            }
            if(id != 0) { printLit(-id, bound); countNeg+=bound; }            
            out << ">= " << ((int) bound - countNeg) << ";" << endl;
            nConstr++;
        }
        
        inline void printFalseAggregate(const Aggregate& aggregate, int id, unsigned int bound)  {
            int sumOfWeights = 0;
            int countNeg = 0;
            for(unsigned int i = 0; i < aggregate.size(); i++) {
                sumOfWeights += aggregate.getWeight(i);
                if(aggregate.getLiteral(i).toInt() > 0) countNeg+=aggregate.getWeight(i);
                printLit(-aggregate.getLiteral(i).toInt(), aggregate.getWeight(i));
            }
            assert(sumOfWeights >= (int) bound);
            int w = sumOfWeights + 1 - (int) bound;
            if(id != 0) printLit(id, w);
            out << ">= " << (w - countNeg) << ";" << endl;
            nConstr++;
        }
        
        inline void printMinimize(const MinimizeConstraint& minimize, unsigned level) {
            if(level > 100) { cerr << "ERROR: exceed maximum number of levels (100) supported with wbo format." << endl; exit(-1); }
            minimize_[level].copyFrom(minimize);            
            levels++;
            //if(isOpt) { cerr << "ERROR: multilevel programs are not supported with wbo format." << endl; exit(-1); }
            isOpt = 1;
            //minimize_.copyFrom(minimize);            
        }

        inline void printEndProgram(unsigned int nVars) {
            output << "* #variable= " << (nVars-1) << " #constraint= " << nConstr << endl;
            if(isOpt) {         
                vector<Literal> lits;
                vector<uint64_t> weights;
                unsigned int currentLevel = 0;
                uint64_t sumOfWeights = 0;
                while(currentLevel < levels) {  
                    unsigned int weight = sumOfWeights;
                    for(unsigned int i = 0; i < minimize_[currentLevel].size(); i++) {
                        lits.push_back(minimize_[currentLevel].getLiteral(i));
                        weights.push_back(minimize_[currentLevel].getWeight(i)+weight);
                        sumOfWeights += minimize_[currentLevel].getWeight(i)+weight;
                    }
                    //sumOfWeights ++;
                    currentLevel++;
                }
                /*if(sumOfWeights > UINT_MAX) {
                    cerr << "WBO Error: Overflow of weights during conversion" << endl;
                    exit(100);
                }*/  

                assert(lits.size() == weights.size());
                output << "min: ";
                for(unsigned int j = 0; j < lits.size(); j++) {                
                    int lit = lits[j].toInt();
                    if(lit == 1) printLit(1, 0, output);
                    else printLit(lit, weights[j], output);
                }
                output << ";" << endl;
            }
            output << out.str();
        }
        
        inline void printVisibleAtom(unsigned int id, const string& name){
            out << "* x" << id << ": " << name << endl;
        }
        
        inline void printComponent(const Program&, const Rule&, unsigned int, int) {
            cerr << "ERROR: only tight programs are supported with wbo format." << endl;
            exit(-1);
        }
        
        inline void printHCComponent(const Program&, const Rule&, int bodyLit, vector<int>&, vector<int>&, vector<int>&, vector<int>&, unsigned int) {
            cerr << "ERROR: only tight programs are supported with wbo format." << endl;
            exit(-1);
        }
        
        inline void printIncoherence() { output << "* #variable= 1 #constraint= 2" << endl; output << "* detected inconsistency" << endl; output << "+1 x1 = 0;" << endl; output << "+1 x1 = 1;" << endl; }
        inline void newVar() {}
        
        inline void printDisjunctivePropagator(vector<unsigned int>&, vector<unsigned int>&, int) {
            cerr << "ERROR: disjunctive propagator is not supported with wbo format." << endl;
            exit(-1);
        }
        
    private:
        ostream& output;
        stringstream out;
        MinimizeConstraint minimize_[100];
        unsigned int nConstr : 31;
        unsigned int isOpt : 1;
        unsigned int levels;
        inline void printLit(int lit, uint64_t weight) { printLit(lit, weight, out); }
        inline void printLit(int lit, uint64_t weight, ostream& o) { lit < 0 ? o << "-" << weight << " x" << -lit << " " : o << "+" << weight << " x" << lit << " "; }
};

}

#endif
