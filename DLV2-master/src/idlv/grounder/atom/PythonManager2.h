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
 * PythonManager.h
 *
 */

#ifndef PYTHONMANAGER_H_
#define PYTHONMANAGER_H_

#ifdef PYTHON
#include <Python.h>
#else
using PyObject=unsigned;
#endif

#include "../table/TermTable.h"
#include "../../util/Utils.h"
#include "../exception/ExtAtomException.h"
#include <regex>

namespace DLV2 {
namespace grounder {

/*
 * This class is used by each ext atom and performs the communication
 * between the ext atom and the python interpreter in order to call the
 * attached python function and help the PythonExtAtom class to perform
 * the operation of match in an orderly manner.
 */
#ifdef PYTHON
class PythonManager {
public:
	using vec_term = vector<Term*>;
	PythonManager(unsigned separator, const string& name, PyObject* f):
		separator(separator), outTerm(0),name(name), result(nullptr), sequence(nullptr), func(f), index(0), nRows(1),tt(TermTable::getInstance()),resultIsASequence(false){}

	virtual ~PythonManager() { }


	// Execute the python function and stores the result in the PyObject result
	void callPythonFunc(vec_term& terms, var_assignment& assignment);

	///Return the first output of the python function.  output is the translated pyobject in terms and res is the result
	/// of the function if it return a boolean
	bool getNextValueFirst(vec_term& output,bool& res);

	///Return the next output of the python function.The parameter is similar to getNextValueFirst. The function return
	///true if the output of the function ended false otherwise
	bool getNextValueNext(vec_term& output,bool& res);

	/*
	 * Clear the current index and the data-structures used for matching in order
	 * to prepare the manager to perform a new first match
	 */
	void clearObjects();

	// Set the vector of convertion type
	void setTypeConvs(vector<TypeConv>& typeConvs) {
		this->typeConvs = typeConvs;
	}

	vector<TypeConv>& getTypeConvs() {
		return this->typeConvs ;
	}

	PyObject* getPyFunc(){
		return func;
	}

	bool isResultASequence(){
		return resultIsASequence;
	}

private:
	// The position of the possible semicolon-separator of the ext atom
	unsigned separator;

	/// Number of output terms
	unsigned outTerm;

	// The name of the ext atom to which is connected
	string name;

	// The result of the call to python function
	PyObject* result;

	// The sequence-pyObject extracted by result with PySequence_Fast(result, "expected a sequence");
	PyObject* sequence;

	// The python function connected to the ext atom
	PyObject* func;

	/*
	 * Indicates the current rows of PyObject result
	 * that has to be mapped in the var_assignement during nextMatch function
	 */
	unsigned index;

	// Indicates the possible number of rows of the result (def = 1)
	unsigned nRows;

	// Vector that specifies the convertion type of output terms
	vector<TypeConv> typeConvs;

	TermTable* tt;

	bool resultIsASequence;

	///Return true if the object is a sequence (list or tuple)
	static bool isSequence(PyObject* obj){
		PyObject* pytype=PyObject_Type(obj);
		PyObject* pyrepr=PyObject_Repr(pytype);
		string resultType;
		#ifdef PYTHON_THREE
			resultType = PyUnicode_AsUTF8(pyrepr);
		#else
			resultType = PyString_AsString(pyrepr);
		#endif
		Py_XDECREF(pytype);
		Py_XDECREF(pyrepr);
		return (((PySequence_Check(obj) && resultType!=PY_STR)) ||
				resultType==PY_RDF);
	}

	///Return the ith element of the python sequence
	Term* getValueFromSequence(PyObject* obj,unsigned index){
		PyObject* el = PySequence_Fast_GET_ITEM(obj, index);
		Term* r= getValue(el,index);
//		Py_XDECREF(el);
		return r;
	}

	///Return true if the string start with lowercase and contains only alphabetic character and numbers (is an ID ASP-CORE2)
	static bool checkIfIsID(string& s){
		if(s[0]<'a' || s[0]>'z')return false;
		for(auto &c:s){
			if(!((c>='0' && c<='9') || (c>='a' && c<='z') || (c>='A' && c<='Z') || (c=='_')))
				return false;
		}
		return true;
	}

	static string getType(PyObject* obj){
		PyObject* obj1=PyObject_Type(obj);
		PyObject* obj2=PyObject_Repr(obj1);
		string s;
		#ifdef PYTHON_THREE
			s = PyUnicode_AsUTF8(obj2);
		#else
			s = PyString_AsString(obj2);
		#endif
		Py_XDECREF(obj2);
		Py_XDECREF(obj1);
		return s;
//		return PyUnicode_AsUTF8(PyObject_Repr(PyObject_Type(obj)));
	}

	static string getStringToPy(PyObject* obj){
		PyObject* obj1	= PyObject_Str(obj);
		string s;
		#ifdef PYTHON_THREE
			s = PyUnicode_AsUTF8(obj1);
		#else
			s = PyString_AsString(obj1);
		#endif
		Py_XDECREF(obj1);
		return s;
//		return PyUnicode_AsUTF8(PyObject_Str(obj));
	}

	///Translate the pyobject in Term
	Term* getValue(PyObject* obj,unsigned pos=0);


	void trowPyError(string s){
		throw ExtAtomException(s);
	};



public:
	static constexpr char PY_RDF[]="<class \'rdflib.plugins.sparql.processor.SPARQLResult\'>";
#ifdef PYTHON_THREE
	static constexpr char PY_STR[]="<class \'str\'>";
	static constexpr char PY_INT[]="<class \'int\'>";
	static constexpr char PY_NONE[]="<class \'NoneType\'>";
	static constexpr char PY_BOOL[]="<class \'bool\'>";
#else
	static constexpr char PY_STR[]="<type \'str\'>";
	static constexpr char PY_INT[]="<type \'int\'>";
	static constexpr char PY_NONE[]="<type \'NoneType\'>";
	static constexpr char PY_BOOL[]="<type \'bool\'>";
#endif
};
#else
class PythonManager {
public:
	using vec_term = vector<Term*>;
	PythonManager(unsigned separator, const string& name, PyObject* f){}

	virtual ~PythonManager() { }


	// Execute the python function and stores the result in the PyObject result
	void callPythonFunc(vec_term& terms, var_assignment& assignment){

	}

	///Return the first output of the python function.  output is the translated pyobject in terms and res is the result
	/// of the function if it return a boolean
	bool getNextValueFirst(vec_term& output,bool& res){
		return false;
	}

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wreturn-type"
	vector<TypeConv>& getTypeConvs() {
	}
	#pragma GCC diagnostic pop

	///Return the next output of the python function.The parameter is similar to getNextValueFirst. The function return
	///true if the output of the function ended false otherwise
	bool getNextValueNext(vec_term& output,bool& res){
		return false;
	}

	void clearObjects(){

	}

	// Set the vector of convertion type
	void setTypeConvs(vector<TypeConv>& typeConvs) {

	}

	PyObject* getPyFunc(){
		return nullptr;
	}

	bool isResultASequence(){
		return false;
	}

};
#endif

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* PYTHONMANAGER_H_ */
