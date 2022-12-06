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
 * Query.h
 *
 *  Created on: 05 gen 2017
 *      Author: jessica
 */

#ifndef SRC_DIRECTIVES_QUERY_H_
#define SRC_DIRECTIVES_QUERY_H_

#include <string>
#include <regex>
#include "../grounder/table/PredicateExtension.h"
#include "../../common/ErrorMessage.h"
using namespace std;

namespace DLV2{
namespace grounder{

/// This class represents the generic concept of query
class Query {
protected:
	string query;
public:
	Query(){}
	Query(const string& q):query(q){};
	virtual ~Query(){};

	const string& getQuery() const {return query;}
	void setQuery(const string& query) {this->query = query;}

	/// This method append a novel condition in the query,
	/// filtering values accordingly to the dictionary of the given predicate on the given position.
	/// The condition is applied on the given query parameter and is added according to the specified append mode;
	/// while the combine mode specify how to combine the values in the dictionary.
	virtual void addCondition(unsigned parameter,Predicate* predicate,unsigned pos) = 0;

	vector<string> split(const string& input, const string& regx) {
	    regex re(regx);
	    sregex_token_iterator it(input.begin(), input.end(), re, -1);
	    sregex_token_iterator reg_end;
	    vector<string> v;
	    for (; it != reg_end; ++it)
	    	v.push_back(it->str());
	    return v;
	}
};

/// This class represents the generic concept of an SQL query
class QuerySQL : public Query {
public:
	QuerySQL(){};
	QuerySQL(const string& q):Query(q){};
	virtual void addCondition(unsigned parameter,Predicate* predicate,unsigned pos) {
		string lower="";
		for(auto e:query)
			lower+=tolower(e);

		string condition=" (";
		regex ree("^\\s*select\\s*(.+?)\\s*from\\s*(.+?)\\s*where\\s*(.+)");
		smatch m;
		if(!std::regex_search(lower,m,ree)){
			regex ree2("^\\s*select\\s*(.+?)\\s*from\\s*(.+)");
			if(!std::regex_search(lower,m,ree2))
				return;
		}
		string parameterQ="";
		if(m.size()<2)
			return;
		string parameters=m[1];
		if(parameters.find("*")!=string::npos)
			return;
		vector<string> v=split(parameters,",");
		for(unsigned i=0;i<v.size();i++){
			if(i==parameter){
				parameterQ=v[i];
				if(parameterQ.find(" ")!=string::npos){
					regex ree3("(\\w+)\\s+(as\\s+|AS\\s+|As\\s+|aS\\s+)?(\\w*)\\s*$");
					smatch m3;
					if(!std::regex_search(parameterQ,m3,ree3) || m3.size()<4)
						return;
					if(m3[3]!="")
						parameterQ=m3[3];
				}
			}
		}
		auto b=PredicateExtTable::getInstance()->getPredicateExt(predicate)->getPredicateInformation()->getDictionaryBegin(pos);
		auto e=PredicateExtTable::getInstance()->getPredicateExt(predicate)->getPredicateInformation()->getDictionaryEnd(pos);
		for(auto it3=b;it3!=e;++it3){
			if(it3==b)
				condition+=" "+parameterQ+" = "+(*it3)->getName();
			else
				condition+=" OR "+parameterQ+" = "+(*it3)->getName();
		}
		condition+=" ) ";

		size_t wherePos=lower.find("where");
		if(wherePos!=string::npos)
			query.insert(wherePos+6, condition);
		else
			query+=" where"+ condition;
	}

};

/// This class represents the generic concept of a SPARL query
class QuerySPARQL : public Query {
public:
	QuerySPARQL(){};
	QuerySPARQL(const string& q):Query(q){};
	virtual void addCondition(unsigned parameter,Predicate* predicate,unsigned pos) {
		regex regx("(.*)where(.*)", regex_constants::icase);
		string condition="";
		if(regex_match(query,regx))
			condition=" { VALUES ";
		else
			condition=" WHERE { VALUES ";

		regex ree("\\s*from\\s*(.+?)\\s*select\\s*(reduced|distinct)?\\s*(.+?)\\s*where\\s*(.+)", std::regex_constants::icase);
		std::smatch m;
		if(!std::regex_search(query,m,ree)){
			regex ree2("^\\s*select\\s*(.+?)\\s*from\\s*(.+)", std::regex_constants::icase);
			if(!std::regex_search(query,m,ree2)){
				ErrorMessage::errorDBConnection("--> Syntax Error in #import: "+query);
				return;
			}
		}
		string parameterQ="";
		if(m.size()>3){
			string parameters=m[3];
			//TODO fix query with "select *" --> find columns
			if(m[2]=="*")
				return;
			vector<string> v=split(parameters,",");
			for(unsigned i=0;i<v.size();i++){
				if(i==parameter){
					parameterQ=v[i];
				}
			}
		}
		else{
			ErrorMessage::errorDBConnection("--> Syntax Error in #import: "+query);
			return;
		}
		auto b=PredicateExtTable::getInstance()->getPredicateExt(predicate)->getPredicateInformation()->getDictionaryBegin(pos);
		auto e=PredicateExtTable::getInstance()->getPredicateExt(predicate)->getPredicateInformation()->getDictionaryEnd(pos);
		condition+=" ("+parameterQ+") { ";
		for(auto it3=b;it3!=e;++it3){
			condition+="("+(*it3)->getName()+")\n";
		}
		condition+=" } ";
		query+=condition;
	}
};

}
}

#endif /* SRC_DIRECTIVES_QUERY_H_ */
