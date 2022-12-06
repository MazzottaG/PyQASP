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
 * NumericOutputBuilder.cpp
 *
 *  Created on: Jul 7, 2015
 *      Author: davide
 */

#include "NumericOutputBuilder.h"
#include "../table/PredicateExtension.h"

namespace DLV2 {
namespace grounder {

void NumericOutputBuilder::onRule(Rule* rule) {
    //	cout<<"RULE ";
    //	rule->print();
    if (rule->isAStrongConstraint()) {
        onConstraint(rule);
    } else if (rule->isWeakConstraint()) {
        int level = rule->getLevel()->getConstantValue();
        unsigned idHead = rewriteBodyInAux(rule);
        if (levelWeak.count(level)) {
            (*levelWeak[level]).second.push_back(make_tuple(idHead, rule->getWeight()->getConstantValue(), rule->getLabel()));
        } else {
            list<id_weight_label> list(1, make_tuple(idHead, rule->getWeight()->getConstantValue(), rule->getLabel()));
            pair_level_tuple_list pair = {level, list};
            weakLevelConstraints.push_back(pair);
            levelWeak[level] = prev(weakLevelConstraints.end());
        }
    } else if (Options::globalOptions()->isCompactFacts() && rule->getSizeHead() == 1 && rule->getAtomInHead(0)->getIndex() == 0) {
        onFact(rule->getAtomInHead(0));
	} else if (
			StatementDependency::getInstance()->isStratifiedAndNotDisjuctive() && !rule->isChoiceRule() 
			&& rule->getSizeHead() == 1 && !rule->areThereUndefinedAtomInBody()) {
        onFact(rule->getAtomInHead(0));
    }
	else {
        bool toRewrite = false;
        if (rule->isChoiceRule() && Options::globalOptions()->getRewritingType() == COMPACT_NATIVE_CHOICE) {
            Atom* choiceAtom = rule->getAtomInHead(0);
            for (unsigned i = 0; i < choiceAtom->getChoiceElementsSize(); ++i) {
                if (choiceAtom->getChoiceElement(i)->getSize() > 1) {
                    toRewrite = true;
                    break;
                }
            }
        }
        if (toRewrite) {
            Atom* choiceAtom = rule->getAtomInHead(0);
            for (unsigned i = 0; i < choiceAtom->getChoiceElementsSize(); ++i) {
                ChoiceElement* ce = choiceAtom->getChoiceElement(i);
                onHeadChoiceRewrite();
                onClassicalLiteral(ce->getFirstAtom());
                if (ce->getSize() == 2)
                    onBodyChoiceRuleNativelyRewritten(rule, ce->getAtom(1));
                else
                    onBody(rule);
                printf("\n");
            }
        } else {
            onHead(rule->getHead());
            onBody(rule);
            printf("\n");
        }
    }
    if (printStream) {
        printf("%s", stream.str().c_str());
        stream.str("");
        printStream = false;
    }
}

void NumericOutputBuilder::onHeadAtom(Atom* atom) {
    printf("1 ");
    onClassicalLiteral(atom);
}

void NumericOutputBuilder::onHead(const vector<Atom*>& head) {
    if (head.size() > 0 && head[0]->isChoice())
        onChoiceAtom(head[0]);
    else if (head.size() > 1)
        onDisjunctionAtom(head);
    else if (head.size() > 0)
        onHeadAtom(head[0]);
}

void NumericOutputBuilder::onConstraint(Rule* rule) {
    printf("1 1 ");
    onBody(rule);
    printf("\n");
}

void NumericOutputBuilder::onHeadChoiceRewrite() {
    printf("3 1 ");
}

void NumericOutputBuilder::onBodyChoiceRuleNativelyRewritten(Rule *rule, Atom* atom) {
    unsigned body_size = rule->getSizeBody();
    vector<Atom*> negative, positive;
    negative.reserve(body_size);
    positive.reserve(body_size);
    for (unsigned i = 0; i < body_size; i++) {
        if (rule->isAtomToSimplifyInBody(i))continue;
        atom = rule->getAtomInBody(i);
        if (!atom->isNegative())
            positive.push_back(atom);
        else
            negative.push_back(atom);
    }

    unsigned negativeAtoms = negative.size();
    unsigned positiveAtoms = positive.size();

    if (atom->isNegative())
        negativeAtoms++;
    else
        positiveAtoms++;
    printf("%lu %lu ", ((unsigned long) (negativeAtoms + positiveAtoms)), ((unsigned long) negativeAtoms));
    onClassicalLiteral(atom);

    for (auto& atom : negative)
        if (atom->isAggregateAtom()) {
            unsigned agg_pred = onAggregate(atom);
            printf("%d ", agg_pred);
        } else
            onClassicalLiteral(atom);
    for (auto& atom : positive)
        if (atom->isAggregateAtom()) {
            unsigned agg_pred = onAggregate(atom);
            printf("%d ", agg_pred);
        } else
            onClassicalLiteral(atom);
}

void NumericOutputBuilder::onBody(Rule *rule) {
    unsigned body_size = rule->getSizeBody();
    vector<Atom*> negative, positive;
    negative.reserve(body_size);
    positive.reserve(body_size);
    Atom *atom;
    for (unsigned i = 0; i < body_size; i++) {
        if (rule->isAtomToSimplifyInBody(i))continue;
        atom = rule->getAtomInBody(i);
        if (!atom->isNegative())
            positive.push_back(atom);
        else
            negative.push_back(atom);
    }

    printf("%lu %lu ", ((unsigned long) (negative.size() + positive.size())), ((unsigned long) negative.size()));
    for (auto& atom : negative)
        if (atom->isAggregateAtom()) {
            unsigned agg_pred = onAggregate(atom);
            printf("%d ", agg_pred);
        } else
            onClassicalLiteral(atom);
    for (auto& atom : positive)
        if (atom->isAggregateAtom()) {
            unsigned agg_pred = onAggregate(atom);
            printf("%d ", agg_pred);
        } else
            onClassicalLiteral(atom);
}

void NumericOutputBuilder::onClassicalLiteral(Atom* atom) {
    printf("%d ", atom->getIndex());
}

void NumericOutputBuilder::onChoiceAtom(Atom* atom) {
    printf("3 %d ", atom->getChoiceElementsSize());
    for (unsigned i = 0; i < atom->getChoiceElementsSize(); i++)
        onClassicalLiteral(atom->getChoiceElement(i)->getFirstAtom());
}

unsigned NumericOutputBuilder::onAggregate(Atom* atom) {
    printStream = true;
    AggregateFunction function = atom->getAggregateFunction();
    if (function == COUNT || function == SUM) {
        return countSumAggregate(atom);
    } else if (function == MIN || function == MAX) {
        return maxMinAggregate(atom);
    }
    return 0;
}

unsigned NumericOutputBuilder::countSumAggregate(Atom* atom) {
    int pred_first_binop = 0, pred_second_binop = 0;
    //TODO we can optimize scan the aggregate one time also if there is two guard, saving the body of the rule in string
    if (atom->getFirstBinop() != NONE_OP) {
        int bound = atom->getFirstGuard()->getConstantValue();
        if (atom->getFirstBinop() == LESS)bound++;
        if (atom->getAggregateFunction() == COUNT)
            pred_first_binop = onConstraintRule(atom, bound);
        else
            pred_first_binop = onWeightRule(atom, bound);
    }
    if (atom->getSecondBinop() != NONE_OP || atom->getFirstBinop() == EQUAL) {
        int bound;
        if (atom->getFirstBinop() == EQUAL)
            bound = atom->getFirstGuard()->getConstantValue() + 1;
        else
            bound = atom->getSecondGuard()->getConstantValue();
        if (atom->getSecondBinop() == LESS_OR_EQ)bound++;
        if (atom->getAggregateFunction() == COUNT)
            pred_second_binop = onConstraintRule(atom, bound);
        else
            pred_second_binop = onWeightRule(atom, bound);
    }
    unsigned index_aggregate = IdGenerator::getInstance()->getNewId(1);
    printCountSumAggregate(index_aggregate, pred_first_binop, pred_second_binop);
    return index_aggregate;
}

void NumericOutputBuilder::printCountSumAggregate(unsigned index_aggregate, int pred_first_binop, int pred_second_binop) {
    stream << "1 " << index_aggregate << " " << (pred_first_binop != 0 && pred_second_binop != 0) + 1 << " " << (pred_second_binop != 0) << " ";
    if (pred_second_binop != 0)stream << pred_second_binop << " ";
    if (pred_first_binop != 0)stream << pred_first_binop;
    stream << endl;
}

void NumericOutputBuilder::onDisjunctionAtom(const vector<Atom*>& head) {
    printf("8 %lu ", head.size());
    for (auto& atom : head)
        onClassicalLiteral(atom);
}

void NumericOutputBuilder::onAggregateElement(Atom* atom) {
}

void NumericOutputBuilder::handleCompactFactsPrinting(Atom* atom) {
    stringstream tmp;
    ClassicalLiteral::print(atom->getPredicate(), atom->getTerms(), false,
            false, tmp);
    if (streamCompactFactsNumericTableTmp.str().size() + tmp.str().size() > SIZE_COMPACT_FACTS) {
        streamCompactFacts << "1 " << idCompactFacts << " 0 0" << endl;
        idCompactFacts = IdGenerator::getInstance()->getNewId(1);
        streamCompactFactsNumericTable << streamCompactFactsNumericTableTmp.str() << endl;
        streamCompactFactsNumericTableTmp.str("");
        streamCompactFactsNumericTableTmp << idCompactFacts;
    }
}

void NumericOutputBuilder::onFact(Atom* atom) {
    if (!Options::globalOptions()->isCompactFacts()) {
        if(printStreamFacts)
        	streamFacts << 1 << " " << atom->getIndex() << " 0 0\n";
        else
        {
        	if(atomToFilter == nullptr || (atomToFilter != nullptr && *atom == *atomToFilter)){
        		onHeadAtom(atom);
        		printf("0 0\n");
				return;
        	}
        }
    }
    handleCompactFactsPrinting(atom);
}

unsigned NumericOutputBuilder::onWeightRule(Atom* aggregateAtom, int bound) {
    unsigned pred_id = IdGenerator::getInstance()->getNewId(1);
    unsigned body_size = aggregateAtom->getAggregateElementsSize();
    vector<Atom*> negative, positive;
    vector<int> weight_negative, weight_positive;
    negative.reserve(body_size);
    positive.reserve(body_size);
    weight_negative.reserve(body_size);
    weight_positive.reserve(body_size);
    Atom *atom;
    for (unsigned i = 0; i < aggregateAtom->getAggregateElementsSize(); i++) {
        AggregateElement *element = aggregateAtom->getAggregateElement(i);
        atom = element->getNafLiteral(0);
        int weight = element->getTerm(0)->getConstantValue();
        if(weight == 0) continue;
        if (!atom->isNegative() && weight > 0) {
            positive.push_back(atom);
            weight_positive.push_back(weight);
        } else {
            negative.push_back(atom);
            unsigned absweight = abs(weight);
            weight_negative.push_back(absweight);
            if (weight < 0)bound += absweight;
        }
    }

    stream << "5 " << pred_id << " " << bound << " " << negative.size() + positive.size() << " " << negative.size() << " ";
    for (auto& atom : negative)
        stream << atom->getIndex() << " ";
    for (auto& atom : positive)
        stream << atom->getIndex() << " ";


    for (auto& weight : weight_negative)
        stream << weight << " ";
    for (auto& weight : weight_positive)
        stream << weight << " ";

    stream << endl;
    return pred_id;
}

unsigned NumericOutputBuilder::onConstraintRule(Atom* aggregateAtom, unsigned bound) {
    unsigned pred_id = IdGenerator::getInstance()->getNewId(1);
    stream << "2 " << pred_id << " ";
    unsigned body_size = aggregateAtom->getAggregateElementsSize();
    vector<Atom*> negative, positive;
    negative.reserve(body_size);
    positive.reserve(body_size);
    Atom *atom;
    for (unsigned i = 0; i < aggregateAtom->getAggregateElementsSize(); i++) {
        AggregateElement *element = aggregateAtom->getAggregateElement(i);
        atom = element->getNafLiteral(0);
        if (!atom->isNegative())
            positive.push_back(atom);
        else
            negative.push_back(atom);

    }

    stream << negative.size() + positive.size() << " " << negative.size() << " " << bound << " ";
    for (auto& atom : negative)
        stream << atom->getIndex() << " ";
    for (auto& atom : positive) {
        stream << atom->getIndex() << " ";
    }
    stream << endl;
    return pred_id;
}

void NumericOutputBuilder::auxRuleMinMax(Atom *aggregate, function<unsigned(Term*)>& function, unsigned & positivePredicate, unsigned & negativePredicate) {
    IdGenerator* generator = IdGenerator::getInstance();
    for (unsigned i = 0; i < aggregate->getAggregateElementsSize(); i++) {

        unsigned index_agg_atom = aggregate->getAggregateElement(i)->getNafLiteral(0)->getIndex();
        Term *agg_term = aggregate->getAggregateElement(i)->getTerm(0);
        unsigned value = function(agg_term);
        switch (value) {
            case 1:
                if (positivePredicate == 0)positivePredicate = generator->getNewId(1);
                printAuxRuleMinMax(positivePredicate, index_agg_atom);
                break;
            case 2:
                if (negativePredicate == 0)negativePredicate = generator->getNewId(1);
                printAuxRuleMinMax(negativePredicate, index_agg_atom);
                break;
            default:
                break;
        }
    }
}

void NumericOutputBuilder::printAuxRuleMinMax(unsigned predicate, unsigned index_agg_atom) {
    stream << "1 " << predicate << " 1 0 " << index_agg_atom << endl;
}

unsigned NumericOutputBuilder::maxMinAggregate(Atom* atom) {
    function<unsigned(Term*) > function;

    if (atom->getAggregateFunction() == MAX) {

        if (atom->getFirstBinop() != NONE_OP && atom->getFirstBinop() != EQUAL && atom->getSecondBinop() == NONE_OP) {
            function = [&](Term * term) {
                if (atom->getFirstBinop() == LESS && *term>*(atom->getFirstGuard()))return 1;
                if (atom->getFirstBinop() == LESS_OR_EQ && *term >= *(atom->getFirstGuard()))return 1;
                return 0;
            };
        } else if (atom->getSecondBinop() != NONE_OP && atom->getFirstBinop() == NONE_OP) {
            function = [&](Term * term) {
                if (atom->getSecondBinop() == LESS && *term >= *(atom->getSecondGuard()))return 2;
                if (atom->getSecondBinop() == LESS_OR_EQ && *term>*(atom->getSecondGuard()))return 2;
                return 0;
            };
        } else if (atom->getSecondBinop() != NONE_OP && atom->getFirstBinop() != NONE_OP) {
            function = [&](Term * term) {
                if (atom->getFirstBinop() == LESS && atom->getSecondBinop() == LESS && *term>*(atom->getFirstGuard()) && *term<*(atom->getSecondGuard()))return 1;
                if (atom->getFirstBinop() == LESS && atom->getSecondBinop() == LESS_OR_EQ && *term>*(atom->getFirstGuard()) && *term <= *(atom->getSecondGuard()))return 1;
                if (atom->getFirstBinop() == LESS_OR_EQ && atom->getSecondBinop() == LESS && *term >= *(atom->getFirstGuard()) && *term<*(atom->getSecondGuard()))return 1;
                if (atom->getFirstBinop() == LESS_OR_EQ && atom->getSecondBinop() == LESS_OR_EQ && *term >= *(atom->getFirstGuard()) && *term <= *(atom->getSecondGuard()))return 1;

                if (atom->getSecondBinop() == LESS && *term >= *(atom->getSecondGuard()))return 2;
                if (atom->getSecondBinop() == LESS_OR_EQ && *term>*(atom->getSecondGuard()))return 2;

                return 0;
            };
        } else if (atom->getFirstBinop() == EQUAL) {
            function = [&](Term * term) {
                if (*term == *(atom->getFirstGuard()))return 1;
                if (*term>*(atom->getFirstGuard()))return 2;
                return 0;
            };
        }

    } else if (atom->getAggregateFunction() == MIN) {


        if (atom->getFirstBinop() != NONE_OP && atom->getFirstBinop() != EQUAL && atom->getSecondBinop() == NONE_OP) {
            function = [&](Term * term) {
                if (atom->getFirstBinop() == LESS && *term <= *(atom->getFirstGuard()))return 2;
                if (atom->getFirstBinop() == LESS_OR_EQ && *term<*(atom->getFirstGuard()))return 2;
                return 0;
            };
        } else if (atom->getSecondBinop() != NONE_OP && atom->getFirstBinop() == NONE_OP) {
            function = [&](Term * term) {
                if (atom->getSecondBinop() == LESS && *term<*(atom->getSecondGuard()))return 1;
                if (atom->getSecondBinop() == LESS_OR_EQ && *term <= *(atom->getSecondGuard()))return 1;
                return 0;
            };
        } else if (atom->getSecondBinop() != NONE_OP && atom->getFirstBinop() != NONE_OP) {
            function = [&](Term * term) {
                if (atom->getFirstBinop() == LESS && atom->getSecondBinop() == LESS && *term>*(atom->getFirstGuard()) && *term<*(atom->getSecondGuard()))return 1;
                if (atom->getFirstBinop() == LESS && atom->getSecondBinop() == LESS_OR_EQ && *term>*(atom->getFirstGuard()) && *term <= *(atom->getSecondGuard()))return 1;
                if (atom->getFirstBinop() == LESS_OR_EQ && atom->getSecondBinop() == LESS && *term >= *(atom->getFirstGuard()) && *term<*(atom->getSecondGuard()))return 1;
                if (atom->getFirstBinop() == LESS_OR_EQ && atom->getSecondBinop() == LESS_OR_EQ && *term >= *(atom->getFirstGuard()) && *term <= *(atom->getSecondGuard()))return 1;

                if (atom->getFirstBinop() == LESS && *term <= *(atom->getFirstGuard()))return 2;
                if (atom->getFirstBinop() == LESS_OR_EQ && *term<*(atom->getFirstGuard()))return 2;

                return 0;
            };
        } else if (atom->getFirstBinop() == EQUAL) {
            function = [&](Term * term) {
                if (*term == *(atom->getFirstGuard()))return 1;
                if (*term<*(atom->getFirstGuard()))return 2;
                return 0;
            };
        }

    }

    unsigned positivePredicate = 0, negativePredicate = 0;
    auxRuleMinMax(atom, function, positivePredicate, negativePredicate);
    unsigned index_aggregate = IdGenerator::getInstance()->getNewId(1);
    bool boolean = atom->getSecondBinop() != NONE_OP && atom->getFirstBinop() != NONE_OP && positivePredicate == 0;
    if(atom->getSecondBinop() == NONE_OP && atom->getFirstBinop() == EQUAL && positivePredicate == 0){
    	if(atom->getAggregateFunction()==MIN && atom->getFirstGuard()==TermTable::getInstance()->term_max)
    		boolean=false;
    	else if(atom->getAggregateFunction()==MAX && atom->getFirstGuard()==TermTable::getInstance()->term_min)
    		boolean=false;
    	else
    		boolean=true;
    }
    printMaxMinAggregate(index_aggregate, positivePredicate, negativePredicate, boolean);
    return index_aggregate;
}

void NumericOutputBuilder::printMaxMinAggregate(unsigned index_aggregate, unsigned positivePredicate, unsigned negativePredicate, bool boolean) {
    stream << "1 " << index_aggregate << " " << (positivePredicate != 0 && negativePredicate != 0) + 1 + boolean << " " << (negativePredicate != 0) << " ";
    if (negativePredicate != 0)stream << negativePredicate << " ";
    if (positivePredicate != 0)stream << positivePredicate;
    else if (boolean) stream << IdGenerator::getInstance()->getNewId(1);
    stream << endl;
}

void NumericOutputBuilder::printWeak() {
    weakLevelConstraints.sort([](const pair_level_tuple_list& l1, const pair_level_tuple_list & l2) {
        return l1.first < l2.first;
    });
    for (auto list : weakLevelConstraints) {
        weakAtLevel(list.first, list.second);
    }
}

void NumericOutputBuilder::weakAtLevel(int level, list<id_weight_label>& listOfWeak) {
    auto hash = [](const pair<int, vector < Term*>>&ele) {
        size_t seed = 3;
        seed ^= (ele.first + 0x9e3779b9 + (seed << 6) + (seed >> 2));
        for (auto t : ele.second)
            seed ^= t->getIndex() + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    };
    auto equal = [](const pair<int, vector < Term*>>&ele1, const pair<int, vector < Term*>>&ele2) {
        if (ele1.first != ele2.first)return false;
        if (ele1.second.size() != ele2.second.size())return false;
        for (unsigned i = 0; i < ele1.second.size(); i++)
            if (ele1.second[i]->getIndex() != ele2.second[i]->getIndex())
                return false;
        return true;
    };

    unordered_map < pair<int, vector < Term*>>, unsigned, decltype(hash), decltype(equal) > weightLabel(1, hash, equal);
    vector<vector < id_weight_label>> weaks;
    for (auto idWeightLabel : listOfWeak) {
        pair<int, vector < Term*>> pair = {get<1>(idWeightLabel), get<2>(idWeightLabel)};
        auto it = weightLabel.find(pair);
        if (it != weightLabel.end())
            weaks[it->second].push_back(idWeightLabel);
        else {
            weightLabel.insert({pair, weaks.size()});
            weaks.emplace_back(1, idWeightLabel);
        }
    }

    //	cout<<"LEVEL "<<listOfWeak.front()->getLevelInt()<<endl;
    //	for(auto& p:weaks){
    //		cout<<"BLOCK"<<endl;
    //		for(auto& w:p){
    //			w->print();
    //		}
    //		cout<<endl;
    //	}

    printWeakAtLevel(level, weaks);
}

void NumericOutputBuilder::printWeakAtLevel(int level, vector<vector < id_weight_label>> weaks) {
    stringstream atomsIdPositive;
	stringstream atomsIdNegative;
    stringstream weightAtomsIdNegative;
    stringstream weightAtomsIdPositive;
	unsigned positive=0,negative=0;
    for (unsigned i = 0; i < weaks.size(); i++) {
		if(get<1>(weaks[i][0])>=0){
			atomsIdPositive << " ";
			++positive;
			weightAtomsIdPositive << " " << get<1>(weaks[i][0]);
			if (weaks[i].size() == 1)
	            atomsIdPositive << get<0>(weaks[i][0]);
	        else {
	            vector<unsigned> aux;
	            for (auto weak : weaks[i])
	                aux.push_back(get<0>(weak));
	            atomsIdPositive << createMultipleRule(aux);
	        }
		}
		else{
			atomsIdNegative << " ";
			++negative;
			weightAtomsIdNegative << " " << abs(get<1>(weaks[i][0]));
			if (weaks[i].size() == 1)
	            atomsIdNegative << get<0>(weaks[i][0]);
	        else {
	            vector<unsigned> aux;
	            for (auto weak : weaks[i])
	                aux.push_back(get<0>(weak));
				atomsIdNegative << createMultipleRule(aux);
			}
		}
    }
    printf("6 0 %lu %u%s%s%s%s\n", ((unsigned long) weaks.size()), negative, atomsIdNegative.str().c_str(), atomsIdPositive.str().c_str(), weightAtomsIdNegative.str().c_str(), weightAtomsIdPositive.str().c_str());
}

unsigned NumericOutputBuilder::rewriteBodyInAux(Rule* rule) {
    unsigned body_size = rule->getSizeBody();
    vector<Atom*> negative, positive;
    negative.reserve(body_size);
    positive.reserve(body_size);
    Atom* firstPrinted = nullptr;
    Atom *atom;
    for (unsigned i = 0; i < body_size; i++) {
        if (rule->isAtomToSimplifyInBody(i))continue;
        atom = rule->getAtomInBody(i);
        if (!atom->isNegative())
            positive.push_back(atom);
        else
            negative.push_back(atom);
        if (firstPrinted == nullptr)firstPrinted = atom;
    }
    if (positive.size() == 1 && negative.size() == 0) {
        if (firstPrinted->isAggregateAtom())
            return onAggregate(firstPrinted);
        else {
            return firstPrinted->getIndex();
        }
    }

    unsigned index_head = IdGenerator::getInstance()->getNewId(1);
    printf("1 %d %lu %lu ", index_head, ((unsigned long) (negative.size() + positive.size())), ((unsigned long) negative.size()));
    for (auto& atom : negative)
        if (atom->isAggregateAtom()) {
            unsigned agg_pred = onAggregate(atom);
            printf("%d ", agg_pred);
        } else
            onClassicalLiteral(atom);
    for (auto& atom : positive)
        if (atom->isAggregateAtom()) {
            unsigned agg_pred = onAggregate(atom);
            printf("%d ", agg_pred);
        } else
            onClassicalLiteral(atom);
    printf("\n");
    return index_head;
}

unsigned NumericOutputBuilder::createMultipleRule(vector<unsigned>& idatoms) {
    unsigned index_head = IdGenerator::getInstance()->getNewId(1);
    for (auto id : idatoms)
        printf("1 %d 1 0 %d\n", index_head, id);
    return index_head;
}

void NumericOutputBuilder::onEnd() {
    if (weakLevelConstraints.size() > 0)
        printWeak();

    if (Options::globalOptions()->isCompactFacts() && streamCompactFactsNumericTableTmp.str().size() > to_string(idCompactFacts).size())
        printf("%s 1 %d 0 0\n", streamCompactFacts.str().c_str(), idCompactFacts);

    //Print atom filtered
    if (atomToFilter != nullptr) {
        Predicate *predicate = atomToFilter->getPredicate();
        auto predExt = PredicateExtTable::getInstance()->getPredicateExt(predicate);
        Atom* atomInTable = nullptr;
        if (predExt->getPredicateExtentionSize(FACT) > 0) {
            IndexingStructure* as = predExt->addAtomSearcher(FACT, MAP, nullptr);
            if (as == nullptr) {
                vector<unsigned> v;
                v.push_back(0);
                as = predExt->addAtomSearcher(FACT, MAP, &v);
            }
            atomInTable = as->find(atomToFilter);
        }
        if (atomInTable == nullptr && predExt->getPredicateExtentionSize(NOFACT) > 0)
            atomInTable = PredicateExtTable::getInstance()->getPredicateExt(predicate)->addAtomSearcher(NOFACT, MAP, nullptr)->find(atomToFilter);

        if (atomInTable != nullptr) {
            printAtomToFilterInAtomTable(atomInTable);
			if(predicate->isEdb() && atomInTable->isFact())
				onFact(atomInTable);
        }
    }
	printf("%s", streamFacts.str().c_str());
}

void NumericOutputBuilder::printAtomToFilterInAtomTable(Atom* atomInTable) {
    streamAtomTable << atomInTable->getIndex() << " ";
    ClassicalLiteral::print(atomInTable->getPredicate(), atomInTable->getTerms(), false, false, streamAtomTable);
    streamAtomTable << endl;
}

void NumericOutputBuilder::printAtomTable() {
    printf("0\n%s", streamAtomTable.str().c_str());
}

void NumericOutputBuilder::printComputeStatement() {
    printf("0\nB+\n0\nB-\n1\n0\n1\n");
}

void NumericOutputBuilder::appendToStreamAtomTable(Atom* atom, bool fact) {
    if (Options::globalOptions()->isCompactFacts() && fact) {
        stringstream tmp;
        ClassicalLiteral::print(atom->getPredicate(), atom->getTerms(), false, false, tmp);
        streamCompactFactsNumericTableTmp << " " << tmp.str() << ".";
        return;
    }
    if (!atom->getPredicate()->isHiddenForPrinting() && atomToFilter == nullptr) {
        streamAtomTable << atom->getIndex() << " ";
        ClassicalLiteral::print(atom->getPredicate(), atom->getTerms(), false, false, streamAtomTable);
        streamAtomTable << endl;
    }

}


} /* namespace grounder */
} /* namespace DLV2 */

