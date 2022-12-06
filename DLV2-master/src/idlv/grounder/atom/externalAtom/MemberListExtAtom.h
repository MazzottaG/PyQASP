/*
 * MemberListExtAtom.h
 *
 *  Created on: 27 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_MEMBERLISTEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_MEMBERLISTEXTATOM_H_
#include "../ExtAtom.h"
namespace DLV2 {
namespace grounder {

/*
 * MemberListExtAtom class matches the atom &member(Elem,List;) that check if elem is contained in List
 */
class MemberListExtAtom : public ExtAtom {
	public:

		MemberListExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ExtAtom( predicate, terms, separator, naf ){
		}
		MemberListExtAtom( Predicate* predicate, int separator, bool naf ) :
				ExtAtom( predicate, separator, naf ){
		}
		virtual ~MemberListExtAtom(){
		}
		Atom* clone(){
			Atom* clone = new MemberListExtAtom( this->predicate, this->separator, this->negative );
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

} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_MEMBERLISTEXTATOM_H_ */
