/*******************************************************************************
 * Copyright 2016 Francesco Calimeri, Davide Fusca', Simona Perri and Jessica Zangari
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/
/* 
 * File:   IntermediateFormatOutputBuilder.h
 * Author: Nicola Greco
 *
 * Created on September 26, 2017
 */

#ifndef SRC_GROUNDER_OUTPUT_INTERMEDIATEFORMATOUTPUTBUILDER_H
#define SRC_GROUNDER_OUTPUT_INTERMEDIATEFORMATOUTPUTBUILDER_H

#include <sstream>
#include "NumericOutputBuilder.h"
#include "../ground/StatementDependency.h"
#include "../table/IdGenerator.h"

namespace DLV2 {
namespace grounder {

class IntermediateFormatOutputBuilder : public NumericOutputBuilder {
public:
    virtual void onDisjunctionAtom(const vector<Atom*>& head) override;
    virtual void onConstraint(Rule* rule) override;
    virtual void onHead(const vector<Atom*>& head) override;
    virtual void onBody(Rule *rule) override;
    virtual void onClassicalLiteral(Atom *atom) override;
    virtual void onChoiceAtom(Atom *atom) override;
    virtual void onFact(Atom* atom) override;
    virtual void onBegin() override;
    virtual void printCountSumAggregate(unsigned index_aggregate, int pred_first_binop, int pred_second_binop) override;
    virtual unsigned onWeightRule(Atom *aggregateAtom,int bound) override;
    virtual unsigned onConstraintRule(Atom *aggregateAtom,unsigned bound);
    virtual void printMaxMinAggregate(unsigned index_aggregate, unsigned positivePredicate, unsigned negativePredicate, bool boolean) override;
    virtual void printAuxRuleMinMax(unsigned predicate, unsigned index_agg_atom) override;
    virtual void onHeadChoiceRewrite() override;
    virtual void onBodyChoiceRuleNativelyRewritten(Rule* rule, Atom* atom) override;
    virtual void appendToStreamAtomTable(Atom* atom, bool fact=false) override;
    virtual void printAtomToFilterInAtomTable(Atom* atomInTable) override;
    virtual void printAtomTable() override;
    virtual void printComputeStatement() override;

    void printWeakAtLevel(int level, vector<vector<id_weight_label> > weaks) override;
    
    ///Write the body of the rule in a new rule with in the head new aux atom and return the id of the new aux
    virtual unsigned rewriteBodyInAux(Rule* rule) override;
    
    ///Create for each id in idatoms a rule with in the head a new aux atom and in the body an element in the vector. Return the id of the new aux atom in the head.
    virtual unsigned createMultipleRule(vector<unsigned>& idatoms) override;
    
};

}
}

#endif /* SRC_GROUNDER_OUTPUT_INTERMEDIATEFORMATOUTPUTBUILDER_H */

