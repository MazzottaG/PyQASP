/*
 *
 *  Copyright 2013 Mario Alviano, Carmine Dodaro, and Francesco Ricca.
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

#ifndef WASPCREATOR_H
#define	WASPCREATOR_H

#include "preasp/output/VirtualClauseCreator.h"
#include "preasp/Aggregate.h"
#include "WaspFacade.h"
#include "preasp/Program.h"
#include "propagators/HCComponent.h"

class HCCUtil
{
    public:
        inline HCCUtil(const preasp::Program& p, int bl, vector<int>& hR, 
                vector<int>& hNR, vector<int>& bR, vector<int>& bNR, unsigned int c) : program(p), bodyLit(bl), component(c) {
            headRec.swap(hR);
            headNonRec.swap(hNR);
            bodyRec.swap(bR);
            bodyNonRec.swap(bNR);
        }
    
        const preasp::Program& program;
        int bodyLit;
        vector<int> headRec;
        vector<int> headNonRec;
        vector<int> bodyRec;
        vector<int> bodyNonRec;
        unsigned int component;
};

class WaspCreator : public preasp::VirtualClauseCreator
{
    public:
        inline WaspCreator(WaspFacade& w) : waspFacade(w), first(true), computedComponents(false), computedHCC(false) { addExternalPropagators(); }
        virtual ~WaspCreator() {}
    
        
        inline void init() {}
        inline void printClause(int l) { waspFacade.addClause(Literal::int2Lit(l)); }
        inline void printClause(int l1, int l2) { waspFacade.addClause(Literal::int2Lit(l1), Literal::int2Lit(l2)); }
        inline void printClause(int l1, int l2, int l3) {
            vector<int> lits; lits.push_back(l1); lits.push_back(l2); lits.push_back(l3);
            printClause(lits);
        }
        
        inline void printClause(const vector<int>& lits) {
            vector<Literal> lits_;
            for(unsigned int i = 0; i < lits.size(); i++) lits_.push_back(Literal::int2Lit(lits[i]));            
            waspFacade.addClause(lits_);
        }                 
        
        inline void printRuleAsClause(const preasp::Rule& rule, int aux) {
            vector<Literal> clause;
            for(unsigned int i = 0; i < rule.headSize(); i++) clause.push_back(Literal::int2Lit(rule.getHeadAtom(i)));
            if(aux==0) for(unsigned int i = 0; i < rule.bodySize(); i++) clause.push_back(Literal::int2Lit(-rule.getBodyLiteral(i).toInt()));
            else clause.push_back(Literal::int2Lit(-aux));            
            waspFacade.addClause(clause);
        }
        
        inline void printComponent(const preasp::Program& program, const preasp::Rule& rule, unsigned int head, int bodyLit) {
            Solver& solver = waspFacade.getSolver();
            if(!computedComponents) {                
                solver.createCyclicComponents(program.dependencyGraph);
                computedComponents = true;                
            }            
            solver.resizeGUSData();
            assert(program.dependencyGraph.isInCyclicComponent(head));
            assert(bodyLit != 0);
            Component* c = solver.getComponent(head);
            assert(c != NULL);
            waspFacade.freeze(bodyLit < 0 ? -bodyLit : bodyLit);
            if(literalsPostPropagator[bodyLit < 0 ? NEGATIVE : POSITIVE][bodyLit].insert(c).second)
                solver.addPostPropagator(Literal::int2Lit(bodyLit),c);
            if(bodyLit < 0) { c->addExternalLiteralForVariable(head,Literal::int2Lit(bodyLit)); return; }
            assert(bodyLit > 0);
            if(rule.bodySize() == 1) {
                int bl = rule.getBodyLiteral(0).toInt();
                //Body lit is external
                if(bl < 0 || c != solver.getComponent(bl)) { c->addExternalLiteralForVariable(head,Literal::int2Lit(bodyLit)); return; }
                //Body lit is internal
                if(bodyLit == bl) {
                    c->addInternalLiteralForVariable(head,Literal::int2Lit(bodyLit));
                    c->addVariablePossiblySupportedByLiteral(head,Literal::int2Lit(bodyLit));
                    waspFacade.freeze(bodyLit);
                    return;
                }
            }
            //Body lit is an auxiliar atom
            bool cyclic = false;
            for(unsigned int i = 0; i < rule.bodySize(); i++) {
                int bl = rule.getBodyLiteral(i).toInt();
                if(bl > 0 && solver.getComponent(head) == solver.getComponent(bl)) { cyclic = true; break; }
            }
            if(!cyclic) { c->addExternalLiteralForVariable(head, Literal::int2Lit(bodyLit)); return; }
            assert(solver.getComponent(bodyLit) == NULL || solver.getComponent(bodyLit) == c);
            if(solver.getComponent(bodyLit)==NULL) {
                solver.setComponent(bodyLit, c);
                c->variableHasNoSourcePointer(bodyLit);
                c->setAuxVariable(bodyLit);
            }
            assert(solver.isFrozen(bodyLit));            
            assert(c->isAuxVariable(bodyLit));
            c->addInternalLiteralForVariable(head, Literal::int2Lit(bodyLit));
            c->addVariablePossiblySupportedByLiteral(head, Literal::int2Lit(bodyLit));            
            //wasp Literals are not the same of preasp Literals...
            for(unsigned int i = 0; i < rule.bodySize(); i++) {
                int bl = rule.getBodyLiteral(i).toInt();
                if(bl > 0 && c == solver.getComponent(bl)) {
                    waspFacade.freeze(bl);
                    c->addInternalLiteralForVariable(bodyLit, Literal::int2Lit(bl));
                    c->addAuxVariableSupportedByLiteral(bodyLit,Literal::int2Lit(bl));
                    if(literalsPostPropagator[bl < 0 ? NEGATIVE : POSITIVE][bl].insert(c).second)
                        solver.addPostPropagator(Literal::int2Lit(bl),c);
                }
            }
        }

        inline void printHCComponent(
          const preasp::Program& program, const preasp::Rule&, int bodyLit, vector<int>& headRec, 
          vector<int>& headNonRec, vector<int>& bodyRec, vector<int>& bodyNonRec, unsigned int component) {
            hccUtils.push_back(HCCUtil(program, bodyLit, headRec, headNonRec, bodyRec, bodyNonRec, component));            
        }
        
        inline void printMinimize(const preasp::MinimizeConstraint& minimize, unsigned int level) {
            for(unsigned int j = 0; j < minimize.size(); j++) {
                int lit = minimize.getLiteral(j).toInt();
                if(lit == 1) waspFacade.addOptimizationLiteral(Literal::int2Lit(lit), 0, level);
                else if(lit == -1) waspFacade.addOptimizationLiteral(Literal::int2Lit(lit), minimize.getWeight(j), level);
                else waspFacade.addOptimizationLiteral(Literal::int2Lit(lit), minimize.getWeight(j), level);
            }
        }
        
        inline void printEndProgram(unsigned int) {
            Solver& solver = waspFacade.getSolver();   
            if(!hccUtils.empty()) {                
                assert(!computedHCC);
                computeHCCs(solver.numberOfVariables(), hccUtils[0].program);
                solver.resizeGUSData();
                for(unsigned int hcUtil = 0; hcUtil < hccUtils.size(); hcUtil++) {
                    HCComponent* c = solver.getHCComponentById(hccUtils[hcUtil].component);
                    c->processRule(hccUtils[hcUtil].bodyLit, hccUtils[hcUtil].headRec, hccUtils[hcUtil].headNonRec, hccUtils[hcUtil].bodyRec, hccUtils[hcUtil].bodyNonRec);
                }
            }
            for(unsigned int i = 0; i < solver.numberOfHCComponents(); i++) solver.getHCComponentById(i)->processComponentBeforeStarting();            
        }
        
        inline void printVisibleAtom(unsigned int atomId, const string& name) { waspFacade.addVariableName(atomId, name); }
        
        inline void newVar() {
            //TODO: quick fix: to change.
            if(first) { first = false; return; }            
            waspFacade.addVariable(false);
        }        
        
        inline void printAggregate(const preasp::Aggregate& aggregate) {
            if(aggregate.numberOfIdBounds() >= 3 && DLV2::Options::multiAggregates) {
                vector<Literal> ids;
                vector<uint64_t> bounds;
                for(unsigned int i = 0; i < aggregate.numberOfIdBounds(); i++) {
                    if(aggregate.isIdTrue(i)) { printTrueAggregate(aggregate, 0, aggregate.getBound(i)); continue; }
                    else if(aggregate.isIdFalse(i)) { printFalseAggregate(aggregate, 0, aggregate.getBound(i)); continue; }
                    assert(aggregate.getId(i) != 0);
                    ids.push_back(Literal::int2Lit(aggregate.getId(i)));
                    bounds.push_back(aggregate.getBound(i));
                }
                
                vector<Literal> lits;
                vector<uint64_t> weights;
                for(unsigned int i = 0; i < aggregate.size(); i++) {
                    assert(aggregate.getLiteral(i).toInt() != 0);
                    if(aggregate.getWeight(i) == 0) continue;
                    lits.push_back(Literal::int2Lit(aggregate.getLiteral(i).toInt()));
                    weights.push_back(aggregate.getWeight(i));
                }
                waspFacade.addMultiAggregate(lits, weights, ids, bounds, false);
                return;
            }
            
            for(unsigned int i = 0; i < aggregate.numberOfIdBounds(); i++) {
                if(aggregate.isIdTrue(i)) { printTrueAggregate(aggregate, 0, aggregate.getBound(i)); }
                else if(aggregate.isIdFalse(i)) { printFalseAggregate(aggregate, 0, aggregate.getBound(i)); }
                else {
                    assert(aggregate.getId(i) != 0);
                    vector<Literal> lits;
                    vector<uint64_t> weights;
                    for(unsigned int i = 0; i < aggregate.size(); i++) {
                        assert(aggregate.getLiteral(i).toInt() != 0);
                        if(aggregate.getWeight(i) == 0) continue;
                        lits.push_back(Literal::int2Lit(aggregate.getLiteral(i).toInt()));
                        weights.push_back(aggregate.getWeight(i));
                    }
                    waspFacade.addAggregate(lits, weights, aggregate.getId(i), aggregate.getBound(i));
                }
            }
        }
        
        inline void printDisjunctivePropagator(vector<unsigned int>& headAtoms, vector<unsigned int>& auxAtoms, int bodyLit) {
            Var firstTrue = 0;
            DisjunctionPropagator* disjunctionPropagator;
            vector<Literal> lits;
            vector<uint64_t> weights;
            if(bodyLit==0) disjunctionPropagator = new DisjunctionPropagator(Literal::null);
            else {
                Literal body = Literal::int2Lit(-bodyLit);
                waspFacade.freeze(body.getVariable());
                disjunctionPropagator = new DisjunctionPropagator(body.getOppositeLiteral());
                lits.push_back(body);
                weights.push_back(2);
            }
            assert(headAtoms.size()==auxAtoms.size());
            for(unsigned int i=0; i < headAtoms.size(); i++) if(waspFacade.isTrue(headAtoms[i])) { firstTrue=headAtoms[i]; break; }
            
            for(unsigned int i=0; i < headAtoms.size(); i++) {
                Literal headLit = Literal::int2Lit(headAtoms[i]);
                Literal aux = Literal::int2Lit(auxAtoms[i]);
                disjunctionPropagator->addOriginalAndAuxLiterals(headLit, aux);
                waspFacade.freeze(headAtoms[i]);
                waspFacade.freeze(auxAtoms[i]);
                waspFacade.addClause(aux.getOppositeLiteral(), headLit);
                lits.push_back(headLit);
                weights.push_back(1);
                if(firstTrue!=0 && firstTrue !=headAtoms[i]) {
                    assert(!waspFacade.isTrue(headAtoms[i]));
                    waspFacade.addClause(aux.getOppositeLiteral());
                }
                else {
                    lits.push_back(aux);
                    weights.push_back(1);              
                }
            }
            Solver& solver = waspFacade.getSolver();
            solver.addDisjunctionPropagator(disjunctionPropagator);
            disjunctionPropagator->finalize(solver);
            waspFacade.addPseudoBooleanConstraint(lits, weights, 2);            
        }

    private:
        WaspFacade& waspFacade;        
        bool first;
        bool computedComponents;
        bool computedHCC;
        unordered_map< Var, unordered_set< PostPropagator* > > literalsPostPropagator[ 2 ];
        vector<HCCUtil> hccUtils;
        
        inline void computeHCCs(unsigned int n, const preasp::Program& program) {
            if(computedHCC) return;
            Solver& solver = waspFacade.getSolver();
            solver.createCyclicComponents(program.dependencyGraph);
            for(unsigned int i = 0; i < program.dependencyGraph.numberOfCyclicComponents(); i++) {                    
                if(program.dependencyGraph.hcf(i)) continue;
                unsigned int compId = program.dependencyGraph.id(i);
                assert(solver.numberOfHCComponents()==compId);
                HCComponent* c = solver.createHCComponent(n);
                c->setId(compId);
                const vector<unsigned int>& atoms = program.dependencyGraph.atoms(i);
                for(unsigned int j = 0; j < atoms.size(); j++) { waspFacade.freeze(atoms[j]); c->addHCVariable(atoms[j]); }
                statistics(&solver, addCyclicHCComponent(atoms.size()));
                solver.addPostPropagator(c);
                solver.addHCComponent(c);
            }
            computedHCC = true;
        }
        
        inline void printTrueAggregate(const preasp::Aggregate& aggregate, int id, unsigned int bound) {
            vector<Literal> lits;
            vector<uint64_t> weights;
            for(unsigned int i = 0; i < aggregate.size(); i++) {
                assert(aggregate.getLiteral(i).toInt() != 0);
                if(aggregate.getWeight(i) == 0) continue;
                lits.push_back(Literal::int2Lit(aggregate.getLiteral(i).toInt()));
                weights.push_back(aggregate.getWeight(i));
            }
            assert(lits.size() == weights.size());
            if(id != 0) { lits.push_back(Literal::int2Lit(-id)); weights.push_back(bound); }
            waspFacade.addPseudoBooleanConstraint(lits, weights, bound);
        }
        
        inline void printFalseAggregate(const preasp::Aggregate& aggregate, int id, unsigned int bound) {
            vector<Literal> lits;
            vector<uint64_t> weights;
            unsigned int sumOfWeights = 0;
            for(unsigned int i = 0; i < aggregate.size(); i++) {
                assert(aggregate.getLiteral(i).toInt() != 0);
                if(aggregate.getWeight(i) == 0) continue;
                lits.push_back(Literal::int2Lit(-aggregate.getLiteral(i).toInt()));
                weights.push_back(aggregate.getWeight(i));
                sumOfWeights += aggregate.getWeight(i);            
            }
            assert(sumOfWeights >= bound);
            unsigned int w = sumOfWeights + 1 - bound;
            if(id != 0) { lits.push_back(Literal::int2Lit(id)); weights.push_back(w); }
            assert(lits.size() == weights.size());
            waspFacade.addPseudoBooleanConstraint(lits, weights, w);
        }
        
        inline void addExternalPropagators() {
            Solver& solver = waspFacade.getSolver();
            for(unsigned int i=0; i < DLV2::Options::pluginsFilenames.size(); i++)
                solver.addExternalPropagator(new ExternalPropagator(DLV2::Options::pluginsFilenames[i].c_str(), DLV2::Options::interpreter, solver, DLV2::Options::scriptDirectory));            

            if(DLV2::Options::heuristic_scriptname != NULL) {
                string name(DLV2::Options::heuristic_scriptname);
                solver.addExternalPropagator(new ExternalPropagator(name.c_str(), DLV2::Options::interpreter, solver, DLV2::Options::scriptDirectory));
            }
        }                
};

#endif

