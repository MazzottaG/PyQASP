/*
 * ArithmeticExtAtom.cpp
 *
 *  Created on: 29 apr 2017
 *      Author: Elena Mastria
 */

#include "ArithmeticExtAtom.h"
#include "../../../../common/Options.h"
namespace DLV2 {

namespace grounder {

inline bool isStringValue( TermType t ){
	return t == STRING_CONSTANT || t == SYMBOLIC_CONSTANT;
}

bool ArithmeticExtAtom::firstMatch( var_assignment& assignment ){
	/*
	 * This method checks the validity of the input terms and after finding their numeric
	 * values computes the result of any arithmetic operation.
	 * If the output term is not bound then it creates a new term with result obtained and adds it into term table
	 * else it finds the value of output term and compare it with the result of operation.
	 */

	Term* outputTerm = this->terms[separator];

	for ( unsigned i = 0; i < separator; i++ ){
		auto term = this->terms[i];
		if( isStringValue( term->getType() ) || !findNumericValue( term, assignment, inputTermsValues[i] ) ){
			if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
			cerr << "WARNING: arithmetic external atom used with non numeric value " << endl;
			return false;
		}
	}
	if( isStringValue( outputTerm->getType() ) ){
		if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
		cerr << "WARNING: arithmetic external atom used with non numeric value " << endl;
		return false;
	}
	if( !getResult() )
		return false;

	auto& outputAssignment = assignment[outputTerm->getLocalVariableIndex()];
	if( outputTerm->getType() == VARIABLE && outputAssignment == nullptr ){
		Term* resultTerm = new NumericConstantTerm( result < 0, result );
		termTable->addTerm( resultTerm );
		outputAssignment = resultTerm;
		return true;

	}
	isFunctional = true;
	int outputValue = 0;
	if( !findNumericValue( outputTerm, assignment, outputValue ) ){
		if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
		cerr << "WARNING: arithmetic external atom used with non numeric value " << endl;
		return false;
	}
	return compareOutputValue( outputValue );

}

bool ArithmeticExtAtom::nextMatch( var_assignment& assignment ){
	return false;
}

}
} /* namespace DLV2 */
