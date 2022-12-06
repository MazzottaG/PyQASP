/*
 * MemberNthListExtAtom.h
 *
 *  Created on: 29 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_MEMBERNTHLISTEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_MEMBERNTHLISTEXTATOM_H_

#include "ListExtAtom.h"

namespace DLV2 {

namespace grounder {
/*
 * MemberNthListExtAtom class matches the atom &memberNth(List,Pos;Elem) that returns the term
 * at position Pos in List.
 */
class MemberNthListExtAtom : public ListExtAtom {
	public:

		MemberNthListExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ListExtAtom( predicate, terms, separator, naf ){
		}
		MemberNthListExtAtom( Predicate* predicate, int separator, bool naf ) :
				ListExtAtom( predicate, separator, naf ){
		}
		virtual ~MemberNthListExtAtom(){
		}
		Atom* clone(){

			Atom* clone = new MemberNthListExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;

		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}
	protected:
		bool getInputTerm( const unsigned& i, var_assignment& assignment, Term*& t ){
			if( i == 1 )
				return (getInputNumericTerm( assignment, t ) && isValidPosition( t->getConstantValue() ));
			return ListExtAtom::getInputTerm( i, assignment, t );
		}
		virtual Term* getResultTerm(){
			return inputTerms[0]->getTerm( inputTerms[1]->getConstantValue() - 1 );
		}
	private:
		static const unsigned numberOfInputTerms = 2;

		///check if position is a valid position of list
		inline bool isValidPosition( const unsigned int& position ){
			return position > 0 && position <= inputTerms[0]->getTermsSize();
		}
};

}/*namespace grounder*/

} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_MEMBERNTHLISTEXTATOM_H_ */
