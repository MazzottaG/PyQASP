/*
 * CombsGenerator.cpp
 *
 *  Created on: 03 lug 2019
 *      Author: Elena Mastria
 */

#include "CombsGenerator.h"
namespace DLV2 {

namespace grounder {

CombsGenerator* CombsGenerator::makeGenerator( AggregateFunction agF ){
	switch ( agF ){
		case MIN:
			return new MinMax_CombsGenerator( agF );
		case MAX:
			return new MinMax_CombsGenerator( agF );
		case COUNT:
			return new CountCombsGenerator( agF );
		case SUM:
			return new SumCombsGenerator( agF );
		default:
			return new AllCombinationsChainGenerator( agF );
	}
}

}
}


