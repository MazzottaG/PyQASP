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

#ifndef WASP_OPT_H
#define WASP_OPT_H

#include "AbstractWeakStrategy.h"
class Aggregate;

class Opt : public AbstractWeakStrategy
{
    public:
        Opt(WaspFacade& w, bool disable=false) : AbstractWeakStrategy(w), disableprefchoices_(disable), aggregate(NULL), varId(0), numberOfModels(0), currentCost(0) {}
        unsigned int run(bool);
        bool updateOptimizationAggregate();
        bool completedLevel();

    private:
        bool disableprefchoices_;
        Aggregate* aggregate;
        Var varId;
        unsigned int numberOfModels;
        uint64_t currentCost;
        void createOptimizationAggregate();
        bool createAggregateFromOptimizationLiterals();
        bool createFalseAggregate(const vector<Literal>& literals, const vector<uint64_t>& weights, uint64_t bound);
        Aggregate* createAggregate(Var aggrId, const vector<Literal>& literals, const vector<uint64_t>& weights );
        bool processAndAddAggregate(Aggregate* aggregate, uint64_t bound);
        void addPreferredChoicesFromOptimizationLiterals();        
        
        uint64_t costOfOptimizationLiterals();
        bool operator()(Literal l1, Literal l2) const;        

};

#endif