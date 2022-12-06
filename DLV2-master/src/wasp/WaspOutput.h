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

#ifndef WASP_OUTPUT_H
#define WASP_OUTPUT_H

#include "util/WaspConstants.h"
#include "../common/Options.h"
extern int EXIT_CODE;

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

const string NOMODEL = "INCOHERENT";
const string NOMODEL_COMPETITION_OUTPUT = "INCONSISTENT";
const string ANSWER = "ANSWER";
const string ANSWER_THIRD_COMPETITION = "ANSWER SET FOUND";
const string WEIGHT_LEVEL_WEAKCONSTRAINT_SEPARATOR = "@";
const string COST = "COST";
const string OPTIMUM_STRING = "OPTIMUM";

class WaspOutput
{
    public:
        WaspOutput() : numberOfModels(0), first(true), disabled(false) {
            START_MODEL = "{";
            END_MODEL = "}\n";
            PRE_SEPARATOR = ", ";
            AFTER_SEPARATOR = "";                    
            GREETINGS = string(WASP_STRING) + "\n";
            INCO = NOMODEL + "\n";
            updateExitCode = false;
            printId = false;
            comment = "";
            PRE_NUMBER_OF_MODELS = "";
            AFTER_NUMBER_OF_MODELS = "";
            printNumberOfModels = false;   
            output = &cout;            
        }

        void initPolicy() {
            if(DLV2::Options::printOnlyOptimum) {
                DLV2::Options::printLastModelOnly = true;
            }
            if(DLV2::Options::printLastModelOnly) {
                output = &modelStream;
            }
            switch (DLV2::Options::waspOutputPolicy) {
                
                case COMPETITION_OUTPUT:
                    START_MODEL = ANSWER + "\n";
                    END_MODEL = "\n";
                    PRE_SEPARATOR = " ";
                    AFTER_SEPARATOR = ".";                    
                    GREETINGS = "";
                    INCO = NOMODEL_COMPETITION_OUTPUT + "\n";
                    updateExitCode = true;                    
                    comment = "%";                    
                    break;
                
                case SILENT_OUTPUT:
                    GREETINGS = "";
                    if(DLV2::Options::silent==1) {
                        printNumberOfModels = true;
                        PRE_NUMBER_OF_MODELS = "Model: ";
                        AFTER_NUMBER_OF_MODELS = "\n";
                    }
                    if(DLV2::Options::silent>=1) {
                        START_MODEL = "";
                        END_MODEL = "";
                        disabled = true;
                    }
                    break;
                case THIRD_COMPETITION_OUTPUT:
                    START_MODEL = ANSWER_THIRD_COMPETITION + "\n";
                    END_MODEL = "\n";
                    PRE_SEPARATOR = " ";
                    AFTER_SEPARATOR = ".";                    
                    GREETINGS = "";
                    INCO = NOMODEL_COMPETITION_OUTPUT + "\n";
                    updateExitCode = true;               
                    break;
                case MULTI:
                    break;
                case ID_OUTPUT:
                    START_MODEL = "s ";
                    END_MODEL = "\n";
                    PRE_SEPARATOR = "";
                    AFTER_SEPARATOR = "";                    
                    GREETINGS = "% " + string(WASP_STRING);
                    INCO = "0\n";
                    updateExitCode = false;
                    printId = true;
                    comment = "";                    
                    PRE_NUMBER_OF_MODELS = " Solution number:";
                    AFTER_NUMBER_OF_MODELS = "\n";
                    printNumberOfModels = false;
                    break;
                case NO_OUTPUT:
                    START_MODEL = "";
                    END_MODEL = "";
                    PRE_SEPARATOR = "";
                    AFTER_SEPARATOR = "";                    
                    GREETINGS = "";
                    INCO = "";
                    disabled = true;
                    break;                
                case WASP_OUTPUT:                    
                default:         
                    break;
            }
        }

        ~WaspOutput() {}
        void startModel() {  
            modelStream.str("");
            modelStream.clear();                     
            if(updateExitCode) EXIT_CODE = 10;
            first = true;
            ++numberOfModels;
            if(printNumberOfModels)
                (*output) << PRE_NUMBER_OF_MODELS << numberOfModels << AFTER_NUMBER_OF_MODELS;
            (*output) << START_MODEL;
        }

        void printVariable(Var v, const string& name, bool isTrue) {
            if(disabled)
                return;
            if(printId) {
                (*output) << (isTrue ? " " : " -" ) << v;
                return;
            }
            if(!isTrue) return;
            if(!first) (*output) << PRE_SEPARATOR;            
            else first = false;
            (*output) << name << AFTER_SEPARATOR;
        }

        void endModel() {
            (*output) << END_MODEL;            
        }

        void onProgramIncoherent() {
            if(updateExitCode) EXIT_CODE = 20;
            (*output) << INCO;
        }

        void greetings() {
            (*output) << GREETINGS;
        }
        void foundModelOptimization(const vector< uint64_t >& costs) {
            if(updateExitCode) EXIT_CODE = 10;
            (*output) << COST;
            for( int i = costs.size() - 1; i >= 0; --i )
                (*output) << " " << costs[ i ] << WEIGHT_LEVEL_WEAKCONSTRAINT_SEPARATOR << ( i + 1 );
            (*output) << endl;         
        }
        
        void optimumFound() {
            if(updateExitCode) EXIT_CODE = 30;
            numberOfModels = 0;
            (*output) << OPTIMUM_STRING << endl;        
        }
        void foundLowerBound(uint64_t lb) {
            if(DLV2::Options::printBounds)
                cout << "COST lower bound: " << lb << endl;
        }
        void foundUpperBound(uint64_t ub) {
            if(DLV2::Options::printBounds)
                cout << "COST upper bound: " << ub << endl; 
        }

        void onFinish() {
            if(DLV2::Options::silent > 1)
                cout << "Number of printed answers: " << numberOfModels << endl;
            else if(DLV2::Options::printLastModelOnly){
                cout << modelStream.str();
            }
        }
        void onKill() {
            if(DLV2::Options::printLastModelOnly){
                cout << modelStream.str();
            }
        }

        void disable() {
            disabled = true;
        }

        void enable() {
            disabled = false;
        }
    
        bool isDisabled() {
            return disabled;
        }

        void printOptimum() {
            if(!DLV2::Options::printOnlyOptimum) return;
            DLV2::Options::printLastModelOnly = false;
            cout << modelStream.str();
            output = &cout;
        }

    private:
        // To set
        string START_MODEL;
        string END_MODEL;
        string PRE_SEPARATOR;
        string AFTER_SEPARATOR;
        string GREETINGS;
        string INCO;
        bool updateExitCode;
        bool printId;
        string comment;
        string PRE_NUMBER_OF_MODELS;
        string AFTER_NUMBER_OF_MODELS;
        bool printNumberOfModels;

        // Default
        unsigned int numberOfModels;
        bool first;
        bool disabled;
        
        ostream* output;
        stringstream modelStream;
};

#endif
