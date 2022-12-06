/*
 * InsLastListExtAtom.h
 *
 *  Created on: 30 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_INSLASTLISTEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_INSLASTLISTEXTATOM_H_

#include "ListExtAtom.h"
#include "../../term/ListTerm.h"

namespace DLV2 {

namespace grounder {
/*
 * InsLastListExtAtom class matches the atom &insLast(List,Elem;List2) that inserts the Elem at last position
 * in List and returns the list resulting in List2
 */
class InsLastListExtAtom : public ListExtAtom {
	public:

		InsLastListExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ListExtAtom( predicate, terms, separator, naf ){
		}
		InsLastListExtAtom( Predicate* predicate, int separator, bool naf ) :
				ListExtAtom( predicate, separator, naf ){
		}
		virtual ~InsLastListExtAtom(){
		}
		Atom* clone(){

			Atom* clone = new InsLastListExtAtom( this->predicate, this->separator, this->negative );
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
			return ListExtAtom::getInputTerm( i, assignment, t );
		}
		virtual Term* getResultTerm(){
			vector< Term* > resultList( inputTerms[0]->getTerms() );
			resultList.push_back( inputTerms[1] );
			Term * resultTerm = new ListTerm( false, false, resultList );
			termTable->addTerm( resultTerm );
			return resultTerm;
		}
	private:
		static const unsigned numberOfInputTerms = 2;

};

}/*namespace grounder*/

} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_INSLASTLISTEXTATOM_H_ */
