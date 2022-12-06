/*
 * SelectListExtAtom.h
 *
 *  Created on: 22 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_SELECTLISTEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_SELECTLISTEXTATOM_H_

#include "ListExtAtom.h"

namespace DLV2 {

namespace grounder {
/*
 * If field "deleteAll" is true SelectListExtAtom class matches the atom &delete_r(Elem,List;List2)
 * that deletes all occurrences of Elem in List1 and in all its nested lists. Otherwise it matches
 * the atom &select_r(Elem,List;List2) that deletes first occurrence of Elem in List and in and in
 */
class SelectListExtAtom : public ListExtAtom {
	public:

		SelectListExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf, bool deleteAll ) :
				ListExtAtom( predicate, terms, separator, naf ), deleteAll( deleteAll ){
		}
		SelectListExtAtom( Predicate* predicate, int separator, bool naf, bool deleteAll ) :
				ListExtAtom( predicate, separator, naf ), deleteAll( deleteAll ){
		}
		virtual ~SelectListExtAtom(){
		}
		Atom* clone(){

			Atom* clone = new SelectListExtAtom( this->predicate, this->separator, this->negative, this->deleteAll );
			clone->setTerms( this->terms );
			return clone;
		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}
	protected:
		virtual bool getInputTerm( const unsigned& i, var_assignment& assignment, Term*& t ){
			if( i == 0 )
				return getInputElem( assignment, t );
			return ListExtAtom::getInputTerm( i, assignment, t );
		}
		virtual Term* getResultTerm(){
			if( deleteAll )
				return inputTerms[1]->deleteAllOccurrence( inputTerms[0] );
			else{
				bool b = true;
				return inputTerms[1]->deleteFirstOccurrence( inputTerms[0], b );
			}
			return nullptr;
		}
	private:
		static const unsigned numberOfInputTerms = 2;
		//true if is an &delete atom
		bool deleteAll;
};

}/*namespace grounder*/

} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_SELECTLISTEXTATOM_H_ */
