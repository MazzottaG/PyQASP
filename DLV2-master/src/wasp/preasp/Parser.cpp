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

#include "Parser.h"

namespace preasp {

Parser::Parser(StreamBuffer& i, Builder& b) : in(i), builder(b) {}

void Parser::parseGringo() {        
    for (;;) {
        skipWhitespace(in);
        if (*in == EOF) break;
        if(!parseRule(parseInt(in)))
            break;
    }
    builder.endParse();
}

bool Parser::parseRule(int id){
    switch(id) {
        case 1: readNormalRule(); break;
        case 2: readCount(); break;
        case 3: readChoiceRule(); break;
        case 5: readSum(); break;
        case 6: readMinimization(); break;
        case 8: readDisjunctiveRule(); break;
        case 0: readOtherStuff(); break;
        default: fprintf(stderr, "PARSE ERROR! Unexpected symbol\n"), exit(3);
    }    
    return true;
}

int Parser::readVar() {
    int var = parseInt(in);
    builder.addVariable(var);
    return var;
}

int Parser::readHead(vector<Literal>& lits) {
    int size = parseInt(in);
    for(int i = 0; i < size; i++) lits.push_back(Literal(readVar(), false));   
    return size;
}

void Parser::readBody(vector<Literal>& lits) {
    int bodySize = parseInt(in);
    int negativeSize = parseInt(in);
    int i = 0;
    for(; i < negativeSize; i++) lits.push_back(Literal(readVar(), true));
    for(; i < bodySize; i++) lits.push_back(Literal(readVar(), false));
}

void Parser::readNormalRule() {
    vector<Literal> lits;     int headAtom = readVar();
    if(headAtom != 1) lits.push_back(Literal(headAtom, false));
    readBody(lits);
    builder.addRule(lits, headAtom == 1 ? 0 : 1, Builder::NORMAL_RULE);    
}

void Parser::readChoiceRule() {
    vector<Literal> lits;     int headSize = readHead(lits);     readBody(lits);
    builder.addRule(lits, headSize, Builder::CHOICE_RULE);
}

void Parser::readDisjunctiveRule() {
    vector<Literal> lits;     int headSize = readHead(lits);     readBody(lits);    
    builder.addRule(lits, headSize, Builder::DISJUNCTIVE_RULE);
}

void Parser::readAggregateSet(vector<Literal>& lits, unsigned int size, unsigned int negativeSize) {
    unsigned int i = 0;
    for(; i < negativeSize; i++) lits.push_back(Literal(readVar(), true));
    for(; i < size; i++) lits.push_back(Literal(readVar(), false));
}

void Parser::readWeights(vector<unsigned int>& weights, unsigned int size, unsigned int bound) {
    for(unsigned int i = 0; i < size; i++) {
        int w1 = parseInt(in);
        if(w1 <= 0) { fprintf(stderr, "PARSE ERROR! Unexpected weight <= 0 in aggregate set\n"), exit(3); }
        unsigned int w = w1;
        if(w > bound) w = bound;
        weights.push_back(w);
    }    
}

void Parser::readCount() {
    unsigned int id = readVar();
    unsigned int size = parseInt(in);
    unsigned int negativeSize = parseInt(in);
    unsigned int bound = parseInt(in);
    vector<Literal> lits;
    vector<unsigned int> weights;
    readAggregateSet(lits, size, negativeSize);
    while(weights.size() != lits.size()) weights.push_back(1);
    
    if(size < bound) { lits.clear(); lits.push_back(Literal(id, false)); builder.addRule(lits, 0, Builder::NORMAL_RULE); }
    else builder.addAggregate(id, lits, weights, bound);
}

void Parser::readSum() {
    unsigned int id = readVar();
    unsigned int bound = parseInt(in);
    unsigned int size = parseInt(in);
    unsigned int negativeSize = parseInt(in);
    vector<Literal> lits;
    vector<unsigned int> weights;
    readAggregateSet(lits, size, negativeSize);    
    readWeights(weights, size, bound);
    if(weights.size() != lits.size()) { fprintf(stderr, "PARSE ERROR! Each literal in the sum must have a weight\n"), exit(3); }
    builder.addAggregate(id, lits, weights, bound);    
}
    
void Parser::readMinimization() {
    unsigned int zero = parseInt(in);
    if(zero != 0) { fprintf(stderr, "Expected 0\n"), exit(3); }
    unsigned int size = parseInt(in);
    unsigned int negativeSize = parseInt(in);
    vector<Literal> lits;
    vector<unsigned int> weights;
    readAggregateSet(lits, size, negativeSize);    
    for(unsigned int i = 0; i < size; i++) {
        int w = parseInt(in);
        if(w < 0) fprintf(stderr, "PARSE ERROR! Unexpected weight < 0 in minimize constraint\n"), exit(3);
        weights.push_back(w);
    }
    builder.addMinimizeConstraint(lits, weights);
}

void Parser::readAtomTable() {
    int val = parseInt(in);
    while( val != 0 ) {
        builder.addVariable(val);
        string name = parseString(in);
        builder.setVarName(val, name);
        val = parseInt(in);
    }
}

void Parser::readOtherStuff() {
    readAtomTable();    
    readTrueAtoms();
    readFalseAtoms();    
    int nbOfModels = parseInt(in);
    if(nbOfModels != 1) fprintf(stderr, "Warning: ignored number of models > 1\n");
}

void Parser::readTrueAtoms() {
    if(!matchString(in, "B+")) fprintf(stderr, "PARSE ERROR! Expected B+\n"), exit(3);
    int val = parseInt(in);
    while( val != 0 ) {
        builder.addVariable(val);
        vector<Literal> lits;
        lits.push_back(Literal(val, true));
        builder.addRule(lits, 0, Builder::NORMAL_RULE);
        val = parseInt(in);
    }
}

void Parser::readFalseAtoms() {
    if(!matchString(in, "B-")) fprintf(stderr, "PARSE ERROR! Expected B-\n"), exit(3);
    int val = parseInt(in);
    while( val != 0 ) {
        builder.addVariable(val);
        vector<Literal> lits;
        lits.push_back(Literal(val, false));
        builder.addRule(lits, 0, Builder::NORMAL_RULE);
        val = parseInt(in);
    }
}

}
