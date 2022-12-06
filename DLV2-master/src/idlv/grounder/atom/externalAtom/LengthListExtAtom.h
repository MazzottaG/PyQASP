/*
 * LengthListExtAtom.h
 *
 *  Created on: 25 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_LENGTHLISTEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_LENGTHLISTEXTATOM_H_

#include "ListExtAtom.h"

namespace DLV2 {
namespace grounder {
/*
 * The LengthListExtAtom class matches the atom &length(List1; X) that returns in X the length of list.
 */
class LengthListExtAtom : public ListExtAtom {
	public:

		LengthListExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ListExtAtom( predicate, terms, separator, naf ){
		}
		LengthListExtAtom( Predicate* predicate, int separator, bool naf ) :
				ListExtAtom( predicate, separator, naf ){
		}
		virtual ~LengthListExtAtom(){
		}
		Atom* clone(){

			Atom* clone = new LengthListExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;

		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}
	protected:
		virtual Term* getResultTerm(){
			int lenghtList = inputTerms[0]->getTerms().size();
			Term * lengthTerm = new NumericConstantTerm( lenghtList < 0, lenghtList );
			termTable->addTerm( lengthTerm );
			return lengthTerm;
		}
	private:
		static const unsigned numberOfInputTerms = 1;

};

}/*namespace grounder*/

} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_LENGTHLISTEXTATOM_H_ */
