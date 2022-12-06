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
 * InputRewriter.cpp
 *
 *  Created on: 18/feb/2015
 *      Author: tesi2
 */

#include "InputRewriter.h"
#include "../table/IdGenerator.h"
#include "../atom/AggregateAtom.h"
#include "../../util/Utils.h"
#include "../atom/Choice.h"
#include "../term/FunctionTerm.h"
#include "../term/ArithTerm.h"
#include "../../util/Constants.h"
#include "../statement/GroundingPreferences.h"
#include "../ground/NonGroundSimplifier.h"

namespace DLV2 {
namespace grounder {

Atom* InputRewriter::generateNewAuxiliaryAtom(string& predicate_name, vector<Term*>& terms,bool checkPredExistenceInPredTable) {
	Predicate* predicate=new Predicate(predicate_name,terms.size());
	predicate->setHiddenForPrinting(true);
	uintptr_t address=reinterpret_cast<uintptr_t>(&(*predicate));
	predicateTable->getInstance()->insertPredicate(predicate);
	if(checkPredExistenceInPredTable){
		while(reinterpret_cast<uintptr_t>(&(*predicate))!=address){
			predicate_name+=AUXILIARY;
			predicate=new Predicate(predicate_name,terms.size());
			predicate->setHiddenForPrinting(true);
			address=reinterpret_cast<std::uintptr_t>(&(*predicate));
			predicateTable->getInstance()->insertPredicate(predicate);
		}
	}
	predicateExtTable->addPredicateExt(predicate);
	Atom* auxiliaryAtom=new ClassicalLiteral(predicate,terms,false,false);

	return auxiliaryAtom;
}

void BaseInputRewriter::projectAtoms(Rule*& rule, vector<Rule*>& ruleRewrited,const string& pname){
	if(rule->getSizeBody()<=1)return;
	auto pp = GroundingPreferences::getGroundingPreferences()->getProjectionType(rule);
	int rewOpt=(!pp.first)?Options::globalOptions()->getRewProject():pp.second;
	if(rewOpt==0) return;
	bool projectFunction=rewOpt>0 && rewOpt!=NO_PROJECTION_FUNCTION;
	bool projectVariable=rewOpt>0 && rewOpt!=NO_PROJECTION_VARIABLE;
	//First find all the variable in the rule for each atom in the body and
	// all the variable that are in the head of the rule
	vector<set_term>atomsVariables;
	atomsVariables.resize(rule->getSizeBody());
	set_term varInHead;
	set_term variablesWeak;
	for(unsigned i=0;i<rule->getSizeBody();i++){
		Atom* atom=rule->getAtomInBody(i);
		if(atom->isAggregateAtom()){
			atomsVariables[i]=atom->getSharedVariable(rule->getBeginBody(),rule->getEndBody());
			set_term guards=atom->getGuardVariable();
			atomsVariables[i].insert(guards.begin(),guards.end());
		}
		else
			atomsVariables[i]=atom->getVariable();
	}
	if(rule->isWeakConstraint()){
		rule->getLevel()->getVariable(variablesWeak);
		rule->getWeight()->getVariable(variablesWeak);
		for(auto termInLabel:rule->getLabel()){
			termInLabel->getVariable(variablesWeak);
		}
	}

	for (auto it=rule->getBeginHead();it!=rule->getEndHead(); ++it) {
		Atom* atom=*it;
		const set_term& variables=atom->getVariable();
		varInHead.insert(variables.begin(),variables.end());
	}

	unsigned int index_atom=0;
	set_term isolatedVars;
	for (auto it=rule->getBeginBody();it!=rule->getEndBody(); ++it,++index_atom) {
		Atom* atom=*it;
		for (auto term: atomsVariables[index_atom]) {
			if (varInHead.count(term))
				continue;
			if(variablesWeak.count(term))
				continue;

			bool found = false;
			for (unsigned i = 0; i < rule->getSizeBody(); ++i) {
				if (index_atom != i && atomsVariables[i].count(term)) {
					found = true;
					break;
				}
				if(index_atom != i && rule->getAtomInBody(i)->isExternalAtom() && rule->getAtomInBody(i)->getVariable().count(term)){
					found = true;
					break;
				}
			}
			if(found) continue;

			unsigned count=0;
			for (unsigned t1 = 0; t1 < atom->getTermsSize()&&!found; ++t1) {
				Term* term1 = atom->getTerm(t1);
				if (term1->containsVariable(term)){
					count++;
				}
				if(count>=2) found = true;
			}
			if(found) continue;

			isolatedVars.insert(term);
			atom->substituteVarWithAnonymous(term);
		}
	}

	//For each atom in the body if is a classical literal and not negative, find the term to filter.
	//A term is filtered if is a variable and not compare in the head of the rule, in some atom in the
	//body of the rule and also check if the variable compare two or more times in the current atom.
	// If the atom not contain term to filter we can skip this atom
	index_atom=0;
	for(auto it=rule->getBeginBody();it!=rule->getEndBody();++it,++index_atom){

		Atom *atom=rule->getAtomInBody(index_atom);
		if(!(atom->isClassicalLiteral() && ! atom->isNegative()))continue;

		if(atom->containsFunctionalTerms() && !atom->containsArithTerms()){
                        if(!projectFunction)continue;
			vector<Term*> terms;
			for(auto t: atomsVariables[index_atom])
				if(!isolatedVars.count(t))
					terms.push_back(t);

			Rule* ruleProjection=new Rule;

			string name=pname+SEPARATOR+to_string(IdGenerator::getInstance()->getNewId());
			Atom* headAtom=generateNewAuxiliaryAtom(name,terms);
			headAtom->getPredicate()->setIdb();

			ruleProjection->addInHead(headAtom);
			ruleProjection->addInBody(atom);
			ruleRewrited.push_back(ruleProjection);

			rule->setAtomInBody(index_atom,headAtom->clone());

			continue;
		}
//		bool recursive=(recursivePredicate!=nullptr)?recursivePredicate->count(atom->getPredicate()->getIndex()):false;
//		if(f(atom->getPredicate(),recursive))continue;
		unordered_set<unsigned> termToFilter;
		for (unsigned t = 0; t < atom->getTermsSize(); ++t) {
			Term* term = atom->getTerm(t);
			if(term->getType()==ANONYMOUS)
				termToFilter.insert(t);
		}
		if(!projectVariable || termToFilter.size()==0)continue;

		//We have to project the current variable with the variable that are not present in termToFilter.
		//Then check if the atom is not already projected in previous rule else we create a new predicate and
		// a new auxiliary rule for the projection.

		vector<Term*> terms;
		for(unsigned i=0;i<atom->getTermsSize();i++)
			if(!termToFilter.count(i))
				terms.push_back(atom->getTerm(i));
		Atom *projAtom=nullptr;
		if(projectedAtoms.count(atom->getPredicate())){
			for(auto& aux:projectedAtoms[atom->getPredicate()]){
				if(aux.first.size()==termToFilter.size() && Utils::isContained(aux.first,termToFilter)){
					projAtom=new ClassicalLiteral(aux.second,terms,false,atom->isNegative());
				}
			}
		}
		if(projAtom==nullptr){
			unsigned auxNumber=IdGenerator::getInstance()->getNewId();
			string newName=pname+to_string(auxNumber);
			Predicate *newPred=new Predicate(newName,terms.size());
			projAtom=new ClassicalLiteral(newPred,terms,false,atom->isNegative());
			newPred->setHiddenForPrinting(true);
			newPred->setIdb();
			PredicateTable::getInstance()->insertPredicate(newPred);
			PredicateExtTable::getInstance()->addPredicateExt(newPred);

			projectedAtoms[atom->getPredicate()].push_back({termToFilter,newPred});
			Rule *newRule=new Rule;
			vector<Term*> termsInHead;
			vector<Term*> termsInBody;
			unordered_map<unsigned,Term*> mapTermVariable;
			for(unsigned i=0;i<atom->getTermsSize();i++){
				if(termToFilter.count(i))continue;
				string name=AUXILIARY_VAR+to_string(i);
				Term* newTerm =TermTable::getInstance()->generateNewVariable(name);
				termsInHead.push_back(newTerm);
				mapTermVariable[i]=newTerm;
			}
			for(unsigned i=0;i<atom->getTermsSize();i++){
				if(!termToFilter.count(i))
					termsInBody.push_back(mapTermVariable[i]);
				else{
					string name=AUXILIARY_VAR+to_string(i);
					Term* newTerm =TermTable::getInstance()->generateNewVariable(name);
					termsInBody.push_back(newTerm);
				}

			}

			newRule->addInHead(new ClassicalLiteral(projAtom->getPredicate(),termsInHead,false,false));
			newRule->addInBody(new ClassicalLiteral(atom->getPredicate(),termsInBody,false,false));
			ruleRewrited.push_back(newRule);
		}
		rule->setAtomInBody(index_atom,projAtom);
		delete atom;
	}
}

void BaseInputRewriter::pushBuiltin(Rule*& rule){

	if(rule->isChoiceRule())return;
	for(auto& atom:rule->getBody())if(atom->isAggregateAtom())return;

	function<void(Term *& t, Term * var, Term * sub)> subFunction;
	subFunction=[&subFunction](Term *& t, Term * var, Term * sub){
		auto type=t->getType();
		if(type!=FUNCTION && type!=ARITH){
			if(t->getIndex()==var->getIndex())
				t=sub;
			return;
		}
		vector<Term*> terms=t->getTerms();
		for(auto& term:terms)
			subFunction(term,var,sub);
		if(!equal(terms.begin(),terms.end(),t->getTerms().begin())){
			if(type==FUNCTION)
			{
				string functor=t->getName();
				Term* newFunction=new FunctionTerm(functor,t->isNegative(),terms);
				TermTable::getInstance()->addTerm(newFunction);
				t=newFunction;
			}
			else if(type==ARITH){
				Term* newArith=new ArithTerm(t->isNegative(),t->getOperators(),terms);
				TermTable::getInstance()->addTerm(newArith);
				t=newArith;
			}
		}
	};

	function<bool(Term*)> termsToFilter=[](Term* t){
		return t->getType()==VARIABLE || t->getType()==FUNCTION || t->getType()==NUMERIC_CONSTANT || t->getType()==STRING_CONSTANT ||  t->getType()==SYMBOLIC_CONSTANT;
	};


	for(unsigned i=0;i<rule->getSizeBody();i++){
		auto atom=rule->getAtomInBody(i);
		if(atom->isBuiltIn() && atom->getBinop()==EQUAL ){
			Term *var=nullptr;
			Term *toSub=nullptr;
			if(atom->getTerm(0)->getType()==VARIABLE && termsToFilter(atom->getTerm(1))){
				var=atom->getTerm(0);
				toSub=atom->getTerm(1);
			}
			else if(atom->getTerm(1)->getType()==VARIABLE && termsToFilter(atom->getTerm(0))){
				var=atom->getTerm(1);
				toSub=atom->getTerm(0);
			}
			if(var==nullptr || toSub==nullptr)continue;

			for(auto atom2:rule->getBody())
				if( atom2->isClassicalLiteral() || atom2->isBuiltIn() || atom2->isExternalAtom() )
					for(auto& t:atom2->getTerms())
						subFunction(t,var,toSub);

			for(auto atom2:rule->getHead())
				if(atom2->isClassicalLiteral())
					for(auto& t:atom2->getTerms())
						subFunction(t,var,toSub);

			if(rule->isWeakConstraint()){
				Term* w=rule->getWeight();
				subFunction(w,var,toSub);
				rule->setWeight(w);
				for(auto& t:rule->getLabel())
					subFunction(t,var,toSub);
			}

			rule->removeInBody(i);
			delete atom;
		}
	}

}

void BaseInputRewriter::pushBuiltinArithOnlyInBody(Rule*& rule){

	function<void(Term *& t, Term * var, Term * sub)> subFunction;
	subFunction=[&subFunction](Term *& t, Term * var, Term * sub){
//		cerr<<"***** T ";t->print(cerr);cerr<<endl;
//		cerr<<"***** VAR ";var->print(cerr);cerr<<endl;
//		cerr<<"***** SUB ";sub->print(cerr);cerr<<endl;
		auto type=t->getType();
		if(type!=FUNCTION && type!=ARITH){
			if(t->getIndex()==var->getIndex())
				t=sub;
			return;
		}
		vector<Term*> terms=t->getTerms();
		for(auto& term:terms)
			subFunction(term,var,sub);
		if(!equal(terms.begin(),terms.end(),t->getTerms().begin())){
			if(type==FUNCTION){
				string functor=t->getName();
				Term* newFunction=new FunctionTerm(functor,t->isNegative(),terms);
				TermTable::getInstance()->addTerm(newFunction);
				t=newFunction;
			}
			if(type==ARITH){
				vector<OperandOperator> op=t->getOperators();
				Term* newArith=new ArithTerm(t->isNegative(),op,terms);
				TermTable::getInstance()->addTerm(newArith);
				t=newArith;
			}
		}
	};

	function<bool(Term*)> termsToFilter=[](Term* t){
		return t->getType()==VARIABLE || t->getType()==FUNCTION || t->getType()==ARITH || t->getType()==NUMERIC_CONSTANT || t->getType()==STRING_CONSTANT ||  t->getType()==SYMBOLIC_CONSTANT;
	};

	set_term varInHead;
	rule->getVariablesInHead(varInHead);
	for(unsigned i=0;i<rule->getSizeBody();i++){
		auto atom=rule->getAtomInBody(i);
		if(atom->isBuiltIn() && atom->getBinop()==EQUAL ){
			Term *var=nullptr;
			Term *toSub=nullptr;
			if( atom->getTerm(0)->getType()==VARIABLE && !varInHead.count(atom->getTerm(0)) && termsToFilter(atom->getTerm(1))){
				var=atom->getTerm(0);
				toSub=atom->getTerm(1);
			}
			else if( atom->getTerm(1)->getType()==VARIABLE && !varInHead.count(atom->getTerm(1)) && termsToFilter(atom->getTerm(0))){
				var=atom->getTerm(1);
				toSub=atom->getTerm(0);
			}

			if(var==nullptr || toSub==nullptr)continue;
			for(auto atom2:rule->getBody())
				if(atom2->isClassicalLiteral() || atom2->isBuiltIn())
					for(auto& t:atom2->getTerms())
						subFunction(t,var,toSub);

			rule->removeInBody(i);
			delete atom;
		}
	}

}


void BaseInputRewriter::translateAggregate(Rule* r, vector<Rule*>& ruleRewrited, OrderRule* orderRule) {

	/// First, auxiliary rules for aggregates elements are generated
	vector<unsigned int> aggregateAtoms;
	unsigned int index_atom=0;
	for(auto it=r->getBeginBody();it!=r->getEndBody();++it,++index_atom){
		unsigned aggElementsSize=(*it)->getAggregateElementsSize();
		if(aggElementsSize>0){
			Atom* aggregate=(*it);
			set_term variablesRule=aggregate->getSharedVariable(r->getBeginBody(),r->getEndBody());
			aggregateAtoms.push_back(index_atom);
			unsigned id=IdGenerator::getInstance()->getNewId();
			unsigned counter=1;
			for(unsigned i=0;i<aggElementsSize;++i){
				AggregateElement* aggElem=(*it)->getAggregateElement(i);

				vector<Term*> terms;

				//For each variable in the aggregate element and in the rule add in head of auxiliary rule
				set_term variablesAggElem=getVariablesInAggregateElem((*it)->getAggregateElement(i));
				for(auto term:variablesAggElem)
					if(variablesRule.count(term))
						terms.push_back(term);

				unsigned countGroundTermInAggElem=0;
				set_term aggregate_term;
				for(auto term:aggElem->getTerms()){
					if(term->isGround())countGroundTermInAggElem++;
					//term->getGroundTerm(aggregate_term);
					term->getVariable(aggregate_term);
				}


				terms.insert(terms.end(),aggregate_term.begin(),aggregate_term.end());

				//terms.insert(terms.end(),aggElem->getTerms().begin(),aggElem->getTerms().end());

				// Avoid the rewriting if there is just one aggregate element and the rewritten atom will have have the same number of terms of the only atom
				// contained in the aggregate element
				if (aggElem->getNafLiteralsSize() < 2
						&& aggElem->getNafLiteral(0)->getVariable(false).size() == terms.size()
						&& countGroundTermInAggElem == 0
						&& !aggElem->getNafLiteral(0)->isExternalAtom() && !aggElem->getNafLiteral(0)->containsAnonymous())
				continue;

				Rule* rule=new Rule;
				vector<Atom*> atoms=aggElem->getNafLiterals();

				if(orderRule!=nullptr){
					set_term unsafeVars=aggElem->getUnsafeVariable();
					chooseBestSaviorForAggregate(r,aggElem,unsafeVars,atoms,*orderRule);
				}

				rule->addInBody(atoms.begin(),atoms.end());

				string predicate_name=AUXILIARY+SEPARATOR+to_string(id)+SEPARATOR+to_string(counter);
				Atom *auxiliaryAtom=generateNewAuxiliaryAtom(predicate_name,terms);

				rule->addInHead(auxiliaryAtom);
				counter++;
				ruleRewrited.push_back(rule);
				Atom* atomClone=auxiliaryAtom->clone();
				vector<Atom*> newAggElem;
				newAggElem.push_back(atomClone);
				aggElem->setNafLiterals(newAggElem);
			}
		}
	}

}

void BaseInputRewriter::chooseBestSaviorForAggregate(Rule* rule, AggregateElement* aggregateElement, set_term& unsafeVars, vector<Atom*>& atomToAdd, const OrderRule& orderRule) {
	list<Atom*> possibleAtomsBinding;
	for(auto var:unsafeVars){
		unsigned int index_atom=0;
		for(auto it=rule->getBeginBody();it!=rule->getEndBody();++it,++index_atom){
			Atom* atom=*it;
			set_term variables; //TODO togliere dal for o fare cache
			if (atom->isAggregateAtom()){
				if(atom->getFirstBinop()==Binop::EQUAL){
					variables=atom->getGuardVariable();
				}
			}
			else if (atom->isBuiltIn()){
				if (atom->getBinop()==Binop::EQUAL)
					variables=atom->getVariable();
			}
			else if (atom->isClassicalLiteral() && !atom->isNegative())
				variables=atom->getVariable();
			if(variables.count(var) && saviourChoosingPolicy->choose(atom,index_atom,possibleAtomsBinding,atomToAdd,(it==rule->getEndBody()-1), orderRule))
				break;
		}
	}
}

set_term BaseInputRewriter::getVariablesInAggregateElem(AggregateElement* aggregateElem){
	set_term variables;
	aggregateElem->getVariableInNaf(variables);
	for(auto term:aggregateElem->getTerms()){
		set_term variables_in_term;
		term->getVariable(variables_in_term);
		for(auto term_to_delete:variables_in_term)variables.erase(term_to_delete);
	}
	return variables;
}

void BaseInputRewriter::translateChoice(Rule*& rule,vector<Rule*>& ruleRewrited, OrderRule* orderRule) {
	unsigned id=IdGenerator::getInstance()->getNewId();
	unsigned counter=1;

	// Create an auxiliary rule in order to ground the body only once.
	// First the variables shared between the atoms in the body
	// and the choice atom are found, these variables will be
	// the term of the auxiliary atom in the head of the auxiliary rule

	Atom* choice =rule->getAtomInHead(0);
	Atom *auxiliaryAtomBody=nullptr;
	bool isRewriteBody=rule->getSizeBody()>1 || ( rule->getSizeBody()==1 && rule->getAtomInBody(0)->isAggregateAtom());
	if(isRewriteBody){

		const set_term& variables_in_choice=choice->getVariable();
		set_term variables_in_body;

		for(unsigned i=0;i<rule->getSizeBody();++i){
			auto atom=rule->getAtomInBody(i);
			if(atom->isNegative())continue;
			if(atom->isAggregateAtom() && atom->getFirstBinop()!=EQUAL) continue;
			set_term variables;
			if(atom->isAggregateAtom() && atom->getFirstBinop()==EQUAL)
				variables=atom->getGuardVariable();
			else if(atom->isClassicalLiteral() || (atom->isBuiltIn() && atom->getBinop()==EQUAL))
				variables=atom->getVariable();
			variables_in_body.insert(variables.begin(),variables.end());
		}

		set_term variables_intersection;
		Utils::intersectionSet(variables_in_choice,variables_in_body,variables_intersection);

		Rule * body_rule=new Rule;

		if(rule->isMustBeRewritedForAggregates()) body_rule->setMustBeRewritedForAggregates(true);

		//Body
		body_rule->setBody(rule->getBody());

		//Head
		string predicate_name=AUXILIARY+SEPARATOR+to_string(id)+SEPARATOR+to_string(counter);
		vector<Term*> terms(variables_intersection.begin(),variables_intersection.end());
		auxiliaryAtomBody=generateNewAuxiliaryAtom(predicate_name,terms);
		body_rule->addInHead(auxiliaryAtomBody);

		ruleRewrited.push_back(body_rule);
		counter++;

	}else{
		if(rule->getSizeBody()==1)
			auxiliaryAtomBody=rule->getAtomInBody(0);
	}
	// For each choice element a new disjunctive auxiliary rule is created.
	// Each rule has in the head a disjunction with a the first atom of the choice element and a new auxiliary atom
	// having the same terms of the first atom, while in the body it contains the remaining atoms of the choice element
	// and the auxiliary atom previously created for the body.
	// Also, the aggregate elements for the constraint rule are created (see below)
	vector<AggregateElement*> elements = rewriteChoiceElements(rule,id, counter,choice, auxiliaryAtomBody, ruleRewrited,orderRule);

	// Finally a constraint rule is created.
	// It has as body the auxiliary atom previously created for the body, and a negated count aggregate
	// whose guard are the same of the choice atom and inside contains the first atom of each choice element
	// and as aggregate terms all its terms
	rewriteChoiceConstraint(elements, auxiliaryAtomBody, choice, ruleRewrited);

	if(!isRewriteBody)
		delete auxiliaryAtomBody;

	rule->deleteBody([](Atom* atom){
		return 0;
	});
	rule->deleteHead([](Atom* atom){
		return 1;
	});
	delete rule;
	rule=0;
}

vector<AggregateElement*> BaseInputRewriter::rewriteChoiceElements(Rule* rule,unsigned& id,unsigned& counter, Atom* choice, Atom* auxiliaryAtomBody,vector<Rule*>& ruleRewrited, OrderRule* orderRule) {
	// For each choice element a new disjunctive auxiliary rule is created.
	// Each rule has in the head a disjunction with a the first atom of the choice element and a new auxiliary atom
	// having the same terms of the first atom, while in the body it contains the remaining atoms of the choice element
	// and the auxiliary atom previously created for the body.
	// Also, the aggregate elements for the constraint rule are created (see below)
	vector<AggregateElement*> elements;
	for (unsigned i = 0; i < choice->getChoiceElementsSize(); ++i) {
		ChoiceElement* choiceElement = choice->getChoiceElement(i);
		Rule* aux_rule = new Rule;
		//Head
		Atom* first_atom = choiceElement->getFirstAtom();
		aux_rule->addInHead(first_atom);
		string predicate_name = AUXILIARY + SEPARATOR + to_string(id)
				+ SEPARATOR + to_string(counter);
		vector<Term*> terms = first_atom->getTerms();
		Atom* auxiliaryAtom = generateNewAuxiliaryAtom(predicate_name, terms);
		aux_rule->addInHead(auxiliaryAtom);
		//Body
		vector<Atom*> naf_elements;
		set_term terms_in_bodychoice;
		if(auxiliaryAtomBody!=nullptr){
			set_term terms_in_body(auxiliaryAtomBody->getTerms().begin(),auxiliaryAtomBody->getTerms().end());
			const set_term& variable_in_choice=choice->getVariable(false);
			Utils::intersectionSet(terms_in_body,variable_in_choice,terms_in_bodychoice);
		}
		AggregateElement* element;
		//Call with default guard false because this rewriting is independent with the type of guard
		createBodyRuleChoice(rule,id,counter,choiceElement,auxiliaryAtomBody,terms_in_bodychoice,ruleRewrited,naf_elements,element,false,orderRule);
		aux_rule->addInBody(naf_elements.begin(),naf_elements.end());

		ruleRewrited.push_back(aux_rule);
		counter++;
		// Create a new aggregate element
		elements.push_back(element);
	}
	return elements;
}

void BaseInputRewriter::rewriteChoiceConstraint(const vector<AggregateElement*>& elements, Atom* auxiliaryAtomBody,	Atom* choice, vector<Rule*>& ruleRewrited) {
	Rule* constraint_aux_rule = new Rule;
	//Body
	if (auxiliaryAtomBody != nullptr)
		constraint_aux_rule->addInBody(auxiliaryAtomBody->clone());

	AggregateAtom* aggregate_atom = new AggregateAtom;
	aggregate_atom->copyGuard(choice);
	aggregate_atom->setNegative(true);
	aggregate_atom->setAggregateFunction(AggregateFunction::COUNT);
	for(auto ae:elements){
		if(ae->getTerms().size()==0)
			ae->addTerm(TermTable::getInstance()->getTermAnonymous());
	}
	aggregate_atom->setAggregateElements(elements);
	aggregate_atom->changeInStandardFormat();
	constraint_aux_rule->addInBody(aggregate_atom);
	constraint_aux_rule->setMustBeRewritedForAggregates(true);
	ruleRewrited.push_back(constraint_aux_rule);
//	translateAggregate(ruleRewrited.back(),ruleRewrited);
}


void BaseInputRewriter::createBodyRuleChoice(Rule* rule,unsigned& id, unsigned& counter,ChoiceElement* choiceElement,Atom* auxiliaryAtomBody,set_term &terms_in_bodychoice,vector<Rule*>& ruleRewrited,vector<Atom*>& naf_elements,AggregateElement*& element,bool defaultGuard, OrderRule* orderRule){
	//Naf element to add in the choice rule
	Atom* first_atom = choiceElement->getFirstAtom();

	//Calculate if is needed an additional rewriting: if a variable in the body is binded only with an atom in naf literal of the choice element.
	// In this case in the constraint rule we need a binding for the body variable and the first atom in the choice.
	//Then an auxiliary rule is created with in the body the naf literal in the choice element that contain the variable missed in the first atom.
	//ps: if the choice have the default guard we don't have the constraint then we don't have to do the additional rewriting

	vector<Atom*> naf_in_aux;
	const set_term& terms_in_first=first_atom->getVariable();
	set_term terms_missed,terms_in_naf;
	for(unsigned i=1;i<choiceElement->getSize();i++){
		set_term variables=choiceElement->getAtom(i)->getVariable();
		set_term intersection_naf_body;
		Utils::intersectionSet(variables,terms_in_bodychoice,intersection_naf_body);
		if(!defaultGuard && !Utils::isContained(intersection_naf_body,terms_in_first)){
			naf_in_aux.push_back(choiceElement->getAtom(i));
			Utils::differenceSet(intersection_naf_body,terms_in_first,terms_missed);
		}else{
			naf_elements.push_back(choiceElement->getAtom(i));
		}
		terms_in_naf.insert(variables.begin(),variables.end());
	}

	//Push the body of the rule if the intersection of term in the first atom and the naf is > 0, then is unsafe or rewritingtype=2
	//Is there is default guard then the body is must be take
	set_term difference_first_naf;
	Utils::intersectionSet(terms_in_first,terms_in_naf,difference_first_naf);
	if (auxiliaryAtomBody != nullptr){
		if(defaultGuard || difference_first_naf.size()!=terms_in_first.size() || Options::globalOptions()->getRewritingType()!=2)
			naf_elements.push_back(auxiliaryAtomBody->clone());
	}

	Atom *aux_naf=nullptr;
	terms_missed.insert(difference_first_naf.begin(),difference_first_naf.end());

	if(naf_in_aux.size()>1){
		Rule *aux_naf_rule=new Rule;
		string predicate_name=AUXILIARY+SEPARATOR+to_string(id)+SEPARATOR+to_string(counter);
		counter++;
		vector<Term*> terms(terms_missed.begin(),terms_missed.end());
		aux_naf=generateNewAuxiliaryAtom(predicate_name,terms);
		aux_naf_rule->addInHead(aux_naf);
		aux_naf_rule->setBody(naf_in_aux);
		ruleRewrited.push_back(aux_naf_rule);

	}else if(naf_in_aux.size()==1)
		aux_naf=naf_in_aux[0];

	if(aux_naf!=nullptr)
		naf_elements.push_back(aux_naf->clone());

	//For aggregate element in constraint
	if(naf_in_aux.size()==0)
		element = new AggregateElement(first_atom->clone(),	first_atom->getTerms());
	else{
		element = new AggregateElement;
		for(auto term:terms_missed)
			element->addTerm(term);
		element->addNafLiterals(aux_naf->clone());
		element->addNafLiterals(first_atom->clone());

		if(orderRule!=nullptr){
			set_term unsafeVars=element->getUnsafeVariable();
			vector<Atom*> atoms;
			chooseBestSaviorForAggregate(rule,element,unsafeVars,atoms,*orderRule);
			for(auto a:atoms)
				element->addNafLiterals(a->clone());
		}

	}
}


vector<AggregateElement*> ChoiceBaseInputRewriter::rewriteChoiceElements(Rule* rule,unsigned& id, unsigned& counter, Atom* choice, Atom* auxiliaryAtomBody,vector<Rule*>& ruleRewrited, OrderRule* orderRule) {
	vector<AggregateElement*> elements;
	vector<Atom*> atoms_single_choice;

	//All variable in the body of the rule that appear in the choice except the variable in the guard
	set_term terms_in_bodychoice;
	if(auxiliaryAtomBody!=nullptr){
		set_term terms_in_body(auxiliaryAtomBody->getTerms().begin(),auxiliaryAtomBody->getTerms().end());
		const set_term& variable_in_choice=choice->getVariable(false);
		Utils::intersectionSet(terms_in_body,variable_in_choice,terms_in_bodychoice);
	}

	for (unsigned i = 0; i < choice->getChoiceElementsSize(); ++i) {
		ChoiceElement* choiceElement = choice->getChoiceElement(i);
		Atom* first_atom = choiceElement->getFirstAtom();
		AggregateElement* element=nullptr;


		//Put the choice element with one atom in an unique choice rule ( this coice element don't have body/naf literal)
		if(choiceElement->getSize()==1){
			atoms_single_choice.push_back(first_atom);
			element = new AggregateElement(first_atom->clone(),	first_atom->getTerms());
		}else{
			//Naf element to add in the choice rule
			vector<Atom*> naf_elements;

			createBodyRuleChoice(rule,id,counter,choiceElement,auxiliaryAtomBody,terms_in_bodychoice,ruleRewrited,naf_elements,element,choice->isDefaultGuard(),orderRule);

			Rule *aux_rule=createAuxChoiceRule(first_atom,naf_elements);

			ruleRewrited.push_back(aux_rule);
			counter++;

		}
		if(element!=nullptr){
			// Create a new aggregate element
			elements.push_back(element);
		}
	}

	if(atoms_single_choice.size()>0){

		Rule* aux_rule;
		if(auxiliaryAtomBody!=nullptr)
			aux_rule= createAuxChoiceRule(atoms_single_choice,auxiliaryAtomBody->clone());
		else
			aux_rule= createAuxChoiceRule(atoms_single_choice);

		ruleRewrited.push_back(aux_rule);
		counter++;
	}

	return elements;

}




Rule* ChoiceBaseInputRewriter::createAuxChoiceRule(const vector<Atom*>& head,const vector<Atom*>& body){
	Rule* aux_rule = new Rule;
	Atom *choice_atom=new Choice;
	choice_atom->setSecondBinop(GREATER_OR_EQ);
	choice_atom->setSecondGuard(TermTable::getInstance()->term_zero);
	for(auto atom:head)
		choice_atom->addSingleChoiceElement(atom);

	aux_rule->addInHead(choice_atom);

	//Body
	for(auto atom:body)
		aux_rule->addInBody(atom);

	return aux_rule;
}

void ChoiceBaseInputRewriter::rewriteChoiceConstraint(const vector<AggregateElement*>& elements, Atom* auxiliaryAtomBody, Atom* choice, vector<Rule*>& ruleRewrited) {
	if(!choice->isDefaultGuard()){
		BaseInputRewriter::rewriteChoiceConstraint(elements, auxiliaryAtomBody, choice, ruleRewrited);
	}
	else{
		for(auto element:elements){
			element->deleteAtoms();
			delete element;
		}
	}
}


vector<AggregateElement*> AdvancedChoiceBaseInputRewriter::rewriteChoiceElements(Rule* rule,unsigned & id, unsigned & counter, Atom* choice,
		Atom* auxiliaryAtomBody, vector<Rule*>& ruleRewrited, OrderRule* orderRule) {

        //Rewrite the choice element if contains more than 2 atoms in the naf literals or if have exactly 2 atom and the second atom is not positive classical literal.
        //All the choice element rewrited contains 1 or 2 atoms 
        
	vector<AggregateElement*> elements;
        set_term termsInBody;
        if(auxiliaryAtomBody!=nullptr)
                termsInBody.insert(auxiliaryAtomBody->getTerms().begin(),auxiliaryAtomBody->getTerms().end());
        vector<ChoiceElement*> newChoiceElements;

	for (unsigned i = 0; i < choice->getChoiceElementsSize(); ++i) {
		ChoiceElement* choiceElement = choice->getChoiceElement(i);
		Atom* firstAtom = choiceElement->getFirstAtom();

                AggregateElement* element=new AggregateElement;
                element->addNafLiterals(firstAtom->clone());
                vector<Atom*> nafMissedInAE;
                set_term termsInAE=firstAtom->getVariable(true);

                vector<Atom*> newChoiceElem={firstAtom};

                if(choiceElement->getSize()>1){

                        Atom* secondAtom=nullptr;
                        
                        if(choiceElement->getSize()==2 && choiceElement->getAtom(1)->isClassicalLiteral() && !choiceElement->getAtom(1)->isNegative()){
                                secondAtom=choiceElement->getAtom(1);
                                newChoiceElem.push_back(secondAtom);
                        }else {
                                set_term variableInNaf;
                                choiceElement->getVariableInNaf(variableInNaf);
                                auto varInFirst=firstAtom->getVariable(false);
                                set_term intersectionTerms;
                                Utils::intersectionSet(variableInNaf,varInFirst,intersectionTerms);
                                bool isIntersectionBodyAndNaf=Utils::intersectionSet(variableInNaf,termsInBody,intersectionTerms);
                                string predicateName=AUXILIARY+SEPARATOR+to_string(id)+SEPARATOR+to_string(++counter);
                                vector<Term*> terms(intersectionTerms.begin(),intersectionTerms.end());
                                Atom *atomHead=generateNewAuxiliaryAtom(predicateName,terms);

                                Rule* auxRule=new Rule;
                                auxRule->addInHead(atomHead);
                                vector<Atom*> naf;
                                choiceElement->getNafAtoms(naf);
                                auxRule->addInBody(naf.begin(), naf.end());
                                if(isIntersectionBodyAndNaf)auxRule->addInBody(auxiliaryAtomBody->clone());
                                ruleRewrited.push_back(auxRule);
                                newChoiceElem.push_back(atomHead->clone());

                                secondAtom=atomHead;
                        }

                        auto termsInSecond=secondAtom->getVariable(true);
                        set_term intersectionSecondBody;
                        Utils::intersectionSet(termsInSecond, termsInBody, intersectionSecondBody);
                        if(!Utils::isContained(intersectionSecondBody,termsInAE)){
                                nafMissedInAE.push_back(secondAtom);
                                Utils::differenceSet(termsInSecond, termsInAE, termsInAE);
                        }


                }
                ChoiceElement* ce=new ChoiceElement;
                ce->setChoiceElement(newChoiceElem);
                newChoiceElements.push_back(ce);

                for(auto atom:nafMissedInAE)
                        element->addNafLiterals(atom->clone());
                vector<Term*> vTermsInAE={termsInAE.begin(),termsInAE.end()};
                element->setTerms(vTermsInAE);
                elements.push_back(element);
        }
        
        Rule *newRule=new Rule;
        Atom *newChoice=new Choice;
        newChoice->setChoiceElements(newChoiceElements);
        newRule->addInHead(newChoice);
        if(auxiliaryAtomBody!=nullptr)
                newRule->addInBody(auxiliaryAtomBody->clone());
        ruleRewrited.push_back(newRule);

	return elements;
}



void FirstSaviorChoosingPolicy::getRecursiveDependencies(const OrderRule& orderRule, unsigned savior_pos, vector<Atom*>& atomToAdd) {
	vector<pair<unsigned int, Atom*>> atomDependencies = orderRule.getAtomsFromWhichDepends(savior_pos);
	for (unsigned int i=0;i<atomDependencies.size();++i) {
		atomToAdd.push_back(atomDependencies[i].second->clone());
		getRecursiveDependencies(orderRule,atomDependencies[i].first,atomToAdd);
	}
}

bool FirstSaviorChoosingPolicy::choose(Atom* atom,unsigned savior_pos,list<Atom*>& possibleAtomsBinding, vector<Atom*>& atomToAdd, bool end, const OrderRule& orderRule) {
	atomToAdd.push_back(atom->clone());
	getRecursiveDependencies(orderRule, savior_pos, atomToAdd);
	return true;

	possibleAtomsBinding.push_back(atom);
	return false;

}



} /* namespace grounder */
} /* namespace DLV2 */


