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
 * ExtAtomFactory.h
 *
 *  Created on: 22 lug 2016
 *      Author: giovanni
 */

#ifndef EXTATOMFACTORY_H_
#define EXTATOMFACTORY_H_

#include "../grounder/atom/ExtAtom.h"
#include <unordered_map>
#include <cstdio>
#ifdef PYTHON
#include <Python.h>
#else
#include "../../common/ErrorMessage.h"
#endif

using namespace std;

namespace DLV2 {
namespace grounder {
class ExtAtomFactory {
	public:
		virtual ~ExtAtomFactory(){
		}
		;

		static ExtAtomFactory* getInstance(){
			if( factory == nullptr )
				factory = new ExtAtomFactory();
			return factory;
		}

		static void freeInstance(){
			if( factory != nullptr ){
				delete factory;
			}
		}

		virtual ExtAtom* makeExtAtom( Predicate* predicate, vector< Term* >& terms_parsered, unsigned extSeparator, bool naf );

		virtual void splitPythonPath( const string& path ){
		}

		virtual bool addTypeConvOfExtAtom( string& name, vector< TypeConv >& types ){
			return true;
		}
	protected:
		static ExtAtomFactory* factory;
		ExtAtomFactory(){
		}
		;
		static constexpr char SUM_ATOM[] = "sum";
		static constexpr char INT_ATOM[] = "int";
		static constexpr char MOD_ATOM[] = "mod";
		static constexpr char RAND_ATOM[] = "rand";
		static constexpr char APPEND_ATOM_STR[] = "append_str";
		static constexpr char LENGTH_ATOM_STR[] = "length_str";
		static constexpr char REVERSE_ATOM_STR[] = "reverse_str";
		static constexpr char ABS_ATOM[] = "abs";
		static constexpr char SUB_STR_ATOM[] = "sub_str";
		static constexpr char MEMBER_ATOM_STR[] = "member_str";
		static constexpr char TO_QSTR_ATOM[] = "to_qstr";
		static constexpr char APPEND_ATOM[] = "append";
		static constexpr char REVERSE_RECURSIVE_ATOM[] = "reverse_r";
		static constexpr char SELECT_RECURSIVE_ATOM[] = "select_r";
		static constexpr char DELETE_RECURSIVE_ATOM[] = "delete_r";
		static constexpr char LENGTH_ATOM[] = "length";
		static constexpr char DELNTH_ATOM[] = "delNth";
		static constexpr char MEMBER_ATOM[] = "member";
		static constexpr char MEMBER_NTH_ATOM[] = "memberNth";
		static constexpr char LAST_ATOM[] = "last";
		static constexpr char HEAD_ATOM[] = "head";
		static constexpr char TAIL_ATOM[] = "tail";
		static constexpr char INS_NTH_ATOM[] = "insNth";
		static constexpr char INS_LAST_ATOM[] = "insLast";
		static constexpr char SUB_LIST_ATOM[] = "subList";
		static constexpr char FLATTEN_ATOM[] = "flatten";
		static constexpr char REVERSE_ATOM[] = "reverse";
		static constexpr char DELETE_ATOM[] = "delete";
		static constexpr char SELECT_ATOM[] = "select";
};
#ifdef PYTHON
class PythonExtAtomFactory: public ExtAtomFactory{
	public:
	~PythonExtAtomFactory(){};

	static ExtAtomFactory* getInstance(){
		if(ExtAtomFactory::factory == nullptr)
		ExtAtomFactory::factory = new PythonExtAtomFactory();
		return ExtAtomFactory::factory;
	}

	bool addTypeConvOfExtAtom(string& name,vector<TypeConv>& types){
		if(extTypeConv.count(name))
		return false;
		extTypeConv.insert({name,types});
		return true;
	}

	PyObject* importPyFunctionFromString(string moduleName,string pythonScript){
		PyObject* newmodule = PyImport_AddModule("__main__");
		PyObject* dict = PyModule_GetDict(newmodule);
		PyRun_String(pythonScript.c_str(), Py_file_input, dict, dict);
		return newmodule;
	}

	static void freeInstance(){if(factory!=nullptr){((PythonExtAtomFactory*)factory)->pyFinalize(); delete factory;}}

	ExtAtom* makeExtAtom(Predicate* predicate,vector<Term*>& terms_parsered, unsigned extSeparator, bool naf);
	void splitPythonPath(const string& path);

	private:
	PythonExtAtomFactory();

	void appendSysDirectory(unordered_set<string>& path);
	string getCurrentDirectory();
	PyObject* importModule(const string& script);
	PyObject* importFunc(const string& name, PyObject* module);

	void pyInitialize();
	void pyFinalize();
	bool pyRunning;
	bool sparqlModuling;
	bool sparqlLoadFunc;
	bool sparqlLoadEndFunc;
	PyObject* sparqlModule;
	PyObject* sparqlEndpointFunc;
	PyObject* sparqlFunc;

	///Map of script -> loaded python module
	unordered_map<string,PyObject*> scriptName;
	///Map of python function name -> loaded python function
	unordered_map<string,PyObject*> scriptNameFunction;
	///Map of script -> path of the script
	unordered_map<string,string> scriptPath;
	///Map of script -> number of param. input
	unordered_map<string,unsigned> scriptInput;
	///Set of info message given
	unordered_set<string> infoMexGiven;
	const string SPARQL_SCRIPT;
	const string SPARQL_SCRIPT_PATH;
	static ExtAtomFactory* factory;

	unordered_map<string,vector<TypeConv>> extTypeConv;

	static constexpr char SPARQL_FUNC[]="sparql";
	static constexpr char SPARQLENDPOINT_FUNC[]="sparqlEndpoint";
	static constexpr char SPARQLENDPOINT_PYTHON_FUNC[]="import rdflib\nfrom SPARQLWrapper import SPARQLWrapper, JSON\ndef sparqlEndpoint(dataset, query):\n\tsparql = SPARQLWrapper(dataset)\n\tsparql.setQuery(query)\n\tsparql.setReturnFormat(JSON)\n\tresults = sparql.query().convert()\n\tl = []\n\tfor result in results[\"results\"][\"bindings\"]:\n\t\ttup = ()\n\t\tfor sel in sorted(result):\n\t\t\ttup= tup+(result[sel][\"value\"],) \n\t\tl.append(tup)\n\treturn l\n";
	static constexpr char SPARQLENDPOINT_TEMP[]="sparqlEndpoint_idlv";
};

#endif

}
/* namespace grounder */
} /* namespace DLV2 */

#endif /* EXTATOMFACTORY_H_ */
