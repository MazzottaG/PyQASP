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
 * Rule.h
 *
 *  Created on: 03/apr/2014
 *      Author: Jessica
 */

#ifndef RULE_H_
#define RULE_H_

#include <vector>
#include <unordered_set>
#include "../atom/Atom.h"
#include "../hash/HashVecInt.h"

using namespace std;

namespace DLV2{

namespace grounder{

///Tuple for the grounding part of the weak: Weight,Level, Terms (the label of the weak)
using tupleWeak = tuple<Term*,Term*,vector<Term*>>;

class Rule;

using set_rule = unordered_set<Rule*, IndexForTable<Rule>, IndexForTable<Rule>>;

template<typename T>
using map_rule = unordered_map<Rule*, T, IndexForTable<Rule>, IndexForTable<Rule>>;


/**
 * This struct contain the information used for the match of atoms. Bound contain the index of term total bound. Bind the index of term
 * bind. Builtin the builtin atom that can be evaluated while we check the match of the atom. VarUsedInBuiltin the variable binded of the atom that we
 * match shared with one builtin in vector builtin. Other the functional atoms, arith term and variable binded in the atom that compare two times.
 **/
struct MatchInformation{
	vector<unsigned> bound;
	//CAN BE OPTIMIZED with vector<unisnged>
	vector<pair<unsigned,unsigned>> bind;
	vector<pair<unsigned,unsigned>> varUsedInBuiltin;
	vector<Atom*> builtin;
	vector<unsigned> other;
	vector<unsigned> dictionaryIntersection;
	vector<vector<pair<Predicate*, unsigned>>> boundPreds;
};

///RuleInformation contain the information of rule relative:
///		- The number of the variable in the body of the rule
///		- The intersection of the dictionary for each positive classical literal
class RuleInformation{

public:

	set_term::const_iterator getDictionaryIntersectionBegin(index_object index)const{
		return dictionaryIntersection[index].begin();
	}
	set_term::const_iterator getDictionaryIntersectionEnd(index_object index)const{
		return dictionaryIntersection[index].end();
	}

	set_term::iterator getDictionaryIntersectionBegin(index_object index){
		return dictionaryIntersection[index].begin();
	}
	set_term::iterator getDictionaryIntersectionEnd(index_object index){
		return dictionaryIntersection[index].end();
	}

	void removeInDictionaryIntersection(set_term::iterator& it,index_object index){
		it=dictionaryIntersection[index].erase(it);
	}

	void insertInDictionaryIntersection(index_object index,Term* term){
		dictionaryIntersection[index].insert(term);
		dictionaryIntersectionCreation[index]=true;
	}

	void insertInDictionaryIntersection(index_object index,const set_term& set){
		dictionaryIntersection[index].insert(set.begin(),set.end());
		dictionaryIntersectionCreation[index]=true;
	}

	void setDictionaryIntersectionSize(unsigned size){
		dictionaryIntersection.resize(size);
		dictionaryIntersectionCreation.resize(size,false);
	}

	void clearDictionaryIntersection(){
		dictionaryIntersection.clear();
		dictionaryIntersectionCreation.clear();
	}

	inline bool countInDictionaryIntersection(index_object index,Term* term)const{
		return dictionaryIntersection[index].count(term);
	}

	inline unsigned getDictionaryIntersectionSize(index_object index)const{
		return dictionaryIntersection[index].size();
	}

	inline vector<set_term>::const_iterator getDictionaryIntersectionsBegin()const{
		return dictionaryIntersection.begin();
	}

	inline vector<set_term>::const_iterator getDictionaryIntersectionsEnd()const{
		return dictionaryIntersection.end();
	}

	inline bool isCreatedDictionaryIntersection(index_object index)const{
		if(index>=dictionaryIntersectionCreation.size())
			return false;
		return dictionaryIntersectionCreation[index];
	}

	inline void addBounderBuiltin(unsigned i,Atom *builtin){
		if(i>=bounderBuiltins.size())
			bounderBuiltins.resize(i+1);
		bounderBuiltins[i].push_back(builtin);
	}

	inline void clearBounderBuiltin(){
		bounderBuiltins.clear();
	}

	inline const vector<Atom*>& getBounderBuiltin(unsigned i)const{
		return bounderBuiltins[i];
	}

	inline bool isBounderBuiltin(unsigned i)const{
		if(i>=bounderBuiltins.size() || bounderBuiltins[i].empty())
			return false;
		return true;
	}


	void print()const{
		cerr<<"Dictionary Intersection"<<endl;
		for(unsigned i=0;i<dictionaryIntersectionCreation.size();++i){
			if(dictionaryIntersectionCreation[i]){
				for(auto t:joinVariables)
					if(t->getLocalVariableIndex()==i)
					{
						t->print(cerr);cerr<<" --> ";
						break;
					}
				for(auto t:dictionaryIntersection[i]){
					t->print(cerr);cerr<<" ";
				}
				cerr<<endl;
			}
		}
	}

	void insertJoinVariable(Term* term) {joinVariables.insert(term);}

	bool isAJoinVariable(Term* term) const {return joinVariables.count(term);}

	bool areThereJoinVariables() const {return !joinVariables.empty();}

	void computeOutputVariables(Rule* rule);

	bool isAnOutputVariable(Term* term, Rule* rule){
		if(outputVariables.empty())
			computeOutputVariables(rule);
		return outputVariables.count(term);
	}

	set_term& getOutputVariables(Rule* rule){
		if(outputVariables.empty())
			computeOutputVariables(rule);
		return outputVariables;
	}

	const MatchInformation& getMatchInformation(unsigned indexAtom,unsigned subAtom=0)const{
		return atomMatchInformation[indexAtom][subAtom];
	}

	void addMatchInformation(MatchInformation& matchInfo,unsigned indexAtom){
		atomMatchInformation[indexAtom].push_back(matchInfo);
	}

	void clearAndResizeMatchInfo(unsigned size){
		atomMatchInformation.clear();
		atomMatchInformation.resize(size);
	}

private:
	vector<set_term> dictionaryIntersection;
	vector<bool> dictionaryIntersectionCreation;
	set_term joinVariables;
	vector<vector<Atom*>> bounderBuiltins;
	/// The set of variables appearing in the head of the current rule
	set_term outputVariables;
	/// For each atom a vector of match information. If is classical literal the vector contain exactly one matchInformation, if is
	/// an aggregate atom contain a matchInreturn r->;formation for each aggregate element
	vector<vector<MatchInformation>> atomMatchInformation;
};



/**
 * @brief This class represents a rule with its body and head atoms
 */

class Rule : public Indexable {
public:
	const char SEPARATOR_BODY=',';
	const char SEPARATOR_HEAD='|';
	Rule():Indexable(), ground(false), simplifiedHead(0), simplifiedBody(0), mustBeRewritedForAggregates(false),variablesSize(0)  {};
	Rule(bool g):Indexable(), ground(g), simplifiedHead(0), simplifiedBody(0), mustBeRewritedForAggregates(false),variablesSize(0) {};
	Rule(bool g, unsigned sizeHead, unsigned sizeBody) : Indexable(), ground(g), simplifiedHead(0), simplifiedBody(0), mustBeRewritedForAggregates(false),variablesSize(0) {
		if(ground){
			simplifiedHead=new bool[sizeHead];
			simplifiedBody=new bool[sizeBody];
			head.reserve(sizeHead);
			for(unsigned i=0;i<sizeHead;i++){
				simplifiedHead[i]=false;
				head.push_back(nullptr);
			}
			body.reserve(sizeBody);
			for(unsigned i=0;i<sizeBody;i++){
				simplifiedBody[i]=false;
				body.push_back(nullptr);
			}
		}
	};
	
	virtual bool hasOnlyPositiveClassicalLitsAndNoArithTerms(){
		if(isChoiceRule()) return false;
		for(auto atom:head)
			if(atom!=nullptr && !(atom->isClassicalLiteral() && !atom->isNegative() && !atom->containsArithTerms()))
				return false;
		for(auto atom:body)
			if(atom!=nullptr && !(atom->isClassicalLiteral() && !atom->isNegative() && !atom->containsArithTerms()))
				return false;
		return true;
	}

	virtual bool isAnInputGroundRule(){
		if(isChoiceRule()) return false;
		for(auto atom:head)
			if(atom!=nullptr && !atom->isGround())
				return false;
		for(auto atom:body)
			if(atom!=nullptr && (!atom->isClassicalLiteral() || (atom->isClassicalLiteral() && !atom->isGround())))
				return false;
		return true;
	}
	
	///Getter method for body atoms
	const vector<Atom*>& getBody() const {return body;}

	vector<Atom*> getClonedBody()const{
		vector<Atom*> atoms;
		for(auto atom:body)
			atoms.push_back(atom->clone());
		return atoms;
	}

	vector<Atom*> getClonedHead()const{
		vector<Atom*> atoms;
		for(auto atom:head)
			atoms.push_back(atom->clone());
		return atoms;
	}
	///Setter method for body atoms
	void setBody(const vector<Atom*>& body) {this->body = body;}
	///Getter method for head atoms
	const vector<Atom*>& getHead() const {return head;}
	///Setter method for head atoms
	void setHead(const vector<Atom*>& head) {this->head = head;}

	///Clone a non ground rule
	virtual Rule* clone();

	///This method returns true if it is a strong constrain
	virtual bool isAStrongConstraint()const{return head.empty();}
	///This method returns true if it is a fact
	bool isAFact()const{return body.empty() && head.size()==1 && head[0]->isClassicalLiteral();}

	///This method adds an atom in the head
	void addInHead(Atom* a){head.push_back(a);};
	///This method adds an atom in the body
	void addInBody(Atom* a){body.push_back(a);};
	///This method adds a range of atoms in the body
	void addInBody(vector<Atom*>::iterator begin,vector<Atom*>::iterator end){body.insert(body.begin(),begin,end);};

	///This method returns the set of predicate in the head
	set_predicate getPredicateInHead(){return calculatePredicate(head,0,0);};
	///This method returns the set of predicate in the body
	set_predicate getPredicateInBody(){return calculatePredicate(body,0,0);};
	///This method returns the set of predicate of just the positive atoms in the body
	set_predicate getPositivePredicateInBody(){return calculatePredicate(body,1,0);};
	///This method returns the set of predicate index of just the positive atoms in the body
	unordered_set<index_object> getPositivePredicateIndexInBody(){return calculatePredicateIndex(body,1,0);};
	///This method returns the set of predicate of just the negative atoms in the body
	set_predicate getNegativePredicateInBody(){return calculatePredicate(body,1,1);};
	///This method returns the size of the head
	unsigned int getSizeHead() const {return head.size();}
	///This method return the size of the body
	unsigned int getSizeBody() const {return body.size();}
	///Remove atom in body
	void removeInBody(unsigned position){body.erase(body.begin()+position);};
	///Remove atoms in body
	///@positions the index of the body atoms to remove from the vector
	/// WARNING: POSITIONS MUST BE ORDERED
	void removeInBody(vector<unsigned>& positions){unsigned nSize=body.size()-positions.size();for(unsigned i=0;i<positions.size();i++){body.erase(body.begin()+(positions[i]-i));}body.resize(nSize);}
	///Remove atom in body
	void removeInBody(vector<Atom*>::iterator it){body.erase(it);};
	///Insert an element in the body at specified position
	void insertInBody(Atom* atom,unsigned position){body.insert(body.begin()+position,atom);};

	///This method returns an iterator that points to the first atom in the body
	vector<Atom*>::iterator getBeginBody(){return body.begin();};
	vector<Atom*>::const_iterator getBeginBody()const{return body.begin();};
	///This method returns an iterator that points to the last atom in the body
	vector<Atom*>::const_iterator getEndBody()const{return body.end();};
	vector<Atom*>::iterator getEndBody(){return body.end();};
	///This method returns an iterator that points to the first atom in the head
	vector<Atom*>::const_iterator getBeginHead()const{return head.begin();};
	///This method returns an iterator that points to the last atom in the head
	vector<Atom*>::const_iterator getEndHead()const{return head.end();};

	///Return the specific atom in the body
	inline Atom* getAtomInBody(unsigned i) const {return body[i];};
	///Set the specific atom in the body
	inline void setAtomInBody(unsigned i,Atom* atom) {body[i]=atom;};

	///Set the specific atom in the body
	inline void setAtomInHead(unsigned i,Atom* atom) {head[i]=atom;};
	///Return the specific atom in the head
	inline Atom* getAtomInHead(unsigned i) {return head[i];};

	///Set the simplification of the atom in the given position in the head
	void setAtomToSimplifyInHead(unsigned position, bool simplify = true){ simplifiedHead[position]=simplify;}
	///Set the simplification of the atom in the given position in the body
	void setAtomToSimplifyInBody(unsigned position, bool simplify = true){ simplifiedBody[position]=simplify;}
	///Get the simplification of the atom in the given position in the body
	bool isAtomToSimplifyInBody(unsigned position){if(simplifiedBody!=0)return simplifiedBody[position];return false;}

	///This method remove all the atoms in the body and in the head
	void clear(){head.clear();body.clear();};

	virtual void free(){
		deleteBody([](Atom* atom){
			return 2;
		});
		if(!isAStrongConstraint())
			deleteHead([](Atom* atom){
				return 2;
			});
	}
	void freeBody(){
		deleteBody( [](Atom* atom){
			return 2;
		} );
	}
	///Printer method
	virtual void print(ostream& stream=cout) const;

	///Return true if the rule is the answer to the query
	bool isQueryRule(const Predicate* queryPredicate) const{
		//The comparison is between predicate pointer because we know that
		//predicate is a flyweight
		if(head.size()!=1)return false;
		Predicate *p=head[0]->getPredicate();
		if(p==nullptr)return false;
		return p==queryPredicate;
	};

	/** @brief Equal-to operator for rules
	 *  @details Two rules are equal if they have the same atoms in the body and in the head regardless the order in which they appear
	 */
	bool operator==(const Rule & r);

	///Destructor
	~Rule(){
		if(ground){
			delete[] simplifiedBody;
			delete[] simplifiedHead;
		}
	}

	bool isGround() const {return ground;}
	void setGround(bool ground) {this->ground=ground;}

	bool areThereUndefinedAtomInBody() const {for(unsigned i=0;i<body.size();i++) if(!simplifiedBody[i]) return true; return false;}

	///Getter and Setter to determine whether the rule contains at least an aggregate
	bool isMustBeRewritedForAggregates() const { return mustBeRewritedForAggregates; }
	void setMustBeRewritedForAggregates(bool mustBeRewritedForAggregates) {	this->mustBeRewritedForAggregates = mustBeRewritedForAggregates;}
	
	///Return true if is a choice rule
	bool isChoiceRule() const{ return (head.size()==1 && head[0]->isChoice());}

	void deleteBody(function<int(Atom*)> f);

	void deleteHead(function<int(Atom*)> f);

	void deleteGroundRule();

	void setUnsolvedPredicates();
	
	void computeJoinVariables();

	void computeVariablesLocalIndices();

	inline unsigned getVariablesSize() const {
		return variablesSize;
	}

	virtual bool isWeakConstraint()const{
		return false;
	}

	virtual void getVariables(set_term& variables)const{
		getVariablesInHead(variables);
		getVariablesInBody(variables);
	}

	virtual void getGlobalVariables(set_term& variables){
		getVariablesInHead(variables);
		for(auto atom:body){
			set_term terms;
			if(atom->isAggregateAtom()){
				terms=atom->getSharedVariable(body.begin(),body.end());
				set_term guards=atom->getGuardVariable();
				terms.insert(guards.begin(),guards.end());
			}
			else
				terms=atom->getVariable();
			variables.insert(terms.begin(),terms.end());
		}
	}

	virtual void getVariablesInHead(set_term& variables)const{
		for(auto atom:head)
			atom->getVariable(variables);
	}

	virtual void getVariablesInBody(set_term& variables)const{
		for(auto atom:body)
			atom->getVariable(variables);
	}

	virtual void extractAtomsInBodyContainingVariables(const set_term& vars,vector<unsigned>& extractedAtomsPos){
		for(unsigned i=0;i<body.size();++i){
			Atom* atom=body[i];
			if(atom->isAggregateAtom()){
				set_term v=atom->getGuardVariable();
				set_term shared=atom->getSharedVariable(body.begin(),body.end());
				shared.insert(v.begin(),v.end());
				if(Utils::isContained(shared,vars))
					extractedAtomsPos.push_back(i);
			}
			else if(!atom->isAggregateAtom() && Utils::isContained(atom->getVariable(),vars))
				extractedAtomsPos.push_back(i);
		}
	}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
	virtual vector<Term*>& getLabel() {}
#pragma GCC diagnostic pop
	virtual unsigned getLevelInt(){return 0;}
	virtual Term* getLevel() const {	return nullptr;}
	virtual void setWeight(Term* weight) {}
	virtual Term* getWeight() const {return nullptr;}
	virtual void setWeightLevelLabel(tupleWeak&& tp){};
	virtual tupleWeak groundWeightLevel(var_assignment& current_assignment){return make_tuple(nullptr,nullptr,vector<Term*>());
	}

	//-----------------Rule Information Interface -------------------------------

	set_term::const_iterator getDictionaryIntersectionBegin(index_object index) const {
		return ruleInformation.getDictionaryIntersectionBegin(index);
	}
	set_term::const_iterator  getDictionaryIntersectionEnd(index_object index) const{
		return ruleInformation.getDictionaryIntersectionEnd(index);
	}

	set_term::iterator getDictionaryIntersectionBegin(index_object index) {
		return ruleInformation.getDictionaryIntersectionBegin(index);
	}
	set_term::iterator  getDictionaryIntersectionEnd(index_object index) {
		return ruleInformation.getDictionaryIntersectionEnd(index);
	}

	void removeInDictionaryIntersection(set_term::iterator& it,index_object index){
		ruleInformation.removeInDictionaryIntersection(it,index);
	}

	const RuleInformation& getRuleInformation(){
		return ruleInformation;
	}

	void insertInDictionaryIntersection(index_object index,Term* term){
		ruleInformation.insertInDictionaryIntersection(index,term);
	}

	void insertInDictionaryIntersection(index_object index,const set_term& set){
		ruleInformation.insertInDictionaryIntersection(index,set);
	}

	void setDictionaryIntersectionSize(unsigned size){
		ruleInformation.setDictionaryIntersectionSize(size);
	}

	void clearDictionaryIntersection(){
		ruleInformation.clearDictionaryIntersection();
	}

	inline void addBounderBuiltin(unsigned i,Atom *builtin){
		ruleInformation.addBounderBuiltin(i,builtin);
	}

	inline void clearBounderBuiltin(){
		ruleInformation.clearBounderBuiltin();
	}

	bool isAnOutputVariable(Term* term){
		return ruleInformation.isAnOutputVariable(term,this);
	}

	set_term& getOutputVariables() {
		return ruleInformation.getOutputVariables(this);
	}

	void addMatchInformation(MatchInformation& matchInfo,unsigned indexAtom){
		ruleInformation.addMatchInformation(matchInfo,indexAtom);
	}

	void clearAndResizeMatchInfo(unsigned size){
		ruleInformation.clearAndResizeMatchInfo(size);
	}

	bool containsExternalLits(){
		for(auto b:body)
			if(b->isExternalAtom())
				return true;
		return false;
	}

	virtual void getWeightLevelLabeltVariables(set_term& variables)const{}

protected:

	/// Return the predicate in atoms vector, if checkNegative is true compare the negative of atom with the parameter
	/// else insert the predicate
	set_predicate calculatePredicate(vector<Atom*>& atoms,bool checkNegative,bool negative);
	/// Return the predicate index in atoms vector, if checkNegative is true compare the negative of atom with the parameter
	/// else insert the predicate
	unordered_set<index_object> calculatePredicateIndex(vector<Atom*>& atoms,bool checkNegative,bool negative);

	///Vector of the atoms in head
	vector<Atom*> head;
	///Vector of the atoms in body
	vector<Atom*> body;
	//Boolean to set whether the rule is ground
	bool ground;

	///An array containing true at a position in the head if that atom has to be simplified, false otherwise
	bool* simplifiedHead;
	///An array containing true at a position in the body if that atom has to be simplified, false otherwise
	bool* simplifiedBody;

	virtual void printNonGround(ostream& stream=cout) const;

	bool mustBeRewritedForAggregates;

	RuleInformation ruleInformation;

	unsigned variablesSize;

};



class WeakConstraint : public Rule{
public:


	WeakConstraint():weight(nullptr),level(nullptr),levelInt(0){}

	WeakConstraint(const vector<Atom*>& body,Term* weight, Term* level,const vector<Term*>& terms):weight(weight),level((level==nullptr)?TermTable::getInstance()->term_zero:level),label(terms),levelInt(0){
		this->body=body;
	}

	WeakConstraint(bool g, unsigned sizeBody,const vector<Atom*>& body,Term* weight, Term* level,const vector<Term*>& terms):Rule(g,0,sizeBody),weight(weight),level((level==nullptr)?TermTable::getInstance()->term_zero:level),label(terms),levelInt((g)?level->getConstantValue():0){
		this->body=body;
	}

	WeakConstraint(bool g, unsigned sizeBody,Term* weight, Term* level,const vector<Term*>& terms):Rule(g,0,sizeBody),weight(weight),level((level==nullptr)?TermTable::getInstance()->term_zero:level),label(terms),levelInt((g)?level->getConstantValue():0){
	}
	
	virtual bool isAnInputGroundRule(){
		if(!weight->isGround())
			return false;
		if(!level->isGround())
			return false;
		for(auto t:label)
			if(!t->isGround())
				return false;
		return Rule::isAnInputGroundRule();
	}

	virtual bool hasOnlyPositiveClassicalLitsAndNoArithTerms(){
		if(weight->contain(TermType::ARITH))
			return false;
		if(level->contain(TermType::ARITH))
			return false;
		for(auto t:label)
			if(t->contain(TermType::ARITH))
				return false;
		return Rule::isAnInputGroundRule();
	}

	virtual bool isWeakConstraint()const{return true;}

	vector<Term*>& getLabel() {return label;}

	void setLabel(const vector<Term*>& label) {	this->label = move(label);}

	Term* getLevel() const {	return level;}

	unsigned getLevelInt(){return levelInt;}

	void setLevel(Term* level) {	this->level = level;}

	Term* getWeight() const {return weight;}

	void setWeight(Term* weight) {this->weight = weight;}

	virtual void print(ostream& stream=cout) const;

	bool isAStrongConstraint()const{return false;}

	void printNonGround(ostream& stream=cout) const;

	virtual void setWeightLevelLabel(tupleWeak&& groundWeightLevelLabel){
		weight=get<0>(groundWeightLevelLabel);
		level=get<1>(groundWeightLevelLabel);
		label=move(get<2>(groundWeightLevelLabel));
	};

	virtual void free(){
		deleteBody([](Atom* atom){
			return 2;
		});
	}

	virtual void getWeightLevelLabeltVariables(set_term& variables)const{
		if(weight!=0)weight->getVariable(variables);
		if(level!=0)level->getVariable(variables);
		for(auto t:label)
			t->getVariable(variables);
	}

	virtual void getVariables(set_term& variables)const{
		Rule::getVariables(variables);
		getWeightLevelLabeltVariables(variables);
	}

	virtual void getGlobalVariables(set_term& variables){
		getWeightLevelLabeltVariables(variables);
		Rule::getGlobalVariables(variables);
	}

	Rule* clone(){
		Rule * newRule=new WeakConstraint;
		for(auto atom:head)
			newRule->addInHead(atom->clone());
		for(auto atom:body)
			newRule->addInBody(atom->clone());
		WeakConstraint* w=dynamic_cast<WeakConstraint*>(newRule);
		w->setWeight(this->getWeight());
		w->setLevel(this->getLevel());
		w->setLabel(this->getLabel());
		return newRule;
	}


	///Ground the Weight, the level and the label of the weak and return with the tuple
	tupleWeak groundWeightLevel(var_assignment& current_assignment);

private:
	Term* weight;
	Term* level;

	vector<Term*> label;

	unsigned levelInt;
};


};

};
#endif /* RULE_H_ */
