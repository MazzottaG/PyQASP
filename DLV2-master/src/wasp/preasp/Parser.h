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

#ifndef PARSER_H
#define PARSER_H

#include "ParseUtils.h"
#include "Builder.h"
#include "Literal.h"

#include <vector>
using namespace std;

namespace preasp {

class Parser
{
    public:
        Parser(StreamBuffer& i, Builder& b);        
        void parseGringo();   

    private:
        inline Parser(const Parser& orig);
        bool parseRule(int id);
        void readCount();
        void readChoiceRule();
        void readNormalRule();
        void readSum();
        void readMinimization();
        void readDisjunctiveRule();
        void readAtomTable();
        void readOtherStuff();
        void readTrueAtoms();
        void readFalseAtoms();
        int readHead(vector<Literal>& lits);
        void readBody(vector<Literal>& lits);
        int readVar();
        void readAggregateSet(vector<Literal>& lits, unsigned int size, unsigned int negativeSize);
        void readWeights(vector<unsigned int>& weights, unsigned int size, unsigned int bound);
        
        StreamBuffer& in;
        Builder& builder;        
};

}

#endif
