/*
 * ArithmeticExtAtom.h
 *
 *  Created on: 29 apr 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_ARITHMETICEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_ARITHMETICEXTATOM_H_

#include "../ExtAtom.h"

namespace DLV2 {
namespace grounder {

/*
 * ArithmeticExtAtom is the base class for external atoms that must perform an arithmetic operation.
 */
class ArithmeticExtAtom : public ExtAtom {
	public:

		ArithmeticExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ExtAtom( predicate, terms, separator, naf ), inputTermsValues( separator ), result( 0 ), isFunctional( false ){
		}
		ArithmeticExtAtom( Predicate* predicate, int separator, bool naf ) :
				ExtAtom( predicate, separator, naf ), result( 0 ), isFunctional( false ){
		}
		virtual ~ArithmeticExtAtom(){
		}

	protected:
		virtual bool firstMatch( var_assignment& assignment );
		virtual bool nextMatch( var_assignment& assignment );

		// This method computes the result of any arithmetic operation defined in the derived classes.
		virtual bool getResult()=0;

		// This method checks if the value obtained is equal to the expected result.
		inline virtual bool compareOutputValue( const int & value ){
			return value == result;
		}

		vector< int > inputTermsValues;
		int result;

		/******** field useful for relational External Atoms ********/

		/// is true if the output term is bound
		bool isFunctional;

		/************************************************************/

};

}/* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_ARITHMETICEXTATOM_H_ */
