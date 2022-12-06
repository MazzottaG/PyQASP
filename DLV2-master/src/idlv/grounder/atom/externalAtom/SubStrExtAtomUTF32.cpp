/*
 * SubStrExtAtomUTF32.cpp
 *
 *  Created on: 10 mag 2017
 *      Author: Elena Mastria
 */

#include "SubStrExtAtomUTF32.h"
#include "../../../../common/Options.h"
#include <codecvt>
namespace DLV2 {
namespace grounder {

/*
 * If the input string is a quoted string this method increments the input
 * indexes to match the corresponding indexes within the string
 */
bool moveIndex( int & firstIndex, int & secondIndex, u32string & firstString ){
	int validPosition = firstString.size() - 1;
	char32_t c = '"';
	if( secondIndex > validPosition )
		return false;
	if( firstString[0] == c && firstString[validPosition] == c ){
		firstIndex++;
		secondIndex++;
		if( secondIndex > validPosition - 1 )
			return false;
	}
	return true;
}

bool SubStrExtAtomUTF32::firstMatch( var_assignment& assignment ){
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

	if( firstIndex < 0 || firstIndex > secondIndex ){
		if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
		cerr << "WARNING: non valid substring indexes " << endl;
		return false;
	}

	if( !findOnlyStringValue( this->terms[2], assignment, inputString ) ){
		if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
		cerr << "WARNING: the third term must be a string value" << endl;
		return false;
	}

	//Converts the base string to an u16string to handle strings that contain special characters not included in the utf-8
	wstring_convert< codecvt_utf8< char32_t >, char32_t > convert;
	u32string u32FirstString = convert.from_bytes( inputString );

	if( !moveIndex( firstIndex, secondIndex, u32FirstString ) ){
		if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
		cerr << "WARNING: non valid substring indexes" << endl;
		return false;
	}

	int lenghtSubstring = secondIndex - firstIndex + 1;
	inputString = "\"" + convert.to_bytes( u32FirstString.substr( firstIndex, lenghtSubstring ) ) + "\"";

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

bool SubStrExtAtomUTF32::nextMatch( var_assignment& assignment ){
	return false;
}

}

} /* namespace DLV2 */
