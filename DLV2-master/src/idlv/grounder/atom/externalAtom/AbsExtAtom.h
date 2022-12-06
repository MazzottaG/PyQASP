/*
 * AbsExtAtom.h
 *
 *  Created on: 30 apr 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_ABSEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_ABSEXTATOM_H_
#include <stdlib.h>

#include "ArithmeticExtAtom.h"

namespace DLV2 {
namespace grounder {

/*
 *	&abs(X;Z) returns the absolute value of X
 */
class AbsExtAtom : public ArithmeticExtAtom {
	public:

		AbsExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ArithmeticExtAtom( predicate, terms, separator, naf ){
		}
		AbsExtAtom( Predicate* predicate, int separator, bool naf ) :
				ArithmeticExtAtom( predicate, separator, naf ){
		}
		virtual ~AbsExtAtom(){
		}
		Atom* clone(){

			Atom* clone = new AbsExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;

		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}

	protected:
		virtual bool getResult(){
			result = abs( inputTermsValues[0] );
			return true;
		}
	private:
		static const unsigned numberOfInputTerms = 1;

};

}/* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_ABSEXTATOM_H_ */
