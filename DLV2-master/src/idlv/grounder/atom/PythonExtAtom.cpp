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
 * PyExtAtom.cpp
 *
 *  Created on: 23 lug 2016
 *      Author: giovanni
 */

#include "PythonExtAtom.h"
#include "../../input/ExtAtomFactory.h"
#include "../../../common/Trace.h"
#include "../exception/ExtAtomException.h"


namespace DLV2 {
namespace grounder {


bool PythonExtAtom::firstMatch(var_assignment& assignment) {
	//FirstMatch of external python atom. First clear the object of pymanager and call the python function.
	// Then get the first output value of the function. If not match with some value in the assignment and the result is
	// a list search for another result otherwise return false
	//Until exist a tuple in the output of python function match the output with the assignment and return true if match otherwise return false

	pyManager->clearObjects();

	pyManager->callPythonFunc(terms, assignment);

	vector<Term*> outputTerm;
	outputTerm.reserve(terms.size());

	bool res;
	if(pyManager->getNextValueFirst(outputTerm,res)){
		return false;
	}
	if(!outputTerm.size())
		return res;

	if(matchPyResult(assignment,outputTerm)){
		return true;
	}

	outputTerm.clear();
	while(!pyManager->getNextValueNext(outputTerm,res)){
		if(matchPyResult(assignment,outputTerm)){
			return true;
		}
		outputTerm.clear();
	}
	return false;
}

bool PythonExtAtom::nextMatch(var_assignment& assignment) {
	//Next match of external python. If the output is not a list the function getNextValueNext return true otherwise
	// return the next tuple in the list and with matchPyResult check if is coherent with the assignment. Until exist
	// a tuple in the output of python function match the output with the assignment and return true if match otherwise return false
	if(!pyManager->isResultASequence())return false;

	vector<Term*> outputTerm;
	outputTerm.reserve(terms.size());
	bool res;
	while(!pyManager->getNextValueNext(outputTerm,res)){
		if(matchPyResult(assignment,outputTerm)){
			return true;
		}
		outputTerm.clear();
	}
	return false;
}





bool PythonExtAtom::matchPyResult(var_assignment& assignment,vector<Term*>& outputTerm) {
	//This function put the output of the python function in the assignment. If one or more output
	//value is ground then match with the output value and if not match return false and restore the assignment

	vector<unsigned> changedAssignemnt;
	changedAssignemnt.reserve(assignment.size());

	for(unsigned i=separator; i<terms.size(); i++) {
		Term *t1=terms[i];
		Term *t2=outputTerm[i-separator];
		Term *t3=t1;
		if(!t1->isGround())
			t3=assignment[t1->getLocalVariableIndex()];
		if(t3!=nullptr){
			if(t3!=t2){
				for(auto v:changedAssignemnt)assignment[v]=nullptr;
				return false;
			}
		}else{
			auto index=t1->getLocalVariableIndex();
			assignment[index] = t2;
			changedAssignemnt.push_back(index);
		}
	}


	return true;

}

} /* namespace grounder */
} /* namespace DLV2 */
