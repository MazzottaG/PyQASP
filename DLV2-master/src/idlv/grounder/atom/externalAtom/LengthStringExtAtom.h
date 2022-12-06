/*
 * LengthStringExtAtom.h
 *
 *  Created on: 27 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_LENGTHSTRINGEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_LENGTHSTRINGEXTATOM_H_
#include "../ExtAtom.h"

namespace DLV2 {
namespace grounder {
/*
 *
 */
class LengthStringExtAtom : public ExtAtom {
	public:

		LengthStringExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ExtAtom( predicate, terms, separator, naf ), lenghtString( 0 ){
		}
		LengthStringExtAtom( Predicate* predicate, int separator, bool naf ) :
				ExtAtom( predicate, separator, naf ), lenghtString( 0 ){
		}
		virtual ~LengthStringExtAtom(){
		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}
	protected:
		virtual bool firstMatch( var_assignment& assignment );
		virtual bool nextMatch( var_assignment& assignment );
		virtual void getLengthString()=0;
		string inputString;
		int lenghtString;
	private:
		static const unsigned numberOfInputTerms = 1;
};
}

} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_LENGTHSTRINGEXTATOM_H_ */
