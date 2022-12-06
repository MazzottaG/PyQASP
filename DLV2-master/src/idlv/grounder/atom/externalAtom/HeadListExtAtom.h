/*
 * HeadListExtAtom.h
 *
 *  Created on: 30 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_HEADLISTEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_HEADLISTEXTATOM_H_

#include "ListExtAtom.h"

namespace DLV2 {

namespace grounder {
/*
 * HeadListExtAtom class matches the atom &head(List;Elem) that returns the head of List term in input.
 */
class HeadListExtAtom : public ListExtAtom {
	public:

		HeadListExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ListExtAtom( predicate, terms, separator, naf ){
		}
		HeadListExtAtom( Predicate* predicate, int separator, bool naf ) :
				ListExtAtom( predicate, separator, naf ){
		}
		virtual ~HeadListExtAtom(){
		}
		Atom* clone(){
			Atom* clone = new HeadListExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;
		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}
	protected:
		virtual Term* getResultTerm(){
			Term * t = inputTerms[0];
			return t->getTerm( 0 );
		}
	private:
		static const unsigned numberOfInputTerms = 1;

};

}/*namespace grounder*/

} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_HEADLISTEXTATOM_H_ */
