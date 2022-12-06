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

#ifndef WASP_ABSTRACTWEAKSTRATEGY_H
#define WASP_ABSTRACTWEAKSTRATEGY_H

#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

#include "../Literal.h"
#include "../AnswerSetListener.h"
class WaspFacade;
class WeakFacade;

class AbstractWeakStrategy : public AnswerSetListener {
    public:
        AbstractWeakStrategy(WaspFacade& w);
        virtual ~AbstractWeakStrategy() {}
        unsigned int solve();
        void foundAnswerSet();

    protected:
        virtual unsigned int run(bool) = 0;
        bool hardening();

        WaspFacade& waspFacade;
        WeakFacade& weakFacade;
        vector<Literal> assumptions;
        
        inline void incrementLb(uint64_t cost) { assert(currentLevel < lb_.size()); lb_[currentLevel] += cost; }
        inline uint64_t lb() { assert(currentLevel < lb_.size()); return lb_[currentLevel]; }
        inline uint64_t ub() { assert(currentLevel < ub_.size()); return ub_[currentLevel]; }  
        Var addAuxVariable();
        
        inline void addOptimizationLiteral(Literal l, uint64_t weight) {
            assert(find(optLiterals.begin(), optLiterals.end(), l) == optLiterals.end());
            assert(soft.find(l.getId()) == soft.end() || l.getVariable() == 1);
            optLiterals.push_back(l);
            soft[l.getId()] = weight;
        }
        
        inline uint64_t weight(Literal l) const { assert(soft.find(l.getId()) != soft.end()); return soft.at(l.getId()); }        
        inline void removeWeight(Literal l, uint64_t weight_) { assert(soft.find(l.getId()) != soft.end()); assert(weight(l) >= weight_); soft[l.getId()] -= weight_; }
        
        void sortOptimizationLiterals();
        vector<Literal> optLiterals;
        
        void resetSolver();
    private:
        vector<uint64_t> lb_;
        vector<uint64_t> ub_;
        unordered_map<int, uint64_t> soft;        
        unsigned int currentLevel;
                
        uint64_t costOfAnswerSet(unsigned int level);
        bool createOptLiterals();           
};

#endif
