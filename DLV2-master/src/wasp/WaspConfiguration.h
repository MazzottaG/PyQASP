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

#ifndef WASPCONFIGURATION_H
#define	WASPCONFIGURATION_H

#include <string>
#include <vector>
using namespace std;

#include "util/WaspConstants.h"

class Solver;

class WaspConfiguration {

    public:
        WaspConfiguration(Solver& s) : solver(s), learned(0), choices(0), count(0) {}        
        inline void onLearning() { learned++; updateConfiguration(); }
        inline void onChoice() { choices++; updateConfiguration(); }        

    private:
        void updateConfiguration();        
        void restoreOriginalParameters();
        void readConfiguration(string filename);

        void setOption(string option, string value);
        void invalidOption(string option, string value);

        Solver& solver;
        unsigned int learned;
        unsigned int choices;        
        unsigned int count;
        vector<string> updatedParameters;     
};

#endif