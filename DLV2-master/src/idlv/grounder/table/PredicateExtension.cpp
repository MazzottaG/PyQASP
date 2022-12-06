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
 * Instance.cpp
 *
 *  Created on: 24/mar/2014
 *      Author: Jessica
 */

#include "PredicateExtension.h"
//#include "../utility/Timer.h"

namespace DLV2{

namespace grounder{

/****************************************************** PREDICATE EXTENSION ***************************************************/

unsigned int PredicateExtension::MAX_TABLE_NUMBER = 4;

IndexingStructure* PredicateExtension::createAtomSearcher(unsigned table, unsigned indexType, vector<unsigned>* indexingTerms,bool recursive) {
	IndexingStructure* indexingStructure;
	switch (indexType) {
	case (MAP):
		indexingStructure = new UnorderedMapOfUnorderedSet(tables[table],*indexingTerms);
		break;
//	case (MAP_DICTIONARY_INTERSECTION):
//		atomSearcher = new SingleTermMapDictionaryAtomSearcher(tables[table],
//				predicate);
//		break;
	case (MULTIMAP):
		indexingStructure = new UnorderedMultiMap(tables[table],*indexingTerms);
		break;
	case (HASHSET):
		indexingStructure = new UnorderedSet(tables[table]);
		break;
	case (DOUBLEMAP):
		indexingStructure = new UnorderedMapOfUnorderedMultimap(tables[table],*indexingTerms);
		break;
	case (DOUBLEMAP_PAIR):
		indexingStructure = new UnorderedMapOfPair(tables[table],*indexingTerms);
		break;
	case (MULTIPLE_TERMS):
		indexingStructure = new MulplipleTermsMap(tables[table],*indexingTerms);
		break;
	case (MAP_VECTOR):
		indexingStructure = new UnorderedMapOfVector(tables[table],*indexingTerms);
		break;
	case (MAP_HISTORY_VECTOR):
		indexingStructure = new UnorderedMapOfHistoryVector(tables[table],*indexingTerms);
		break;
	case (MAP_PAIR_HISTORY_VECTOR):
		indexingStructure = new UnorderedMapOfPairHistoryVector(tables[table],*indexingTerms);
		break;
	case (DEFAULT_RECURSIVE):
		indexingStructure = new IndexingStructureRecursive(tables[table]);
		break;
	case (HISTORY_HASHSET):
		indexingStructure = new HistoryUnorderedSet(tables[table]);
		break;
	default:
			indexingStructure = new IndexingStructure(tables[table]);
		break;
	}
	return indexingStructure;
}

IndexingStructure* PredicateExtension::addAtomSearcher(unsigned table, vector<unsigned>* indexingTerms, bool recursive){
	// Properly set the IndexAtom type
	if(table<tables.size()){
		int indexType=-1; //Options::globalOptions()->getPredicateIndexType(predicate->getName());
		if(indexType==-1){
			if(predicate->getArity()==1 && indexType!=DEFAULT_RECURSIVE ) //StatementDependency::getInstance()->isOnlyInHead(predicate->getIndex()) ||
				indexType=HASHSET;
//			else if(indexingTerms->size()>1)
//				indexType=DOUBLEMAP;
			else
				indexType=Options::globalOptions()->getIndexType();
		}

		if(predicate->getArity()==0)
			indexType=DEFAULT;

		if(indexType==DEFAULT || indexType==DEFAULT_RECURSIVE || indexType==HASHSET || indexType==HISTORY_HASHSET){ indexingTerms=nullptr;}


	#ifdef DEBUG_ATOM_SEARCHER
		cout<<"Predicate: "<<predicate->getName()<<"  Index type: "<<indexType<<endl;
	#endif
		IndexingStructure* indexingStruct;
		if(indexType==MAP_VECTOR)
			indexingStruct=atomSearchers[table]->getIndexingStructure(indexingTerms);
		else
			indexingStruct=atomSearchers[table]->getIndexingStructure(indexType,indexingTerms);
		if(indexingStruct==nullptr){
			indexingStruct = createAtomSearcher(table, indexType, indexingTerms,recursive);
			atomSearchers[table]->addIndexingStructure(indexingStruct);
		}
		return indexingStruct;
	}
	return 0;
}

IndexingStructure* PredicateExtension::addFullIndexAtomSearcher(unsigned table, bool recursive) {
	if(table<tables.size()){
		int indexType=-1; //Options::globalOptions()->getPredicateIndexType(predicate->getName());
		if(indexType==-1){
			if(predicate->getArity()==1 && indexType!=DEFAULT_RECURSIVE) //StatementDependency::getInstance()->isOnlyInHead(predicate->getIndex()) ||
				indexType=HASHSET;
			else
				indexType=Options::globalOptions()->getIndexType();
		}
		if(predicate->getArity()==0)
			indexType=DEFAULT;

		IndexingStructure* indexingStruct = atomSearchers[table]->getIndexingStructure(SINGLE_ARG_FULL,nullptr);
		if(indexingStruct==nullptr){
			indexingStruct = new FullIndexingStructure(tables[table], predicate, atomSearchers[table], recursive, indexType);
			atomSearchers[table]->addIndexingStructure(indexingStruct);
		}
		return indexingStruct;
	}
	return 0;
}

IndexingStructure* PredicateExtension::addAtomSearcher(unsigned table, unsigned type, vector<unsigned>* indexingTerms, bool recursive) {
	if(table<tables.size()){
		int indexType=type;
		if(predicate->getArity()==0 && type!=DEFAULT_RECURSIVE && type!=HISTORY_HASHSET)
			indexType=DEFAULT;
		if(predicate->getArity()==1 && type!=DEFAULT_RECURSIVE && type!=HISTORY_HASHSET)
			indexType=HASHSET;

		if(indexType==DEFAULT || indexType==DEFAULT_RECURSIVE || indexType==HASHSET || indexType==HISTORY_HASHSET){ indexingTerms=nullptr;}

		IndexingStructure* indexingStruct=atomSearchers[table]->getIndexingStructure(indexType,indexingTerms);
		if(indexingStruct==nullptr){
			if(indexingTerms==nullptr){
				vector<unsigned> v;
				v.push_back(0);
				indexingTerms=&v;
				indexingStruct = createAtomSearcher(table, indexType, indexingTerms,recursive);
			}
			else
				indexingStruct = createAtomSearcher(table, indexType, indexingTerms,recursive);
			atomSearchers[table]->addIndexingStructure(indexingStruct);
		}
		return indexingStruct;
	}
	return 0;
}

bool PredicateExtension::isAlreadyCreatedAtomSearcher(unsigned type, vector<unsigned>* indexingTerms, bool recursive) {
	for(unsigned table=0;table<tables.size();++table){
		int indexType=type;
		if(predicate->getArity()==0 && type!=DEFAULT_RECURSIVE && type!=HISTORY_HASHSET)
			indexType=DEFAULT;
		if(predicate->getArity()==1 && type!=DEFAULT_RECURSIVE && type!=HISTORY_HASHSET)
			indexType=HASHSET;

		IndexingStructure* indexingStruct=atomSearchers[table]->getIndexingStructure(indexType,indexingTerms);
		if(indexingStruct==nullptr)
			return false;
	}
	return true;
}

PredicateExtension::~PredicateExtension() {
	if(tables.empty()) return;
	bool nofact=Options::globalOptions()->isNofacts();
	bool hidden=predicate->isHiddenForPrinting();
	for(unsigned int i=0;i<tables.size();++i){
		if(tables[i]==nullptr) continue;
		for (auto it = tables[i]->begin(); it != tables[i]->end(); ++it){
			if(!nofact && !hidden && i==0){
				Atom*& atom=(*it);
				OutputBuilder::getInstance()->onFact(atom);
				OutputBuilder::getInstance()->appendToStreamAtomTable(atom,atom->isFact());
			}
			delete *it;
			*it = nullptr;
		}
		delete tables[i];
		tables[i]=nullptr;
	}
	vector<AtomHistoryVector*>().swap(tables);

	for(unsigned int i=0;i<atomSearchers.size();++i){
		if(atomSearchers[i]!=nullptr)
		{
			delete atomSearchers[i];
			atomSearchers[i]=nullptr;
		}
	}
	vector<AtomSearcher*>().swap(atomSearchers);

	if(predicateInformation!=nullptr){
		delete predicateInformation;
		predicateInformation=nullptr;
	}
}

void PredicateExtension::swapTables(unsigned tableFrom,unsigned tableTo){
//	assert_msg(tableFrom<tables.size(),"The specified table doesn't exist.");
//	assert_msg(tableTo<tables.size(),"The specified table doesn't exist.");

	AtomHistoryVector *table_from=tables[tableFrom];
	AtomHistoryVector *table_to=tables[tableTo];

	unsigned size=table_from->size();
	table_to->reserve(size+table_to->size());
	for (int i = size-1; i >= 0; --i) {
		Atom* currentAtom=(*table_from)[i];

		table_to->push_back(currentAtom);
		table_from->pop_back();

	}

	atomSearchers[tableFrom]->clear();
}

void PredicateExtension::swapPointersTables(unsigned tableFrom, unsigned tableTo) {
//	assert_msg(tableFrom<tables.size(),"The specified table doesn't exist.");
//	assert_msg(tableTo<tables.size(),"The specified table doesn't exist.");

	AtomSearcher *seacher_from=atomSearchers[tableFrom];
	AtomSearcher *seacher_to=atomSearchers[tableTo];
	AtomSearcher *searcher_tmp=seacher_from;
	atomSearchers[tableFrom]=seacher_to;
	atomSearchers[tableTo]=searcher_tmp;

	AtomHistoryVector* table_from=tables[tableFrom];
	AtomHistoryVector* table_to=tables[tableTo];
	AtomHistoryVector* table_tmp=table_from;
	tables[tableFrom]=table_to;
	tables[tableTo]=table_tmp;

}

/****************************************************** PREDICATE EXT TABLE ***************************************************/

PredicateExtTable *PredicateExtTable::predicateExtTable_=nullptr;

/****************************************************** PREDICATE INFORMATION ***************************************************/

PredicateInformation::PredicateInformation(unsigned arity) : min(arity,INT_MAX), max(arity,INT_MIN) {
	min.reserve(arity);
	max.reserve(arity);
	numeric.resize(arity,false);
	termDictionary.resize(arity);
}

bool PredicateInformation::isOnlyPositive(unsigned index) const {
	if(min[index]>=0)
		return true;
	return false;
}

bool PredicateInformation::isOnlyNegative(unsigned index) const {
	if(max[index]<=0)
		return true;
	return false;
}

bool PredicateInformation::isOnlyPositive() const {
	for(auto n:min){
		if(n<0)
			return false;
	}
	return true;
}

bool PredicateInformation::isOnlyNegative() const {
	for(auto n:max){
		if(n>0)
			return false;
	}
	return true;
}

void PredicateInformation::update(Atom* atom) {
	for (unsigned i = 0; i < atom->getTermsSize(); ++i) {
		Term* t=atom->getTerm(i);
		if(t->getType()==TermType::NUMERIC_CONSTANT){
			int val=t->getConstantValue();
			if(t->getConstantValue()>max[i])
				max[i]=val;
			if(t->getConstantValue()<min[i])
				min[i]=val;
		}
		else
			numeric[i]=false;
		addInDictionary(i,t);
	}
}

int PredicateInformation::getMax(unsigned index) const {
	return max[index];
}

int PredicateInformation::getMin(unsigned index) const {
	return min[index];
}

void PredicateInformation::addInDictionary(unsigned position, Term* term) {
	termDictionary[position].insert(term);
}

bool PredicateInformation::isPresent(unsigned position, Term* term) const {
	return termDictionary[position].count(term);
}


}
}

