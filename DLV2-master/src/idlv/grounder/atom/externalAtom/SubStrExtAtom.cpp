/*
 * SubStrExtAtom.cpp
 *
 *  Created on: 05 mag 2017
 *      Author: Elena Mastria
 */

#include "SubStrExtAtom.h"
namespace DLV2 {
namespace grounder {

/*
 * If the input string is a quoted string this method increments the input
 * indexes to match the corresponding indexes within the string
 */
bool moveIndex( int & firstIndex, int & secondIndex, string & firstString ){
	int validPosition = firstString.size() - 1;
	if( secondIndex > validPosition )
		return false;
	if( firstString[0] == '"' && firstString[validPosition] == '"' ){
		firstIndex++;
		secondIndex++;
		if( secondIndex > validPosition - 1 )
			return false;
	}
	return true;
}

bool SubStrExtAtom::getOutputString(){
	if( !moveIndex( firstIndex, secondIndex, inputString ) )
		return false;

	int lenghtSubstring = secondIndex - firstIndex + 1;
	inputString = "\"" + inputString.substr( firstIndex, lenghtSubstring ) + "\"";
	return true;
}

}

} /* namespace DLV2 */

