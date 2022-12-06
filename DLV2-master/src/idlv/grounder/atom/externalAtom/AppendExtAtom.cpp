/*
 * AppendExtAtom.cpp
 *
 *  Created on: 30 apr 2017
 *      Author: Elena Mastria
 */
#include "AppendExtAtom.h"

namespace DLV2 {

namespace grounder {

void AppendExtAtom::cleanString( string & str, bool & symbolicConstant ){
	int strLenght = str.length() - 1;
	if( str[0] == '"' && str[strLenght] == '"' ){
		str = str.substr( 1, strLenght - 1 );
		symbolicConstant = true;
	}
}
void AppendExtAtom::getResult(){
	bool symbolicConstant = false;
	resultString = "";
	for ( string str : inputTermsValues ){
		cleanString( str, symbolicConstant );
		resultString += str;
	}
	if( symbolicConstant )
		resultString = "\"" + resultString + "\"";
}

}
} /* namespace DLV2 */
