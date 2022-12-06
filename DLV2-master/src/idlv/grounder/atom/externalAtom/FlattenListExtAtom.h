/*
 * FlattenListExtAtom.h
 *
 *  Created on: 30 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_FLATTENLISTEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_FLATTENLISTEXTATOM_H_

#include "ListExtAtom.h"
#include "../../term/ListTerm.h"

namespace DLV2 {

namespace grounder {
/*
 * FlattenListExtAtom class matches the atom &flatten(List1;List2)
 * that if List1 contain other lists it returns in List2 a list of
 * only individual terms obtained flattening all nested lists
 */
class FlattenListExtAtom : public ListExtAtom {
	public:

		FlattenListExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ListExtAtom( predicate, terms, separator, naf ){
		}
		FlattenListExtAtom( Predicate* predicate, int separator, bool naf ) :
				ListExtAtom( predicate, separator, naf ){
		}
		virtual ~FlattenListExtAtom(){
		}
		Atom* clone(){
			Atom* clone = new FlattenListExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;
		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}
	protected:
		virtual Term* getResultTerm(){
			vector< Term* > listTerms;
			inputTerms[0]->flatten( listTerms );
			Term* outputTerm = new ListTerm( false, false, listTerms );
			termTable->addTerm( outputTerm );
			return outputTerm;
		}
	private:
		static const unsigned numberOfInputTerms = 1;

}
;

}/*namespace grounder*/
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_FLATTENLISTEXTATOM_H_ */
