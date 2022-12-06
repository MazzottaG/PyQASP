/*
 * SubStrExtAtomUTF16.h
 *
 *  Created on: 10 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_SUBSTREXTATOMUTF16_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_SUBSTREXTATOMUTF16_H_
#include "BaseSubStrExtAtom.h"

namespace DLV2 {
namespace grounder {

/*
 * SubStrExtAtomUTF16 implements the atom &sub_str(X,Y,Z;W) and this class also handles strings that contain special characters
 */
class SubStrExtAtomUTF16 : public BaseSubStrExtAtom {
	public:

		SubStrExtAtomUTF16( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				BaseSubStrExtAtom( predicate, terms, separator, naf ){
		}
		SubStrExtAtomUTF16( Predicate* predicate, int separator, bool naf ) :
				BaseSubStrExtAtom( predicate, separator, naf ){
		}
		virtual ~SubStrExtAtomUTF16(){
		}
		Atom* clone(){

			Atom* clone = new SubStrExtAtomUTF16( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;

		}

	protected:
		virtual bool getOutputString();

};

}/* grounder */

} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_SUBSTREXTATOMUTF16_H_ */
