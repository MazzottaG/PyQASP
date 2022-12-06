/*
 *  Copyright (C) 2017  Carmine Dodaro (carminedodaro@gmail.com)
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

#ifndef AGGREGATE_H
#define	AGGREGATE_H

#include "Literal.h"
#include <vector>
#include <iostream>
#include <cassert>
#include <climits>
#include <unordered_map>
using namespace std;

namespace preasp {

class Interpretation;
class AtomData;

class Id
{
    public:
        inline Id() : id_(0), truthvalue_(2) {}
        inline Id(unsigned int id) : id_(id), truthvalue_(2) {}
        inline unsigned int id() const { return id_; }
        inline void setId(unsigned int id) { id_ = id; }
        inline bool isTrue() const { return truthvalue_ == 1; }
        inline bool isFalse() const { return truthvalue_ == 0; }
        inline void setTrue() { truthvalue_ = 1; }
        inline void setFalse() { truthvalue_ = 0; }

    private:
        unsigned int id_ : 30;
        unsigned int truthvalue_ : 2;
};

class Aggregate
{
    public:
        Aggregate(vector<Literal>& lits, vector<unsigned int>& w) {
            literals.swap(lits);
            weights.swap(w);
            assert(literals.size()==weights.size() || weights.empty());
            assert(orderOfLits());
            assert(noDuplicates());
        }
        
        unsigned int addBound(unsigned int id, unsigned int bound);
        
        inline bool isWeighted() const { return !weights.empty(); }
        inline bool isRemoved() const { return bounds.empty(); }        
        
        inline Literal getLiteral(unsigned int pos) const { assert(pos < literals.size()); return literals[pos]; }
        inline unsigned int getWeight(unsigned int pos) const { if(!isWeighted()) return 1; assert(pos < weights.size()); return weights[pos]; }
                
        inline unsigned int numberOfIdBounds() const { assert(ids.size() == bounds.size()); return ids.size(); }
        inline unsigned int getBound(unsigned int pos) const { assert(pos < numberOfIdBounds()); return bounds[pos]; }
        inline unsigned int getId(unsigned int pos) const { assert(pos < numberOfIdBounds()); return ids[pos].id(); }
        
        inline void setIdTrue(unsigned int pos) { assert(pos < numberOfIdBounds()); ids[pos].setTrue(); }
        inline void setIdFalse(unsigned int pos) { assert(pos < numberOfIdBounds()); ids[pos].setFalse(); }
        inline bool isIdTrue(unsigned int pos) const { assert(pos < numberOfIdBounds()); return ids[pos].isTrue(); }
        inline bool isIdFalse(unsigned int pos) const { assert(pos < numberOfIdBounds()); return ids[pos].isFalse(); }
        
//        void printAggregateSet(ostream& out) const;
//        void printWeights(ostream& out) const;
        void printGringo(ostream& out) const;
        
        void remapId(unsigned int pos, unsigned int newValue) { assert(pos < numberOfIdBounds()); ids[pos].setId(newValue); }
        void remap(const AtomData& atomData);
        unsigned int removeFalseLiteral(Literal);
        unsigned int removeTrueLiteral(Literal, vector<unsigned int>& trueIds);        
        void simplify(Interpretation& interpretation, vector<unsigned int>& trueIds);
        
        void removeFalseId(unsigned int pos);
        inline unsigned int size() const { return literals.size(); }
        void removeUselessIdsAndBounds(vector<pair<Literal,Literal>>& equivalences);                
        
        void replaceLiterals(unordered_map<int,int>&);
        
    private:
        inline Aggregate(const Aggregate& orig);
        vector<Literal> literals;
        vector<unsigned int> weights;
        
        vector<Id> ids;
        vector<unsigned int> bounds;                
        
        unsigned int removeLit(Literal lit);
        void removedTrueLiteral(unsigned int weight, vector<unsigned int>& trueIds);
        
        void bubblesort();
        
        #ifndef NDEBUG
        bool orderOfLits() const {
            bool foundPos = false;
            for(unsigned int i = 0; i < literals.size(); i++) {
                if(!literals[i].sign()) foundPos = true;
                else if(foundPos) return false;
            }
            return true;
        }
        
        bool noDuplicates() const {
            vector<Literal> check;
            for(unsigned int i = 0; i < literals.size(); i++) {
                for(unsigned int j = 0; j < check.size(); j++)
                    if(check[j] == literals[i])
                        return false;
                check.push_back(literals[i]);
            }
            return true;
        }
        #endif
};

}

#endif
