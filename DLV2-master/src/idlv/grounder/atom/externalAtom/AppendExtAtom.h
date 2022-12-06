/*
 * AppendExtAtom.h
 *
 *  Created on: 30 apr 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_APPENDEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_APPENDEXTATOM_H_

#include "StringExtAtom.h"

namespace DLV2 {
namespace grounder {

/*
 * &append_str(X,Y;Z)
 */
class AppendExtAtom : public StringExtAtom {
	public:

		AppendExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				StringExtAtom( predicate, terms, separator, naf ){
		}
		AppendExtAtom( Predicate* predicate, int separator, bool naf ) :
				StringExtAtom( predicate, separator, naf ){
		}
		virtual ~AppendExtAtom(){
		}
		Atom* clone(){

			Atom* clone = new AppendExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;

		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}

	protected:
		virtual void getResult();
	private:
		static const unsigned numberOfInputTerms = 2;
		///If str is a quoted string this method deletes the quotes from it.
		void cleanString( string & str, bool & symbolicConstant );

};

}/*namespace grounder*/
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_APPENDEXTATOM_H_ */
