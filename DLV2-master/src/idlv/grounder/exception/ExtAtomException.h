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
 * ExtAtomException.h
 *
 *  Created on: 24 lug 2016
 *      Author: giovanni
 */

#ifndef EXTATOMEXCEPTION_H_
#define EXTATOMEXCEPTION_H_

#include <exception>
#include<string>
using namespace std;

namespace DLV2 {
namespace grounder {

class ExtAtomException : public exception {
	public:
		ExtAtomException(){
		}
		ExtAtomException( string message ) :
				message( message ){
		}
		virtual ~ExtAtomException(){
		}

		virtual const char* what() const throw (){
			return message.c_str();
		}
		string getMessage() const throw (){
			return message;
		}
	private:
		string message;
};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* EXTATOMEXCEPTION_H_ */
