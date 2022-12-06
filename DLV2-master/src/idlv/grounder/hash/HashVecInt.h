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
 * HashVecInt.h
 *
 *  Created on: Jun 17, 2014
 *      Author: david
 */

#ifndef HASHVECINT_H_
#define HASHVECINT_H_

#include <vector>
#include <iostream>
#include <cmath>

#include "../../util/Constants.h"
#include "Hashable.h"
#include "../term/Term.h"

using namespace std;


namespace DLV2{

namespace grounder{

/**
 *  Calculate the hash of a vector of integers
 */
class HashVecInt {
public:
	HashVecInt(){};

	/// Calculate the hash of a vector of index_object
	virtual size_t computeHash(const vector<index_object>& values)=0;
	/// Calculate the hash of a vector of size_t
	virtual size_t computeHashSize_T(const vector<size_t>& values)=0;
	/// Calculate the hash of a vector of size_t
	virtual size_t computeHashTerm(const vector<Term*>& values)=0 ;

	static void freeInstance(){ if(hashInt!=0) delete hashInt;}
	virtual ~HashVecInt(){}
	/// Return an HashVecInt according the configuration
	static HashVecInt* getHashVecInt();
private:
	static HashVecInt *hashInt;
};

/**
 *  Calculate the hash with java method
 */
class JavaHashVecInt : public HashVecInt{
public:
	JavaHashVecInt(){}
	inline size_t computeHash(const vector<index_object> &values){
		int length=values.size();
		size_t hash=0;
		for (int i = 0; i < length; i++) {
			hash+=values[i]*pow(31.0,length-(i));
		}

		return hash;
	}
	inline size_t computeHashSize_T(const vector<size_t> &values){
		int length=values.size();
		size_t hash=0;
		for (int i = 0; i < length; i++) {
			hash+=values[i]*pow(31.0,length-(i));
		}

		return hash;
	}

	inline size_t computeHashTerm(const vector<Term*> &values){
		int length=values.size();
		size_t hash=0;
		for (int i = 0; i < length; i++) {
			hash+=values[i]->getIndex()*pow(31.0,length-(i));
		}

		return hash;
	}

};

/**
 *  Calculate the hash with boost combine method
 */
class BoostCombineHashVecInt : public HashVecInt{
public:
	inline size_t computeHash(const vector<index_object> & values){
        size_t seed = 0;
        for(auto e:values)
        {
		    hash<size_t> hasher;
		    seed ^= hasher(e) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
        return seed;
	}

	inline size_t computeHashSize_T(const vector<size_t> & values){
        size_t seed = 0;
        for(auto e:values)
        {
		    hash<size_t> hasher;
		    seed ^= hasher(e) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
        return seed;
	}

	inline size_t computeHashTerm(const vector<Term*> & values){
		if(values.empty())
			return 0;
		size_t seed=values[0]->getIndex();
		for(unsigned i=1;i<values.size();i++){
		    hash<size_t> hasher;
		    seed ^= hasher(values[i]->getIndex()) + 0x9e3779b9 + (seed<<6) + (seed>>2);
		}
		return seed;
	}

};

};

};
#endif /* HASHVECINT_H_ */
