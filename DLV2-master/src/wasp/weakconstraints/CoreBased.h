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

#ifndef WASP_CORE_BASED
#define WASP_CORE_BASED

#include "AbstractWeakStrategy.h"
#include "../../common/Options.h"
#include "../Clause.h"
class WaspFacade;

class CoreBased : public AbstractWeakStrategy {
    public:
        inline CoreBased(WaspFacade& w) : AbstractWeakStrategy(w), limit(UINT64_MAX) {}
        virtual ~CoreBased() {}
        virtual unsigned int run(bool);

    protected:
        virtual bool foundUnsat(vector<Literal>& unsatCore, uint64_t minWeight) = 0;
        
    private:
        unsigned int runWeighted();
        unsigned int runUnweighted();
        bool disjointCorePreprocessing();
        void preprocessingWeights();
        void computeAssumptions(bool stratification, unsigned int size);        
        bool changeWeight();
        uint64_t computeMinWeight(const vector<Literal>& unsatCore);        
        
        virtual bool foundUnsat_(vector<Literal>& unsatCore);
        vector<uint64_t> weights;
        uint64_t limit;
        
 };

 #endif
