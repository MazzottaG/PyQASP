/*
 * ListExtAtom.h
 *
 *  Created on: 03 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_STRINGEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_STRINGEXTATOM_H_

#include "../ExtAtom.h"

namespace DLV2 {
namespace grounder {

/*
 * StringExtAtom is a base class for string external atoms.
 */
class StringExtAtom : public ExtAtom {
	public:

		StringExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ExtAtom( predicate, terms, separator, naf ), inputTermsValues( separator ){
		}
		StringExtAtom( Predicate* predicate, int separator, bool naf ) :
				ExtAtom( predicate, separator, naf ), inputTermsValues( separator ){
		}
		virtual ~StringExtAtom(){
		}

	protected:
		virtual bool firstMatch( var_assignment& assignment );
		virtual bool nextMatch( var_assignment& assignment );

		/*
		 * 	Updates the resultString field with the string resulting from the manipulation of input terms
		 * 	according to the definition of derived classes.
		 */
		virtual void getResult()=0;
		/*
		 * Checks the validity of the output term
		 */
		inline virtual bool invalidOutputString( Term* outputTerm, var_assignment& assignment, string& outputString ){
			return !findStringValue( outputTerm, assignment, outputString );
		}
		inline bool isQuoted( const string& str ){
			return str[0] == '"' && str[str.length() - 1] == '"';
		}
		vector< string > inputTermsValues;
		string resultString;

};

}/* grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_STRINGEXTATOM_H_ */
