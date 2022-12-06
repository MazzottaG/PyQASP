/*
 * LastListExtAtom.h
 *
 *  Created on: 30 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_LASTLISTEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_LASTLISTEXTATOM_H_

#include "ListExtAtom.h"

namespace DLV2 {

namespace grounder {
/*
 * LastListExtAtom class matches the atom &last(List;Elem) that returns the last term in List.
 */
class LastListExtAtom : public ListExtAtom {
	public:

		LastListExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ListExtAtom( predicate, terms, separator, naf ){
		}
		LastListExtAtom( Predicate* predicate, int separator, bool naf ) :
				ListExtAtom( predicate, separator, naf ){
		}
		virtual ~LastListExtAtom(){
		}
		Atom* clone(){
			Atom* clone = new LastListExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;
		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}
	protected:
		virtual Term* getResultTerm(){
			Term * t = inputTerms[0];
			return t->getTerm( t->getTermsSize() - 1 );
		}
	private:
		static const unsigned numberOfInputTerms = 1;
};

}/*namespace grounder*/
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_LASTLISTEXTATOM_H_ */
