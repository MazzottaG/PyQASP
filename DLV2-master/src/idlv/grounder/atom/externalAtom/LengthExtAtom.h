/*
 * LenghtExtAtom.h
 *
 *  Created on: 15 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_LENGTHEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_LENGTHEXTATOM_H_
#include "LengthStringExtAtom.h"

namespace DLV2 {
namespace grounder {
/*
 *
 */
class LengthExtAtom : public LengthStringExtAtom {
	public:

		LengthExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				LengthStringExtAtom( predicate, terms, separator, naf ){
		}
		LengthExtAtom( Predicate* predicate, int separator, bool naf ) :
				LengthStringExtAtom( predicate, separator, naf ){
		}
		virtual ~LengthExtAtom(){
		}
		Atom* clone(){

			Atom* clone = new LengthExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;

		}

	protected:
		virtual void getLengthString(){
			lenghtString = inputString.length();
			if( inputString[0] == '"' && inputString[lenghtString - 1] == '"' )
				lenghtString -= 2;
		}
};
}
}

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_LENGTHEXTATOM_H_ */
