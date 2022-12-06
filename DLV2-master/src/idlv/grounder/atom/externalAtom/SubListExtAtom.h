/*
 * SubListExtAtom.h
 *
 *  Created on: 30 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_SUBLISTEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_SUBLISTEXTATOM_H_
#include "../ExtAtom.h"

namespace DLV2 {

namespace grounder {

/*
 * SubListExtAtom class matches the atom &subList(List1,List2;) that check if List1 is a sublist of List2
 */
class SubListExtAtom : public ExtAtom {
	public:

		SubListExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ExtAtom( predicate, terms, separator, naf ){
		}
		SubListExtAtom( Predicate* predicate, int separator, bool naf ) :
				ExtAtom( predicate, separator, naf ){
		}
		virtual ~SubListExtAtom(){
		}
		Atom* clone(){
			Atom* clone = new SubListExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;
		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}
	protected:
		virtual bool firstMatch( var_assignment& assignment );
		virtual bool nextMatch( var_assignment& assignment );
	private:
		static const unsigned numberOfInputTerms = 2;
};

}/* grounder */

/*
 *
 */
class SubListExtAtom {
};

} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_SUBLISTEXTATOM_H_ */
