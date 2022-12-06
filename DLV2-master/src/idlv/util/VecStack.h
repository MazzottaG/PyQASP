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
 * VecStack.h
 *
 *  Created on: May 10, 2016
 *      Author: davide
 */

#ifndef SRC_UTIL_VECSTACK_H_
#define SRC_UTIL_VECSTACK_H_

#include <vector>

using namespace std;

namespace DLV2 {

template<typename T>
struct VecStack {
	VecStack():topIndex(0){};
	void reserve(unsigned s){vec.reserve(s);};
	T& pop(){
		--topIndex;
		return vec[topIndex];
	}
	void push(T t){
		if(topIndex==vec.size())
			vec.push_back(t);
		else
			vec[topIndex]=t;
		topIndex++;
	}

	vector<T> vec;
	unsigned topIndex;
};

} /* namespace DLV2 */

#endif /* SRC_UTIL_VECSTACK_H_ */
