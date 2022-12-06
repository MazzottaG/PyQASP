/*
 * Length32ExtAtom.h
 *
 *  Created on: 15 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_LENGTH32EXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_LENGTH32EXTATOM_H_
#include "../ExtAtom.h"

namespace DLV2 {
namespace grounder {
/*
 *
 */
class Length32ExtAtom : public ExtAtom {
	public:

		Length32ExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ExtAtom( predicate, terms, separator, naf ), lenghtString( 0 ){
		}
		Length32ExtAtom( Predicate* predicate, int separator, bool naf ) :
				ExtAtom( predicate, separator, naf ), lenghtString( 0 ){
		}
		virtual ~Length32ExtAtom(){
		}
		Atom* clone(){

			Atom* clone = new Length32ExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;

		}

	protected:
		virtual bool firstMatch( var_assignment& assignment );
		virtual bool nextMatch( var_assignment& assignment );
	private:
		string inputString;
		int lenghtString;
};

}
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_LENGTH32EXTATOM_H_ */
