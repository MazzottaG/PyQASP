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
 * TermTable.h
 *
 *  Created on: 26/feb/2014
 *      Author: Davide
 */

#ifndef TERMTABLE_H_
#define TERMTABLE_H_


#include "../term/Term.h"
#include "../term/ConstantTerm.h"
#include "../term/VariableTerm.h"
#include "../hash/Hashable.h"

using namespace std;

namespace DLV2{

namespace grounder{


/*
 *  TermTable is an abstract class that represent a table of all Term in
 *  the logical program
 *
 *  TermTable is a Singleton
 */
class TermTable {
public:
	///Add term in the table and return the index
	virtual void addTerm(Term *&t){	terms.insert(t);}
	/// Get term
	virtual void getTerm(Term*& t){	terms.get(t);}
	/// Get size of the table
	virtual unsigned int getSize(){	return terms.flyweight_set.size();}
	/// Get the number of collision of the IdManager
	//TODO
	virtual int getCollision(){return 0;};

	virtual Term* generateVariableAuxTerm(set_term& variablesInRule){
		Term * t=nullptr;
		do{
			t=generateNewVariable(NEW_VARIABLE_NAME+to_string(counter++));
		}while(variablesInRule.count(t));

		return t;
	}

	virtual Term* generateNewVariable(string name){
		Term * term=new VariableTerm(false,name);
		addTerm(term);
		return term;
	}


	// Method for fetch the instance of singleton
    // If termTable is null instance the table based on parameter in the configuration
	static TermTable* getInstance();
	virtual ~TermTable(){};

	Term* getTermAnonymous(){
			if(term_anonymous==nullptr){
					term_anonymous = new VariableTerm(false,"_");
					addTerm(term_anonymous);
			}
			return term_anonymous;
	}

	bool containsVariable(){
			for(auto t:terms.flyweight_set)
					if(t->getType()==VARIABLE || t->getType()==ANONYMOUS)
							return true;
			return false;
	}

	static void freeInstance(){
		if(termTable!=nullptr){
			delete termTable;
			termTable=nullptr;
		}
	}

	//please not modify me :(
	Term* term_max;
	Term* term_min;
	Term* term_zero;
	Term* term_minus_one;
	Term* term_one;
protected:
	// Instance of the singleton
	static TermTable* termTable;
private:
	static constexpr char NEW_VARIABLE_NAME[]="AUX";

//	TermTable(): term_max(new StringConstantTerm(false,"+∞")), term_min(new StringConstantTerm(false,"-∞")), term_zero(new NumericConstantTerm(false,0)) {
	TermTable(): term_max(new StringConstantTerm(false,"#sup")), term_min(new StringConstantTerm(false,"#inf")),
		term_zero(new NumericConstantTerm(false,0)),term_minus_one(new NumericConstantTerm(false,-1)),
		term_one(new NumericConstantTerm(false,1)),counter(0),term_anonymous(nullptr){
		addTerm(term_max);
		addTerm(term_min);
		addTerm(term_zero);
		addTerm(term_minus_one);
		addTerm(term_one);
	}
	/*
	 *  Flyweight Factory of terms
	 */
	FlyweightIndexFactory<Term> terms;

	unsigned counter;
        Term* term_anonymous;
};


};

};

#endif /* TERMTABLE_H_ */
