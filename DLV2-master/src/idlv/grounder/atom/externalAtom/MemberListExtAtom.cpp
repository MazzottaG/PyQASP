/*
 * MemberListExtAtom.cpp
 *
 *  Created on: 27 mag 2017
 *      Author: Elena Mastria
 */

#include "MemberListExtAtom.h"
#include "../../../../common/Options.h"
namespace DLV2 {

namespace grounder {


bool MemberListExtAtom::firstMatch( var_assignment& assignment ){

	Term* elem = this->terms[0];
	Term* list = this->terms[1];

	if( !getInputElem( assignment, elem ) || !getInputList( assignment, list ) ){
		if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
		cerr << "WARNING: Invalid input in list external atom" << endl;
		return false;
	}

	for ( auto t : list->getTerms() )
		if( t == elem )
			return true;
	return false;

}

bool MemberListExtAtom::nextMatch( var_assignment& assignment ){
	return false;
}

}

} /* namespace DLV2 */
