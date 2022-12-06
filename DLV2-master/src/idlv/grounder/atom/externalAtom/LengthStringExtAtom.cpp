/*
 * LengthStringExtAtom.cpp
 *
 *  Created on: 27 mag 2017
 *      Author: Elena Mastria
 */

#include "LengthStringExtAtom.h"
#include "../../../../common/Options.h"
namespace DLV2 {
namespace grounder {

bool LengthStringExtAtom::firstMatch( var_assignment& assignment ){

	/*
	 * This method after finding the value of the input string calculates its length.
	 * If the output term is not bound then it creates a new term with result obtained and adds it into term table
	 * else it finds the value of output term and compare it with the result.
	 */

	Term* outputTerm = this->terms[separator];

	if( !findStringValue( this->terms[0], assignment, inputString ) ){
		if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
		cerr << "WARNING: string external atom only accept numeric or string values. " << endl;
		return false;
	}
	getLengthString();

	auto& outputAssignment = assignment[outputTerm->getLocalVariableIndex()];
	if( outputTerm->getType() == VARIABLE && outputAssignment == nullptr ){

		Term* resultTerm = new NumericConstantTerm( lenghtString < 0, lenghtString );
		termTable->addTerm( resultTerm );
		outputAssignment = resultTerm;
		return true;

	}

	int value2;
	if( !findNumericValue( outputTerm, assignment, value2 ) ){
		if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
		cerr << "WARNING: length external atom only accept numeric output term. " << endl;
		return false;
	}

	return value2 == lenghtString;
}

bool LengthStringExtAtom::nextMatch( var_assignment& assignment ){
	return false;
}

}

} /* namespace DLV2 */
