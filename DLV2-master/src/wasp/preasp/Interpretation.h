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

#ifndef INTERPRETATION_H
#define	INTERPRETATION_H

#include "Literal.h"
#include <vector>
#include <cassert>
using namespace std;

namespace preasp {

class Interpretation {
    
    #define PREASP_UNDEFINED 0
    #define PREASP_TRUE 1
    #define PREASP_TRUE_SUPP 3
    #define PREASP_FALSE 8
    
    public:
        inline bool setTrue(unsigned int var) { 
            assert( var < interpretation.size() ); assert( !isTrue(var) );
            if(isFalse(var)) return false;
            interpretation[var] = PREASP_TRUE; return true;
        }
        
        inline bool setTrueSupported(unsigned int var) {
            assert( var < interpretation.size() ); assert( !isTrueSupported(var) );
            if(isFalse(var)) return false;
            interpretation[var] = PREASP_TRUE_SUPP; return true;
        }
        
        inline bool setFalse(unsigned int var) {
            assert( var < interpretation.size() ); assert( !isFalse(var) );
            if(isTrue(var)) return false;
            interpretation[var] = PREASP_FALSE; return true;
        }
        
        inline bool isTrue(unsigned int var) const { assert( var < interpretation.size() ); return interpretation[var] & 1; }
        inline bool isTrueSupported(unsigned int var) const { assert( var < interpretation.size() ); return interpretation[var] == PREASP_TRUE_SUPP; }
        inline bool isTrueNotSupported(unsigned int var) const { assert( var < interpretation.size() ); return interpretation[var] == PREASP_TRUE; }
        inline bool isFalse(unsigned int var) const { assert( var < interpretation.size() ); return interpretation[var] == PREASP_FALSE; }
        inline bool isUndefined(unsigned int var) const { assert( var < interpretation.size() ); return interpretation[var] == PREASP_UNDEFINED; }
        
        inline bool isTrue(Literal lit) const { return lit.sign() ? isFalse(lit.var()) : isTrue(lit.var()); }
        inline bool isFalse(Literal lit) const { return lit.sign() ? isTrue(lit.var()) : isFalse(lit.var()); }
        inline bool isUndefined(Literal lit) const { return isUndefined(lit.var()); }
        
        inline void init(unsigned int numberOfVars) { while (interpretation.size() <= numberOfVars) { interpretation.push_back(PREASP_UNDEFINED); } setFalse(0); }
        inline unsigned int size() const { return interpretation.size(); }

    private:
        vector<unsigned int> interpretation;
};

}

#endif
