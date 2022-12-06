/*
 * RandExtAtom.cpp
 *
 *  Created on: 26 nov 2020
 *      Author: Elena Mastria
 */
#include "RandExtAtom.h"
namespace DLV2 {

namespace grounder {

bool RandExtAtom::set_seed=true;

void RandExtAtom::set_generator() {
		if (RandExtAtom::set_seed) {
			srand(time(0));
			RandExtAtom::set_seed = false;
		}
	}
}

}



