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
 * Predicate.h
 *
 *  Created on: 07/mar/2014
 *      Author: Jessica
 */

#ifndef PREDICATE_H_
#define PREDICATE_H_

#include <string>
#include <cstring>
#include "../hash/Hashable.h"
#include "../hash/HashVecInt.h"
#include "../hash/HashString.h"

using namespace std;

namespace DLV2{

namespace grounder{


class Predicate : public Hashable, public Indexable{
public:

	///Default constructor
	Predicate() : Indexable(), arity(0), edb(true), hiddenForPrinting(false), solved(true),trueNegated(false) {};

	/** Constructor
	 * @param name set the name of the predicate
	 * @param arity set the arity of the predicate
	 */
	Predicate(string& name, unsigned int arity) : Indexable(), arity(arity), name(name), edb(true), hiddenForPrinting(false), solved(true),trueNegated(false) {};


	/** Constructor
	 * @param name set the name of the predicate
	 * @param arity set the arity of the predicate
	 * @param edbIdb set whether the the predicate is an EDB or not
	 */
	Predicate(string& name, unsigned int arity, bool edbIdb) : Indexable(), arity(arity), name(name), edb(edbIdb), hiddenForPrinting(false), solved(true),trueNegated(false)  {};



	///Getter method for the arity
	unsigned int getArity() const {return arity;}
	///Setter method for the arity
	void setArity(unsigned int arity) {this->arity = arity;}
	///Getter method for the arity
	bool isEdb() const {return edb;}

	void setIdb() {this->edb=false;};

	void setEdb() {this->edb=true;};
	///Getter method for the name
	const string getName() const {return name;}
	///Setter method for the name
	void setName(const string& name) {this->name = name;}

	/// @brief Equal-to operator for predicates
	/// @details Two predicates are equal if they have the same name and the same arity
	bool operator==(Predicate &p)const{return p.getArity()==this->getArity() && p.getName()==this->getName() && trueNegated==p.trueNegated;}

	virtual size_t hash(){
		vector<size_t> hash_vec(2);
		string nameToHash=(trueNegated)?"-"+name:name;
		hash_vec[0]=HashString::getHashString()->computeHash(nameToHash);
		hash_vec[1]=arity;
		return HashVecInt::getHashVecInt()->computeHashSize_T(hash_vec);
	}

	bool isHiddenForPrinting() const {return hiddenForPrinting;}
	void setHiddenForPrinting(bool hiddenForPrinting) {this->hiddenForPrinting = hiddenForPrinting;}

	~Predicate() {}

	inline bool isSolved() const {	return solved;}
	inline void setSolved(bool solved) { this->solved = solved; }

	inline bool isTrueNegated(){return trueNegated;}
	inline void setTrueNegated(bool trueN){trueNegated=trueN;}

	void print(){
		cout<<"PREDICATE: "<<name<<" INDEX: "<<index<<endl;
	}

	inline string getNameArity(){
		string s=(trueNegated)?"-":"";
		return s+name+"\\"+to_string(arity);
	}

private:
	///Arity
	unsigned int arity;
	///Name
	string name;
	///Whether it is EDB or IDB
	bool edb;
	///Whether the predicate is hidden from printing
	bool hiddenForPrinting;
	///Whether the predicate is solved (not undefined)
	bool solved;
	///Is true if the predicate have is a true negation
	bool trueNegated;
};

template<typename T>
using map_predicate = unordered_map<Predicate*, T, IndexForTable<Predicate>, IndexForTable<Predicate>>;

typedef hashIndexSet<Predicate> set_predicate;
typedef unordered_map<Predicate*, vector<unsigned>, IndexForTable<Predicate>,IndexForTable<Predicate>> mapIndexingPredicates;


};

};

#endif /* PREDICATE_H_ */
