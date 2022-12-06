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
 * Instance.hMULTIPLE_TERMS
 *
 *  Created on: 24/mar/2014
 *      Author: Jessica
 */

#ifndef INSTANCE_H_
#define INSTANCE_H_

#include <iostream>

#include "../../../common/Options.h"
#include "IndexingStructure.h"
#include "IdGenerator.h"
#include "../output/OutputBuilder.h"
#include "../ground/StatementDependency.h"
#include "../atom/ClassicalLiteral.h"

using namespace std;

namespace DLV2{

namespace grounder{

/* Table of Predicate Extension:
 * 		FACT: Table of fact
 * 		NOFACT: Table of atom derived
 * 		DELTA, NEXTDELTA: Auxiliary table for grounding recursive rule
*/
#define FACT 0
#define NOFACT 1
#define DELTA 2
#define NEXTDELTA 3

class PredicateInformation{
public:
	PredicateInformation(unsigned arity);
	bool isOnlyPositive(unsigned index) const;
	bool isOnlyNegative(unsigned index) const;
	bool isOnlyPositive() const;
	bool isOnlyNegative() const;
	bool isNumeric(unsigned i) const {if(i<numeric.size()) return numeric[i]; return false;};
	void update(Atom* atom);
	int getMax(unsigned index) const;
	int getMin(unsigned index) const;
	void addInDictionary(unsigned position,Term* term);
	unsigned getSelectivity(unsigned pos)const{return termDictionary[pos].size();}
	bool isPresent(unsigned position,Term* term)const;
	const set_term& getDictionary(unsigned i)const {return termDictionary[i];};
	set_term::const_iterator getDictionaryBegin(unsigned i)const {return termDictionary[i].begin();};
	set_term::const_iterator getDictionaryEnd(unsigned i)const {return termDictionary[i].end();};

private:
	vector<int> min;
	vector<int> max;
	vector<bool> numeric;
	vector<set_term> termDictionary;

};


/**
 * This class represents the extension of each predicate (instances).
 * The class may have multiple table of atoms and for each table have an AtomSearcher associate
 */

class PredicateExtension {
public:
	///Constructor
	PredicateExtension(Predicate* predicate, unsigned tableNumber = 2): predicate(predicate), predicateInformation(new PredicateInformation(predicate->getArity())){
		tables.reserve(MAX_TABLE_NUMBER);
		atomSearchers.reserve(MAX_TABLE_NUMBER);
		addTables(tableNumber);
	}

	///Getter for the predicate
	inline Predicate* getPredicate() const {return predicate;}

	/// Add table and an AtomSearcher
	void addTables(unsigned numTables=1){
		for(unsigned i=0;i<numTables;++i){
			tables.push_back(new AtomHistoryVector());
			atomSearchers.push_back(new AtomSearcher(tables[i]));
		}
	}

	inline AtomHistoryVector* getTable(unsigned i)const{
		return tables[i];
	}

	///Returns the i-th AtomSeacher in atomSearchers
	inline AtomSearcher* getAtomSearcher(unsigned table)const{
		return atomSearchers[table];
	}

	IndexingStructure* getIndexingStructure(unsigned table, unsigned indexType, vector<unsigned>* indexingTerms=nullptr){
		return atomSearchers[table]->getIndexingStructure(indexType,indexingTerms);
	}

	IndexingStructure* getIndexingStructure(unsigned table, vector<unsigned>* indexingTerms=nullptr){
//		int indexType=Options::globalOptions()->getPredicateIndexType(predicate->getName());
//		if(indexType==-1){
//			if(StatementDependency::getInstance()->isOnlyInHead(predicate->getIndex()) || predicate->getArity()==1)
//				indexType=HASHSET;
//			else
//				indexType=Options::globalOptions()->getIndexType();
//		}
//		if(predicate->getArity()==0)
//			indexType=DEFAULT;
		return atomSearchers[table]->getIndexingStructure(indexingTerms);
	}

	///Set the index of the atom with new id if the atom is not yet indexed, and send it to the output builder
	void setIndexOfAtom(Atom* atom,unsigned table){
		if(atom->getPredicate()->isEdb() && Options::globalOptions()->isNofacts())
			return;
		Atom* query = StatementDependency::getInstance()->getGroundQueryAtom();
		if(query != nullptr && *atom == *query)
			atom->setIndex(query->getIndex());
		else if(Options::globalOptions()->isCompactFacts() && !atom->isFact())
			atom->setIndex(IdGenerator::getInstance()->getNewId(1));
		else
			atom->setIndex(IdGenerator::getInstance()->getNewId(1));
		if(!atom->getPredicate()->isHiddenForPrinting() && table!=0)
			OutputBuilder::getInstance()->appendToStreamAtomTable(atom,atom->isFact());
	}

	///Search the given ground atom in all tables
	inline Atom* getGroundAtom(Atom* atom){
		for(unsigned table=0;table<tables.size();++table){
			IndexingStructure* indexingStructure=atomSearchers[table]->getDefaultIndexingStructure();
			if(indexingStructure!=nullptr){
				Atom* atomFound=indexingStructure->find(atom);
				if(atomFound!=nullptr)
					return atomFound;
			}
		}
		return nullptr;
	}

	void addAtom(Atom* atom, unsigned table){
		tables[table]->push_back(atom);
		predicateInformation->update(atom);

		if(predicate->isSolved() && !atom->isFact())
			predicate->setSolved(false);
		if(atom->getIndex()==0) setIndexOfAtom(atom,table);
	}

	//Moves the content of the tableFrom (source) to the tableTo (destination)
	void swapTables(unsigned tableFrom,unsigned tableTo);

	//Swap the pointers of the tableFrom (source) to the tableTo (destination)
	void swapPointersTables(unsigned tableFrom,unsigned tableTo);

	///Update the indexes of Delta and NF table for the recursion
	void updateIndexesTable(unsigned table,unsigned iteration){
		if(tables.size()>table){
			tables[table]->updateIndices(iteration);
			trace_action_tag(recursion,1,cerr<<"TABLE "<<table<<" IT: "<<tables[table]->getCurrentIteration()<<" DELTA-INDEX: "<<
					tables[table]->getDeltaIndexIteration()<<" NF-INDEX: "<<tables[table]->getNFIndexIteration()<<endl;);
			trace_action_tag(recursion,1,for(unsigned i=0;i<tables[table]->size();i++)(*tables[table])[i]->print(cerr);;cerr<<endl;);

		}

	}

	///Printer method for a single table
	inline void print(unsigned table){for(auto fact:*tables[table]){OutputBuilder::getInstance()->onFact(fact);}}

	inline void printTextual(ostream& stream,unsigned table){
		for(auto fact:*tables[table]){
			ClassicalLiteral::print(predicate,fact->getTerms(),false,false,stream);stream<<"."<<endl;
		}
	}

	inline void printQueryFactTable(){
		bool numeric=Options::globalOptions()->getOutputFormat()==OUTPUT_NUMERIC_SMODELS || Options::globalOptions()->getOutputFormat()==OUTPUT_NUMERIC_INTERMEDIATE;
		bool query=Options::globalOptions()->getOutputFormat()==OUTPUT_QUERY_TEXTUAL;
		OutputBuilder::getInstance()->setEnablePrint(true);
		for(auto fact:*tables[FACT]){
			if(numeric)
				OutputBuilder::getInstance()->appendToStreamAtomTable(fact,fact->isFact());
			else if(query){
				OutputBuilder::getInstance()->onFact(fact);
			}
		}
		OutputBuilder::getInstance()->setEnablePrint(false);
	}

	///Printer method for a single table
	inline void print(){for(unsigned table=0;table<tables.size();table++) print(table);}

	inline void printTextual(ostream& stream){for(unsigned table=0;table<tables.size();table++) printTextual(stream,table);}

	///Destructor
	~PredicateExtension();

	static void setMaxTableNumber(unsigned int maxTableNumber) {MAX_TABLE_NUMBER = maxTableNumber;}

	PredicateInformation* getPredicateInformation() const {return predicateInformation;}

	unsigned getPredicateExtentionSize(unsigned table) const {if(table<tables.size()) return tables[table]->size(); return 0;}
	unsigned getPredicateExtentionSize() const { unsigned size=0; for(auto table:tables) size+=table->size(); return size;}
	unsigned getPredicateExtentionSize(unsigned table,SearchType type) const {
		if(table<tables.size())
			return tables[table]->size_iteration(type);
		return 0;
	}
	unsigned getPredicateExtentionSize(unsigned table,SearchType type,unsigned iteration) const {
		if(table<tables.size()) {
			auto it=tables[table]->getElements(type,iteration);
			return it.second-it.first;
		}
		return 0;
	}

	inline void printFacts(){
		OutputBuilder *out=OutputBuilder::getInstance();
		for(unsigned table=0;table<tables.size();table++)
			for(auto fact:*tables[table])
				out->onFact(fact);
	}

	inline vector<Atom*>::const_iterator getTableBegin(unsigned i)const{
		return tables[i]->begin();
	}

	inline vector<Atom*>::const_iterator getTableEnd(unsigned i)const{
		return tables[i]->end();
	}


	IndexingStructure* addAtomSearcher(unsigned table, vector<unsigned>* indexingTerms,bool recursive=false);
	IndexingStructure* addAtomSearcher(unsigned table, unsigned type, vector<unsigned>* indexingTerms,bool recursive=false);
	IndexingStructure* addFullIndexAtomSearcher(unsigned table, bool recursive=false);
	IndexingStructure* createAtomSearcher(unsigned table, unsigned indexType,  vector<unsigned>* indexingTerms, bool recursive=false);
	bool isAlreadyCreatedAtomSearcher(unsigned type, vector<unsigned>* indexingTerms, bool recursive);

private:
	///The predicate
	Predicate* predicate;

	///For each AtomTable in tables is present an AtomSeacher in atomSearchers
	///The vector of tables
	vector<AtomHistoryVector*> tables;
	///The vector of  AtomSeacher
	vector<AtomSearcher*> atomSearchers;

	static unsigned int MAX_TABLE_NUMBER;

	///A PredicateInformation object that stores the information about the max and the min values of each term in the instances of the predicate
	PredicateInformation* predicateInformation;

};

/**
 * This class stores the Predicate Extensions for all the predicate.
 * Is a Singleton.
 */

class PredicateExtTable {
public:

	///This method adds an Instance for a predicate
	void addPredicateExt(Predicate* p) {
		if(!predicateExtTable.count(p->getIndex())){
			PredicateExtension *is=new PredicateExtension(p);
			predicateExtTable.insert({p->getIndex(),is});
		}
	};

	///Getter for the instances of a predicate
	PredicateExtension* getPredicateExt(Predicate* p) {
		auto result= predicateExtTable.find(p->getIndex());
		if(result==predicateExtTable.end()) return nullptr;
		return (result->second);
	};

	///Getter for the instances of a predicate index
	PredicateExtension* getPredicateExt(index_object p) {
		auto result= predicateExtTable.find(p);
		if(result==predicateExtTable.end()) return nullptr;
		return (result->second);
	};

	///This method return the size of the map of instances
	unsigned int getSize() {return predicateExtTable.size();};

	///Printer method for the first table in Predicate Extension
	void print(unsigned table) {for (auto& i : predicateExtTable) if(!(i.second->getPredicate()->isHiddenForPrinting())) i.second->print(table);};

	void print(){ for (auto& i : predicateExtTable) if(!(i.second->getPredicate()->isHiddenForPrinting())) i.second->print();}

	void print(ostream& stream){ for (auto& i : predicateExtTable) i.second->printTextual(stream);}

	void printFacts(){
		for(auto& it:predicateExtTable)
			it.second->printFacts();
	}

	///Destructor
	~PredicateExtTable(){for(auto pair_predExt:predicateExtTable) delete pair_predExt.second;};

	static PredicateExtTable* getInstance(){
		if(predicateExtTable_== nullptr)
			predicateExtTable_= new PredicateExtTable;
		return predicateExtTable_;
	}

	static void freeInstance(){
		if(predicateExtTable_!=nullptr){
			delete predicateExtTable_;
			predicateExtTable_=nullptr;
		}
	}


private:
	///Constructor
	PredicateExtTable(){}

	static PredicateExtTable *predicateExtTable_;

	///The map that stores all the Predicate Extension
	unordered_map<index_object,PredicateExtension*> predicateExtTable;
};

};

};

#endif /* INSTANCE_H_ */
