/*
 * ListExtAtom.cpp
 *
 *  Created on: 21 mag 2017
 *      Author: Elena Mastria
 */

#include "ListExtAtom.h"
#include "../../term/ListTerm.h"
#include "../../../../common/Options.h"
namespace DLV2 {

namespace grounder {

bool ListExtAtom::getInputTerm( const unsigned& i, var_assignment& assignment, Term*& t ){
	return getInputList( assignment, t );
}

bool ListExtAtom::firstMatch( var_assignment& assignment ){

	Term* outputTerm = this->terms[separator];

	for ( unsigned i = 0; i < separator; i++ ){
		if( !getInputTerm( i, assignment, (inputTerms[i] = this->terms[i]) ) ){
			if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
			cerr << "WARNING: Invalid input in list external atom" << endl;
			return false;
		}
	}
	auto result = getResultTerm();
	auto& outputAssignment = assignment[outputTerm->getLocalVariableIndex()];
	if( outputTerm->getType() == VARIABLE ){
		if( outputAssignment == nullptr ){
			outputAssignment = result;
			return true;
		} else{
			if( outputAssignment->getType() != result->getType() ){
				if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
				cerr << "WARNING: Invalid output term type in list external atom" << endl;
				return false;
			}
			return outputAssignment == result;
		}
	}

	if( !getInputElem( assignment, outputTerm ) ){
		if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
		cerr << "WARNING: Invalid input in list external atom" << endl;
		return false;
	}
	if( outputTerm->getType() == result->getType() )
		return outputTerm == result;
	else{
		if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
			cerr << "WARNING: Invalid output term type in list external atom" << endl;
	}

	return false;

}

bool ListExtAtom::nextMatch( var_assignment& assignment ){
	return false;
}

}

} /* namespace DLV2 */
