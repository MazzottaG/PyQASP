/*
 * CombsChainGenerator.h
 *
 *  Created on: 02 lug 2019
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_AGGREGATEUTILITY_COMBSGENERATOR_H_
#define SRC_GROUNDER_ATOM_AGGREGATEUTILITY_COMBSGENERATOR_H_
#include "../../term/Term.h"
#include "../Atom.h"
#include "ChainElementCombsGenerator.h"
#include <map>
using namespace std;
namespace DLV2 {

namespace grounder {
/*
 * This class define the interface for all generators of combinations for Undefined Aggregate Atoms.
 * An undefined aggregate atom contains a set of Aggregate Terms that could be true or false,
 * so the role of generators is to create all possible truth value combinations of that terms
 * in an efficient way based on aggregate type.
 */
class CombsGenerator {
	public:
		virtual ~CombsGenerator(){
		}
		/// Add the term to the generator.
		virtual void addTerm( Term* term )=0;
		/// Request a new combination
		virtual bool nextCombination()=0;
		/// Return the current evaluation of the aggregate based on the current truth values combination
		virtual Term* getCurrentEvaluation()=0;
		// Return the number of terms into the generator
		virtual unsigned size()=0;
		// Factory Method that return the generator for the Aggregate Function given as parameter
		static CombsGenerator* makeGenerator( AggregateFunction agF );
		virtual CombsGenerator* clone()=0;
	protected:
		CombsGenerator() :
				firstCombination( true ), finish( false ), aggregateFunction( AggregateFunction::NONE ){
		}
		CombsGenerator( AggregateFunction agf ) :
				firstCombination( true ), finish( false ), aggregateFunction( agf ){
		}
		CombsGenerator( bool firstCombination, bool finish, AggregateFunction agf ) :
				firstCombination( firstCombination ), finish( finish ), aggregateFunction( agf ){
		}
		bool firstCombination;
		bool finish;
		AggregateFunction aggregateFunction;

};

/*
 * This Generator return all possible value that the Count Aggregate Atom can assume
 * based on the Aggregate terms that it contains.
 * Given the Count property, this generator return all possible values between 0 and SUM_MAX_OCCURRENCY,
 * where SUM_MAX_OCCURRENCY is obtained first taking uniquely each aggregate term with the times in
 * which it could be true and after summing the occurrences of all aggregate terms.
 */
class CountCombsGenerator : public CombsGenerator {
	public:
		CountCombsGenerator( AggregateFunction agF ) :
				CombsGenerator( agF ), maxCount( 0 ), currentCount( TermTable::getInstance()->term_zero ){
		}
		virtual ~CountCombsGenerator(){
		}
		/// Add the term to the generator.
		virtual void addTerm( Term* term );
		/// Request a new combination
		virtual bool nextCombination();
		/// Return the current evaluation of the aggregate based on the current truth values combination
		virtual Term* getCurrentEvaluation();
		// Return the number of terms into the generator
		virtual unsigned size(){
			return this->uniqueTerms.size();
		}
		virtual CombsGenerator* clone(){
			return new CountCombsGenerator( firstCombination, finish, aggregateFunction, uniqueTerms, maxCount, currentCount );
		}

	protected:
		CountCombsGenerator( bool firstCombination, bool finish, AggregateFunction agF, map_term< unsigned > uTerms, int maxCount, Term*currentCount ) :
				CombsGenerator( firstCombination, finish, agF ), uniqueTerms( move( uTerms ) ), maxCount( maxCount ), currentCount( currentCount ){
		}
	private:
		map_term< unsigned > uniqueTerms;
		int maxCount;
		Term* currentCount;
};

class SumCombsGenerator : public CombsGenerator {
	public:
		SumCombsGenerator( AggregateFunction agF ) :
				CombsGenerator( agF ), partialSums( { 0 } ){
		}
		virtual ~SumCombsGenerator(){

		}
		/// Add the term to the generator.
		virtual void addTerm( Term* term );
		/// Request a new combination
		virtual bool nextCombination();
		/// Return the current evaluation of the aggregate based on the current truth values combination
		virtual Term* getCurrentEvaluation();
		// Return the number of terms into the generator
		virtual unsigned size(){
			return this->partialSums.size();
		}
		virtual CombsGenerator* clone(){
			return new SumCombsGenerator( firstCombination, finish, aggregateFunction, partialSums, actualValue );
		}
	protected:
		SumCombsGenerator( bool firstCombination, bool finish, AggregateFunction agF, set< int > partialSums, set< int >::iterator actualValue ) :
				CombsGenerator( firstCombination, finish, agF ), partialSums( move( partialSums ) ), actualValue( actualValue ){
		}
	private:
		set< int > partialSums;
		set< int >::iterator actualValue;
};

/*
 * This Generator return all possible value that the Min or Max Aggregate Atoms can assume
 * based on the Aggregate terms that they contains.
 * Given the Min and Max properties, this generator return all values corresponding to
 * the unique aggregate terms that they contains
 */
class MinMax_CombsGenerator : public CombsGenerator {
	public:
		MinMax_CombsGenerator( AggregateFunction agf ) :
				CombsGenerator( agf ){

		}
		virtual ~MinMax_CombsGenerator();
		/// Add the term to the chain and create a generator for it.
		virtual void addTerm( Term* term );
		/// Request a new combination to the head of the chain
		virtual bool nextCombination();
		/// Return the current evaluation of the aggregate corresponding to the generator
		virtual Term* getCurrentEvaluation();
		virtual CombsGenerator* clone(){
			return new MinMax_CombsGenerator( firstCombination, finish, aggregateFunction, uniqueTerms, iterator );
		}
		virtual unsigned size(){
			return this->uniqueTerms.size();
		}
	protected:
		MinMax_CombsGenerator( bool firstCombination, bool finish, AggregateFunction agF, set_term uniqueTerms, set_term::iterator iterator ) :
				CombsGenerator( firstCombination, finish, agF ), uniqueTerms( move( uniqueTerms ) ), iterator( iterator ){
		}
	private:
		set_term uniqueTerms;
		set_term::iterator iterator;
};

/*
 * This class implements a Chain of Responsibility pattern to generate all
 * possible truth values of terms in the chain.
 * A term could be true also more than one time so a combination is formed by
 * all terms in the chain with the times in which they are considered true.
 * The chain compute the combination in a bottom-up way for this reason the
 * first term added to the chain is the TAIL while the last one is the HEAD.
 * A request for a new combination is made to the Head and if necessary it
 * forwards the request to its successor in the computation and so on for
 * each element in the chain.
 */
class AllCombinationsChainGenerator : public CombsGenerator {
	public:
		AllCombinationsChainGenerator( AggregateFunction agf );
		virtual ~AllCombinationsChainGenerator();
		AllCombinationsChainGenerator& operator=( const AllCombinationsChainGenerator& other );
		/// Add the term to the chain and create a generator for it.
		void addTerm( Term* term );
		/// Request a new combination to the head of the chain
		bool nextCombination();
		void print( ostream& stream = cout );
		void printChain( ostream& stream = cout );
		/// return the current combination
		map_term< ChainElementCombsGenerator* >& getCombination(){
			return this->chain;
		}
		virtual CombsGenerator* clone();
		unsigned size(){
			return this->chain.size();
		}
		/// Return the current evaluation of the aggregate corresponding to the generator
		virtual Term* getCurrentEvaluation();
	protected:
		AllCombinationsChainGenerator( bool firstCombination, bool finish, AggregateFunction aggregateFunction, Term* headChain ) :
				CombsGenerator( firstCombination, finish, aggregateFunction ), headChain( headChain ){

		}
		void addElementInChain( Term * t, ChainElementCombsGenerator* element ){
			chain[t] = element;
		}
	private:
		map_term< ChainElementCombsGenerator* > chain;
		Term* headChain;
};
/*
 * This Generator return all possible value that the Sum Aggregate Atom can assume
 * based on the Aggregate terms that they contains and the times in which they could
 * be true.
 * This class implements a Chain of Responsibility pattern to generate all possible
 * sum values of terms in the chain. Each element into the chain contains a set of
 * partial sums based on the term to which it refers, when a next combination is required,
 * the request is forward to the elements into the chain in order to compute a new
 * sequence of partial sums.The summation of all partial sums are the actual evaluation
 * of the aggregate
 *
 */
class SumCombsChainGenerator : public CombsGenerator {
	public:
		SumCombsChainGenerator( AggregateFunction agf );
		virtual ~SumCombsChainGenerator();
		SumCombsChainGenerator& operator=( const SumCombsChainGenerator& other );
		/// Add the term to the chain and create a generator for it.
		void addTerm( Term* term );
		/// Request a new combination to the head of the chain
		bool nextCombination();
		/// return the current combination
		map< int, ChainElementPrimeCombsGenerator* >& getCombination(){
			return this->chain;
		}
		virtual CombsGenerator* clone();
		unsigned size(){
			return this->chain.size();
		}
		/// Return the current evaluation of the aggregate corresponding to the generator
		virtual Term* getCurrentEvaluation();
	protected:
		SumCombsChainGenerator( bool firstCombination, bool finish, AggregateFunction aggregateFunction, int headChain ) :
				CombsGenerator( firstCombination, finish, aggregateFunction ), headChain( headChain ){

		}
		void addElementInChain( int value, ChainElementPrimeCombsGenerator* element ){
			chain[value] = element;
		}
	private:
		map< int, ChainElementPrimeCombsGenerator* > chain;
		int headChain;
		void chainReduction();
};

}
}
#endif /* SRC_GROUNDER_ATOM_AGGREGATEUTILITY_COMBSGENERATOR_H_ */
