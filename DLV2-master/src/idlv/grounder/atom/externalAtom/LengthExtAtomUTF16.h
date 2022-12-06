/*
 * LengthExtAtomUTF16.h
 *
 *  Created on: 30 apr 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_LENGTHEXTATOMUTF16_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_LENGTHEXTATOMUTF16_H_

#include "LengthStringExtAtom.h"
#include <codecvt>
namespace DLV2 {
namespace grounder {
/*
 *
 */
class LengthExtAtomUTF16 : public LengthStringExtAtom {
	public:

		LengthExtAtomUTF16( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				LengthStringExtAtom( predicate, terms, separator, naf ){
		}
		LengthExtAtomUTF16( Predicate* predicate, int separator, bool naf ) :
				LengthStringExtAtom( predicate, separator, naf ){
		}
		virtual ~LengthExtAtomUTF16(){
		}
		Atom* clone(){

			Atom* clone = new LengthExtAtomUTF16( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;

		}

	protected:
		virtual void getLengthString(){
			wstring_convert< codecvt_utf8_utf16< char16_t >, char16_t > convert;
			u16string u16InputString = convert.from_bytes( inputString );
			lenghtString = u16InputString.length();
			char16_t c = '"';
			if( u16InputString[0] == c && u16InputString[lenghtString - 1] == c )
				lenghtString -= 2;
		}
};

}
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_LENGTHEXTATOMUTF16_H_ */
