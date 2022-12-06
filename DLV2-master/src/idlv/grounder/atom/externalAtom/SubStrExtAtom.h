/*
 * SubStrExtAtom.h
 *
 *  Created on: 05 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_SUBSTREXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_SUBSTREXTATOM_H_
#include "BaseSubStrExtAtom.h"

namespace DLV2 {
namespace grounder {

/*
 * SubStrExtAtom implements the atom &sub_str(X,Y,Z;W) but this class does not handle strings that contain
 * special characters. The generated result might be wrong if the string contains it.
 */
class SubStrExtAtom : public BaseSubStrExtAtom {
	public:

		SubStrExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				BaseSubStrExtAtom( predicate, terms, separator, naf ){
		}
		SubStrExtAtom( Predicate* predicate, int separator, bool naf ) :
				BaseSubStrExtAtom( predicate, separator, naf ){
		}
		virtual ~SubStrExtAtom(){
		}
		Atom* clone(){

			Atom* clone = new SubStrExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;

		}

	protected:
		virtual bool getOutputString();

};

}/* grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_SUBSTREXTATOM_H_ */
