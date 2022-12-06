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

#ifndef WEAKFACADE_H
#define	WEAKFACADE_H

#include "AbstractWeakStrategy.h"
#include "../../common/Options.h"
#include "One.h"
#include "Opt.h"
#include "PMRes.h"
#include "K.h"
#include <vector>
using namespace std;

class WaspFacade;

class OptLiteral {
    
    public:
        inline OptLiteral(Literal lit, uint64_t weight) : lit_(lit), weight_(weight) {}
        inline Literal lit() const { return lit_; }
        inline uint64_t weight() const { return weight_; }
        
    private:
        Literal lit_;
        uint64_t weight_;
};

class WeakFacade {
    
    public:
        inline WeakFacade() : runtime_(false) {}
        inline ~WeakFacade() {
            for(unsigned int i = 0; i < optLiterals.size(); i++)
                for(unsigned int j = 0; j < optLiterals[i].size(); j++)
                    delete optLiterals[i][j];
        }
        
        void addOptimizationLiteral(Literal lit, uint64_t weight, unsigned int level) {
            assert(!runtime_);
            while(level >= optLiterals.size()) optLiterals.push_back(vector<OptLiteral*>());            
            optLiterals[level].push_back(new OptLiteral(lit, weight));            
        }

        unsigned int solve(WaspFacade& wf) {
            runtime_=true;
            AbstractWeakStrategy* w = NULL;
            switch(DLV2::Options::weakConstraintsAlg) {
                case OPT: w = new Opt(wf); break;
                case BB: w = new Opt(wf, true); break;
                case PMRES: w = new PMRes(wf); break;
                case KALG: w = new K(wf); break;
                case ONE: w = new One(wf); break;
                default: assert(0); break;
            }
            unsigned int res = w->solve();
            delete w;
            return res;
        }
        
        inline unsigned int numberOfLevels() const { return optLiterals.size(); }        
        inline unsigned int numberOfOptLiterals(unsigned int level_) const { assert(level_ < optLiterals.size()); return optLiterals[level_].size(); }
        inline OptLiteral& getOptLiteral(unsigned int pos, unsigned int level_ ) { assert(level_ < optLiterals.size()); assert(pos < optLiterals[level_].size()); return *optLiterals[level_][pos]; }
        
    private:
        vector< vector<OptLiteral*> > optLiterals;        
        bool runtime_;
        inline WeakFacade(const WeakFacade& orig);
};

#endif

