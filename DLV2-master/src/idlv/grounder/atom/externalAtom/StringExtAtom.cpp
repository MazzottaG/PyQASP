/*
 * ListExtAtom.cpp
 *
 *  Created on: 03 mag 2017
 *      Author: Elena Mastria
 */

#include "StringExtAtom.h"
#include "../../../../common/Options.h"
namespace DLV2 {
namespace grounder {

bool StringExtAtom::firstMatch( var_assignment& assignment ){
	/*
	 * This method checks the validity of the input terms and after finding their string
	 * values computes the string result of any manipulation over them.
	 * If the output term is not bound then it creates a new term with result obtained and
	 * add it into term table else it finds the value of output term and compare it with the string result.
	 */
	Term* outputTerm = this->terms[separator];

	for ( unsigned i = 0; i < separator; i++ ){
		if( !findStringValue( this->terms[i], assignment, inputTermsValues[i] ) ){
			if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
			cerr << "WARNING: string external atom only accept numeric or string values. " << endl;
			return false;
		}
	}

	getResult();

	auto& outputAssignment = assignment[outputTerm->getLocalVariableIndex()];
	if( outputTerm->getType() == VARIABLE && outputAssignment == nullptr ){

		Term* resultTerm = new StringConstantTerm( false, resultString );
		termTable->addTerm( resultTerm );
		outputAssignment = resultTerm;
		return true;

	}

	string outputString;
	if( invalidOutputString( outputTerm, assignment, outputString ) ){
		if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
		cerr << "WARNING: Invalid output term. " << endl;
		return false;
	}
	return outputString == resultString;

}

bool StringExtAtom::nextMatch( var_assignment& assignment ){
	return false;
}

}/* namespace grounder*/

} /* namespace DLV2 */
