/*
 * CountCombsGenerator.cpp
 *
 *  Created on: 03 lug 2019
 *      Author: Elena Mastria
 */

#include "CombsGenerator.h"

namespace DLV2 {
namespace grounder {



void CountCombsGenerator::addTerm( Term* term ){
	if( uniqueTerms.count( term ) )
		uniqueTerms[term]++;
	else
		uniqueTerms[term] = 1;
}

bool CountCombsGenerator::nextCombination(){
	if( uniqueTerms.empty() ){
		if( firstCombination ){ //case: all false
		firstCombination = false;
			return true;
		}
		return false;
	}
	//the maximum number into the range is equal to the summation of all unique terms maximum occurrences
	if( firstCombination )
		for ( auto& x : uniqueTerms )
			maxCount += x.second;
	else{
		if( currentCount->getConstantValue() < maxCount )
			currentCount = currentCount->increment();
		else
			return false;
	}
	return true;
}

Term* CountCombsGenerator::getCurrentEvaluation(){
	if( firstCombination )
		firstCombination = false;
	return currentCount;
}

} /* namespace grounder */
} /* namespace DLV2 */
