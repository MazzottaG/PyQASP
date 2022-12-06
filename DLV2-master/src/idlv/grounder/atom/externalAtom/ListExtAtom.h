/*
 * ListExtAtom.h
 *
 *  Created on: 21 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_LISTEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_LISTEXTATOM_H_
#include "../ExtAtom.h"
namespace DLV2 {
namespace grounder {

/*
 * ListExtAtom is a base class for list external atoms.
 */
class ListExtAtom : public ExtAtom {
	public:

		ListExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ExtAtom( predicate, terms, separator, naf ), inputTerms( separator ){
		}
		ListExtAtom( Predicate* predicate, int separator, bool naf ) :
				ExtAtom( predicate, separator, naf ), inputTerms( separator ){
		}
		virtual ~ListExtAtom(){
		}

	protected:
		virtual bool firstMatch( var_assignment& assignment );
		virtual bool nextMatch( var_assignment& assignment );

		/// This method stores the input terms within the "inputTerms" structure
		virtual bool getInputTerm( const unsigned& i, var_assignment& assignment, Term*& t );

		///This method returns the term resulting from the operation defined by the derived classes
		virtual Term* getResultTerm()=0;

		vector< Term* > inputTerms;
};

}/* grounder */

} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_LISTEXTATOM_H_ */
