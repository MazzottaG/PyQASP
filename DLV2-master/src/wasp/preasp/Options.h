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

#ifndef PREASP_OPTIONS_H
#define	PREASP_OPTIONS_H

#define PREASP_PRINT_GRINGO 0
#define PREASP_PRINT_GRINGO_ONE_LEVEL 1
#define PREASP_PRINT_EXTENDED_DIMACS 2
#define PREASP_PRINT_WASP 3
#define PREASP_PRINT_WBO 4
#define PREASP_WELL_FOUNDED_MODEL 5
#define PREASP_WELL_FOUNDED_MODEL_CAUTIOUS 6

#define PREASP_COMPLETION_PROPAGATOR 0
#define PREASP_COMPLETION_DISJUNCTION 1
#define PREASP_COMPLETION_AUTO 2

namespace preasp {
    
class Options {
    public:
        inline Options() {
            completion_ = PREASP_COMPLETION_AUTO;
            printOption_ = PREASP_PRINT_GRINGO;            
            rewriteAggregates_ = 1;
            useDictionaryForAggregates_ = 1;
            useDictionaryForConstraints_ = 0;
            useDictionaryForRules_ = 1;            
            verbosity_ = 0;
            ruleChecks_ = 1;
        }
        inline bool useDictionaryForAggregates() const { return useDictionaryForAggregates_; }
        inline bool useDictionaryForConstraints() const { return useDictionaryForConstraints_; }
        inline bool useDictionaryForRules() const { return useDictionaryForRules_; }        
        inline unsigned int verbosity() const { return verbosity_; }
        inline unsigned int printOption() const { return printOption_; }
        inline bool rewriteAggregates() const { return rewriteAggregates_; }
        inline void setPrintOption(unsigned int value) {
            if(value > PREASP_WELL_FOUNDED_MODEL_CAUTIOUS) return;            
            printOption_ = value;            
        }
        
        inline void setVerbosity(unsigned int value) { verbosity_ = value; }

        inline unsigned int completionMode() const { return completion_; }
        inline void setCompletionMode(unsigned int value) {
            if(value > PREASP_COMPLETION_AUTO) return;
            completion_ = value;
        }
        
        inline void disableRuleChecks() { ruleChecks_ = 0; }
        inline bool checkRule() const { return ruleChecks_; }
        
    private:
        uint32_t completion_ : 2;
        uint32_t printOption_ : 3;
        uint32_t rewriteAggregates_ : 1;
        uint32_t useDictionaryForAggregates_ : 1;
        uint32_t useDictionaryForConstraints_ : 1;
        uint32_t useDictionaryForRules_ : 1;
        uint32_t ruleChecks_ : 1;
        uint32_t verbosity_ : 22;         
};  
}
#endif

