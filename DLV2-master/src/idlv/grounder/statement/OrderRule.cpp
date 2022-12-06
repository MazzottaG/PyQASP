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
 * OrderRule.cpp
 *
 *  Created on: May 28, 2015
 *      Author: jessica
 */

#include "OrderRule.h"
#include "../../../common/Options.h"
#include "../atom/Choice.h"
#include "../../util/Utils.h"
#include "../atom/BuiltInAtom.h"
#include "GroundingPreferences.h"

namespace DLV2 {
namespace grounder {

/***************************************************** PartialOrders *******************************************************************/

void PartialOrders::checkIfPresentPartialOrders() {
	for(auto it=rule->getBeginBody();it!=rule->getEndBody();++it){
		for(auto it2=it+1;it2!=rule->getEndBody();++it2){

			containsFunctionalTerms(*it,*it2);
			containsExtAtom(*it, *it2);

			if((*it)->isBuiltIn())
				containsABuiltIAssignmentVariable(*it,*it2);

		}
	}
}

void PartialOrders::containsExtAtom( Atom* atom1, Atom* atom2 ){
	/*
	 * atom1			atom2
	 * not b(Y), 		&ext(;Y) 		swap -> atom2 has priority
	 * not &ext(;Y), 	b(Y) 			swap -> atom2 has priority
	 * #{...} = Z, 		&ext(;Z) 		swap -> atom2 has priority
	 * not &ext(;Z), 	#{...} = Z, 	swap -> atom2 has priority
	 * not &ext(;Z), 	X<>=!=Z  		swap -> atom2 has priority
	 * &atom1(Y;), 		&atom2(;Y) 		swap -> atom2 has priority
	 * * &atom1(;Z◊n),	b(Z)			◊∈{'+','-','/','*'} swap -> atom2 has priority
	 * b(Z),			&atom2(;Z◊n)	◊∈{'+','-','/','*'} -> atom1 has priority
	 * &atom1(;Z◊n),	Z=...			◊∈{'+','-','/','*'} and atom2 is a built-in: swap -> atom2 has priority
	 * Z=...,			&atom2(;Z◊n)	◊∈{'+','-','/','*'} and atom1 is a built-in: -> atom1 has priority
	 */
	if( atom2->isExternalAtom() && !atom2->isNegative() && atom1->isNegative() && atom1->isClassicalLiteral() ){ //***
//		cout << "caso1" << endl;
		for ( unsigned i = 0; i < atom1->getTermsSize(); i++ ){
			Term* term1 = atom1->getTerm( i );
			if( term1->contain( TermType::VARIABLE ) ){
				for ( unsigned j = atom2->getSeparator(); j < atom2->getTermsSize(); j++ ){
					Term* term2 = atom2->getTerm( j );
					if( term1->containsVariable( term2 ) ){
						GroundingPreferences::getGroundingPreferences()->addRulePartialOrder( rule );
						GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom( rule, atom2 );
						GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom( rule, atom1 );
						return;
					}
				}
			}
		}
	} else if( atom2->isClassicalLiteral() && !atom2->isNegative() && atom1->isExternalAtom() && atom1->isNegative() ){
//		cout << "caso2" << endl;
		for ( unsigned i = 0; i < atom2->getTermsSize(); i++ ){
			Term* term2 = atom2->getTerm( i );
			if( term2->contain( TermType::VARIABLE ) ){
				for ( unsigned j = atom1->getSeparator(); j < atom1->getTermsSize(); j++ ){
					Term* term1 = atom1->getTerm( j );
					if( term2->containsVariable( term1 ) ){
						GroundingPreferences::getGroundingPreferences()->addRulePartialOrder( rule );
						GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom( rule, atom2 );
						GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom( rule, atom1 );
						return;
					}
				}
			}
		}
	} else if( atom2->isExternalAtom() && !atom2->isNegative() && atom1->isAggregateAtom() && atom1->isAnAssigment() ){
//		cout << "caso3" << endl;
		set_term guards = atom1->getGuardVariable();
		for ( unsigned i = atom2->getSeparator(); i < atom2->getTermsSize(); i++ ){
			Term* term2 = atom2->getTerm( i );
			if( term2->contain( TermType::VARIABLE ) ){
				for ( auto j = guards.begin(); j != guards.end(); j++ ){
					if( term2->containsVariable( (*j) ) ){
						GroundingPreferences::getGroundingPreferences()->addRulePartialOrder( rule );
						GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom( rule, atom2 );
						GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom( rule, atom1 );
						return;
					}
				}
			}
		}
	} else if( atom2->isAggregateAtom() && atom1->isExternalAtom() && atom1->isNegative() ){
//		cout << "caso4" << endl;
		set_term guards = atom2->getGuardVariable();
		for ( unsigned i = atom1->getSeparator(); i < atom1->getTermsSize(); i++ ){
			Term* term1 = atom1->getTerm( i );
			if( term1->contain( TermType::VARIABLE ) ){
				for ( auto j = guards.begin(); j != guards.end(); j++ ){
					if( term1->containsVariable( (*j) ) ){
						GroundingPreferences::getGroundingPreferences()->addRulePartialOrder( rule );
						GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom( rule, atom2 );
						GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom( rule, atom1 );
						return;
					}
				}
			}
		}
	} else if( atom2->isBuiltIn() && atom1->isExternalAtom() && atom1->isNegative() ){ //***
//		cout << "caso5" << endl;
		for ( unsigned i = atom1->getSeparator(); i < atom1->getTermsSize(); i++ ){
			Term* term1 = atom1->getTerm( i );
			if( term1->contain( TermType::VARIABLE ) ){
				for ( unsigned j = 0; j < atom2->getTermsSize(); j++ ){
					Term* term2 = atom2->getTerm( j );
					if( term1->containsVariable( term2 ) ){
						GroundingPreferences::getGroundingPreferences()->addRulePartialOrder( rule );
						GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom( rule, atom2 );
						GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom( rule, atom1 );
						return;
					}
				}
			}
		}
	} else if( atom1->isExternalAtom() && atom2->isExternalAtom() ){
//		cout << "caso6" << endl;
							for ( unsigned i = 0; i < atom1->getSeparator(); i++ ){
								Term* term1 = atom1->getTerm( i );
								if( term1->contain( TermType::VARIABLE ) ){
									for ( unsigned j = atom2->getSeparator(); j < atom2->getTermsSize(); j++ ){
										Term* term2 = atom2->getTerm( j );
										if( term1->containsVariable( term2 ) ){
											GroundingPreferences::getGroundingPreferences()->addRulePartialOrder( rule );
											GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom( rule, atom2 );
											GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom( rule, atom1 );
											return;
										}
									}
								}
							}
						} else
							if( atom1->isExternalAtom() && atom2->isClassicalLiteral() && !atom2->isNegative() ){
//		cout << "atom 1";	atom1->print();		cout << "atom 2 ";		atom2->print();		cout << " caso7 " << endl;
								for ( unsigned i = atom1->getSeparator(); i < atom1->getTermsSize(); i++ ){
									Term* term1 = atom1->getTerm( i );
									if( term1->getType() == ARITH && term1->contain( VARIABLE ) ){

										for ( unsigned j = 0; j < atom2->getTermsSize(); j++ ){
											Term* term2 = atom2->getTerm( j );
											if( term1->containsVariable( term2 ) ){

												GroundingPreferences::getGroundingPreferences()->addRulePartialOrder( rule );
												GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom( rule, atom2 );
												GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom( rule, atom1 );
												return;

											}

										}

									}
								}

							}

							else
								if( atom1->isClassicalLiteral() && !atom1->isNegative() && atom2->isExternalAtom() ){
//		cout << "atom 1";		atom1->print();		cout << "atom 2 ";		atom2->print();		cout << " caso8 " << endl;
									for ( unsigned i = 0; i < atom1->getTermsSize(); i++ ){
										Term* term1 = atom1->getTerm( i );
										if( term1->contain( VARIABLE ) ){
											for ( unsigned j = atom2->getSeparator(); j < atom2->getTermsSize(); j++ ){
												Term* term2 = atom2->getTerm( j );
												if( term2->getType() == ARITH && term2->containsVariable( term1 ) ){
													GroundingPreferences::getGroundingPreferences()->addRulePartialOrder( rule );
													GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom( rule, atom1 );
													GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom( rule, atom2 );
													return;
												}
											}
										}
									}
								} else
									if( atom1->isExternalAtom() && atom2->isBuiltIn() && !atom2->isNegative() ){
//		cout << "atom 1";		atom1->print();		cout << "atom 2 ";		atom2->print();		cout << " caso9 " << endl;
										for ( unsigned i = atom1->getSeparator(); i < atom1->getTermsSize(); i++ ){
											Term* term1 = atom1->getTerm( i );
											if( term1->contain( VARIABLE ) ){

												for ( unsigned j = 0; j < atom2->getTermsSize(); j++ ){
													Term* term2 = atom2->getTerm( j );
													if( term1->containsVariable( term2 ) ){

														GroundingPreferences::getGroundingPreferences()->addRulePartialOrder( rule );
														GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom( rule, atom2 );
														GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom( rule, atom1 );
														return;

													}

												}

											}
										}

									} else
										if( atom1->isBuiltIn() && !atom1->isNegative() && atom2->isExternalAtom() ){
//		cout << "atom 1";		atom1->print();		cout << "atom 2 ";		atom2->print();		cout << " caso10 " << endl;
											for ( unsigned i = 0; i < atom1->getTermsSize(); i++ ){
												Term* term1 = atom1->getTerm( i );
												if( term1->contain( VARIABLE ) ){
													for ( unsigned j = atom2->getSeparator(); j < atom2->getTermsSize(); j++ ){
														Term* term2 = atom2->getTerm( j );
														if( term2->containsVariable( term1 ) ){
															GroundingPreferences::getGroundingPreferences()->addRulePartialOrder( rule );
															GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom( rule, atom1 );
															GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom( rule, atom2 );
															return;
														}
													}
												}
											}
										}
}

void PartialOrders::containsFunctionalTerms(Atom* atom1, Atom* atom2) {
	for(unsigned i=0;i<atom1->getTermsSize();++i){
		Term* term1=atom1->getTerm(i);
		if(term1->contain(TermType::FUNCTION)){
			for(unsigned j=0;j<atom2->getTermsSize();++j){
				Term* term2=atom2->getTerm(j);
				if(term2->getType()==VARIABLE)
				{
					if(term1->containsVariable(term2)){
						GroundingPreferences::getGroundingPreferences()->addRulePartialOrder(rule);
						GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom(rule,atom2);
						GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom(rule,atom1);
						return;
					}
				}
			}
		}
	}
}


void PartialOrders::containsABuiltIAssignmentVariable(Atom* atom1, Atom* atom2) {
	Term* term1=atom1->getTerm(0);
	Term* term2=atom1->getTerm(1);
	bool hasModulo=false;
	set_term varsInvolvedInModulo;
	if(term1->getType()==ARITH)
		for(unsigned i=0;i<term1->getSizeOperator();++i)
			if(term1->getOperator(i).first==Operator::MODULO)
			{
				term1->getVariable(varsInvolvedInModulo);
				hasModulo=true;
			}
	if(term2->getType()==ARITH)
		for(unsigned i=0;i<term2->getSizeOperator();++i)
			if(term2->getOperator(i).first==Operator::MODULO)
			{
				term2->getVariable(varsInvolvedInModulo);
				hasModulo=true;
			}
	if(!hasModulo) return;

	for(auto var:varsInvolvedInModulo){
		for(unsigned j=0;j<atom2->getTermsSize();++j){
			Term* term=atom2->getTerm(j);
			if(term->getType()==VARIABLE)
			{
				if(term->containsVariable(var)){
					GroundingPreferences::getGroundingPreferences()->addRulePartialOrder(rule);
					GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom(rule,atom2);
					GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom(rule,atom1);
					return;
				}
			}
		}
	}
}

/***************************************************** OrderRule *******************************************************************/

vector<Atom*> OrderRule::rewriteArith(Atom* current_atom,
		unordered_map<Term*, Term*, IndexForTable<Term>, IndexForTable<Term> >& arithRewrited) {
	vector<Atom*> newBuiltins;
	for (unsigned i = 0; i < current_atom->getTermsSize(); i++) {
		auto currentTerm = current_atom->getTerm(i);
		if (currentTerm->getType() == ARITH) {
			Term* newTerm = nullptr;
			//Check if is already rewrite
			if (arithRewrited.count(currentTerm)) {
				newTerm = arithRewrited[currentTerm];
			} else {
				newTerm = TermTable::getInstance()->generateVariableAuxTerm(variablesInRule);
				arithRewrited.insert( { currentTerm, newTerm });
				Atom* rewAtom=new BuiltInAtom(Binop::EQUAL, false, newTerm,current_atom->getTerm(i));
				newBuiltins.push_back(rewAtom);
				rewBuiltins.push_back(rewAtom);
			}
			current_atom->setTerm(i, newTerm);

		}
	}
	return newBuiltins;
}

OrderRule::OrderRule(Rule* r):rule(r){
	bindAtomsDependency.resize(r->getSizeBody()+100);
	computeAtomsVariables();
	unordered_map<Term*,Term*,IndexForTable<Term>,IndexForTable<Term>> arithRewrited;
	for(unsigned atom_counter=0;atom_counter<rule->getSizeBody();++atom_counter){
		Atom* current_atom=rule->getAtomInBody(atom_counter);
		if( current_atom->isClassicalLiteral() || current_atom->isExternalAtom() ){
			if(current_atom->isNegative())
				negativeAtoms.push_back(atom_counter);
			else{
				/// Find for each positive classical literal the variables that must be bound
				/// (for example variables appearing in arith terms)
				bool mustBeBound=false;
				if(Options::globalOptions()->getRewriteArith() || GroundingPreferences::getGroundingPreferences()->getRewArith(r)){
					vector<Atom*> newBuiltins=rewriteArith(current_atom, arithRewrited);
					for(auto newBuiltin:newBuiltins)
						rule->addInBody(newBuiltin);
					if(!newBuiltins.empty())
						mapAtomsVariables[atom_counter]=current_atom->getVariable();
				}else{
					mapPositiveAtomsBoundVariables.insert({atom_counter,set_term()});
					for(auto v:current_atom->getTerms()){
						v->getVariablesInArith(mapPositiveAtomsBoundVariables[atom_counter]);
						if(mapPositiveAtomsBoundVariables[atom_counter].size()>0 && !mustBeBound){
							positiveAtomsToBeBound.push_back(atom_counter);
							mustBeBound=true;
						}
					}
				}

				if( !mustBeBound ){
					if( current_atom->isExternalAtom() )
						extAtoms.push_back( atom_counter );
					else
						positiveAtoms.push_back( atom_counter );

				}
			}
		} else
			if( current_atom->isBuiltIn() )
				builtInAtoms.push_back( atom_counter );
			else
				if( current_atom->isAggregateAtom() )
					aggregatesAtoms.push_back( atom_counter );
//		else if( current_atom->isExternalAtom() )
//			extAtoms.push_back( atom_counter );

	}
	rule->getVariables(variablesInRule);
}

bool OrderRule::order() {
	PartialOrders p(rule);
	p.checkIfPresentPartialOrders();

	// A first attempt to order the body ignoring cyclic dependencies
	// by iterating the atoms in the body and resolving their dependencies when are not cyclic
	while(positiveAtoms.size()>0){
		unsigned atom_counter=positiveAtoms.front();
		Atom* current_atom=rule->getAtomInBody(atom_counter);
		addSafeVariablesInAtom(current_atom, atom_counter);
		positiveAtoms.pop_front();
		unlockAtoms (positiveAtoms);
		unlockAtoms (positiveAtomsToBeBound);
		unlockAtoms (builtInAtoms);
		unlockAtoms (negativeAtoms);
		unlockAtoms( extAtoms );
	}

	// Second, solve the cyclic dependencies

	while ( builtInAtoms.size() > 0 || negativeAtoms.size() > 0 || aggregatesAtoms.size() > 0 || positiveAtomsToBeBound.size() > 0 || extAtoms.size() > 0 ){
		unsigned sizeBuiltIns = builtInAtoms.size();
		unsigned sizeNegatives = negativeAtoms.size();
		unsigned sizePositivesToBeBound = positiveAtomsToBeBound.size();
		unsigned sizeExtAtoms = extAtoms.size();
		unlockAtoms (builtInAtoms);
		unlockAtoms (negativeAtoms);
		unlockAtoms (positiveAtomsToBeBound);
		unlockAtoms( extAtoms );
		if( positiveAtomsToBeBound.size() == sizePositivesToBeBound && builtInAtoms.size() == sizeBuiltIns && sizeNegatives == negativeAtoms.size()
		        && extAtoms.size() == sizeExtAtoms ){
			unsigned sizeAggregates = aggregatesAtoms.size();
			unlockAtoms( aggregatesAtoms );
			if( aggregatesAtoms.size() == sizeAggregates && !unlockAtomWithArith( positiveAtomsToBeBound ) ){
				for(auto& atom:rewBuiltins){delete atom; atom=0;}				
				return false;
			}
		}
	}
	//If is a weak constraint check the variable in the level weight and label
	if(rule->isWeakConstraint() && !checkWeakSafety()){
		for(auto& atom:rewBuiltins){delete atom; atom=0;}
		return false;
	}
	// Check head safety once that the safe variables are known
	if(!checkHeadSafety()){
		for(auto& atom:rewBuiltins){delete atom; atom=0;}
		return false;
	}

	// Finally, set the ordered body as the body of the rule
	rule->setBody(orderedBody);

	return true;

}

bool OrderRule::checkHeadSafety(){
	set_term variableToCheck;
	if(!rule->isChoiceRule()){
		for(auto atom=rule->getBeginHead();atom!=rule->getEndHead();++atom){
			(*atom)->getVariable(variableToCheck);
		}
	}else{
		Choice* choice=dynamic_cast<Choice*> (*rule->getBeginHead());
		variableToCheck=choice->getVariableToSave();
	}

	if(safeVariables.size()<variableToCheck.size())
		return false;

	return Utils::isContained(variableToCheck,safeVariables);
}

bool OrderRule::checkWeakSafety(){
	set_term variableToCheck;
	WeakConstraint* w=dynamic_cast<WeakConstraint*>(rule);
	w->getWeightLevelLabeltVariables(variableToCheck);
	return Utils::isContained(variableToCheck,safeVariables);
}

void OrderRule::addSafeVariablesInAtom(Atom* atom, unsigned pos) {
	set_term variables=mapAtomsVariables[pos];
	orderedBody.push_back(atom);
	addSafeVariablesInAtom(variables,pos,orderedBody.size()-1);
}


void OrderRule::addSafeVariablesInAtom(set_term& variables, unsigned pos,unsigned bodypos) {
	for(auto variable: variables){
		if(!mapVariablesAtoms.count(variable))
			mapVariablesAtoms.insert({variable,bodypos});
		safeVariables.insert(variable);
	}
}



void OrderRule::foundAnAssigment(Atom* atom, Term* bindVariable, unsigned pos) {
	atom->setAssignment(true);
	safeVariables.insert(bindVariable);
	if (!mapVariablesAtoms.count(bindVariable)){
		mapVariablesAtoms.insert( { bindVariable, orderedBody.size()-1 });
	}
	set_term variables=mapAtomsVariables[pos];
	for(auto var: variables)
		if(var->getIndex()!=bindVariable->getIndex())
			bindAtomsDependency[orderedBody.size()-1].insert(mapVariablesAtoms.find(var)->second);
}

bool OrderRule::unlockAtomWithArith(list<unsigned>& atoms) {
	list<unsigned>::iterator atomUnlocked=atoms.end();
	unordered_map<Term*, Term*, IndexForTable<Term>, IndexForTable<Term> > arithRewrited;
	unsigned atom_counter=0;
	for(auto it=atoms.begin();it!=atoms.end();++it,atom_counter++){
		Atom* atom=rule->getAtomInBody(*it);
		set_term variables=mapAtomsVariables[*it];
		if(atom->isClassicalLiteral() && !atom->isNegative()){
			if(mapPositiveAtomsBoundVariables[*it].size()>0){
				addSafeVariablesInAtom(atom,*it);
				auto newBuiltins=rewriteArith(atom,arithRewrited);
				unsigned nPos=orderedBody.size()-1;
				for(auto newBuiltin:newBuiltins){
					newBuiltin->setAssignment(true);
					orderedBody.push_back(newBuiltin);
				}
				if(!newBuiltins.empty())
					mapAtomsVariables[atom_counter]=atom->getVariable();
				for(auto var: mapPositiveAtomsBoundVariables[*it])
					bindAtomsDependency[nPos].insert(mapVariablesAtoms.find(var)->second);
				atomUnlocked=it;
				break;
			}
		}
	}
	if(atomUnlocked==atoms.end())
		return false;
	atoms.erase(atomUnlocked);
	return true;
}

void OrderRule::unlockAtoms(list<unsigned>& atoms) {
	vector<list<unsigned>::iterator> atomsUnlocked;
	for(auto it=atoms.begin();it!=atoms.end();++it){
		Atom* atom=rule->getAtomInBody(*it);
		set_term variables=mapAtomsVariables[*it];
		if(atom->isClassicalLiteral() && !atom->isNegative()){
			if(Utils::isContained(variables,safeVariables)){
				orderedBody.push_back(atom);
				atomsUnlocked.push_back(it);
			}
			else if(mapPositiveAtomsBoundVariables[*it].size()>0){
				if(Utils::isContained(mapPositiveAtomsBoundVariables[*it],safeVariables)){
					atomsUnlocked.push_back(it);
					orderedBody.push_back(atom);
					for(auto variable: variables){
						if(!mapVariablesAtoms.count(variable) && !mapPositiveAtomsBoundVariables[*it].count(variable))
							mapVariablesAtoms.insert({variable,orderedBody.size()-1});
						safeVariables.insert(variable);
					}
					for(auto var: mapPositiveAtomsBoundVariables[*it]){
						bindAtomsDependency[orderedBody.size()-1].insert(mapVariablesAtoms.find(var)->second);
					}
				}
			}
		}
		// If the atom is negative or is not an assignment or is an aggregate
		// then if every variable is safe, the atom is safe
		// and thus it can be added to the new body
		if(atom->isNegative() || (atom->isBuiltIn() && atom->getBinop()!=Binop::EQUAL) || atom->isAggregateAtom() || (atom->isExternalAtom() && atom->isNegative())){
			bool containsUnsafe=lookForVariablesUnsafe(variables, atom, it, atomsUnlocked);
			//Moreover if it is an assignment aggregate the bind variable must be inserted into the safeVariables
			if(!containsUnsafe && !atom->isNegative() && atom->isAggregateAtom() && atom->getFirstBinop()==EQUAL && !safeVariables.count(atom->getFirstGuard())){
				Term* bindVariable=atom->getFirstGuard();
				foundAnAssigment(atom, bindVariable,*it);
			}
		}
		if(atom->isBuiltIn() && atom->getBinop()==Binop::EQUAL){
			Term* firstTerm=atom->getTerm(0);
			Term* secondTerm=atom->getTerm(1);
			bool firstSafe=true;
			bool secondSafe=true;
			Term* bindVariable=nullptr;

			checkBuiltInSafety(firstSafe, firstTerm, bindVariable);
			checkBuiltInSafety(secondSafe, secondTerm, bindVariable);

			if(firstSafe && secondSafe){
				orderedBody.push_back(atom);
				atomsUnlocked.push_back(it);
				for(auto var: variables)
					bindAtomsDependency[orderedBody.size()-1].insert(mapVariablesAtoms.find(var)->second);
			}
			if(bindVariable!=nullptr && ((firstSafe && bindVariable->getIndex()==secondTerm->getIndex())
					|| (secondSafe && bindVariable->getIndex()==firstTerm->getIndex())
					|| (firstSafe && !secondSafe)
					|| (!firstSafe && secondSafe)
					)){
				atomsUnlocked.push_back(it);
				orderedBody.push_back(atom);
				foundAnAssigment(atom, bindVariable,*it);
				atom->setAssignment(true);
			}
		}
		// if the atom is a positive external atom
		// then if every input variables are safe,
		// the atom is safe and thus is added to the ordered body.
		if(atom->isExternalAtom() && !atom->isNegative()){
			set_term inputVariables = atom->getInputVariables();
			auto boudVarSize = mapPositiveAtomsBoundVariables[*it].size();
			if( Utils::isContained( inputVariables, safeVariables )
			        && (boudVarSize == 0 || (boudVarSize > 0 && Utils::isContained( mapPositiveAtomsBoundVariables[*it], safeVariables ))) ){
				atomsUnlocked.push_back( it );
				orderedBody.push_back( atom );
				set_term outVariable;
				for ( unsigned i = atom->getSeparator(); i < atom->getTermsSize(); i++ ){
					atom->getTerm( i )->getVariable( outVariable );
				}
				addSafeVariablesInAtom( outVariable, *it, orderedBody.size() - 1 );
			}
		}
	}

	for ( auto iterator : atomsUnlocked )
		atoms.erase( iterator );
}

void OrderRule::computeAtomsVariables() {
	for (unsigned i = 0; i < rule->getSizeBody(); ++i) {
		Atom* atom = rule->getAtomInBody(i);
		if (!atom->isAggregateAtom())
			mapAtomsVariables.insert( { i, atom->getVariable() });
		else{
			mapAtomsVariables.insert( { i, atom->getSharedVariable(rule->getBeginBody(),rule->getEndBody()) });
			if(atom->isNegative() || atom->getFirstBinop()!=EQUAL){
				set_term guards=atom->getGuardVariable();
				mapAtomsVariables[i].insert(guards.begin(),guards.end());
			}
		}
	}
}

void OrderRule::checkBuiltInSafety(bool& safe, Term* term, Term*& bindVariable) {
	if( term->getType() == VARIABLE ){
		if( !safeVariables.count( term ) ){
			safe = false;
			bindVariable = term;
		}
	} else{
		safe = false;
		map_term< unsigned > unsafeVars;
		for ( unsigned i = 0; i < term->getTermsSize(); ++i ){
			Term* t = term->getTerm( i );
			set_term varsInCurrentTerm;
			t->getVariable( varsInCurrentTerm );
			for ( auto t1 : varsInCurrentTerm ){
				if( !safeVariables.count( t1 ) ){
					if( unsafeVars.count( t1 ) )
						++unsafeVars[t1];
					else
						unsafeVars.insert( { t1, 1 } );
				}
			}
		}
		if( unsafeVars.empty() ){
			safe = true;
			return;
		}
		if( unsafeVars.size() == 1 && unsafeVars.begin()->second == 1 ){
			if( term->getType() == TermType::ARITH )
				for ( auto op : term->getOperators() )
					if( op.first == Operator::MODULO )
						return;
			bindVariable = unsafeVars.begin()->first;
		}

	}

}

bool OrderRule::lookForVariablesUnsafe(set_term& variables,Atom* atom, list<unsigned>::iterator it, vector<list<unsigned>::iterator>& atomsUnlocked){
	bool foundAnUnsafeVar=false;
	bool checkIfPossibleAssigment=!atom->isNegative() && atom->isAggregateAtom() && atom->getFirstBinop()==EQUAL;
	for(auto variable: variables){
		if(!safeVariables.count(variable)){
			if(checkIfPossibleAssigment && variable->getIndex()==atom->getFirstGuard()->getIndex())
				continue;
			foundAnUnsafeVar=true;
			break;
		}
	}
	if(!foundAnUnsafeVar){
		orderedBody.push_back(atom);
		atomsUnlocked.push_back(it);
		for(auto var: variables){
			if(checkIfPossibleAssigment && var->getIndex()==atom->getFirstGuard()->getIndex())continue;
			bindAtomsDependency[orderedBody.size()-1].insert(mapVariablesAtoms.find(var)->second);
		}
	}

	return foundAnUnsafeVar;
}

vector<pair<unsigned int, Atom*>> OrderRule::getAtomsFromWhichDepends(unsigned atom_position) const {
	vector<pair<unsigned int, Atom*>> atoms;
	atoms.reserve(orderedBody.size());
	for(auto atom_pos: bindAtomsDependency[atom_position])
		atoms.push_back({atom_pos,orderedBody[atom_pos]});
	return atoms;
}

} /* namespace grounder */
} /* namespace DLV2 */
