/*
 * MemberExtAtom.cpp
 *
 *  Created on: 05 mag 2017
 *      Author: Elena Mastria
 */

#include "MemberExtAtom.h"
#include "../../../../common/Options.h"
namespace DLV2 {
namespace grounder {

void MemberExtAtom::cleanString( string & str ){
	int validPosition = str.length() - 1;
	if( str[0] == '"' && str[validPosition] == '"' ){
		str = str.substr( 1, --validPosition );
	}
}

bool MemberExtAtom::firstMatch( var_assignment& assignment ){

	if( !findStringValue( this->terms[0], assignment, inputString1 ) || !findStringValue( this->terms[1], assignment, inputString2 ) ){
		if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
		cerr << "WARNING: string external atom only accept numeric or string values. " << endl;
		return false;
	}
	cleanString (inputString1);
	cleanString (inputString2);
	return inputString2.find( inputString1 ) != string::npos;

}

bool MemberExtAtom::nextMatch( var_assignment& assignment ){
	return false;
}

}

} /* namespace DLV2 */
