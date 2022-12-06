/*
 * BaseSubStrExtAtom.h
 *
 *  Created on: 27 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_BASESUBSTREXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_BASESUBSTREXTATOM_H_
#include "../ExtAtom.h"

namespace DLV2 {
namespace grounder {

/*
 * BaseSubStrExtAtom is a base class for the implementations of external atom  &sub_str(X,Y,Z;W).
 * &sub_str(X,Y,Z;W) generates a quoted substring W of Z from the first index X to the second index Y.
 * X and Y must be valid positions in Z such that X<=Y
 */
class BaseSubStrExtAtom : public ExtAtom {
	public:

		BaseSubStrExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ExtAtom( predicate, terms, separator, naf ), firstIndex( 0 ), secondIndex( 0 ){
		}
		BaseSubStrExtAtom( Predicate* predicate, int separator, bool naf ) :
				ExtAtom( predicate, separator, naf ), firstIndex( 0 ), secondIndex( 0 ){
		}
		virtual ~BaseSubStrExtAtom(){
		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}
	protected:
		virtual bool firstMatch( var_assignment& assignment );
		virtual bool nextMatch( var_assignment& assignment );
		///This method creates a substring from first index to second index
		virtual bool getOutputString()=0;
		int firstIndex;
		int secondIndex;
		string inputString;
	private:
		static const unsigned numberOfInputTerms = 3;

};

}/* grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_BASESUBSTREXTATOM_H_ */
