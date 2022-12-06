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

#ifndef STREAMCREATOR_H
#define	STREAMCREATOR_H

#include "VirtualClauseCreator.h"
#include <iostream>
using namespace std;

namespace preasp {
class StreamCreator : public VirtualClauseCreator {
    #define AGGREGATE_ID "a"
    #define DISJUNCTIVE_ID "d"
    #define HCC_ID "h"
    #define NUMBER_OF_ATOMS_ID "n"
    #define SOURCE_ID "s"
    #define ATOM_ID "v"
    #define WEAK_ID "w"
    #define VERSION "1 0 0"

    public:
        inline StreamCreator(ostream& o) : VirtualClauseCreator(), out(o) {}
        
        inline void init() { out << "p asp " << VERSION << endl; }
        
        inline void printClause(int lit) { out << lit << " 0" << endl; }
        inline void printClause(int lit1, int lit2)  { out << lit1 << " " << lit2 << " 0" << endl; }
        inline void printClause(int lit1, int lit2, int lit3)  { out << lit1 << " " << lit2 << " " << lit3 << " 0" << endl; }
        inline void printClause(const vector<int>& lits)  {
            for(unsigned int i = 0; i < lits.size(); i++) out << lits[i] << " ";
            out << "0" << endl;
        }
        
        inline void printRuleAsClause(const Rule& rule, int aux)  {
            for(unsigned int i = 0; i < rule.headSize(); i++) out << rule.getHeadAtom(i) << " ";            
            if(aux==0) for(unsigned int i = 0; i < rule.bodySize(); i++) out << -rule.getBodyLiteral(i).toInt() << " ";
            else out << -aux << " ";
            out << "0" << endl;
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
                
        inline void printMinimize(const MinimizeConstraint& minimize, unsigned int level) {
            for(unsigned int j = 0; j < minimize.size(); j++) {
                int lit = minimize.getLiteral(j).toInt();
                if(lit == 1) out << WEAK_ID << " " << 0 << " " << 0 << " " << (level+1) << endl;
                else if(lit == -1) out << WEAK_ID << " " << 0 << " " << minimize.getWeight(j) << " " << (level+1) << endl;
                else out << WEAK_ID << " " << -lit << " " << minimize.getWeight(j) << " " << (level+1) << endl;
            }
        }

        inline void printEndProgram(unsigned int n) { out << NUMBER_OF_ATOMS_ID << " " << n-1 << endl; }
        inline void printVisibleAtom(unsigned int atomId, const string& name){ out << ATOM_ID << " " << atomId << " " << name << endl; }        
        
        inline void printComponent(const Program& program, const Rule& rule, unsigned int head, int bodyLit) {
            assert(program.dependencyGraph.isInCyclicComponent(head));
            out << SOURCE_ID << " " << head << " " << bodyLit;
            for(unsigned int j = 0; j < rule.bodySize(); j++) {
                Literal lit = rule.getBodyLiteral(j);
                if(!lit.sign() && program.dependencyGraph.sameComponent(head, lit.var())) out << " " << lit.var();
            }
            out << " 0" << endl;            
        }
        
        inline void printHCComponent(
          const Program& program, const Rule&, int bodyLit, vector<int>& headRec, 
          vector<int>& headNonRec, vector<int>& bodyRec, vector<int>& bodyNonRec, unsigned int component) {  
            out << HCC_ID << " " << program.dependencyGraph.id(component);
            for(unsigned int i = 0; i < headRec.size(); i++)
                out << " " << headRec[i];
            out << " 0";
            if(bodyLit == 0) {
                for(unsigned int i = 0; i < bodyNonRec.size(); i++)
                    out << " " << bodyNonRec[i];
            }
            else {
                out << " " << bodyLit;                
            }
            for(unsigned int i = 0; i < headNonRec.size(); i++)
                out << " " << -headNonRec[i];                
            out << " 0";
            for(unsigned int i = 0; i < bodyRec.size(); i++)
                out << " " << bodyRec[i];
            out << " 0" << endl;
        }                
        
        inline void printIncoherence() { init(); out << "1 0" << endl; out << "-1 0" << endl; }
        inline void newVar() {}
        
        inline void printDisjunctivePropagator(vector<unsigned int>& headAtoms, vector<unsigned int>& auxAtoms, int bodyLit) {
            assert(auxAtoms.size() == headAtoms.size());
            out << DISJUNCTIVE_ID;            
            out << " " << bodyLit;
            for(unsigned int i=0; i < headAtoms.size(); i++) out << " " << headAtoms[i];
            out << " 0";
            for(unsigned int i=0; i < auxAtoms.size(); i++) out << " " << auxAtoms[i];
            out << endl;
        }
        
    private:
        ostream& out;
        inline void printTrueAggregate(const Aggregate& aggregate, int id, unsigned int bound)  {
            out << AGGREGATE_ID;
            for(unsigned int i = 0; i < aggregate.size(); i++) out << " " << aggregate.getLiteral(i).toInt();
            if(id != 0) out << " " << -id;
            out << " 0";
            for(unsigned int i = 0; i < aggregate.size(); i++) out << " " << aggregate.getWeight(i);
            if(id != 0) out << " " << bound;
            out << " " << bound << endl;            
        }
        
        inline void printFalseAggregate(const Aggregate& aggregate, int id, unsigned int bound)  {
            out << AGGREGATE_ID;
            for(unsigned int i = 0; i < aggregate.size(); i++) out << " " << -aggregate.getLiteral(i).toInt();
            if(id != 0) out << " " << id;
            out << " 0";
            unsigned int sumOfWeights = 0;                
            for(unsigned int i = 0; i < aggregate.size(); i++) { 
                unsigned int weight = aggregate.getWeight(i);
                sumOfWeights += weight;
                out << " " << weight;
            }
            assert(sumOfWeights >= bound);
            unsigned int w = sumOfWeights + 1 - bound;
            if(id != 0) out << " " << w;
            out << " " << w << endl;            
        }        
};

}

#endif
