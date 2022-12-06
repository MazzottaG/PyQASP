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
 * InputRewriter.h
 *
 *  Created on: 18/feb/2015
 *      Author: tesi2
 */

#ifndef INPUTREWRITER_H_
#define INPUTREWRITER_H_

#include "Rule.h"
#include "../atom/ClassicalLiteral.h"
#include "../table/PredicateExtension.h"
#include "../table/PredicateTable.h"
#include "OrderRule.h"

namespace DLV2 {
namespace grounder {

/// Abstract class that defines a choosing policy for savior atoms
class SaviorChoosingPolicy{
public:
	virtual bool choose(Atom* atom, unsigned savior_pos, list<Atom*>& possibleAtomsBinding, vector<Atom*>& atomToAdd, bool end, const OrderRule& orderRule) = 0;
	virtual ~SaviorChoosingPolicy() {};
};

/// A concrete implementation of SaviorChoosingPolicy class that just choose the first savior
class FirstSaviorChoosingPolicy : public SaviorChoosingPolicy{
public:
	virtual bool choose(Atom* atom, unsigned savior_pos, list<Atom*>& possibleAtomsBinding, vector<Atom*>& atomToAdd, bool end, const OrderRule& orderRule);
	virtual ~FirstSaviorChoosingPolicy() {};

private:
	void getRecursiveDependencies(const OrderRule& orderRule, unsigned savior_pos, vector<Atom*>& atomToAdd);
};


/// Abstract class that defines how to rewrite the input program
class InputRewriter {
public:
	using PredicateToSkip=function<bool(Predicate*,bool recursive)>;

	InputRewriter():predicateExtTable(PredicateExtTable::getInstance()), predicateTable(PredicateTable::getInstance()) {};
	virtual void translateAggregate(Rule* rule, vector<Rule*>& ruleRewrited, OrderRule* orderRule=nullptr) = 0;
	virtual void translateChoice(Rule*& rule, vector<Rule*>& ruleRewrited, OrderRule* orderRule=nullptr) = 0;
	virtual void projectAtoms(Rule*& rule, vector<Rule*>& ruleRewrited,const string& pname=AUXILIARY)=0;
	virtual void pushBuiltin(Rule*& rule)=0;
	virtual void pushBuiltinArithOnlyInBody(Rule*& rule)=0;
	virtual void  chooseBestSaviorForAggregate(Rule* rule, AggregateElement* aggregateElement, set_term& unsafeVars, vector<Atom*>& atomToAdd, const OrderRule& orderRule) = 0;
	virtual ~InputRewriter(){};
protected:
	PredicateExtTable* predicateExtTable;
	PredicateTable* predicateTable;

	Atom* generateNewAuxiliaryAtom(string& predicate_name,vector<Term*>& terms,bool checkPredExistenceInPredTable=true);

};


/// A concrete implementation of InputRewriter that defines the translation of aggregates atoms using auxiliary rules
class BaseInputRewriter : public InputRewriter {
public:
	BaseInputRewriter():InputRewriter(), saviourChoosingPolicy(new FirstSaviorChoosingPolicy) {};
	/**
	 * For each aggregate element in each aggregate atom in the given rule an auxiliary rule is produced, so that:
	 * 		- its head is composed by an auxiliary atom whose terms are the variables of aggregation and the variables shared
	 * 			with the other atoms of the rule,
	 *		- its body is composed by :
	 *			- the atoms appearing in the aggregate element,
	 *			- the atoms needed to save any atom in the aggregate element.
	 * The choose of the savior(s) is delegated to the method chooseBestSaviorForAggregate
	 */
	virtual void translateAggregate(Rule* rule, vector<Rule*>& ruleRewrited, OrderRule* orderRule=nullptr);

	/**
	 * Given a choice rule it is rewritten by means of new auxiliary rules.
	 * In particular, the rules created are:
	 * 		- One rule to factorize the body of the choice rule
	 * 		- One disjunctive rule for each choice element
	 * 		- One constraint rule with a negated count aggregate
	 *  More details are given in the method itself.
	 */
	virtual void translateChoice(Rule*& rule, vector<Rule*>& ruleRewrited, OrderRule* orderRule=nullptr);

	virtual void projectAtoms(Rule*& rule, vector<Rule*>& ruleRewrited,const string& pname=AUXILIARY);

	virtual void pushBuiltin(Rule*& rule);
	virtual void pushBuiltinArithOnlyInBody(Rule*& rule);

	/**
	 * This template method searches for possible saviors of the negative atoms in the given aggregateElement.
	 * Each possible savior found is passed to the saviorChoosingPolicy object that will independently select
	 * a savior among the saviors found.
	 */
	virtual void  chooseBestSaviorForAggregate(Rule* rule, AggregateElement* aggregateElement, set_term& unsafeVars, vector<Atom*>& atomToAdd, const OrderRule& orderRule);

	virtual ~BaseInputRewriter() {delete saviourChoosingPolicy;}


protected:
	/// The savior choosing policy adopted
	SaviorChoosingPolicy* saviourChoosingPolicy;

	/// Utility method: returns the variables in the given aggregate element without the aggregation variables
	set_term getVariablesInAggregateElem(AggregateElement* aggregateElem);

	///Create an auxiliary rule for each  choice element
	virtual vector<AggregateElement*> rewriteChoiceElements(Rule* rule,unsigned& id,unsigned& counter, Atom* choice, Atom* auxiliaryAtomBody,	vector<Rule*>& ruleRewrited, OrderRule* orderRule=nullptr);

	virtual void rewriteChoiceConstraint(const vector<AggregateElement*>& elements, Atom* auxiliaryAtomBody, Atom* choice, vector<Rule*>& ruleRewrited);

	virtual void createBodyRuleChoice(Rule* rule,unsigned& id, unsigned& counter,ChoiceElement* choiceElement,Atom* auxiliaryAtomBody,set_term &terms_in_bodychoice,vector<Rule*>& ruleRewrited,vector<Atom*>& naf_elements,AggregateElement*& element,bool defaultGuard, OrderRule* orderRule=nullptr);
private:
	unordered_map<Predicate*,vector<pair<unordered_set<unsigned>,Predicate*>>,IndexForTable<Predicate>,IndexForTable<Predicate>> projectedAtoms;

};

class ChoiceBaseInputRewriter : public BaseInputRewriter{
public:
	virtual vector<AggregateElement*> rewriteChoiceElements(Rule* rule,unsigned& id,unsigned& counter, Atom* choice, Atom* auxiliaryAtomBody,	vector<Rule*>& ruleRewrited, OrderRule* orderRule=nullptr);
	virtual void rewriteChoiceConstraint(const vector<AggregateElement*>& elements, Atom* auxiliaryAtomBody, Atom* choice, vector<Rule*>& ruleRewrited);
protected:


	Rule* createAuxChoiceRule(const vector<Atom*>& head,const vector<Atom*>& body);
	Rule* createAuxChoiceRule(Atom* head,const vector<Atom*>& body){return createAuxChoiceRule(vector<Atom*>({head}),body);}
	Rule* createAuxChoiceRule(const vector<Atom*>& head,Atom* body){return createAuxChoiceRule(head,vector<Atom*>({body}));}
	Rule* createAuxChoiceRule(const vector<Atom*>& head){return createAuxChoiceRule(head,vector<Atom*>());}
	Rule* createAuxChoiceRule(Atom* head,Atom* body){return createAuxChoiceRule(vector<Atom*>({head}),vector<Atom*>({body}));}



};

class AdvancedChoiceBaseInputRewriter : public ChoiceBaseInputRewriter{
public:
	virtual vector<AggregateElement*> rewriteChoiceElements(Rule* rule,unsigned& id,unsigned& counter, Atom* choice, Atom* auxiliaryAtomBody,	vector<Rule*>& ruleRewrited, OrderRule* orderRule=nullptr);
};


} /* namespace grounder */
} /* namespace DLV2 */

#endif /* INPUTREWRITER_H_ */
