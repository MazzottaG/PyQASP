/*******************************************************************************
 *   Copyright 2016 Francesco Calimeri, Davide Fusca', Simona Perri and Jessica Zangari
 *  
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *  
 *        http://www.apache.org/licenses/LICENSE-2.0
 *  
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *******************************************************************************/
/*
 * DecompositionMapper.h
 *
 *  Created on: Mar 28, 2017
 *      Author: jessica
 */

#ifndef SRC_GROUNDER_GROUND_DECOMPOSITIONMAPPER_H_
#define SRC_GROUNDER_GROUND_DECOMPOSITIONMAPPER_H_

#include "../statement/Rule.h"
#include "../hash/Hashable.h"
#include <unordered_map>
#include <vector>
#include "StatementDependency.h"
using namespace std;

namespace DLV2 {
namespace grounder {

class DecompositionMapper {
private:
	DecompositionMapper():currentDecomposedRuleIndex(-1){};
	map_rule<vector<Rule*>> rulesDecomposition;
	set_rule rulesDecomposed;
	int currentDecomposedRuleIndex;
	static DecompositionMapper* decompositionMapper;

	unsigned getNextIndex(){
		if(currentDecomposedRuleIndex<0)
			currentDecomposedRuleIndex=StatementDependency::getInstance()->getStatementsSize();
		else
			++currentDecomposedRuleIndex;
		return currentDecomposedRuleIndex;
	}
public:
	~DecompositionMapper(){
		for(auto r:rulesDecomposition){
			for(auto rr:r.second){
				rr->free();
				delete rr;
			}
			r.first->free();
			delete r.first;
		}
	}
	static DecompositionMapper* getInstance(){
		if(decompositionMapper==0)
			decompositionMapper=new DecompositionMapper;
		return decompositionMapper;
	}
	static void freeInstance(){delete decompositionMapper; decompositionMapper=0;}
	void remove(Rule* r){
		auto it=rulesDecomposition.find(r);
		if(it!=rulesDecomposition.end()){
			for(auto& rr:it->second){
				rr->free();
				delete rr;
				rr=0;
			}
			Rule* rule=it->first;
			rulesDecomposition.erase(it->first);
			rule->free();
			delete rule;
		}
	}
	void add(Rule* r,vector<Rule*> rulesDecomp){
		for(auto rule:rulesDecomp)
			rule->setIndex(getNextIndex());
		rulesDecomposition.insert({r,rulesDecomp});
	}
	Rule* get(Rule* r){
		return rulesDecomposition.find(r)->first;
	}
	vector<Rule*>::const_iterator getBegin(Rule* r){
		return rulesDecomposition.at(r).begin();
	}
	vector<Rule*>::const_iterator getEnd(Rule* r){
		return rulesDecomposition.at(r).end();
	}
	bool isDecomposable(Rule* r)const{
		if(rulesDecomposition.count(r))
			return !(rulesDecomposition.at(r).empty());
		return false;
	}
	void setDecomposed(Rule* r){
		rulesDecomposed.insert(r);
	}
	bool isDecomposed(Rule* r)const{
		return rulesDecomposed.count(r);
	}

};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_GROUND_DECOMPOSITIONMAPPER_H_ */
