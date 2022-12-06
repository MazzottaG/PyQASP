/*
 * PreaspOutputBuilder.cpp
 *
 *  Created on: Feb 6, 2018
 *      Author: jessica
 */

#include "PreaspOutputBuilder.h"

namespace DLV2 {
namespace grounder {


void PreaspOutputBuilder::onRule(Rule* rule) {
	if(rule->getSizeHead()>=1 && !rule->isChoiceRule())
		onNormalOrDisjunctiveRule(rule);
	else if(rule->isChoiceRule())
		onChoiceRule(rule);
	else if (rule->isAStrongConstraint())
        onConstraint(rule);
    else if (rule->isWeakConstraint()){
    	onWeakConstraint(rule);
    }
}

void PreaspOutputBuilder::onNormalOrDisjunctiveRule(Rule* rule) {
	onHead(rule->getHead());
	onBody(rule);
    if(rule->getSizeHead()<=1)
    	preaspBuilder->addRule(lits, 1, preasp::Builder::NORMAL_RULE);
    else
    	preaspBuilder->addRule(lits, rule->getSizeHead(), preasp::Builder::DISJUNCTIVE_RULE);
    lits.clear();
}

void PreaspOutputBuilder::onChoiceRule(Rule* rule){
	onChoiceAtom(rule->getAtomInHead(0));
	onBody(rule);
    preaspBuilder->addRule(lits, rule->getAtomInHead(0)->getChoiceElementsSize(), preasp::Builder::CHOICE_RULE);
    lits.clear();
}

void PreaspOutputBuilder::onChoiceAtom(Atom* atom) {
	for(unsigned i = 0; i < atom->getChoiceElementsSize(); ++i)
		onClassicalLiteral(atom->getChoiceElement(i)->getAtom(0));
}

void PreaspOutputBuilder::onWeakConstraint(Rule* rule) {
	int level = rule->getLevel()->getConstantValue();
	unsigned idHead=0;
    unsigned body_size = rule->getSizeBody();
    Atom* firstPrinted = nullptr;
    Atom *atom;
    unsigned positive=0,negative=0;
    for (unsigned i = 0; i < body_size; i++) {
        if (rule->isAtomToSimplifyInBody(i))
        	continue;
        atom = rule->getAtomInBody(i);
        if (firstPrinted == nullptr)
        	firstPrinted = atom;
        if(atom->isNegative()) ++negative; else ++positive;
    }
    if (positive == 1 && negative == 0) {
        if (firstPrinted->isAggregateAtom())
            idHead=onAggregate(firstPrinted);
        else
        	idHead=firstPrinted->getIndex();
        preaspBuilder->addVariable(idHead);
    }
    else{
		vector<preasp::Literal> localLits;
		idHead = IdGenerator::getInstance()->getNewId(1);
		preaspBuilder->addVariable(idHead);
		localLits.push_back(preasp::Literal(idHead,false));
	    for (unsigned i = 0; i < body_size; i++) {
	        if (rule->isAtomToSimplifyInBody(i)) continue;
	        atom = rule->getAtomInBody(i);
	        if(atom->isAggregateAtom()){
	        	unsigned agg_pred = onAggregate(atom);
	    		preaspBuilder->addVariable(agg_pred);
	    		localLits.push_back(preasp::Literal(agg_pred,atom->isNegative()));
	        }
	        else{
	    		preaspBuilder->addVariable(atom->getIndex());
	    		localLits.push_back(preasp::Literal(atom->getIndex(),atom->isNegative()));
	        }
	    }
	    preaspBuilder->addRule(localLits,1,preasp::Builder::RuleType::NORMAL_RULE);
    }
	if (levelWeak.count(level)) {
		(*levelWeak[level]).second.push_back(make_tuple(idHead, rule->getWeight()->getConstantValue(), rule->getLabel()));
	} else {
		list<id_weight_label> list(1, make_tuple(idHead, rule->getWeight()->getConstantValue(), rule->getLabel()));
		pair_level_tuple_list pair = {level, list};
		weakLevelConstraints.push_back(pair);
		levelWeak[level] = prev(weakLevelConstraints.end());
	}
}

void PreaspOutputBuilder::onConstraint(Rule* rule) {
	onBody(rule);
    preaspBuilder->addRule(lits, 0, preasp::Builder::NORMAL_RULE);
    lits.clear();
}

void PreaspOutputBuilder::onHead(const vector<Atom*>& head) {
    for(unsigned i = 0; i < head.size(); ++i)
    	onClassicalLiteral(head[i]);
}

void PreaspOutputBuilder::onBody(Rule* rule) {
    unsigned body_size = rule->getSizeBody();
    for (unsigned i = 0; i < body_size; ++i) {
        if (rule->isAtomToSimplifyInBody(i))continue;
    	Atom* atom=rule->getAtomInBody(i);
        if (atom->isAggregateAtom()) {
            unsigned agg_pred = onAggregate(atom);
        	lits.push_back(preasp::Literal(agg_pred, atom->isNegative()));
        } else onClassicalLiteral(atom);
    }
}

void PreaspOutputBuilder::onClassicalLiteral(Atom *atom){
	preaspBuilder->addVariable(atom->getIndex());
	lits.push_back(preasp::Literal(atom->getIndex(), atom->isNegative()));
}

unsigned PreaspOutputBuilder::onAggregate(Atom* atom) {
    AggregateFunction function = atom->getAggregateFunction();
    if (function == COUNT || function == SUM) {
        return countSumAggregate(atom);
    } else if (function == MIN || function == MAX) {
        return maxMinAggregate(atom);
    }
    return 0;
}

unsigned PreaspOutputBuilder::countSumAggregate(Atom* atom) {
    int pred_first_binop = 0, pred_second_binop = 0;
    //TODO we can optimize scan the aggregate one time also if there is two guard, saving the body of the rule in string
    if (atom->getFirstBinop() != NONE_OP) {
        int bound = atom->getFirstGuard()->getConstantValue();
        if (atom->getFirstBinop() == LESS)bound++;
        if (atom->getAggregateFunction() == COUNT)
            pred_first_binop = onConstraintRule(atom, bound);
        else
            pred_first_binop = onWeightRule(atom, bound);
    }
    if (atom->getSecondBinop() != NONE_OP || atom->getFirstBinop() == EQUAL) {
        int bound;
        if (atom->getFirstBinop() == EQUAL)
            bound = atom->getFirstGuard()->getConstantValue() + 1;
        else
            bound = atom->getSecondGuard()->getConstantValue();
        if (atom->getSecondBinop() == LESS_OR_EQ)bound++;
        if (atom->getAggregateFunction() == COUNT)
            pred_second_binop = onConstraintRule(atom, bound);
        else
            pred_second_binop = onWeightRule(atom, bound);
    }
	vector<preasp::Literal> auxlits;
	unsigned index_aggregate=IdGenerator::getInstance()->getNewId(1);
	preaspBuilder->addVariable(index_aggregate);
	auxlits.push_back(preasp::Literal(index_aggregate,false));
	if(pred_second_binop!=0)
		auxlits.push_back(preasp::Literal(pred_second_binop,true));
	if(pred_first_binop!=0)
		auxlits.push_back(preasp::Literal(pred_first_binop,false));
	preaspBuilder->addRule(auxlits,1,preasp::Builder::NORMAL_RULE);
    return index_aggregate;
}

unsigned PreaspOutputBuilder::onConstraintRule(Atom* aggregateAtom, unsigned bound) {
    unsigned pred_id = IdGenerator::getInstance()->getNewId(1);
	vector<preasp::Literal> agglits;
    vector<unsigned int> weights;
    for (unsigned i = 0; i < aggregateAtom->getAggregateElementsSize(); ++i) {
    	// Due to I-DLV rewriting of aggregate atoms,
    	// each aggregate element contains at most a literal
    	Atom* atom = aggregateAtom->getAggregateElement(i)->getNafLiteral(0);
    	preaspBuilder->addVariable(atom->getIndex());
    	agglits.push_back(preasp::Literal(atom->getIndex(), atom->isNegative()));
    }
    while(weights.size() != aggregateAtom->getAggregateElementsSize()) weights.push_back(1);
    if(agglits.size() < bound) {
    	agglits.clear();
    	agglits.push_back(preasp::Literal(1, false));
    	preaspBuilder->addVariable(1);
    	pred_id = IdGenerator::getInstance()->getNewId(1);
    	agglits.push_back(preasp::Literal(pred_id, false));
    	preaspBuilder->addRule(agglits, 1, preasp::Builder::NORMAL_RULE);
    }
    else{
		pred_id = IdGenerator::getInstance()->getNewId(1);
		preaspBuilder->addVariable(pred_id);
		preaspBuilder->addAggregate(pred_id, agglits, weights, bound);
    }
	return pred_id;
}

void PreaspOutputBuilder::manageWeakConstraints(){
    weakLevelConstraints.sort([](const pair_level_tuple_list& l1, const pair_level_tuple_list & l2) {
        return l1.first < l2.first;
    });
    for (auto l : weakLevelConstraints) {
    	auto hash = [](const pair<int, vector < Term*>>&ele) {
    		size_t seed = 3;
    		seed ^= (ele.first + 0x9e3779b9 + (seed << 6) + (seed >> 2));
    		for (auto t : ele.second)
    			seed ^= t->getIndex() + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    		return seed;
    	};
    	auto equal = [](const pair<int, vector < Term*>>&ele1, const pair<int, vector < Term*>>&ele2) {
    		if (ele1.first != ele2.first)return false;
    		if (ele1.second.size() != ele2.second.size())return false;
    		for (unsigned i = 0; i < ele1.second.size(); i++)
    			if (ele1.second[i]->getIndex() != ele2.second[i]->getIndex())
    				return false;
    		return true;
    	};
        list<id_weight_label> listOfWeak=l.second;
        unordered_map < pair<int, vector < Term*>>, unsigned, decltype(hash), decltype(equal) > weightLabel(1, hash, equal);
        vector<vector < id_weight_label>> weaks;
        for (auto idWeightLabel : listOfWeak) {
            pair<int, vector < Term*>> pair = {get<1>(idWeightLabel), get<2>(idWeightLabel)};
            auto it = weightLabel.find(pair);
            if (it != weightLabel.end())
                weaks[it->second].push_back(idWeightLabel);
            else {
                weightLabel.insert({pair, weaks.size()});
                weaks.emplace_back(1, idWeightLabel);
            }
        }
		vector<unsigned int> weights;
		for (unsigned i = 0; i < weaks.size(); i++) {
			if (weaks[i].size() == 1)
				lits.push_back(preasp::Literal(get<0>(weaks[i][0]),false));
			else {
				vector<unsigned> aux;
				for (auto weak : weaks[i])
					aux.push_back(get<0>(weak));
				unsigned index_head=IdGenerator::getInstance()->getNewId(1);
                preaspBuilder->addVariable(index_head);
				for(auto id: aux){
					vector<preasp::Literal> auxlits;
					auxlits.push_back(preasp::Literal(index_head,false));
					auxlits.push_back(preasp::Literal(id,false));
					preaspBuilder->addRule(auxlits,1,preasp::Builder::NORMAL_RULE);
				}
				lits.push_back(preasp::Literal(index_head,false));
			}
			weights.push_back(get<1>(weaks[i][0]));
		}
		preaspBuilder->addMinimizeConstraint(lits, weights);
		lits.clear();
		weights.clear();
    }
}

void PreaspOutputBuilder::onEnd() {
	manageWeakConstraints();
	preaspBuilder->endParse();
	lits.clear();
	weakLevelConstraints.clear();
	levelWeak.clear();
}

unsigned PreaspOutputBuilder::onWeightRule(Atom* aggregateAtom, int bound) {
    unsigned pred_id = IdGenerator::getInstance()->getNewId(1);
    unsigned body_size = aggregateAtom->getAggregateElementsSize();
    vector<Atom*> negative, positive;
    vector<int> weight_negative, weight_positive;
    negative.reserve(body_size);
    positive.reserve(body_size);
    weight_negative.reserve(body_size);
    weight_positive.reserve(body_size);
    Atom *atom;
    for (unsigned i = 0; i < aggregateAtom->getAggregateElementsSize(); i++) {
        AggregateElement *element = aggregateAtom->getAggregateElement(i);
        atom = element->getNafLiteral(0);
        int weight = element->getTerm(0)->getConstantValue();
        if(weight ==  0) continue;
        if (!atom->isNegative() && weight > 0) {
            positive.push_back(atom);
            weight_positive.push_back(weight);
        } else {
            negative.push_back(atom);
            unsigned absweight = abs(weight);
            weight_negative.push_back(absweight);
            if (weight < 0)bound += absweight;
        }
    }

    vector<preasp::Literal> agglits;
    vector<unsigned int> aggweights;
    for(auto l:negative){
    	preaspBuilder->addVariable(l->getIndex());
    	agglits.push_back(preasp::Literal(l->getIndex(),true));
    }
    for(auto l:positive){
    	preaspBuilder->addVariable(l->getIndex());
    	agglits.push_back(preasp::Literal(l->getIndex(),false));
    }
    for(auto l:weight_negative)
    	aggweights.push_back(l);
    for(auto l:weight_positive)
    	aggweights.push_back(l);
    if(aggweights.size() != agglits.size())
    { fprintf(stderr, "PARSE ERROR! Each literal in the sum must have a weight\n"), exit(3); }
	preaspBuilder->addVariable(pred_id);
	preaspBuilder->addAggregate(pred_id, agglits, aggweights, bound);
    return pred_id;
}

unsigned PreaspOutputBuilder::maxMinAggregate(Atom* atom) {
    function<unsigned(Term*) > function;

    if (atom->getAggregateFunction() == MAX) {

        if (atom->getFirstBinop() != NONE_OP && atom->getFirstBinop() != Binop::EQUAL && atom->getSecondBinop() == NONE_OP) {
            function = [&](Term * term) {
                if (atom->getFirstBinop() == Binop::LESS && *term>*(atom->getFirstGuard()))return 1;
                if (atom->getFirstBinop() == Binop::LESS_OR_EQ && *term >= *(atom->getFirstGuard()))return 1;
                return 0;
            };
        } else if (atom->getSecondBinop() != NONE_OP && atom->getFirstBinop() == NONE_OP) {
            function = [&](Term * term) {
                if (atom->getSecondBinop() == Binop::LESS && *term >= *(atom->getSecondGuard()))return 2;
                if (atom->getSecondBinop() == Binop::LESS_OR_EQ && *term>*(atom->getSecondGuard()))return 2;
                return 0;
            };
        } else if (atom->getSecondBinop() != NONE_OP && atom->getFirstBinop() != NONE_OP) {
            function = [&](Term * term) {
                if (atom->getFirstBinop() == Binop::LESS && atom->getSecondBinop() == Binop::LESS && *term>*(atom->getFirstGuard()) && *term<*(atom->getSecondGuard()))return 1;
                if (atom->getFirstBinop() == Binop::LESS && atom->getSecondBinop() == Binop::LESS_OR_EQ && *term>*(atom->getFirstGuard()) && *term <= *(atom->getSecondGuard()))return 1;
                if (atom->getFirstBinop() == Binop::LESS_OR_EQ && atom->getSecondBinop() == Binop::LESS && *term >= *(atom->getFirstGuard()) && *term<*(atom->getSecondGuard()))return 1;
                if (atom->getFirstBinop() == Binop::LESS_OR_EQ && atom->getSecondBinop() == Binop::LESS_OR_EQ && *term >= *(atom->getFirstGuard()) && *term <= *(atom->getSecondGuard()))return 1;

                if (atom->getSecondBinop() == Binop::LESS && *term >= *(atom->getSecondGuard()))return 2;
                if (atom->getSecondBinop() == Binop::LESS_OR_EQ && *term>*(atom->getSecondGuard()))return 2;

                return 0;
            };
        } else if (atom->getFirstBinop() == Binop::EQUAL) {
            function = [&](Term * term) {
                if (*term == *(atom->getFirstGuard()))return 1;
                if (*term>*(atom->getFirstGuard()))return 2;
                return 0;
            };
        }

    } else if (atom->getAggregateFunction() == MIN) {
        if (atom->getFirstBinop() != NONE_OP && atom->getFirstBinop() != Binop::EQUAL && atom->getSecondBinop() == NONE_OP) {
            function = [&](Term * term) {
                if (atom->getFirstBinop() == Binop::LESS && *term <= *(atom->getFirstGuard()))return 2;
                if (atom->getFirstBinop() == Binop::LESS_OR_EQ && *term<*(atom->getFirstGuard()))return 2;
                return 0;
            };
        } else if (atom->getSecondBinop() != NONE_OP && atom->getFirstBinop() == NONE_OP) {
            function = [&](Term * term) {
                if (atom->getSecondBinop() == Binop::LESS && *term<*(atom->getSecondGuard()))return 1;
                if (atom->getSecondBinop() == Binop::LESS_OR_EQ && *term <= *(atom->getSecondGuard()))return 1;
                return 0;
            };
        } else if (atom->getSecondBinop() != NONE_OP && atom->getFirstBinop() != NONE_OP) {
            function = [&](Term * term) {
                if (atom->getFirstBinop() == Binop::LESS && atom->getSecondBinop() == Binop::LESS && *term>*(atom->getFirstGuard()) && *term<*(atom->getSecondGuard()))return 1;
                if (atom->getFirstBinop() == Binop::LESS && atom->getSecondBinop() == Binop::LESS_OR_EQ && *term>*(atom->getFirstGuard()) && *term <= *(atom->getSecondGuard()))return 1;
                if (atom->getFirstBinop() == Binop::LESS_OR_EQ && atom->getSecondBinop() == Binop::LESS && *term >= *(atom->getFirstGuard()) && *term<*(atom->getSecondGuard()))return 1;
                if (atom->getFirstBinop() == Binop::LESS_OR_EQ && atom->getSecondBinop() == Binop::LESS_OR_EQ && *term >= *(atom->getFirstGuard()) && *term <= *(atom->getSecondGuard()))return 1;

                if (atom->getFirstBinop() == Binop::LESS && *term <= *(atom->getFirstGuard()))return 2;
                if (atom->getFirstBinop() == Binop::LESS_OR_EQ && *term<*(atom->getFirstGuard()))return 2;

                return 0;
            };
        } else if (atom->getFirstBinop() == Binop::EQUAL) {
            function = [&](Term * term) {
                if (*term == *(atom->getFirstGuard()))return 1;
                if (*term<*(atom->getFirstGuard()))return 2;
                return 0;
            };
        }

    }

    unsigned positivePredicate = 0, negativePredicate = 0;
    IdGenerator* generator = IdGenerator::getInstance();
    for (unsigned i = 0; i < atom->getAggregateElementsSize(); ++i) {

        unsigned index_agg_atom = atom->getAggregateElement(i)->getNafLiteral(0)->getIndex();
        preaspBuilder->addVariable(index_agg_atom);
        Term *agg_term = atom->getAggregateElement(i)->getTerm(0);
        unsigned value = function(agg_term);
        if(value==1){
			if (positivePredicate == 0) positivePredicate = generator->getNewId(1);
			vector<preasp::Literal> auxlits;
			preaspBuilder->addVariable(positivePredicate);
			auxlits.push_back(preasp::Literal(positivePredicate,false));
			auxlits.push_back(preasp::Literal(index_agg_atom,false));
			preaspBuilder->addRule(auxlits, 1, preasp::Builder::NORMAL_RULE);

        }
        else if(value==2){
			if (negativePredicate == 0) negativePredicate = generator->getNewId(1);
			preaspBuilder->addVariable(negativePredicate);
			vector<preasp::Literal> auxlits;
			auxlits.push_back(preasp::Literal(negativePredicate,false));
			auxlits.push_back(preasp::Literal(index_agg_atom,false));
			preaspBuilder->addRule(auxlits, 1, preasp::Builder::NORMAL_RULE);
		}
    }
    vector<preasp::Literal> agglits;
    unsigned index_aggregate = generator->getNewId(1);
    preaspBuilder->addVariable(index_aggregate);
    agglits.push_back(preasp::Literal(index_aggregate,false));
    bool boolean = atom->getSecondBinop() != NONE_OP && atom->getFirstBinop() != NONE_OP && positivePredicate == 0;
    if(atom->getSecondBinop() == NONE_OP && atom->getFirstBinop() == Binop::EQUAL && positivePredicate == 0){
    	if(atom->getAggregateFunction()==MIN && atom->getFirstGuard()==TermTable::getInstance()->term_max)
    		boolean=false;
    	else if(atom->getAggregateFunction()==MAX && atom->getFirstGuard()==TermTable::getInstance()->term_min)
    		boolean=false;
    	else
    		boolean=true;
    }
    if (negativePredicate != 0)agglits.push_back(preasp::Literal(negativePredicate,true));
    if (positivePredicate != 0)agglits.push_back(preasp::Literal(positivePredicate,false));
    else if (boolean) {
    	unsigned fakeIdx = generator->getNewId(1);
    	preaspBuilder->addVariable(fakeIdx);
    	agglits.push_back(preasp::Literal(fakeIdx,false));
    }
    preaspBuilder->addRule(agglits, 1, preasp::Builder::NORMAL_RULE);
    return index_aggregate;
}

void PreaspOutputBuilder::appendToStreamAtomTable(Atom* atom, bool fact){
	if(!atom->getPredicate()->isHiddenForPrinting()){
		preaspBuilder->addVariable(atom->getIndex());
		preaspBuilder->setVarName(atom->getIndex(), atom->toString());
	}
}

void PreaspOutputBuilder::onFact(Atom* atom) {
	onClassicalLiteral(atom);
	preaspBuilder->addRule(lits,1,preasp::Builder::NORMAL_RULE);
	lits.clear();
}

void PreaspOutputBuilder::appendToDictionary(const vector<Term*>& terms,unsigned id) {
	string entry=to_string(id)+":(";
	for(auto t:terms){
		if(t->getType()==TermType::NUMERIC_CONSTANT)
			entry+=to_string(t->getConstantValue());
		else if(t->getType()!=TermType::STRING_CONSTANT){
			stringstream s;
			s<<"\"";
			t->print(s);
			s<<"\"";
			entry+=s.str();
		}
		else
			entry+=t->getName();
		entry+=",";
	}
	//preaspBuilder-> TODO Pass id, entry(without id?), True(if heuristic)/False(if propagator)
}



} /* namespace grounder */
} /* namespace DLV2 */

