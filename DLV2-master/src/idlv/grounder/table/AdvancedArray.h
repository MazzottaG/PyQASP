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
 * AdvancedArray.h
 *
 */

#ifndef SRC_GROUNDER_TABLE_ADVANCEDARRAY_H_
#define SRC_GROUNDER_TABLE_ADVANCEDARRAY_H_

#include <array>
namespace DLV2 {
namespace grounder {

using namespace std;


template <typename T,unsigned S>
class AdvancedArray {
public:

	AdvancedArray():chooseStaticArray(true){}

	AdvancedArray(const AdvancedArray& array):chooseStaticArray(array.chooseStaticArray),staticArray(array.staticArray),dynamicArray(array.dynamicArray){};

	inline void setSize(unsigned size){
		if(size<S)
			chooseStaticArray=true;
		else{
			chooseStaticArray=false;
			dynamicArray.resize(size);
		}
	}

	inline void setSize(unsigned size,T element){
		if(size<S){
			chooseStaticArray=true;
			staticArray.fill(element);
		}else{
			chooseStaticArray=false;
			dynamicArray.resize(size,element);
		}
	}

	inline T& operator[](unsigned i){
		if(chooseStaticArray)
			return staticArray[i];
		return dynamicArray[i];
	}

	inline T operator[](unsigned i) const {
		if(chooseStaticArray)
			return staticArray[i];
		return dynamicArray[i];
	}

	inline unsigned size() const{
		if(chooseStaticArray)
			return S;
		return dynamicArray.size();
	}

	void fill(T element){
		if(chooseStaticArray)
			staticArray.fill(element);
		dynamicArray.assign(dynamicArray.size(),element);
	}

private:


	bool chooseStaticArray;
	array<T,S> staticArray;
	vector<T> dynamicArray;
};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_TABLE_ADVANCEDARRAY_H_ */
