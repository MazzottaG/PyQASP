/*************************************************************************************
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
 ************************************************************************************/
/*
 * BackTrackingGrounder.cpp
 *
 *  Created on: Jan 27, 2015
 *      Author: davide
 */

#include "BackTrackingGrounder.h"
#include "../../util/Timer.h"
#include "../atom/AggregateAtom.h"
#include "../term/ConstantTerm.h"
#include "../atom/Choice.h"
#include "../statement/GroundingPreferences.h"

namespace DLV2 {
namespace grounder {


#ifdef TRACE_ON
void BackTrackingGrounder::printAssignment(){
	for(unsigned i=0;i<current_assignment.size();i++){
		Term *value=current_assignment[i];
		if(value!=nullptr){
			cerr<<"VAR "<<i<<" = ";
			value->print(cerr);
			cerr<<" - ";
		}
	}
	cerr<<endl;
}
#endif

bool BackTrackingGrounder::findGroundMatch(){
	trace_action_tag(grounding,1,cerr<<"FIND GROUND ATOM ON: ";templateSetAtom[index_current_atom]->print(cerr);cerr<<endl;);
	// For each table to search call the first match until one table return find
	bool find=false;
	unsigned current_table=current_id_match_iterator[index_current_atom];
	unsigned n_table=current_id_match[index_current_atom].size();
	Atom* templateAtom=templateSetAtom[index_current_atom];

	// Avoid to clone template atom for each table for negative atoms
	bool isPossibleUndef=true;
	index_object indexNegativeAtom=0;
	bool negativeToClone=false;
	while(current_table<n_table){

		IndexingStructure *searcher=nullptr;
		Atom* atomFound=nullptr;
		if(current_table<predicate_searchInsert_atomSearcher[index_current_atom+currentRule->getSizeHead()][0].size()){
			searcher=predicate_searchInsert_atomSearcher[index_current_atom+currentRule->getSizeHead()][0][current_table];
			atomFound=searcher->find(templateAtom,{predicate_searchInsert_table[index_current_atom+currentRule->getSizeHead()][current_table].second,iteration});
		}

		//Initialize false for negative atom
		bool undef=false;

		find=(atomFound!=nullptr);
		if(atomFound!=nullptr)
			undef=!atomFound->isFact();

		if(templateAtom->isNegative() && find)
			find=!undef;

		if(templateAtom->isNegative() && !find){
			if(atomFound!=nullptr){
				indexNegativeAtom=atomFound->getIndex();
				isPossibleUndef=false;
			}
			//We can avoid to clone the template atom for each table and do it just at the end
			negativeToClone=true;
			if(StatementDependency::getInstance()->isPredicateNotStratified(templateAtom->getPredicate()->getIndex()))
				ground_rule->setAtomToSimplifyInBody(index_current_atom,false);
			else
				ground_rule->setAtomToSimplifyInBody(index_current_atom,!undef);
		}

		if(find){
			trace_action_tag(grounding,1,cerr<<"Match OK: ";atomFound->print(cerr);cerr<<endl;);
			if(!is_bound_atom[index_current_atom]){
				current_id_match[index_current_atom][current_table].second = MATCH;
			}else{
				current_id_match[index_current_atom][current_table].second = NO_MATCH;
				current_id_match_iterator[index_current_atom]=0;
			}
			substiteInGroundRule(index_current_atom,atomFound);
			ground_rule->setAtomToSimplifyInBody(index_current_atom,!undef);

			return find;
		}
		current_id_match[index_current_atom][current_table].second = NO_MATCH;
		current_id_match_iterator[index_current_atom]=++current_table;

	}

	//If at least a possible undef atom has been found then update accordingly the vector of undef atoms of this rule
	if(negativeToClone){
		while(!atomsPossibleUndef.empty() && int(atomsPossibleUndef.back())>=index_current_atom)
			atomsPossibleUndef.pop_back();
		if(isPossibleUndef && !ground_rule->isAtomToSimplifyInBody(index_current_atom))
			atomsPossibleUndef.push_back(index_current_atom);
		Atom* atomFound=templateAtom->clone();
		atomFound->setIndex(indexNegativeAtom);
		substiteInGroundRule(index_current_atom,atomFound);
	}

	current_id_match_iterator[index_current_atom]=0;

	return false;
}


bool BackTrackingGrounder::match() {
	//Avoid call multiple time method match for the ground atom in the last position of the rule
	if(is_bound_atom[index_current_atom] && !direction)
		return false;

	 if(templateSetAtom[index_current_atom]->isClassicalLiteral()){
		if(is_bound_atom[index_current_atom]){
			trace_action_tag(grounding,1,cerr<<"MATCH GROUND CLASSICAL LIT: ";templateSetAtom[index_current_atom]->print(cerr);cerr<<endl;);
			return findGroundMatch() == !templateSetAtom[index_current_atom]->isNegative();
		}
		else{
			trace_action_tag(grounding,1,cerr<<"MATCH CLASSICAL LIT: ";templateSetAtom[index_current_atom]->print(cerr);cerr<<endl;);
			bool match;
			unsigned current_table=current_id_match_iterator[index_current_atom];
			//if is the first table to visit and the id of first match is NO_MATCH
			if(current_table==0 && current_id_match[index_current_atom][current_table].second==NO_MATCH){
				match = firstMatch() == !templateSetAtom[index_current_atom]->isNegative();
			}else{
				match = nextMatch() == !templateSetAtom[index_current_atom]->isNegative();
			}
			return match;
		}

	}else if(templateSetAtom[index_current_atom]->isBuiltIn() ){
		trace_action_tag(grounding,1,cerr<<"MATCH BUILT-IN: ";templateSetAtom[index_current_atom]->print(cerr);cerr<<endl;);
		current_id_match[index_current_atom][0].second=1;
		ground_rule->setAtomToSimplifyInBody(index_current_atom);

		if(builtAlreadyEvaluated[index_current_atom])
			return true;

		return templateSetAtom[index_current_atom] -> evaluate(current_assignment);

	}else if(templateSetAtom[index_current_atom]->isAggregateAtom()){
		trace_action_tag(grounding,1,cerr<<"MATCH AGGREGATE: ";templateSetAtom[index_current_atom]->print(cerr);cerr<<endl;);
		return groundAggregate();
	}else if(currentRule->getAtomInBody(index_current_atom)->isExternalAtom()) {
		Atom * extAtom = currentRule->getAtomInBody(index_current_atom);
		return extAtom->match(current_assignment, direction) == !extAtom->isNegative();
	}

	return false;

}



bool BackTrackingGrounder::firstMatch(){
	trace_action_tag(grounding,1,cerr<<"FIRST MATCH ON: ";templateSetAtom[index_current_atom]->print(cerr);cerr<<endl;);
	// For each table to search call the first match until one table return find
	bool find=false;
	unsigned current_table=current_id_match_iterator[index_current_atom];
	unsigned n_table=current_id_match[index_current_atom].size();
	Atom* templateAtom=templateSetAtom[index_current_atom];
	PredicateExtension* predicateExtension=predicateExtTable->getPredicateExt(templateAtom->getPredicate());

	while(current_table<n_table){

		unsigned predSearch=index_current_atom+currentRule->getSizeHead();

		unsigned tableToSearch = current_id_match[index_current_atom][current_table].first;
		AtomSearcher* atomSearcher=predicateExtension->getAtomSearcher(tableToSearch);
		IndexingStructure *searcher=predicate_searchInsert_atomSearcher[predSearch][0][current_table];
		//Initialize false for negative atom
		bool undef=false;

		Atom* atomFound=nullptr;
		atomSearcher->firstMatch(index_current_atom,templateAtom,current_assignment,atomFound,currentRule->getRuleInformation(),searcher,indexingArguments[index_current_atom][0],outputVariablesInAtoms[index_current_atom],{predicate_searchInsert_table[predSearch][current_table].second,iteration},currentRule->getRuleInformation().getMatchInformation(index_current_atom));
		find=(atomFound!=nullptr);
		if(atomFound!=nullptr)
			undef=!atomFound->isFact();

		if(find){
			trace_action_tag(grounding,1,cerr<<"Match OK: ";atomFound->print(cerr);cerr<<endl;);
			if(!is_bound_atom[index_current_atom]){
				current_id_match[index_current_atom][current_table].second = MATCH;
			}else{
				current_id_match[index_current_atom][current_table].second = NO_MATCH;
				current_id_match_iterator[index_current_atom]=0;
			}
			substiteInGroundRule(index_current_atom,atomFound);
			ground_rule->setAtomToSimplifyInBody(index_current_atom,!undef);

			return find;
		}
		current_id_match[index_current_atom][current_table].second = NO_MATCH;
		current_id_match_iterator[index_current_atom]=++current_table;

	}

	current_id_match_iterator[index_current_atom]=0;

	return false;
}

bool BackTrackingGrounder::nextMatch(){
	trace_action_tag(grounding,1,cerr<<"NEXT MATCH ON: ";templateSetAtom[index_current_atom]->print(cerr);cerr<<endl;);

	bool find=false;
	unsigned current_table=current_id_match_iterator[index_current_atom];
	unsigned n_table=current_id_match[index_current_atom].size();
	Atom* templateAtom=templateSetAtom[index_current_atom];
	PredicateExtension* predicateExtension=predicateExtTable->getPredicateExt(templateAtom->getPredicate());

	while(current_table<n_table){

		unsigned predSearch=index_current_atom+currentRule->getSizeHead();


		unsigned tableToSearch = current_id_match[index_current_atom][current_table].first;
		bool match = current_id_match[index_current_atom][current_table].second;
		AtomSearcher* atomSearcher=predicateExtension->getAtomSearcher(tableToSearch);
		IndexingStructure *searcher=predicate_searchInsert_atomSearcher[predSearch][0][current_table];
		Atom* atomFound=nullptr;
		if(match != NO_MATCH){
			trace_action_tag(grounding,2,cerr<<"Invoked Next Match on table: "<<tableToSearch<<endl;);
			atomSearcher->nextMatch(index_current_atom,templateAtom,current_assignment,atomFound,currentRule->getRuleInformation(),outputVariablesInAtoms[index_current_atom],currentRule->getRuleInformation().getMatchInformation(index_current_atom));
		}
		else{
			trace_action_tag(grounding,2,cerr<<"Invoked First Match on table: "<<tableToSearch<<endl;);
			atomSearcher->firstMatch(index_current_atom,templateAtom,current_assignment,atomFound,currentRule->getRuleInformation(),searcher,indexingArguments[index_current_atom][0],outputVariablesInAtoms[index_current_atom],{predicate_searchInsert_table[predSearch][current_table].second,iteration},currentRule->getRuleInformation().getMatchInformation(index_current_atom));
		}

		find=(atomFound!=nullptr);
		bool undef=false;
		if(find)
			undef=!atomFound->isFact();

		if(find){
			trace_action_tag(grounding,1,cerr<<"Match OK: ";atomFound->print(cerr);cerr<<endl;);
			substiteInGroundRule(index_current_atom,atomFound);
			ground_rule->setAtomToSimplifyInBody(index_current_atom,!undef);
			current_id_match[index_current_atom][current_table].second = MATCH;
			return find;
		}
		current_id_match[index_current_atom][current_table].second = NO_MATCH;
		current_id_match_iterator[index_current_atom]=++current_table;
	}
	current_id_match_iterator[index_current_atom]=0;

	return false;
}

bool BackTrackingGrounder::next() {
	direction=1;
	callFoundAssignment = false;
	if( unsigned(index_current_atom+1)>=currentRule->getSizeBody())
		return false;

	++index_current_atom;
	generateTemplateAtom();
	return true;
}

void BackTrackingGrounder::onGroundRuleToPrint(const vector<bool>& newAtomsInHead) {
	for (unsigned i = 0; i < currentRule->getSizeHead(); ++i) {
		if (newAtomsInHead[i]) {
			Atom* headGroundAtom=groundTemplateAtomHead[i];
			Atom* searchAtom=nullptr;
			PredicateExtension* predicateExt = predicateExtTable->getPredicateExt(headGroundAtom->getPredicate());
			if(headAtomsWithTheSamePredicate[i]){
				for(auto atomSearcher:predicate_searchInsert_atomSearcher[i][0]){
					if(atomSearcher==nullptr) continue;
					searchAtom=atomSearcher->find(headGroundAtom);
					if(searchAtom!=nullptr)
						break;
				}
			}
			if(searchAtom==nullptr){
				Atom* newAtom = groundTemplateAtomHead[i]->clone();
				predicateExt->addAtom(newAtom,predicate_searchInsert_table[i][0].first);
				ground_rule->setAtomInHead(i, newAtom);
			}
			else
				ground_rule->setAtomInHead(i, searchAtom);
		}
	}
	outputBuilder->onRule(ground_rule);
	trace_action_tag(grounding,1,cerr<<"Ground Rule Produced: ";ground_rule->print(cerr);cerr<<endl;);
	if (printStats)
		rstats->groundNewRule(currentRule->getIndex());
}

bool BackTrackingGrounder::foundAssignment() {
	callFoundAssignment=true;

	for(unsigned vi:variableMustList)
		if(current_assignment[vi]->getType()!=LIST)
			return false;
        
	unsigned sizeHead=currentRule->getSizeHead();
	bool isAChoiceRule=currentRule->isChoiceRule();
	bool strongConstraint=currentRule->isAStrongConstraint();
	bool isWeak=currentRule->isWeakConstraint();

	bool undefinedAtomInBody=ground_rule->areThereUndefinedAtomInBody();
	bool head_true=(sizeHead == 1  && !isAChoiceRule) && (!undefinedAtomInBody);
	bool ground_new_atom=false;
	bool find_new_true_atom=false;
	bool foundATrueAtomInDisjuction=false;

	unsigned atom_counter=0;
	Atom *searchAtom=nullptr;
	if(isAChoiceRule){
		if(Options::globalOptions()->getRewritingType()==COMPACT_NATIVE_CHOICE){
			groundChoiceNatively(find_new_true_atom,ground_new_atom);
			if(ground_rule!=0 && ground_rule->getSizeHead()==1 && isAChoiceRule && ground_rule->getAtomInHead(0)->getChoiceElementsSize()==0)
				return false;
		}
		else
			groundChoice(find_new_true_atom,ground_new_atom);
	}

	for(auto atom=currentRule->getBeginHead();atom!=currentRule->getEndHead()&&!isAChoiceRule;++atom,++atom_counter){
		newAtomsInHead[atom_counter]=false;

		// When grounding head atoms, head template atoms are used in order to avoid the creation and deletion of atoms that are already present in the predicate extensions.
		// In case an atom is not already present in the predicate extension then the corresponding grounded template atom is cloned
		// and the atom obtained in this way is stored in the predicate extension.
		Atom * headGroundAtom=groundTemplateAtomHead[atom_counter];
		(*atom)->ground(current_assignment,headGroundAtom);

		searchAtom=nullptr;
		for(auto atomSearcher:predicate_searchInsert_atomSearcher[atom_counter][0]){
			if(atomSearcher==nullptr) continue;
			searchAtom=atomSearcher->find(headGroundAtom);
			if(searchAtom!=nullptr)
				break;
		}

		if(searchAtom==nullptr){
			ground_new_atom = true;

			headGroundAtom->setFact(head_true);
			if(head_true) foundATrueAtomInDisjuction=true;
			if(sizeHead==1){
				PredicateExtension* predicateExt = predicateExtTable->getPredicateExt(headGroundAtom->getPredicate());
				Atom* newAtom = groundTemplateAtomHead[atom_counter]->clone();
				predicateExt->addAtom(newAtom,predicate_searchInsert_table[atom_counter][0].first);
				ground_rule->setAtomInHead(atom_counter, newAtom);
			}
			else
				newAtomsInHead[atom_counter]=true;

		}else{
			//TODO If searchAtom is true ??? {a|b. a.} o {a :- b(X,Y).b(1,1).b(1,2)|d.}

			//Previus atom is undef and now is true
			if(head_true && !searchAtom->isFact()){
				searchAtom->setFact(true);
				find_new_true_atom=true;
			}
			else if(searchAtom->isFact())
				foundATrueAtomInDisjuction=true;
			//Check if previus is false now is true ground_new atom i have put true
			ground_rule->setAtomInHead(atom_counter,searchAtom);
		}

	}

	// FIXME In the can simplification is added to choice rules, be aware that atoms in head are added in predicate extension
	// before the actual simplification (see the method groundChoice)
	if(isAChoiceRule || strongConstraint || isWeak || find_new_true_atom
			||(!foundATrueAtomInDisjuction && undefinedAtomInBody)
			||(sizeHead==1 && !isAChoiceRule && ground_new_atom)
			||(sizeHead>1 && !foundATrueAtomInDisjuction)){
		if(isWeak)
			ground_rule->setWeightLevelLabel(currentRule->groundWeightLevel(current_assignment));
		if(sizeHead>1){
			for (unsigned i = 0; i < sizeHead; ++i) {
				if (newAtomsInHead[i]) {
					Atom* headGroundAtom=groundTemplateAtomHead[i];
					Atom* searchAtom=nullptr;
					PredicateExtension* predicateExt = predicateExtTable->getPredicateExt(headGroundAtom->getPredicate());
					if(headAtomsWithTheSamePredicate[i]){
						for(auto atomSearcher:predicate_searchInsert_atomSearcher[i][0]){
							if(atomSearcher==nullptr) continue;
							searchAtom=atomSearcher->find(headGroundAtom);
							if(searchAtom!=nullptr)
								break;
						}
					}
					if(searchAtom==nullptr){
						Atom* newAtom = groundTemplateAtomHead[i]->clone();
						predicateExt->addAtom(newAtom,predicate_searchInsert_table[i][0].first);
						ground_rule->setAtomInHead(i, newAtom);
					}
					else
						ground_rule->setAtomInHead(i, searchAtom);
				}
			}
		}
		// If the rule has possible undef atoms in its body its printing is postponed to the end of grounding
		// So that:
		// 	- if the printing type is numeric we give to that atoms the right indices,
		// 	- independently from the output format, we simplify that atoms.
		// In this case ground_rule content cannot be changed (for example, substituting atoms in its body),
		// so after a ground rule is saved to be processed later, the ground_rule must be reinitialized coping
		// into it, the body of the saved ground rule.
		if(!atomsPossibleUndef.empty()){
			atomsPossibleUndefPositions.push_back(atomsPossibleUndef);
			rulesWithPossibleUndefAtoms.push_back(ground_rule);
			Rule* savedRule=ground_rule;
			ground_rule=new Rule(true, sizeHead, currentRule->getSizeBody());
			for(unsigned i=0;i<currentRule->getSizeBody();i++){
				Atom* atom=savedRule->getAtomInBody(i);
				if(atom!=nullptr && ((atom->isClassicalLiteral() && atom->isNegative()) || atom->isAggregateAtom()))
					substiteInGroundRule(i,atom->clone());
				else
					substiteInGroundRule(i,atom);
				ground_rule->setAtomToSimplifyInBody(i,savedRule->isAtomToSimplifyInBody(i));
			}
		}
		else{
			outputBuilder->onRule(ground_rule);
			trace_action_tag(grounding,1,cerr<<"Ground Rule Produced: ";ground_rule->print(cerr);cerr<<endl;);
		}
		if (printStats)
			rstats->groundNewRule(currentRule->getIndex());
	}
	if(strongConstraint && !undefinedAtomInBody){throw ConstrainException{};};

	return ground_new_atom;
}

bool BackTrackingGrounder::back() {
	direction=0;
	if(callFoundAssignment){
		callFoundAssignment=false;
		removeBindValueFromAssignment(atoms_bind_variables[index_current_atom]);
		return true;
	}

	callFoundAssignment = false;
	if (index_current_atom <=  0)
		return false;


	--index_current_atom;

	while (is_bound_atom[index_current_atom]){
		if (index_current_atom <= 0){

			return false;
		}

		if(atoms_bind_variables[index_current_atom].size()>0)
			removeBindValueFromAssignment(atoms_bind_variables[index_current_atom]);

		--index_current_atom;

	}

	removeBindValueFromAssignment(atoms_bind_variables[index_current_atom]);
	generateTemplateAtom();
	return true;
}

void BackTrackingGrounder::findVariablesMustList() {
	variableMustList.clear();
	vector<Term*> variables;
	if(currentRule->isChoiceRule()){
			Atom* head=currentRule->getAtomInHead(0);
			for (unsigned i = 0; i < head->getChoiceElementsSize(); i++)
					for(auto a:head->getChoiceElement(i)->getChoiceElement())
							for(auto t:a->getTerms())
									t->getVariableMustBeList(variables);
	}else
			for(auto atom:currentRule->getHead())
					for(auto t:atom->getTerms())
							t->getVariableMustBeList(variables);
	if(variables.empty())return;
	variableMustList.reserve(variables.size());
	for (auto& t : variables)
			variableMustList.push_back(t->getLocalVariableIndex());
}

void BackTrackingGrounder::inizialize(Rule* rule, unordered_set<index_object>* componentPredicateInHead) {
	direction=1;
	currentRule=rule;
	index_current_atom = 0;
	callFoundAssignment = false;
	for(auto atom:templateSetAtom) {if(atom!=nullptr) atom->deleteAtoms(); delete atom;}
	templateSetAtom.resize(rule->getSizeBody());
	for(auto& atom:templateSetAtom) atom=nullptr;
	is_bound_atom.clear();
	indexingArguments.clear();
	indexingArguments.resize(rule->getSizeBody());
	findBindVariablesRule();
	findSearchTables();
	if(rule->getSizeBody()>0)
		generateTemplateAtom();
	findBuiltinFastEvaluated();
	setAtomsMatch();
	findVariablesMustList();

	outputVariablesInAtoms.clear();
	outputVariablesInAtoms.resize(currentRule->getSizeBody());

	newAtomsInHead.clear();
	newAtomsInHead.resize(currentRule->getSizeHead(),false);
	headAtomsWithTheSamePredicate.clear();
	headAtomsWithTheSamePredicate.resize(currentRule->getSizeHead(),false);
	for(unsigned i=0;i<currentRule->getSizeHead();++i){
		Predicate* p1=currentRule->getAtomInHead(i)->getPredicate();
		for(unsigned j=i+1;j<currentRule->getSizeHead();++j){
			Predicate* p2=currentRule->getAtomInHead(j)->getPredicate();
			if(p2->getIndex()==p1->getIndex()){
				headAtomsWithTheSamePredicate[i]=true;
				headAtomsWithTheSamePredicate[j]=true;
			}
		}
	}


//	cerr<<"Head atoms"<<endl;
//	for(unsigned i=0;i<currentRule->getSizeHead();++i){
//		cerr<<headAtomsWithTheSamePredicate[i]<<" ";
//	}
//	cerr<<endl;


	if(ground_rule!=0)
		ground_rule->deleteGroundRule();
	ground_rule=(currentRule->isWeakConstraint())
			? new WeakConstraint(true,rule->getSizeBody(),currentRule->getWeight(),currentRule->getLevel(),currentRule->getLabel())
			: new Rule(true, rule->getSizeHead(), rule->getSizeBody());

	for(unsigned i=0;i<rule->getSizeBody();i++){
		auto atom=rule->getAtomInBody(i);
		if(atom->isExternalAtom() || atom->isBuiltIn())
				ground_rule->setAtomToSimplifyInBody(i);
	}

	atomsPossibleUndef.clear();

	if(!currentRule->isChoiceRule()){
		/// If the current rule is not a choice rule, then the vector groundTemplateAtomHead has to be filled with placeholder atoms as follows
		for(auto& atom:groundTemplateAtomHead){
			delete atom;
			atom=0;
		}
		groundTemplateAtomHead.resize(currentRule->getSizeHead(),0);

		unsigned i=0;
		for(auto headIt=currentRule->getBeginHead();headIt!=currentRule->getEndHead();++headIt,++i){
			groundTemplateAtomHead[i]=(*headIt)->clone();
		}
	}
	setDefaultAtomSearchers(rule,componentPredicateInHead);
}


void BackTrackingGrounder::createMatchInfoClassicalLiteral(unsigned int index_current_atom, Atom* current_atom) {
	/* For each term in the atom:
	 * 	If is a variable, check if is binded from this atom. If yes and is the first time that this variable compare then add
	 * 	in the bind terms. If is the second times that this variable compare than is a selection and put in other (Ex: a(X,X), 0 is bind 1 in other).
	 * 	If the bind variable is a bounder builtin(with the evaluation of this variable we can evaluate a builtin) add all the builtin evaluable in builtin.
	 *  If the variable is bounded of another atom then the term is bound else (the atom that bind the varaible is -1) is a temp variable (variable in the aggregate element)
	 *
	 *  If the term is total ground add the term in bound
	 *
	 *  If the term isn't anonymous then check if the variable in the term is bound, then the term have all variable bound and therefore the term is bound, else  put in other
	 *
	 *  At the end for each builtin we have to verify that the bind variable in the atom that compare in functional term not compare in one builtin. If compare we have
	 *  to evaluate the builtin with the old match function and then put the bounder builtin variable in other
	 */
	MatchInformation atomMatchInfo;
	set_term variablesInAtom;
	for (unsigned i = 0; i < current_atom->getTermsSize(); i++) {
		Term* term = current_atom->getTerm(i);
		TermType type = term->getType();
		if (type == VARIABLE) {
			//The first part of the if represents local variables in aggregates
			if ((variablesBinder[term->getLocalVariableIndex()]==-1 && currentRule->getAtomInBody(index_current_atom)->isAggregateAtom())
					|| (variablesBinder[term->getLocalVariableIndex()]== (int) (index_current_atom))) {
				if (variablesInAtom.count(term)) {
					//Variable binded in this atom and compare more than 2 times
					atomMatchInfo.other.push_back({ i });
				} else {
					variablesInAtom.insert(term);
					if (currentRule->getRuleInformation().isBounderBuiltin(term->getLocalVariableIndex())){
						for(auto builtin:currentRule->getRuleInformation().getBounderBuiltin(term->getLocalVariableIndex())){
								atomMatchInfo.builtin.push_back(builtin);
						}
					}
					atomMatchInfo.bind.push_back({ i,term->getLocalVariableIndex() });
				}
				if(currentRule->getRuleInformation().isCreatedDictionaryIntersection(term->getLocalVariableIndex()))
					atomMatchInfo.dictionaryIntersection.push_back({i});
			}else if(variablesBinder[term->getLocalVariableIndex()]!=-1)
				atomMatchInfo.bound.push_back({ i });
			else
				atomMatchInfo.bind.push_back({ i,term->getLocalVariableIndex() });

		} else if (term->isGround())
			atomMatchInfo.bound.push_back( { i });
		else if(type!=ANONYMOUS){
			//if the term contain bound variable is bound else go in others
			set_term variablesInTerm;
			term->getVariable(variablesInTerm);
			bool isBound = !term->contain(ANONYMOUS);
			for (Term* var : variablesInTerm)
				if ( variablesBinder[var->getLocalVariableIndex()]==-1 || variablesBinder[var->getLocalVariableIndex()]== (int) (index_current_atom)) {
					isBound = false;
					break;
				}
			if (isBound)
				atomMatchInfo.bound.push_back({ i });
			else
				atomMatchInfo.other.push_back({ i });

		}
		if((!Options::globalOptions()->isEnabledDictionaryIntersection() && Options::globalOptions()->isLookAhead())
				|| (!GroundingPreferences::getGroundingPreferences()->getAlignSubstitutions(currentRule) &&
						GroundingPreferences::getGroundingPreferences()->getLookAhead(currentRule))){
			if(current_atom->isClassicalLiteral() && !current_atom->isNegative()){
				atomMatchInfo.boundPreds.resize(current_atom->getTermsSize());
				for(auto v:atomMatchInfo.bind)
					for(unsigned i=index_current_atom+1;i<currentRule->getSizeBody();i++){
						Atom* atom=currentRule->getAtomInBody(i);
						if(atom->isClassicalLiteral() && !atom->isNegative()){
							for(unsigned k=0;k<atom->getTermsSize();k++){
								if(atom->getTerm(k)->getLocalVariableIndex()==v.second){
									atomMatchInfo.boundPreds[v.first].push_back({atom->getPredicate(),k});
								}
							}
						}
					}
			}
		}
	}
	if(atomMatchInfo.builtin.size()>0){
		set_term variablesInBuiltin;
		for(auto builtin:atomMatchInfo.builtin)
			builtin->getVariable(variablesInBuiltin);
		unordered_map<unsigned,unsigned> normalBind;
		for(auto bind:atomMatchInfo.bind)normalBind.insert({bind.second,bind.first});
		bool moveBinderBuiltinInOther=false;
		for(auto var:variablesInBuiltin){
			unsigned index=var->getLocalVariableIndex();
			if(variablesBinder[index]== (int) (index_current_atom)){
				if(!normalBind.count(var->getLocalVariableIndex())){
					moveBinderBuiltinInOther=true;
				}else
					atomMatchInfo.varUsedInBuiltin.push_back({normalBind[index],index});
			}
		}
		if(moveBinderBuiltinInOther){
			atomMatchInfo.varUsedInBuiltin.clear();
			atomMatchInfo.builtin.clear();
			auto& bind=atomMatchInfo.bind;
			bind.erase(std::remove_if(bind.begin(),bind.end(),[&](pair<unsigned,unsigned>& b){
				if(currentRule->getRuleInformation().isBounderBuiltin(b.second)){
					atomMatchInfo.other.push_back(b.first);
					return true;
				}
				return false;
			}));
		}
	}

	currentRule->addMatchInformation(atomMatchInfo,index_current_atom);
}

void BackTrackingGrounder::setAtomsMatch(){
	currentRule->clearAndResizeMatchInfo(currentRule->getSizeBody());
	unsigned int index_current_atom = 0;
	for (auto current_atom_it = currentRule->getBeginBody(); current_atom_it != currentRule->getEndBody(); ++current_atom_it,++index_current_atom) {
		//For each atom calculate the match information. If is an aggregate atom calculate the match info for each single element in aggregate element.
		Atom *current_atom = *current_atom_it;
		if(current_atom->isClassicalLiteral() && !current_atom->isNegative()){
			createMatchInfoClassicalLiteral(index_current_atom, current_atom);
		}else if(current_atom->isAggregateAtom()){
			for(unsigned i=0;i<current_atom->getAggregateElementsSize();i++){
				Atom * atomInAgg=current_atom->getAggregateElement(i)->getNafLiteral(0);
				createMatchInfoClassicalLiteral(index_current_atom, atomInAgg);
			}
		}
	}
//	currentRule->print();
//	for(unsigned i=0;i<atomMatchInformation.size();i++){
//		for(unsigned j=0;j<atomMatchInformation[i].size();j++){
//			cout<<"ATOM "<<i<<" "<<j<<endl;
//			cout<<"\t"<<"BOUND ";
//			for(auto bounds:atomMatchInformation[i][j].bound){
//				cout<<bounds<<",";
//			}
//			cout<<endl<<"\t"<<"BIND ";
//			for(auto bounds:atomMatchInformation[i][j].bind){
//				cout<<bounds.first<<",";
//			}
//			cout<<endl<<"\t"<<"BUILTIN ATOM ";
//			for(auto bounds:atomMatchInformation[i][j].builtin){
//				bounds->print();cout<<",";
//			}
//			cout<<endl<<"\t"<<"BUILTIN VARIABLE ";
//			for(auto bounds:atomMatchInformation[i][j].varUsedInBuiltin){
//				cout<<bounds.first<<",";
//			}
//			cout<<endl<<"\t"<<"OTHER ";
//			for(auto bounds:atomMatchInformation[i][j].other){
//				cout<<bounds<<",";
//			}
//			cout<<endl;
//		}
//	}
}

void BackTrackingGrounder::findBoundTerms(unsigned int index_current_atom, unsigned position, Atom* current_atom) {
	for (unsigned j = 0; j < current_atom->getTermsSize(); ++j) {
		Term* term = current_atom->getTerm(j);
		bool anonymous = current_atom->getTerm(j)->contain(TermType::ANONYMOUS);
		if(anonymous) continue;
		if (term->getType()==TermType::STRING_CONSTANT ||  term->getType()==TermType::NUMERIC_CONSTANT || term->getType()==TermType::SYMBOLIC_CONSTANT) {
			boundTermsInAtoms[index_current_atom][position].push_back(j);
			continue;
		}
		set_term vars;
		term->getVariable(vars);
		bool bound = true;
		for(auto v:vars){
			unsigned localIdx=v->getLocalVariableIndex();
			if(variablesBinder[localIdx]==-1 || (index_current_atom<currentRule->getSizeHead() && localIdx==0) || (index_current_atom>=currentRule->getSizeHead() && variablesBinder[localIdx]>=0 && unsigned(variablesBinder[localIdx])>=index_current_atom-currentRule->getSizeHead())){
				bound=false;
				break;
			}
		}
		if (bound)
			boundTermsInAtoms[index_current_atom][position].push_back(j);
	}
}

void BackTrackingGrounder::findBindVariablesRule() {

	set_term total_variable;
	unsigned int index_current_atom = 0;
	atoms_bind_variables.clear();
	atoms_bind_variables.resize(currentRule->getSizeBody());
	variablesBinder.setSize(currentRule->getVariablesSize(),-1);
	boundTermsInAtoms.clear();
	unsigned sizeHead=currentRule->getSizeHead();
	boundTermsInAtoms.resize(currentRule->getSizeBody()+sizeHead);

	//For each atom determines the bound and the bind variables
	for (auto current_atom_it = currentRule->getBeginBody(); current_atom_it != currentRule->getEndBody(); ++current_atom_it,++index_current_atom) {
		Atom *current_atom = *current_atom_it;

		set_term variablesInAtom =current_atom->getVariable();

		if(current_atom->isAggregateAtom())
			variablesInAtom=current_atom->getGuardVariable();

		atoms_bind_variables[index_current_atom].reserve(variablesInAtom.size());
		for (auto variable : variablesInAtom) {
			if (!total_variable.count(variable)){
				unsigned var=variable->getLocalVariableIndex();
				atoms_bind_variables[index_current_atom].push_back(var);
				variablesBinder[var]=index_current_atom;
			}
		}

		total_variable.insert(variablesInAtom.begin(),variablesInAtom.end());
		///Set true if is ground
		if(is_bound_atom.size()<=index_current_atom){
			is_bound_atom.push_back((current_atom->isBuiltIn() || (current_atom->isClassicalLiteral() && current_atom->isNegative()) || (atoms_bind_variables[index_current_atom].size()==0 && !current_atom->containsAnonymous()) ));
		}

		if(current_atom->isClassicalLiteral()){
			indexingArguments[index_current_atom].resize(1);
			boundTermsInAtoms[index_current_atom+sizeHead].push_back(vector<unsigned>());
			boundTermsInAtoms[index_current_atom+sizeHead][0].reserve(current_atom->getTermsSize());
			findBoundTerms(index_current_atom+sizeHead, 0, current_atom);
		}
		else if(current_atom->isAggregateAtom()){
			indexingArguments[index_current_atom].resize(current_atom->getAggregateElementsSize());
			for(unsigned ag=0;ag<current_atom->getAggregateElementsSize();++ag){
				boundTermsInAtoms[index_current_atom+sizeHead].push_back(vector<unsigned>());
				boundTermsInAtoms[index_current_atom+sizeHead][ag].reserve(current_atom->getTermsSize());
				findBoundTerms(index_current_atom+sizeHead, ag, current_atom->getAggregateElement(ag)->getNafLiteral(0));
			}
		}
	}

	if(Options::globalOptions()->getRewritingType()==COMPACT_NATIVE_CHOICE){
		index_current_atom=0;
		for (auto current_atom_it = currentRule->getBeginHead(); current_atom_it != currentRule->getEndHead(); ++current_atom_it,++index_current_atom) {
			Atom *current_atom = *current_atom_it;
			boundTermsInAtoms[index_current_atom].push_back(vector<unsigned>());
			boundTermsInAtoms[index_current_atom].resize(current_atom->getChoiceElementsSize());
			if(current_atom->isChoice()){
				for(unsigned i=0;i<current_atom->getChoiceElementsSize();++i){
					Atom* atomInChoice=current_atom->getChoiceElement(i)->getAtom(1);
					if(atomInChoice!=nullptr){
						boundTermsInAtoms[index_current_atom][i].reserve(atomInChoice->getTermsSize());
						findBoundTerms(index_current_atom, i, atomInChoice);
					}
				}
			}
		}
	}

	current_assignment.setSize(currentRule->getVariablesSize(),nullptr);


	trace_action_tag(backjumping,1,cerr<<"VARIABLES BINDER: ";
		for(unsigned i=0;i<currentRule->getVariablesSize();++i){
			cerr<<"var "<<i<<variablesBinder[i]<<endl;
		}
	);

	trace_action_tag(backtracking,1,
		cerr<<"BINDER OF ATOMS: ";
		int i=0;
		for(auto v:atoms_bind_variables){
			cerr<<"ATOM "<<i<<" [ ";
			for(auto binder:v){
				cerr<<binder<<" ";
			}
			cerr<<"] ";
			i++;
		}
		cerr<<endl;
	);

//	for(unsigned i=0;i<boundTermsInAtoms.size();++i){
//		if(i<sizeHead)
//			currentRule->getAtomInHead(i)->print();
//		else
//			currentRule->getAtomInBody(i-sizeHead)->print();
//		cout<<"\t";
//		for(auto j:boundTermsInAtoms[i])
//			for(auto a:j)
//				cout<<a<<" ";
//		cout<<endl;
//	}
}

void BackTrackingGrounder::findSearchTables() {
	unsigned sizeRule=currentRule->getSizeBody();
	//TODO AVOID CLEAR
	current_id_match.clear();
	current_id_match.reserve(sizeRule);
	current_id_match_iterator.reserve(sizeRule);
	for (unsigned index_current_atom = 0; index_current_atom < sizeRule; ++index_current_atom) {
		//find the table to search for each atom in the body
		current_id_match.emplace_back();
		if(predicate_searchInsert_table[currentRule->getSizeHead()+index_current_atom].size()==0){
			current_id_match[index_current_atom].push_back({0,NO_MATCH});
		}
		else{
			for(unsigned i=0;i<predicate_searchInsert_table[currentRule->getSizeHead()+index_current_atom].size();++i){
				current_id_match[index_current_atom].push_back({predicate_searchInsert_table[currentRule->getSizeHead()+index_current_atom][i].first,NO_MATCH});
			}
		}
		current_id_match_iterator[index_current_atom]=0;

	}

}

void BackTrackingGrounder::removeBindValueFromAssignment(const vector<index_object>& bind_variables) {
	for (auto variable : bind_variables)
		current_assignment[variable]=nullptr;

}

/*
 * For each atom in the body of the rule, find an a BuiltinAtom that is not an assignment. Then find the greates atom that bind some of the variable
 * in the builtin that is an a classical literal. After that the greatest binder is founded we have to find the rightmost variable ,in the classical literal, is also in the builtin.
 * Then the variable selected in the classical literal have to evaluate the built-in while do the matchTerm.
 */
void BackTrackingGrounder::findBuiltinFastEvaluated(){
	builtAlreadyEvaluated.clear();
	builtAlreadyEvaluated.resize(currentRule->getSizeBody(),false);
	matchBuiltin.clear();
	matchBuiltin.resize(currentRule->getSizeBody());
	currentRule->clearBounderBuiltin();
	unsigned index_current_atom=0;
	for (auto current_atom_it = currentRule->getBeginBody(); current_atom_it != currentRule->getEndBody(); ++current_atom_it,++index_current_atom) {
		Atom *atom=*current_atom_it;
		if(atom->isBuiltIn() && !atom->isAssignment()){
			//Find the greatest classical literal that bind some variable in the builtin
			auto varInBuiltin=atom->getVariable();
			unsigned positionAtomBinder=0;
			for(auto variable:varInBuiltin)
				if(variablesBinder[variable->getLocalVariableIndex()]>(int)positionAtomBinder)
					positionAtomBinder=variablesBinder[variable->getLocalVariableIndex()];

			Atom * possibleBinder=currentRule->getAtomInBody(positionAtomBinder);
			if(possibleBinder->isClassicalLiteral()){
				set_term variablePossibleBinder=possibleBinder->getVariable();
				if(possibleBinder->isClassicalLiteral()){
					vector<Term*> variable=possibleBinder->getVectorVariable();

					//Find the rightmost variable in the classical literal that is contained also in the builtin
					for(int i=variable.size()-1;i>=0;--i){
						Term *lastTerm=variable[i];
						if(lastTerm->getType()==VARIABLE && varInBuiltin.count(lastTerm) && variablesBinder[lastTerm->getLocalVariableIndex()]==(int)positionAtomBinder){
								//VARIABLE FINDED, then the builtin can be evaluated while matching the term in the classical literal
								builtAlreadyEvaluated[index_current_atom]=true;
								matchBuiltin[positionAtomBinder].push_back(atom);
								currentRule->addBounderBuiltin(lastTerm->getLocalVariableIndex(),atom);
								break;
							}
						}

					}
				}
			}
		}
	}


/*
 * Check if the rule contains only classical positive atoms,classical negative atoms, builtin (not assignment builtin) and all the variable is bind or total bound
 * If we have negation with no stratification is not cartesian
 */
bool BackTrackingGrounder::isCartesianProductRule(Rule *currentRule){
	unsigned index_current_atom=0;
	for (auto current_atom_it = currentRule->getBeginBody(); current_atom_it != currentRule->getEndBody(); ++current_atom_it,++index_current_atom) {
		Atom *current_atom = *current_atom_it;
		if(current_atom->isBuiltIn())
			continue;
		if(current_atom->isClassicalLiteral() && current_atom->isNegative()){
			if(StatementDependency::getInstance()->isPredicateNotStratified(current_atom->getPredicate()->getIndex()))
				return false;
			continue;
		}
		if(! current_atom->isClassicalLiteral())
			return false;
		if(atoms_bind_variables[index_current_atom].size()==0 && !current_atom->containsAnonymous())
			continue;
		unsigned sizeVar=0;
		for(unsigned i=0;i<current_atom->getTermsSize();i++){
			Term *term=current_atom->getTerm(i);
			if(!(term->getType()==VARIABLE || term->getType()==ANONYMOUS)){
				return false;
			}
			if(term->getType()==VARIABLE)
				sizeVar++;
		}
		if(sizeVar==0 || sizeVar!=atoms_bind_variables[index_current_atom].size())
			return false;

	}
	return true;
}

/*
 * ground a rule with only cartesian product. tables contain a vector of the atoms to search, and two index. The first
 * is the index of the table to search and the second is the the index of the AtomVector. Thsi method simulate the groundRule procedure
 * but is more light, because we know that the atom not share variable and then we have to search in the AtomVector all the combinations
 *
 */
bool BackTrackingGrounder::groundCartesian(Rule* rule,unordered_set<index_object>* componentPredicateInHead){
	unsigned size=rule->getSizeBody();
	vector<AtomCartesianInfo> tables;
	tables.resize(rule->getSizeBody());
	vector<bool> isBound(rule->getSizeBody(),false);

	int i=0;
	for(i=0;(unsigned)i<rule->getSizeBody();i++){
		Predicate *predicate=rule->getAtomInBody(i)->getPredicate();
		if(predicate==nullptr){
			ground_rule->setAtomToSimplifyInBody(i,true);
			continue;
		}
		if(rule->getAtomInBody(i)->isNegative() || atoms_bind_variables[i].size()==0){
			isBound[i]=true;
			continue;
		}
		for(auto table:predicate_searchInsert_table[i+rule->getSizeHead()]){
			AtomHistoryVector* t=predicateExtTable->getPredicateExt(predicate)->getTable(table.first);
			pair<unsigned,unsigned> startEnd={0,t->size()};
			if(componentPredicateInHead!=nullptr && componentPredicateInHead->count(predicate->getIndex()))
				startEnd=t->getElements(table.second,iteration);
			tables[i].addTable(t,startEnd.first,startEnd.second);
			tables[i].indexTable=startEnd.first;
//			if(table.second==ALL){
//				tables[i].addTable(t,0,t->size());
//			}if(table.second==OLD){
//				tables[i].addTable(t,0,t->getIndexIteration());
//			}if(table.second==NEW){
//				tables[i].addTable(t,t->getIndexIteration(),t->size());
//				tables[i].indexTable=t->getIndexIteration();
//			}
		}
	}

	bool ground_new_atom=false;
	unsigned sizeHead=currentRule->getSizeHead();
	i=0;
	direction=true;
	while(true){
		if(i==-1)
			break;
		if((unsigned)i==size){
			if(foundAssignment())
				ground_new_atom=true;
			--i;
			direction=false;
			continue;
		}

		if(currentRule->getAtomInBody(i)->isBuiltIn()){
			if(builtAlreadyEvaluated[i]){
				if(direction)
					++i;
				else
					--i;
				continue;
			}
			if(!direction){
				removeBindValueFromAssignment(atoms_bind_variables[i]);
				--i;
				continue;
			}
			ground_rule->setAtomToSimplifyInBody(i);
//			currentRule->getAtomInBody(i)->ground(current_assignment,templateSetAtom[i]);
			if(currentRule->getAtomInBody(i)->groundAndEvaluate(current_assignment)){
				++i;
				continue;
			}else{
				--i;
				direction=false;
				continue;
			}
		}

		if(isBound[i]){
			if(!direction){
				i--;
				continue;
			}
			Atom* find=nullptr;
			currentRule->getAtomInBody(i)->ground(current_assignment,templateSetAtom[i]);
			Atom *tmpAtom=templateSetAtom[i];
			for(unsigned j=0;j<predicate_searchInsert_atomSearcher[i+sizeHead][0].size();j++){
				find=predicate_searchInsert_atomSearcher[i+sizeHead][0][j]->find(tmpAtom,{predicate_searchInsert_table[i+sizeHead][j].second,iteration});
				if(find!=nullptr)break;
			}
			bool isNegative=currentRule->getAtomInBody(i)->isNegative();
			if((isNegative && (find==nullptr || !find->isFact()) ) || (!isNegative && find!=nullptr)){
				bool simplify=(find!=nullptr)?find->isFact():true;
				if(find!=nullptr){
					if(isNegative){
						tmpAtom->setIndex(find->getIndex());
						substiteInGroundRule(i,tmpAtom->clone());

					}else
						ground_rule->setAtomInBody(i,find);
				}
				ground_rule->setAtomToSimplifyInBody(i,simplify);
				i++;
			}else{
				direction=false;
				i--;
			}
			continue;
		}

		auto& table = tables[i].table;
		auto& indexTable=tables[i].indexTable;

		if(table>=tables[i].tables.size()){
			table=0;
			indexTable = tables[i].tables[0].start;
			--i;
			direction=false;
			continue;
		}

		AtomHistoryVector *vec=tables[i].tables[table].table;
		unsigned end=tables[i].tables[table].end;
		if(indexTable>=end){
			table++;
			indexTable=(table<tables[i].tables.size())?tables[i].tables[table].start:0;
			continue;
		}

		bool match=true;
		Atom * atom=(*vec)[indexTable];
//		Atom *nonGroundAtom=rule->getAtomInBody(i);
		auto& mi=currentRule->getRuleInformation().getMatchInformation(i,0);

		auto& terms=atom->getTerms();

		for(unsigned i=0;i<mi.boundPreds.size()&&match;++i)
			for(auto pair:mi.boundPreds[i]){
				if(!PredicateExtTable::getInstance()->getPredicateExt(pair.first)->getPredicateInformation()->isPresent(pair.second,terms[i])){
					match=false;
					break;
				}
			}

		if(!match){indexTable++; continue;}

		if(!mi.builtin.empty()){
			for(auto var:mi.varUsedInBuiltin)
				current_assignment[var.second]=terms[var.first];

			for(auto builtin:mi.builtin){
				match = builtin->groundAndEvaluate(current_assignment);
				if (!match)break;
			}

			for(auto var:mi.varUsedInBuiltin)
				current_assignment[var.second]=nullptr;
		}

		for(auto bind:mi.bind)
			current_assignment[bind.second]=terms[bind.first];

//		for(unsigned j=0;j<atom->getTermsSize();j++){
//			if(atom->getTerm(j)->getType()==ANONYMOUS)continue;
//			current_assignment[nonGroundAtom->getTerm(j)->getLocalVariableIndex()]=atom->getTerm(j);
//			if(currentRule->getRuleInformation().isBounderBuiltin(nonGroundAtom->getTerm(j)->getLocalVariableIndex())){
//				match=AtomSearcher::evaluateFastBuiltin(currentRule->getRuleInformation(),nonGroundAtom->getTerm(j)->getLocalVariableIndex(),current_assignment,atom->getTerm(j));
//				if(!match)break;
//			}
//		}

		indexTable++;

		if(match){
			ground_rule->setAtomInBody(i,atom);
			ground_rule->setAtomToSimplifyInBody(i,atom->isFact());
			++i;
			direction=true;
		}
	}


	return ground_new_atom;
}

bool BackTrackingGrounder::groundAggregate() {
	Atom *aggregateAtom=templateSetAtom[index_current_atom];


	//Check if we can simplify without evaluate the aggregate
	bool alwaysTrue;
	if(aggregateAtom->checkAggregateSumCountStringGuard(alwaysTrue) || aggregateAtom->checkAggregateCountNegativeGuard(alwaysTrue)){
		substiteInGroundRule(index_current_atom,nullptr);
		ground_rule->setAtomToSimplifyInBody(index_current_atom,true);
		return alwaysTrue;
	}

	Atom *ground_aggregate;
	vector<unsigned> emptyVector;
	if(aggregateAtom->isAnAssigment() && ground_rule->getAtomInBody(index_current_atom)!=nullptr){
		//An assignment is in the lower guard
		//The atom is already grounded, just change the guard
		ground_aggregate=ground_rule->getAtomInBody(index_current_atom);
		bool finish=0;
		Term* val=ground_aggregate->generateNextCombination(finish);
		if(finish){
			substiteInGroundRule(index_current_atom,nullptr);
			return false;
		}

		ground_aggregate->setFirstGuard(val);
		current_assignment[aggregateAtom->getFirstGuard()->getLocalVariableIndex()]=val;
		return true;
	}
	//Create a ground aggregate empty and set the aggregate to not simplify
	ground_aggregate=new AggregateAtom(aggregateAtom->getFirstGuard(),aggregateAtom->getFirstBinop(),aggregateAtom->getSecondGuard(),aggregateAtom->getSecondBinop(),aggregateAtom->getAggregateFunction(),aggregateAtom->isNegative());
	ground_rule->setAtomToSimplifyInBody(index_current_atom,false);

	set_term variablesInAtom;
	unsigned numTables=predicate_searchInsert_table[index_current_atom+currentRule->getSizeHead()].size();

	ResultEvaluation result=UNDEF;
	for(unsigned i=0;i<aggregateAtom->getAggregateElementsSize()&&result==UNDEF;i++){

		// For each atom in the aggregate element (is assumed to be one because the rewriting)
		// search in the table of fact and nofact the extension and put in the ground aggregate all the extension
		// with a new aggregate element
		// For reasoning of correctness the current assignment have to be copied for maintaining the current assignment

		Atom* atom=aggregateAtom->getAggregateElement(i)->getNafLiteral(0);
		variablesInAtom=atom->getVariable();

		PredicateExtension* predicateExtension=predicateExtTable->getPredicateExt(atom->getPredicate());

		int counter=0;
		for(unsigned j=0;j<numTables&&result==UNDEF;j++){

			unsigned table=predicate_searchInsert_table[index_current_atom+currentRule->getSizeHead()][j].first;
			IndexingStructure *searcher=predicate_searchInsert_atomSearcher[index_current_atom+currentRule->getSizeHead()][i][j];
			AtomSearcher* atomSearcher=predicateExtension->getAtomSearcher(table);

			if(searcher==nullptr) continue;
			bool find=false;
			Atom* atomFound=nullptr;

			//Each aggregate element have one atom with no relation with the other atoms in the aggregate elements, then we can
			//overwrite the general iterator in the Atom Searcher with index_current_atom
			const MatchInformation& mi=currentRule->getRuleInformation().getMatchInformation(index_current_atom,i);
			atomSearcher->firstMatch(index_current_atom,atom,current_assignment,atomFound,currentRule->getRuleInformation(),searcher,indexingArguments[index_current_atom][i],emptyVector,{ALL,0},mi);
			find=(atomFound!=nullptr);
			while(find){
				counter++;

				AggregateElement *ground_aggregateElement=new AggregateElement;
				ground_aggregateElement->addNafLiterals(atomFound);

				//Add the id ground term in the ground aggregate element
				for(auto term_aggregateElement:aggregateAtom->getAggregateElement(i)->getTerms()){
					auto termType=term_aggregateElement->getType();
					if(termType==NUMERIC_CONSTANT || termType==STRING_CONSTANT || termType==SYMBOLIC_CONSTANT)
						ground_aggregateElement->addTerm(term_aggregateElement);
					else if(termType==VARIABLE)
						ground_aggregateElement->addTerm(current_assignment[term_aggregateElement->getLocalVariableIndex()]);
					else if(termType==FUNCTION){
						ground_aggregateElement->addTerm(term_aggregateElement->substitute(current_assignment));
					}
				}

				ground_aggregate->addAggregateElement(ground_aggregateElement);
				result=ground_aggregate->partialEvaluate();

				//Remove bind variables of template atom in the ground aggregate element
				//IF REMOVE TEMPLATE ATOM THIS NOT WORK because the variables in the template atom
				//are bind variable
				for(auto variableBind:variablesInAtom)
					current_assignment[variableBind->getLocalVariableIndex()]=nullptr;

				if(result!=UNDEF || atom->isGround())break;

				atomSearcher->nextMatch(index_current_atom,atom,current_assignment,atomFound,currentRule->getRuleInformation(),emptyVector,mi);
				find=(atomFound!=nullptr);
			}
		}
	}

	if(result==UNDEF)
		result=ground_aggregate->finalEvaluate();

	//If is a first assignment set the initial value of the guard to the partial value
	if(ground_aggregate->isAnAssigment()){
		bool finish=0;
		Term* val=ground_aggregate->generateNextCombination(finish);

		current_assignment[ground_aggregate->getFirstGuard()->getLocalVariableIndex()]=val;
		ground_aggregate->setFirstGuard(val);

		if(!ground_aggregate->isUndefAssignment())
			ground_rule->setAtomToSimplifyInBody(index_current_atom);
	}

	substiteInGroundRule(index_current_atom,nullptr);

	//( ground_aggregate->getAggregateElementsSize()==0 && !aggregateAtom->isAnAssigment()) ||
	if(result!=UNDEF)
	{
		delete ground_aggregate;
		ground_rule->setAtomToSimplifyInBody(index_current_atom);
		if(result==UNSATISFY)
			return false;

		//Aggregate is satisfy
		return true;
	}

	ground_rule->setAtomInBody(index_current_atom,ground_aggregate);
	return true;
}

void BackTrackingGrounder::groundChoice(bool& find_new_true_atom,bool& ground_new_atom){
	Atom* searchAtom=nullptr;
	Atom* ground_choice=new Choice;
	Atom *choice=currentRule->getAtomInHead(0);

	for(unsigned i=0;i<choice->getChoiceElementsSize();i++){

		Atom *atom_in_choice=choice->getChoiceElement(i)->getFirstAtom();

		Atom *headGroundAtom=nullptr;
		atom_in_choice->ground(current_assignment,headGroundAtom);
		PredicateExtension* predicateExt=predicateExtTable->getPredicateExt(headGroundAtom->getPredicate());
//		searchAtom=predicateExt->getAtom(headGroundAtom);

//		for(unsigned j=i*numTables;j<(i*numTables)+numTables;j++){
		for(unsigned j=0;j<predicate_searchInsert_atomSearcher[0][i].size();++j){
			auto *searcher=predicate_searchInsert_atomSearcher[0][i][j];
			if(searcher==nullptr) continue;
			searchAtom=searcher->find(headGroundAtom);
			if(searchAtom!=nullptr){
				break;
			}
		}

		if(searchAtom==nullptr){
			ground_new_atom = true;

			headGroundAtom->setFact(false);
//			for(unsigned i=0;i<predicate_searchInsert_table[0].size();++i)
				predicateExt->addAtom(headGroundAtom,predicate_searchInsert_table[0][0].first);

			ground_choice->addSingleChoiceElement(headGroundAtom);
		}else{
			delete headGroundAtom;

			//Check if previous is false now is true ground_new atom i have put true
			ground_choice->addSingleChoiceElement(searchAtom);
		}

	}
	Atom* currentGroundChoice=ground_rule->getAtomInHead(0);
	if(currentGroundChoice!=0)
		delete currentGroundChoice;
	ground_rule->setAtomInHead(0,ground_choice);
}


void BackTrackingGrounder::groundChoiceNatively(bool& find_new_true_atom,bool& ground_new_atom){
	Atom* ground_choice=new Choice;
	Atom *choice=currentRule->getAtomInHead(0);
	unsigned numChoiceElements=choice->getChoiceElementsSize();
	vector<Atom*> templateAtomsInChoice;
	templateAtomsInChoice.resize(numChoiceElements,nullptr);

	for(unsigned i=0;i<numChoiceElements;i++){
		Atom* innerAtom=choice->getChoiceElement(i)->getAtom(1);
		//TODO Se il primo atomo è un built in
		if(innerAtom!=nullptr){
			if(innerAtom->isBuiltIn()){
//				vector<unsigned> bindVariables;
//				for(auto v:innerAtom->getVariable()){
//					if(variablesBinder[v->getLocalVariableIndex()]==-1)
//						bindVariables.push_back(v->getLocalVariableIndex());
//				}
				bool evaluation = innerAtom->groundAndEvaluate(current_assignment);
				if(evaluation){
					groundChoiceFirstAtom(choice->getChoiceElement(i)->getFirstAtom(),i,ground_new_atom,ground_choice);
//					removeBindValueFromAssignment(bindVariables);
				}
			}
			else if(innerAtom->isClassicalLiteral()){
				Atom* atomFound=nullptr;
				vector<unsigned> bind_variables;
				for(unsigned a=0;a<innerAtom->getPredicate()->getArity();++a){
					bool bind=true;
					for(auto b: boundTermsInAtoms[0][i]){
						if(a==b){
							bind=false;
							break;
						}
					}
					if(bind){
						set_term vars;
						innerAtom->getTerm(a)->getVariable(vars);
						for(auto v:vars)
							bind_variables.push_back(v->getLocalVariableIndex());
					}
				}
				if(templateAtomsInChoice[i]==nullptr)
					innerAtom->ground(current_assignment,templateAtomsInChoice[i]);
				PredicateExtension* predicateExt=predicateExtTable->getPredicateExt(innerAtom->getPredicate());
				bool firstMatch=true;
				unsigned table=0;
				IndexingStructure* indexingStructure=0;

				while(table<=1){
					AtomSearcher* atomSearcher=predicateExt->getAtomSearcher(table);
					unsigned indexStruct=predicate_searchInsert_atomSearcher[0][i].size()-(1+table);
					indexingStructure=predicate_searchInsert_atomSearcher[0][i][indexStruct];

					if(indexingStructure==nullptr){
						table++;
						firstMatch=true;
						continue;
					}

					if(bind_variables.empty()){
						atomFound=atomSearcher->findGroundAtom(templateAtomsInChoice[i],indexingStructure);
						table++;
						if((atomFound==nullptr && !innerAtom->isNegative()) || (atomFound!=nullptr && innerAtom->isNegative()))
							continue;
					}
					else{
						//FIXME If we want to maintain this function calculate, like atom in the body, the match information for the inner atom in the choice
						MatchInformation mi;
						for(unsigned j=0;j<templateAtomsInChoice[i]->getTermsSize();j++) mi.other.push_back(j);
						if(firstMatch){
							atomSearcher->firstMatch(i,templateAtomsInChoice[i],current_assignment,atomFound,currentRule->getRuleInformation(),indexingStructure,0,vector<unsigned>(),{ALL,0},mi);
							if(atomFound==nullptr){
								table++;
								continue;
							}
							else
								firstMatch=false;
						}
						else{
							atomSearcher->nextMatch(i,templateAtomsInChoice[i],current_assignment,atomFound,currentRule->getRuleInformation(),vector<unsigned>(),mi);
							if(atomFound==nullptr){
								table++;
								firstMatch=true;
								continue;
							}
						}
					}
					groundChoiceFirstAtom(choice->getChoiceElement(i)->getFirstAtom(),i,ground_new_atom,ground_choice);
					if(atomFound!=nullptr && !atomFound->isFact()){
						unsigned lastChoiceElement=ground_choice->getChoiceElementsSize();
						if(lastChoiceElement>0)
							ground_choice->getChoiceElement(lastChoiceElement-1)->add(atomFound);
					}
					removeBindValueFromAssignment(bind_variables);
				}
			}
		}
		else
			groundChoiceFirstAtom(choice->getChoiceElement(i)->getFirstAtom(),i,ground_new_atom,ground_choice);
	}


	Atom* currentGroundChoice=ground_rule->getAtomInHead(0);
	if(currentGroundChoice!=0)
		delete currentGroundChoice;
	ground_rule->setAtomInHead(0,ground_choice);

	for(unsigned i=0;i<numChoiceElements;i++){
		delete templateAtomsInChoice[i];
	}

}


void BackTrackingGrounder::groundChoiceFirstAtom(Atom* firstAtom, unsigned i, bool& ground_new_atom, Atom* ground_choice){
	Atom* searchAtom=nullptr;
	Atom* headGroundAtom=nullptr;
	firstAtom->ground(current_assignment,headGroundAtom);
	PredicateExtension* predicateExt=predicateExtTable->getPredicateExt(headGroundAtom->getPredicate());

	unsigned stop=predicate_searchInsert_atomSearcher[0][i].size();
	if(currentRule->getAtomInHead(0)->getChoiceElement(i)->getSize()>1)
		stop=predicate_searchInsert_atomSearcher[0][i].size()-2;
	for(unsigned j=0;j<stop;++j){
		auto *searcher=predicate_searchInsert_atomSearcher[0][i][j];
		if(searcher==nullptr) continue;
		searchAtom=searcher->find(headGroundAtom);
		if(searchAtom!=nullptr){
			break;
		}
	}

	if(searchAtom==nullptr){
		ground_new_atom = true;
		headGroundAtom->setFact(false);
		predicateExt->addAtom(headGroundAtom,predicate_searchInsert_table[0][0].first);
		ground_choice->addSingleChoiceElement(headGroundAtom);
	}else{
		delete headGroundAtom;

		//Check if previous is false now is true ground_new atom i have put true
		ground_choice->addSingleChoiceElement(searchAtom);
	}
}


void BackTrackingGrounder::setIndexingStructureInHeadAndBody(unsigned position, unsigned atomPos,
		PredicateExtension* predicateExtension, Predicate* predicate,
		unordered_set<index_object>* componentPredicateInHead, unsigned table) {
		predicateExtension->getAtomSearcher(table)->setSizeResultVector(
				currentRule->getSizeBody());
		IndexingStructure* atomSearcher=nullptr;
		if (boundTermsInAtoms[position][atomPos].size()	== predicate->getArity()) {
			if(componentPredicateInHead==nullptr || !componentPredicateInHead->count(predicate->getIndex())){
				auto atomSearcherMAP = predicateExtension->getIndexingStructure(
						table, MAP);
				auto atomSearcherHASH = predicateExtension->getIndexingStructure(
						table, HASHSET);
				if (atomSearcherMAP != nullptr)
					atomSearcher = atomSearcherMAP;
				else if (atomSearcherHASH != nullptr)
					atomSearcher = atomSearcherHASH;
				else {
					vector<unsigned> terms(1, 0);
					atomSearcher = predicateExtension->addAtomSearcher(table, MAP,
							&terms);
				}

			}else{
				auto atomSearcherHISTORYHASH = predicateExtension->getIndexingStructure(table, HISTORY_HASHSET);
				if (atomSearcherHISTORYHASH != nullptr)
					atomSearcher=atomSearcherHISTORYHASH;
				else {
					vector<unsigned> terms(1, 0);
					atomSearcher = predicateExtension->addAtomSearcher(table, HISTORY_HASHSET,&terms);
				}

			}
		} else if (!boundTermsInAtoms[position][atomPos].empty()) {
			Atom* atom=0;
			if(position>=currentRule->getSizeHead()){
				if(atomPos==0)
					atom=currentRule->getAtomInBody(position-currentRule->getSizeHead());
				else
					atom=currentRule->getAtomInBody(position-currentRule->getSizeHead())->getAggregateElement(atomPos)->getNafLiteral(0);
			}
			else {
				if(atomPos==0)
					atom=currentRule->getAtomInHead(position);
				else
					atom=currentRule->getAtomInHead(position)->getChoiceElement(atomPos)->getAtom(1);
			}
			vector<unsigned> idxTerms;
			if(GroundingPreferences::getGroundingPreferences()->checkAtomIndexed(currentRule,atom,boundTermsInAtoms[position][atomPos],idxTerms)){
				if (componentPredicateInHead != nullptr && componentPredicateInHead->count(predicate->getIndex())) {
					if (boundTermsInAtoms[position][atomPos].size()>=2) {
						vector<unsigned> indexingTerm;
						indexingTerm.push_back(boundTermsInAtoms[position][atomPos][0]);
						indexingTerm.push_back(boundTermsInAtoms[position][atomPos][1]);
						atomSearcher = predicateExtension->addAtomSearcher(table,
								MAP_PAIR_HISTORY_VECTOR, &indexingTerm, true);
					} else {
						vector<unsigned> indexingTerm;
						indexingTerm.push_back(boundTermsInAtoms[position][atomPos][0]);
						atomSearcher = predicateExtension->addAtomSearcher(table,
								MAP_HISTORY_VECTOR, &indexingTerm, true);
					}
				} else {
					if (idxTerms.size()>2) {
						atomSearcher = predicateExtension->addAtomSearcher(table,
								MULTIPLE_TERMS, &idxTerms);
					} else if (idxTerms.size()==2) {
						atomSearcher = predicateExtension->addAtomSearcher(table, DOUBLEMAP, &idxTerms);
					} else {
						atomSearcher = predicateExtension->addAtomSearcher(table, MAP, &idxTerms);
					}
				}
				predicate_searchInsert_atomSearcher[position][atomPos].push_back(atomSearcher);
				return;
			}
//			int indexingTermSetByUser =
//					Options::globalOptions()->getPredicateIndexTerm(
//							predicate->getName());
			unsigned bestArg = 0;
			unsigned bestSelectivityArg = 0;
			unsigned nextBestArg = 0;
			unsigned nextBestSelectivityArg = 0;
			PredicateInformation* predicateInfo =
					predicateExtTable->getPredicateExt(predicate)->getPredicateInformation();
			for (auto boundArg : boundTermsInAtoms[position][atomPos]) {
//				if (indexingTermSetByUser >= 0
//						&& boundArg == unsigned(indexingTermSetByUser)) {
//					bestArg = indexingTermSetByUser;
//					break;
//				}
				if (predicateInfo->getSelectivity(boundArg)
						> bestSelectivityArg) {
					nextBestSelectivityArg = bestSelectivityArg;
					nextBestArg = bestArg;
					bestSelectivityArg = predicateInfo->getSelectivity(
							boundArg);
					bestArg = boundArg;
				} else if (predicateInfo->getSelectivity(boundArg)
						== bestSelectivityArg
						|| predicateInfo->getSelectivity(boundArg)
								> nextBestSelectivityArg) {
					nextBestSelectivityArg = predicateInfo->getSelectivity(
							boundArg);
					nextBestArg = boundArg;
				}
			}
			vector<unsigned> indexingTerm(2);
			indexingTerm[0] = bestArg;
			//			cout<<"---> Predicate: "<<predicate->getName()<<endl;
			//			for(auto e: indexingTerm)
			//				cout<<e<<" ";
			//			cout<<endl;
			//			For FULL INDEXING ON EACH SINGLE ARGUMENT:
			//			atomSearcher=predicateExtension->addFullIndexAtomSearcher(table,(componentPredicateInHead!=nullptr && componentPredicateInHead->count(predicate->getIndex())));
			//			indexingArguments[position-currentRule->getSizeHead()][atomPos]=bestArg;
			if (componentPredicateInHead != nullptr
					&& componentPredicateInHead->count(predicate->getIndex())) {
				if (nextBestSelectivityArg > 0) {
					indexingTerm[1] = nextBestArg;
					atomSearcher = predicateExtension->addAtomSearcher(table,
							MAP_PAIR_HISTORY_VECTOR, &indexingTerm, true);
				} else {
					atomSearcher = predicateExtension->addAtomSearcher(table,
							MAP_HISTORY_VECTOR, &indexingTerm, true);
				}
			} else {
				if (nextBestSelectivityArg > 0) {
					indexingTerm[1] = nextBestArg;
					atomSearcher = predicateExtension->addAtomSearcher(table,
							DOUBLEMAP, &indexingTerm);
				} else {
					atomSearcher = predicateExtension->addAtomSearcher(table,
							&indexingTerm);
				}
			}
		} else {
			if (componentPredicateInHead != nullptr
					&& componentPredicateInHead->count(predicate->getIndex()))
				atomSearcher = predicateExtension->addAtomSearcher(table,
						DEFAULT_RECURSIVE, nullptr, true);
			else{
				atomSearcher = predicateExtension->addAtomSearcher(table,
						DEFAULT, nullptr, false);
			}
		}
		predicate_searchInsert_atomSearcher[position][atomPos].push_back(atomSearcher);
}

void BackTrackingGrounder::createAtomSearchersForPredicateBody(unsigned position, unsigned atomPos, Predicate* predicate, unordered_set<index_object>* componentPredicateInHead){
	PredicateExtension* predicateExtension = predicateExtTable->getPredicateExt(predicate);
	if(!predicate_searchInsert_atomSearcher[position][atomPos].empty() && atomPos==0){
		for(auto tablePair:predicate_searchInsert_table[position]){
			unsigned table=tablePair.first;
			predicateExtension->getAtomSearcher(table)->setSizeResultVector(currentRule->getSizeBody());
//			indexingArguments[position-currentRule->getSizeHead()][atomPos]=??;
		}
		return;
	}
	for (auto tablePair : predicate_searchInsert_table[position]) {
		setIndexingStructureInHeadAndBody(position, atomPos, predicateExtension, predicate, componentPredicateInHead, tablePair.first);
	}
}

void BackTrackingGrounder::createAtomSearchersForPredicateHead(unsigned position, unsigned choiceElementPos, Predicate* predicate, unordered_set<index_object>* componentPredicateInHead, bool firstAtom){
	unsigned sizeRule=currentRule->getSizeBody();
	PredicateExtension* predicateExtension = predicateExtTable->getPredicateExt(predicate);

	if(!firstAtom){
		if(predicateExtension->getPredicateExtentionSize(NOFACT))
			setIndexingStructureInHeadAndBody(position, choiceElementPos, predicateExtension, predicate, componentPredicateInHead, NOFACT);
		else
			predicate_searchInsert_atomSearcher[position][choiceElementPos].push_back(0);
		if(predicateExtension->getPredicateExtentionSize(FACT))
			setIndexingStructureInHeadAndBody(position, choiceElementPos, predicateExtension, predicate, componentPredicateInHead, FACT);
		else
			predicate_searchInsert_atomSearcher[position][choiceElementPos].push_back(0);
	}
	else{
		for(unsigned i=0;i<=predicate_searchInsert_table[position][0].first;++i){
			predicateExtension->getAtomSearcher(i)->setSizeResultVector(sizeRule);
			vector<unsigned> indexing(1,0);
			IndexingStructure* atomSearcher=predicateExtension->addAtomSearcher(i,MAP,&indexing);
			if(i==predicate_searchInsert_table[position][0].first || predicateExtension->getPredicateExtentionSize(i))
				predicate_searchInsert_atomSearcher[position][choiceElementPos].push_back(atomSearcher);
			else
				predicate_searchInsert_atomSearcher[position][choiceElementPos].push_back(0);
		}
	}
}

} /* namespace grounder */
} /* namespace DLV2 */

