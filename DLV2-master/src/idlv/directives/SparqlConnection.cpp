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
 * SparqlConnection.cpp
 *
 *  Created on: Jan 3, 2017
 *      Author: david
 */

#include "SparqlConnection.h"

#ifdef SPARQL
#include <redland.h>
#else

#endif

#include <regex>
#include <cstring>

#include "../grounder/table/PredicateExtension.h"


namespace DLV2 {
namespace grounder {

SparqlConnection* SparqlConnection::instance=nullptr;
constexpr char SparqlConnection::FORMAT[];

#ifdef SPARQL
bool SparqlConnection::importFromFile(SparqlImportDirective& directive) {
	  librdf_world* world;
	  librdf_storage *storage;
	  librdf_model* model = NULL;
	  const char *parser_name;
	  librdf_parser* parser;
	  librdf_query* query;
	  librdf_query_results* results;
	  librdf_uri *uri;
	  const unsigned char *query_string = NULL;
	  const char *format;

	  world = librdf_new_world();
	  librdf_world_open(world);

	  string url=directive.getURL();
	  if(!(Utils::hasStart(url,"https://") || Utils::hasStart(url,"http://"))){
		  url="file:"+url;
	  }

	  uri = librdf_new_uri(world, (const unsigned char*)url.c_str());
	  format = this->FORMAT;
	  if(!strcmp(format, "-"))
	    format = NULL;
	  char *queryChr=strdup(directive.getQuery().c_str());
	  query_string = (const unsigned char*)queryChr;

	  std::string nameStorage="storage"+id;
	  storage = librdf_new_storage(world, "memory", nameStorage.c_str(), NULL);
	  if(storage)
	    model = librdf_new_model(world, storage, NULL);

	  if(!model || !storage) {
		  std::string err="Failed to make model or storage for predicate "+directive.predicateName();
		  ErrorMessage::errorSparqlImport(err);
		  return false;
	  }

	  parser_name = librdf_parser_guess_name2(world, NULL, NULL, librdf_uri_as_string(uri));
	  parser = librdf_new_parser(world, parser_name, NULL, NULL);
	  librdf_parser_parse_into_model(parser, uri, NULL, model);
	  librdf_free_parser(parser);
	  librdf_free_uri(uri);

	  query = librdf_new_query(world, "sparql", NULL, query_string, NULL);

	  results = librdf_model_query_execute(model, query);
	  if(!results) {
		  std::string err="Query of model with "+directive.getQuery()+ " failed";
		  ErrorMessage::errorSparqlImport(err);
	    return false;
	  }

	  result_values values;
	  while(results && !librdf_query_results_finished(results)) {
		  values.emplace_back();
	    for (int i=0; i < librdf_query_results_get_bindings_count(results); i++)
	    {
	    	auto val=librdf_query_results_get_binding_value(results, i);
			const char* str_val = NULL;
			if(val!=nullptr){
				switch (librdf_node_get_type(val)) {
				case LIBRDF_NODE_TYPE_RESOURCE:
					str_val = (const char*)librdf_uri_as_string(librdf_node_get_uri(val));
					break;
				case LIBRDF_NODE_TYPE_LITERAL:
					str_val = (const char*)librdf_node_get_literal_value(val);
					break;
				case LIBRDF_NODE_TYPE_BLANK:
					str_val = (const char*)librdf_node_get_blank_identifier(val);
					break;
				case LIBRDF_NODE_TYPE_UNKNOWN:
				default:
					break;
				}
			}else{
				str_val="";
			}
			values.back().push_back(str_val);
			librdf_free_node(val);
	    }
	    librdf_query_results_next(results);
	  }
	  parseResultAndAddInEDB(values,directive);


	  free(queryChr);

	  librdf_free_query_results(results);
	  librdf_free_query(query);

	  librdf_free_model(model);
	  librdf_free_storage(storage);

	  librdf_free_world(world);

	  return true;
}
#endif

void SparqlConnection::parseResultAndAddInEDB(result_values& values,SparqlImportDirective& directive){
	string predName=directive.predicateName();
	Predicate* pred=new Predicate(predName,directive.predArity());
	PredicateTable::getInstance()->insertPredicate(pred);
	auto predExt=PredicateExtTable::getInstance()->getPredicateExt(pred);
	if(predExt==nullptr){
		PredicateExtTable::getInstance()->addPredicateExt(pred);
		predExt=PredicateExtTable::getInstance()->getPredicateExt(pred);
	}
	auto output=OutputBuilder::getInstance();
	auto& typeConvs=directive.getTypeConv();
	if(typeConvs.size()!=pred->getArity()){
		ErrorMessage::errorSparqlImport(" the number of conversion types must be the same to the predicate "+pred->getName()+" arity.");
	}
	for(auto& row:values){
		vector<Term*> terms;
		unsigned i=0;
                if(row.size()!=typeConvs.size())
                        ErrorMessage::errorSparqlImport(" incorrect arity for "+directive.predicateName()+" predicate");
		for(auto& s:row){
			terms.push_back(getTermFromString(typeConvs[i],s));
			i++;
		}
	    if(terms.size()!=pred->getArity()){
	    	ErrorMessage::errorSparqlImport(" incorrect arity for "+directive.predicateName()+" predicate");
	    }

	    Atom* atom=new ClassicalLiteral(pred,terms,false,false);
            atom->setFact(true);
	    predExt->addAtom(atom,FACT);
	    output->onFact(atom);

	}
}


void SparqlConnection::parseResultAndAddInEDB(char* resultQuery,SparqlImportDirective& directive){
//	regex e("((.?*)\\n)");
//	smatch m;
//	string ss(resultQuery);
//	cout<<"RESULT: "<<resultQuery<<endl;
//	cout<<"MATCH"<<endl;
//	if(regex_search(ss,m,e)) {
//		for(unsigned i=1;i<m.size();i++) {
//			cout<<"M "<<i<< " "<<m[i]<<endl;
//		}
//	}
//
	string predName=directive.predicateName();
	Predicate* pred=new Predicate(predName,directive.predArity());
	PredicateTable::getInstance()->insertPredicate(pred);
	auto predExt=PredicateExtTable::getInstance()->getPredicateExt(pred);
	if(predExt==nullptr){
		PredicateExtTable::getInstance()->addPredicateExt(pred);
		predExt=PredicateExtTable::getInstance()->getPredicateExt(pred);
	}
	auto output=OutputBuilder::getInstance();

	std::stringstream ss(resultQuery);
	std::string to;
	unsigned i=0;
	auto& typeConvs=directive.getTypeConv();
	if (resultQuery != NULL){
		char* pch;
		pch = strtok(resultQuery,"\n");
		while(pch!=NULL){
			//skipt header
			if(i==0){i++;pch = strtok(NULL,"\n");continue;}

			string line(pch);
			line.pop_back();

		    istringstream iss(line);
		    std::string token;
		    auto i=0;
		    vector<Term*> terms;
		    while (std::getline(iss, token, ','))
		    {
		        terms.push_back(getTermFromString(typeConvs[i],token));
		        i++;
		    }

		    if(terms.size()!=pred->getArity()){
		    	ErrorMessage::errorSparqlImport(" incorrect arity for "+directive.predicateName()+" predicate");
		    }
		    Atom* atom=new ClassicalLiteral(pred,terms,false,false);
                    atom->setFact(true);
		    predExt->addAtom(atom,FACT);
		    output->onFact(atom);

			pch = strtok(NULL,"\n");
			i++;
		}
		free(pch);
	}

}


Term* SparqlConnection::getTermFromString(TypeConv type,string& param){
	Term* resultTerm=nullptr;

	if(type == TypeConv::U_INT ||
		type == TypeConv::UT_INT ||
			type == TypeConv::UR_INT ||
				type == TypeConv::T_INT ||
					type == TypeConv::R_INT) {
		long unsigned l_constant = 0;
		long double l_d_constant = 0;

		try {
			l_d_constant = stod(param);
		} catch(...) {
			ErrorMessage::errorGeneric("Invalid type conversion of sparql query result");
		}

		if(type == TypeConv::U_INT)
			l_constant = unsigned(l_d_constant);
		else if(type == TypeConv::UT_INT)
			l_constant = unsigned(trunc(l_d_constant));
		else if(type == TypeConv::UR_INT)
			l_constant = unsigned(round(l_d_constant));
		else if(type == TypeConv::T_INT)
			l_constant = (trunc(l_d_constant));
		else if(type == TypeConv::R_INT)
			l_constant = (round(l_d_constant));
		else
			ErrorMessage::errorGeneric("Invalid type conversion of sparql query result");
		resultTerm = new NumericConstantTerm(l_constant<0, l_constant);
	}
	else if(type == TypeConv::_CONST ||
				type == TypeConv::Q_CONST) {
		if(type == TypeConv::Q_CONST)
				resultTerm = new StringConstantTerm(false, "\""+param+"\"");
		else
				resultTerm = new StringConstantTerm(false, param);
	}
	tb->addTerm(resultTerm);
	return resultTerm;
}


} /* namespace grounder */
} /* namespace DLV2 */
