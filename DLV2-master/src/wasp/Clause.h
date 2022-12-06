/*
 *
 *  Copyright 2013 Mario Alviano, Carmine Dodaro, and Francesco Ricca.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#ifndef WASP_CLAUSE_H
#define WASP_CLAUSE_H

#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>

#include "Clause.h"
#include "Literal.h"
#include "Learning.h"
#include "Reason.h"
#include "../common/Assert.h"

using namespace std;

class Solver;

/**
 *  This class represents a clause.
 *  A clause is a disjunction of literals.
 *  For example (a1 v a2 v not a3) is a clause and a1, a2 are positive literals and not a3 is a negative literal.
 * 
 */
class Clause : public Reason
{
    friend ostream &operator<<( ostream & out, const Clause & clause );
    friend Clause* Learning::onConflict( Literal conflictLiteral, Reason* conflictClause );
    friend Clause* Learning::learnClausesFromUnfoundedSet( Vector< Var >& unfoundedSet );

    public:                
        inline ~Clause(){}
        inline Clause(unsigned reserve=8) : lbd_(0), canBeDeleted_(1) { literals.reserve(reserve); clauseData.inQueue = 0; clauseData.learned = 0; }

        inline Literal getAt( unsigned idx ) const { assert( idx < size() ); return literals[ idx ]; }
        inline void setAt( unsigned idx, Literal lit ) { assert( idx < size() ); literals[ idx ] = lit; }
        inline void flipLiteralAt( unsigned idx ) { assert( idx < size() ); literals[ idx ] = literals[ idx ].getOppositeLiteral(); }
        inline void markAsDeleted() { literals.push_back( literals[ 0 ] ); literals[ 0 ] = Literal::null; }
        inline bool hasBeenDeleted() const { assert( !literals.empty() ); return literals[ 0 ] == Literal::null; }
        inline void addLiteral(Literal literal) { assert(!isLearned()); literals.push_back(literal); signature() |= getSignature(literal.getVariable()); }
        inline void addLiteralInLearnedClause(Literal literal) { assert(isLearned()); literals.push_back(literal); }

        inline bool contains(Literal literal);

        inline Literal operator[]( unsigned int idx ) const { assert_msg( idx < size(), "Index is " << idx << " - literals: " << size() ); return literals[ idx ]; }
        inline Literal& operator[]( unsigned int idx ) { assert_msg( idx < size(), "Index is " << idx << " - literals: " << size() ); return literals[ idx ]; }

        inline void removeLiteral( Literal literal );
        inline void removeLastLiteralNoWatches(){ literals.pop_back(); }
        
        void onLearning( const Solver& solver, Learning* strategy, Literal lit );
        bool onNavigatingLiteralForAllMarked( const Solver& solver, Learning* strategy, Literal lit );
        void onNavigatingForUnsatCore( const Solver& solver, vector< unsigned int >& visited, unsigned int numberOfCalls, Literal lit );

        inline unsigned int size() const { return literals.size(); }               

        inline uint64_t getSignature() const { return signature(); }
        
        inline void setPositionInSolver( unsigned int pos ) { clauseData.positionInSolver = pos; }
        inline unsigned int getPositionInSolver(){ return clauseData.positionInSolver; }
        
        inline Activity& activity(){ return act(); }
        inline const Activity& activity() const { return act(); }
        inline void setLearned() { clauseData.learned = 1; }
        inline void setOriginal() { clauseData.learned = 0; }
        inline bool isLearned() const { return clauseData.learned == 1; }
        
        inline bool removeDuplicatesAndCheckIfTautological();
        bool removeDuplicatesAndFalseAndCheckIfTautological( Solver& solver );
        
        inline void resetInQueue(){ clauseData.inQueue = 0; }
        inline void setInQueue(){ clauseData.inQueue = 1; }
        inline bool isInQueue(){ return clauseData.inQueue == 1; }
        
        inline void copyLiterals(const Clause& c) { for(unsigned int i = 0; i < c.size(); i++) addLiteral(c.literals[i]); }
        
        inline void swapLiterals(unsigned int pos1, unsigned int pos2);
        
        inline void recomputeSignature();                
        #ifndef NDEBUG
        inline bool isTautology() const
        {
            for( unsigned i = 0; i < size(); ++i )
                for( unsigned j = i+1; j < size(); ++j )
                    if( getAt( i ) == getAt( j ).getOppositeLiteral() )
                        return true;
            return false;
        }
        #endif
        
        inline void setLbd( unsigned int lbd ) { lbd_ = lbd; }
        inline unsigned int lbd() const { return lbd_; }
        
        inline void setCanBeDeleted( bool b ) { canBeDeleted_ = b; }
        inline bool canBeDeleted() const { return canBeDeleted_; }                
        
        inline void shrink( unsigned int value ) { literals.shrink( value ); }                
        
    private:
        Vector< Literal > literals;
        ostream& print( ostream& out ) const;        
        Clause( const Clause& );
        
        union SignatureAct
        {
            uint64_t signature;
            Activity act;
            
            SignatureAct() { memset( this, 0, sizeof( SignatureAct ) ); }
        } signature_act;
        inline uint64_t& signature() { assert( !isLearned() ); return signature_act.signature; }
        inline const uint64_t& signature() const { assert( !isLearned() ); return signature_act.signature; }
        inline Activity& act() { assert( isLearned() ); return signature_act.act; }
        inline const Activity& act() const { assert( isLearned() ); return signature_act.act; }
        
        inline uint64_t getSignature( Var v ) const { return ( ( uint64_t ) 1 ) << ( ( v - 1 ) & 63 ); }
        
        struct
        {        
            unsigned inQueue                : 1;
            unsigned learned                : 1;            
            unsigned positionInSolver       : 30;
        } clauseData;
        
        unsigned int lbd_ : 31;
        unsigned int canBeDeleted_ : 1;
};

void Clause::removeLiteral(Literal literal) {
    unsigned int i=0;
    for(; i < size()-1; ++i) {
        if(literals[i]==literal) {
            literals[i]=literals.back();
            break;
        }
    }

    assert(literals.back()==literal || literals.back()==literals[i]);
    literals.pop_back();
    recomputeSignature();
}

void Clause::swapLiterals(unsigned int pos1, unsigned int pos2) {
    assert_msg(pos1 < size(), "First position is out of range: " << pos1 << " of " << size());
    assert_msg(pos2 < size(), "Second position is out of range: " << pos2 << " of " << size());
    Literal tmp=literals[pos1];
    literals[pos1]=literals[pos2];
    literals[pos2]=tmp;
}

bool Clause::contains(Literal literal) {
    for(unsigned k = 0; k < size(); ++k)
        if(literals[k]==literal)
            return true;
    return false;
}

inline int literalComparator(Literal l1, Literal l2){ return l1.getVariable() < l2.getVariable(); }

bool Clause::removeDuplicatesAndCheckIfTautological() {
    assert(size() > 0);
    literals.sort(literalComparator);
    Literal previousLiteral=literals[0];
    unsigned int j=1;
    for(unsigned int i=1; i < size(); i++) {
        literals[j]=literals[i];
        if(previousLiteral==literals[i].getOppositeLiteral()) return true; //TAUTOLOGICAL
        if(previousLiteral==literals[i]) continue; //DUPLICATE
        previousLiteral=literals[i];
        j++;
    }
    
    if(j!=size()) {
        shrink(j);
        recomputeSignature();
    }
    return false;
}

void Clause::recomputeSignature() {
    signature()=0;
    for(unsigned int i=0; i < size(); i++)
        signature() |= getSignature(literals[i].getVariable());
}

#endif
