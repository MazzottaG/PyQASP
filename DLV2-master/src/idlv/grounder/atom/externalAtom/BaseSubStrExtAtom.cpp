/*
 * BaseSubStrExtAtom.cpp
 *
 *  Created on: 27 mag 2017
 *      Author: Elena Mastria
 */

#include "BaseSubStrExtAtom.h"
#include "../../../../common/Options.h"
namespace DLV2 {
namespace grounder {

bool BaseSubStrExtAtom::firstMatch( var_assignment& assignment ){
	/*
	 * This method checks the validity of the input terms and after finding their
	 * values creates a substring from first index to second index. If the output
	 * term is not bound then it creates a new term with substring obtained and
	 * adds it into term table else it finds the value of output term and compare
	 * it with the string result.
	 */

	Term* outputTerm = this->terms[separator];

	if( !findNumericValue( this->terms[0], assignment, firstIndex ) || !findNumericValue( this->terms[1], assignment, secondIndex ) ){
		if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
		cerr << "WARNING: first and second term in &sub_str(X,Y,Z;W) only accept numeric values. " << endl;
		return false;
	}

	if( --firstIndex < 0 || firstIndex > --secondIndex ){
		if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
		cerr << "WARNING: non valid substring indexes " << endl;
		return false;
	}

	if( !findOnlyStringValue( this->terms[2], assignment, inputString ) ){
		if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
		cerr << "WARNING: the third term must be a string value" << endl;
		return false;
	}

	if( !getOutputString() ){
		if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
		cerr << "WARNING: non valid substring indexes" << endl;
		return false;
	}

	auto& outputAssignment = assignment[outputTerm->getLocalVariableIndex()];
	if( outputTerm->getType() == VARIABLE && outputAssignment == nullptr ){

		Term* resultTerm = new StringConstantTerm( false, inputString );
		termTable->addTerm( resultTerm );
		outputAssignment = resultTerm;
		return true;

	}

	string outputString;
	if( !findOnlyStringValue( outputTerm, assignment, outputString ) || (outputString[0] != '"' && outputString[outputString.length() - 1] != '"') ){
		if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
		cerr << "WARNING: the third term must be a quoted string. " << endl;
		return false;
	}
	return inputString == outputString;

}

bool BaseSubStrExtAtom::nextMatch( var_assignment& assignment ){
	return false;
}

}

} /* namespace DLV2 */
