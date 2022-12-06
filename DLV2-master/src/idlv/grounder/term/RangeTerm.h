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
 * RangeTerm.h
 *
 *  Created on: 08/feb/2015
 *      Author: tesi2
 */

#ifndef RANGETERM_H_
#define RANGETERM_H_

#include "Term.h"

namespace DLV2{

namespace grounder{

class RangeTerm: public Term {
public:
	RangeTerm():Term(),lowerBound(0),upperBound(0){}
	RangeTerm(int l, int u):Term(),lowerBound(l),upperBound(u){}

	virtual bool isRange() const {return true;}

	virtual bool contain(TermType type){
		return type==TermType::RANGE;
	}

	int getLowerBound() const {return lowerBound;}
	void setLowerBound(int lowerBound) {this->lowerBound = lowerBound;}

	int getUpperBound() const {return upperBound;}
	void setUpperBound(int upperBound) {this->upperBound = upperBound;}

	void setNegative(bool n){if(n) lowerBound=-lowerBound;};

private:
	int lowerBound;
	int upperBound;
};

}

}

#endif /* RANGETERM_H_ */
