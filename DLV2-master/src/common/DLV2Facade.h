/*******************************************************************************
 * Copyright 2016 Francesco Calimeri, Davide Fusca', Simona Perri and Jessica Zangari
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/

/* 
 * File:   DLV2Facade.h
 * Author: cesco
 *
 * Created on 4 aprile 2014, 18.50
 */

#ifndef DLV2FACADE_H
#define	DLV2FACADE_H

#include "Options.h"
#include "../idlv/input/InputDirector.h"
#include "../wasp/WaspFacade.h"

namespace DLV2
{
    // TODO: discuss the issue of options and facade methods

    class DLV2Facade {
    public:
        DLV2Facade() : options(*Options::globalOptions()), builder(NULL), parserDuration(0), idlvTime(0), waspTime(0) { }
        ~DLV2Facade();

        void parseOptions( int, char* const* );
        void greetings();
        int readInput();
        int readWaspInput() { waspFacade.readInput(cin); return 0; }
        bool ground();
        void solve();
        
        inline void onFinish() {
            waspFacade.onFinish();
            printStatistics();                     
        }
        inline void onKill() {
            waspTime = ( clock() - waspInitTime ) / (double) CLOCKS_PER_SEC;  
            waspFacade.onKill();
            printStatistics();            
        }
        
        Options& getOptions() { return options; }
        
        void init();
        unsigned int printPreprocessing() { return waspFacade.printPre(); }
        void checkParacoherent();

    private:

        inline void printStatistics() {
            if(options.stats && options.mode == MODE_DLV2) {
                cerr << "\n---------------------------\n";
                cerr << "\nTime stats\n\n";
                cerr << "    Non-ground program parsing time: " << parserDuration << "s\n";
                cerr << "    Grounding time: " << idlvTime << "s\n";
                cerr << "    Solving time: " << waspTime << "s\n";                
                cerr << "\n";
            }
        }
        Options& options;

        InputDirector director;
        InputBuilder* builder;
        WaspFacade waspFacade;
        
        double parserDuration;
        double idlvTime;
        double waspTime;
        clock_t waspInitTime;

    }; 
    
}; 

#endif	/* DLV2FACADE_H */

