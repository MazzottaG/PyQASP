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
 * OutputBuilder.h
 *
 *  Created on: Jul 7, 2015
 *      Author: davide
 */

#ifndef SRC_GROUNDER_OUTPUT_OUTPUTBUILDER_H_
#define SRC_GROUNDER_OUTPUT_OUTPUTBUILDER_H_

#include "../statement/Rule.h"
#include "../../../common/Options.h"
#include "../ground/StatementDependency.h"
#include "../table/IdGenerator.h"

using namespace std;

namespace DLV2 {
namespace grounder {

using id_weight_label= tuple<unsigned,int,vector<Term*>>;
using pair_level_tuple_list = pair<int,list<id_weight_label>>;
using list_pair_it=list<pair_level_tuple_list>::iterator;

class OutputBuilder {
public:

    virtual void onRule(Rule *rule) {};
    virtual void onWeakConstraint(Rule* rule) {};
    virtual void onQuery() {};
    virtual void onHeadAtom(Atom * atom) {};
    virtual void onDisjunctionAtom(const vector<Atom*>& head) {};
    virtual void onConstraint(Rule* rule) {};
    virtual void onHead(const vector<Atom*>& head) {};
    virtual void onBody(Rule *rule) {};
    virtual void onClassicalLiteral(Atom *atom) {};
    virtual void onChoiceAtom(Atom *atom) {};
    virtual unsigned onAggregate(Atom *atom) {return 0;};
    virtual void onAggregateElement(Atom *atom) {};
    virtual void onFact(Atom* atom) {};
    virtual void onEnd(){};
    virtual void onBegin(){};
    virtual void onEndParsing(){};
    virtual void onPrintAtomTable() {};

    virtual void appendToStreamAtomTable(Atom* atom, bool fact=false){};
	virtual void setAtomToFilter(Atom* atomToFilter){};
	virtual void clearStreamAtomTable(){}

    virtual ~OutputBuilder(){};

    virtual void setEnablePrint(bool enable){}

    static OutputBuilder* getInstance();
    static void freeInstance(){delete outputBuilder;}

protected:
	OutputBuilder(){};

    static OutputBuilder* outputBuilder;

};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_OUTPUT_OUTPUTBUILDER_H_ */
