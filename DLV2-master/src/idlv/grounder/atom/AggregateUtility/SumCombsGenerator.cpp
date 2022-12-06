/*
 * SumCombsGenerator.cpp
 *
 *  Created on: 22 lug 2019
 *      Author: Elena Mastria
 */
#include "CombsGenerator.h"
namespace DLV2 {

namespace grounder {

void SumCombsGenerator::addTerm( Term* term ){
	if( term->getType() != TermType::NUMERIC_CONSTANT )
		return;
	vector< int > toAdd;
	int currentValue = term->getConstantValue();
	toAdd.reserve( partialSums.size() );
	for ( const auto& gSum : partialSums )
		toAdd.push_back( gSum + currentValue );
	partialSums.insert( currentValue );
	if( !toAdd.empty() )
		partialSums.insert( toAdd.begin(), toAdd.end() );
}

bool SumCombsGenerator::nextCombination(){
	if( partialSums.size() == 1 ){
		if( firstCombination ){ //case: all false
			actualValue = partialSums.begin();
			firstCombination = false;
			return true;
		}
		return false;
	}
	if( firstCombination ){
		actualValue = partialSums.begin();
		firstCombination = false;
	} else{
		if( actualValue != partialSums.end() )
			actualValue++;
		else
			return false;
	}
	return true;
}

Term*SumCombsGenerator::getCurrentEvaluation(){
	Term* toReturn = new NumericConstantTerm( false, *actualValue );
	TermTable::getInstance()->addTerm( toReturn );
	return toReturn;
}
}
}






