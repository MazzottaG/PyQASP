/*
 * CombinationChainGenerator.cpp
 *
 *  Created on: 01 giu 2019
 *      Author: Elena Mastria
 */

#include "CombsGenerator.h"
namespace DLV2 {

namespace grounder {
AllCombinationsChainGenerator::AllCombinationsChainGenerator( AggregateFunction agf ) :
		CombsGenerator( agf ), headChain( nullptr ){

}

AllCombinationsChainGenerator::~AllCombinationsChainGenerator(){
	for ( auto & generator : chain ){
		delete generator.second;
	}
}

/*
 * Add the term to the chain and create a generator for it.
 * If the term already exists in the chain then it increases
 * the number of times in which it could be true
 */
void AllCombinationsChainGenerator::addTerm( Term* term ){
//	cout << "add term";	term->print();	cout << endl;
	if( chain.count( term ) ){
		chain[term]->newOccurrencyFound();
	} else{
		chain[term] = headChain != nullptr ? new ChainElementAllCombsGenerator( term, chain[headChain] ) : new ChainElementAllCombsGenerator( term );
		headChain = term;
	}
	
}
/*
 * Request a new combination to the head of the chain.
 * Return false if there is no other combination to generate
 */
bool AllCombinationsChainGenerator::nextCombination(){

	//all 0 occurrence combination
	if( firstCombination ){
		firstCombination = false;
		return true;
	}
	if( headChain == nullptr )
		return false;
	if( finish )
		return false;
	finish = !chain[headChain]->nextCombination();
	return true;
}
void AllCombinationsChainGenerator::print( ostream& stream ){
	chain[headChain]->print( stream );
	stream << endl;
}
void AllCombinationsChainGenerator::printChain( ostream& stream ){
	chain[headChain]->printTerm( stream );
	stream << endl;

}
AllCombinationsChainGenerator& AllCombinationsChainGenerator::operator =( const AllCombinationsChainGenerator& other ){
	if( this != &other ){
		this->headChain = other.headChain;
		this->finish = other.finish;
		this->firstCombination = other.finish;
		this->chain = other.chain;
	}
	return *this;
}
Term* AllCombinationsChainGenerator::getCurrentEvaluation(){
	Term* evaluation;
	Term* term;
	if( aggregateFunction == AggregateFunction::MIN )
		evaluation = TermTable::getInstance()->term_max;
	else if( aggregateFunction == MAX )
		evaluation = TermTable::getInstance()->term_min;
	else
		evaluation = TermTable::getInstance()->term_zero;
	for ( auto & generator : chain ){
		Term* x = generator.first;
			switch ( aggregateFunction ){
				case MIN:
					if( *evaluation > *x )
						evaluation = x;
				break;
				case MAX:
					if( *evaluation < *x )
						evaluation = x;
				break;
				case COUNT:
				term = new NumericConstantTerm( false, generator.second->getActualValue() );
				TermTable::getInstance()->addTerm( term );
				evaluation = evaluation->sum( term );
				break;
				default:
				if( evaluation->getType() == NUMERIC_CONSTANT && x->getType() == NUMERIC_CONSTANT ){
					term = new NumericConstantTerm( false, generator.second->getActualValue() * x->getConstantValue() );
					TermTable::getInstance()->addTerm( term );
					evaluation = evaluation->sum( term );
				}
				break;
			}
	}
	return evaluation;
}
CombsGenerator* AllCombinationsChainGenerator::clone(){
	AllCombinationsChainGenerator* toReturn = new AllCombinationsChainGenerator( firstCombination, finish, aggregateFunction, headChain );
	if( !chain.empty() ){
	vector< ChainElementCombsGenerator* > clones;
	clones.reserve( chain.size() );
	clones.push_back( chain[headChain]->clone() );
	ChainElementCombsGenerator* nextGenerator = chain[headChain]->getNextGenerator();
	while ( nextGenerator != nullptr ){
		clones.push_back( nextGenerator->clone() );
		nextGenerator = nextGenerator->getNextGenerator();
	}
	for ( unsigned i = 0; i < clones.size(); i++ ){
		ChainElementCombsGenerator* c = clones[i];
		if( i < clones.size() - 1 )
			c->setNextInChain( clones[i + 1] );
		toReturn->addElementInChain( c->getTerm(), c );
		}
	}
	return toReturn;
}
}
}


