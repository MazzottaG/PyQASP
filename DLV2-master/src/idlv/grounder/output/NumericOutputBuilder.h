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
 * NumericOutputBuilder.h
 *
 *  Created on: Jul 7, 2015
 *      Author: davide
 */

#ifndef SRC_GROUNDER_OUTPUT_NUMERICOUTPUTBUILDER_H_
#define SRC_GROUNDER_OUTPUT_NUMERICOUTPUTBUILDER_H_

#include <sstream>
#include "OutputBuilder.h"

namespace DLV2 {
namespace grounder {

class NumericOutputBuilder: public OutputBuilder {
public:

    NumericOutputBuilder():printStream(false),printStreamFacts(false),SIZE_COMPACT_FACTS(1024),atomToFilter(nullptr){
    	if(Options::globalOptions()->isCompactFacts()){
    		idCompactFacts=IdGenerator::getInstance()->getNewId(1);
    		streamCompactFactsNumericTable<<idCompactFacts;
    	}
    };
    virtual ~NumericOutputBuilder(){delete atomToFilter;};

    virtual void onRule(Rule *rule) override;
    virtual void onHeadAtom(Atom * atom) override;
    virtual void onDisjunctionAtom(const vector<Atom*>& head) override;
    virtual void onConstraint(Rule* rule) override;
    virtual void onHead(const vector<Atom*>& head) override;
    virtual void onBody(Rule *rule) override;
    virtual void onClassicalLiteral(Atom *atom) override;
    virtual void onChoiceAtom(Atom *atom) override;
    virtual unsigned onAggregate(Atom *atom) override;
    virtual void onAggregateElement(Atom *atom) override;
    virtual void onFact(Atom* atom) override;
    virtual void onEnd() override;
    virtual void onEndParsing() override { printStreamFacts=false; };
    virtual void onPrintAtomTable() override {
    	printAtomTable();
    	if (Options::globalOptions()->isCompactFacts() && streamCompactFactsNumericTableTmp.str().size() > to_string(idCompactFacts).size()) {
        	printf("%s%s", streamCompactFactsNumericTable.str().c_str(), streamCompactFactsNumericTableTmp.str().c_str());
    	}
    	printComputeStatement();
    }

    virtual void appendToStreamAtomTable(Atom* atom, bool fact=false) override;
    void clearStreamAtomTable(){streamAtomTable.str(""); streamFacts.str(""); }
    void setAtomToFilter(Atom* atomToFilter){this->atomToFilter=atomToFilter->clone();}

    virtual unsigned countSumAggregate(Atom *atom);
    virtual void printCountSumAggregate(unsigned index_aggregate, int pred_first_binop, int pred_second_binop);
    virtual unsigned onWeightRule(Atom *aggregateAtom,int bound);
    virtual unsigned onConstraintRule(Atom *aggregateAtom,unsigned bound);

    virtual unsigned maxMinAggregate(Atom *atom);
    virtual void printMaxMinAggregate(unsigned index_aggregate, unsigned positivePredicate, unsigned negativePredicate, bool boolean);
    virtual void auxRuleMinMax(Atom *aggregate,function<unsigned(Term*)>& function,unsigned& positivePredicate,unsigned& negativePredicate);
    virtual void printAuxRuleMinMax(unsigned predicate, unsigned index_agg_atom);
    virtual void printAtomToFilterInAtomTable(Atom* atomInTable);
    virtual void printAtomTable();
    virtual void printComputeStatement();

    ///Print weak constraint
    virtual void printWeak();
    ///Print the weak in the list. All the weak on the list have the same level
    void weakAtLevel(int level, list<id_weight_label>& listOfWeak);
    virtual void printWeakAtLevel(int level, vector<vector < id_weight_label>> weaks);

    ///Write the body of the rule in a new rule with in the head new aux atom and return the id of the new aux
    virtual unsigned rewriteBodyInAux(Rule* rule);

    ///Create for each id in idatoms a rule with in the head a new aux atom and in the body an element in the vector. Return
    /// the id of the new aux atom in the head.
    virtual unsigned createMultipleRule(vector<unsigned>& idatoms);

    virtual void onHeadChoiceRewrite();
    virtual void onBodyChoiceRuleNativelyRewritten(Rule* rule, Atom* atom);



protected:
    stringstream stream;
    stringstream streamAtomTable;
    stringstream streamFacts;
    bool printStream;
    bool printStreamFacts;

    /// id_weight_label represent a tuple of : body of the weak rewrited, the weight and the label of the weak
    /// weakLevelConstraints is a list of list, which the second list group the weak with the same level
    list<pair_level_tuple_list> weakLevelConstraints;
    /// Map for each level return the iterator of the list that group the weak with the level in the key
    unordered_map<unsigned,list_pair_it> levelWeak;

    const unsigned SIZE_COMPACT_FACTS;
    stringstream streamCompactFactsNumericTable;
    stringstream streamCompactFactsNumericTableTmp;
    stringstream streamCompactFacts;
    unsigned idCompactFacts;

    Atom *atomToFilter;
    void handleCompactFactsPrinting(Atom* atom);

};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_OUTPUT_NUMERICOUTPUTBUILDER_H_ */
