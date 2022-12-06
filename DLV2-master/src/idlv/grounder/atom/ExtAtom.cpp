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
 * ExtAtom.cpp
 *
 *  Created on: 19 lug 2016
 *      Author: giovanni
 */

#include "ExtAtom.h"
#include "../../../common/Options.h"
namespace DLV2 {

namespace grounder{
ExtAtom::ExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
		Atom( terms ), predicate( predicate ), separator( separator ), negative( naf ), termTable( TermTable::getInstance() ){
}

ExtAtom::ExtAtom( Predicate* predicate, int separator, bool naf ) :
		Atom(), predicate( predicate ), separator( separator ), negative( naf ), termTable( TermTable::getInstance() ) {
}

bool ExtAtom::match(var_assignment& assignment, bool direction) {
	if(direction)
		return firstMatch(assignment);
	else
		return nextMatch(assignment);
}

bool ExtAtom::operator==(const Atom& a) const {
	if(!a.isExternalAtom())
		return false;
	if(this->getPredicate()->getName() != a.getPredicate()->getName())
		return false;
	if(terms.size() != a.getTermsSize())
		return false;
	if(separator != a.getSeparator())
		return false;
	//comparison of terms
	for(unsigned i=0; i<terms.size(); i++)
		if(terms[i]->getIndex()!=a.getTerm(i)->getIndex())
			return false;
	return true;
}

void ExtAtom::print(ostream& stream=cout) const {
	if(negative)
		stream<<"not ";
	stream<<"&"<<predicate->getName()<<"(";
	for(unsigned i=0; i<separator; i++) {
		terms[i]->print(stream);
		if(i < separator-1)
			stream<<",";
	}
	stream<<";";
	unsigned arity = terms.size();
	for(unsigned i=separator; i<arity; i++) {
		terms[i]->print(stream);
		if(i < arity-1)
			stream<<",";
	}
	stream << ")";
}

bool ExtAtom::findNumericValue( Term* t, var_assignment& assignment, int & value ){

	if( t->getType() == NUMERIC_CONSTANT ){
		value = t->getConstantValue();
		return true;
	}
	Term* termAssignment = assignment[t->getLocalVariableIndex()];
	if( t->getType() == VARIABLE && termAssignment->getType() == NUMERIC_CONSTANT ){
		value = termAssignment->getConstantValue();
		return true;
	}
	if( t->getType() == ARITH ){
		bool isOk = true;
		value = t->substituteAndCalculate( assignment, isOk );
		return isOk;
	}

	return false;

}
bool ExtAtom::findStringValue( Term* t, var_assignment& assignment, string & value ){
	int valueTmp = 0;

	if( findNumericValue( t, assignment, valueTmp ) ){
		value = '"' + to_string( valueTmp ) + '"';
		return true;
	}
	TermType termType = t->getType();
	if( termType == SYMBOLIC_CONSTANT || termType == STRING_CONSTANT ){
		value = t->getName();
		return true;
	}

	Term * termAssignment = assignment[t->getLocalVariableIndex()];
	TermType termAssignmentType = termAssignment->getType();

	if( termType == VARIABLE && (termAssignmentType == SYMBOLIC_CONSTANT || termAssignmentType == STRING_CONSTANT) ){
		value = termAssignment->getName();
		return true;
	}
	//todo if type == function??
	return false;
}
bool ExtAtom::findOnlyStringValue( Term* t, var_assignment& assignment, string & value ){
	TermType termType = t->getType();
	if( termType == NUMERIC_CONSTANT )
		return false;

	if( termType == SYMBOLIC_CONSTANT || termType == STRING_CONSTANT ){
		value = t->getName();
		return true;
	}

	Term * termAssignment = assignment[t->getLocalVariableIndex()];
	TermType termAssignmentType = termAssignment->getType();

	if( termType == VARIABLE && (termAssignmentType == SYMBOLIC_CONSTANT || termAssignmentType == STRING_CONSTANT) ){
		value = termAssignment->getName();
		return true;
	}
	//todo if type == function??
	return false;
}

bool ExtAtom::getInputElem( var_assignment& assignment, Term*& t ){
	TermType termType = t->getType();
	if( isConstantTerm( termType ) ){
		return true;
	}
	if( isList( termType ) || termType == TermType::FUNCTION ){
		if( !t->isGround() )
			t = t->substitute( assignment );
		else
			t = t->calculate();
		return true;
	}
	if( termType == TermType::ARITH ){
		t = t->substitute( assignment );
		return true;
	}
	if( termType == TermType::VARIABLE ){
		t = assignment[t->getLocalVariableIndex()];
		return true;
	}
	return false;
}
bool ExtAtom::getInputList( var_assignment& assignment, Term*& t ){
	TermType termType = t->getType();
	if( isList( termType ) ){

		if( !t->isGround() )
			t = t->substitute( assignment );
		else
			t = t->calculate();
		if( t->getTermsSize() == 0 ){
			if(Options::globalOptions()->isEnableExternalAtomsWarnings())
			cerr << "WARNING: Input list is empty" << endl;
			return false;
		}
		return true;

	}
	if( termType == TermType::VARIABLE && isList( (t = assignment[t->getLocalVariableIndex()])->getType() ) ){
		if( t->getTermsSize() == 0 ){
			if(Options::globalOptions()->isEnableExternalAtomsWarnings())
			cerr << "WARNING: Input list is empty" << endl;
			return false;
		}
		return true;
	}
	return false;
}
bool ExtAtom::getInputNumericTerm( var_assignment& assignment, Term*& t ){
	TermType tType = t->getType();
	if( tType == NUMERIC_CONSTANT ){
		return true;
	}
	if( tType == ARITH ){
		bool isOk = true;
		int tValue = t->substituteAndCalculate( assignment, isOk );
		if( isOk ){
		Term * posTerm = new NumericConstantTerm( tValue < 0, tValue );
		termTable->addTerm( posTerm );
			t = posTerm;
		}
		return isOk;
	}
	if( tType == VARIABLE && (t = assignment[t->getLocalVariableIndex()])->getType() == NUMERIC_CONSTANT ){
		return true;
	}
	return false;
}

} /* namespace DLV2 */
} /* namespace grounder */
