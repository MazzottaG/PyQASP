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

#include "Completion.h"
#include "Program.h"
#include "output/VirtualClauseCreator.h"
using namespace std;

namespace preasp {

void Completion::execute(Program& program, bool clean) {
    if(program.options.verbosity() >= 1) cerr << "start completion..." << endl;    
    clauseCreator->init();
    
    createDataStructures(program.numberOfVars);
    for(unsigned int i = 0; i < program.falseAtoms.size(); i++) clauseCreator->printClause(-program.falseAtoms[i]);
    for(unsigned int i = 0; i < program.trueAtoms.size(); i++) clauseCreator->printClause(program.trueAtoms[i]);
    for(unsigned int i = 0; i < program.facts.size(); i++) clauseCreator->printClause(program.facts[i]);
    if(clean) {
        deepClean(program.falseAtoms);
        deepClean(program.trueAtoms);
        deepClean(program.facts);
    }
    for(unsigned int i = 0; i < program.binaryConstraints.size(); i++) {
        BinaryConstraint& binary = *program.binaryConstraints[i];        
        clauseCreator->printClause(-binary.getLiteral(0).toInt(), -binary.getLiteral(1).toInt());
        if(clean) delete program.binaryConstraints[i];
    }
    if(clean) deepClean(program.binaryConstraints);
    
    for(unsigned int i = 0; i < program.rules.size(); i++) {
        processRule(program, *program.rules[i]);
        if(clean) delete program.rules[i];
    }
    if(clean) deepClean(program.rules);

    for(unsigned int i = 2; i < supportForAtoms.size(); i++) {
        vector<int>& supp = supportForAtoms[i];
        if(supp.empty() || supported(i)) continue;        
        supp.push_back(-i);
        if(removeDuplicatesTautology(supp)) continue;
        clauseCreator->printClause(supp);
    }
    if(clean) deepClean(supportForAtoms);
    
    for(unsigned int i = 0; i < program.aggregates.size(); i++) { clauseCreator->printAggregate(*program.aggregates[i]); if(clean) delete program.aggregates[i]; }
    if(clean) deepClean(program.aggregates);

    for(unsigned int i = 0; i < program.minimizeConstraints.size(); i++) {
        MinimizeConstraint& minimize = *program.minimizeConstraints[i];
        clauseCreator->printMinimize(minimize, i);
        if(clean) delete program.minimizeConstraints[i];
    }
    if(clean) deepClean(program.minimizeConstraints);

    for(unsigned int i = 0; i < program.numberOfAtomsToPrint(); i++) clauseCreator->printVisibleAtom(program.getAtomToPrint(i), program.getNameAtomToPrint(i));
    clauseCreator->printEndProgram(nextAux);
    if(clean) { program.numberOfVars = 0; program.deepClean(); }
    if(program.options.verbosity() >= 1) cerr << "...end completion" << endl;
}

void Completion::processRule(const Program& program, const Rule& rule) {
    assert(!rule.isRemoved());
    int aux = 0;
    if(rule.isConstraint()) { clauseCreator->printRuleAsClause(rule, aux); return; }    
        
    if(rule.headSize() == 1 || rule.isChoiceRule()) {
        aux = addAuxForBody(rule, 0);        
        for(unsigned int i = 0; i < rule.headSize(); i++) {
            unsigned int head = rule.getHeadAtom(i);            
            if(aux == 0) { assert(!program.dependencyGraph.isInCyclicComponent(head)); setSupported(head); continue; }
            addSupportingLiteral(head, aux);
            printComponent(program, rule, head, aux);
        }
        printHCComponent(program, rule, aux);
    }
    else {
        unsigned int hs = rule.headSize();        
        assert(hs >= 2 && !rule.isChoiceRule());
        if(hs == 2) shift(program, rule);
        else {
            bool propagator = program.options.completionMode() == PREASP_COMPLETION_PROPAGATOR;
            if(program.options.completionMode()==PREASP_COMPLETION_AUTO && hs > 4) propagator = true;
            aux = addAuxForBody(rule, 0);
            if(rule.canSupportHead()) {
                if(propagator) {
                    vector<unsigned int> headAtoms;
                    vector<unsigned int> auxs;                
                    for(unsigned int i = 0; i < rule.headSize(); i++) {
                        headAtoms.push_back(rule.getHeadAtom(i));
                        auxs.push_back(newAuxAtom());
                        addSupportingLiteral(rule.getHeadAtom(i), auxs.back());
                        printComponent(program, rule, rule.getHeadAtom(i), auxs.back());
                    }
                    clauseCreator->printDisjunctivePropagator(headAtoms, auxs, aux);
                }
                else normalizeRule(program, rule, aux);
            }
            printHCComponent(program, rule, aux);
        }        
    }
    if(!rule.isChoiceRule()) clauseCreator->printRuleAsClause(rule, aux);
}

int Completion::addAuxForBody(const Rule& rule, unsigned int headAtom) {
    assert(!rule.isRemoved());
    assert(!rule.isConstraint());
    //headAtom can be 0.
    if(rule.bodySize() == 0) return -headAtom;
    if(rule.bodySize() == 1 && headAtom == 0) return rule.getBodyLiteral(0).toInt();
    
    vector<int> c;
    vector<int> binaryClauses;
    usedBody.startInsertion();
    if(headAtom != 0) { c.push_back(headAtom); binaryClauses.push_back(-headAtom); usedBody.addElement(-headAtom); }
    for(unsigned int i = 0; i < rule.bodySize(); i++) {
        int lit = rule.getBodyLiteral(i).toInt();
        c.push_back(-lit);
        binaryClauses.push_back(lit);
        usedBody.addElement(lit);
    }
    unsigned int usedLit = usedBody.endInsertionGetLabel(nextAux);
    if(usedLit != UINT_MAX) return usedLit;

    unsigned int var = newAuxAtom();
    for(unsigned int i = 0; i < binaryClauses.size(); i++) clauseCreator->printClause(binaryClauses[i], -var);    
    c.push_back(var);    
    clauseCreator->printClause(c);
    return var;
}

void Completion::normalizeRule(const Program& program, const Rule& rule, int bodyLit)
{
    unsigned int m = rule.headSize();
    assert(m >= 3);            
    vector<unsigned int> d;     d.push_back(0);
    vector<unsigned int> s;     s.push_back(bodyLit);
    vector<unsigned int> pr;    pr.push_back(newAuxAtom());
    for(unsigned int i = 0; i < m-1; i++) { d.push_back(newAuxAtom());    s.push_back(newAuxAtom());   pr.push_back(newAuxAtom()); }
    
    //d_i <-> p_i v d_{i+1}
    for(unsigned int i = 1; i < m-1; i++) litEqTwoOr(d[i], rule.getHeadAtom(i), d[i+1]);
    
    //d_m <-> p_m
    printEquivalence(d[m-1], rule.getHeadAtom(m-1));

    //s_i <-> s_{i-1} ^ -p_{i-1}
    for(unsigned int i = 1; i < m; i++) s[i-1]!=0 ? litEqTwoAnd(s[i], s[i-1], -rule.getHeadAtom(i-1)) : printEquivalence(s[i], -rule.getHeadAtom(i-1));    

    //pr_i <-> s_{i} ^ -d_{i+1}
    for(unsigned int i = 0; i < m-1; i++) s[i]!=0 ? litEqTwoAnd(pr[i], s[i], -d[i+1]) : printEquivalence(pr[i], -d[i+1]);    

    //pr_m <-> s_m
    printEquivalence(pr[m-1], s[m-1]);
    for(unsigned int i = 0; i < m; i++) { addSupportingLiteral(rule.getHeadAtom(i), pr[i]); printComponent(program, rule, rule.getHeadAtom(i), pr[i]); }    
}

void Completion::shift(const Program& program, const Rule& rule) {
    assert(rule.headSize() == 2);
    if(rule.canSupportHead()) {
        unsigned int h1 = rule.getHeadAtom(0);
        unsigned int h2 = rule.getHeadAtom(1);
        int aux1 = addAuxForBody(rule, h2);
        int aux2 = addAuxForBody(rule, h1);

        addSupportingLiteral(h1, aux1);
        addSupportingLiteral(h2, aux2);
        printComponent(program, rule, h1, aux1);
        printComponent(program, rule, h2, aux2);
    }
    printHCComponent(program, rule, 0);
}

}
