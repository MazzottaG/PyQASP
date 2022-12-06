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
 * File:   IntermediateFormatOutputBuilder.cpp
 * Author: Nicola Greco
 *
 * Created on September 26, 2017
 */

#include "IntermediateFormatOutputBuilder.h"
#include "../table/PredicateExtension.h"

namespace DLV2 {
namespace grounder {

void IntermediateFormatOutputBuilder::onConstraint(Rule * rule) {
    printf("1 0 0 ");
    onBody(rule);
    printf("\n");
}

void IntermediateFormatOutputBuilder::onHead(const vector<Atom*>& head) {
    printf("1 ");
    if (head.size() > 0 && head[0]->isChoice())
        onChoiceAtom(head[0]);
    else
        onDisjunctionAtom(head);
}

void IntermediateFormatOutputBuilder::onChoiceAtom(Atom * atom) {
    printf("1 %d ", atom->getChoiceElementsSize());
    for (unsigned i = 0; i < atom->getChoiceElementsSize(); i++)
        onClassicalLiteral(atom->getChoiceElement(i)->getFirstAtom());
}

void IntermediateFormatOutputBuilder::onDisjunctionAtom(const vector<Atom*>& head) {
    printf("0 %lu ", ((unsigned long)(head.size())));
    for (auto& atom : head)
        onClassicalLiteral(atom);
}

void IntermediateFormatOutputBuilder::onBody(Rule * rule) {
    vector<Atom*> atoms;
    unsigned body_size = rule->getSizeBody();
    for (unsigned i = 0; i < body_size; ++i) {
        if (!rule->isAtomToSimplifyInBody(i)) {
            atoms.push_back(rule->getAtomInBody(i));
        }
    }
    unsigned atoms_size = atoms.size();
    printf("0 %u ", atoms_size);
    for (auto& atom : atoms) {
        if (atom->isAggregateAtom()) {
            int agg_pred = onAggregate(atom);
            if (atom->isNegative())
                printf("%d ", -agg_pred);
            else
                printf("%d ", agg_pred);
        } else {
            onClassicalLiteral(atom);
        }
    }
}

void IntermediateFormatOutputBuilder::printCountSumAggregate(unsigned index_aggregate, int pred_first_binop, int pred_second_binop) {
    stream << "1 0 1 " << index_aggregate << " 0 " << (pred_first_binop != 0 && pred_second_binop != 0) + 1 << " ";
    if (pred_second_binop != 0)stream << -pred_second_binop << " ";
    if (pred_first_binop != 0)stream << pred_first_binop;
    stream << endl;
}

unsigned IntermediateFormatOutputBuilder::onConstraintRule(Atom* aggregateAtom, unsigned bound) {
    unsigned pred_id = IdGenerator::getInstance()->getNewId(1);
    unsigned body_size = aggregateAtom->getAggregateElementsSize();
    stream << "1 0 1 " << pred_id << " 1 " << bound << " " << body_size << " ";
    Atom *atom;
    for (unsigned i = 0; i < body_size; i++) {
        AggregateElement *element = aggregateAtom->getAggregateElement(i);
        atom = element->getNafLiteral(0);
        if (!atom->isNegative())
            stream << atom->getIndex() << " " << 1 << " ";
        else
            stream << -atom->getIndex() << " " << 1 << " ";

    }
    stream << endl;
    return pred_id;
}

void IntermediateFormatOutputBuilder::printMaxMinAggregate(unsigned index_aggregate, unsigned positivePredicate, unsigned negativePredicate, bool boolean) {
    stream << "1 0 1 " << index_aggregate << " 0 " << (positivePredicate != 0 && negativePredicate != 0) + 1 + boolean << " ";
    if (negativePredicate != 0) {
        int np = -negativePredicate;
        stream << np << " ";
    }
    if (positivePredicate != 0)
        stream << positivePredicate;
    else if (boolean)
        stream << IdGenerator::getInstance()->getNewId(1);
    stream << endl;
}

void IntermediateFormatOutputBuilder::printAuxRuleMinMax(unsigned predicate, unsigned index_agg_atom) {
    stream << "1 0 1 " << predicate << " 0 1 " << index_agg_atom << endl;
}

unsigned IntermediateFormatOutputBuilder::onWeightRule(Atom* aggregateAtom, int bound) {
    unsigned pred_id = IdGenerator::getInstance()->getNewId(1);
    unsigned body_size = aggregateAtom->getAggregateElementsSize();
    stringstream tmp;
    Atom *atom;
    for (unsigned i = 0; i < aggregateAtom->getAggregateElementsSize(); i++) {
        AggregateElement *element = aggregateAtom->getAggregateElement(i);
        atom = element->getNafLiteral(0);
        int weight = element->getTerm(0)->getConstantValue();
        if(weight == 0) continue;
        if (!atom->isNegative() && weight > 0) {
            tmp << atom->getIndex() << " " << weight << " ";
        } else {
            unsigned absweight = abs(weight);
            int index = -atom->getIndex();
            tmp << index << " " << absweight << " ";
            if (weight < 0)bound += absweight;
        }
    }

    stream << "1 0 1 " << pred_id << " 1 " << bound << " " << body_size << " " << tmp.str() << endl;
    return pred_id;
}

void IntermediateFormatOutputBuilder::onClassicalLiteral(Atom * atom) {
    if (atom->isNegative())
        printf("%d ", -atom->getIndex());
    else
        printf("%d ", atom->getIndex());
}

void IntermediateFormatOutputBuilder::onFact(Atom * atom) {
    printf("1 0 1 %d 0 0\n", atom->getIndex());
}

void IntermediateFormatOutputBuilder::onBegin() {
    printf("asp 1 0 0\n");
}

void IntermediateFormatOutputBuilder::printAtomToFilterInAtomTable(Atom* atomInTable) {
    stringstream streamTmp;
    ClassicalLiteral::print(atomInTable->getPredicate(), atomInTable->getTerms(), false, false, streamTmp);
    streamTmp.seekg(0, ios::end);
    unsigned bytes = streamTmp.tellg();
    streamAtomTable << "4 " << bytes << " " << streamTmp.str() << " 1 " << atomInTable->getIndex() << endl;
}

void IntermediateFormatOutputBuilder::printAtomTable() {
    printf("%s", streamAtomTable.str().c_str());
}

void IntermediateFormatOutputBuilder::printComputeStatement() {
    printf("0\n");
}

void IntermediateFormatOutputBuilder::printWeakAtLevel(int level, vector<vector<id_weight_label> > weaks) {
    stringstream str;
    str << "2 " << level << " " << weaks.size() << " ";

    for (unsigned i = 0; i < weaks.size(); i++) {
        if (i != 0)
            str << " ";
        if (weaks[i].size() == 1)
            str << get<0>(weaks[i][0]) << " ";
        else {
            vector<unsigned> aux;
            for (auto weak : weaks[i])
                aux.push_back(get<0>(weak));
            str << createMultipleRule(aux) << " ";
        }
        str << get<1>(weaks[i][0]);
    }
    cout << str.str() << endl;
}

unsigned IntermediateFormatOutputBuilder::rewriteBodyInAux(Rule * rule) {
    unsigned body_size = rule->getSizeBody();
    vector<Atom*> atoms;
    atoms.reserve(body_size);
    Atom* firstPrinted = nullptr;
    Atom *atom;
    unsigned negative = 0;
    for (unsigned i = 0; i < body_size; i++) {
        if (rule->isAtomToSimplifyInBody(i))continue;
        atom = rule->getAtomInBody(i);
        atoms.push_back(atom);
        if (atom->isNegative())
            negative++;
        if (firstPrinted == nullptr)
            firstPrinted = atom;
    }
    if (atoms.size() == 1 && negative == 0) {
        if (firstPrinted->isAggregateAtom())
            return onAggregate(firstPrinted);
        else {
            return firstPrinted->getIndex();
        }
    }

    unsigned index_head = IdGenerator::getInstance()->getNewId(1);
    printf("1 0 1 %u 0 %lu ", index_head, ((unsigned long) (atoms.size())));

    for (auto& atom : atoms)
        if (atom->isAggregateAtom()) {
            unsigned agg_pred = onAggregate(atom);
            if (atom->isNegative())
                printf("%d ", -agg_pred);
            else
                printf("%d ", agg_pred);
        } else
            onClassicalLiteral(atom);
    printf("\n");
    return index_head;
}

unsigned IntermediateFormatOutputBuilder::createMultipleRule(vector<unsigned>& idatoms) {
    unsigned index_head = IdGenerator::getInstance()->getNewId(1);
    for (auto id : idatoms)
        printf("1 0 1 %d 0 1 %d\n", index_head, id);
    return index_head;
}

void IntermediateFormatOutputBuilder::appendToStreamAtomTable(Atom* atom, bool fact) {
    if (!atom->getPredicate()->isHiddenForPrinting() && atomToFilter == nullptr) {
        stringstream streamTmp;
        ClassicalLiteral::print(atom->getPredicate(), atom->getTerms(), false, false, streamTmp);
        streamTmp.seekg(0, ios::end);
        unsigned bytes = streamTmp.tellg();
        streamAtomTable << "4 " << bytes << " " << streamTmp.str() << " ";
        if (!fact) {
            streamAtomTable << "1 " << atom->getIndex() << endl;
        } else {
            streamAtomTable << "0" << endl;
        }
    }
}

void IntermediateFormatOutputBuilder::onHeadChoiceRewrite() {
    printf("1 1 1 ");
}

void IntermediateFormatOutputBuilder::onBodyChoiceRuleNativelyRewritten(Rule *rule, Atom * atom) {
    unsigned body_size = rule->getSizeBody();
    vector<Atom*> atoms;
    atoms.reserve(body_size);
    for (unsigned i = 0; i < body_size; i++) {
        if (rule->isAtomToSimplifyInBody(i))continue;
        atom = rule->getAtomInBody(i);
        atoms.push_back(atom);
    }

    printf("0 %lu ", ((unsigned long) (atoms.size() + 1)));
    onClassicalLiteral(atom);

    for (auto& atom : atoms)
        if (atom->isAggregateAtom()) {
            int agg_pred = onAggregate(atom);
            if (atom->isNegative())
                printf("%d ", -agg_pred);
            else
                printf("%d ", agg_pred);
        } else
            onClassicalLiteral(atom);
}

}
}

