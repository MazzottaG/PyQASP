/*
 * MinMaxCombsChainGenerator.cpp
 *
 *  Created on: 02 lug 2019
 *      Author: Elena Mastria
 */

#include "CombsGenerator.h"

namespace DLV2 {

namespace grounder {


MinMax_CombsGenerator::~MinMax_CombsGenerator(){
	// TODO Auto-generated destructor stub
}

void DLV2::grounder::MinMax_CombsGenerator::addTerm( Term* term ){
	uniqueTerms.insert( term );
}

bool DLV2::grounder::MinMax_CombsGenerator::nextCombination(){
	if( uniqueTerms.empty() ){
		if( firstCombination ){ //case: all false
			firstCombination = false;
			return true;
		}
		return false;
	}
	if( firstCombination )
		iterator = uniqueTerms.begin();
	else{
		if( iterator != uniqueTerms.end() )
			iterator++;
		else
			return false;
	}
	return true;
}

Term* MinMax_CombsGenerator::getCurrentEvaluation(){
	if( firstCombination )
		firstCombination = false;
	return (iterator != uniqueTerms.end()) ? *iterator : nullptr;
}

}
}
