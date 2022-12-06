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
 * PyExtAtom.h
 *
 *  Created on: 23 lug 2016
 *      Author: giovanni
 */

#ifndef PYTHONEXTATOM_H_
#define PYTHONEXTATOM_H_

#include "ExtAtom.h"
#include "PythonManager2.h"


using namespace std;


namespace DLV2 {
namespace grounder {

class PythonExtAtom: public ExtAtom {
public:
	PythonExtAtom(Predicate* predicate, PyObject* func, vector<Term*>& terms, unsigned separator, bool naf) :
		ExtAtom(predicate, terms, separator, naf)
		{ pyManager = new PythonManager(separator, predicate->getName(), func); }

	PythonExtAtom(Predicate* predicate, PyObject* func, unsigned separator, bool naf) :
		ExtAtom(predicate, separator, naf)
		{ pyManager = new PythonManager(separator, predicate->getName(), func); }

	virtual ~PythonExtAtom() {delete pyManager;}


	void setTypeConvs(vector<TypeConv>& typeConvs) {
		this->pyManager->setTypeConvs(typeConvs);
	}

	Atom* clone() {
		Atom* atom = new PythonExtAtom(predicate, pyManager->getPyFunc(), this->separator, this->negative);
		atom->setTerms(this->terms);
		atom->setTypeConvs(pyManager->getTypeConvs());
		return atom;
	}



protected:
	virtual bool firstMatch(var_assignment& assignment);
	virtual bool nextMatch(var_assignment& assignment);

private:
	bool matchPyResult(var_assignment& assignment,vector<Term*>& outputTerm);


	//the python manager used to perform python operations
	PythonManager* pyManager;

};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* PYTHONEXTATOM_H_ */
