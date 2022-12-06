/*
 * SimpleSelectListExtAtom.h
 *
 *  Created on: 01 giu 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_SIMPLESELECTLISTEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_SIMPLESELECTLISTEXTATOM_H_
#include "ListExtAtom.h"
#include "../../term/ListTerm.h"
namespace DLV2 {

namespace grounder {
/*
 * If field "deleteAll" is true SelectListExtAtom class matches the atom &delete(Elem,List;List2)
 * that deletes all occurrences of Elem in List1 otherwise it matches the atom &select(Elem,List;List2)
 * that deletes first occurrence of Elem in List
 */
class SimpleSelectListExtAtom : public ListExtAtom {
	public:

		SimpleSelectListExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf, bool deleteAll ) :
				ListExtAtom( predicate, terms, separator, naf ), deleteAll( deleteAll ){
		}
		SimpleSelectListExtAtom( Predicate* predicate, int separator, bool naf, bool deleteAll ) :
				ListExtAtom( predicate, separator, naf ), deleteAll( deleteAll ){
		}
		virtual ~SimpleSelectListExtAtom(){
		}
		Atom* clone(){

			Atom* clone = new SimpleSelectListExtAtom( this->predicate, this->separator, this->negative, this->deleteAll );
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
		virtual Term* getResultTerm();
	private:

		static const unsigned numberOfInputTerms = 2;
		//true if is an &delete atom
		bool deleteAll;
};

}/*namespace grounder*/
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_SIMPLESELECTLISTEXTATOM_H_ */
