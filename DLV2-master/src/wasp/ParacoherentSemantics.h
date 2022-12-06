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
#ifndef PARACOHERENTSEMANTICS_H
#define	PARACOHERENTSEMANTICS_H

#include <unordered_map>
using namespace std;

#include "preasp/Builder.h"

class ParacoherentSemantics
{
    public:
        class Constraint : public preasp::Rule
        {
            public:
                Constraint(vector<preasp::Literal>& lits) : preasp::Rule(lits, 0, false) {}
                void fromBinaryConstraint(preasp::BinaryConstraint& cr) { addBodyLiteral(cr.getLiteral(0)); addBodyLiteral(cr.getLiteral(1)); }
                void fromUnaryConstraint(preasp::Literal lit) { addBodyLiteral(lit); }                
        };
        
        inline ParacoherentSemantics() {}
        inline virtual ~ParacoherentSemantics(){}
        void rewrite(preasp::Builder& builder, const string& semantics);
        
    private:
        void processRuleSemiequilibrium(preasp::Builder& builder, preasp::Rule& cr);
        void processRuleSemistable(preasp::Builder& builder, preasp::Rule* cr);
        void processConstraintSemiequilibrium(preasp::Builder& builder, const preasp::Rule* cr, unsigned int aux);
        
        unordered_map<unsigned int, unsigned int> atomsToK;
        vector<preasp::Literal> finalChoiceRule;
        vector<preasp::Literal> weakConstraints;
        vector<unsigned int> weights;
        preasp::DependencyGraph negDG;
        vector<preasp::Literal> KfromConstraints;
        
        inline unsigned int addAux(preasp::Builder& builder) const {
            unsigned int aux = builder.nextFreeVar();
            builder.addVariable(aux);
            return aux;
        }
        
        inline unsigned int createGapAtom(preasp::Builder& builder, int bl) {
            int ubl = bl > 0 ? bl : -bl;
            if(atomsToK.find(ubl) == atomsToK.end()) {
                unsigned int gapbl = addAux(builder);
                atomsToK[ubl] = gapbl;
                finalChoiceRule.push_back(preasp::Literal(gapbl, false));
                if(DLV2::Options::predMinimizationAlgorithm == PREDMIN_WEAK) {
                    weakConstraints.push_back(preasp::Literal(gapbl, false));
                    weights.push_back(1);
                }
                builder.setVarName(gapbl, builder.getProgram().isHidden(ubl) ? "#gap("+to_string(ubl)+")" : "#gap("+builder.getProgram().getName(ubl)+")");
            }
            return atomsToK[ubl];
        }
                
        inline void createAndAddGap(preasp::Builder& builder, preasp::Rule* rule, int bl) {
            if(bl >= 0) return;
            unsigned int gap = createGapAtom(builder, bl);
            rule->addBodyLiteral(preasp::Literal(gap, true));            
        }                

        inline unsigned int createGapForConstraint(preasp::Builder& builder) {
            unsigned int aux = addAux(builder);
            KfromConstraints.push_back(preasp::Literal(aux,false));
            finalChoiceRule.push_back(preasp::Literal(aux, false));
            if(DLV2::Options::predMinimizationAlgorithm == PREDMIN_WEAK) {
                weakConstraints.push_back(preasp::Literal(aux, false));
                weights.push_back(1);
            }
            builder.setVarName(aux, "#gap(__constr"+to_string(aux)+")");
            return aux;
        }
        
        inline void processUnaryBinaryConstraint(preasp::Builder& builder, preasp::Literal l, preasp::BinaryConstraint* b, bool semiequilibrium) {
            vector<preasp::Literal> lits;
            Constraint* c = new Constraint(lits);
            b == NULL ? c->fromUnaryConstraint(l) : c->fromBinaryConstraint(*b);
            unsigned int aux = createGapForConstraint(builder);
            if(semiequilibrium) processConstraintSemiequilibrium(builder, c, aux);
            unsigned int size = c->bodySize();
            for(unsigned int i = 0; i < size; i++) createAndAddGap(builder, c, c->getBodyLiteral(i).toInt());
            c->addBodyLiteral(preasp::Literal(aux, true));
            builder.getProgram().rules.push_back(c);
        }

};

#endif	/* PARACOHERENTSEMANTICS_H */

