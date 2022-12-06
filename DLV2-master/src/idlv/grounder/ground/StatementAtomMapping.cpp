/*
 * StatementAtomMappinga.cpp
 *
 *  Created on: Mar 26, 2019
 *      Author: jessica
 */

#include "StatementAtomMapping.h"

namespace DLV2 {
namespace grounder {

void StatementAtomMapping::addRule(Rule* r) {
	set_predicate head = r->getPredicateInHead();
	set_predicate body = r->getPredicateInBody();
	if(r->isChoiceRule()){
		set_predicate p=(r->getAtomInHead(0)->getNafLiteralsPredicates());
		body.insert(p.begin(),p.end());
	}
	for (auto i : head) {
		headMap.insert( { i->getIndex(), r });
	}
	for (auto i : body) {
		bodyMap.insert( { i->getIndex(), r });
	}
	for(auto predHead:head){
		unsigned h=predHead->getIndex();
		if(predicateDependencies.size()<=h)
			predicateDependencies.resize(h+1);
		for(auto predBody:body){
			unsigned b=predBody->getIndex();
			if(predicateDependencies[h].size()<=b)
				predicateDependencies[h].resize(b+1,false);
			predicateDependencies[h][b]=true;
		}
	}
}

void StatementAtomMapping::removeRule(Rule* r) {
	set_predicate head = r->getPredicateInHead();
	set_predicate body = r->getPredicateInBody();

	for (auto i : head) {
		auto it=headMap.equal_range(i->getIndex());
		for(auto iit=it.first;iit!=it.second;++iit){
			if((*iit).second->getIndex()==r->getIndex()){
				headMap.erase(iit);
				break;
			}
		}
	}
	for (auto i : body) {
		auto it=bodyMap.equal_range(i->getIndex());
		for(auto iit=it.first;iit!=it.second;++iit)
			if((*iit).second->getIndex()==r->getIndex()){
				bodyMap.erase(iit);
				break;
			}
	}
}

void StatementAtomMapping::getRuleInHead(index_object p, vector<Rule*>& rules) const{
	auto pair1 = headMap.equal_range(p);
	for (; pair1.first != pair1.second; ++pair1.first)
		rules.push_back(pair1.first->second);
}

void StatementAtomMapping::getRuleInHead(index_object p, unordered_set<unsigned>& rules) const{
	auto pair1 = headMap.equal_range(p);
	for (; pair1.first != pair1.second; ++pair1.first)
		rules.insert(pair1.first->second->getIndex());
}

void StatementAtomMapping::getRuleInBody(index_object p, unordered_set<unsigned>& rules) const {
	auto pair1 = bodyMap.equal_range(p);
	for (; pair1.first != pair1.second; ++pair1.first)
		rules.insert(pair1.first->second->getIndex());
}


bool StatementAtomMapping::isInHead(index_object p) const{
	if (headMap.find(p) != headMap.end())
		return true;
	return false;
}

bool StatementAtomMapping::isInBody(index_object p) const{
	if (bodyMap.find(p) != bodyMap.end())
		return true;
	return false;
}

void StatementAtomMapping::addPredicateDependencies(Predicate* a, Predicate* b) {
	if(a==nullptr || b==nullptr) return;
//	if(!completedComputationOfDependencies){
//		completeComputationOfDependencies();
//		completedComputationOfDependencies=true;
//	}
	unsigned indexPredA=a->getIndex();
	unsigned indexPredB=b->getIndex();
	if (predicateDependencies.size() <= indexPredA)
		predicateDependencies.resize(indexPredA + 1);
	if (predicateDependencies.size() <= indexPredB)
		predicateDependencies.resize(indexPredB + 1);

	// 1. a depends from b
	if (predicateDependencies[indexPredA].size() <= indexPredB)
		predicateDependencies[indexPredA].resize(indexPredB + 1, false);
	predicateDependencies[indexPredA][indexPredB]=true;

	//	2. a depends from b' if b depends from b', i.e. a depends from all predicates from which b depends
	//	3. a' depends from b if a' depends from a, i.e. all predicates depending from a, depend also from b
	//	4. a' depends from b' if a' depends from a and b depends from b',
	//	  i.e. all predicates depending from a, depend also from b
	//	all these cases are taken into account by completing dependencies
//	unordered_set<unsigned> dep={indexPredB};
//	computeIndirectDependencies(indexPredA,dep);
//	for (unsigned i = 0; i < predicateDependencies.size(); ++i) {
//		unordered_set<unsigned> dep2;
//		if (predicateDependencies[i].size()>indexPredA && predicateDependencies[i][indexPredA]){
//			dep2.insert(i);
//		}
//		computeIndirectDependencies(i,dep2);
//	}
}

void StatementAtomMapping::resetDependencies() {
	predicateDependencies.clear();
	completedComputationOfDependencies=false;
}

void StatementAtomMapping::computeIndirectDependencies(unsigned i, unordered_set<unsigned>& dep) {
	unordered_set<unsigned> depProcessed;
	while (!dep.empty()) {
		unsigned j = *dep.begin();
		dep.erase(j);
		depProcessed.insert(j);
		if (predicateDependencies.size() <= j)
			predicateDependencies.resize(j + 1);
		for (unsigned k = 0; k < predicateDependencies[j].size(); ++k) {
			if (depProcessed.count(k)) continue;
			if (predicateDependencies[i].size() <= k)
				predicateDependencies[i].resize(k + 1, false);
			if (predicateDependencies[j][k] ) {
				dep.insert(k);
				predicateDependencies[i][k] = true;
			}
		}
	}
}

void StatementAtomMapping::completeComputationOfDependencies() {
	for (unsigned i = 0; i < predicateDependencies.size(); ++i) {
		unordered_set<unsigned> dep;
		for (unsigned i2 = 0; i2 < predicateDependencies[i].size(); ++i2)
			if (predicateDependencies[i][i2])
				dep.insert(i2);
		computeIndirectDependencies(i,dep);
	}
}

bool StatementAtomMapping::checkIfDepended(Predicate* a, Predicate* b){
	// Check if predicate a depends from predicate b
	if(a==nullptr || b==nullptr) return false;
	if(!completedComputationOfDependencies){
		completeComputationOfDependencies();
		completedComputationOfDependencies=true;
	}
	unsigned indexPredA=a->getIndex();
	unsigned indexPredB=b->getIndex();
	if (predicateDependencies.size() <= indexPredA){
		predicateDependencies.resize(indexPredA + 1);
		return false;
	}
	if (predicateDependencies[indexPredA].size() <= indexPredB){
		predicateDependencies[indexPredA].resize(indexPredB + 1, false);
		return false;
	}
	return predicateDependencies[indexPredA][indexPredB];
}

void StatementAtomMapping::resizeToPredicateSize(unsigned factor){
	unsigned newSize=PredicateTable::getInstance()->getSize()*factor;
	if (predicateDependencies.size() < newSize)
		predicateDependencies.resize(newSize);
	for (unsigned i = 0; i < predicateDependencies.size(); ++i)
		if (predicateDependencies[i].size() < newSize)
			predicateDependencies[i].resize(newSize);
}

void StatementAtomMapping::print() const {
	for(auto e:predicateDependencies){
		cerr<<"\t";
		for(auto f:e)
			cerr<<f<<" ";
		cerr<<endl;
	}
}

void StatementAtomMapping::printWithPredicateNames() const {
    for( unsigned i=0; i<predicateDependencies.size(); i++ )
        {
        cerr << PredicateTable::getInstance()->searchPredicateByIndex(i)->getName() << " ->";
        for( unsigned j=0; j<predicateDependencies[i].size(); j++ )
            if( predicateDependencies[i][j] )
                cerr << " " << PredicateTable::getInstance()->searchPredicateByIndex(j)->getName();
        cerr << endl;
        }
}

} /* namespace grounder */
} /* namespace DLV2 */
