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
 * DirectiveHolder.h
 *
 *  Created on: Jul 13, 2016
 *      Author: jessica
 */

#ifndef SRC_DIRECTIVES_DIRECTIVESHOLDER_H_
#define SRC_DIRECTIVES_DIRECTIVESHOLDER_H_

#include <vector>
#include "Directive.h"
#include "ImportDirective.h"
#include "ExportDirective.h"
#include "DBConnection.h"
#include "SparqlDirective.h"
#include "SparqlConnection.h"
#include "ExtPredConversionDirective.h"
#include <string>
#include "../grounder/table/PredicateTable.h"
#include "../grounder/ground/RewriteMagicExternalSources.h"
#include "ShowDirective.h"
#include "../input/ExtAtomFactory.h"

using namespace std;

namespace DLV2 {
namespace grounder {


class DirectivesHolder {
public:

	void addImportDirective(DBDirective* directive){
		createDBConnection(directive);
		importDirectives.push_back(directive);
	}

	void addExportDirective(DBDirective* directive){
		createDBConnection(directive);
		exportDirectives.push_back(directive);
	}

	void addShowDirective(ShowDirective& sd){
		showDirective.push_back(sd);
	}

	void addSparqlImportDirective(SparqlImportDirective& directive){
		sparqlImportDirectives.push_back(directive);
	}

	void addSparqlEndpointImportDirective(SparqlImportDirective& directive){
		sparqlImportDirectives.push_back(directive);
	}

	void addExtPredConversionDirective(ExtPredConversionDirective& directive){
			extPredConversionDirectives.push_back(directive);
	}

	void exportAll(){
		for(auto exportDir:exportDirectives){
			DBConnection* connection=findDBConnection(exportDir);
			ExportDirective* ed=dynamic_cast<ExportDirective*>(exportDir);
			connection->connect();
			string n=ed->getPredicateName();
			Predicate* p=new Predicate(n,ed->getPredArity());
			PredicateTable::getInstance()->insertPredicate(p);
			connection->optimizeExportIntoDB(p,ed->getTableName(),ed->getReplaceCondition());
		}
	}

	void importAll(){
		for(auto importDir:importDirectives){
			DBConnection* connection=findDBConnection(importDir);
			connection->connect();
			ImportDirective* id=dynamic_cast<ImportDirective*>(importDir);
			if(!StatementDependency::getInstance()->getQueryAtoms().empty()){
				Predicate* p=connection->determinePredicate(importDir->getQuery()->getQuery(),importDir->getPredicateName());
				RewriteMagicExternalSources::getInstance()->addMapping(p,importDir->getQuery());
			}
			connection->importFromDB(id->getQuery()->getQuery(),id->getPredicateName(),id->getTypesConvertion());
		}
	}

	void importAllSparql() {
		auto sparqlConnection=SparqlConnection::getInstance();
		for(auto& dir: sparqlImportDirectives){
			sparqlConnection->importFromFile(dir);
		}
	}

	void connectAll(){
		for(auto connection:connections)
			connection.second->connect();
	}

	void disconnectAll(){
		for(auto connection:connections)
			connection.second->disconnect();
	}

	void applyShows();

	bool isEmptyShowDirectives() const {return showDirective.size();}

	void applyExtPredConversion(){
		unordered_map<unsigned,vector<TypeConv>> conversionMap;
		for(auto& directive : extPredConversionDirectives){
				auto tc = directive.getTypeConv();
				conversionMap.insert({directive.getPredicate()->getIndex(),tc});
		}
		auto sd=StatementDependency::getInstance();
		auto applyExtTypeConv=[&](Rule* rule){
				for(auto atom:rule->getBody()){
						if(atom->isExternalAtom()){
								auto id=atom->getPredicate()->getIndex();
								if(conversionMap.count(id))
										atom->setTypeConvs(conversionMap[id]);
						}
				}
		};
		for(auto r:sd->getRules())applyExtTypeConv(r);
		for(auto r:sd->getConstraints())applyExtTypeConv(r);
		for(auto r:sd->getWeakCs())applyExtTypeConv(r);
	}

	static DirectivesHolder* getInstance();
	static void freeInstance();

private:
	DirectivesHolder() {}

	void createDBConnection(DBDirective* directive) {
		string connectionParameters = directive->getDb()
				+ directive->getUsername() + directive->getPassword();
		if (!connections.count(connectionParameters))
			connections.insert(
					{ connectionParameters, new DBConnection(directive->getDb(),
							directive->getUsername(), directive->getPassword()) });
	}

	DBConnection* findDBConnection(DBDirective* directive) {
		string connectionParameters = directive->getDb()
				+ directive->getUsername() + directive->getPassword();
		auto it=connections.find(connectionParameters);
		if(it!=connections.end())
			return (*it).second;
		return 0;
	}

	static DirectivesHolder* directivesHolder;
	vector<DBDirective*> importDirectives;
	vector<DBDirective*> exportDirectives;
	vector<SparqlImportDirective> sparqlImportDirectives;
	vector<ShowDirective> showDirective;
        vector<ExtPredConversionDirective> extPredConversionDirectives;

	unordered_map<string,DBConnection*> connections;
};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_DIRECTIVES_DIRECTIVESHOLDER_H_ */
