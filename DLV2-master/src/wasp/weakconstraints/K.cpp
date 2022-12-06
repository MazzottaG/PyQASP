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

#include "K.h"
#include "../WaspFacade.h"

/*
 * Adapted from aspino: https://github.com/alviano/aspino/
 */
bool K::foundUnsat(vector<Literal>& unsatCore, uint64_t minWeight) {
    //Flip literals in order to uniform with aspino
    for(unsigned int i = 0; i < unsatCore.size(); i++) unsatCore[i]=unsatCore[i].getOppositeLiteral();
    int b, m, N;
    initCounters(b,m,N, unsatCore.size());

    Literal prec = Literal::null;
    for(;;) {
        assert(!unsatCore.empty());
        vector<Literal> cc;
        int i = N;
        if(prec != Literal::null) { cc.push_back(prec); i--; }
        for(; i > 0; i--) {
            if(unsatCore.empty()) break;
            cc.push_back(unsatCore.back().getOppositeLiteral());
            unsatCore.pop_back();
        }
        assert(!cc.empty());
        unsigned int bound = cc.size()-1;

        if(!unsatCore.empty()) bound++;
        for(unsigned int i = 0; i < bound; i++) {
            Var v = addAuxVariable();                
            cc.push_back(Literal(v, NEGATIVE));
            if(i != 0) { if(!waspFacade.addClause(Literal(v-1,NEGATIVE), Literal(v,POSITIVE))) return false; } // symmetry breaker
            if(i == 0 && !unsatCore.empty()) prec = Literal(v, POSITIVE);                
            else addOptimizationLiteral(Literal(v,NEGATIVE),minWeight);
        }
        if(!waspFacade.addCardinalityConstraint(cc, bound)) return false;

        if(unsatCore.empty()) break;
    }

    assert(unsatCore.empty());
    return true;
}

void K::initCounters(int& b, int& m, int& N, unsigned int size) {
    switch(kthreshold) {
        case 0:
            //Algorithm kdyn
            b=size <= 2 ? 8 : ceil(log10(size)*16);
        break;

        case 1:
            //Algorithm one
            b=INT_MAX;
            break;

        default:
            b=kthreshold*2;
            break;
    }
    m=ceil(2.0*size/(b-2.0));
    N=ceil((size+size-1+2*(m-1))/(m*2.0));
}
