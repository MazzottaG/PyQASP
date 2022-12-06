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
 * Rule.cpp
 *
 *  Created on: 03/apr/2014
 *      Author: Jessica
 */

#include "Rule.h"
#include "iostream"
#include "OrderRule.h"
#include "../table/PredicateExtension.h"

namespace DLV2{

namespace grounder{

//-------------------------------RuleInformation---------------------

void RuleInformation::computeOutputVariables(Rule* rule) {
	for (auto it=rule->getBeginHead();it!=rule->getEndHead(); ++it) {
		Atom* atom=*it;
		atom->getVariable(outputVariables);
	}
	unsigned position=0;
	for (auto it=rule->getBeginBody();it!=rule->getEndBody(); ++it,++position) {
		Atom* atom=*it;
		const set_term& variablesInAtom=atom->getVariable();
		if(atom->isClassicalLiteral()){
			if(!atom->getPredicate()->isSolved()){
				outputVariables.insert(variablesInAtom.begin(),variablesInAtom.end());
			}
		}
		else if(atom->isAggregateAtom()){
			set_term variablesUnsolved;
			atom->getUnsolvedPredicateVariable(variablesUnsolved);
			outputVariables.insert(variablesUnsolved.begin(),variablesUnsolved.end());
			if(!variablesUnsolved.empty()){
				set_term guards=atom->getGuardVariable();
				outputVariables.insert(guards.begin(),guards.end());
			}
		}
	}
	if(rule->isWeakConstraint()){rule->getWeightLevelLabeltVariables(outputVariables);}
}


// ------------------------------Rule---------------------------------

set_predicate Rule::calculatePredicate(vector<Atom*>& atoms,bool checkNegative,bool negative){
	set_predicate predicates;
	for (auto atom:atoms)
		if(!checkNegative || (atom->isNegative() == negative)){
			set_predicate atom_predicates=atom->getPredicates();
			predicates.insert(atom_predicates.begin(),atom_predicates.end());
		}

	return predicates;
}

unordered_set<index_object> Rule::calculatePredicateIndex(vector<Atom*>& atoms,bool checkNegative,bool negative){
	unordered_set<index_object> predicates;
	for (auto atom:atoms) {
		set_predicate atom_predicates=atom->getPredicates();
		for(auto predicate: atom_predicates)
			if(!checkNegative || (atom->isNegative() == negative))
				predicates.insert(predicate->getIndex());
	}
	return predicates;
}

void Rule::print(ostream& stream)const{
	//Print for debug
	if(!ground){
		printNonGround(stream);return;
	}
	unsigned int i=0;
	bool firstAtomPrinted=false;
	for (auto atom:head) {
		if(firstAtomPrinted && !simplifiedHead[i])
			stream<<SEPARATOR_HEAD;
		if(atom!=nullptr && !simplifiedHead[i]){
			atom->print(stream);
			if(!firstAtomPrinted)
				firstAtomPrinted=true;
		}
		i++;
	}
	firstAtomPrinted=false;
	if(areThereUndefinedAtomInBody() || isAStrongConstraint()){
		stream<<":-";
		unsigned int i=0;
		for (auto atom:body) {
			if(firstAtomPrinted && !simplifiedBody[i])
				stream<<SEPARATOR_BODY;
			if(atom!=nullptr && !simplifiedBody[i]){
				atom->print(stream);
				if(!firstAtomPrinted)
					firstAtomPrinted=true;
			}
			i++;
		}
	}
	stream<<"."<<endl;
}

void Rule::printNonGround(ostream& stream)const{
	unsigned int i=0;
	for (auto atom:head) {
		atom->print(stream);
		if(i!=head.size()-1)
			stream<<SEPARATOR_HEAD;
		i++;
	}
	if(body.size()>0 || isAStrongConstraint()){
		stream<<":-";
		unsigned int i=0;
		for (auto atom:body) {
			atom->print(stream);
			if(i!=body.size()-1)
				stream<<SEPARATOR_BODY;
			i++;
		}
	}
	stream<<"."<<endl;
}


bool Rule::operator ==(const Rule& r) {
	  if(getSizeBody()!=r.getSizeBody())return false;
	  if(getSizeHead()!=r.getSizeHead())return false;
	  for(auto atom:head){
		  bool find=false;
		  for(auto it=r.getBeginHead();it!=r.getEndHead();it++)
			  if(*atom==*(*it)){
				  find=true;
				  break;
			  }
		  if(!find)return false;
	  }
	  for(auto atom:body){
		  bool find=false;
		  for(auto it=r.getBeginBody();it!=r.getEndBody();it++)
			  if(*atom==*(*it)){
				  find=true;
				  break;
			  }
		  if(!find)return false;
	  }

	  return true;

}

void Rule::deleteBody(function<int(Atom*)> f) {
	for(auto& atom:body){
		if(atom==nullptr)continue;
		int result=f(atom);
		if(result==1)
			delete atom;
		if(result==2){
			atom->deleteAtoms();
			delete atom;
		}
	}
}

void Rule::deleteHead(function<int(Atom*)> f) {
	for(auto& atom:head){
		if(atom==nullptr)continue;
		int result=f(atom);
		if(result==1)
			delete atom;
		if(result==2){
			atom->deleteAtoms();
			delete atom;
		}
	}
}

void Rule::deleteGroundRule(){
	this->deleteBody([](Atom* atom){
		//Delete the given atom if is a false negative atom or is an aggregate (atoms not present in PredicateExtension)
		if(atom!=nullptr && ((atom->isClassicalLiteral() && atom->isNegative()) || atom->isAggregateAtom()))
			return 1;
		return 0;
	});
	this->deleteHead([](Atom* atom){
		if(atom!=0 && atom->isChoice())
			return 1;
		return 0;
	});
	delete this;
}

void Rule::setUnsolvedPredicates() {
	if(isChoiceRule()){
		Atom* choiceAtom=head[0];
		for(unsigned i=0;i<choiceAtom->getChoiceElementsSize();++i)
			choiceAtom->getChoiceElement(i)->getFirstAtom()->getPredicate()->setSolved(false);
	}
	if(head.size()>1){
		for(auto atom:head)
			atom->getPredicate()->setSolved(false);
	}
}

void Rule::computeJoinVariables() {
	set_term variableLocalIndex;
	for(auto atom:body)
		for(auto term:atom->getVariable())
			if(!variableLocalIndex.count(term))
				variableLocalIndex.insert(term);
			else if(atom->isClassicalLiteral() && !atom->isNegative())
					ruleInformation.insertJoinVariable(term);
}

void Rule::computeVariablesLocalIndices() {
	set_term variableLocalIndex;
	for(auto atom:body){
		for(auto term:atom->getVariable()){
			if(!variableLocalIndex.count(term)){
				term->setLocalVariableIndex(variableLocalIndex.size()+1);
				variableLocalIndex.insert(term);
				trace_action_tag(grounding,1,
					cerr<<"VARIABLE-INDEX : ";term->print(cerr);cerr<<" = "<<term->getLocalVariableIndex()<<endl;
				);
			}
//			else{
//				if(atom->isClassicalLiteral() && !atom->isNegative()){
//					ruleInformation.insertJoinVariable(term);
//					trace_action_tag(grounding,1,
//							cerr<<"JOIN VARIABLE : ";term->print(cerr);cerr<<endl;
//					);
//				}
//			}
		}
	}
	if(head.size()==1 && head[0]->isChoice()){
		for(unsigned i=0;i<head[0]->getChoiceElementsSize();++i){
			ChoiceElement* choiceEl=head[0]->getChoiceElement(i);
			if(choiceEl->getSize()>1){
				for(unsigned j=1;j<choiceEl->getSize();++j){
					Atom* atom=choiceEl->getAtom(j);
					for(auto term:atom->getVariable()){
						if(!variableLocalIndex.count(term)){
							term->setLocalVariableIndex(variableLocalIndex.size()+1);
							variableLocalIndex.insert(term);
							trace_action_tag(grounding,1,
								cerr<<"VARIABLE-INDEX: ";term->print(cerr);cerr<<" = "<<term->getLocalVariableIndex()<<endl;
							);
						}
					}
				}
			}
		}
	}

	this->variablesSize=variableLocalIndex.size()+1;
}

Rule* Rule::clone() {
	Rule *newRule=new Rule;
	for(auto atom:head)
		newRule->addInHead(atom->clone());

	for(auto atom:body)
		newRule->addInBody(atom->clone());

	return newRule;
}

// ******************************* WeakConstraint *****************************************

void WeakConstraint::print(ostream& stream)const{
	//Print for debug
	if(!ground){printNonGround(stream);return;}
	bool firstAtomPrinted=false;
	stream<<":~";
	unsigned int i=0;
	for (auto atom:body) {
		if(firstAtomPrinted && !simplifiedBody[i])
			stream<<";";
		if(!simplifiedBody[i]){
			atom->print(stream);
			if(!firstAtomPrinted)
				firstAtomPrinted=true;
		}
		i++;
	}
	stream<<". [";
	if(weight!=nullptr)
		weight->print(stream);
	stream<<"@";
	if(level!=nullptr)
		level->print(stream);
	for(unsigned i=0;i<label.size();i++){
		stream<<",";
		label[i]->print(stream);
	}
	stream<<"]"<<endl;
}

void WeakConstraint::printNonGround(ostream& stream) const{
	stream<<":~";
	for(unsigned i=0;i<body.size();i++){
		if(i!=0)stream<<",";
		body[i]->print(stream);
	}
	stream<<". [";
	if(weight!=nullptr)
		weight->print(stream);
	stream<<"@";
	if(level!=nullptr)
		level->print(stream);
	for(unsigned i=0;i<label.size();i++){
		stream<<",";
		label[i]->print(stream);
	}
	stream<<"]"<<endl;
}



tupleWeak WeakConstraint::groundWeightLevel(var_assignment& current_assignment){
	tupleWeak groundSquare;
	get<0>(groundSquare)=weight->substitute(current_assignment)->calculate();
	get<1>(groundSquare)=level->substitute(current_assignment)->calculate();
	for(auto t:label)
		get<2>(groundSquare).push_back(t->substitute(current_assignment)->calculate());

	return groundSquare;
}






}
}


