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
 * PredicateTable.cpp
 *
 *  Created on: 13/mar/2014
 *      Author: Davide
 */

#include "PredicateTable.h"

namespace DLV2{

namespace grounder{

PredicateTable *PredicateTable::predicateTable=nullptr;

void PredicateTable::getEdbPredicate(set_predicate& edb_pred) {
	for (auto p : predicate_set.flyweight_set)
		if (p->isEdb())
			edb_pred.insert(p);
}

PredicateTable* PredicateTable::getInstance() {
		if(predicateTable==nullptr)
			predicateTable=new PredicateTable;

		return predicateTable;
}

void PredicateTable::getEdbPredicate(unordered_set<index_object>& edb_pred) {
	for (auto p : predicate_set.flyweight_set)
		if (p->isEdb())
			edb_pred.insert(p->getIndex());
}

void PredicateTable::getPredicateName(unordered_set<string>& names) {
	for (auto p : predicate_set.flyweight_set)
		names.insert(p->getName());
}

};

};
