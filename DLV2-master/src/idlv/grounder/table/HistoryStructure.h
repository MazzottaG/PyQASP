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
 * HistoryVector.h
 *
 *  Created on: Jan 18, 2016
 *      Author: davide
 */

#ifndef SRC_GROUNDER_TABLE_HISTORYSTRUCTURE_H_
#define SRC_GROUNDER_TABLE_HISTORYSTRUCTURE_H_

#include<vector>
#include <unordered_map>
#include "../../../common/Assert.h"
#include "../hash/Hashable.h"

using namespace std;

namespace DLV2 {
namespace grounder {

enum SearchType{ALL,OLD,NEW};

template<typename T>
class HistorySet{
public:
	bool insert(T* obj,unsigned iteration){
		if(set.count(obj)) return false;
		return set.insert({obj,iteration}).second;
	}
	T* find(T* obj,const SearchType type,const unsigned iteration){
		auto findIt=set.find(obj);
		if(findIt==set.end())return nullptr;
		switch (type) {
			case ALL:
				if(findIt->second<iteration)
					return findIt->first;
				return nullptr;
				break;
			case OLD:
				if(findIt->second<iteration-1)
					return findIt->first;
				return nullptr;
				break;
			case NEW:
				if(findIt->second==iteration-1)
					return findIt->first;
				return nullptr;
				break;
			default:
				return nullptr;
		}
	}

	void clear(){
		set.clear();
	}
private:
	unordered_map<T*,unsigned,HashForTable<T>,HashForTable<T>> set;
};


template<typename T>
class HistoryVector : public vector<T>{
public:
	using ElementsType = vector<T>;
	typedef typename std::vector<T>::iterator iterator;

	HistoryVector():current_iteration(0),index_iteration(0),index_previous_iteration(0){}
	HistoryVector(unsigned iteration):current_iteration(iteration),index_iteration(0),index_previous_iteration(0){}

	void updateIndices(unsigned iteration){
		if(current_iteration+1==iteration){
			index_previous_iteration=index_iteration;
			index_iteration=vector<T>::size();
			current_iteration=iteration;
		}else if(current_iteration+1<iteration){
			index_previous_iteration=vector<T>::size();
			index_iteration=vector<T>::size();
			current_iteration=iteration;
		}
	}

	unsigned size_iteration(SearchType type){
		unsigned start=(type==NEW)? index_previous_iteration:0;
		unsigned end=(type==OLD)?index_previous_iteration:index_iteration;
		return end-start;
	}

	pair<unsigned,unsigned> getElements(SearchType type,unsigned iteration){
		///TODO AFTER test it DELETE ASSERT
		assert(current_iteration==iteration);

		unsigned start=(type==NEW)? index_previous_iteration:0;
		unsigned end=(type==OLD)?index_previous_iteration:index_iteration;

		return {start,end};
	}


	inline unsigned getCurrentIteration(){
		return current_iteration;
	}


	inline unsigned getDeltaIndexIteration(){
		return index_iteration;
	}

	inline unsigned getNFIndexIteration(){
		return index_previous_iteration;
	}

	void push_back_delta(T& element){
		vector<T>::push_back(element);
		index_iteration=vector<T>::size();
	}

	void push_back_nf(T& element){
		vector<T>::push_back(element);
		index_previous_iteration=vector<T>::size();
		index_iteration=vector<T>::size();
	}

	void print(ostream& o=cout){
		for(auto e:*this){
			e->print(o);o<<" ";
		}
		o<<endl;
	}

private:
	unsigned current_iteration;
	unsigned index_iteration;
	unsigned index_previous_iteration;

};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_TABLE_HISTORYSTRUCTURE_H_ */
