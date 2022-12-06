/*
 * IntExtAtom.cpp
 *
 *  Created on: 29 apr 2017
 *      Author: Elena Mastria
 */

#include "IntExtAtom.h"

namespace DLV2 {
namespace grounder {

bool IntExtAtom::nextMatch( var_assignment& assignment ){
	if( isFunctional )
		return false;
	if( ++result <= inputTermsValues[1] ){
		Term* currentTerm = new NumericConstantTerm( result < 0, result );
		termTable->addTerm( currentTerm );
		assignment[terms[2]->getLocalVariableIndex()] = currentTerm;
		return true;
	}
	return false;
}
}/* namespace grounder */
} /* namespace DLV2 */
