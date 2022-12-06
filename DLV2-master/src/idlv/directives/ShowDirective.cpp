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

#include "ShowDirective.h"
#include <string>
#include "../../common/ErrorMessage.h"
using namespace std;

namespace DLV2 {

namespace grounder{

bool ShowDirective::split(){
	string predicateArity(value);
	predicateArity.erase(remove(predicateArity.begin(),predicateArity.end(),' '),predicateArity.end());
	std::regex ree("(.*?)/(\\d+)\\.");
	smatch m;
	regex_search(predicateArity,m,ree);
	if(m.size()<2){
		ErrorMessage::errorDuringParsing("--> Invalid #show directive: " + value);
		return false;
	}
	string predicateName = m[1].str();
	predicateName.erase(std::remove(predicateName.begin(),predicateName.end(),' '),predicateName.end());
	int arity = atoi(m[2].str().c_str());
        bool trueNeg=false;
        if(predicateName.at(0) == '-'){
                predicateName.erase(0, 1);
                trueNeg = true;

        }


	predicate = new Predicate(predicateName,arity);
        predicate->setTrueNegated(trueNeg);
	predicate->setHiddenForPrinting(false);
	PredicateTable::getInstance()->insertPredicate(predicate);

	return true;
}

}
} /* namespace DLV2 */
