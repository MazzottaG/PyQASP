/*******************************************************************************
 Copyright 2016 Francesco Calimeri, Davide Fusca', Simona Perri and Jessica Zangari
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
       http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 *******************************************************************************/

#ifndef PropositionalGrounder_h
#define PropositionalGrounder_h

#include "ProgramGrounder.h"


using namespace std;

namespace DLV2{

namespace grounder{

class GroundRuleGrounder
{
public:
	GroundRuleGrounder():gRule(0){};
	bool groundRule(Rule* r, unordered_set<index_object>* componentPredicateInHead=nullptr){
		unsigned sizeHead =r->getSizeHead();
		unsigned sizeBody=r->getSizeBody();
		gRule = (r->isWeakConstraint())? new WeakConstraint(true,sizeBody,r->getWeight(),r->getLevel(),r->getLabel())
				: new Rule(true,sizeHead,sizeBody);
		var_assignment fakeAss;

		for (unsigned i = 0; i < sizeBody; i++) {
			   Atom* atom=r->getAtomInBody(i);
			   Atom* gAtom = nullptr;
			   bool simply=false;
			   if(atom->isClassicalLiteral()){
					   if(!groundClassical(atom,gAtom)){
							   delete gAtom;
							   freeGRule();
							   return false;
					   }
					   simply=gAtom->isFact();
			   }else if(atom->isBuiltIn()){
					   simply=true;
					   if(!groundBuiltin(atom)){
							   freeGRule();
							   return false;
					   }
			   }else if(atom->isAggregateAtom()){
					   ResultEvaluation result = groundAggregate(atom,gAtom);
					   simply=(result==SATISFY);
			   }
			   gRule->setAtomInBody(i,gAtom);
			   gRule->setAtomToSimplifyInBody(i, simply);
		}

		unsigned numFacts=0;
		if(r->isChoiceRule()){
			   Atom *gAtom = nullptr;
			   groundChoice(r->getAtomInHead(0),gAtom);
			   gRule->setAtomInHead(0,gAtom);
		}else{
				for (unsigned i = 0; i < sizeHead; i++) {
						Atom* atomToInsert = getAtomToInsert(r->getAtomInHead(i),!gRule->areThereUndefinedAtomInBody() && !r->isChoiceRule() && r->getSizeHead() <=1 );
						gRule->setAtomInHead(i,atomToInsert);
						if(atomToInsert->isFact())
						 numFacts++;
				}
		}
		if(!(numFacts>=1 && r->getSizeHead()>1))
		OutputBuilder::getInstance()->onRule(gRule);
		freeGRule();
		return true;
	}
private:
    ///Given an atom if exist return the atom in the predicate extension otherwise insert it in a
    /// predicate extension. If setFact is true insert as a fact if not exist otherwise set FACT in the predicate extension
    Atom* getAtomToInsert(Atom* a,bool setFact=false);

    inline void  freeGRule(){
    	gRule->deleteGroundRule();
    }

    ///Given a ClassicalLiteral return a ground classical literal gAtom to insert in the groun Rule and check if is not negative and true
    inline bool groundClassical(Atom* atom,Atom*& gAtom){
		gAtom = getAtomToInsert(atom);
		if(gAtom->isNegative() && gAtom->isFact())
				return false;
		return true;
    };

    ///Given a builtin check if the builtin is true
    inline bool groundBuiltin(Atom* atom){
		var_assignment fakeAss;
		if(!atom->groundAndEvaluate(fakeAss))
				return false;
		return true;
    };

    ///Given an Aggregate return the ground aggregate to insert in the ground rule and check if is satisfiable
    ResultEvaluation groundAggregate(Atom* atom,Atom*& gAtom);

    ///Given a choice atom return a choice atom to insert in the ground rule
    void groundChoice(Atom* atom,Atom*& gAtom);

    Rule * gRule;

};

class PropositionalGrounder : public ProgramGrounder
{     

public:
	PropositionalGrounder():ProgramGrounder(){};
	~PropositionalGrounder(){  }

	virtual void ground();
	virtual void inizialize(Rule* rule, unordered_set<index_object>* componentPredicateInHead) {};
	virtual bool match() {return true;};
	virtual bool next() {return true;};
	virtual bool foundAssignment() {return true;};
	virtual bool back() {return true;};
	virtual bool groundCartesian(Rule *r,unordered_set<index_object>* componentPredicateInHead){return true;}
	virtual bool groundRule(Rule* r, unordered_set<index_object>* componentPredicateInHead=nullptr);

};

}
}

#endif // PropositionalGrounder_h
