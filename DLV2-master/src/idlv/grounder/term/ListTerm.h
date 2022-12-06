/*******************************************************************************
 Copyright 2016 Francesco Calimeri, Davide Fusca', Simona Perri and Jessica Zangari
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 *******************************************************************************/

#ifndef LISTTERM_H_
#define LISTTERM_H_ 1

#include "Term.h"
#include <vector>
#include "../table/TermTable.h"
using namespace std;

namespace DLV2 {
namespace grounder {

class ListTerm : public Term {

	public:
		ListTerm() :
				isHeadTail( false ){
		}
//		ListTerm( bool n, bool headTail, vector< Term* >& terms ) :
//				Term( n ), isHeadTail( headTail ), terms( terms ){
//		}
		ListTerm( bool n, bool headTail, const vector< Term* >& terms ) :
				Term( n ), isHeadTail( headTail ), terms( terms ){
		}
		ListTerm( bool n, bool headTail, vector< Term* >::const_iterator begin, vector< Term* >::const_iterator end ) :
				Term( n ), isHeadTail( headTail ), terms( begin, end ){

		}
		ListTerm( bool n, bool headTail, vector< Term* >::const_reverse_iterator begin, vector< Term* >::const_reverse_iterator end ) :
				Term( n ), isHeadTail( headTail ), terms( begin, end ){

		}
		~ListTerm(){
		}

		virtual unsigned getTermsSize() const{
			return terms.size();
		}
		virtual Term* getTerm( unsigned i ) const{
			return terms[i];
		}

		virtual void addTerm( Term* term ){
			terms.push_back( term );
		}
		;
		virtual void popTerm(){
			terms.pop_back();
		}
		;

		virtual TermType getType() const{
			return (isHeadTail) ? TermType::LIST_HEAD_TAIL : TermType::LIST;
		}
		;
		virtual bool contain( TermType type ){
			if( type == TermType::LIST && !isHeadTail )
				return true;
			if( type == TermType::LIST_HEAD_TAIL && isHeadTail )
				return true;
			for ( auto term : terms )
				if( term->contain( type ) )
					return true;
			return false;
		}
		virtual bool isGround(){
			for ( auto term : terms )
				if( !term->isGround() )
					return false;
			return true;
		}

		virtual void getVariable( set_term& variables ){
			for ( auto term : terms )
				term->getVariable( variables );
		}
		;

		virtual void getVariable( vector< Term* >& variables ){
			for ( auto term : terms )
				term->getVariable( variables );
		}
		;

		virtual void getGroundTerm( set_term& variables ){
			for ( auto term : terms )
				term->getGroundTerm( variables );
		}
		;

		vector< Term* >& getTerms(){
			return terms;
		}
		;
		virtual bool operator==( const Term& term ) const;

		virtual bool operator>( const Term& term ) const;
		virtual bool operator>=( const Term& term ) const;

		virtual bool operator<( const Term& term ) const;
		virtual bool operator<=( const Term& term ) const;

		virtual Term* calculate();
		virtual Term* substitute( var_assignment& substitutionTerm );
		/// Return the name of the function concatenated with '*' and the id of the composites term
		virtual size_t hash();

		virtual void print( ostream& stream = cout ) const;

		virtual void getVariablesInArith( set_term& vars ){
			for ( auto t : terms )
				t->getVariablesInArith( vars );
		}

		virtual void substituteVarWithAnonymous( Term* t, Term* t1 ){
		}

		virtual Term* getReverseTerms();
//		virtual Term* getReverseList(){
//			Term * r = new ListTerm( negative, false, vector< Term* >( this->terms.rbegin(), this->terms.rend() ) );
//			TermTable::getInstance()->addTerm( r );
//			return r;
//		}
		virtual Term* deleteAllOccurrence( Term * elem );
		virtual Term* deleteFirstOccurrence( Term * elem, bool& dlt );
		virtual void flatten( vector< Term * >& termsF );
		virtual string toString() const{
			string n = "[";
			for ( unsigned int i = 0; i < terms.size(); i++ ){
				if( i != 0 )
					n += ",";
				n += terms[i]->getName();
			}
			n += "]";
			return n;
		}

		Term* toList();
		inline pair< Term*, Term* > getHeadTailList();

		void getVariableMustBeList( vector< Term* >& variables ){
			for ( auto t : terms )
				t->getVariableMustBeList( variables );
			if( !isHeadTail )
				return;
			if( terms[1]->getType() == VARIABLE )
				variables.push_back( terms[1] );

		}
	private:

		bool isHeadTail;
		vector< Term* > terms;

};

}
}

#endif // 
