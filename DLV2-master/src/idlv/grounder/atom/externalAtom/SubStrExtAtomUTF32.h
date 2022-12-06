/*
 * SubStrExtAtomUTF32.h
 *
 *  Created on: 10 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_SUBSTREXTATOMUTF32_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_SUBSTREXTATOMUTF32_H_
#include "../ExtAtom.h"

namespace DLV2 {
namespace grounder {

/*
 * &sub_str(X,Y,Z;W) generates a quoted substring W of Z from the first index X to the second index Y.
 * X and Y must be valid positions in Z such that X<=Y
 */
class SubStrExtAtomUTF32 : public ExtAtom {
	public:

		SubStrExtAtomUTF32( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ExtAtom( predicate, terms, separator, naf ), firstIndex( 0 ), secondIndex( 0 ){
		}
		SubStrExtAtomUTF32( Predicate* predicate, int separator, bool naf ) :
				ExtAtom( predicate, separator, naf ), firstIndex( 0 ), secondIndex( 0 ){
		}
		virtual ~SubStrExtAtomUTF32(){
		}
		Atom* clone(){

			Atom* clone = new SubStrExtAtomUTF32( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;

		}

	protected:
		virtual bool firstMatch( var_assignment& assignment );
		virtual bool nextMatch( var_assignment& assignment );
	private:
		int firstIndex;
		int secondIndex;
		string inputString;
};

}/* grounder */

} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_SUBSTREXTATOMUTF32_H_ */
