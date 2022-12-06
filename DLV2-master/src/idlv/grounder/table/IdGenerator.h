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
 * IdGenerator.h
 *
 *  Created on: Feb 18, 2015
 *      Author: davide
 */

#ifndef SRC_GROUNDER_TABLE_IDGENERATOR_H_
#define SRC_GROUNDER_TABLE_IDGENERATOR_H_

#include <vector>

using namespace std;

namespace DLV2 {
namespace grounder {


class IdGenerator {
public:
	virtual ~IdGenerator(){};

	unsigned getNewId(unsigned id=0){
		return ++(idGenerators[id]);
	}

	unsigned getTheAtomReservedId(){
		return INIT_VALUE[1];
	}

	static IdGenerator* getInstance(){
		if(idGenerator==nullptr)
			idGenerator=new IdGenerator;
		return idGenerator;
	}

	static void freeInstance(){
		if(idGenerator!=nullptr){
			delete idGenerator;
			idGenerator=nullptr;
		}
	}

private:
	IdGenerator(){
		INIT_VALUE.push_back(1);
		INIT_VALUE.push_back(2);
		idGenerators.push_back(INIT_VALUE[0]);
		idGenerators.push_back(INIT_VALUE[1]);
	};

	static IdGenerator *idGenerator;
	vector<unsigned> idGenerators;
	vector<unsigned> INIT_VALUE;
};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_TABLE_IDGENERATOR_H_ */
