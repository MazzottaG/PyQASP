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

#include "One.h"
#include "../WaspFacade.h"

One::~One()
{
}

bool One::foundUnsat(vector<Literal>& lits, uint64_t minWeight) {    
    unsigned int n = lits.size();
    unsigned int firstId = waspFacade.numberOfVariables()+1;
    for(unsigned int i=0; i < n-1; i++) {
        Var v = addAuxVariable();
        lits.push_back(Literal(v,POSITIVE));
        addOptimizationLiteral(Literal(v,POSITIVE),minWeight);
    }

    for(unsigned int i=waspFacade.numberOfVariables(); i > firstId; i--)
        waspFacade.addClause(Literal(i,NEGATIVE),Literal(i-1,POSITIVE));
    
    trace_action(optimization,2, {
        trace_tag(cerr,optimization,2);
        cerr << "Adding: #{";
        if(lits.size() > 0) {
        cerr << lits[0];
        for(unsigned int i=1; i < lits.size(); i++)
            cerr << ", " << lits[i];
        cerr << "} >=" << n-1;
        }
        cerr << endl;
    });
    return waspFacade.addCardinalityConstraint(lits, n-1);    
}
