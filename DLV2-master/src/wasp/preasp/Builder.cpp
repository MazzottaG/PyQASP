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

#include "Builder.h"
#include "Simplifications.h"
#include "Rule.h"
#include "Aggregate.h"
#include "MinimizeConstraint.h"
#include "DependencyGraph.h"
#include "../util/Statistics.h"

#include <algorithm>
#include <unordered_set>
using namespace std;

namespace preasp {

void Builder::addRule(vector<Literal>& lits, unsigned int headSize, RuleType type) {
    Rule* rule = NULL;
    if(type == NORMAL_RULE) {
        headSize==0 ? statistics( NULL, readConstraint() ) : statistics( NULL, readNormalRule() );
        assert(headSize == 0 || headSize == 1);
        if(isNormalRuleUseless(lits, headSize == 0)) return;
        if(headSize == 1 && lits.size() != 1 && !checkRule(lits, headSize, false)) return;
        if(lits.size() != 1) {
            if(headSize == 0 && lits.size() == 2) {
                if(lits[0].toInt() == lits[1].toInt()) program.falseAtoms.push_back(lits[0].var());
                else if(lits[0].toInt() != -lits[1].toInt()) program.binaryConstraints.push_back(new BinaryConstraint(lits[0], lits[1]));
                return;
            }
            rule = new Rule(lits, headSize, false);
        }
        else {
            if(headSize == 1) program.facts.push_back(lits[0].var());
            else if(lits[0].sign()) program.trueAtoms.push_back(lits[0].var());                
            else program.falseAtoms.push_back(lits[0].var());
            return;
        }                
    }
    else if(type == DISJUNCTIVE_RULE) {
        statistics( NULL, readDisjunctiveRule() );
        if(checkRule(lits, headSize, false)) rule = new Rule(lits, headSize, false);
        else return;
    }
    else if(type == CHOICE_RULE) {
        statistics( NULL, readChoiceRule() );
        if(checkRule(lits, headSize, true)) rule = new Rule(lits, headSize, true);
        else return;
    }
    else fprintf(stderr, "BUILDER ERROR! Unsupported rule\n"), exit(3);
    program.rules.push_back(rule);
}

void Builder::aggregateIsCountGT1(unsigned int id, const vector<Literal>& lits) {
    for(unsigned int i = 0; i < lits.size(); i++) program.rules.push_back(new Rule(Literal(id, false), lits[i], false));
}

void Builder::aggregateIsCountGT2(unsigned int id, const vector<Literal>& lits) {
    for(unsigned int i = 0; i < lits.size() - 1; i++)
        for(unsigned int j = i+1; j < lits.size(); j++) program.rules.push_back(new Rule(Literal(id, false), lits[i], lits[j], false));
}

void Builder::addAggregate(unsigned int id, vector<Literal>& lits, vector<unsigned int>& weights, unsigned int bound) {
    weights.empty() ? statistics( NULL, readCount() ) : statistics( NULL, readSum() );
    if(bound == 0) { program.facts.push_back(id); return; }
    if(!checkDuplicatesAndRemoveFalse(id, lits, weights, bound, false)) return;
    if(program.options.rewriteAggregates()) {
        if(weights.empty()) {
            if(bound == 1) { aggregateIsCountGT1( id, lits ); return; }
            if(bound == 2 && lits.size() <= 6) { aggregateIsCountGT2( id, lits ); return; }
        }
    }
    if(!program.options.useDictionaryForAggregates()) { program.aggregates.push_back(new Aggregate(lits, weights)); program.aggregates.back()->addBound(id, bound); return; }
    aggregatesTrie.startInsertion();
    for( unsigned int i = 0; i < lits.size(); i++) {
        aggregatesTrie.addElement(lits[i].toInt());
        aggregatesTrie.addElement(weights.empty() ? 1 : weights[i]);
    }
    unsigned int pos = aggregatesTrie.endInsertionGetLabel();
    if( pos == UINT_MAX ) { pos = program.aggregates.size(); program.aggregates.push_back(new Aggregate(lits, weights)); }  
    else --pos;
    assert(pos < program.aggregates.size());
    unsigned int res = program.aggregates[pos]->addBound( id, bound );
    if(res != UINT_MAX) aggregateEquivalences.push_back(pair<Literal,Literal>(Literal(res,false), Literal(id,false)));
    aggregatesIds.push_back(Literal(id, false));
}

void Builder::addMinimizeConstraint(vector<Literal>& lits, vector<unsigned int>& weights) {
    statistics( NULL, readWeakConstraints(lits.size()) );    
    checkDuplicatesAndRemoveFalse(0, lits, weights, 0, true);
    MinimizeConstraint* minimize = new MinimizeConstraint(lits, weights);
    program.minimizeConstraints.push_back(minimize);
}

bool Builder::checkDuplicatesAndRemoveFalse(unsigned int id, vector<Literal>& lits, vector<unsigned int>& weights, unsigned int bound, bool isMinimization) {
    unsigned int j = 0;
    unordered_map<int, unsigned int> litToPos;
    bool isCount = true;    
    assert(lits.size() == weights.size());
    unsigned int sum = 0;
    for(unsigned int i = 0; i < lits.size(); i++) {
        if(!isMinimization && weights[i] > bound) weights[i] = bound;
        lits[j] = lits[i];      weights[j] = weights[i];
        sum += weights[i];
        if(weights[j] > 1) isCount = false;
        int litId = lits[j].toInt();
        if(litToPos.find(litId) != litToPos.end()) {
            unsigned int pos = litToPos[litId];
            assert(pos < weights.size());
            weights[pos] += weights[j];
            isCount = false;
            continue;
        }
        litToPos[litId] = j;
        j++;
    }
    lits.resize(j);
    if(isCount && !isMinimization) { vector<unsigned int> tmp; weights.swap(tmp); }    
    else weights.resize(j);
    if(!isMinimization && sum < bound) { program.falseAtoms.push_back(id); return false; }
    return true;
}

bool Builder::isNormalRuleUseless(vector<Literal>& lits, bool isConstraint) {
    if( isConstraint ) {
        if(!program.options.useDictionaryForConstraints()) return false;
        unordered_set<int> elements;
        constraintsTrie.startInsertion();
        stable_sort(lits.begin(), lits.end());
        bool useless = false;
        unsigned int j = 0;
        for(unsigned int i = 0; i < lits.size(); i++) {
            lits[j] = lits[i];
            if(!elements.insert(lits[i].toInt()).second) continue;
            if(elements.find(-lits[i].toInt()) != elements.end()) useless = true;           
            if(constraintsTrie.addElement(lits[i].toInt()) || useless ) { constraintsTrie.endInsertion(); return true; }
            j++;
        }
        lits.resize(j);
        return constraintsTrie.endInsertion();        
    }
    else {
        if(!program.options.useDictionaryForRules()) return false;
        if(program.rules.size() >= 1000000 && !used) { rulesTrie.disable(); return false; }
        rulesTrie.startInsertion();
        for(unsigned int i = 0; i < lits.size(); i++) rulesTrie.addElement(lits[i].toInt());
        
        bool res = rulesTrie.endInsertion();
        if(res) used=true; 
        return res;
    }
}

void Builder::endParse() {    
    constraintsTrie.disable();
    rulesTrie.disable();
    aggregatesTrie.disable();
}

bool Builder::simplify() {
    Simplifications simp(program);    
    return simp.simplify(aggregateEquivalences, aggregatesIds);
}

void Builder::computeSCC() {
    if(program.options.verbosity() >= 1) cerr << "start computation of SCCs..." << endl;
    program.dependencyGraph.init(program.numberOfVars);
    for(unsigned int i = 0; i < program.rules.size(); i++) {
        Rule& r = *program.rules[i];
        assert(!r.isRemoved());
        if(r.isConstraint()) continue;
        
        if(r.isChoiceRule() && r.bodySize()==0)
            for(unsigned int k = 0; k < r.headSize(); k++) program.dependencyGraph.removeAllEdges(r.getHeadAtom(k));        
        
        for(unsigned int j = 0; j < r.bodySize(); j++) {
            if(r.getBodyLiteral(j).sign()) continue;
            unsigned int bodyVar = r.getBodyLiteral(j).var();
            for(unsigned int k = 0; k < r.headSize(); k++) program.dependencyGraph.addEdge(r.getHeadAtom(k), bodyVar);            
        }
    }
    program.dependencyGraph.computeSCC();    
    if(program.options.verbosity() >= 1) cerr << "...end computation of SCCs" << endl;
    program.checkHCF();
}

bool Builder::checkRule(vector<Literal>& lits, unsigned int& headSize, bool isChoiceRule) {
    if(!program.options.checkRule()) return true;
    assert(headSize <= lits.size());
    unordered_set<unsigned int> headAtoms;
    unordered_set<int> bodyLiterals;    
    unordered_set<unsigned int> toRemove;
    
    unsigned int numberOfRemoved = 0;
    unsigned int j = 0;
    for(unsigned int i = 0; i < headSize; i++) {
        lits[j] = lits[i];
        if(headAtoms.insert(lits[i].toInt()).second) j++;
        else ++numberOfRemoved;
    }
    
    for(unsigned int i = headSize; i < lits.size(); i++) {
        lits[j] = lits[i];
        if(headAtoms.find(lits[i].toInt()) != headAtoms.end()) {
            if(!isChoiceRule) return false;
            toRemove.insert(lits[i].var()); j++;
        }
        if(headAtoms.find(lits[i].var()) != headAtoms.end()) { toRemove.insert(lits[i].var()); j++; }
        else if(bodyLiterals.find(-lits[i].toInt()) != bodyLiterals.end()) return false;
        else if(bodyLiterals.insert(lits[i].toInt()).second) j++;        
    }
    lits.resize(j);
    
    headSize -= numberOfRemoved;
    if(toRemove.empty()) return true;
    
    j = 0;
    for(unsigned int i = 0; i < lits.size(); i++) {        
        lits[j] = lits[i];
        if(i < headSize && toRemove.find(lits[j].var()) != toRemove.end()) continue;
        j++;
    }
    lits.resize(j);
    assert(headSize >= toRemove.size());
    headSize -= toRemove.size();
    
    if(isChoiceRule && headSize==0) return false;
    return true;
}

}
