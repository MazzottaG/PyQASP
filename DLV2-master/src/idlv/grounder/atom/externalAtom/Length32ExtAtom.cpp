/*
 * Length32ExtAtom.cpp
 *
 *  Created on: 15 mag 2017
 *      Author: Elena Mastria
 */

#include "Length32ExtAtom.h"
#include "../../../../common/Options.h"
#include <codecvt>

namespace DLV2 {
namespace grounder {

bool Length32ExtAtom::firstMatch( var_assignment& assignment ){

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

	wstring_convert< codecvt_utf8< char32_t >, char32_t > convert;
	u32string u32FirstString = convert.from_bytes( inputString );

	lenghtString = u32FirstString.length();
	char32_t c = '"';
	if( u32FirstString[0] == c && u32FirstString[lenghtString - 1] == c )
		lenghtString -= 2;

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

bool Length32ExtAtom::nextMatch( var_assignment& assignment ){
	return false;
}

}

} /* namespace DLV2 */
