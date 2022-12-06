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
#include "ParacoherentSemantics.h"
#include "preasp/Rule.h"
#include "preasp/Builder.h"
#include "../common/ErrorMessage.h"

void ParacoherentSemantics::processRuleSemiequilibrium(preasp::Builder& builder, preasp::Rule& cr) {
    vector<preasp::Literal> head;
    vector<preasp::Literal> body;
    for(unsigned int j = 0; j < cr.headSize(); j++) {
        head.push_back(preasp::Literal(createGapAtom(builder, cr.getHeadAtom(j)), false));
        body.push_back(preasp::Literal(cr.getHeadAtom(j), true));
    }
    for(unsigned int j = 0; j < cr.bodySize(); j++) {
        int bl = cr.getBodyLiteral(j).toInt();
        if(bl < 0) {
            head.push_back(preasp::Literal(createGapAtom(builder, bl), false));
            body.push_back(cr.getBodyLiteral(j));
        }
        else
            body.push_back(preasp::Literal(createGapAtom(builder, bl), false));
    }

    unsigned int headSize = head.size();
    for(unsigned int j = 0; j < body.size(); j++) head.push_back(body[j]);

    builder.addRule(head, headSize, headSize > 1 ? preasp::Builder::DISJUNCTIVE_RULE : preasp::Builder::NORMAL_RULE);
}

void ParacoherentSemantics::processConstraintSemiequilibrium(preasp::Builder& builder, const preasp::Rule* cr, unsigned int aux) {
    vector<preasp::Literal> head;
    head.push_back(preasp::Literal(aux, false));
    vector<preasp::Literal> body;
    for(unsigned int j = 0; j < cr->bodySize(); j++) {
        int bl = cr->getBodyLiteral(j).toInt();
        if(bl < 0) {
            head.push_back(preasp::Literal(createGapAtom(builder, bl), false));
            body.push_back(cr->getBodyLiteral(j));
        }
        else
            body.push_back(preasp::Literal(createGapAtom(builder, bl), false));
    }

    unsigned int headSize = head.size();
    for(unsigned int j = 0; j < body.size(); j++) head.push_back(body[j]);

    builder.addRule(head, headSize, headSize > 1 ? preasp::Builder::DISJUNCTIVE_RULE : preasp::Builder::NORMAL_RULE);
}

void ParacoherentSemantics::processRuleSemistable(preasp::Builder& builder, preasp::Rule* cr) {
    unsigned int size = cr->bodySize();
    for(unsigned int j = 0; j < size; j++) createAndAddGap(builder, cr, cr->getBodyLiteral(j).toInt());
}

//a1| ··· |an←b1,···,bk,∼bk+1,···,∼bm
//Ka1| ··· |Kan|Kbk+1| ··· |Kbm←Kb1,···,Kbk,∼a1,···,∼an,∼bk+1,···,∼bm
//gap in testa per i disgiuntivi, gap in testa per i negativi, gap positivi nel corpo, testa negata nel corpo, negativi ripetuti nel corpo

void ParacoherentSemantics::rewrite(preasp::Builder& builder, const string& semantics) {
    preasp::Program& program = builder.getProgram();
    if(program.aggregates.size() != 0) DLV2::ErrorMessage::errorGeneric("Aggregates are not supported in paracoherent mode");
    if(program.minimizeConstraints.size() != 0) DLV2::ErrorMessage::errorGeneric("Weak constraints are not supported in paracoherent mode");

    if(semantics == "split") {
        for(unsigned int i = 0; i < program.rules.size(); i++) {
            preasp::Rule& r = *program.rules[i];
            assert(!r.isRemoved());
            if(r.isConstraint()) continue;

            if(r.isChoiceRule()) DLV2::ErrorMessage::errorGeneric("Choice rules are not supported in paracoherent mode");

            for(unsigned int j = 0; j < r.bodySize(); j++)
                for(unsigned int k = 0; k < r.headSize(); k++)
                    negDG.addEdge(r.getHeadAtom(k), r.getBodyLiteral(j).var(), !r.getBodyLiteral(j).sign());
        }
    }

    bool semiequilibrium = semantics != "semistable";
    unsigned int pSize = program.rules.size();
    for(unsigned int i = 0; i < pSize; i++) {
        assert(i < program.rules.size());
        preasp::Rule& cr = *program.rules[i];
        if(cr.isChoiceRule()) DLV2::ErrorMessage::errorGeneric("Choice rules are not supported in paracoherent mode");
        if(cr.isConstraint()) {
            unsigned int aux = createGapForConstraint(builder);
            if(semiequilibrium) processConstraintSemiequilibrium(builder, program.rules[i], aux);
            processRuleSemistable(builder, program.rules[i]);
            cr.addBodyLiteral(preasp::Literal(aux, true));
        }
        else {
            if(semiequilibrium) processRuleSemiequilibrium(builder, cr);
            processRuleSemistable(builder, program.rules[i]);
        }

    }
    for(unsigned int i = 0; i < program.binaryConstraints.size(); i++) {
        processUnaryBinaryConstraint(builder, preasp::Literal(1, false), program.binaryConstraints[i], semiequilibrium);
        delete program.binaryConstraints[i];
    }
    program.binaryConstraints.clear();
    for(unsigned int i = 0; i < program.falseAtoms.size(); i++) {
        if(program.falseAtoms[i]==1) continue;
        processUnaryBinaryConstraint(builder, preasp::Literal(program.falseAtoms[i], false), NULL, semiequilibrium);
    }
    program.falseAtoms.clear();
    program.falseAtoms.push_back(1);
    for(unsigned int i = 0; i < program.trueAtoms.size(); i++) processUnaryBinaryConstraint(builder, preasp::Literal(program.trueAtoms[i], true), NULL, semiequilibrium);
    program.trueAtoms.clear();

    builder.addRule(finalChoiceRule, finalChoiceRule.size(), preasp::Builder::CHOICE_RULE);

    if(semantics == "split") {
        //Parameter as true means we want to compute all components
        negDG.computeSCC(true);
        vector< vector<preasp::Literal> > weaks;
        vector< vector<unsigned int> > weights;
        //TODO: name of numberOfCyclicComponents() is misleading, actually computed all components as intended
        bool found = false;
        for(unsigned int i = 0; i < negDG.numberOfCyclicComponents(); i++) {
            const vector<unsigned int>& component = negDG.atoms(i);
            if(!found) {
                if(!negDG.hasOddCycle(i)) {
                    for(unsigned int j = 0; j < component.size(); j++) {
                        if(atomsToK.find(component[j]) != atomsToK.end()) {                            
                            vector<preasp::Literal> v;
                            v.push_back(preasp::Literal(atomsToK[component[j]], false));
                            builder.addRule(v, 0, preasp::Builder::NORMAL_RULE);
                        }
                    }
                    continue;
                }
                else {
                    found = true;
                }
            }
            weaks.push_back(vector<preasp::Literal>());
            weights.push_back(vector<unsigned int>());
            for(unsigned int j = 0; j < component.size(); j++) {
                unsigned int atom = component[j];
                if(atomsToK.find(atom) != atomsToK.end()) {
                    weaks.back().push_back(preasp::Literal(atomsToK[atom], false));
                    weights.back().push_back(1);
                }
            }
        }
        vector<unsigned int> weight;
        for(unsigned int i = 0; i < KfromConstraints.size(); i++) weight.push_back(1);
        if(!KfromConstraints.empty())
            builder.addMinimizeConstraint(KfromConstraints, weight);

        assert(weaks.size() == weights.size());
        for(int i = weaks.size()-1; i >= 0; i--) {
            if(!weaks[i].empty()) {
                builder.addMinimizeConstraint(weaks[i], weights[i]);
            }
        }
        DLV2::Options::predMinimizationAlgorithm = NO_PREDMINIMIZATION;
    }
    else {
        if(DLV2::Options::predMinimizationAlgorithm == PREDMIN_WEAK) {
            builder.addMinimizeConstraint(weakConstraints, weights);
            DLV2::Options::predMinimizationAlgorithm = NO_PREDMINIMIZATION;
        }
    }
}
