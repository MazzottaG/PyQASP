#include "ListTerm.h"
#include "../table/TermTable.h"
#include "../hash/HashVecInt.h"

namespace DLV2 {
namespace grounder {

bool ListTerm::operator==( const Term& term ) const{

	auto termType = term.getType();
	if( termType != LIST_HEAD_TAIL && termType != LIST )
		return false;
	if( termType == LIST_HEAD_TAIL && !isHeadTail )
		return false;
	if( termType == LIST && isHeadTail )
		return false;
	if( term.getTermsSize() != terms.size() )
		return false;

	for ( unsigned i = 0; i < terms.size(); ++i )
		if( !(*(terms[i]) == *(term.getTerm( i ))) )
			return false;

	return true;
}

bool ListTerm::operator>( const Term& term ) const{

	//TODO
	return false;
}
bool ListTerm::operator>=( const Term& term ) const{

	//TODO
	return false;
}

bool ListTerm::operator<( const Term& term ) const{

	//TODO
	return false;
}

bool ListTerm::operator<=( const Term& term ) const{

	//TODO
	return false;
}

Term* ListTerm::calculate(){
	// Create a new function replacing the term in a vector
	// Recursively call calculate for nested function
	// At the end add a new function in a table and return index
	if( !contain( TermType::ARITH ) )
		return this;

	vector< Term* > sub_terms( terms.size() );

	TermTable *termTable = TermTable::getInstance();

	for ( unsigned int i = 0; i < terms.size(); i++ ){
		if( terms[i]->contain( TermType::ARITH ) ){
			sub_terms[i] = terms[i]->calculate();
		} else
			sub_terms[i] = terms[i];
	}

	Term *subTerm = new ListTerm( negative, isHeadTail, sub_terms );
	termTable->addTerm( subTerm );
	return subTerm;

}

Term* ListTerm::substitute( var_assignment& substitutionTerm ){
	vector< Term* > subTerms( terms.size() );

	bool isGround = true;
	for ( unsigned int i = 0; i < terms.size(); i++ ){
		Term* subTerm = terms[i]->substitute( substitutionTerm );
		subTerms[i] = subTerm;
		if( isGround && !subTerm->isGround() )
			isGround = false;
	}
	Term *newTerm = new ListTerm( negative, isHeadTail, subTerms );
	TermTable::getInstance()->addTerm( newTerm );
	return (isGround) ? newTerm->toList() : newTerm;	//TODO vedi appunti (CDC1)
}

size_t ListTerm::hash(){
	vector < size_t > hashVec( terms.size() );
	for ( unsigned int i = 0; i < terms.size(); i++ )
		hashVec[i] = terms[i]->getIndex();

	return HashVecInt::getHashVecInt()->computeHashSize_T( hashVec );
}

void ListTerm::print( ostream& stream ) const{
	stream << "[";
	for ( unsigned int i = 0; i < terms.size(); i++ ){
		if( i != 0 ){
			if( isHeadTail )
				stream << "|";
			else
				stream << ",";
		}
		terms[i]->print( stream );
	}
	stream << "]";
}

Term* ListTerm::toList(){
	if( !isHeadTail )
		return this;
	vector< Term* > subTerms;
	auto tail = terms[1];
	if( tail->getType() != LIST && tail->getType() != LIST_HEAD_TAIL ){
		// If the tail is not a list then return empty list
		Term* emptyList = new ListTerm( false, false, { } );
		TermTable::getInstance()->addTerm( emptyList );
		return emptyList;
	}
	unsigned sizeTail = tail->getTermsSize();
	subTerms.resize( sizeTail + 1 );
	subTerms[0] = terms[0];
	for ( unsigned i = 0; i < sizeTail; i++ )
		subTerms[i + 1] = tail->getTerm( i )->toList();
	Term* listTerm = new ListTerm( negative, false, subTerms );
	TermTable::getInstance()->addTerm( listTerm );
	return listTerm;
}

Term* ListTerm::getReverseTerms(){
	vector< Term* > rTerms( terms.size() );
	int i = 0;
	for ( auto it = terms.rbegin(); it != terms.rend(); it++, i++ ){
		Term* currentTerm = *it;
		rTerms[i] = currentTerm->getReverseTerms();
	}
	Term* reverseListTerm = new ListTerm( negative, isHeadTail, rTerms );
	TermTable::getInstance()->addTerm( reverseListTerm );
	return reverseListTerm;

}

Term* ListTerm::deleteAllOccurrence( Term * elem ){
	vector< Term* > cleanVector;
	for ( auto t : terms ){
		if( t != elem ){
			Term* termTmp = t->deleteAllOccurrence( elem );
			if( termTmp != nullptr )
				cleanVector.push_back( termTmp );
		}
	}
	if( cleanVector.empty() )
		return nullptr;
	Term* cleanListTerm = new ListTerm( negative, isHeadTail, cleanVector );
	TermTable::getInstance()->addTerm( cleanListTerm );
	return cleanListTerm;
}
Term* ListTerm::deleteFirstOccurrence( Term * elem, bool& dlt ){
	vector< Term* > cleanVector;
	for ( auto t : terms ){
		if( t != elem || !dlt ){
			if( !dlt ){
				cleanVector.push_back( t );
			} else{
				Term* tmpTerm = t->deleteFirstOccurrence( elem, dlt );
				if( tmpTerm != nullptr )
					cleanVector.push_back( tmpTerm );
			}
		} else
			if( t == elem && dlt )
				dlt = false;
	}

	if( cleanVector.empty() )
		return nullptr;
	Term* cleanListTerm = new ListTerm( negative, isHeadTail, cleanVector );
	TermTable::getInstance()->addTerm( cleanListTerm );
	return cleanListTerm;
}

void ListTerm::flatten( vector< Term* >& termsF ){

	for ( auto t : terms ){
		if( t->getType() == LIST || t->getType() == LIST_HEAD_TAIL ){
			for ( auto t2 : t->getTerms() ){
				t2->flatten( termsF );
			}
		} else{
			t->flatten( termsF );
		}
	}
}
pair< Term*, Term* > ListTerm::getHeadTailList(){
	if( isHeadTail )
		return{terms[0],terms[1]};

	Term* tail = new ListTerm( false, false, { terms.begin() + 1, terms.end() } );
	TermTable::getInstance()->addTerm( tail );
	return{terms[0],tail};
}

}
}

