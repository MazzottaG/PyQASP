/*
 * SubListExtAtom.cpp
 *
 *  Created on: 30 mag 2017
 *      Author: Elena Mastria
 */

#include "SubListExtAtom.h"
#include "../../../../common/Options.h"

namespace DLV2 {

namespace grounder {

bool SubListExtAtom::firstMatch( var_assignment& assignment ){

	Term* sublist = this->terms[0];
	Term* list = this->terms[1];

	if( !getInputList( assignment, sublist ) || !getInputList( assignment, list ) ){
		if( Options::globalOptions()->isEnableExternalAtomsWarnings() )
		cerr << "WARNING: Invalid input in list external atom" << endl;
		return false;
	}
	auto it = search( list->getTerms().begin(), list->getTerms().end(), sublist->getTerms().begin(), sublist->getTerms().end() ); //Versione1
	if( it != list->getTerms().end() )
		return true;
	return false;

}

bool SubListExtAtom::nextMatch( var_assignment& assignment ){
	return false;
}

}

} /* namespace DLV2 */
