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
 * Hashable.h
 *
 *  Created on: Nov 12, 2014
 *      Author: davide
 */

#ifndef HASHABLE_H_
#define HASHABLE_H_

#include <functional>
#include <unordered_set>
#include <iostream>

#include "../../util/Constants.h"

using namespace std;


namespace DLV2{

namespace grounder{


class Hashable {
public:
	Hashable(){};
	virtual ~Hashable(){};

	virtual size_t hash()=0;
};



class Indexable{
public:
	Indexable():index(0){};
	Indexable(unsigned i):index(i){};
	virtual ~Indexable() {};

	virtual index_object getIndex() const {	return index;};
	virtual void setIndex(index_object index) {	this->index = index;};

protected:
	index_object index;
};


template<typename T>
struct IndexForTable{
	inline size_t operator()(T* obj) const {
		return obj->getIndex();
	}

	inline bool operator()(T* obj1, T* obj2) const {
		return obj1->getIndex()==obj2->getIndex();
	}
};

template<typename T>
struct HashForTable{
	inline size_t operator()(T* obj) const {
		return obj->hash();
	}

	inline bool operator()(T* obj1, T* obj2) const {
		return *obj1==*obj2;
	}
};


template<typename T>
struct ConstHashForTable{
	inline size_t operator()(const T& obj) const {
		return obj.hash();
	}

	inline bool operator()(const T& obj1,const T& obj2) const {
		return obj1==obj2;
	}
};

template<typename T>
class FlyweightFactory{
public:
	FlyweightFactory(){};
	~FlyweightFactory(){for(auto obj:flyweight_set)delete obj;};

	void insert(T*& obj){
		auto result=flyweight_set.insert(obj);
		if(!result.second)
			delete obj;
		obj = *result.first;
	}

	void get(T*& obj){
		auto result=flyweight_set.find(obj);
		delete obj;
		if(result==flyweight_set.end())
			obj = nullptr;
		else
			obj = *result;
	}


	void print(){
		for(auto obj:flyweight_set)
			obj->print();
	}

	unordered_set<T*,HashForTable<T>,HashForTable<T>> flyweight_set;
};


template<typename T>
class FlyweightIndexFactory{
public:

	FlyweightIndexFactory():index_counter(0){};
	~FlyweightIndexFactory(){for(auto obj:flyweight_set){delete obj;}};

	void insert(T*& obj){
		auto result=flyweight_set.insert(obj);
		if(!result.second)
			delete obj;
		else{
			obj->setIndex(++index_counter);
		}
		obj= *result.first;
	}

	void get(T*& obj){
		auto result=flyweight_set.find(obj);
		if(result==flyweight_set.end()){
			delete obj;
			obj=nullptr;
	  }else
			obj = *result;
	}

	void print(){
		for(auto obj:flyweight_set)
			obj->print();
	}

	void callForEachObj(function<void(T*)> f){
		for(auto ele:flyweight_set)
			f(ele);
	}

	unordered_set<T*,HashForTable<T>,HashForTable<T>> flyweight_set;
	unsigned int index_counter;
};

template<typename T>
struct HashPair{
	inline size_t operator()(pair<T,T> obj) const {
		hash<T> hasher;
		size_t seed=3;
		seed ^= hasher(obj.first) + 0x9e3779b9 + (seed<<6) + (seed>>2);
		seed ^= hasher(obj.second) + 0x9e3779b9 + (seed<<6) + (seed>>2);
		return seed;
	}

	inline bool operator()(pair<T,T> obj1, pair<T,T> obj2) const {
		return obj1.first==obj2.first && obj1.second==obj2.second;
	}


};

template<class T>
using hashSet = unordered_set<T*,HashForTable<T>,HashForTable<T>>;

template<class T>
using hashIndexSet = unordered_set<T*,IndexForTable<T>,IndexForTable<T>>;


};

};

#endif /* HASHABLE_H_ */
