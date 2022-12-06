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
 * ImportDirective.h
 *
 *  Created on: Jul 13, 2016
 *      Author: jessica
 */

#ifndef SRC_DIRECTIVES_IMPORTDIRECTIVE_H_
#define SRC_DIRECTIVES_IMPORTDIRECTIVE_H_

#include "DBDirective.h"
#include <string>
#include <regex>
#include "../../common/ErrorMessage.h"
using namespace std;

namespace DLV2 {
namespace grounder {


class ImportDirective : public DBDirective {
public:
	ImportDirective( char* v ): DBDirective(v),query(new QuerySQL) {}

	virtual string getPredicateName() const {return predName;}

	virtual Query* getQuery() const {return query;}

	const string& getTypesConvertion() const {return typesConvertion;}

	virtual bool split();
	virtual ~ImportDirective(){delete query;}

private:
	string predName, typesConvertion;
	QuerySQL* query;
};

inline bool ImportDirective::split(){
	regex_token_iterator<string::iterator> rend;
	regex ree("^\\s*\\(\\s*(.+?)\\s*,\\s*\\\"(.+?)\\\"\\s*,\\s*\\\"(.+?)\\\"\\s*,\\s*\\\"(.+?)\\\"\\s*,\\s*([a-z][A-Za-z_0-9]*?)\\s*,\\s*type\\s*:\\s*(.+?)\\s*\\)\\s*\\.");
	std::smatch m;
	if(!std::regex_search(value,m,ree)){
		regex ree2("^\\s*\\(\\s*(.+?)\\s*,\\s*\\\"(.+?)\\\"\\s*,\\s*\\\"(.+?)\\\"\\s*,\\s*\\\"(.+?)\\\"\\s*,\\s*([a-z][A-Za-z_0-9]*?)\\s*\\)\\s*\\.");
		if(!std::regex_search(value,m,ree2)){
			ErrorMessage::errorDBConnection("--> Syntax Error in #import_sql"+value);
			return false;
		}
	}

	for(unsigned i=1;i<m.size();i++){
		switch (i-1) {
			case 0:
				db=m[i];
				break;
			case 1:
			{
				username=m[i];
				break;
			}
			case 2:
			{
				password=m[i];
				break;
			}
			case 3:
			{
				query->setQuery(m[i]);
				break;
			}
			case 4:
				predName=m[i];
				break;
			case 5:
			{
				typesConvertion=m[i];
				regex re("\\s*,\\s*");
				regex_token_iterator<string::iterator> tokens(typesConvertion.begin(),typesConvertion.end(), re, -1);
				regex ree3("(U_INT|UT_INT|UR_INT|T_INT|R_INT|CONST|Q_CONST)");
				while(tokens!=rend){
					string s=*tokens;
					if(!regex_match(s,ree3)){
						ErrorMessage::errorDBConnection("--> Syntax Error in #import_sql"+value);
						return false;
					}
					tokens++;
				}
				break;
			}
			default:
				break;
		}
	}
	return true;
}

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_DIRECTIVES_IMPORTDIRECTIVE_H_ */
