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
 * IndexingStructure.h
 *
 *  Created on: Jan 15, 2016
 *      Author: jessica
 */

#ifndef SRC_GROUNDER_TABLE_INDEXINGSTRUCTURE_H_
#define SRC_GROUNDER_TABLE_INDEXINGSTRUCTURE_H_

#include <vector>
#include "../atom/Atom.h"
#include "../../util/Constants.h"
#include "../statement/Rule.h"
#include "AdvancedArray.h"

using namespace std;

namespace DLV2 {
namespace grounder {

/** This class implements a general way to access to different atom containers.
 * 	It is implemented according to the GOF pattern Iterator.
 **/
class GeneralIterator {
public:
	virtual void add(GeneralIterator* g){};
	virtual void next()=0;
	virtual bool isDone()=0;
	virtual Atom* currentItem()=0;
	virtual ~GeneralIterator(){};
};
///This class implements and hides an iterator for atom vector with iterator
class VectorIterator : public GeneralIterator {
public:
	VectorIterator(const AtomHistoryVector::const_iterator& s, const AtomHistoryVector::const_iterator& e): start(s), end(e){};
	virtual void next(){ if(start!=end) ++start;}
	virtual bool isDone(){return start==end;}
	virtual Atom* currentItem(){return *(start);}
private:
	AtomHistoryVector::const_iterator start;
	AtomHistoryVector::const_iterator end;
};
///This class implements and hides an iterator for atom vector with indexing
class VectorIteratorIndex : public GeneralIterator {
public:
	VectorIteratorIndex(unsigned s,unsigned e,AtomHistoryVector* table): start(s), end(e),table(table){};
	virtual void next(){ if(start!=end) ++start;}
	virtual bool isDone(){return start==end;}
	virtual Atom* currentItem(){return (*table)[start];}
private:
	unsigned start;
	unsigned end;
	AtomHistoryVector* table;
};
///This class implements and hides an iterator for atom unordered set
class UnorderedSetIterator : public GeneralIterator {
public:
	UnorderedSetIterator(const AtomTable::const_iterator& s, const AtomTable::const_iterator& e): start(s), end(e){};
	virtual void next(){ if(start!=end) ++start;}
	virtual bool isDone(){return start==end;}
	virtual Atom* currentItem(){return *(start);}
private:
	AtomTable::const_iterator start;
	AtomTable::const_iterator end;
};
///This class implements and hides an iterator for atom unordered multimap
class UnorderedMultiMapIterator : public GeneralIterator {
public:
	UnorderedMultiMapIterator(const Multimap_Atom::const_iterator& s, const Multimap_Atom::const_iterator& e): start(s), end(e){};
	virtual void next(){ if(start!=end) ++start;}
	virtual bool isDone(){return start==end;}
	virtual Atom* currentItem(){return start->second;}
private:
	Multimap_Atom::const_iterator start;
	Multimap_Atom::const_iterator end;
};
///This class implements and hides an iterator for atom vector
class MultipleIterators : public GeneralIterator{
public:
	MultipleIterators():currentIterator(0){};
	virtual void add(GeneralIterator* g){iterators.push_back(g);}
	__attribute__ ((always_inline)) virtual void next(){
		iterators[currentIterator]->next();
		if(iterators[currentIterator]->isDone())
			currentIterator++;
	}
	virtual bool isDone(){return currentIterator>=iterators.size();}
	virtual Atom* currentItem(){return iterators[currentIterator]->currentItem();}
	virtual ~MultipleIterators(){for(auto it:iterators) delete it;};
private:
	vector<GeneralIterator*> iterators;
	unsigned currentIterator;
};

struct HashVectorOfTerms{
	inline size_t operator()(const vector<index_object>& obj) const {
		return HashVecInt::getHashVecInt()->computeHash(obj);
	}

	inline bool operator()(const vector<index_object>& obj1, const vector<index_object>& obj2) const {
		if(obj1.size()!=obj2.size())return false;
		return equal(obj1.begin(),obj1.end(),obj2.begin());
	}
};



class AtomSearcher;

/**
 * This class represents a generic indexing data structure of atoms (@see Atom.h)
 **/
class IndexingStructure {
public:
	IndexingStructure(AtomHistoryVector* table):table(table),lastUpdate(0){};
	IndexingStructure(AtomHistoryVector* table, vector<unsigned>& indexingTerms):table(table),lastUpdate(0),indexingTerms(move(indexingTerms)){};

	///Add an atom the the indexing data structure
	virtual void add(Atom* atom){};
	///Search a (ground) atom in the indexing data structure
	virtual Atom* find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification={ALL,0});
	///Erase the content of the indexing data structure
	virtual void clear(){lastUpdate=0;};
	///Update the indexing data structure by adding the atoms in the table starting from lastUpdate
	virtual void update(){};
	///Given a partially ground atom compute the matching atoms according to the variables assignment in these atom
	inline virtual GeneralIterator* computeMatchIterator(Atom* templateAtom,const RuleInformation& ruleInformation,const pair<SearchType,unsigned>& searchSpecification,unsigned arg=0){return new VectorIterator(table->begin(),table->end());};
	///Univocal label for each class implementing IndexingStructure
	virtual unsigned getType(){return DEFAULT;};

	///Get the indexing terms
	const vector<unsigned>& getIndexingTerms() const {return indexingTerms;}
	///Set the indexing terms
	void setIndexingTerms(const vector<unsigned>& indexingTerm) {this->indexingTerms = indexingTerm;}
	///Add an indexing term
	void addIndexingTerm(unsigned indexingTerm) {this->indexingTerms.push_back(indexingTerm);}

	virtual ~IndexingStructure(){}

	virtual bool isEqual(unsigned type, vector<unsigned>* indexingTerms){
		if(this->getType()==type){
			if(indexingTerms==nullptr)
				return true;
			else{
				if(this->indexingTerms.size()!=indexingTerms->size())
					return false;
				for(unsigned i=0;i<indexingTerms->size();++i){
					if(this->indexingTerms[i]!=(*indexingTerms)[i])
						return false;
				}
				return true;
			}
		}
		return false;
	}

protected:
	///Vector of atoms for which the indexing data structure is created
	AtomHistoryVector* table;
	///The position of the last atom in table to which the indexing data structure has been updated
	unsigned lastUpdate;
	///The vector of indexing terms
	vector<unsigned> indexingTerms;
};

/**
 * This class implements IndexingStructure and is used for the recursive predicate. The compute match search in the history vector
 * the elements based on the type of search and the current iteration
 **/

class IndexingStructureRecursive : public IndexingStructure{
public:
	IndexingStructureRecursive(AtomHistoryVector* table):IndexingStructure(table){};

	inline virtual GeneralIterator* computeMatchIterator(Atom* templateAtom,const RuleInformation& ruleInformation,const pair<SearchType,unsigned>& searchSpecification,unsigned arg=0){
		auto it=table->getElements(searchSpecification.first,searchSpecification.second);
		return new VectorIteratorIndex(it.first,it.second,table);
	};

	virtual unsigned getType(){return DEFAULT_RECURSIVE;};

};

/**
 * This class implements IndexingStructure and provides an unordered set of atoms as indexing data structure (@see Atom.h)
 **/
class UnorderedSet : public IndexingStructure {
public:
	UnorderedSet(AtomHistoryVector* table):IndexingStructure(table){};
	void add(Atom* atom){indexingStructure.insert(atom);};
	Atom* find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification={ALL,0});
	void clear(){IndexingStructure::clear(); indexingStructure.clear();};
	virtual void update();
	virtual unsigned getType(){return HASHSET;};
private:
	AtomTable indexingStructure;
};

/**
 * This class implements IndexingStructure and provides an unordered set of atoms as indexing data structure (@see Atom.h)
 **/
class HistoryUnorderedSet : public IndexingStructure {
public:
	HistoryUnorderedSet(AtomHistoryVector* table):IndexingStructure(table),updateIteration(0){};
	void add(Atom* atom){};
	Atom* find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification);
	void clear(){IndexingStructure::clear(); deltaIndexingStructure.clear();nfIndexingStructure.clear();};
	virtual void update();
	virtual unsigned getType(){return HISTORY_HASHSET;};
private:
	AtomTable deltaIndexingStructure;
	AtomTable nfIndexingStructure;
	unsigned updateIteration;
};

/**
 * This class implements IndexingStructure and provides a single term indexing data structure
 * implemented by means of an unordered map of unordered sets of atoms as indexing data structure (@see Atom.h)
 **/
class UnorderedMapOfUnorderedSet : public IndexingStructure {
public:
	UnorderedMapOfUnorderedSet(AtomHistoryVector* table, vector<unsigned>& indexingTerm): IndexingStructure(table,indexingTerm){};
	void add(Atom* atom);
	Atom* find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification={ALL,0});
	void clear(){IndexingStructure::clear(); indexingStructure.clear();};
	virtual void update();
	virtual GeneralIterator* computeMatchIterator(Atom* templateAtom,const RuleInformation& ruleInformation,const pair<SearchType,unsigned>& searchSpecification,unsigned arg=0);
	virtual unsigned getType(){return MAP;}
	virtual bool isEqual(unsigned type, vector<unsigned>* indexingTerms){
		if(indexingTerms!=nullptr && (type==MAP || type==MULTIMAP || type==MAP_VECTOR)){
			if(this->indexingTerms[0]==(*indexingTerms)[0])
				return true;
		}
		return false;
	}
private:
	unordered_map<index_object,AtomTable> indexingStructure;
};

/**
 * This class implements IndexingStructure and provides a single term indexing data structure
 * implemented by means of an unordered map of vectors of atoms as indexing data structure (@see Atom.h)
 **/
class UnorderedMapOfVector : public IndexingStructure {
public:
	UnorderedMapOfVector(AtomHistoryVector* table, vector<unsigned>& indexingTerm): IndexingStructure(table,indexingTerm){};
	void add(Atom* atom);
	Atom* find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification={ALL,0});
	void clear(){IndexingStructure::clear(); indexingStructure.clear();};
	virtual void update();
	virtual GeneralIterator* computeMatchIterator(Atom* templateAtom,const RuleInformation& ruleInformation,const pair<SearchType,unsigned>& searchSpecification,unsigned arg=0);
	virtual unsigned getType(){return MAP_VECTOR;}
	virtual bool isEqual(unsigned type, vector<unsigned>* indexingTerms){
		if(indexingTerms!=nullptr && (type==MAP || type==MULTIMAP || type==DOUBLEMAP || type==MAP_VECTOR)){
			if(this->indexingTerms[0]==(*indexingTerms)[0])
				return true;
		}
		return false;
	}
private:
	unordered_map<index_object,vector<Atom*>> indexingStructure;
};

/**
 * This class implements IndexingStructure and provides a single term indexing data structure
 * implemented by means of an unordered map of history vectors of atoms as indexing data structure (@see Atom.h)
 **/
class UnorderedMapOfHistoryVector : public IndexingStructure {
public:
	UnorderedMapOfHistoryVector(AtomHistoryVector* table, vector<unsigned>& indexingTerm): IndexingStructure(table,indexingTerm),lastUpdateIteration(0){};
	void add(Atom* atom);
	Atom* find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification={ALL,0});
	void clear(){IndexingStructure::clear(); indexingStructure.clear();};
	virtual void update(unsigned iteration);
	virtual GeneralIterator* computeMatchIterator(Atom* templateAtom,const RuleInformation& ruleInformation,const pair<SearchType,unsigned>& searchSpecification,unsigned arg=0);
	virtual unsigned getType(){return MAP_HISTORY_VECTOR;}
private:
	unordered_map<index_object,AtomHistoryVector> indexingStructure;
	unsigned lastUpdateIteration;
};

/**
 * This class implements IndexingStructure and provides a single term indexing data structure
 * implemented by means of an unordered map of history vectors of atoms as indexing data structure (@see Atom.h)
 **/
class UnorderedMapOfPairHistoryVector : public IndexingStructure {
public:
	UnorderedMapOfPairHistoryVector(AtomHistoryVector* table, vector<unsigned>& indexingTerm): IndexingStructure(table,indexingTerm),lastUpdateIteration(0){};
	void add(Atom* atom);
	Atom* find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification={ALL,0});
	void clear(){IndexingStructure::clear(); indexingStructure.clear();};
	virtual void update(unsigned iteration);
	virtual GeneralIterator* computeMatchIterator(Atom* templateAtom,const RuleInformation& ruleInformation,const pair<SearchType,unsigned>& searchSpecification,unsigned arg=0);
	virtual unsigned getType(){return MAP_PAIR_HISTORY_VECTOR;}
private:
	unordered_map<pair<index_object,index_object>,AtomHistoryVector,HashPair<index_object>,HashPair<index_object>> indexingStructure;
	unsigned lastUpdateIteration;
};

/**
 * This class implements IndexingStructure and provides a single term indexing data structure
 * implemented by means of an unordered multimap of atoms as indexing data structure (@see Atom.h)
 **/
class UnorderedMultiMap : public IndexingStructure {
public:
	UnorderedMultiMap(AtomHistoryVector* table, vector<unsigned>& indexingTerm): IndexingStructure(table,indexingTerm){};
	void add(Atom* atom);
	Atom* find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification={ALL,0});
	void clear(){IndexingStructure::clear(); indexingStructure.clear();};
	virtual void update();
	virtual GeneralIterator* computeMatchIterator(Atom* templateAtom,const RuleInformation& ruleInformation,const pair<SearchType,unsigned>& searchSpecification,unsigned arg=0);
	virtual unsigned getType(){return MULTIMAP;}
	virtual bool isEqual(unsigned type, vector<unsigned>* indexingTerms){
		if(indexingTerms!=nullptr && (type==MAP || type==MULTIMAP || type==DOUBLEMAP || type==MAP_VECTOR)){
			if(this->indexingTerms[0]==(*indexingTerms)[0])
				return true;
		}
		return false;
	}
private:
	unordered_multimap<index_object,Atom*> indexingStructure;
};

/**
 * This class implements IndexingStructure and provides a double term indexing data structure
 * implemented by means of an unordered map of unordered multimap of atoms as indexing data structure (@see Atom.h)
 **/
class UnorderedMapOfUnorderedMultimap : public IndexingStructure {
public:
	UnorderedMapOfUnorderedMultimap(AtomHistoryVector* table, vector<unsigned>& indexingTerm): IndexingStructure(table,indexingTerm){};
	void add(Atom* atom);
	Atom* find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification={ALL,0});
	void clear(){IndexingStructure::clear(); indexingStructure.clear();};
	virtual void update();
	virtual GeneralIterator* computeMatchIterator(Atom* templateAtom,const RuleInformation& ruleInformation,const pair<SearchType,unsigned>& searchSpecification,unsigned arg=0);
	virtual unsigned getType(){return DOUBLEMAP;};
	virtual bool isEqual(unsigned type, vector<unsigned>* indexingTerms){
		if(indexingTerms!=nullptr){
//			if(type==MAP || type==MULTIMAP || type==MAP_VECTOR){
//				if(this->indexingTerms[0]==(*indexingTerms)[0])
//					return true;
//			}
//			else
			if(type==DOUBLEMAP){
				if(this->indexingTerms[0]==(*indexingTerms)[0] && this->indexingTerms[1]==(*indexingTerms)[1])
					return true;
			}
		}
		return false;
	}
private:
	unordered_map<index_object,unordered_multimap<index_object,Atom*>> indexingStructure;
};


/**
 * This class implements IndexingStructure and provides a double term indexing data structure
 * implemented by means of an unordered map of pair of terms as indexing data structure (@see Atom.h)
 **/
class UnorderedMapOfPair : public IndexingStructure {
public:
	UnorderedMapOfPair(AtomHistoryVector* table, vector<unsigned>& indexingTerm): IndexingStructure(table,indexingTerm){};
	void add(Atom* atom);
	Atom* find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification={ALL,0});
	void clear(){IndexingStructure::clear(); indexingStructure.clear();};
	virtual void update();
	virtual GeneralIterator* computeMatchIterator(Atom* templateAtom,const RuleInformation& ruleInformation,const pair<SearchType,unsigned>& searchSpecification,unsigned arg=0);
	virtual unsigned getType(){return DOUBLEMAP_PAIR;};
	virtual bool isEqual(unsigned type, vector<unsigned>* indexingTerms){
		if(indexingTerms!=nullptr){
//			if(type==MAP || type==MULTIMAP || type==MAP_VECTOR){
//				if(this->indexingTerms[0]==(*indexingTerms)[0])
//					return true;
//			}
//			else
			if(type==DOUBLEMAP_PAIR){
				if(this->indexingTerms[0]==(*indexingTerms)[0] && this->indexingTerms[1]==(*indexingTerms)[1])
					return true;
			}
		}
		return false;
	}
private:
	unordered_map<pair<index_object,index_object>,AtomTable,HashPair<index_object>,HashPair<index_object>> indexingStructure;
};

/**
 * This class implements IndexingStructure and provides a general multiple terms indexing data structure
 * implemented by means of an unordered map of vectors of terms as indexing data structure (@see Atom.h)
 **/
class MulplipleTermsMap : public IndexingStructure {
public:
	MulplipleTermsMap(AtomHistoryVector* table, vector<unsigned>& indexingTerm): IndexingStructure(table,indexingTerm){};
	void add(Atom* atom);
	Atom* find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification={ALL,0});
	void clear(){IndexingStructure::clear(); indexingStructure.clear();};
	virtual void update();
	virtual GeneralIterator* computeMatchIterator(Atom* templateAtom,const RuleInformation& ruleInformation,const pair<SearchType,unsigned>& searchSpecification,unsigned arg=0);
	virtual unsigned getType(){return MULTIPLE_TERMS;};
	virtual bool isEqual(unsigned type, vector<unsigned>* indexingTerms){
		if(indexingTerms!=nullptr){
			if(type==MULTIPLE_TERMS){
				if(indexingTerms->size()!=this->indexingTerms.size())
					return false;
				for(unsigned i=0;i<this->indexingTerms.size() ;++i)
					if(this->indexingTerms[i]!=(*indexingTerms)[i])
						return false;
				return true;
			}
		}
		return false;
	}
private:
	unordered_map<vector<index_object>,AtomTable,HashVectorOfTerms,HashVectorOfTerms> indexingStructure;
};

/**
 * This class implements IndexingStructure for recursive predicate and provides a general multiple terms indexing data structure
 * implemented by means of an unordered map of vectors of terms as indexing data structure (@see Atom.h)
 *
 **/
class MulplipleTermsMapHistoryVector : public IndexingStructure {
public:
	MulplipleTermsMapHistoryVector(AtomHistoryVector* table, vector<unsigned>& indexingTerm): IndexingStructure(table,indexingTerm),lastUpdateIteration(0){};
	void add(Atom* atom);
	Atom* find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification={ALL,0});
	void clear(){IndexingStructure::clear(); indexingStructure.clear();};
	virtual void update(unsigned iteration);
	virtual GeneralIterator* computeMatchIterator(Atom* templateAtom,const RuleInformation& ruleInformation,const pair<SearchType,unsigned>& searchSpecification,unsigned arg=0);
	virtual unsigned getType(){return MULTIPLE_TERMS;};
	virtual bool isEqual(unsigned type, vector<unsigned>* indexingTerms){
		if(indexingTerms!=nullptr){
			if(type==MULTIPLE_TERMS_HISTORY_VECTOR){
				if(indexingTerms->size()!=this->indexingTerms.size())
					return false;
				for(unsigned i=0;i<this->indexingTerms.size() ;++i)
					if(this->indexingTerms[i]!=(*indexingTerms)[i])
						return false;
				return true;
			}
		}
		return false;
	}
private:
	unordered_map<vector<index_object>,AtomHistoryVector,HashVectorOfTerms,HashVectorOfTerms> indexingStructure;
	unsigned lastUpdateIteration;

	inline vector<index_object> createIndexes(Atom* atom){
		vector<index_object> indices;
		indices.reserve(indexingTerms.size());
		for(auto e:indexingTerms)
			indices.push_back(atom->getTerm(e)->getIndex());
		return indices;

	}

};

/**
 **/
class FullIndexingStructure : public IndexingStructure {
public:
    FullIndexingStructure(AtomHistoryVector* table, Predicate* predicate, AtomSearcher* atomSearcher, bool recursive, unsigned indexType);
    void add(Atom* atom);
    Atom* find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification={ALL,0});
    void clear(){IndexingStructure::clear(); for(auto indexingStructure: indexingStructures) indexingStructure->clear();};
    virtual void update();
    virtual GeneralIterator* computeMatchIterator(Atom* templateAtom, const RuleInformation& ruleInformation,const pair<SearchType,unsigned>& searchSpecification,unsigned arg=0);
    virtual unsigned getType(){return SINGLE_ARG_FULL;}
    IndexingStructure* getIndexingStructure(unsigned i) {return indexingStructures[i];}
    virtual ~FullIndexingStructure(){for(auto& indexingStructure: indexingStructures){ delete indexingStructure; indexingStructure=0;}}
private:
    Predicate* predicate;
    vector<IndexingStructure*> indexingStructures;
};


/**
 * This class models a general searching strategy for a predicate's extension.
 */
class AtomSearcher {
public:
	AtomSearcher(AtomHistoryVector* table) : table(table) {resultVector.resize(ATOMS_IN_RULE,nullptr);};
	/// Given a partially ground atom, this method is meant to find all the matching atoms satisfying the variables assignment in the given atom.
	virtual void firstMatch(unsigned id,Atom *templateAtom, var_assignment& currentAssignment, Atom*& atomFound,const RuleInformation& ruleInformation, IndexingStructure* indexingStructure,unsigned arg,const vector<unsigned>& outputVariables,const pair<SearchType,unsigned>& searchSpecification,const MatchInformation& mi);
	/// Invoked after a first match iterate trough the matching atoms found one by one.
	virtual void nextMatch(unsigned id, Atom* templateAtom, var_assignment& currentAssignment, Atom*& atomFound,const RuleInformation& ruleInformation,const vector<unsigned>& outputVariables,const MatchInformation& mi);
	/// Search a given ground atom by means of the given indexing structure
	virtual Atom* findGroundAtom(Atom *atom, IndexingStructure* indexingStructure,const pair<SearchType,unsigned>& searchSpecification={ALL,0});
	/// This method checks if the two given atoms match according to the current assignment.
	/// If they match the current assignment is update accordingly.
	bool checkMatch(unsigned int id,Atom *genericAtom, Atom *templateAtom, var_assignment& currentAssignment,const RuleInformation& ruleInformation,const vector<unsigned>& outputVariables,const MatchInformation& mi);
	/// Match a function with given id of term, compare the constant term and put in binds
	/// a value of the variable term present in termToMatch
	/// Return true if constant term are equal, else false
	bool matchTerm(Term *genericTerm, Term *termToMatch, var_assignment& varAssignment,vector<index_object>& addedVariables,const RuleInformation& ruleInformation);
	bool matchTermFunctional(Term *genericTerm, Term *termToMatch, var_assignment& varAssignment,vector<index_object>& addedVariables,const RuleInformation& ruleInformation);

	/// This method given an iterator increases it in order to find matching atoms with the given atom
	/// according to the current assignment.
	bool computeMatch(unsigned int id,GeneralIterator* currentMatch, Atom *templateAtom, var_assignment& currentAssignment, Atom*& atomFound,const RuleInformation& ruleInformation,const vector<unsigned>& outputVariables,const MatchInformation& mi);

	/// Erase the indexing structures
	inline virtual void clear(){indexingStructures.clear();}

	///Set the size of the result vector
	inline void setSizeResultVector(unsigned int size){	if(size>resultVector.size()) resultVector.resize(size,nullptr);outputVariablesValues.resize(size);}

	///Function for evaluation of builtin related with the current matching atom
	static bool evaluateFastBuiltin(const RuleInformation& ruleInformation, index_object index,	var_assignment& varAssignment, Term* genericTerm);

	///Check if the output variable is already evaluated
	bool checkOutputVariables(const vector<unsigned>& outputVariables,unsigned int id, Atom* genericAtom);

	///Add a specific indexing structure for the table
	void addIndexingStructure(IndexingStructure* indexingStructure){indexingStructures.push_back(indexingStructure);};
	///If present, returns an indexing structure for the table of the specified type and with the specified indexing terms
	IndexingStructure* getIndexingStructure(unsigned type, vector<unsigned>* indexingTerms);
	///If present, returns an indexing structure for the table of any type but with the specified indexing terms
	IndexingStructure* getIndexingStructure(vector<unsigned>* indexingTerms);

	///Return a default hash indexing structure
	IndexingStructure* getDefaultIndexingStructure(){
		if(indexingStructures.size()==0)
			indexingStructures.push_back(new UnorderedSet(table));

		for(auto indexingStructure: indexingStructures)
			if(indexingStructure->getType()==HASHSET || indexingStructure->getType()==MAP)
				return indexingStructure;

		return indexingStructures.front();
	}

	///Printer method. Useful mainly for debug purpose.
	virtual void print(ostream& stream=cout){for(auto atom:*table) atom->print(stream);}

	virtual ~AtomSearcher();

protected:
	///The basic data-structure that collects the atoms
	AtomHistoryVector* table;

	/// This maps stores the calls to the firstMatch method.
	/// Indeed, for each call it stores a pair with the counter and the iterator to the next matching atoms.
	vector<GeneralIterator*> resultVector;

	//This vector stores the created indexing structures for the table
	vector<IndexingStructure*> indexingStructures;

	vector<unordered_set<vector<index_object>,HashVectorOfTerms,HashVectorOfTerms>> outputVariablesValues;

};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_TABLE_INDEXINGSTRUCTURE_H_ */
