/*
 * AppendListExtAtom.h
 *
 *  Created on: 19 mag 2017
 *      Author: Elena Mastria
 */

#ifndef SRC_GROUNDER_ATOM_EXTERNALATOM_APPENDLISTEXTATOM_H_
#define SRC_GROUNDER_ATOM_EXTERNALATOM_APPENDLISTEXTATOM_H_
#include "ListExtAtom.h"
#include "../../term/ListTerm.h"

namespace DLV2 {
namespace grounder {
/*
 * The AppendListExtAtom class matches the atom &append(List1, List2; List3) that appends List2 to List1.
 */
class AppendListExtAtom : public ListExtAtom {
	public:

		AppendListExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf ) :
				ListExtAtom( predicate, terms, separator, naf ){
		}
		AppendListExtAtom( Predicate* predicate, int separator, bool naf ) :
				ListExtAtom( predicate, separator, naf ){
		}
		virtual ~AppendListExtAtom(){
		}
		Atom* clone(){

			Atom* clone = new AppendListExtAtom( this->predicate, this->separator, this->negative );
			clone->setTerms( this->terms );
			return clone;

		}
		static unsigned getNumberOfInputTerms(){
			return numberOfInputTerms;
		}
	protected:
		virtual Term* getResultTerm(){
			vector< Term* > outputList( inputTerms[0]->getTerms() );
			Term* t = inputTerms[1];
			outputList.insert( outputList.end(), t->getTerms().begin(), t->getTerms().end() );

			Term* outputT = new ListTerm( false, false, outputList );
			termTable->addTerm( outputT );
			return outputT;
		}
	private:
		static const unsigned numberOfInputTerms = 2;

};

}/*namespace grounder*/
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_ATOM_EXTERNALATOM_APPENDLISTEXTATOM_H_ */
