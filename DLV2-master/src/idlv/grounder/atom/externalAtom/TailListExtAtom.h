/*
 * TailListExtAtom.h
 *
 *  Created on: 30 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_TAILLISTEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_TAILLISTEXTATOM_H_

#include "ListExtAtom.h"
#include "../../term/ListTerm.h"
namespace DLV2 {

namespace grounder {
/*
 * TailListExtAtom class matches the atom &tail(List1;List2) that deletes the
 * first element of list and returns a list containing the other terms of list.
 */
class TailListExtAtom : public ListExtAtom {
	public:

		TailListExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ListExtAtom( predicate, terms, separator, naf ){
		}
		TailListExtAtom( Predicate* predicate, int separator, bool naf ) :
				ListExtAtom( predicate, separator, naf ){
		}
		virtual ~TailListExtAtom(){
		}
		Atom* clone(){
			Atom* clone = new TailListExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;
		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}
	protected:
		virtual Term* getResultTerm(){
			vector< Term* > list = inputTerms[0]->getTerms();
			if( list.size() > 1 ){
//				Term *tail = new ListTerm( false, false, vector< Term* >( list.begin() + 1, list.end() ) );
				Term *tail = new ListTerm( false, false, list.begin() + 1, list.end() );

				termTable->addTerm( tail );
				return tail;
			} else{
				Term *tailEmpty = new ListTerm( false, false, vector< Term* >() );
				termTable->addTerm( tailEmpty );
				return tailEmpty;
			}
		}
	private:
		static const unsigned numberOfInputTerms = 1;
}
;

}/*namespace grounder*/

} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_TAILLISTEXTATOM_H_ */
