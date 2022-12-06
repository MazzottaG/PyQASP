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
 * ShowDirective.cpp
 *
 *  Created on: Feb 21, 2017
 *      Author: davide
 */

#include "ExtPredConversionDirective.h"
#include <string>
#include "../../common/ErrorMessage.h"

using namespace std;

namespace DLV2 {

namespace grounder{

constexpr char ExtPredConversionDirective::DIR_NAME[];

bool ExtPredConversionDirective::split(){
	std::regex ree("^\\(\"(.+?)\"\\s*,\\s*(.+?),\\s*type:\\s*(.+?)\\)\\s*.");
	smatch m;
	regex_search(value,m,ree);
	if(m.size()<2){
                string name = {DIR_NAME};
		ErrorMessage::errorDuringParsing("--> Syntax Error in "+name+value);
		return false;
	}
        string predicateName = m[1].str();
	int arity = atoi(m[2].str().c_str());

        string tc = m[3];
        regex ee("\\s*,\\s*");
        regex_token_iterator<string::iterator> tokens(tc.begin(), tc.end(), ee, -1);
        regex eee("(U_INT|UT_INT|UR_INT|T_INT|R_INT|CONST|Q_CONST)");
        regex_token_iterator<string::iterator> rend;
        while(tokens != rend){
                string s = *tokens;
                if(!regex_match(s, eee)) {
                        ErrorMessage::errorDuringParsing("#import_local_sparql Invalid type conv: \"" + s +"\". Only {U_INT, UT_INT, UR_INT, T_INT, R_INT, CONST, Q_CONST} are possible.");
                        return false;
                }
                extAtomTypeConv.push_back(Utils::toTypeConv(s));
                tokens++;
        }
        
	predicate = new Predicate(predicateName,arity);
	predicate->setHiddenForPrinting(false);
	PredicateTable::getInstance()->insertPredicate(predicate);

	return true;
}

}
} /* namespace DLV2 */
