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

#include <cstring>
#include <cstdlib>
#include <algorithm>
#include "SelectorBuilder.h"

using namespace std;
using namespace DLV2;

SelectorBuilder::SelectorBuilder():
        InputBuilder(), foundQuery(false), foundChoice(false),
        predName("") //, inBody(false)
{}

unsigned
SelectorBuilder::addPredicateInHash(
    string predname )
{
	unsigned id = pred2nodeId.size();

	unordered_map<string,unsigned>::iterator it = pred2nodeId.find(predname);
	if(it == pred2nodeId.end())
	{
		pred2nodeId[predname] = id;
		return id;
	}
	else
		return pred2nodeId[predname];
}

void
SelectorBuilder::cleanTemp()
{
	//inBody = false;
	head.clear();
	body.clear();
	predName="";
}

void
SelectorBuilder::addArc(
    unsigned f,
    unsigned t)
{
	if(grafo.find(f) == grafo.end())
	{
		vector<unsigned> a;
		a.push_back(t);
		grafo[f] = a;
	}
	else
	{
		grafo[f].push_back(t);
	}

}

void
SelectorBuilder::onRule()
{
	//cout << "rule" << endl;

	vector<unsigned> headInt; headInt.clear();

	for(vector<string>::iterator ith = head.begin(); ith != head.end(); ++ith)
	{
		unsigned aid = addPredicateInHash(*ith);
		headInt.push_back(aid);

		for(vector<Literal>::iterator it = body.begin(); it!= body.end() ;++it)
		{
			unsigned from = addPredicateInHash(it->predname);
			unsigned to = addPredicateInHash(*ith);
			if(it->sign == 0 && from != to)
				addArc(from,to);
		}
	}
	if(headInt.size()>1)
		heads.push_back(headInt);

	/*
	for(vector<unsigned>::iterator it = headInt.begin(); it != headInt.end(); ++it)
		cout << "HH"<< *it << endl;
	for(vector<unsigned>::iterator it = headInt.begin(); it != headInt.end(); ++it)
	{
		vector<vector<unsigned>>& heads = headsOf[*it];
		heads.push_back(headInt);
	}
	*/

	cleanTemp();
}

void
SelectorBuilder::onConstraint()
{
	cleanTemp();
}

void
SelectorBuilder::onWeakConstraint()
{
	cleanTemp();
}

void
SelectorBuilder::onQuery()
{
	foundQuery = true;
}

void
SelectorBuilder::onHeadAtom()
{
	head.push_back(predName);
	//cout << "push in head:"<< predName << endl;
}

void
SelectorBuilder::onBodyLiteral()
{
}

void
SelectorBuilder::onNafLiteral(
    bool naf )
{

	Literal l;
	l.predname = predName;
	l.sign = naf;
	body.push_back(l);
	//cout << "push in body:"<< predName << "sign" << naf << endl;
}

void
SelectorBuilder::onAtom(
    bool isStrongNeg )
{
}

void
SelectorBuilder::onExistentialAtom()
{
}

void
SelectorBuilder::onPredicateName(
    char* p )
{
	predName=p;
	//cout << "pred"<< p << endl;
}

void
SelectorBuilder::onExistentialVariable(
    char* v )
{
}

void 
SelectorBuilder::onEqualOperator()
{    
}

void 
SelectorBuilder::onUnequalOperator()
{
}

void
SelectorBuilder::onLessOperator()
{
}

void
SelectorBuilder::onLessOrEqualOperator()
{
}

void
SelectorBuilder::onGreaterOperator()
{
}

void 
SelectorBuilder::onGreaterOrEqualOperator()
{
}

void
SelectorBuilder::onTerm(
    char* t )
{
}

void
SelectorBuilder::onTerm(
    int int1 )
{
}

void
SelectorBuilder::onUnknownVariable() 
{

}

void
SelectorBuilder::onFunction(
    char* fs, 
    int int1)
{
}

void
SelectorBuilder::onTermDash()
{
}

void
SelectorBuilder::onTermParams()
{
}

void
SelectorBuilder::onArithmeticOperation(
    char char1 )
{
}

void
SelectorBuilder::onWeightAtLevels(
    int int1, 
    int int2, 
    int int3)
{
}

void
SelectorBuilder::onChoiceLowerGuard()
{
}

void
SelectorBuilder::onChoiceUpperGuard()
{
}

void
SelectorBuilder::onChoiceElementAtom()
{
	foundChoice = true;
	head.push_back(predName);
	//cout << "push in choicerule head:"<< predName << endl;
	//cout << "ch elem atom" << endl;
}

void
SelectorBuilder::onChoiceElementLiteral()
{
	//cout << "ch literal" << endl;
	//head.push_back(body.back().predname);
	body.pop_back();
}

void
SelectorBuilder::onChoiceElement()
{
	//cout << "ch eleent" << endl;
}

void
SelectorBuilder::onChoiceAtom()
{
	foundChoice = true;
	//cout << "choiuce atom" << endl;
}

void
SelectorBuilder::onBuiltinAtom()
{
}

void
SelectorBuilder::onAggregateLowerGuard()
{
}

void
SelectorBuilder::onAggregateUpperGuard()
{
}

void
SelectorBuilder::onAggregateFunction(char*)
{
}

void
SelectorBuilder::onAggregateGroundTerm(char*, bool dash)
{
}

void
SelectorBuilder::onAggregateVariableTerm(char*)
{
}

void
SelectorBuilder::onAggregateUnknownVariable()
{
    
}

void
SelectorBuilder::onAggregateNafLiteral()
{
}

void
SelectorBuilder::onAggregateElement()
{
}

void
SelectorBuilder::onAggregate(bool naf)
{
}

unsigned
SelectorBuilder::getSolverToCall()
{
	if(foundQuery || !HCF())
		return 1; //DLV

	return 0; // wasp
}

bool
SelectorBuilder::HCF()
{
	for(vector<vector<unsigned>>::iterator itH = heads.begin(); itH!= heads.end(); ++itH)
	{
//		if(itH->size()>1) // only for disj
		{
			for(vector<unsigned>::iterator itA = itH->begin(); itA!= itH->end(); ++itA)
			{
				if(reaches(*itA,*itH))
					return false;
			}
		}
	}
	return true;
}

bool
SelectorBuilder::reaches(unsigned start, vector<unsigned> goal)
{
	vector<unsigned> daVisitare;
	vector<unsigned> visitato;

	daVisitare.push_back(start);

	while(daVisitare.size() > 0)
	{
		unsigned v = daVisitare.back();
		daVisitare.pop_back();

		vector<unsigned> figli = grafo[v];
		for(unsigned i = 0; i < figli.size(); i++)
		{
			if(std::find(goal.begin(), goal.end(), figli[i]) != goal.end())
				return true;
			if(std::find(visitato.begin(), visitato.end(), figli[i]) == visitato.end())
				if(v != figli[i])
					daVisitare.push_back(figli[i]);
		}
		visitato.push_back(v);
	}

	return false;
}

SelectorBuilder::~SelectorBuilder()
{
	/*
	for(unordered_map<string,unsigned>::iterator it = pred2nodeId.begin();
			it != pred2nodeId.end(); it++)
	{
		cout << it->first << " " << it->second<< endl;
	}
	cout << endl;
	for(unordered_map<unsigned,vector<unsigned>>::iterator itg = grafo.begin();
			itg != grafo.end(); ++itg)
	{
		for(vector<unsigned>::iterator i = itg->second.begin(); i != itg->second.end(); ++i)
			cout << itg->first << " - " << *i<< endl;
	}
	*/
}
