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
 * SparqlDirective.h
 *
 *  Created on: Jan 3, 2017
 *      Author: david
 */

#ifndef SRC_DIRECTIVES_SPARQLDIRECTIVE_H_
#define SRC_DIRECTIVES_SPARQLDIRECTIVE_H_

#include <string>
#include <regex>
#include <vector>

#include "Directive.h"
#include "../../common/ErrorMessage.h"
#include "../util/Utils.h"

using namespace std;

namespace DLV2 {
namespace grounder {

class SparqlDirective : public Directive{
public:
	SparqlDirective(char* v): Directive(v){};

	virtual bool split(){return false;};

	virtual string getURL(){return url;}

	virtual string predicateName(){return "";}
	virtual unsigned predArity(){return 0;}

	virtual string getQuery(){return "";}

	virtual vector<TypeConv>& getTypeConv()=0;

	virtual ~SparqlDirective(){};
protected:
	string url;
};

class SparqlImportDirective : public SparqlDirective{
public:
	SparqlImportDirective(char* v): SparqlDirective(v),arity(0){};
//	SparqlImportDirective(SparqlImportDirective&& s): query(move(s.query)),predName(move(s.predName)),arity(s.arity),typeConvs(move(s.typeConvs)),url(move(s.url)){};

//	SparqlImportDirective(const SparqlImportDirective& that) = delete;

	virtual bool split(){
		regex_token_iterator<string::iterator> rend;
		//FIXME make typeConvs optionals within regex
		regex e("^\\(\"(.+?)\",\\s*\"(.+?)\",\\s*(.+?),\\s*([0-9]+?),\\s*type:\\s*(.+?)\\)\\s*.");
		smatch m;
		if(regex_search(value,m,e)) {
			for(unsigned i=1;i<m.size();i++) {
				switch (i) {
					case 1:
						url = m[i];
						break;
					case 2:
						query = m[i];
						break;
					case 3:
						predName = m[i];
						break;
					case 4:
						arity = atoi(string(m[i]).c_str());
						break;
					case 5:
						string tc = m[i];
						regex ee("\\s*,\\s*");
						regex_token_iterator<string::iterator> tokens(tc.begin(), tc.end(), ee, -1);
						regex eee("(U_INT|UT_INT|UR_INT|T_INT|R_INT|CONST|Q_CONST)");
						while(tokens != rend){
							string s = *tokens;
							if(!regex_match(s, eee)) {
								ErrorMessage::errorDuringParsing("#import_local_sparql Invalid type conv: \"" + s +"\". Only {U_INT, UT_INT, UR_INT, T_INT, R_INT, CONST, Q_CONST} are possible.");
								return false;
							}
							typeConvs.push_back(Utils::toTypeConv(s));
							tokens++;
						}
						break;
				}
			}
		}else{
			ErrorMessage::errorDuringParsing("--> Syntax Error in #export"+value);
			return false;
		}
		return true;
	};

	 string predicateName(){return predName;}
	 unsigned predArity(){return arity;}

	 string getQuery(){return query;}

	 vector<TypeConv>& getTypeConv(){return typeConvs;}

	 ~SparqlImportDirective(){};
private:
	string query;
	string predName;
	unsigned arity;
	vector<TypeConv> typeConvs;
};





} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_DIRECTIVES_SPARQLDIRECTIVE_H_ */
