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
 * TextualOutputBuilder.h
 *
 *  Created on: Jul 10, 2015
 *      Author: jessica
 */

#ifndef SRC_GROUNDER_OUTPUT_TEXTUALOUTPUTBUILDER_H_
#define SRC_GROUNDER_OUTPUT_TEXTUALOUTPUTBUILDER_H_

#include "OutputBuilder.h"

namespace DLV2 {
namespace grounder {

class TextualOutputBuilder: public OutputBuilder {
public:
	TextualOutputBuilder(){};
	virtual ~TextualOutputBuilder(){};

    virtual void onRule(Rule *rule) {
    	if(!rule->areThereUndefinedAtomInBody() && rule->getSizeHead()==1){
    		Atom* atom=rule->getAtomInHead(0);
    		if(!atom->isChoice() && atom->isFact() && atom->getPredicate()->isHiddenForPrinting())
    			return;
    	}
    	rule->print();}
    virtual void onFact(Atom* atom) {if(atom->getPredicate()->isHiddenForPrinting()) return; atom->print();cout<<"."<<endl;}

};

class QueryTextualOutputBuilder : public TextualOutputBuilder {
public:
	QueryTextualOutputBuilder():enablePrint(false){}

	void onRule(Rule *rule){
		if(enablePrint)
			rule->print();
	}

    void setEnablePrint(bool enable){
    	enablePrint=enable;
    }

    void onFact(Atom* atom){
    	if(enablePrint){
    		atom->print();cout<<"."<<endl;
    	}
    }
private:
    bool enablePrint;

};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_OUTPUT_TEXTUALOUTPUTBUILDER_H_ */
