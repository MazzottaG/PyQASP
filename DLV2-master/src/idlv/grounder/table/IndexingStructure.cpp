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
 * IndexingStructure.cpp
 *
 *  Created on: Jan 15, 2016
 *      Author: jessica
 */

#include "IndexingStructure.h"
#include "PredicateExtension.h"
#include "../../../common/Options.h"

namespace DLV2 {
namespace grounder {

/******************************************************* ATOM SEARCHER ***************************************************/

bool AtomSearcher::checkOutputVariables(const vector<unsigned>& outputVariables, unsigned int id,  Atom* genericAtom) {
	vector<unsigned> outputVariablesTerms;
	outputVariablesTerms.reserve(outputVariables.size());
	for (auto i : outputVariables)
		outputVariablesTerms.push_back(genericAtom->getTerm(i)->getIndex());
	if (!outputVariablesValues[id].insert(outputVariablesTerms).second)
		return true;
	return false;
}

bool AtomSearcher::checkMatch(unsigned int id,Atom *genericAtom, Atom *templateAtom, var_assignment& currentAssignment,const RuleInformation& ruleInformation,const vector<unsigned>& outputVariables,const MatchInformation& mi){
	// New semi fast cache match for atom. First check  if the bound term is equal. After if there are fast builtin to check add in the current assignment
	// the variable binded from the atom used in the builtin, evaluate the builtin and after restore the assignment. After the bound and builtin add the bind variable
	// in the assignment and at the end check the other terms (functional or arith term). For the other term is used the old match, and if one term fail restore the assignment
	// and return false. If all the match of the term is matched return true

	vector<Term*>& genericTerms=genericAtom->getTerms();
	vector<Term*>& templateTerms=templateAtom->getTerms();

	for(auto bound:mi.bound)
		if(genericTerms[bound]!=templateTerms[bound])return false;

	for(unsigned i=0;i<mi.boundPreds.size();++i)
		for(auto pair:mi.boundPreds[i]){
			if(!PredicateExtTable::getInstance()->getPredicateExt(pair.first)->getPredicateInformation()->isPresent(pair.second,genericTerms[i])){
				return false;
			}
		}

	if(!mi.dictionaryIntersection.empty()){
		for(auto index:mi.dictionaryIntersection)
			if(!ruleInformation.countInDictionaryIntersection(templateTerms[index]->getLocalVariableIndex(),genericTerms[index]))
				return false;
	}

	if(!mi.builtin.empty()){
		for(auto var:mi.varUsedInBuiltin)
			currentAssignment[var.second]=genericTerms[var.first];

		bool evaluation=true;
		for(auto builtin:mi.builtin){
			evaluation = builtin->groundAndEvaluate(currentAssignment);
			if (!evaluation)break;
		}

		for(auto var:mi.varUsedInBuiltin)
			currentAssignment[var.second]=nullptr;

		if(!evaluation)return false;
	}

	for(auto bind:mi.bind)
		currentAssignment[bind.second]=genericTerms[bind.first];

	if(!mi.other.empty()){
		vector<unsigned> addedVar;
		addedVar.reserve(currentAssignment.size());
		bool fail=false;
		for(auto oth:mi.other){
			if(!matchTermFunctional(genericTerms[oth],templateTerms[oth],currentAssignment,addedVar,ruleInformation)){
				fail=true;
				break;
			}
		}
		if(!fail && !outputVariables.empty()){
			if(checkOutputVariables(outputVariables, id, genericAtom))
				fail=true;
		}

		if(fail){
			for(auto var:addedVar){currentAssignment[var]=nullptr;}
			return false;
		}
	}else if(!outputVariables.empty() && checkOutputVariables(outputVariables, id, genericAtom))
		return false;


	return true;
}

bool AtomSearcher::matchTermFunctional(Term *generic, Term *toMatch, var_assignment& varAssignment,vector<index_object>& addedVariables,const RuleInformation& ruleInformation){
	//TO-OPTIMIZE
	unsigned i=0;
	vector<pair<Term*,Term*>> termsToProcess;
	termsToProcess.push_back({generic,toMatch});
	while(i<termsToProcess.size()){
		Term* genericTerm=termsToProcess[i].first;
		Term* termToMatch=termsToProcess[i].second;
		TermType genericTermType=genericTerm->getType();
		TermType termToMatchType=termToMatch->getType();
		if(termToMatchType==TermType::NUMERIC_CONSTANT || termToMatchType==TermType::STRING_CONSTANT || termToMatchType==TermType::SYMBOLIC_CONSTANT)
			{if(genericTerm!=termToMatch)return false;}
		else if (termToMatchType==TermType::VARIABLE) {
			index_object index=termToMatch->getLocalVariableIndex();
			if(ruleInformation.isCreatedDictionaryIntersection(index) && !ruleInformation.countInDictionaryIntersection(index,genericTerm)){
				return false;
			}
			Term* term=varAssignment[index];
			if(term!=nullptr)
				{
					if( term != genericTerm){return false;}
				}
			else{

				if(ruleInformation.isBounderBuiltin(index)){
					if(!evaluateFastBuiltin(ruleInformation, index, varAssignment, genericTerm))
						return false;
				}

				varAssignment[index]=genericTerm;
				addedVariables.push_back(index);
			}
		}else if(termToMatchType==TermType::ARITH){
			Term *new_term=termToMatch->substitute(varAssignment);
			assert_msg(new_term->isGround(),"Arith term not safe");
			termToMatch=new_term->calculate();
			if(genericTerm!=termToMatch)return false;
		}
		else if(termToMatchType==TermType::FUNCTION){
			if(genericTermType!=TermType::FUNCTION) return false;
			if(termToMatch->getName()!=genericTerm->getName())return false;
			if(termToMatch->getTermsSize() != genericTerm->getTermsSize())return false;
			for(unsigned int i=0;i<genericTerm->getTermsSize();++i)
				termsToProcess.push_back({genericTerm->getTerm(i),termToMatch->getTerm(i)});

		}
                else if(termToMatchType==TermType::LIST_HEAD_TAIL){
                        if(genericTermType!=TermType::LIST)return false;
                        if(genericTerm->getTermsSize()==0)return false;
                        auto headTailMatch=termToMatch->getHeadTailList();
                        auto headTailGeneric=genericTerm->getHeadTailList();
                        termsToProcess.push_back({headTailGeneric.first,headTailMatch.first});
                        termsToProcess.push_back({headTailGeneric.second,headTailMatch.second});
                }
                else if(termToMatchType==TermType::LIST){
                        if(genericTermType!=TermType::LIST)return false;
                        if(termToMatch->getTermsSize() != genericTerm->getTermsSize())return false;
			for(unsigned int i=0;i<genericTerm->getTermsSize();++i)
				termsToProcess.push_back({genericTerm->getTerm(i),termToMatch->getTerm(i)});
                }                
		i++;
	}
	return true;
}

/*
 * For each builtin atom that the bind variable(with local index=index) is the last variable evaluated in the builtin, check if the builtin is satisfied. If the
 * evaluation of builtin is false the match of the term, then the match of the atom fail
 */
bool AtomSearcher::evaluateFastBuiltin(const RuleInformation& ruleInformation,index_object index, var_assignment& varAssignment, Term* genericTerm) {
	for (auto builtin : ruleInformation.getBounderBuiltin(index)) {
		varAssignment[index] = genericTerm;
		bool evaluation = builtin->groundAndEvaluate(varAssignment);
		varAssignment[index] = nullptr;
		if (!evaluation) {
			return false;
		}
	}
	return true;
}


void AtomSearcher::firstMatch(unsigned id, Atom *templateAtom, var_assignment& currentAssignment, Atom*& atomFound,const RuleInformation& ruleInformation,IndexingStructure* indexingStructure,unsigned arg,const vector<unsigned>& outputVariables,const pair<SearchType,unsigned>& searchSpecification,const MatchInformation& mi) {
	GeneralIterator* currentMatch=indexingStructure->computeMatchIterator(templateAtom,ruleInformation,searchSpecification,arg);
	if(id<outputVariablesValues.size())
		outputVariablesValues[id].clear();
	if(computeMatch(id,currentMatch,templateAtom,currentAssignment,atomFound,ruleInformation,outputVariables,mi)){
		delete resultVector[id];
		resultVector[id]=currentMatch;
		return;
	}
	delete currentMatch;
}

bool AtomSearcher::computeMatch(unsigned int id,GeneralIterator* currentMatch, Atom *templateAtom, var_assignment& currentAssignment, Atom*& atomFound,const RuleInformation& ruleInformation,const vector<unsigned>& outputVariables,const MatchInformation& mi){
	for(;!currentMatch->isDone();currentMatch->next()){
		Atom* atom=currentMatch->currentItem();
		if (checkMatch(id,atom,templateAtom,currentAssignment,ruleInformation,outputVariables,mi)){
			atomFound=atom;
			return true;
		}
	}
	atomFound=nullptr;
	return false;
}

void AtomSearcher::nextMatch(unsigned int id, Atom *templateAtom, var_assignment& currentAssignment, Atom*& atomFound,const RuleInformation& ruleInformation,const vector<unsigned>& outputVariables,const MatchInformation& mi) {
	GeneralIterator* currentMatch=resultVector[id];
	currentMatch->next();
	computeMatch(id,currentMatch,templateAtom,currentAssignment,atomFound,ruleInformation,outputVariables,mi);

}

Atom* AtomSearcher::findGroundAtom(Atom *atom,IndexingStructure* indexingStructure,const pair<SearchType,unsigned>& searchSpecification){
	return indexingStructure->find(atom,searchSpecification);
}

IndexingStructure* AtomSearcher::getIndexingStructure(unsigned type, vector<unsigned>* indexingTerms) {
	for(auto indexingStruct:indexingStructures){
		if(indexingStruct->getType()==type){
			if(indexingTerms==nullptr)
				return indexingStruct;
			else{
				bool ok=true;
				vector<unsigned> indexingStructIndexingTerms=indexingStruct->getIndexingTerms();
				if(indexingStructIndexingTerms.size()!=indexingTerms->size())
					ok=false;
				for(unsigned i=0;i<indexingTerms->size()&&ok;++i)
					if(indexingStructIndexingTerms[i]!=(*indexingTerms)[i])
						ok=false;
				if(ok) return indexingStruct;
			}
		}
	}
	return nullptr;
}

IndexingStructure* AtomSearcher::getIndexingStructure(vector<unsigned>* indexingTerms) {
	for(auto indexingStruct:indexingStructures){
		if(indexingTerms==nullptr)
			return indexingStruct;
		else{
			vector<unsigned> indexingStructIndexingTerms=indexingStruct->getIndexingTerms();
			if(indexingStructIndexingTerms.size()!=indexingTerms->size())
				return nullptr;
			for(unsigned i=0;i<indexingTerms->size();++i){
				if(indexingStructIndexingTerms[i]!=(*indexingTerms)[i])
					return nullptr;
			}
			return indexingStruct;
		}
	}
	return nullptr;
}

AtomSearcher::~AtomSearcher() {
	for(auto& it:resultVector)
		delete it;

	for(auto& indexingStructure: indexingStructures){
		delete indexingStructure;
		indexingStructure=0;
	}
};

/******************************************************** Indexing Structure (Vector) **************************************************/

Atom* IndexingStructure::find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification) {
	for(auto atom1:(*table)){
		if(*atom1==*atom){
			return atom1;
		}
	}
	return nullptr;
}

/******************************************************** Unordered Set **************************************************/

Atom* UnorderedSet::find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification) {
	if(lastUpdate<table->size())
		update();

	auto atomFound_it=indexingStructure.find(atom);
	if(atomFound_it!=indexingStructure.end())
		return *atomFound_it;
	return nullptr;
}

void UnorderedSet::update() {
	for (;lastUpdate<table->size();++lastUpdate)
		indexingStructure.insert((*table)[lastUpdate]);
}

/******************************************************** History Unordered Set **************************************************/

Atom* HistoryUnorderedSet::find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification) {
	if(updateIteration!=table->getCurrentIteration())
		update();
	Atom *atomFound=nullptr;
	if(searchSpecification.first==NEW || searchSpecification.first==ALL){
		auto atomFound_it=deltaIndexingStructure.find(atom);
		if(atomFound_it!=deltaIndexingStructure.end())
			return *atomFound_it;
	}
	if(searchSpecification.first==OLD || searchSpecification.first==ALL){
		auto atomFound_it=nfIndexingStructure.find(atom);
		if(atomFound_it!=nfIndexingStructure.end())
			return *atomFound_it;
	}
	return atomFound;
}

void HistoryUnorderedSet::update() {
	for(auto atom:deltaIndexingStructure)
		nfIndexingStructure.insert(atom);
	deltaIndexingStructure.clear();
	unsigned deltaIt=table->getDeltaIndexIteration();
	unsigned nfIt=table->getNFIndexIteration();
	for (;lastUpdate<deltaIt;++lastUpdate) {
		if(lastUpdate<nfIt)
			nfIndexingStructure.insert((*table)[lastUpdate]);
		else
			deltaIndexingStructure.insert((*table)[lastUpdate]);
	}
	updateIteration=table->getCurrentIteration();
}


/******************************************************** Unordered Map of Unordered Set **************************************************/

void UnorderedMapOfUnorderedSet::add(Atom* atom) {
	unsigned i=indexingTerms[0];
	if(lastUpdate==table->size()){
		index_object termIndex=atom->getTerm(i)->getIndex();
		if(indexingStructure.count(termIndex))
			indexingStructure[termIndex].insert(atom);
		else{
			indexingStructure.emplace(termIndex,AtomTable({atom}));
		}
	}
}

Atom* UnorderedMapOfUnorderedSet::find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification) {
	if(lastUpdate<table->size())
		update();

	unsigned i=indexingTerms[0];
	index_object term = atom->getTerm(i)->getIndex();

	auto atomFound_it=indexingStructure[term].find(atom);
	if(atomFound_it!=indexingStructure[term].end()){
		return *atomFound_it;
	}
	return nullptr;
}

void UnorderedMapOfUnorderedSet::update() {
	unsigned i=indexingTerms[0];
	for (;lastUpdate<table->size();++lastUpdate) {
		Atom *a=(*table)[lastUpdate];
		index_object termIndex=a->getTerm(i)->getIndex();
		if(!indexingStructure.count(termIndex)){
			AtomTable values;
//			values.reserve(table->size()/PredicateExtTable::getInstance()->getPredicateExt(predicate)->getPredicateInformation()->getSelectivity(indexingTerm));
			values.insert(a);
			indexingStructure.insert({termIndex,values});
		}
		else
			indexingStructure[termIndex].insert(a);
	}
}

GeneralIterator* UnorderedMapOfUnorderedSet::computeMatchIterator(Atom* templateAtom, const RuleInformation& ruleInformation,const pair<SearchType,unsigned>& searchSpecification,unsigned arg) {
	if(lastUpdate<table->size())
		update();

	int indexingTerm=indexingTerms[0];
	GeneralIterator* currentMatch;

	index_object term = templateAtom->getTerm(indexingTerm)->getIndex();
	AtomTable* matchingTable=&indexingStructure[term];
	currentMatch=new UnorderedSetIterator(matchingTable->begin(),matchingTable->end());
	return currentMatch;

}

/******************************************************** Unordered Map of Vector **************************************************/

void UnorderedMapOfVector::add(Atom* atom) {
	unsigned i=indexingTerms[0];
	if(lastUpdate==table->size()){
		index_object termIndex=atom->getTerm(i)->getIndex();
		if(indexingStructure.count(termIndex))
			indexingStructure[termIndex].push_back(atom);
		else{
			indexingStructure.emplace(termIndex,vector<Atom*>({atom}));
		}
	}
}

Atom* UnorderedMapOfVector::find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification) {
	if(lastUpdate<table->size())
		update();

	unsigned i=indexingTerms[0];
	index_object term = atom->getTerm(i)->getIndex();
	vector<Atom*>* matchingTable=&indexingStructure[term];

	for(auto atom1:(*matchingTable)){
		if(*atom1==*atom){
			return atom1;
		}
	}
	return nullptr;
}

void UnorderedMapOfVector::update() {
	unsigned i=indexingTerms[0];
	for (;lastUpdate<table->size();++lastUpdate) {
		Atom *a=(*table)[lastUpdate];
		index_object termIndex=a->getTerm(i)->getIndex();
		if(!indexingStructure.count(termIndex)){
			vector<Atom*> values;
//			values.reserve(table->size()/PredicateExtTable::getInstance()->getPredicateExt(predicate)->getPredicateInformation()->getSelectivity(indexingTerm));
			values.push_back(a);
			indexingStructure.insert({termIndex,values});
		}
		else
			indexingStructure[termIndex].push_back(a);
	}
}

GeneralIterator* UnorderedMapOfVector::computeMatchIterator(Atom* templateAtom, const RuleInformation& ruleInformation,const pair<SearchType,unsigned>& searchSpecification,unsigned arg) {
	if(lastUpdate<table->size())
		update();

	int indexingTerm=indexingTerms[0];
	GeneralIterator* currentMatch;

	index_object term = templateAtom->getTerm(indexingTerm)->getIndex();
	vector<Atom*>* matchingTable=&indexingStructure[term];
	currentMatch=new VectorIterator(matchingTable->begin(),matchingTable->end());

	return currentMatch;
}

/******************************************************** Unordered Map of History Vector **************************************************/

//TODO for full index version
void UnorderedMapOfHistoryVector::add(Atom* atom) {

}

Atom* UnorderedMapOfHistoryVector::find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification) {
	if(lastUpdateIteration!=searchSpecification.second)
		update(searchSpecification.second);

	unsigned i=indexingTerms[0];
	index_object term = atom->getTerm(i)->getIndex();
	auto find_it=indexingStructure.find(term);
	if(find_it==indexingStructure.end())return nullptr;
	AtomHistoryVector& matchingTable=find_it->second;

	auto it=matchingTable.getElements(searchSpecification.first,searchSpecification.second);
	for(unsigned i=it.first;i<it.second;i++){
		if(*(matchingTable[i])==*atom){
			return matchingTable[i];
		}
	}
	return nullptr;
}

void UnorderedMapOfHistoryVector::update(unsigned iteration) {
	//First update the vector in the index table
	for(auto& element:indexingStructure)
		element.second.updateIndices(iteration);
	//After update the history in the indexingStructure add the atom after the
	// lastUpdate index in indexingStructure
	unsigned i=indexingTerms[0];
	unsigned deltaIt=table->getDeltaIndexIteration();
	unsigned nfIt=table->getNFIndexIteration();
	for (;lastUpdate<table->size();++lastUpdate) {
		Atom *a=(*table)[lastUpdate];
		index_object termIndex=a->getTerm(i)->getIndex();
		if(!indexingStructure.count(termIndex)){
			AtomHistoryVector values(iteration);
//			values.reserve(table->size()/PredicateExtTable::getInstance()->getPredicateExt(predicate)->getPredicateInformation()->getSelectivity(indexingTerm));
			indexingStructure.insert({termIndex,values});
		}
		auto& values=indexingStructure[termIndex];
		if(lastUpdate<nfIt)
			values.push_back_nf(a);
		else if(lastUpdate<deltaIt)
			values.push_back_delta(a);
		else
			values.push_back(a);
	}
	lastUpdateIteration=iteration;
}


GeneralIterator* UnorderedMapOfHistoryVector::computeMatchIterator(Atom* templateAtom, const RuleInformation& ruleInformation,const pair<SearchType,unsigned>& searchSpecification,unsigned arg) {
	if(lastUpdateIteration!=searchSpecification.second)
		update(searchSpecification.second);

	trace_action_tag(recursion,2,cerr<<"FIND ";templateAtom->print(cerr);cerr<<" IN "<<searchSpecification.first<<" IT "<<searchSpecification.second;cerr<<endl;);
	GeneralIterator* currentMatch;
	int indexingTerm=indexingTerms[0];
	index_object term = templateAtom->getTerm(indexingTerm)->getIndex();
	auto find_it=indexingStructure.find(term);
	if(find_it==indexingStructure.end())return new VectorIteratorIndex(table->size(),table->size(),table);
	AtomHistoryVector* matchingTable=&find_it->second;
	auto it=matchingTable->getElements(searchSpecification.first,searchSpecification.second);
	trace_action_tag(recursion,2,cerr<<"MATCH TABLE IT "<<matchingTable->getCurrentIteration()<<" DELTA "<<matchingTable->getDeltaIndexIteration()<<" NF "<<matchingTable->getNFIndexIteration();cerr<<endl;);
	trace_action_tag(recursion,2,cerr<<"RESULT SEARCH "<<it.first<<" "<<it.second;cerr<<endl;);
	currentMatch=new VectorIteratorIndex(it.first,it.second,matchingTable);

	return currentMatch;
}

/******************************************************** Unordered Unordered Map Of Pair HistoryVector **************************************************/

//TODO for full index version
void UnorderedMapOfPairHistoryVector::add(Atom* atom) {

}

Atom* UnorderedMapOfPairHistoryVector::find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification) {
	if(lastUpdateIteration!=searchSpecification.second)
		update(searchSpecification.second);

	unsigned i=indexingTerms[0];
	unsigned next=indexingTerms[1];

	index_object term = atom->getTerm(i)->getIndex();
	index_object nextTerm=atom->getTerm(next)->getIndex();


	auto find_it=indexingStructure.find({term,nextTerm});
	if(find_it==indexingStructure.end())return nullptr;
	AtomHistoryVector& matchingTable=find_it->second;

	auto it=matchingTable.getElements(searchSpecification.first,searchSpecification.second);
	for(unsigned i=it.first;i<it.second;i++){
		if(*(matchingTable[i])==*atom){
			return matchingTable[i];
		}
	}
	return nullptr;
}

void UnorderedMapOfPairHistoryVector::update(unsigned iteration) {
	//First update the vector in the index table
	for(auto& element:indexingStructure)
		element.second.updateIndices(iteration);

	unsigned i=indexingTerms[0];
	unsigned next=indexingTerms[1];
	unsigned deltaIt=table->getDeltaIndexIteration();
	unsigned nfIt=table->getNFIndexIteration();

	for (;lastUpdate<table->size();++lastUpdate) {
		Atom *a=(*table)[lastUpdate];
		index_object termIndex=a->getTerm(i)->getIndex();
		index_object nextTermIndex=a->getTerm(next)->getIndex();
		if(!indexingStructure.count({termIndex,nextTermIndex})){
			AtomHistoryVector values(iteration);
//			values.reserve(table->size()/PredicateExtTable::getInstance()->getPredicateExt(predicate)->getPredicateInformation()->getSelectivity(indexingTerm));
			indexingStructure.insert({{termIndex,nextTermIndex},values});
		}
		auto& values=indexingStructure[{termIndex,nextTermIndex}];
		if(lastUpdate<nfIt)
			values.push_back_nf(a);
		else if(lastUpdate<deltaIt)
			values.push_back_delta(a);
		else
			values.push_back(a);
	}
	lastUpdateIteration=iteration;
}


GeneralIterator* UnorderedMapOfPairHistoryVector::computeMatchIterator(Atom* templateAtom, const RuleInformation& ruleInformation,const pair<SearchType,unsigned>& searchSpecification,unsigned arg) {
	if(lastUpdateIteration!=searchSpecification.second)
		update(searchSpecification.second);

	GeneralIterator* currentMatch;
	unsigned i=indexingTerms[0];
	unsigned next=indexingTerms[1];

	index_object term = templateAtom->getTerm(i)->getIndex();
	index_object nextTerm=templateAtom->getTerm(next)->getIndex();

	auto find_it=indexingStructure.find({term,nextTerm});
	if(find_it==indexingStructure.end())return new VectorIteratorIndex(table->size(),table->size(),table);
	AtomHistoryVector& matchingTable=find_it->second;
	auto it=matchingTable.getElements(searchSpecification.first,searchSpecification.second);
	currentMatch=new VectorIteratorIndex(it.first,it.second,&matchingTable);

	return currentMatch;
}


/******************************************************** Unordered Multi Map **************************************************/

void UnorderedMultiMap::add(Atom* atom) {
	unsigned i=indexingTerms[0];
	if(lastUpdate==table->size()){
		index_object termIndex=atom->getTerm(i)->getIndex();
		indexingStructure.insert({termIndex,atom});
	}
}

Atom* UnorderedMultiMap::find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification) {
	if(lastUpdate<table->size())
		update();

	unsigned i=indexingTerms[0];
	index_object term = atom->getTerm(i)->getIndex();
	auto matchingTable=indexingStructure.equal_range(term);

	for(auto it=matchingTable.first;it!=matchingTable.second;++it){
		if(*((*it).second)==*atom){
			return (*it).second;
		}
	}
	return nullptr;
}

void UnorderedMultiMap::update() {
	unsigned i=indexingTerms[0];
	for (;lastUpdate<table->size();++lastUpdate) {
		Atom *a=(*table)[lastUpdate];
		index_object termIndex=a->getTerm(i)->getIndex();
		indexingStructure.insert({termIndex,a});
	}
}

GeneralIterator* UnorderedMultiMap::computeMatchIterator(Atom* templateAtom, const RuleInformation& ruleInformation,const pair<SearchType,unsigned>& searchSpecification,unsigned arg) {
	if(lastUpdate<table->size())
		update();

	int indexingTerm=indexingTerms[0];
	GeneralIterator* currentMatch;

	index_object term = templateAtom->getTerm(indexingTerm)->getIndex();
	auto matchingTable=indexingStructure.equal_range(term);
	currentMatch=new UnorderedMultiMapIterator(matchingTable.first,matchingTable.second);

	return currentMatch;
}

/******************************************************** Unordered Map of Unordered Multi Map **************************************************/

void UnorderedMapOfUnorderedMultimap::add(Atom* atom) {
	unsigned i=indexingTerms[0];
	unsigned next=indexingTerms[1];
	if(lastUpdate<table->size()){
		index_object termIndex=atom->getTerm(i)->getIndex();
		int nextTermIndex=-1;
		if(next>0)
			nextTermIndex=atom->getTerm(next)->getIndex();
		if(indexingStructure.count(termIndex)){
			if(nextTermIndex>-1)
				indexingStructure[termIndex].insert({nextTermIndex,atom});
			else
				indexingStructure[termIndex].insert({termIndex,atom});
		}
		else{
			Multimap_Atom values;
			if(nextTermIndex>-1)
				values.insert({nextTermIndex,atom});
			else
				values.insert({termIndex,atom});
			indexingStructure.insert({termIndex,values});
		}
	}
}

Atom* UnorderedMapOfUnorderedMultimap::find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification) {
	if(lastUpdate<table->size())
		update();

	unsigned i=indexingTerms[0];
	unsigned next=indexingTerms[1];
	index_object term = atom->getTerm(i)->getIndex();
	Multimap_Atom::iterator start;
	Multimap_Atom::iterator end;

	if(next>0){
		index_object nextTerm=atom->getTerm(next)->getIndex();
		auto pair=indexingStructure[term].equal_range(nextTerm);
		start=pair.first;
		end=pair.second;
	}
	else{
		start=indexingStructure[term].begin();
		end=indexingStructure[term].end();
	}

	for(auto it=start;it!=end;++it){
		if(*(it->second)==*atom)
			return it->second;
	}
	return nullptr;

}

void UnorderedMapOfUnorderedMultimap::update() {
	if(table->empty()) return;
	unsigned indexingTerm=indexingTerms[0];
	unsigned next=indexingTerms[1];
	for (;lastUpdate<table->size();++lastUpdate) {
		Atom* a=(*table)[lastUpdate];
		index_object termIndex=a->getTerm(indexingTerm)->getIndex();
		int nextTermIndex=-1;
		if(next>0)
			nextTermIndex=a->getTerm(next)->getIndex();
		if(indexingStructure.count(termIndex)){
			if(nextTermIndex>-1)
				indexingStructure[termIndex].insert({nextTermIndex,a});
			else
				indexingStructure[termIndex].insert({termIndex,a});
		}
		else{
			Multimap_Atom values;
			if(nextTermIndex>-1)
				values.insert({nextTermIndex,a});
			else
				values.insert({termIndex,a});
			indexingStructure.insert({termIndex,values});
		}
	}
}

GeneralIterator* UnorderedMapOfUnorderedMultimap::computeMatchIterator(Atom* templateAtom, const RuleInformation& ruleInformation,const pair<SearchType,unsigned>& searchSpecification,unsigned arg) {
	if(lastUpdate<table->size())
		update();

	unsigned indexingTerm=indexingTerms[0];
	unsigned next=indexingTerms[1];

	index_object term = templateAtom->getTerm(indexingTerm)->getIndex();
	Multimap_Atom::iterator start;
	Multimap_Atom::iterator end;

	Term* nextTerm=templateAtom->getTerm(next);
	if(next>0 && nextTerm->isGround()){ //FIXME avoid to call isGround
		index_object nextTermIndex=nextTerm->getIndex();
		auto pair=indexingStructure[term].equal_range(nextTermIndex);
		start=pair.first;
		end=pair.second;
	}
	else{
		start=indexingStructure[term].begin();
		end=indexingStructure[term].end();
	}
	return new UnorderedMultiMapIterator(start,end);
}

/******************************************************** Unordered Map of Pair **************************************************/

void UnorderedMapOfPair::add(Atom* atom) {
	unsigned i=indexingTerms[0];
	unsigned next=indexingTerms[1];
	if(lastUpdate<table->size()){
		index_object termIndex=atom->getTerm(i)->getIndex();
		index_object nextTermIndex=atom->getTerm(next)->getIndex();
		pair<index_object,index_object> p={termIndex,nextTermIndex};
		if(indexingStructure.count(p)){
			indexingStructure[p].insert(atom);
		}
		else{
			AtomTable values;
			values.insert(atom);
			indexingStructure.insert({p,values});
		}
	}
}

Atom* UnorderedMapOfPair::find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification) {
	if(lastUpdate<table->size())
		update();
	unsigned i=indexingTerms[0];
	unsigned next=indexingTerms[1];
	index_object termIndex=atom->getTerm(i)->getIndex();
	index_object nextTermIndex=atom->getTerm(next)->getIndex();
	pair<index_object,index_object> p={termIndex,nextTermIndex};
	auto it=indexingStructure[p].find(atom);
	if(it==indexingStructure[p].end())
		return nullptr;
	return *it;
}

void UnorderedMapOfPair::update() {
	if(table->empty()) return;
	unsigned indexingTerm=indexingTerms[0];
	unsigned next=indexingTerms[1];
	for (;lastUpdate<table->size();++lastUpdate) {
		Atom* atom=(*table)[lastUpdate];
		index_object termIndex=atom->getTerm(indexingTerm)->getIndex();
		index_object nextTermIndex=atom->getTerm(next)->getIndex();
		pair<index_object,index_object> p={termIndex,nextTermIndex};
		if(indexingStructure.count(p)){
			indexingStructure[p].insert(atom);
		}
		else{
			AtomTable values;
			values.insert(atom);
			indexingStructure.insert({p,values});
		}
	}
}

GeneralIterator* UnorderedMapOfPair::computeMatchIterator(Atom* templateAtom,
		const RuleInformation& ruleInformation,
		const pair<SearchType, unsigned>& searchSpecification, unsigned arg) {
	if(lastUpdate<table->size())
		update();
	unsigned i=indexingTerms[0];
	unsigned next=indexingTerms[1];
	index_object termIndex=templateAtom->getTerm(i)->getIndex();
	index_object nextTermIndex=templateAtom->getTerm(next)->getIndex();
	pair<index_object,index_object> p={termIndex,nextTermIndex};
	return new UnorderedSetIterator(indexingStructure[p].begin(),indexingStructure[p].end());
}

/*************************************************************** Unordered Map of Vectors *************************************************************************/

void MulplipleTermsMap::add(Atom* atom) {
	vector<index_object> indices;
	indices.reserve(indexingTerms.size());
	for(auto e:indexingTerms)
		indices.push_back(atom->getTerm(e)->getIndex());
	if(lastUpdate<table->size()){
		if(indexingStructure.count(indices)){
			indexingStructure[indices].insert(atom);
		}
		else{
			AtomTable values;
			values.insert(atom);
			indexingStructure.insert({indices,values});
		}
	}
}

Atom* MulplipleTermsMap::find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification) {
	if(lastUpdate<table->size())
		update();
	vector<index_object> indices;
	indices.reserve(indexingTerms.size());
	for(auto e:indexingTerms)
		indices.push_back(atom->getTerm(e)->getIndex());
	auto it=indexingStructure[indices].find(atom);
	if(it==indexingStructure[indices].end())
		return nullptr;
	return *it;
}

void MulplipleTermsMap::update() {
	if(table->empty()) return;
	for (;lastUpdate<table->size();++lastUpdate) {
		Atom* atom=(*table)[lastUpdate];
		vector<index_object> indices;
		indices.reserve(indexingTerms.size());
		for(auto e:indexingTerms)
			indices.push_back(atom->getTerm(e)->getIndex());
		if(indexingStructure.count(indices)){
			indexingStructure[indices].insert(atom);
		}
		else{
			AtomTable values;
			values.insert(atom);
			indexingStructure.insert({indices,values});
		}
	}
}

GeneralIterator* MulplipleTermsMap::computeMatchIterator(Atom* templateAtom,
		const RuleInformation& ruleInformation,
		const pair<SearchType, unsigned>& searchSpecification, unsigned arg) {
	if(lastUpdate<table->size())
		update();
	vector<index_object> indices;
	indices.reserve(indexingTerms.size());
	for(auto e:indexingTerms)
		indices.push_back(templateAtom->getTerm(e)->getIndex());
	return new UnorderedSetIterator(indexingStructure[indices].begin(),indexingStructure[indices].end());
}

/*************************************************************** Unordered Map of Vectors - History Vector *************************************************************************/

void MulplipleTermsMapHistoryVector::add(Atom* atom) {

}

Atom* MulplipleTermsMapHistoryVector::find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification) {
	if(lastUpdateIteration!=searchSpecification.second)
		update(searchSpecification.second);

	vector<index_object> indices=createIndexes(atom);
	auto find_it=indexingStructure.find(indices);
	if(find_it==indexingStructure.end())return nullptr;
	auto& matchingTable=find_it->second;
	auto it=matchingTable.getElements(searchSpecification.first,searchSpecification.second);
	for(unsigned i=it.first;i<it.second;i++){
		if(*(matchingTable[i])==*atom){
			return matchingTable[i];
		}
	}
	return nullptr;
}

void MulplipleTermsMapHistoryVector::update(unsigned iteration) {
	//First update the vector in the index table
	for(auto& element:indexingStructure)
		element.second.updateIndices(iteration);

	unsigned deltaIt=table->getDeltaIndexIteration();
	unsigned nfIt=table->getNFIndexIteration();

	for (;lastUpdate<table->size();++lastUpdate) {
		Atom *a=(*table)[lastUpdate];
		vector<index_object> indices=createIndexes(a);
		if(!indexingStructure.count(indices)){
			AtomHistoryVector values(iteration);
//			values.reserve(table->size()/PredicateExtTable::getInstance()->getPredicateExt(predicate)->getPredicateInformation()->getSelectivity(indexingTerm));
			indexingStructure.insert({indices,values});
		}
		auto& values=indexingStructure[indices];
		if(lastUpdate<nfIt)
			values.push_back_nf(a);
		else if(lastUpdate<deltaIt)
			values.push_back_delta(a);
		else
			values.push_back(a);
	}
	lastUpdateIteration=iteration;
}

GeneralIterator* MulplipleTermsMapHistoryVector::computeMatchIterator(Atom* templateAtom,
		const RuleInformation& ruleInformation,
		const pair<SearchType, unsigned>& searchSpecification, unsigned arg) {
	if(lastUpdateIteration!=searchSpecification.second)
		update(searchSpecification.second);
	vector<index_object> indices=createIndexes(templateAtom);

	auto find_it = indexingStructure.find(indices);
	if(find_it==indexingStructure.end())return new VectorIteratorIndex(table->size(),table->size(),table);
	AtomHistoryVector& matchingTable=find_it->second;
	auto it=matchingTable.getElements(searchSpecification.first,searchSpecification.second);
	return new VectorIteratorIndex(it.first,it.second,&matchingTable);
}

/*************************************************************** Full Index on Single Argument *****************************************************************/

FullIndexingStructure::FullIndexingStructure(AtomHistoryVector* table, Predicate* predicate, AtomSearcher* atomSearcher, bool recursive, unsigned indexType): IndexingStructure(table), predicate(predicate){
    indexingStructures.reserve(predicate->getArity());
    for(unsigned i=0;i<predicate->getArity();++i){
        vector<unsigned> indexingTerm(1,i);
        if(recursive)
			indexingStructures.push_back(new UnorderedMapOfHistoryVector(table,indexingTerm));
        else{
			IndexingStructure* iS=atomSearcher->getIndexingStructure(&indexingTerm);
			if(iS!=nullptr){
				indexingStructures.push_back(iS);
			}
			else{
				if(indexType==MAP)
					indexingStructures.push_back(new UnorderedMapOfUnorderedSet(table,indexingTerm));
				else if(indexType==MAP_VECTOR)
					indexingStructures.push_back(new UnorderedMapOfVector(table,indexingTerm));
				else if(indexType==MAP_HISTORY_VECTOR)
					indexingStructures.push_back(new UnorderedMapOfHistoryVector(table,indexingTerm));

			}
        }
    }
}

void FullIndexingStructure::add(Atom* atom) {
    for(auto indexingStructure: indexingStructures)
        indexingStructure->add(atom);
}

Atom* FullIndexingStructure::find(Atom* atom,const pair<SearchType,unsigned>& searchSpecification) {
    return indexingStructures[0]->find(atom);
}

void FullIndexingStructure::update() {
    for (;lastUpdate<table->size();++lastUpdate) {
        Atom* a=(*table)[lastUpdate];
        add(a);
    }
}

GeneralIterator* FullIndexingStructure::computeMatchIterator(Atom* templateAtom, const RuleInformation& ruleInformation, const pair<SearchType, unsigned>& searchSpecification,unsigned arg) {
	if(lastUpdate<table->size())
		update();
	return indexingStructures[arg]->computeMatchIterator(templateAtom,ruleInformation,searchSpecification,arg);
}

} /* namespace grounder */
} /* namespace DLV2 */


