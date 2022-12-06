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

#include "PMRes.h"
#include "../WaspFacade.h"


/*
 * Adapted from aspino: https://github.com/alviano/aspino/
 */
bool PMRes::foundUnsat(vector<Literal>& unsatCore, uint64_t minWeight) {
    //Flip literals in order to uniform with aspino
    for(unsigned int i = 0; i < unsatCore.size(); i++) unsatCore[i]=unsatCore[i].getOppositeLiteral();

    vector<Literal> auxAtoms;
    Literal prec = Literal::null;
    while(unsatCore.size() > 0) {
        if(prec == Literal::null) prec = unsatCore.back().getOppositeLiteral();
        else {
            // disjunction
            Var v = addAuxVariable();
            auxAtoms.push_back(Literal(v, POSITIVE));
            addOptimizationLiteral(Literal(v,NEGATIVE),minWeight);
            if(!waspFacade.addClause(prec,unsatCore.back().getOppositeLiteral(),Literal(v, NEGATIVE))) return false;
            if(!waspFacade.addClause(prec.getOppositeLiteral(), Literal(v, POSITIVE))) return false;
            if(!waspFacade.addClause(unsatCore.back(), Literal(v, POSITIVE))) return false;

            if(unsatCore.size() > 1) {
                // conjunction
                v = addAuxVariable();
                if(!waspFacade.addClause(prec, Literal(v, NEGATIVE))) return false;
                if(!waspFacade.addClause(unsatCore.back().getOppositeLiteral(), Literal(v, NEGATIVE))) return false;
                if(!waspFacade.addClause(prec.getOppositeLiteral(),unsatCore.back(),Literal(v, POSITIVE))) return false;
                prec = Literal(v, POSITIVE);
            }
        }
        unsatCore.pop_back();
    }
    assert(unsatCore.size() == 0);
    return true;
}