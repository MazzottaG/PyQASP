/*
 * AllPrimeCombinationsChainGenerator.cpp
 *
 *  Created on: 09 lug 2019
 *      Author: Elena Mastria
 */

#include "CombsGenerator.h"
namespace DLV2 {

namespace grounder {

SumCombsChainGenerator::SumCombsChainGenerator( AggregateFunction agf ) :
		CombsGenerator( agf ), headChain( 0 ){

}

SumCombsChainGenerator::~SumCombsChainGenerator(){
	for ( auto & generator : chain ){
		delete generator.second;
	}
}
void SumCombsChainGenerator::addTerm( Term* term ){
	//the #sum aggregate ignores the others term type
	if( term->getType() != TermType::NUMERIC_CONSTANT )
		return;
	int constantValue = term->getConstantValue();
	if( chain.count( constantValue ) ){
		chain[constantValue]->newOccurrencyFound();
	} else{
		chain[constantValue] = new ChainElementPrimeCombsGenerator( term );
	}
}
Term* SumCombsChainGenerator::getCurrentEvaluation(){
	Term* evaluation = TermTable::getInstance()->term_zero;
	Term * term;
	for ( auto & generator : chain ){
		term = new NumericConstantTerm( false, generator.second->getActualValue() );
		TermTable::getInstance()->addTerm( term );
		evaluation = evaluation->sum( term );
	}
	return evaluation;
}
/*
 * Given that the same value can obtained by summing different smaller values,
 * in order to avoid the generation of the same sum many times, this method
 * apply a reduction of the elements that are divisible into the chain.
 * In particular, if a value A is divisible by a value B, the generator of B
 * absorbs all partial sums of A, given that a possible value of A can be
 * obtained by multiplying B for a value K.
 */
void SumCombsChainGenerator::chainReduction(){
	map< int, ChainElementPrimeCombsGenerator* >::const_iterator smallerElement = chain.begin();
	map< int, ChainElementPrimeCombsGenerator* >::const_iterator greatherElemet = chain.begin();
	while ( smallerElement != chain.end() ){
		greatherElemet = smallerElement;
		greatherElemet++;
		while ( greatherElemet != chain.end() ){
			if( smallerElement->second->absorbGenerator( greatherElemet->second ) ){
				delete greatherElemet->second;
				greatherElemet = chain.erase( greatherElemet );
			} else{
				greatherElemet++;
			}
		}
		smallerElement++;
	}
	auto &headChainGenerator = chain.begin()->second;
	this->headChain = chain.begin()->first;
	for ( auto &chainElement : chain ){
		if( headChainGenerator != chainElement.second ){
			chainElement.second->setNextInChain( headChainGenerator );
			headChainGenerator = chainElement.second;
			this->headChain = chainElement.first;
		}
	}
}
/*
 * Request a new combination to the head of the chain.
 * Return false if there is no other combination to generate
 */
bool SumCombsChainGenerator::nextCombination(){
	if( chain.empty() ){
		//all 0 occurrence combination
		if( firstCombination ){
			firstCombination = false;
			return true;
		}
		return false;
	}
	//all 0 occurrence combination
	if( firstCombination ){
		chainReduction();
		firstCombination = false;
		return true;
	}
	if( finish )
		return false;
	finish = !chain[headChain]->nextCombination();
	return true;
}
CombsGenerator* SumCombsChainGenerator::clone(){
	SumCombsChainGenerator* toReturn = new SumCombsChainGenerator( firstCombination, finish, aggregateFunction, headChain );
	if( !chain.empty() ){
	vector< ChainElementPrimeCombsGenerator* > clones;
	clones.reserve( chain.size() );
	clones.push_back( (ChainElementPrimeCombsGenerator*) (chain[headChain]->clone()) );
	ChainElementCombsGenerator* nextGenerator = chain[headChain]->getNextGenerator();
	while ( nextGenerator != nullptr ){
		clones.push_back( (ChainElementPrimeCombsGenerator*) nextGenerator->clone() );
		nextGenerator = nextGenerator->getNextGenerator();
	}
	for ( unsigned i = 0; i < clones.size(); i++ ){
		ChainElementPrimeCombsGenerator* c = clones[i];
		if( i < clones.size() - 1 )
			c->setNextInChain( clones[i + 1] );
		toReturn->addElementInChain( c->getTerm()->getConstantValue(), c );
		}
	}
	return toReturn;
}
}
}
