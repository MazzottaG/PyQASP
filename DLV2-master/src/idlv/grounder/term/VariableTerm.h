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
 * VariableTerm.h
 *
 *  Created on: 25/feb/2014
 *      Author: Davide
 */

#ifndef VARIABLETERM_H_
#define VARIABLETERM_H_


#include "Term.h"
#include "../hash/HashString.h"

using namespace std;

namespace DLV2{

namespace grounder{

/*
 * Represent a term variable
 *
 * The variable is in IdsManager for avoiding duplication
 */
class VariableTerm: public Term {
public:
	VariableTerm():Term(),isAnonymous(0),localIndex(0){};
	VariableTerm(bool negative):Term(negative),isAnonymous(0),localIndex(0){};
	VariableTerm(bool negative,index_object index,string& name):Term(negative,index),name(name),localIndex(0){setAnonymous();};
	VariableTerm(bool negative,string& name):Term(negative),name(name),localIndex(0){setAnonymous();};
	VariableTerm(bool negative,const char* name):Term(negative),name(name),localIndex(0){setAnonymous();};

	virtual string getName()const{return name;};
	virtual void setName(string& name){setAnonymous();};

	//Return the type of term
	virtual TermType getType()const{if(isAnonymous)return TermType::ANONYMOUS;return TermType::VARIABLE;};
	//Return true if contain a term of the given type
	virtual bool contain(TermType type){
		if(TermType::VARIABLE==type || (TermType::ANONYMOUS==type && isAnonymous))
			return true;
		return false;
	};


	virtual void getVariable(set_term& variables){if(!isAnonymous)variables.insert(this);};
	virtual void getVariable(vector<Term*>& variables){if(!isAnonymous)variables.push_back(this);};

	virtual size_t hash(){	return HashString::getHashString()->computeHash(name);};
	virtual Term* substitute(var_assignment& substitutionTerm){
		if(isAnonymous)return this;
		Term *findTerm=substitutionTerm[localIndex];
		if(findTerm!=nullptr)
			return findTerm;
		return this;
	};

	virtual void print(ostream& stream=cout)const{
		stream<<name;
	}


	virtual bool operator>(const Term& term)const{
		if(getType()!=term.getType())return false;
		return getName()>getName();
	};

	virtual bool operator>=(const Term& term)const{
		if(getType()!=term.getType())return false;
		return getName()>=getName();
	}

	virtual bool operator<(const Term& term)const {
		if(getType()!=term.getType())return false;
		return getName()<getName();
	};
	virtual bool operator<=(const Term& term)const{
		if(getType()!=term.getType())return false;
		return getName()<=getName();
	};

	virtual LINE transformToLineEq(){
		LINE A;
		A.a=0;
		A.k=1;
		return A;
	}

	inline virtual index_object getLocalVariableIndex()const{return localIndex;};
	inline virtual void setLocalVariableIndex(index_object index){localIndex=index;};

	virtual string toString()const{return name;}


private:
	void setAnonymous(){
		if(name.compare("_")==0)
			isAnonymous=true;
		else
			isAnonymous=false;
	}

	/**
	 *   The name of a variable
	 */
	string name;
	/*
	 *  True if is an anonymous variable ''_''
	 */
	bool isAnonymous;

	index_object localIndex;

};

};

};

#endif /* VARIABLETERM_H_ */
