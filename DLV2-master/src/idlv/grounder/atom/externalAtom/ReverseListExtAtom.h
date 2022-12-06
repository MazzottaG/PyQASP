/*
 * ReverseListExtAtom.h
 *
 *  Created on: 21 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_REVERSELISTEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_REVERSELISTEXTATOM_H_
#include "ListExtAtom.h"

namespace DLV2 {

namespace grounder {
/*
 * The ReverseListExtAtom class matches the atom &reverse_r(List1; List2) where List2
 * is a list of terms obtained by reversing terms in List1 and in all its nested lists.
 */
class ReverseListExtAtom : public ListExtAtom {
	public:

		ReverseListExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ListExtAtom( predicate, terms, separator, naf ){
		}
		ReverseListExtAtom( Predicate* predicate, int separator, bool naf ) :
				ListExtAtom( predicate, separator, naf ){
		}
		virtual ~ReverseListExtAtom(){
		}
		Atom* clone(){

			Atom* clone = new ReverseListExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;

		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}
	protected:
		virtual Term* getResultTerm(){
			return inputTerms[0]->getReverseTerms();
		}
	private:
		static const unsigned numberOfInputTerms = 1;
};

}/*namespace grounder*/

} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_REVERSELISTEXTATOM_H_ */
