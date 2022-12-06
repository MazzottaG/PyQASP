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
 * ExtAtomFactory.cpp
 *
 *  Created on: 22 lug 2016
 *      Author: giovanni
 */

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#define FILENAME_MAX MAX_PATH
#else
#include <unistd.h>
#include <limits.h>
#define GetCurrentDir getcwd
#define FILENAME_MAX PATH_MAX
#endif

#include "ExtAtomFactory.h"
#include "../grounder/atom/PythonExtAtom.h"
#include "../grounder/atom/externalAtom/SumExtAtom.h"
#include "../grounder/atom/externalAtom/IntExtAtom.h"
#include "../grounder/atom/externalAtom/ModExtAtom.h"
#include "../grounder/atom/externalAtom/RandExtAtom.h"
#include "../grounder/atom/externalAtom/AppendExtAtom.h"
#include "../grounder/atom/externalAtom/LengthExtAtomUTF16.h"
#include "../grounder/atom/externalAtom/LengthExtAtom.h"
#include "../grounder/atom/externalAtom/Length32ExtAtom.h"
#include "../grounder/atom/externalAtom/ReverseExtAtom.h"
#include "../grounder/atom/externalAtom/AbsExtAtom.h"
#include "../grounder/atom/externalAtom/SubStrExtAtom.h"
#include "../grounder/atom/externalAtom/SubStrExtAtomUTF16.h"
#include "../grounder/atom/externalAtom/SubStrExtAtomUTF32.h"
#include "../grounder/atom/externalAtom/MemberExtAtom.h"
#include "../grounder/atom/externalAtom/ToQuotedStrExtAtom.h"
#include "../grounder/exception/ExtAtomException.h"
#include "../grounder/table/PredicateTable.h"
#include "../grounder/atom/externalAtom/AppendListExtAtom.h"
#include "../grounder/atom/externalAtom/ReverseListExtAtom.h"
#include "../grounder/atom/externalAtom/SelectListExtAtom.h"
#include "../grounder/atom/externalAtom/LengthListExtAtom.h"
#include "../grounder/atom/externalAtom/DelNthListExtAtom.h"
#include "../grounder/atom/externalAtom/MemberListExtAtom.h"
#include "../grounder/atom/externalAtom/MemberNthListExtAtom.h"
#include "../grounder/atom/externalAtom/LastListExtAtom.h"
#include "../grounder/atom/externalAtom/HeadListExtAtom.h"
#include "../grounder/atom/externalAtom/TailListExtAtom.h"
#include "../grounder/atom/externalAtom/InsNthListExtAtom.h"
#include "../grounder/atom/externalAtom/InsLastListExtAtom.h"
#include "../grounder/atom/externalAtom/SubListExtAtom.h"
#include "../grounder/atom/externalAtom/FlattenListExtAtom.h"
#include "../grounder/atom/externalAtom/SimpleReverseListExtAtom.h"
#include "../grounder/atom/externalAtom/SimpleSelectListExtAtom.h"

#include "../../common/Options.h"
#include <regex>
#include <fstream>
using namespace std;
namespace DLV2 {
namespace grounder {

ExtAtomFactory* ExtAtomFactory::factory = nullptr;
constexpr char ExtAtomFactory::SUM_ATOM[];
constexpr char ExtAtomFactory::INT_ATOM[];
constexpr char ExtAtomFactory::MOD_ATOM[];
constexpr char ExtAtomFactory::RAND_ATOM[];
constexpr char ExtAtomFactory::APPEND_ATOM_STR[];
constexpr char ExtAtomFactory::LENGTH_ATOM_STR[];
constexpr char ExtAtomFactory::REVERSE_ATOM_STR[];
constexpr char ExtAtomFactory::ABS_ATOM[];
constexpr char ExtAtomFactory::SUB_STR_ATOM[];
constexpr char ExtAtomFactory::MEMBER_ATOM_STR[];
constexpr char ExtAtomFactory::TO_QSTR_ATOM[];
constexpr char ExtAtomFactory::APPEND_ATOM[];
constexpr char ExtAtomFactory::REVERSE_RECURSIVE_ATOM[];
constexpr char ExtAtomFactory::SELECT_RECURSIVE_ATOM[];
constexpr char ExtAtomFactory::DELETE_RECURSIVE_ATOM[];
constexpr char ExtAtomFactory::LENGTH_ATOM[];
constexpr char ExtAtomFactory::DELNTH_ATOM[];
constexpr char ExtAtomFactory::MEMBER_ATOM[];
constexpr char ExtAtomFactory::MEMBER_NTH_ATOM[];
constexpr char ExtAtomFactory::LAST_ATOM[];
constexpr char ExtAtomFactory::HEAD_ATOM[];
constexpr char ExtAtomFactory::TAIL_ATOM[];
constexpr char ExtAtomFactory::INS_NTH_ATOM[];
constexpr char ExtAtomFactory::INS_LAST_ATOM[];
constexpr char ExtAtomFactory::SUB_LIST_ATOM[];
constexpr char ExtAtomFactory::FLATTEN_ATOM[];
constexpr char ExtAtomFactory::REVERSE_ATOM[];
constexpr char ExtAtomFactory::DELETE_ATOM[];
constexpr char ExtAtomFactory::SELECT_ATOM[];

ExtAtom* ExtAtomFactory::makeExtAtom( Predicate* predicate, vector< Term* >& terms, unsigned separator, bool naf ){

	unsigned sizeInputTems = separator;
	unsigned sizeOutputTerms = terms.size() - separator;
	string atom = predicate->getName();
	bool error = false;
	if( sizeOutputTerms > 1 )
		error = true;
	else{
		if( predicate->getName() == SUM_ATOM ){
			if( sizeInputTems == SumExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new SumExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == INT_ATOM ){
			if( sizeInputTems == IntExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new IntExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == MOD_ATOM ){
			if( sizeInputTems == ModExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new ModExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == RAND_ATOM ){
			if( sizeInputTems == RandExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new RandExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == APPEND_ATOM_STR ){
			if( sizeInputTems == AppendExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new AppendExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == LENGTH_ATOM_STR ){
			if( sizeInputTems == LengthStringExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 ){
				if( Options::globalOptions()->noStringConversion() )
					return new LengthExtAtom( predicate, terms, separator, naf );
				return new LengthExtAtomUTF16( predicate, terms, separator, naf );
			}
			error = true;
		} else if( predicate->getName() == REVERSE_ATOM_STR ){
			if( sizeInputTems == ReverseExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new ReverseExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == ABS_ATOM ){
			if( sizeInputTems == AbsExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new AbsExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == SUB_STR_ATOM ){
			if( sizeInputTems == BaseSubStrExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 ){
				if( Options::globalOptions()->noStringConversion() )
					return new SubStrExtAtom( predicate, terms, separator, naf );
				return new SubStrExtAtomUTF16( predicate, terms, separator, naf );
			}
			error = true;
		} else if( predicate->getName() == MEMBER_ATOM_STR ){
			if( sizeInputTems == MemberExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 0 )
				return new MemberExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == TO_QSTR_ATOM ){
			if( sizeInputTems == ToQuotedStrExt::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new ToQuotedStrExt( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == APPEND_ATOM ){
			if( sizeInputTems == AppendListExtAtom::getNumberOfInputTerms() )
				return new AppendListExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == REVERSE_RECURSIVE_ATOM ){
			if( sizeInputTems == ReverseListExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new ReverseListExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == SELECT_RECURSIVE_ATOM ){
			if( sizeInputTems == SelectListExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new SelectListExtAtom( predicate, terms, separator, naf, false );
			error = true;
		} else if( predicate->getName() == DELETE_RECURSIVE_ATOM ){
			if( sizeInputTems == SelectListExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new SelectListExtAtom( predicate, terms, separator, naf, true );
			error = true;
		} else if( predicate->getName() == LENGTH_ATOM ){
			if( sizeInputTems == LengthListExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new LengthListExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == DELNTH_ATOM ){
			if( sizeInputTems == DelNthListExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new DelNthListExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == MEMBER_ATOM ){
			if( sizeInputTems == MemberListExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 0 )
				return new MemberListExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == MEMBER_NTH_ATOM ){
			if( sizeInputTems == MemberNthListExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new MemberNthListExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == LAST_ATOM ){
			if( sizeInputTems == LastListExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new LastListExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == HEAD_ATOM ){
			if( sizeInputTems == HeadListExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new HeadListExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == TAIL_ATOM ){
			if( sizeInputTems == TailListExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new TailListExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == INS_NTH_ATOM ){
			if( sizeInputTems == InsNthListExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new InsNthListExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == INS_LAST_ATOM ){
			if( sizeInputTems == InsLastListExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new InsLastListExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == SUB_LIST_ATOM ){
			if( sizeInputTems == SubListExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 0 )
				return new SubListExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == FLATTEN_ATOM ){
			if( sizeInputTems == FlattenListExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new FlattenListExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == REVERSE_ATOM ){
			if( sizeInputTems == SimpleReverseListExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new SimpleReverseListExtAtom( predicate, terms, separator, naf );
			error = true;
		} else if( predicate->getName() == DELETE_ATOM ){
			if( sizeInputTems == SimpleSelectListExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new SimpleSelectListExtAtom( predicate, terms, separator, naf, true );
			error = true;
		} else if( predicate->getName() == SELECT_ATOM ){
			if( sizeInputTems == SimpleSelectListExtAtom::getNumberOfInputTerms() && sizeOutputTerms == 1 )
				return new SimpleSelectListExtAtom( predicate, terms, separator, naf, false );
			error = true;
		}
	}
	if( error ){
#ifdef PYTHON
		return nullptr;
#else
		throw ExtAtomException( "ERROR: Invalid number of term in External atom &" + atom );
#endif
	}
	return nullptr;

}
#ifdef PYTHON
constexpr char PythonExtAtomFactory::SPARQL_FUNC[];
constexpr char PythonExtAtomFactory::SPARQLENDPOINT_FUNC[];
constexpr char PythonExtAtomFactory::SPARQLENDPOINT_PYTHON_FUNC[];
constexpr char PythonExtAtomFactory::SPARQLENDPOINT_TEMP[];

PythonExtAtomFactory::PythonExtAtomFactory() :
pyRunning(false), sparqlModuling(false), sparqlLoadFunc(false), sparqlLoadEndFunc(
		false), sparqlModule(nullptr), sparqlEndpointFunc(nullptr), sparqlFunc(
		nullptr), SPARQL_SCRIPT("py_sparql_import"), SPARQL_SCRIPT_PATH(
		"scripts/python"){
}

ExtAtom* PythonExtAtomFactory::makeExtAtom(Predicate* predicate,
		vector<Term*>& terms, unsigned separator, bool naf){

	ExtAtom* extAtom = nullptr;
	if(Options::globalOptions()->useDefaultExternalAtoms())
	extAtom = ExtAtomFactory::makeExtAtom(predicate, terms, separator, naf);
	if (extAtom != nullptr)
	return extAtom;
	else{
		if (!pyRunning)
		pyInitialize();

		//Check if exist other predicate with different arity

		PyObject* pFunc = nullptr;

		//Load module for sparql query
		if (!sparqlModuling && predicate->getName() == SPARQLENDPOINT_FUNC){

			sparqlModuling = true;
			sparqlModule = importPyFunctionFromString(SPARQLENDPOINT_TEMP,
					SPARQLENDPOINT_PYTHON_FUNC);
		}

		if (predicate->getName() == SPARQLENDPOINT_FUNC){
			//load sparqlEndpoint function for query an Endpoint sparql
			if (!sparqlLoadEndFunc){
				sparqlLoadEndFunc = true;
				sparqlEndpointFunc = importFunc(predicate->getName(),
						sparqlModule);
				if (sparqlEndpointFunc == nullptr){
					throw ExtAtomException(
							" cannot load sparqlEndpoint function.");
				}
			}

			pFunc = sparqlEndpointFunc;
		} else{
			//Load a function in a script specified by the user

			// if --python-file is not specified
			if (scriptPath.size() == 0){
				extAtom = ExtAtomFactory::makeExtAtom( predicate, terms, separator, naf );
				if( extAtom != nullptr ){
					if(Options::globalOptions()->isEnableExternalAtomsWarnings())
						cerr << "WARNING: No python file specified. A default external atom has been used." << endl;
					return extAtom;
				}
				pyFinalize();
				throw ExtAtomException(
						" cannot load python scripts.  You have not specified any python file.");
			}
			if (scriptNameFunction.count(predicate->getName())){
				pFunc = scriptNameFunction[predicate->getName()];
				//todo check number of input terms
			} else{
				//Find the function in scripts
				for (auto it : scriptName){
					auto script = it.first;
					auto& module = it.second;
					if (module == nullptr)
					module = importModule(script);
					pFunc = importFunc(predicate->getName(), module);
					if (pFunc != nullptr){
						scriptNameFunction[predicate->getName()] = pFunc;
						break;
					}
				}
			}
			if (pFunc == nullptr){
				extAtom = ExtAtomFactory::makeExtAtom(predicate, terms, separator, naf);

				if(extAtom==nullptr){
					pyFinalize();
					if (scriptName.size() == 1)
					throw ExtAtomException(
							" cannot load function. There is no function named \""
							+ predicate->getName()
							+ "\" in the python script \""
							+ (*scriptName.begin()).first + "\".");
					string files = "";
					for (auto p : scriptName)
					files += p.first + " ";
					throw ExtAtomException(
							" cannot load function. There is no function named \""
							+ predicate->getName()
							+ "\" in the python scripts \"" + files
							+ "\".");
				}
				else{
					string name=predicate->getName();
					if(!infoMexGiven.count(name)){
						cerr<<"--> Info: In the input python file(s) there is no function named "
						+ name
						+ + ". The default external atom &"+name+" has been used instead."<<endl;
						infoMexGiven.insert(name);
					}
					return extAtom;
				}
			}
		}

		string name = predicate->getName();
		for (unsigned i = separator; i < terms.size(); i++){
			if (terms[i]->getType() == FUNCTION
					|| terms[i]->getType() == ARITH){
				extAtom = ExtAtomFactory::makeExtAtom(predicate, terms, separator, naf);
				if(extAtom==nullptr)
				{
					throw ExtAtomException(
							" Invalid type term of &" + name
							+ ": output terms only accept variable, numeric, string or symbolic constant.");
				}
				else{
					string name=predicate->getName();
					if(!infoMexGiven.count(name)){
						cerr<<"--> Info: In the input python file(s) there is no function named "
						+ name
						+ + ". The default external atom &"+name+" has been used instead."<<endl;
						infoMexGiven.insert(name);
					}
					return extAtom;
				}
			}
		}

		extAtom = new PythonExtAtom(predicate, pFunc, terms, separator, naf);
		if (extTypeConv.count(name)){
			extAtom->setTypeConvs(extTypeConv[name]);
		}

		if (scriptInput.count(name)){
			if (scriptInput[name] != separator)
			cerr
			<< "WARNING: " + name
			+ ", first used with input terms arity "
			+ to_string(scriptInput[name])
			+ ", now seen with input terms "
			+ to_string(separator) + "." << endl;
		} else
		scriptInput[name] = separator;
	}
	return extAtom;
}

PyObject* PythonExtAtomFactory::importFunc(const string& name,
		PyObject* module){
	if (!PyObject_HasAttrString(module, name.c_str()))
	return nullptr;
	PyObject* pFunc = PyObject_GetAttrString(module, name.c_str());
	if (!pFunc || !PyCallable_Check(pFunc))
	return nullptr;
	return pFunc;
}

PyObject* PythonExtAtomFactory::importModule(const string& script){
	PyObject* pName = PyUnicode_FromString(script.c_str());
	PyObject* module;
	module = PyImport_Import(pName);
	Py_DECREF(pName);
	if (module == nullptr){
		if (PyErr_Occurred()){
			PyErr_Print();
		}
		pyFinalize();
		throw ExtAtomException(
				" during import of module " + script + ". Cannot load script.");
	}

	return module;
}

#ifdef _WIN32
int setenv(const char *name, const char *value, int overwrite)
{
    int errcode = 0;
    if(!overwrite) {
        size_t envsize = 0;
        errcode = _wgetenv_s(&envsize, NULL, 0, reinterpret_cast<const wchar_t *>(name));
        if(errcode || envsize) return errcode;
    }
    return _putenv_s(name, value);
}
#endif

void PythonExtAtomFactory::pyInitialize(){
	setenv("PYTHONDONTWRITEBYTECODE", " ", 1);

	pyRunning = true;
	Py_Initialize();

	unordered_set<string> paths;
	for (auto p : scriptPath)
	paths.insert(p.second);
	appendSysDirectory(paths);
}

void PythonExtAtomFactory::pyFinalize(){
	if (pyRunning){
		Py_XDECREF(sparqlFunc);
		Py_XDECREF(sparqlModule);
		for (auto p : scriptName)
		Py_XDECREF(p.second);
		for (auto p : scriptNameFunction)
		Py_XDECREF(p.second);
		Py_Finalize();
		pyRunning = false;
	}
}

void PythonExtAtomFactory::splitPythonPath(const string& path){
	size_t i = path.find_last_of("/\\");
	// if path is not an absolute path, scriptPath = CURR_DIR
	string paths, script;
	if (i == path.npos)
	paths = getCurrentDirectory();
	else
	paths = path.substr(0, i);
	script = path.substr(i + 1);
	// erase the extension ".py" in scriptName if it exists
	regex e("(.*?).py");
	if (regex_match(script, e))
	script.erase(script.find(".py"));
	scriptName[script] = nullptr;
	scriptPath[script] = paths;
}

string PythonExtAtomFactory::getCurrentDirectory(){
	char cCurrentPath[FILENAME_MAX];
	if (GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)) != 0)
	return cCurrentPath;
	throw ExtAtomException("Cannot retrieve current working directory.");
	return "";
}

void PythonExtAtomFactory::appendSysDirectory(unordered_set<string>& path){
	PyObject *sysPath = PySys_GetObject((char*) "path");
	for (auto p : path)
	PyList_Insert(sysPath, 0, PyUnicode_FromString(p.c_str()));
}

#endif

}
/* namespace grounder */
} /* namespace DLV2 */
