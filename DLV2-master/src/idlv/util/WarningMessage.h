/*******************************************************************************
 *   Copyright 2016 Francesco Calimeri, Davide Fusca', Simona Perri and Jessica Zangari
 *  
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *  
 *        http://www.apache.org/licenses/LICENSE-2.0
 *  
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *******************************************************************************/
/*
 * WarningMessage.h
 *
 *  Created on: 17 nov 2016
 *      Author: giovanni
 */

#ifndef WARNINGMESSAGE_H_
#define WARNINGMESSAGE_H_

namespace DLV2 {

#include <string>
#include <iostream>
#include <cstdlib>

#include "Constants.h"

class WarningMessage {

public:
	static void warningDuringParsing(const std::string& warningMessage) {
		warningDuringParsing(warningMessage.c_str());
	}

	static void warningDuringParsing(const char* warningMessage) {
		std::cerr << WARNING_PARSING << ": " << warningMessage << std::endl;
	}

	static void warningGeneric(const std::string& warningMessage) {
		warningGeneric(warningMessage.c_str());
	}

	static void warningGeneric(const char* warningMessage) {
		std::cerr << WARNING_GENERIC << ": " << warningMessage << std::endl;
	}
};

} /* namespace DLV2 */

#endif /* SRC_UTIL_WARNINGMESSAGE_H_ */
