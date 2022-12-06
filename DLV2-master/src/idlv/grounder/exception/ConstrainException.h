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
 * ConstrainException.h
 *
 *  Created on: Jul 21, 2015
 *      Author: davide
 */

#ifndef SRC_GROUNDER_EXCEPTION_CONSTRAINEXCEPTION_H_
#define SRC_GROUNDER_EXCEPTION_CONSTRAINEXCEPTION_H_

#include <exception>

namespace DLV2 {
namespace grounder {

using namespace std;

class ConstrainException:public exception {
public:
	ConstrainException(){};
	virtual const char* what() const throw(){
		return "";
	}
} ;

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_EXCEPTION_CONSTRAINEXCEPTION_H_ */
