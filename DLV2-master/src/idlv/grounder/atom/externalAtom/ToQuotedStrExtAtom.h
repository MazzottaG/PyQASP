/*
 * ToQuotedStrExtAtom.h
 *
 *  Created on: 07 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_TOQUOTEDSTREXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_TOQUOTEDSTREXTATOM_H_
#include "StringExtAtom.h"

namespace DLV2 {
namespace grounder {
/*
 * &to_qstr(X;Y) Performs the cast of X in a quoted string Y.
 */
class ToQuotedStrExt : public StringExtAtom {
	public:

		ToQuotedStrExt( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				StringExtAtom( predicate, terms, separator, naf ){
		}
		ToQuotedStrExt( Predicate* predicate, int separator, bool naf ) :
				StringExtAtom( predicate, separator, naf ){
		}
		virtual ~ToQuotedStrExt(){
		}
		Atom* clone(){

			Atom* clone = new ToQuotedStrExt( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;

		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}
	protected:
		virtual void getResult(){
			string str = inputTermsValues[0];
			resultString = isQuoted( str ) ? str : '"' + str + '"';
		}
		inline virtual bool invalidOutputString( Term* outputTerm, var_assignment& assignment, string& outputString ){
			return !findOnlyStringValue( outputTerm, assignment, outputString ) || !isQuoted( outputString );
		}
	private:
		static const unsigned numberOfInputTerms = 1;

};

}/* grounder */

} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_TOQUOTEDSTREXTATOM_H_ */
