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

#ifndef ATOMDATA_H
#define	ATOMDATA_H

#include "Type.h"

#include <vector>
using namespace std;

namespace preasp {

class AtomData
{
    public:
        AtomData() {}
        inline void init(unsigned int numberOfVars) {
            while (supportingRules.size() <= numberOfVars) {
                supportingRules.push_back(vector<unsigned int>());
                atomRules.push_back(vector<Type>());
                binaryConstraints.push_back(vector<Type>());
                additionalInfo.push_back(AdditionalInfo(0, false));                
            }
        }
        
        inline bool isPropagated(unsigned int var) const { assert(var < additionalInfo.size()); return additionalInfo[var].isPropagated(); }
        inline void setPropagated(unsigned int var) { assert(var < additionalInfo.size()); additionalInfo[var].setPropagated(); }
        
        inline bool isAggregateAtom(unsigned int var) const { assert(var < additionalInfo.size()); return additionalInfo[var].isAggregateAtom(); }
        inline void setAggregateAtom(unsigned int var, unsigned int id) { assert(var < additionalInfo.size()); additionalInfo[var].setAggregateAtom(); additionalInfo[var].setAggregateId(id); }
        
        inline bool isEliminated(unsigned int var) const { assert(var < additionalInfo.size()); return additionalInfo[var].isEliminated(); }
        inline void setEliminated(unsigned int var) { assert(var < additionalInfo.size()); additionalInfo[var].setEliminated(); }
        inline void addSupportingRule(unsigned int var, unsigned int id) { assert(var < supportingRules.size()); supportingRules[var].push_back(id); assert(var < additionalInfo.size()); additionalInfo[var].addSupportingRule(); }
        inline void removeSupportingRule(unsigned int var) { assert(var < additionalInfo.size()); additionalInfo[var].removeSupportingRule(); }
        inline unsigned int numberOfSupportingRule(unsigned int var) const { assert(var < additionalInfo.size()); return additionalInfo[var].numberOfSupporting(); }                
        inline unsigned int getSupportingRule(unsigned int var, unsigned int pos) const { assert(var < supportingRules.size() && pos < supportingRules[var].size()); return supportingRules[var][pos]; }        
        inline unsigned int supportingRuleSize(unsigned int var) const { assert(var < supportingRules.size()); return supportingRules[var].size(); }
        inline void clearSupportingRules(unsigned int var) {
            assert(var < supportingRules.size());
            vector<unsigned int> empty;
            supportingRules[var].swap(empty);
            assert(var < additionalInfo.size());
            additionalInfo[var].resetNumberOfSupporting();
        }

        
        inline void addPositiveBody(unsigned int var, unsigned int id) { assert(var < atomRules.size()); atomRules[var].push_back(Type(id, TYPE_POSITIVEBODY)); }
        inline void addNegativeBody(unsigned int var, unsigned int id) { assert(var < atomRules.size()); atomRules[var].push_back(Type(id, TYPE_NEGATIVEBODY)); }
        
        inline void addPositiveAggregate(unsigned int var, unsigned int id) { assert(var < atomRules.size()); atomRules[var].push_back(Type(id, TYPE_POSITIVEAGGREGATE)); }        
        inline void addNegativeAggregate(unsigned int var, unsigned int id) { assert(var < atomRules.size()); atomRules[var].push_back(Type(id, TYPE_NEGATIVEAGGREGATE)); }
                
        inline Type getRule(unsigned int var, unsigned int pos) const { assert(var < atomRules.size() && pos < atomRules[var].size()); return atomRules[var][pos]; }
        inline unsigned int rulesSize(unsigned int var) const { assert(var < atomRules.size()); return atomRules[var].size(); }
        inline void clearRules(unsigned int var) { assert(var < atomRules.size()); vector<Type> empty; atomRules[var].swap(empty); }
        
        inline unsigned int getAggregateId(unsigned int var) const { assert(isAggregateAtom(var)); assert(var < additionalInfo.size()); return additionalInfo[var].getAggregateId(); }
        
        inline void createRemapping(unsigned int numberOfVars) { for(unsigned int i = 0; i <= numberOfVars; i++) remapping.push_back(i); }
        inline void remap(unsigned int var, unsigned int newVar) { assert(var < remapping.size()); remapping[var]=newVar; }
        inline bool removed(unsigned int var) const { assert(var < remapping.size()); return remapping[var] == 0; }
        inline unsigned int getNewVar(unsigned int var) const { assert(var < remapping.size()); return remapping[var]; }
        
        inline bool isFounded(unsigned int var) const { assert(var < additionalInfo.size()); return additionalInfo[var].isFounded(); }
        inline void setFounded(unsigned int var) { assert(var < additionalInfo.size()); additionalInfo[var].setFounded(); }
        
        inline void addPositiveBinaryConstraint(unsigned int var, unsigned int id) { assert(var < binaryConstraints.size()); binaryConstraints[var].push_back(Type(id, TYPE_POSITIVEBODY)); }
        inline void addNegativeBinaryConstraint(unsigned int var, unsigned int id) { assert(var < binaryConstraints.size()); binaryConstraints[var].push_back(Type(id, TYPE_NEGATIVEBODY)); }
        
        inline Type getBinaryConstraint(unsigned int var, unsigned int pos) const { assert(var < binaryConstraints.size() && pos < binaryConstraints[var].size()); return binaryConstraints[var][pos]; }
        inline unsigned int binaryConstraintsSize(unsigned int var) const { assert(var < binaryConstraints.size()); return binaryConstraints[var].size(); }
        inline void clearBinaryConstraints(unsigned int var) { assert(var < binaryConstraints.size()); vector<Type> empty; binaryConstraints[var].swap(empty); }
        
    private:
        vector< vector<unsigned int> > supportingRules;
        vector< vector<Type> > atomRules;
        vector< vector<Type> > binaryConstraints;
        vector<unsigned int> remapping;

        class AdditionalInfo{
            public:
                inline AdditionalInfo(unsigned num, bool prop) : numberOfSupporting_(num), propagated_(prop), aggregateAtom_(false), founded_(false), eliminated_(false) {}
                inline void addSupportingRule() { numberOfSupporting_++; }
                inline void removeSupportingRule() { assert(numberOfSupporting_ > 0); numberOfSupporting_--; }
                inline void resetNumberOfSupporting() { assert(!isAggregateAtom()); numberOfSupporting_ = 0; }
                inline unsigned int numberOfSupporting() const { return numberOfSupporting_; }
                inline void setAggregateId(unsigned int id) { numberOfSupporting_ = id; }
                inline unsigned int getAggregateId() const { return numberOfSupporting_; }
                inline void setPropagated() { propagated_ = 1; }
                inline bool isPropagated() const { return propagated_; }
                inline void setAggregateAtom() { aggregateAtom_ = 1; founded_ = 1; }
                inline bool isAggregateAtom() const { return aggregateAtom_; }
                inline void setFounded() { founded_ = 1; }
                inline bool isFounded() const { return founded_; }
                inline void setEliminated() { eliminated_ = 1; }
                inline bool isEliminated() const { return eliminated_; }

            private:
                unsigned int numberOfSupporting_ : 28;
                unsigned int propagated_ : 1;
                unsigned int aggregateAtom_ : 1;
                unsigned int founded_ : 1;
                unsigned int eliminated_ : 1;
        };
        vector<AdditionalInfo> additionalInfo;        
};

}

#endif
