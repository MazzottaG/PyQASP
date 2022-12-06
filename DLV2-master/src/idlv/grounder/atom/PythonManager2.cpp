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
 * PythonManager2.cpp
 *
 *  Created on: 04 ago 2016
 *      Author: giovanni
 */

#include "PythonManager2.h"

#include "../../../common/Trace.h"
#include "../../util/WarningMessage.h"
#include <math.h>
#include <ctype.h>

namespace DLV2 {
namespace grounder {


#ifdef PYTHON
constexpr char PythonManager::PY_RDF[];
constexpr char PythonManager::PY_STR[];
constexpr char PythonManager::PY_INT[];
constexpr char PythonManager::PY_NONE[];
constexpr char PythonManager::PY_BOOL[];

void PythonManager::callPythonFunc(vec_term& terms, var_assignment& assignment) {
	outTerm=terms.size()-separator;
	if(typeConvs.size() && (typeConvs.size()!=outTerm)){
		throw ExtAtomException("Invalid conversion type for &"+name+". The number of output values are "+to_string(outTerm)+" but the conversion types are "+to_string(typeConvs.size()));
	}
	PyObject* pArgs = PyTuple_New(separator);
	for (unsigned i=0; i<separator; i++) {
		Term* term = terms[i]->substitute(assignment);
		PyObject* pTerm = nullptr;
		if(term->getType() == TermType::NUMERIC_CONSTANT)
			pTerm = Py_BuildValue("i", term->getConstantValue());
		else if(term->getType() == TermType::SYMBOLIC_CONSTANT)
			pTerm = Py_BuildValue("s", term->getName().c_str());
		else if(term->getType() == TermType::STRING_CONSTANT) {
			string constant = term->getName().substr(1, term->getName().size() - 2);
			pTerm = Py_BuildValue("s", constant.c_str());
		}
		else {
			//cout<<term->getType()<<endl;
			Py_DECREF(pArgs);
			throw ExtAtomException("Invalid type term: input terms only accept numeric, string or symbolic constant.");
		}
//		cout<<"input "<<terms[i]->getName()<<":"<<term->getName()<<endl;
		PyTuple_SetItem(pArgs, i, pTerm);
	}

	result = PyObject_CallObject(func, pArgs);
	if(PyErr_Occurred()) {
		Py_DECREF(pArgs);
		PyErr_Print();
		throw ExtAtomException();
	}
	Py_DECREF(pArgs);
}




Term* PythonManager::getValue(PyObject* var,unsigned pos){
	Term* resultTerm = nullptr;

	if(typeConvs.size() == 0) {

		string sType = getType(var);
		if(sType == PY_INT) {
			int l_constant = PyLong_AsLong(var);
			resultTerm = new NumericConstantTerm(l_constant<0, l_constant);
		} else {
			string s_constant = getStringToPy(var);
			if(checkIfIsID(s_constant))
				resultTerm = new StringConstantTerm(false, s_constant);
			else
				resultTerm = new StringConstantTerm(false, "\""+s_constant+"\"");
		}
	} else {
		TypeConv type = typeConvs[pos];
		if(type == TypeConv::U_INT ||
				type == TypeConv::UT_INT ||
					type == TypeConv::UR_INT || type == TypeConv::T_INT || type == TypeConv::R_INT)  {
			long unsigned l_constant = 0;
			long double l_d_constant = 0;



			try {
				l_d_constant = stod(getStringToPy(var));
			} catch(...) {
				throw ExtAtomException("Invalid conversion of &"+name+" outputs: "+Utils::toTypeConv(type)+" not valid. The output is not a number.");
			}

			if(type == TypeConv::U_INT)
				l_constant = l_d_constant;
			else if(type == TypeConv::UT_INT)
				l_constant = unsigned(trunc(l_d_constant));
			else if(type == TypeConv::UR_INT)
				l_constant = unsigned(round(l_d_constant));
			else if(type == TypeConv::T_INT)
				l_constant = trunc(l_d_constant);
			else if(type == TypeConv::R_INT)
				l_constant = round(l_d_constant);

			resultTerm = new NumericConstantTerm(l_constant<0, l_constant);
		}
		else if(type == TypeConv::_CONST ||
					type == TypeConv::Q_CONST) {
			string s_constant = "";

			try {
				s_constant = getStringToPy(var);
			} catch(...) {
				throw ExtAtomException("Invalid conversion of &"+name+" outputs: "+Utils::toTypeConv(type)+" not valid. The output can not be converted to string.");
			}

			if(type == TypeConv::Q_CONST)
				resultTerm = new StringConstantTerm(false, "\""+s_constant+"\"");
			else
					resultTerm = new StringConstantTerm(false, s_constant);
		}
	}

	tt->addTerm(resultTerm);
	return resultTerm;
}


bool PythonManager::getNextValueFirst(vec_term& output,bool& res) {

	res=false;
	//If no output variable check if the function return true or false
	if(outTerm==0){
		nRows=0;
		string sType = getType(result);
		if(sType == PY_NONE)
			res=true;
		if(sType != PY_BOOL){
			Py_DecRef(result);
			trowPyError("Error call external atom &"+name+". The atom haven't output parameter but the python function return non boolean value");
		}
		res = PyLong_AsLong(result);
		return false;
	}

	//If the result is not a sequence then the function return one output
	if(!isSequence(result)){
		nRows=1;
		if(outTerm!=1){
			Py_DecRef(result);
			trowPyError("Error call external atom &"+name+". The python function return one value but the external atom have more than one output parameters.");
		}
		output.push_back(getValue(result,0));
		index++;
		return false;
	}
	//A result is a sequence of obj
	resultIsASequence=true;
	Py_XDECREF(sequence);
	sequence = PySequence_Fast(result, "expected a sequence");
	nRows = PySequence_Size(result);

	if(index>=nRows)
		return true;

	//Take the first element if sequence and check if it is another sequence. If is a value result
	//is a list of value. Otherwise result is a list of tuple
	PyObject* elem = PySequence_Fast_GET_ITEM(sequence, index);
	if(!isSequence(elem)){
		output.push_back(getValue(elem,0));
		index++;
		if(outTerm==1)return false;
		if(outTerm!=nRows){
			Py_DecRef(result);
			Py_DecRef(sequence);
			Py_DecRef(elem);
			trowPyError("Error call external atom &"+name+": number of output values of python function not match with the output parameter of the external atom");
		}
		for(;index<nRows;index++)
			output.push_back(getValueFromSequence(result,index));
		return false;
	}

	PyObject * elemsequence = PySequence_Fast(elem, "expected a sequence");
	if(PySequence_Size(elemsequence)!=outTerm){
		Py_DecRef(result);
		Py_DecRef(sequence);
		Py_DecRef(elem);
		Py_DecRef(elemsequence);
		trowPyError("Error call external atom &"+name+": number of values in the sequence of output values of python function not match with the output parameter of the external atom");
	}

	for(unsigned i=0;i<outTerm;i++){
		output.push_back(getValueFromSequence(elemsequence,i));
	}
	Py_XDECREF(elem);
	index++;
	return false;
};

bool PythonManager::getNextValueNext(vec_term& output,bool& res){

	if(index>=nRows)
		return true;
	//Take the first element if sequence and check if it is another sequence. If is a value result
	//is a list of value. Otherwise result is a list of tuple
	PyObject* elem = PySequence_Fast_GET_ITEM(sequence, index);
	if(!isSequence(elem)){
		if(outTerm!=1){
			Py_DecRef(result);
			Py_DecRef(sequence);
			Py_DecRef(elem);
			trowPyError("Error call external atom &"+name+": number of output values of python function not match with the output parameter of the external atom");
		}
		output.push_back(getValue(elem,0));
		index++;
		return false;
	}
	PyObject * elemsequence = PySequence_Fast(elem, "expected a sequence");
	if(PySequence_Size(elemsequence)!=outTerm){
		Py_DecRef(result);
		Py_DecRef(sequence);
		Py_DecRef(elem);
		Py_DecRef(elemsequence);
		trowPyError("Error call external atom &"+name+": number of values in the sequence of output values of python function not match with the output parameter of the external atom");
	}

	for(unsigned i=0;i<outTerm;i++){
		output.push_back(getValueFromSequence(elem,i));
	}
	Py_XDECREF(elem);
	index++;
	return false;
}


void PythonManager::clearObjects() {
	Py_XDECREF(sequence);
	Py_XDECREF(result);
	result=nullptr;
	sequence=nullptr;
	resultIsASequence=false;
	index = 0;
	nRows=1;
}
#endif

} /* namespace grounder */
} /* namespace DLV2 */
