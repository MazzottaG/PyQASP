/*
 * SimpleSelectListExtAtom.cpp
 *
 *  Created on: 01 giu 2017
 *      Author: Elena Mastria
 */
#include "SimpleSelectListExtAtom.h"
namespace DLV2 {
namespace grounder {
Term* SimpleSelectListExtAtom::getResultTerm(){
	Term * elem = inputTerms[0];
	vector< Term* > resultList;
	if( deleteAll ){
		for ( auto t : inputTerms[1]->getTerms() ){
			if( t != elem )
				resultList.push_back( t );
		}
	} else{
		bool first = true;
		for ( auto t : inputTerms[1]->getTerms() ){
			if( t != elem || !first )
				resultList.push_back( t );
			else if( t == elem )
				first = false;
		}
	}
	Term* resultTerm = new ListTerm( false, false, resultList );
	termTable->addTerm( resultTerm );
	return resultTerm;

}
}
}

