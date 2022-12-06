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

#ifndef RULEDATA_H
#define	RULEDATA_H

namespace preasp {

class RuleData {
    public:
        inline RuleData() : headSize_(0), isChoice_(0), removed_(0), canSupportHead_(1) {}
        inline RuleData(unsigned int headSize, bool isChoice) : headSize_(headSize), isChoice_(isChoice), removed_(0), canSupportHead_(1) {}
        inline RuleData(const RuleData& orig) : headSize_(orig.headSize_), isChoice_(orig.isChoice_), removed_(orig.removed_), canSupportHead_(orig.canSupportHead_) {}
        inline unsigned int headSize() const { return headSize_; }
        inline bool isChoice() const { return isChoice_; }
        inline bool removed() const { return removed_; }
        inline void remove() { removed_ = 1; }
        inline bool canSupportHead() const { return canSupportHead_; }
        inline void disableSupportForHead() { canSupportHead_ = 0; }
        
    private:
        unsigned int headSize_ : 29;
        unsigned int isChoice_ : 1;
        unsigned int removed_ : 1;
        unsigned int canSupportHead_ : 1;
};

}

#endif
