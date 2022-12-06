/*
 * ChainElementCombsGenerator.h
 *
 *  Created on: 03 lug 2019
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_AGGREGATEUTILITY_CHAINELEMENTCOMBSGENERATOR_H_
#define SRC_GROUNDER_ATOM_AGGREGATEUTILITY_CHAINELEMENTCOMBSGENERATOR_H_

#include "../../term/Term.h"
#include "../../table/TermTable.h"
#include <algorithm>
namespace DLV2 {

namespace grounder {

/*
 * This class implements the be behavior of a single element in the chain of generators.
 */
class ChainElementCombsGenerator {
	public:
		ChainElementCombsGenerator( Term* term, ChainElementCombsGenerator* generator ) :
				term( term ), nextGenerator( generator ), finish( false ){
		}
		ChainElementCombsGenerator( Term* term, bool finish ) :
				term( term ), nextGenerator( 0 ), finish( finish ){
		}
		ChainElementCombsGenerator( Term * term ) :
				term( term ), nextGenerator( 0 ), finish( false ){

		}
		ChainElementCombsGenerator( const ChainElementCombsGenerator&o ) :
				term( o.term ), nextGenerator( o.nextGenerator ), finish( o.finish ){
		}
		ChainElementCombsGenerator() :
				term( 0 ), nextGenerator( 0 ), finish( false ){
		}
		ChainElementCombsGenerator& operator=( const ChainElementCombsGenerator& other );
		virtual ~ChainElementCombsGenerator(){
		}
		/// Compute the next value of combination, if no other combination can be generated then return false
		virtual bool nextCombination()=0;
		virtual void newOccurrencyFound()=0;
		virtual int getActualValue() =0;
		virtual void print( ostream& stream = cout ) const=0;
		virtual void printTerm( ostream& stream = cout ) const=0;
		virtual ChainElementCombsGenerator* clone()=0;
		ChainElementCombsGenerator* getNextGenerator(){
			return nextGenerator;
		}
		Term * getTerm(){
			return term;
		}
		void setNextInChain( ChainElementCombsGenerator*ng ){
			nextGenerator = ng;
		}


	protected:
		/// only for information, the term related to this generator
		Term *term;
		/// the successor generator in the chain, nullptr if this generator is a Tail
		ChainElementCombsGenerator* nextGenerator;

		/// true if the successor has nothing left to generate
		bool finish;

};

class ChainElementAllCombsGenerator : public ChainElementCombsGenerator {
	public:
		ChainElementAllCombsGenerator( Term* term, ChainElementCombsGenerator* generator ) :
				ChainElementCombsGenerator( term, generator ), maxOccurrency( 1 ), actualValue( 0 ){
		}
		ChainElementAllCombsGenerator( Term* term, bool finish, unsigned maxOccurrency, unsigned actualValue ) :
				ChainElementCombsGenerator( term, finish ), maxOccurrency( maxOccurrency ), actualValue( actualValue ){
		}
		ChainElementAllCombsGenerator( Term * term ) :
				ChainElementCombsGenerator( term ), maxOccurrency( 1 ), actualValue( 0 ){

		}
		ChainElementAllCombsGenerator( const ChainElementAllCombsGenerator&o ) :
				ChainElementCombsGenerator( o ), maxOccurrency( 1 ), actualValue( 0 ){
		}
		virtual ~ChainElementAllCombsGenerator(){
		}
		/// Compute the next value of combination, if no other combination can be generated then return false
		virtual bool nextCombination();
		virtual void newOccurrencyFound(){
			maxOccurrency++;
		}
		virtual int getActualValue(){
			return this->actualValue;
		}
		virtual void print( ostream& stream = cout ) const;
		virtual void printTerm( ostream& stream = cout ) const;
		virtual ChainElementCombsGenerator* clone(){
			return new ChainElementAllCombsGenerator( term, finish, maxOccurrency, actualValue );
		}
	private:
		/// the maximum number of times in which the term could be true
		unsigned maxOccurrency;
		/// the actual number of times in which the term is considered true
		unsigned actualValue;
};

class ChainElementPrimeCombsGenerator : public ChainElementCombsGenerator {
	public:
		ChainElementPrimeCombsGenerator( Term* term, ChainElementCombsGenerator* generator ) :
				ChainElementCombsGenerator( term, generator ), partialSums( { 0, term->getConstantValue() } ), actualValue( partialSums.begin() ), firstRequest( true ){
		}
		ChainElementPrimeCombsGenerator( Term * term ) :
				ChainElementCombsGenerator( term ), partialSums( { 0, term->getConstantValue() } ), actualValue( partialSums.begin() ), firstRequest( true ){

		}
		ChainElementPrimeCombsGenerator( const ChainElementPrimeCombsGenerator&o ) :
				ChainElementCombsGenerator( o ), partialSums( o.partialSums ), actualValue( o.actualValue ), firstRequest( o.firstRequest ){
		}
		virtual ~ChainElementPrimeCombsGenerator(){
		}
		/// Compute the next value of combination, if no other combination can be generated then return false
		virtual bool nextCombination();
		virtual void newOccurrencyFound();
		virtual int getActualValue(){
			if( firstRequest ){
				firstRequest = false;
				return 0;
			}
			return *(this->actualValue);
		}
		virtual void print( ostream& stream = cout ) const{
		}
		virtual void printTerm( ostream& stream = cout ) const{
		}
		virtual ChainElementCombsGenerator* clone(){
			return new ChainElementPrimeCombsGenerator( term, finish, partialSums, actualValue, firstRequest );
		}
		bool absorbGenerator( ChainElementPrimeCombsGenerator* genToAssorb );
		set< int >& getPartialSums(){
			return partialSums;
		}
	protected:
		ChainElementPrimeCombsGenerator( Term *term, bool finish, set< int > partialSums, set< int >::iterator actualValue, bool firstRequest ) :
				ChainElementCombsGenerator( term, finish ), partialSums( move( partialSums ) ), actualValue( actualValue ), firstRequest( firstRequest ){

		}
	private:
		set< int > partialSums;
		set< int >::iterator actualValue;
		bool firstRequest;
};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_AGGREGATEUTILITY_CHAINELEMENTCOMBSGENERATOR_H_ */
