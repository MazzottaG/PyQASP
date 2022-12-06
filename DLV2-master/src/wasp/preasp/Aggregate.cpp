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

#include "Aggregate.h"
#include "Interpretation.h"
#include "AtomData.h"
#include <cmath>
#include <unordered_set>
using namespace std;

namespace preasp {

void Aggregate::simplify(Interpretation& interpretation, vector<unsigned int>& trueIds) {
    unsigned int j = 0;
    for(unsigned int i = 0; i < literals.size(); i++) {
        literals[j] = literals[i];
        if(isWeighted()) weights[j] = weights[i];        
        if(interpretation.isFalse(literals[i])) continue;
        if(interpretation.isTrue(literals[i])) {
            removedTrueLiteral(isWeighted() ? weights[i] : 1, trueIds);
            if(bounds.empty()) return;
            continue;
        }        
        j++;
    }
    literals.resize(j); 
    if(isWeighted()) weights.resize(j);    
}

unsigned int Aggregate::addBound(unsigned int id, unsigned int bound) {
    assert(ids.size() == bounds.size());
    for(unsigned int i = 0; i < bounds.size(); i++) if(bounds[i]==bound) return getId(i);
    ids.push_back(id);
    bounds.push_back(bound);
    return UINT_MAX;
}

unsigned int Aggregate::removeLit(Literal lit) {
    unsigned int j = 0;
    unsigned int weight = UINT_MAX;
    for(unsigned int i = 0; i < literals.size(); i++) {
        literals[j] = literals[i];
        if(isWeighted()) weights[j] = weights[i];
        if(literals[i] == lit)
            weight = getWeight(i);
        else
            j++;
    }
    literals.resize(j);
    if(isWeighted()) weights.resize(j);
    assert(weight != UINT_MAX);
    assert(orderOfLits());
    return weight;
}

unsigned int Aggregate::removeFalseLiteral(Literal lit) {
    return removeLit(lit);
}

unsigned int Aggregate::removeTrueLiteral(Literal lit, vector<unsigned int>& trueIds) {    
    unsigned int weight = removeLit(lit);
    removedTrueLiteral(weight, trueIds);
    return weight;
}

void Aggregate::removeFalseId(unsigned int pos) {
    assert(pos < numberOfIdBounds()); 
    bounds[pos] = bounds.back(); bounds.pop_back();
    ids[pos] = ids.back(); ids.pop_back();
}

void Aggregate::removedTrueLiteral(unsigned int weight, vector<unsigned int>& trueIds) {
    unsigned int j = 0;           
    for(unsigned int i = 0; i < bounds.size(); i++) {
        unsigned int bound = bounds[i];
        bounds[j] = bound-weight;        
        ids[j] = ids[i];
        if(bound <= weight) trueIds.push_back(getId(i));
        else j++;
    }
    bounds.resize(j);
    ids.resize(j);
}

void Aggregate::remap(const AtomData& atomData) {
    if(!isWeighted()) {
        for(unsigned int i = 0; i < literals.size(); i++) {
            unsigned int newVar = atomData.getNewVar(literals[i].var());
            assert(newVar != 0);
            literals[i].setVar(newVar);
        }
    }
    else
    {
        unsigned int twoDifferentWeights=0;
        assert(literals.size() == weights.size());
        for(unsigned int i = 0; i < literals.size(); i++) {
            if(twoDifferentWeights==0) twoDifferentWeights = weights[i];
            else if(twoDifferentWeights!=UINT_MAX && weights[i] != twoDifferentWeights) twoDifferentWeights = UINT_MAX;
            
            unsigned int newVar = atomData.getNewVar(literals[i].var());
            assert(newVar != 0);
            literals[i].setVar(newVar);
        }

        assert(twoDifferentWeights != 0);
        //They all have the same weight: thus this is a count.
        if(twoDifferentWeights != UINT_MAX) { 
            vector<unsigned int> tmp; weights.swap(tmp);
            for(unsigned int i = 0; i < numberOfIdBounds(); i++) {
                bounds[i] = twoDifferentWeights > bounds[i] ? 1 : ceil((float)bounds[i]/twoDifferentWeights); 
            }
        }
    }
}

void Aggregate::printGringo(ostream& out) const {
    if(isRemoved()) return;
    unsigned int countNeg = 0;
    for(unsigned int i = 0; i < literals.size(); i++) {
        if(literals[i].sign()) countNeg++;
    }

    assert(!isWeighted() || literals.size() == weights.size());

    for(unsigned int i = 0; i < numberOfIdBounds(); i++) {
        unsigned int id = getId(i);
        assert(id != 0);
        unsigned int bound = getBound(i);
        if(isWeighted()) out << "5 " << id << " " << bound << " " << literals.size() << " " << countNeg;            
        else out << "2 " << id << " " << literals.size() << " " << countNeg << " " << bound;            

        for(unsigned int i = 0; i < literals.size(); i++ ) out << " " << literals[i].var();    
        for(unsigned int i = 0; i < weights.size(); i++) out << " " << weights[i];
        out << endl;
    }
}

void Aggregate::removeUselessIdsAndBounds(vector<pair<Literal,Literal> >& equivalences) {
    bubblesort();
    if(weights.empty()) return;
    unordered_set<unsigned int> sumsSet;
    vector<unsigned int> possibleSums;
    possibleSums.push_back(0);
    for(unsigned int i = 0; i < weights.size(); i++) {
        unsigned size = possibleSums.size();
        for(unsigned int j = 0; j < size; j++) {
            unsigned int sum = possibleSums[j] + getWeight(i);
            if( sumsSet.insert(sum).second )
                possibleSums.push_back(sum);
        }
    }
    assert(!possibleSums.empty());
    unsigned int j = 0;
    for(unsigned int i = 0; i < bounds.size(); i++) {
        bounds[j] = bounds[i]; ids[j] = ids[i];        
        if(sumsSet.find(bounds[i]) != sumsSet.end()) { j++; continue; }
        if(i < bounds.size() - 1) equivalences.push_back(pair<Literal,Literal>(Literal(getId(i+1),false), Literal(getId(i),false)));
        else j++;
    }
    bounds.resize(j);
    ids.resize(j); 
}

template<class T>
void swap(vector<T>& elems, unsigned int j) { T tmp = elems[j];  elems[j] = elems[j+1];  elems[j+1] = tmp; }

void Aggregate::bubblesort() {
    assert( ids.size() == bounds.size() );    
    for(int i = ids.size()-1; i > 0; i--) {
        bool swaps = false;
        for(int j = 0; j < i; j++)
            if(bounds[j] > bounds[j+1]) { swap(ids, j); swap(bounds, j); swaps = true; }
        if(!swaps) break;
    }    
}

void Aggregate::replaceLiterals(unordered_map<int,int>& replacements) {
    for(unsigned int i = 0; i < literals.size(); i++) {
        if(replacements.find(literals[i].toInt()) != replacements.end()) literals[i] = Literal(replacements[literals[i].toInt()]);
        if(replacements.find(-literals[i].toInt()) != replacements.end()) literals[i] = Literal(-replacements[-literals[i].toInt()]); 
    }
}

}