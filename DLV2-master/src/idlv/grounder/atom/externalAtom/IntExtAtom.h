/*
 * IntExtAtom.h
 *
 *  Created on: 29 apr 2017
 *      Author: Elena Mastria
 */

#ifndef INTEXTATOM_H_
#define INTEXTATOM_H_
/*
 * &int(X,Y;Z) generates all integers Z such that X <= Z <= Y.
 */
#include "ArithmeticExtAtom.h"
using namespace std;

namespace DLV2 {
namespace grounder {

class IntExtAtom : public ArithmeticExtAtom {
	public:
		IntExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ArithmeticExtAtom( predicate, terms, separator, naf ){
		}
		IntExtAtom( Predicate* predicate, int separator, bool naf ) :
				ArithmeticExtAtom( predicate, separator, naf ){
		}
		virtual ~IntExtAtom(){
		}
		Atom* clone(){

			Atom* clone = new IntExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;

		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}

	protected:
		virtual bool nextMatch( var_assignment& assignment );
		virtual bool getResult(){
			int lowerBound = inputTermsValues[0];
			if( inputTermsValues[1] < lowerBound )
				return false;
			result = lowerBound;

			return true;
		}
		inline virtual bool compareOutputValue( const int & value ){
			return inputTermsValues[0] <= value && value <= inputTermsValues[1];
		}
	private:
		static const unsigned numberOfInputTerms = 2;

};

}/* namespace grounder */
} /* namespace DLV2 */

#endif /* INTEXTATOM_H_ */
