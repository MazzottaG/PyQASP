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
 * ShowDirective.h
 *
 *  Created on: Feb 21, 2017
 *      Author: davide
 */

#ifndef SRC_DIRECTIVES_EXTPREDCONVERSIONDIRECTIVE_H_
#define SRC_DIRECTIVES_EXTPREDCONVERSIONDIRECTIVE_H_

#include "../grounder/table/PredicateTable.h"
#include "Directive.h"
#include "../util/Utils.h"

#include <regex>


using namespace std;

namespace DLV2 {
namespace grounder{

class ExtPredConversionDirective : public Directive {
public:
	using type_conv = vector<TypeConv>;
	ExtPredConversionDirective ( char* v ) : Directive(v),predicate(nullptr){};
	virtual bool split();
	Predicate* getPredicate(){return predicate;};
	type_conv getTypeConv(){return extAtomTypeConv;};
private:
	Predicate * predicate;
        type_conv extAtomTypeConv;
public:
        static constexpr char DIR_NAME[] = "#external_predicate_conversion"; 
};

}
} /* namespace DLV2 */

#endif /* SRC_DIRECTIVES_SHOWDIRECTIVE_H_ */
