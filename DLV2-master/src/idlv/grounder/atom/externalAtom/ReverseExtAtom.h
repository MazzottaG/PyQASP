/*
 * ReverseExtAtom.h
 *
 *  Created on: 30 apr 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_REVERSEEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_REVERSEEXTATOM_H_

namespace DLV2 {
namespace grounder {

/*
 *
 */
class ReverseExtAtom : public StringExtAtom {
	public:

		ReverseExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				StringExtAtom( predicate, terms, separator, naf ){
		}
		ReverseExtAtom( Predicate* predicate, int separator, bool naf ) :
				StringExtAtom( predicate, separator, naf ){
		}
		virtual ~ReverseExtAtom(){
		}
		Atom* clone(){

			Atom* clone = new ReverseExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;

		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}
	protected:
		virtual void getResult(){
			string str = inputTermsValues[0];
			resultString = string( str.rbegin(), str.rend() );
		}
	private:
		static const unsigned numberOfInputTerms = 1;

};

}
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_REVERSEEXTATOM_H_ */
