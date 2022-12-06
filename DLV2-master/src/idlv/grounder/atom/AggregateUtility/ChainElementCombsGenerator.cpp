/*
 * ChainElementCombsGenerator.cpp
 *
 *  Created on: 03 lug 2019
 *      Author: Elena Mastria
 */

#include "ChainElementCombsGenerator.h"

namespace DLV2 {
namespace grounder {

/*
 * Compute the next value of combination, if no other combination
 * can be generated then return false.
 */
bool ChainElementAllCombsGenerator::nextCombination(){
	/*
	 * If this generator is a Tail then increases its actual value.
	 * If in the previous update it reached its maximum then return
	 * false in order to notify to its predecessor that there is no
	 * other combination left
	 */
	if( nextGenerator == nullptr ){
		if( actualValue == maxOccurrency ){
			return false;
		}
		actualValue++;
		return true;
	} else{
		/*
		 * Else if this generator isn't a Tail then increases its actual value.
		 * If in the previous update it reached its maximum then it resets its
		 * actual value and forward the request to the successor.
		 * The forward is made only if the successor didn't finish, in this case
		 * return false in order to notify to its predecessor that there is no
		 * other combination left
		 */
		if( actualValue == maxOccurrency ){
			if( finish )
				return false;
			else{
				finish = !nextGenerator->nextCombination();
				actualValue = 0;
			}
		} else{
			actualValue++;
		}
		return true;
	}
}

void ChainElementAllCombsGenerator::print( ostream& stream ) const{
	if( nextGenerator != nullptr )
		nextGenerator->print( stream );
	stream << actualValue;
}

void ChainElementAllCombsGenerator::printTerm( ostream& stream ) const{
	if( nextGenerator != nullptr ){
		nextGenerator->printTerm( stream );
		term->print( stream );
	} else{
		term->print( stream );
		stream << "|";
	}
}
ChainElementCombsGenerator& ChainElementCombsGenerator::operator =( const ChainElementCombsGenerator& other ){
	if( this != &other ){
		this->nextGenerator = other.nextGenerator;
		this->finish = other.finish;
		this->term = other.term;
	}
	return *this;
}

void ChainElementPrimeCombsGenerator::newOccurrencyFound(){
	//add the value of generator term multiplied for the actual occurrence found
	partialSums.insert( partialSums.end(), (*(++partialSums.begin())) * (partialSums.size() - 1) ); //TODO check -1
}

bool ChainElementPrimeCombsGenerator::nextCombination(){
	/*
	 * If this generator is a Tail then increases its actual value.
	 * If in the previous update it reached its maximum then return
	 * false in order to notify to its predecessor that there is no
	 * other combination left
	 */
	if( nextGenerator == nullptr ){
		if( actualValue == --partialSums.end() ){
			return false;
		}
		actualValue++;
		return true;
	} else{
		/*
		 * Else if this generator isn't a Tail then increases its actual value.
		 * If in the previous update it reached its maximum then it resets its
		 * actual value and forward the request to the successor.
		 * The forward is made only if the successor didn't finish, in this case
		 * return false in order to notify to its predecessor that there is no
		 * other combination left
		 */
		if( actualValue == --partialSums.end() ){
			if( finish )
				return false;
			else{
				finish = !nextGenerator->nextCombination();
				actualValue = partialSums.begin();
			}
		} else{
			actualValue++;
		}
		return true;
	}
}

bool ChainElementPrimeCombsGenerator::absorbGenerator( ChainElementPrimeCombsGenerator* genToAssorb ){
	Term *termToAssorb = genToAssorb->getTerm();
	if( !(this->term->getType() == TermType::NUMERIC_CONSTANT) || !(termToAssorb->getType() == TermType::NUMERIC_CONSTANT) )
		return false;

	if( (termToAssorb->getConstantValue() % this->term->getConstantValue()) == 0 ){
		set< int > partialSumsToAssorb( genToAssorb->getPartialSums() );
		vector< int > partialSumsToAdd;
		partialSumsToAdd.reserve( partialSums.size() * partialSumsToAssorb.size() );
		for ( const auto& gSum : partialSums )
			for ( const auto& gTaSum : partialSumsToAssorb )
				partialSumsToAdd.push_back( gSum + gTaSum );
		partialSums.insert( partialSumsToAssorb.begin(), partialSumsToAssorb.end() ); //almost linear
		partialSums.insert( partialSumsToAdd.begin(), partialSumsToAdd.end() ); //nlog(size+n)
		return true;
	}
	return false;
	
}
} /* namespace grounder */
} /* namespace DLV2 */


