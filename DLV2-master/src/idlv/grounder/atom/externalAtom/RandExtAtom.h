/*
 * RandExtAtom.h
 *
 *  Created on: 29 apr 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_RANDEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_RANDEXTATOM_H_
#include <stdlib.h>
#include <time.h>
#include "ArithmeticExtAtom.h"
namespace DLV2 {
namespace grounder {
/*
 * &rand(X,Y;Z) generates an integers Z such that X <= Z <= Y.
 */
class RandExtAtom: public ArithmeticExtAtom {

public:
	RandExtAtom(Predicate* predicate, vector<Term*>& terms, int separator, bool naf) :
			ArithmeticExtAtom(predicate, terms, separator, naf) {
		set_generator();
	}
	RandExtAtom(Predicate* predicate, int separator, bool naf) :
			ArithmeticExtAtom(predicate, separator, naf) {
		set_generator();
	}
	virtual ~RandExtAtom() {
	}

	Atom* clone() {

		Atom* clone = new RandExtAtom(this->predicate, this->separator, this->negative);
		clone->setTerms(this->terms);
		return clone;

	}
	static unsigned getNumberOfInputTerms() {
		return numberOfInputTerms;
	}
protected:
	virtual bool getResult() {
		int value1 = inputTermsValues[1];
		int value0 = inputTermsValues[0];
		if (value1 < value0)
			return false;
		int diff = value1 - value0 + 1;
		result = (rand() % diff) + value0;
		return true;
	}
private:
	static const unsigned numberOfInputTerms = 2;
	static bool set_seed;
	void set_generator();


};

}
/*namespace grounder*/
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_RANDEXTATOM_H_ */
