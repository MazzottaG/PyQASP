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
 * BackJumpingGrounder.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: jessica
 */

#include "BackJumpingGrounder.h"
#include "../../util/Utils.h"

namespace DLV2 {
namespace grounder {

bool BackJumpingGrounder::back() {

	direction=0;
	callFoundAssignment = false;
	if (index_current_atom <  0)
		return false;

	int current_index=index_current_atom;
	switch(current_status){
		case SUCCESSFULL:
			backFromSolutionFound();
			break;
		case FIRST_MATCH:
			backFromFirstMatch();
			break;
		case NEXT_MATCH:
			backFromNextMatch();
			break;
	}

	if (index_current_atom <  0)
		return false;

	for (int i=index_current_atom; i<=current_index; ++i){
		if(atoms_bind_variables[i].size()>0)
			removeBindValueFromAssignment(atoms_bind_variables[i]);
		if(i>index_current_atom && !is_bound_atom[i]){
			Predicate *currentPredicate=currentRule->getAtomInBody(i)->getPredicate();
			for (unsigned j = 0; j < current_id_match[i].size()&&currentPredicate!=nullptr; ++j) {
				current_id_match[i][j].second=NO_MATCH;
			}
			current_id_match_iterator[i]=0;
			if(currentRule->getAtomInBody(i)->isAggregateAtom())
				substiteInGroundRule(i,nullptr);
		}
	}

	generateTemplateAtom();
	return true;

}

bool BackJumpingGrounder::foundAssignment() {
	current_status=SUCCESSFULL;
	return BackTrackingGrounder::foundAssignment();
}

bool BackJumpingGrounder::firstMatch() {
	current_status=FIRST_MATCH;
	return BackTrackingGrounder::firstMatch();
}

bool BackJumpingGrounder::nextMatch() {
	current_status=NEXT_MATCH;
	return BackTrackingGrounder::nextMatch();
}

void BackJumpingGrounder::inizialize(Rule* rule, unordered_set<index_object>* componentPredicateInHead) {
	trace_msg(backjumping,1,"---> INIZIALIZE");

	for(auto atom:rule->getBody()){
		if(!atom->isClassicalLiteral())continue;
		if(!atom->getPredicate()->isSolved()){
			auto& terms=atom->getTerms();
			if(atom->isGround())continue;
			if(terms.size()){
				set_term varTerms;
				for(auto t:terms){
					t->getVariable(varTerms);
					if(varTerms.size())break;
				}
				if(!varTerms.size()){
					set_term variablesInRule;
					rule->getVariables(variablesInRule);
					atom->setTerm(0,TermTable::getInstance()->generateVariableAuxTerm(variablesInRule));
				}
			}
		}
	}



	BackTrackingGrounder::inizialize(rule,componentPredicateInHead);

	closestSuccessfulBinder_index=-1;
	current_status=SUCCESSFULL;

	historyBackFromFirst.setSize(currentRule->getSizeBody(),-2);
	historyBackOutputVars.setSize(currentRule->getSizeBody(),-2);
	historyBackFromSolutionFound=-2;


	outputVariables=&(rule->getOutputVariables());
	atomsVariables.clear();

	failureMap.resize(current_assignment.size(),false);
	unsigned position=0;
	for (auto it=currentRule->getBeginBody();it!=currentRule->getEndBody(); ++it,++position) {
		Atom* atom=*it;
		const set_term& variablesInAtom=atom->getVariable();
		atomsVariables.push_back(variablesInAtom);
		if(atom->isClassicalLiteral()){
			//Add also the variable in the builtin that the atom evaluate inside the matchTerm
			if(!matchBuiltin[position].empty())
				for(auto builtinAtom:matchBuiltin[position]){
					auto variablesInBuiltIn=builtinAtom->getVariable();
					atomsVariables[position].insert(variablesInBuiltIn.begin(),variablesInBuiltIn.end());
				}
		}
	}

	if(!Options::globalOptions()->isDisabledAnonymousFilter()){
		position=0;
		for (auto it=currentRule->getBeginBody();it!=currentRule->getEndBody(); ++it,++position) {
			Atom* atom=*it;
			if(atom->isClassicalLiteral() && atom->getPredicate()->isSolved()){
				outputVariablesInAtoms[position].reserve(atom->getTermsSize());
				for(unsigned i=0;i<atom->getTermsSize();++i){
					if(atom->getTerm(i)->getType()!=TermType::ANONYMOUS){
						outputVariablesInAtoms[position].push_back(i);
					}
	//				set_term variables;
	//				atom->getTerm(i)->getVariable(variables);
	//				bool isBinder=false;
	//				for(auto v:variables){
	//					if(variablesBinder[v->getLocalVariableIndex()]==position)
	//						isBinder=true;
	//				}
	//				if(isBinder && !Utils::isDisjoint(variables,outputVariables)){
	//					outputVariablesInAtoms[position].push_back(i);
	//				}
				}
				if(outputVariablesInAtoms[position].size()==atom->getPredicate()->getArity())
					outputVariablesInAtoms[position].clear();
			}
		}
//		position=0;
//		for (auto it=currentRule->getBeginBody();it!=currentRule->getEndBody(); ++it,++position) {
//			if(outputVariablesInAtoms[position].size()>0){
//				cerr<<"---> Binder: ";(*it)->print(cerr);cerr<<" ";
//				for(auto i:outputVariablesInAtoms[position]){
//					cerr<<i<<" ";
//				}
//				cerr<<endl;
//			}
//		}
	}


//	trace_action_tag(backjumping,1,
//		currentRule->print(cerr);
//		cerr<<"OUTPUT VARIABLES: ";
//		for(auto var: *outputVariables){
//			var->print(cerr);cerr<<" ";
//		}
//		cerr<<endl;);

}

void BackJumpingGrounder::closestBinder( int literal_pos, const set_term& variables,int& positionCB, bool includeCurrentLiteral) {
	positionCB=-1;
	for(auto variable:variables){
		int binder=variablesBinder[variable->getLocalVariableIndex()];
		if(((!includeCurrentLiteral && binder<literal_pos) || (includeCurrentLiteral && binder<=literal_pos)) && binder>positionCB)
			positionCB=binder;
	}
}

void BackJumpingGrounder::closestBinder( int literal_pos,int& positionCB) {
	positionCB=-1;
	for(unsigned i=0;i<failureMap.size();++i){
		if(!failureMap[i]) continue;
		int binder=variablesBinder[i];
		if(binder<literal_pos && binder>positionCB)
			positionCB=binder;
	}
}

void BackJumpingGrounder::backFromSolutionFound() {
	trace_msg(backjumping,1,"---> BACK SOLUTION FOUND");
	current_status=NEXT_MATCH;

	if(historyBackFromSolutionFound!=-2){
		index_current_atom=historyBackFromSolutionFound;
		closestSuccessfulBinder_index=historyBackOutputVars[index_current_atom];
	}
	else{
		vector<Atom*>::iterator closestBinder_it;
		int closestBinder_pos;
		closestBinder(index_current_atom,*outputVariables,closestBinder_pos,true);
		historyBackFromSolutionFound=closestBinder_pos;
		index_current_atom=closestBinder_pos;
		closestBinder(index_current_atom,*outputVariables,closestSuccessfulBinder_index,false);
		if(index_current_atom>=0)
			historyBackOutputVars[index_current_atom]=closestSuccessfulBinder_index;
	}

	trace_msg(backjumping,1,"CSB "<<closestSuccessfulBinder_index);
	trace_msg(backjumping,1,"CURRENT ATOM "<<index_current_atom);
}

void BackJumpingGrounder::backFromFirstMatch() {
	trace_msg(backjumping,1,"---> BACK FIRST MATCH");
	int it=historyBackFromFirst[index_current_atom];

	if(it!=-2){
		index_current_atom=it;
	}
	else{
		vector<Atom*>::iterator closestBinder_it;
		int closestBinder_pos;
		const set_term& variables=atomsVariables[index_current_atom];

		closestBinder(index_current_atom,variables,closestBinder_pos,false);
		if(index_current_atom>=0)
			historyBackFromFirst[index_current_atom]=closestBinder_pos;
		index_current_atom=closestBinder_pos;
	}

	trace_msg(backjumping,1,"CSB "<<closestSuccessfulBinder_index);
	trace_msg(backjumping,1,"CURRENT ATOM "<<index_current_atom);
}


bool BackJumpingGrounder::match() {
	trace_msg(backjumping,1,"---> MATCH");
	bool result = true;
	Atom* current_atom=currentRule->getAtomInBody(index_current_atom);
	if(current_atom->isBuiltIn() && current_atom->isAssignment() && !direction)
		result = false;

	if( result ){
		if( direction )
			current_status = FIRST_MATCH;
		else
			current_status = NEXT_MATCH;
		result = BackTrackingGrounder::match();
	}

	if(!result){
		const set_term& vars=atomsVariables[index_current_atom];
		for(auto var:vars)
			failureMap[var->getLocalVariableIndex()]=true;

	}
	else{
		for(auto var: atoms_bind_variables[index_current_atom])
			failureMap[var]=false;

	}

	trace_msg(backjumping,1,"MATCH RESULT: "<<result);
	trace_msg(backjumping,1,"CURRENT ATOM: "<<index_current_atom);
	trace_action_tag(backjumping,1,
		cerr<<"FAILURE SET: ";
		for(unsigned i=0;i<failureMap.size();++i){
			if(failureMap[i]){
				cerr<<i<<" ";
			}
		}
		cerr<<endl;
	);
	trace_action_tag(backjumping,1,
		cerr<<"ASSIGNMENT: ";
		printAssignment();
	);

	return result;
}

void BackJumpingGrounder::backFromNextMatch() {
	trace_msg(backjumping,1,"---> BACK NEXT MATCH");

	vector<Atom*>::iterator closestBinder_it;
	int closestBinder_pos;
	closestBinder(index_current_atom,closestBinder_pos);

	if(closestSuccessfulBinder_index>closestBinder_pos){
		index_current_atom=closestSuccessfulBinder_index;
	}
	else {
		index_current_atom=closestBinder_pos;
	}

	if(index_current_atom==closestSuccessfulBinder_index && index_current_atom>=0){
		auto it=historyBackOutputVars[index_current_atom];
		if(it!=-2){
			closestSuccessfulBinder_index=it;
		}
		else{
			closestBinder(index_current_atom,*outputVariables,closestSuccessfulBinder_index,false);
			historyBackOutputVars[index_current_atom]=closestSuccessfulBinder_index;
		}
	}

	trace_action_tag(backjumping,1,
		cerr<<"FAILURE SET: ";
		for(unsigned i=0;i<failureMap.size();++i){
			if(failureMap[i]){
				cerr<<i<<" ";
			}
		}
		cerr<<endl;
	);

	trace_msg(backjumping,1,"CSB "<<closestSuccessfulBinder_index);
	trace_msg(backjumping,1,"CB "<<closestBinder_pos);
	trace_msg(backjumping,1,"CURRENT ATOM: "<<index_current_atom);
}

} /* namespace grounder */
} /* namespace DLV2 */
