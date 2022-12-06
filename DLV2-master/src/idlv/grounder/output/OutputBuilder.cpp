/*******************************************************************************
 * Copyright 2016 Francesco Calimeri, Davide Fusca', Simona Perri and Jessica Zangari
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/
/*
 * OutputBuilder.cpp
 *
 *  Created on: Jul 7, 2015
 *      Author: davide
 */

#include "NumericOutputBuilderStream.h"
#include "NumericOutputBuilder.h"
#include "TextualOutputBuilder.h"
#include "IntermediateFormatOutputBuilder.h"
#include "PreaspOutputBuilder.h"

namespace DLV2 {
namespace grounder {

OutputBuilder* OutputBuilder::outputBuilder=nullptr;


OutputBuilder* OutputBuilder::getInstance() {
	if(outputBuilder==nullptr){
		switch(Options::globalOptions()->getOutputFormat()){
			case OUTPUT_TEXTUAL:
				outputBuilder=new TextualOutputBuilder;
				break;
			case OUTPUT_QUERY_TEXTUAL:
				outputBuilder=new QueryTextualOutputBuilder;
				break;
			 case OUTPUT_NUMERIC_SMODELS:
				outputBuilder = new NumericOutputBuilder;
				break;
			 case OUTPUT_NUMERIC_INTERMEDIATE:
				outputBuilder = new IntermediateFormatOutputBuilder;
				break;
			default:
//				outputBuilder=new NumericOutputBuilderStream(cout);
				outputBuilder=new PreaspOutputBuilder;
				break;
		}
		outputBuilder->onBegin();
	}
	return outputBuilder;
}

} /* namespace grounder */
} /* namespace DLV2 */
