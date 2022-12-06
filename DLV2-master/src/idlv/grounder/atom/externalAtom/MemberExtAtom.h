/*
 * MemberExtAtom.h
 *
 *  Created on: 05 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_MEMBEREXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_MEMBEREXTATOM_H_

#include "../ExtAtom.h"

namespace DLV2 {
namespace grounder {

/*
 * &member_str(X,Y;) check if X is contained in Y
 */
class MemberExtAtom : public ExtAtom {
	public:

		MemberExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ExtAtom( predicate, terms, separator, naf ){
		}
		MemberExtAtom( Predicate* predicate, int separator, bool naf ) :
				ExtAtom( predicate, separator, naf ){
		}
		virtual ~MemberExtAtom(){
		}
		Atom* clone(){

			Atom* clone = new MemberExtAtom( this->predicate, this->separator, this->negative );
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

		///If str is a quoted string this method deletes the quotes from it.
		void cleanString( string & str );
		string inputString1;
		string inputString2;
};

}/* grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_MEMBEREXTATOM_H_ */
