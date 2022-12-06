/*******************************************************************************
 *   Copyright 2016 Francesco Calimeri, Davide Fusca', Simona Perri and Jessica Zangari
 *  
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *  
 *        http://www.apache.org/licenses/LICENSE-2.0
 *  
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *******************************************************************************/
/*
 * LpoptException.h
 *
 *  Created on: Mar 28, 2017
 *      Author: jessica
 */

#ifndef SRC_GROUNDER_EXCEPTION_LPOPTEXCEPTION_H_
#define SRC_GROUNDER_EXCEPTION_LPOPTEXCEPTION_H_

#include <exception>
using namespace std;

namespace DLV2 {
namespace grounder {

class LpoptException : public exception {
public:
	LpoptException() {}
	LpoptException(string message) : msg(message) {}

	virtual const char* what() const throw(){
		return msg.c_str();
	}

private:
	string msg;
};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_EXCEPTION_LPOPTEXCEPTION_H_ */
