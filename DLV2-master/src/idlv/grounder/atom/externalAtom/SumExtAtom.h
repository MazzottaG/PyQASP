/*
 * SumExtAtom.h
 *
 *  Created on: 19 apr 2017
 *      Author: Elena Mastria
 */

#ifndef SUMEXTATOM_H_
#define SUMEXTATOM_H_

/*
 *
 */
#include "ArithmeticExtAtom.h"
using namespace std;

namespace DLV2 {
namespace grounder {

class SumExtAtom : public ArithmeticExtAtom {
	public:
		SumExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ArithmeticExtAtom( predicate, terms, separator, naf ){
		}
		SumExtAtom( Predicate* predicate, int separator, bool naf ) :
				ArithmeticExtAtom( predicate, separator, naf ){
		}
		virtual ~SumExtAtom(){
		}

		Atom* clone(){

			Atom* clone = new SumExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;

		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}
	protected:
		virtual bool getResult(){
			result = 0;
			for ( int i : inputTermsValues ){
				result += i;
			}
			return true;
		}
	private:
		static const unsigned numberOfInputTerms = 2;
};

}
}
#endif /* SUMEXTATOM_H_ */
