/*
 * SimpleReverseListExtAtom.h
 *
 *  Created on: 01 giu 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_SIMPLEREVERSELISTEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_SIMPLEREVERSELISTEXTATOM_H_
#include "ListExtAtom.h"
#include "../../term/ListTerm.h"

namespace DLV2 {

namespace grounder {
/*
 * The ReverseListExtAtom class matches the atom &reverse(List1; List2) where List2
 * is a list of terms obtained by reversing terms in List1.
 */
class SimpleReverseListExtAtom : public ListExtAtom {
	public:

		SimpleReverseListExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ListExtAtom( predicate, terms, separator, naf ){
		}
		SimpleReverseListExtAtom( Predicate* predicate, int separator, bool naf ) :
				ListExtAtom( predicate, separator, naf ){
		}
		virtual ~SimpleReverseListExtAtom(){
		}
		Atom* clone(){

			Atom* clone = new SimpleReverseListExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;

		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}
	protected:
		virtual Term* getResultTerm(){
			Term* t = inputTerms[0];
			Term * resultTerm = new ListTerm( false, false, t->getTerms().rbegin(), t->getTerms().rend() );
			termTable->addTerm( resultTerm );
			return resultTerm;
		}
	private:
		static const unsigned numberOfInputTerms = 1;
};

}/*namespace grounder*/

} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_SIMPLEREVERSELISTEXTATOM_H_ */
