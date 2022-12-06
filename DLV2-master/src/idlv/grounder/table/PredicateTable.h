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
 * PredicateTable.h
 *
 *  Created on: 13/mar/2014
 *      Author: Davide
 */

#ifndef PREDICATETABLE_H_
#define PREDICATETABLE_H_

#include <unordered_set>
#include <iostream>

#include "../atom/Predicate.h"


using namespace std;

namespace DLV2{

namespace grounder{


/**
 * Hash table of predicate
 */
class PredicateTable {
public:

	static PredicateTable* getInstance();

	/// Insert predicate in a table and return the index of the predicate
	/// If the predicate is true negated insert also in the set predicate_true_negated
	inline void insertPredicate(Predicate *&p){
		predicate_set.insert(p);
		if(p->isTrueNegated()){
			string name=p->getName();
			Predicate *predNotTrueNegated=new Predicate(name,p->getArity());
			predNotTrueNegated->setTrueNegated(false);
			predicate_true_negated.insert(p);
			predicate_set.insert(predNotTrueNegated);
		}
	};

	Predicate* searchPredicateByIndex(index_object p){
		for(auto pred:predicate_set.flyweight_set)
			if(pred->getIndex()==p)
				return pred;
		return nullptr;
	};

	/// Get the predicate
	void getPredicate(Predicate*& p){predicate_set.get(p);};
	/// Get all the Edb predicate
	void getEdbPredicate(set_predicate& edb_pred);
	/// Get all the Edb predicate index
	void getEdbPredicate(unordered_set<index_object>& edb_pred);
	///Get all predicates in the program
	void getPredicateName(unordered_set<string>& names);

	bool existsInTrueNegatedVersion(Predicate* predicate) {
		if(predicate->isTrueNegated()) return true;
		string name=predicate->getName();
		Predicate *predTrueNegated=new Predicate(name,predicate->getArity());
		predTrueNegated->setTrueNegated(true);
		getPredicate(predTrueNegated);
		return predTrueNegated!=nullptr;
	}

	const set_predicate& getPredicateTrueNegated(){return predicate_true_negated;}

	void print(){predicate_set.print();}

	void setAllPredicateEDB(){
		for(auto predicate:predicate_set.flyweight_set){
			predicate->setEdb();
		}
	}

	void setAllPredicateHidden(){
		for(auto predicate:predicate_set.flyweight_set){
			predicate->setHiddenForPrinting(true);
		}
	}

	bool isPropositionalProgram(){
		for (auto& p : predicate_set.flyweight_set) {
			if(p->getArity()>0)
				return false;
		}
		return true;
	}

	Predicate* getQueryPredicate() const {
		return queryPredicate;
	}

	void setQueryPredicate(Predicate* queryPredicate = 0) {
		this->queryPredicate = queryPredicate;
	}

	unsigned getSize() const {return predicate_set.flyweight_set.size();}

	~PredicateTable(){};

	static void freeInstance(){
		if(predicateTable != nullptr)
		{
			delete predicateTable;
			predicateTable = nullptr;
		}
	}

	unordered_set<Predicate*,HashForTable<Predicate>,HashForTable<Predicate>>::const_iterator getPredicateSetBegin() const{
		return predicate_set.flyweight_set.begin();
	}

	unordered_set<Predicate*,HashForTable<Predicate>,HashForTable<Predicate>>::const_iterator getPredicateSetEnd() const{
		return predicate_set.flyweight_set.end();
	}

protected:
	static PredicateTable* predicateTable;
private:
	PredicateTable(){};

	FlyweightIndexFactory<Predicate> predicate_set;

	/// Set of predicate that have an atom true negated
	set_predicate predicate_true_negated;

	Predicate* queryPredicate=0;
};

};

};

#endif /* PREDICATETABLE_H_ */
