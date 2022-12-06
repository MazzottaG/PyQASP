/*
 * InsNthListExtAtom.h
 *
 *  Created on: 30 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_INSNTHLISTEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_INSNTHLISTEXTATOM_H_
#include "ListExtAtom.h"
#include "../../term/ListTerm.h"

namespace DLV2 {

namespace grounder {
/*
 * InsNthListExtAtom class matches the atom &insNth(List,Elem,Pos;List2) that inserts the Elem at position
 * Pos in List and returns the list resulting in List2
 */
class InsNthListExtAtom : public ListExtAtom {
	public:

		InsNthListExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ListExtAtom( predicate, terms, separator, naf ){
		}
		InsNthListExtAtom( Predicate* predicate, int separator, bool naf ) :
				ListExtAtom( predicate, separator, naf ){
		}
		virtual ~InsNthListExtAtom(){
		}
		Atom* clone(){

			Atom* clone = new InsNthListExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;

		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}
	protected:
		bool getInputTerm( const unsigned& i, var_assignment& assignment, Term*& t ){
			if( i == 1 )
				return getInputElem( assignment, t );

			if( i == 2 )
				return (getInputNumericTerm( assignment, t ) && isValidPosition( t->getConstantValue() ));

			return ListExtAtom::getInputTerm( i, assignment, t );
		}
		virtual Term* getResultTerm(){
			vector< Term* > resultList = inputTerms[0]->getTerms();
			resultList.insert( resultList.begin() + (inputTerms[2]->getConstantValue() - 1), inputTerms[1] );
			Term * resultTerm = new ListTerm( false, false, resultList );
			termTable->addTerm( resultTerm );
			return resultTerm;
		}
	private:
		static const unsigned numberOfInputTerms = 3;

		///check if position is a valid position of list
		inline bool isValidPosition( const unsigned int& position ){
			return position > 0 && position - 1 <= inputTerms[0]->getTermsSize();
		}
};

}/*namespace grounder*/

} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_INSNTHLISTEXTATOM_H_ */
