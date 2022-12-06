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

#ifndef SIMPLIFICATIONS_H
#define	SIMPLIFICATIONS_H

#include <vector>
#include <iostream>
#include <string>

#include "Literal.h"
#include "Interpretation.h"
#include "Program.h"
#include "AtomData.h"
using namespace std;

namespace preasp {

class Simplifications
{
    public:
        Simplifications(Program& data);
        bool simplify(vector<pair<Literal,Literal> >& equivalences, vector<Literal>& aggregatesIds);
        
        inline const AtomData& getAtomData() const { return atomData; }         
        
    private:
        inline Simplifications(const Simplifications& orig);
        void createDataStructures();
        void analyzeBinaryConstraints();
        void analyzeRules();
        void analyzeAggregates();
        
        void checkEquivalences(vector<pair<Literal,Literal> >& equivalences);
        
        void propagate();
        void propagateFact(unsigned int);
        void propagateFalse(unsigned int);
        void propagateTrue(unsigned int);          
        
        inline void addSupportingRule(unsigned int var, unsigned int id) { atomData.addSupportingRule(var, id); }
        inline void addNegativeBody(unsigned int var, unsigned int id) { atomData.addNegativeBody(var, id); }
        inline void addPositiveBody(unsigned int var, unsigned int id) { atomData.addPositiveBody(var, id); }
        
        void createDataStructuresForRule(Rule& rule, unsigned int id, unsigned int numberOfTrueInTheHead );
        void createDataStructuresForAggregate(Aggregate& aggr, unsigned int id );
        void fire(Rule& rule, unsigned int id);
        void fire(Aggregate& aggr, unsigned int id);
        void inferBody(const Rule& rule);
        void inferHead(const Rule& rule);
        
        inline void doInferenceOnBinaryConstraint(const BinaryConstraint& constraint) { inferFalseLiteral(constraint.getLiteral(interpretation.isTrue(constraint.getLiteral(0)) ? 1 : 0)); }
        void bodyLiteralIsTrue(unsigned int id);
        void headAtomIsTrue(unsigned int var, unsigned int id);
        void headAtomIsFact(unsigned int var, unsigned int id, bool wasPropagated);
        void removeLiteralFromBody(Literal lit, unsigned int id, bool wasPropagated);
        void removeVarFromHead(unsigned int var, unsigned int id);
        void removeRule(unsigned int id);
        void removedSupportingRule(unsigned int var);
        
        void checkSupport(unsigned int var);
        void checkAggregateAtom(unsigned int var);
        
        void inferAllBodyTrue(unsigned int id);
        void inferAllHeadFalse(unsigned int var, unsigned int id);
        
        void inferTrueLiteral(Literal lit);
        void inferFalseLiteral(Literal lit);
        void inferFact(unsigned int var);
        
        void litIsTrueInAggregate(Literal lit, Aggregate& aggr, unsigned int id);
        void litIsFalseInAggregate(Literal lit, Aggregate& aggr, unsigned int id);
        
        bool checkFounded(const Rule& r, vector<unsigned int>& foundedVars);
        bool operatorR();
        
        void finalize();        
        
        bool ruleCanSupportVar(unsigned int var, unsigned int id) const;    
        
        vector<unsigned int> trueBodyCounter;
        vector<unsigned int> trueHeadSpecialCounter;
        
        vector<unsigned int> maxPossibleSum;
        vector<Rule*> rulesNotConstraint;
        vector<unsigned int> countPositiveFoundedBody;
        
        Interpretation interpretation;
        bool ok;
        Program& program;
        AtomData atomData;        
};

}

#endif
