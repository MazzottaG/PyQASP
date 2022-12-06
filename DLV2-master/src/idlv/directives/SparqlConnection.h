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
 * SparqlConnection.h
 *
 *  Created on: Jan 3, 2017
 *      Author: david
 */

#ifndef SRC_DIRECTIVES_SPARQLCONNECTION_H_
#define SRC_DIRECTIVES_SPARQLCONNECTION_H_

#include <string>
#include "SparqlDirective.h"
#include "../grounder/atom/ClassicalLiteral.h"


using namespace std;

namespace DLV2 {
namespace grounder {

class SparqlConnection {
public:
	using result_values=vector<vector<string>>;
	virtual ~SparqlConnection(){};
#ifdef SPARQL
	bool importFromFile(SparqlImportDirective& directive);
#else
	bool importFromFile(SparqlImportDirective& directive){
		ErrorMessage::errorSparqlImport("Sparql import not supported.");
		return false;
	}
#endif


	static SparqlConnection* getInstance(){
		if(instance==nullptr)
			instance=new SparqlConnection();
		return instance;
	}
	static void freeInstance(){ if(instance==nullptr) return; delete instance; instance=nullptr;}

private:
	SparqlConnection():filePrefix("file:"),tb(TermTable::getInstance()),id(0){};
	static SparqlConnection* instance;

	void parseResultAndAddInEDB(char* resultQuery,SparqlImportDirective&);
	void parseResultAndAddInEDB(result_values& values,SparqlImportDirective& directive);


	Term* getTermFromString(TypeConv tc,string& param);

	string filePrefix;
	TermTable *tb;
	static constexpr char FORMAT[]="json";
	unsigned id;
};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_DIRECTIVES_SPARQLCONNECTION_H_ */
