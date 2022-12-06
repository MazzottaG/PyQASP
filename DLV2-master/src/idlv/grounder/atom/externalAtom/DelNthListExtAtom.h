/*
 * DelNthListExtAtom.h
 *
 *  Created on: 26 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_DELNTHLISTEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_DELNTHLISTEXTATOM_H_
#include "ListExtAtom.h"
#include "../../term/ListTerm.h"

namespace DLV2 {
namespace grounder {
/*
 * DelNthListExtAtom class matches the atom &delNth(List,Pos;List2) that deletes the term
 * at position Pos in List and returns the result in List2.
 */
class DelNthListExtAtom : public ListExtAtom {
	public:

		DelNthListExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ListExtAtom( predicate, terms, separator, naf ){
		}
		DelNthListExtAtom( Predicate* predicate, int separator, bool naf ) :
				ListExtAtom( predicate, separator, naf ){
		}
		virtual ~DelNthListExtAtom(){
		}
		Atom* clone(){

			Atom* clone = new DelNthListExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;

		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}
	protected:
		virtual bool getInputTerm( const unsigned& i, var_assignment& assignment, Term*& t ){
			if( i == 1 )
				return (getInputNumericTerm( assignment, t ) && isValidPosition( t->getConstantValue() ));

			return ListExtAtom::getInputTerm( i, assignment, t );
		}
		virtual Term* getResultTerm(){
			vector< Term* > resultList( inputTerms[0]->getTerms() );
			resultList.erase( resultList.begin() + (inputTerms[1]->getConstantValue() - 1) );

			Term* outputTerm = new ListTerm( false, false, resultList );
			termTable->addTerm( outputTerm );
			return outputTerm;
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

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_DELNTHLISTEXTATOM_H_ */
