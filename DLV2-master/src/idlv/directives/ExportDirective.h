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
 * ExportDirective.h
 *
 *  Created on: Jul 13, 2016
 *      Author: jessica
 */

#ifndef SRC_DIRECTIVES_EXPORTDIRECTIVE_H_
#define SRC_DIRECTIVES_EXPORTDIRECTIVE_H_

#include "DBDirective.h"
#include <string>
#include <regex>
#include "../../common/ErrorMessage.h"
using namespace std;

namespace DLV2 {
namespace grounder {

class ExportDirective: public DBDirective {
public:
	ExportDirective( char* v ): DBDirective(v), predArity(0) {}

	virtual string getPredicateName() const {return predName;}

	const string& getReplaceCondition() const {return replaceCondition;}
	void setReplaceCondition(const string& replaceCondition) {this->replaceCondition = replaceCondition;}

	const string& getTableName() const {return tableName;}
	void setTableName(const string& tableName) {this->tableName = tableName;}

	unsigned getPredArity() const {return predArity;}
	void setPredArity(unsigned predArity) {this->predArity = predArity;}

	virtual bool split();

private:
	string predName, tableName, replaceCondition;
	unsigned predArity;
};

inline bool ExportDirective::split(){
	regex ree("^\\s*\\(\\s*(.+?)\\s*,\\s*\\\"(.+?)\\\"\\s*,\\s*\\\"(.+?)\\\"\\s*,\\s*([a-z][A-Za-z_0-9]*?)\\s*,\\s*([0-9]+?)\\s*,\\s*(.+?)\\s*,\\s*\\\"(REPLACE.*?)\\\"\\s*\\)\\s*\\.");
	std::smatch m;
	if(!std::regex_search(value,m,ree)){
		regex ree2("^\\s*\\(\\s*(.+?)\\s*,\\s*\\\"(.+?)\\\"\\s*,\\s*\\\"(.+?)\\\"\\s*,\\s*([a-z][A-Za-z_0-9]*?)\\s*,\\s*([0-9]+?)\\s*,\\s*(.+?)\\s*\\)\\s*\\.");
		if(!std::regex_search(value,m,ree2)){
			ErrorMessage::errorDBConnection("--> Syntax Error in #export_sql"+value);
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
					predName=m[i];
					break;
				}
				case 4:
				{
					predArity=stoi(m[i]);
					break;
				}
				case 5:
				{
					tableName=m[i];
					break;
				}
				case 6:
				{
					replaceCondition=m[i];
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

#endif /* SRC_DIRECTIVES_EXPORTDIRECTIVE_H_ */
