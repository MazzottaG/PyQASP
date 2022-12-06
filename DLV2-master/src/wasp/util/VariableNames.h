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

#ifndef WASP_VARIABLENAMES_H
#define WASP_VARIABLENAMES_H

#include <cassert>
#include <iostream>
#include <vector>
#include "WaspConstants.h"
using namespace std;

class VariableNames
{
    public:
        inline void addName(Var v, string name) { toPrint.push_back(v); stringToPrint.push_back(name); }
        inline unsigned int numberOfVisibleAtoms() const { return toPrint.size(); }
        inline Var getVisibleAtom(unsigned int pos) const { assert(pos < toPrint.size()); return toPrint[pos]; }
        inline const string& getVisibleAtomName(unsigned int pos) const { assert(pos < stringToPrint.size()); return stringToPrint[pos]; }
        inline bool isVisible(Var v) const {
            for(unsigned int i = 0; i < toPrint.size(); i++) if(toPrint[i] == v) return true;
            return false;
        }
        
        inline string getName(Var v) const {
            assert(isVisible(v));
            assert(toPrint.size() == stringToPrint.size());
            for(unsigned int i = 0; i < toPrint.size(); i++) if(toPrint[i] == v) return stringToPrint[i];
            return "";
        }
        
    private:
        vector<Var> toPrint;
        vector<string> stringToPrint;
};

#endif
