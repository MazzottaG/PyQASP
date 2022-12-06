/*
 * ModExtAtom.h
 *
 *  Created on: 29 apr 2017
 *      Author: Elena Mastria
 */

#ifndef MODEXTATOM_H_
#define MODEXTATOM_H_

namespace DLV2 {
namespace grounder {
/*
 *
 */
class ModExtAtom : public ArithmeticExtAtom {
	public:
		ModExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ArithmeticExtAtom( predicate, terms, separator, naf ){
		}
		ModExtAtom( Predicate* predicate, int separator, bool naf ) :
				ArithmeticExtAtom( predicate, separator, naf ){
		}
		virtual ~ModExtAtom(){
		}

		Atom* clone(){
			Atom* clone = new ModExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;
		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}
	protected:
		virtual bool getResult(){
			int value1 = inputTermsValues[1];
			if( value1 == 0 )
				return false;
			result = inputTermsValues[0] % value1;
			return true;
		}
	private:
		static const unsigned numberOfInputTerms = 2;

};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* MODEXTATOM_H_ */
