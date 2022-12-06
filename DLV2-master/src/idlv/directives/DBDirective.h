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
 * DBDirective.h
 *
 *  Created on: Jul 13, 2016
 *      Author: jessica
 */

#ifndef SRC_DIRECTIVES_DBDIRECTIVE_H_
#define SRC_DIRECTIVES_DBDIRECTIVE_H_

#ifdef MINGW
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#endif

#include <string>


#include "Directive.h"
#include "Query.h"
using namespace std;

namespace DLV2 {
namespace grounder {

class DBDirective: public Directive {
public:
	DBDirective( char* n, char* v ): Directive(n,v), connected(false){}
	DBDirective( char* v ): Directive(v), connected(false){}

	const string& getDb() const {return db;}
	void setDb(const string& db) {this->db = db;}

	const string& getPassword() const {return password;}
	void setPassword(const string& password) {this->password = password;}

	const string& getUsername() const {return username;}
	void setUsername(const string& username) {this->username = username;}

	virtual Query* getQuery() const {return nullptr;}
	virtual string getPredicateName() const {return "";};

	virtual bool split(){return false;};

protected:
	string db, username, password;
	bool connected;
};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_DIRECTIVES_DBDIRECTIVE_H_ */
