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
 * GroundingPreferences.h
 *
 *  Created on: Mar 10, 2016
 *      Author: jessica
 */

#ifndef SRC_GROUNDER_STATEMENT_GROUNDINGPREFERENCES_H_
#define SRC_GROUNDER_STATEMENT_GROUNDINGPREFERENCES_H_

#include <unordered_map>
#include "Rule.h"
#include <iostream>
#include <vector>
#include <list>

using namespace std;

namespace DLV2 {
namespace grounder {

struct HashAtomPointer{
	inline size_t operator()(Atom* obj) const {
		return size_t(obj);
	}
	inline bool operator()(Atom* obj1, Atom* obj2) const {
		return obj1==obj2;
	}
};

enum AnnotationsError {OK, ATOM_NOT_PRESENT, ARITY_ERROR, CONFLICT_FOUND};
typedef unordered_map<Atom*,vector<unsigned>,HashAtomPointer,HashAtomPointer> unordered_map_pointers_atom_arguments;
typedef unordered_map<Atom*,vector<unsigned>,HashForTable<Atom>,HashForTable<Atom>> unordered_map_atom_arguments;

class GroundingPreferences {
public:
	bool addRuleOrderingType(Rule* rule, unsigned orderingType);
	bool addRuleProjectionType(Rule* rule, unsigned pType){
		rulesProjectionTypes.insert({rule->getIndex(), pType});
		return true;
	}
	void addRuleRewArith(Rule* rule){
		rulesRewArith.insert(rule->getIndex());
	}
	void addRuleLookAhead(Rule* rule){
		rulesLookAhead.insert(rule->getIndex());
	}
	void addRuleAlignSubstitutions(Rule* rule){
		rulesAlignSubstitutions.insert(rule->getIndex());
	}
	void addRuleToDecompose(Rule* rule){
		rulesToDecompose.insert(rule->getIndex());
	}
	void addRuleToNotDecompose(Rule* rule){
		rulesToNotDecompose.insert(rule->getIndex());
	}
        
	AnnotationsError addRuleAtomIndexingSetting(Rule* rule, Atom* atom, vector<unsigned>& arguments);
	void addRulePartialOrder(Rule* rule){rulesPartialOrders[rule->getIndex()].emplace_back();rulesPartialOrdersAtoms[rule->getIndex()].emplace_back();}
	AnnotationsError addRulePartialOrderAtom(Rule* rule, Atom* atom);
	AnnotationsError checkRulePartialOrderConflicts(Rule* rule);
	AnnotationsError applyRulePartialOrder(Rule* rule);

	void clearRulePartialOrder(Rule* rule);
	void swapRulePartialOrder(Rule* originalRule, Rule* newRule);

	bool addGlobalOrderingType(unsigned orderingType);
	void addGlobalAtomIndexingSetting(Atom* atom, vector<unsigned>& arguments);
	void addGlobalPartialOrder(){ globalPartialOrdersAtoms.emplace_back();}
	void addGlobalPartialOrderAtomStart(Atom* atom);
	void addGlobalPartialOrderAtomEnd(Atom* atom);

	int getOrderingType(Rule* r) ;
	pair<bool,int> getProjectionType(Rule* r){
		auto i =r->getIndex();
		if(rulesProjectionTypes.count(i))
				return {true,rulesProjectionTypes[i]};
		return {false,-1};
	}
	bool getRewArith(Rule* r){
		return rulesRewArith.count(r->getIndex());
	}
	bool getLookAhead(Rule* r){
		return rulesLookAhead.count(r->getIndex());
	}
	bool getAlignSubstitutions(Rule* r){
		return rulesAlignSubstitutions.count(r->getIndex());
	}
	bool getRuleToNotDecompose(Rule* r){
		return rulesToNotDecompose.count(r->getIndex());
	}
	bool getRuleToDecompose(Rule* r){
		return rulesToDecompose.count(r->getIndex());
	}
	bool checkPartialOrder(Rule* rule,unsigned atomPosition,const list<unsigned>& atoms) ;
	bool checkAtomIndexed(Rule* rule,Atom* atom,const vector<unsigned>& possibileArgs, vector<unsigned>& idxTerms) ;

	static GroundingPreferences* getGroundingPreferences() {
		if(groundingPreferences==0)
			groundingPreferences=new GroundingPreferences();
		return groundingPreferences;
	}

	~GroundingPreferences(){};
	static void freeInstance(){	delete groundingPreferences;}

	static void checkIfAtomIsPresentInRule(Rule* rule, Atom* atom, vector<unsigned>& positions);

	void print(Rule* rule) const;
private:
	unordered_map<unsigned,unsigned> rulesOrderingTypes;
	unordered_map<unsigned,unsigned> rulesProjectionTypes;
	unordered_set<unsigned> rulesRewArith;
	unordered_set<unsigned> rulesLookAhead;
	unordered_set<unsigned> rulesAlignSubstitutions;
	unordered_map<unsigned,unordered_map_pointers_atom_arguments> rulesAtomsIndexed;
	unordered_map<unsigned,vector<vector<bool>>> rulesPartialOrders;
	unordered_map<unsigned,vector<vector<Atom*>>> rulesPartialOrdersAtoms;
	unordered_set<unsigned> rulesToNotDecompose;
	unordered_set<unsigned> rulesToDecompose;

	int globalOrderingType;
	unordered_map_atom_arguments globalAtomsIndexed;
	vector<list<Atom*>> globalPartialOrdersAtoms;

	bool applayedGlobalAnnotations;


	bool applyGlobalAtomIndexingSetting();
	bool applyGlobalPartialOrder();

	void setGlobalAnnotations();

	GroundingPreferences():globalOrderingType(-1),applayedGlobalAnnotations(false){};
	static GroundingPreferences* groundingPreferences;


};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_STATEMENT_GROUNDINGPREFERENCES_H_ */
