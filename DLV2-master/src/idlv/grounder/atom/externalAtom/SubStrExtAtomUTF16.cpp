/*
 * SubStrExtAtomUTF16.cpp
 *
 *  Created on: 10 mag 2017
 *      Author: Elena Mastria
 */

#include "SubStrExtAtomUTF16.h"

#include <codecvt>
namespace DLV2 {
namespace grounder {

/*
 * If the input string is a quoted string this method increments the input
 * indexes to match the corresponding indexes within the string
 */
bool moveIndex( int & firstIndex, int & secondIndex, u16string & firstString ){
	int validPosition = firstString.size() - 1;
	char16_t c = '"';
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
bool SubStrExtAtomUTF16::getOutputString(){
	//Converts the base string to an u16string to handle strings that contain special characters not included in the utf-8
	wstring_convert< codecvt_utf8_utf16< char16_t >, char16_t > convert;
	u16string u16FirstString = convert.from_bytes( inputString );
//	cout << "input string " << convert.to_bytes( u16FirstString ) << "first index" << firstIndex << "second index" << secondIndex << endl;
	if( !moveIndex( firstIndex, secondIndex, u16FirstString ) ){
//		cout << "not valid position" << endl;
		return false;
	}
	int lenghtSubstring = secondIndex - firstIndex + 1;
	inputString = "\"" + convert.to_bytes( u16FirstString.substr( firstIndex, lenghtSubstring ) ) + "\"";
//	cout << "input string afer" << inputString << endl;
	return true;
}

}

} /* namespace DLV2 */
