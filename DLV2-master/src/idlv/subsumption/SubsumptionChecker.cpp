/* 
 * File:   SubsumptionChecker.cpp
 * Author: cesco
 *
 * Created on 08/ott/2018, 11:46:21
 */

#include "SubsumptionChecker.h"
#include "RuleHashBuilder.h"
#include "../grounder/ground/StatementDependency.h"
#include "../grounder/atom/ClassicalLiteral.h"
#include "../grounder/term/FunctionTerm.h"
#include <cassert>
#include <queue>
#include <ctime>

namespace DLV2{ namespace grounder{

    void
    SubsumptionChecker::run(
        vector<Rule*>& rules ) {

        assert( rhb != nullptr );
        vector<RuleHashBuilder::hash_t> rulesHashCode;
        // Compute the hash codes of our rules.
        for( unsigned i=0;i<rules.size();i++ )
            rulesHashCode.push_back(buildHashCode(rules[i]));

        // Let h1 and h2 be the hash codes of two different rules, r1 and r2 respectively.
        // Let bAND be the bit-sequence resulting from the bitwise AND between h1 and h2.
        // If bAND is different from both h1 and h2, then r1 cannot be subsumed by r2 and viceversa.
        // Thus, we can avoid the actual check (that is hard). Otherwise, if bAND
        // is equal to h1 (resp. h2) then r2 (resp. r1) could be subsumed by r1 (resp. r2).
        unsigned naiveComparisons = 0;
        unsigned actualComparisons = 0;
        unsigned erasedRules = 0;
//        unsigned oldNumberOfRules = rules.size();
//        std::clock_t start;
//        double duration;
//        start = std::clock();

        vector<Rule*> updatedRules;
        unordered_set<size_t> idxRulesToBeErased;
        for( unsigned i=0;i<rulesHashCode.size();i++ ) {
            if( idxRulesToBeErased.find(i) == idxRulesToBeErased.end() ) {
                bool isSubsumed = false;
                for( unsigned j=i+1;j<rulesHashCode.size();j++ ) {
                    if( idxRulesToBeErased.find(j) == idxRulesToBeErased.end() ) {
                        naiveComparisons++;
                        RuleHashBuilder::hash_t hashRuleI = rulesHashCode[i];
                        RuleHashBuilder::hash_t hashRuleJ = rulesHashCode[j];
                        if( (hashRuleI&hashRuleJ) == hashRuleJ ) {
                            actualComparisons++;
                            if( isSubsumedBy(rules[i],rules[j]) )
                                {
                                isSubsumed = true;
                                erasedRules++;
                                idxRulesToBeErased.insert(i);
                                break;
                                }
                        }
                        if( (hashRuleI&hashRuleJ) == hashRuleI ) {
                            actualComparisons++;
                            if( isSubsumedBy(rules[j],rules[i]) )
                            {
                                erasedRules++;
                                idxRulesToBeErased.insert(j);
                            }
                        }
                    }
                }
                if( ! isSubsumed )
                    updatedRules.push_back(rules[i]);
            }
        }
        for( auto idx: idxRulesToBeErased ) {
            assert( idx < rules.size() );
            rules[idx]->free();
            delete rules[idx];
        }
        rules.erase(rules.begin(),rules.end());
        for( auto r: updatedRules )
            rules.push_back(r);

//        for( unsigned i=0;i<rulesHashCode.size();i++ ) {
//            for( unsigned j=i+1;j<rulesHashCode.size();j++ ) {
////                naiveComparisons++;
//                RuleHashBuilder::hash_t hashRuleI = rulesHashCode[i];
//                RuleHashBuilder::hash_t hashRuleJ = rulesHashCode[j];
//                if( (hashRuleI&hashRuleJ) == hashRuleI ) {
////                    actualComparisons++;
//                    // Check whether ruleJ is subsumed by ruleI
//                    if( isSubsumedBy(rules[j],rules[i])) {
//                        rules.erase(rules.begin()+j);
//                        rulesHashCode.erase(rulesHashCode.begin()+j);
////                        erasedRules++;
//                        continue;
//                    }
//                }
//                if( (hashRuleI&hashRuleJ) == hashRuleJ ) {
////                    actualComparisons++;
//                    // Check whether ruleI is subsumed by ruleJ
//                    if( isSubsumedBy(rules[i],rules[j]) ) {
//                        rules.erase(rules.begin()+i);
//                        rulesHashCode.erase(rulesHashCode.begin()+i);
////                        erasedRules++;
//                    }
//                }
//            }
//        }
        /*cerr << "Number of rules:" << oldNumberOfRules << endl;
        cerr << "Number of naive comparisons:" << naiveComparisons*2 << endl;
        cerr << "Number of actual comparisons:" << actualComparisons << endl;
        cerr << "Number of erased rules:" << erasedRules << endl;
        duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        std::cerr<<"Time (sec): "<< duration << endl;*/
    }

    RuleHashBuilder::hash_t
    SubsumptionChecker::buildHashCode(
        Rule* r ) {

        // The generation of the hash code of a given rule is delegated to a RuleHashBuilder.
        assert_msg( rhb != nullptr, "RuleHashBuilder not set!");
        rhb->newHash();
        const vector<Atom*>& head = r->getHead();
        for( unsigned i=0; i<head.size(); i++ ) {
            assert_msg(head[i]->isClassicalLiteral(),"Subsumption Check: only classical literals are supported!");
            onPredicateMethod predMethod = &RuleHashBuilder::onHeadPredicate;
            onConstantMethod constMethod = &RuleHashBuilder::onHeadConstant;
            onFunctionMethod functMethod = &RuleHashBuilder::onHeadFunction;
            buildHashCodeAtom(*head[i],predMethod,constMethod,functMethod);
        }
        const vector<Atom*>& body = r->getBody();
        for( unsigned i=0; i<body.size(); i++ ) {
            if( isPermissible(body[i]) ) {
                onPredicateMethod predMethod;
                onConstantMethod constMethod;
                onFunctionMethod functMethod;
                if( body[i]->isNegative() ) {
                    predMethod = &RuleHashBuilder::onBodyNegativePredicate;
                    constMethod = &RuleHashBuilder::onBodyNegativeConstant;
                    functMethod = &RuleHashBuilder::onBodyNegativeFunction;
                }
                else {
                    predMethod = &RuleHashBuilder::onBodyPositivePredicate;
                    constMethod = &RuleHashBuilder::onBodyPositiveConstant;
                    functMethod = &RuleHashBuilder::onBodyPositiveFunction;
                }
                buildHashCodeAtom(*body[i],predMethod,constMethod,functMethod);
            }
        }
        return rhb->getHash();

    }

    void
    SubsumptionChecker::buildHashCodeAtom(
        const Atom& a,
        onPredicateMethod predMethod,
        onConstantMethod constMethod,
        onFunctionMethod functMethod ) {

        (rhb->*predMethod)(a.getPredicate()->getIndex());
        for( unsigned j=0; j<a.getTermsSize(); j++ ) {
            Term* t = a.getTerm(j);
            if( t->getType() == TermType::NUMERIC_CONSTANT ||
                    t->getType() == STRING_CONSTANT ||
                    t->getType() == SYMBOLIC_CONSTANT )
                (rhb->*constMethod)(t->getIndex());
            else if( t->getType() == FUNCTION ) {
                queue<Term*> visit;
                visit.push(t);
                while( !visit.empty() ) {
                    Term* vt = visit.front();
                    visit.pop();
                    (rhb->*functMethod)(vt->getIndex());
                    for( auto nt: vt->getTerms() ) {
                        if( nt->getType() == TermType::NUMERIC_CONSTANT ||
                                nt->getType() == STRING_CONSTANT ||
                                nt->getType() == SYMBOLIC_CONSTANT )
                            (rhb->*constMethod)(nt->getIndex());
                        else if( nt->getType() == FUNCTION )
                            visit.push(nt);
                    }
                }
            }
        }

    }

    bool
    SubsumptionChecker::isSubsumedBy(
        Rule* r1,
        Rule* r2 ) {

        trace_action_tag(grounding,1, std::cerr<<"Is rule:"<<std::endl; r1->print(std::cerr);
            std::cerr<<"subsumed by"<<std::endl; r2->print(cout); );
        // To show that r1 is subsumed by r2, we need to find out a variable mapping
        // \sigma such that \sigma(r2) \subseteq r1. To do so, we replace each variable
        // in r1 by a fresh constant (to avoid cluttering among variable names) and then
        // we check whether it is possible to map every atom in r2 to an atom in r1.
        // P.S. Remember that every program P is actually a DNF formula \Phi where every rule
        // in P is a clause of \Phi. In the following, we denote by W the clause associated to
        // r1 and by C the clause related to r2.
        substitution sigma;
        clause W;
        freshConstantId = 0;
        const vector<Atom*>& headR1 = r1->getHead();
        for( unsigned i=0; i<headR1.size(); i++ ) {
            if( ! isPermissible(headR1[i]) ) {
                trace_msg(grounding,1,"Not permissible atom: subsumption check FAILED");
                for(auto itW=W.begin();itW!=W.end();itW++) delete (*itW);
                return false;
            }
            const Atom& a = *headR1[i];
            vector<Term*> substTermList;
            for( unsigned j=0; j<a.getTermsSize(); j++ ) {
                Term* t = a.getTerm(j);
                substTermList.push_back(getSubstitution(t,sigma));
            }
            // REMINDER: aPrime must be deleted at the end of the process.
            Atom* aPrime = new ClassicalLiteral(a.getPredicate(),substTermList,a.isHasMinus(),a.isNegative());
            W.push_back(aPrime);
        }
        const vector<Atom*>& bodyR1 = r1->getBody();
        for( unsigned i=0; i<bodyR1.size(); i++ ) {
            if( ! isPermissible(bodyR1[i]) ) {
                trace_msg(grounding,1,"Not permissible atom: subsumption check FAILED");
                for(auto itW=W.begin();itW!=W.end();itW++) delete (*itW);
                return false;
            }
            const Atom& a = *bodyR1[i];
            vector<Term*> substTermList;
            for( unsigned j=0; j<a.getTermsSize(); j++ ) {
                Term* t = a.getTerm(j);
                substTermList.push_back(getSubstitution(t,sigma));
            }
            // REMINDER: aPrime must be deleted at the end of the process.
            Atom* aPrime = new ClassicalLiteral(a.getPredicate(),substTermList,a.isHasMinus(),!(a.isNegative()));
            W.push_back(aPrime);
        }

        clause C;
        const vector<Atom*>& headR2 = r2->getHead();
        for( unsigned i=0; i<headR2.size(); i++ ) {
            if( ! isPermissible(headR2[i]) ) {
                trace_msg(grounding,1,"Not permissible atom: subsumption check FAILED");
                for(auto itC=C.begin();itC!=C.end();itC++) delete (*itC);
                for(auto itW=W.begin();itW!=W.end();itW++) delete (*itW);
                return false;
            }
            // REMINDER: aPrime must be deleted at the end of the process.
            Atom* aPrime = headR2[i]->clone();
            C.push_back(aPrime);
        }
        const vector<Atom*>& bodyR2 = r2->getBody();
        for( unsigned i=0; i<bodyR2.size(); i++ ) {
            if( ! isPermissible(bodyR2[i]) ) {
                trace_msg(grounding,1,"Not permissible atom: subsumption check FAILED");
                for(auto itC=C.begin();itC!=C.end();itC++) delete (*itC);
                for(auto itW=W.begin();itW!=W.end();itW++) delete (*itW);
                return false;
            }
            // REMINDER: aPrime must be deleted at the end of the process.
            Atom* aPrime = bodyR2[i]->clone();
            aPrime->setNegative(!(bodyR2[i]->isNegative()));
            C.push_back(aPrime);
        }
        sigma.clear();
        // Atoms occurring just once in the rule can be deterministically mapped.
        if( !simplifySingletonAtomsFromClause(C,W,sigma)) {
            trace_msg(grounding,1,"Subsumption check FAILED");
            for(auto itC=C.begin();itC!=C.end();itC++) delete (*itC);
            for(auto itW=W.begin();itW!=W.end();itW++) delete (*itW);
            return false;
        }
        bool check = checkClause(C,C.begin(),W,sigma);
        trace_msg(grounding,1,(check?"Subsumption check OK":"Subsumption check FAILED"));
        for(auto itC=C.begin();itC!=C.end();itC++) delete (*itC);
        for(auto itW=W.begin();itW!=W.end();itW++) delete (*itW);
        return check;
    }

    Term*
    SubsumptionChecker::getSubstitution(
        Term* t,
        substitution& sigma ) {

        // This function updates sigma by replacing every variable in t with a fresh constant.
        if( t->getType() == TermType::NUMERIC_CONSTANT ||
            t->getType() == TermType::STRING_CONSTANT ||
            t->getType() == TermType::SYMBOLIC_CONSTANT ){
            return t;
        }
        else if( t->getType() == TermType::VARIABLE ||
            t->getType() == TermType::ANONYMOUS ) {
            auto substTermIt = sigma.find(t);
            if( substTermIt == sigma.end() ) {
                // create a fresh constant
                Term* tPrime = new StringConstantTerm(false,INTERNAL_CONSTANT+std::to_string(freshConstantId));
                freshConstantId++;
                TermTable::getInstance()->addTerm(tPrime);
                sigma.insert({t,tPrime});
                return tPrime;
            }
            else {
                return substTermIt->second;
            }
        }
        else {
            assert_msg( t->getType() == TermType::FUNCTION, "Subsumption Check: only functions, variables and constants are supported!");
            vector<Term*> terms;
            for( auto subTerm:t->getTerms() )
                terms.push_back(getSubstitution(subTerm,sigma));
            string functorName = t->getName();
            Term* tPrime = new FunctionTerm(functorName,t->isNegative(),terms);
            TermTable::getInstance()->addTerm(tPrime);
            return tPrime;
        }

    }

    bool
    SubsumptionChecker::simplifySingletonAtomsFromClause(
        clause& C,
        const clause& W,
        substitution& sigma ) const {

        vector<clause::iterator> toRemove;
        for( auto itC=C.begin(); itC!=C.end(); itC++ ) {
            unsigned count = 0;
            clause::const_iterator lastCandidate = W.end();
            for( auto itW=W.begin(); itW!=W.end() && count<2; itW++ ) {
                if( (*itC)->getPredicate() == (*itW)->getPredicate() &&
                    (*itC)->getTerms().size() == (*itW)->getTerms().size() &&
                    (*itC)->isNegative() == (*itW)->isNegative() ) {
                    lastCandidate = itW;
                    count++;
                }
            }
            if( count == 0 )
                return false;
            if( count > 1 )
                continue;
            if( !matchingAtoms(*itC,*lastCandidate,sigma) )
                return false;
            toRemove.push_back(itC);
        }
        for( unsigned i=0; i<toRemove.size(); i++ ) {
            delete (*toRemove[i]);
            C.erase(toRemove[i]);
        }
        return true;
    }

    bool
    SubsumptionChecker::checkClause(
        const clause& C,
        clause::const_iterator itC,
        const clause& W,
        substitution& sigma ) const {

        // This function follows a backtracking paradigm to build a matching substitution between C and W.
        // It gets through the atoms in C by invoking itself recursively up to the end of C.
        // At each step, for the index-th atom in C, it guesses an atom candidate from W for matching.
        // The first branch of the computation which reaches the end of C returns true, thus stopping the exploration.
        if( itC == C.end() )
            return true;
        Atom* currAtom = *itC;
        for( auto it=W.begin(); it!= W.end(); it++ ) {
            // If this is a potential match then go on, otherwise try with another branch.
            if( currAtom->getPredicate() == (*it)->getPredicate() &&
                currAtom->getTerms().size() == (*it)->getTerms().size() &&
                currAtom->isNegative() == (*it)->isNegative() ) {
                // The maximum number of substitutions derived at every step is given by the maximum predicate arity.
                // Thus, we store the susbtitutions inferred at this step to retract them if any inconsistency is found.
                vector<Term*> currSubstKeys;
                clause::const_iterator oldItC = itC;
                if( matchingAtoms(currAtom,*it,sigma,&currSubstKeys) && checkClause(C,++itC,W,sigma) )
                    return true;
                // Retract chosen substitutions
                itC = oldItC;
                for( unsigned i=0; i<currSubstKeys.size(); i++ )
                    sigma.erase(currSubstKeys[i]);
            }
        }
        return false;

    }

    bool
    SubsumptionChecker::matchingAtoms(
        Atom* a1,
        Atom* a2,
        substitution& sigma,
        vector<Term*>* delta ) const {

        trace_action_tag(grounding,1,std::cerr<<"Trying to match"<<std::endl; a1->print(std::cerr);
            std::cerr<<" and "; a2->print(std::cerr); std::cerr<<std::endl;);

        assert( a1->getPredicate() == a2->getPredicate() &&
            a1->getTerms().size() == a2->getTerms().size() &&
            a1->isNegative() == a2->isNegative() );
        for( unsigned i=0; i<a1->getTerms().size(); i++ ) {
////            assert_msg( a1->getTerm(i)->getType() == TermType::VARIABLE ||
////                    a1->getTerm(i)->getType() == TermType::ANONYMOUS ||
////                    a1->getTerm(i)->getType() == TermType::NUMERIC_CONSTANT ||
////                    a1->getTerm(i)->getType() == TermType::STRING_CONSTANT ||
////                    a1->getTerm(i)->getType() == TermType::SYMBOLIC_CONSTANT,
////                    "Subsumption Check: only variables and constants are supported!" );
//            if( (a1->getTerm(i)->getType() == TermType::NUMERIC_CONSTANT ||
//                    a1->getTerm(i)->getType() == TermType::STRING_CONSTANT ||
//                    a1->getTerm(i)->getType() == TermType::SYMBOLIC_CONSTANT) &&
//                    a1->getTerm(i) != a2->getTerm(i) ) {
//                    trace_msg(grounding,1,"Match FAILED!");
//                    return false;
//            }
//            else if( a1->getTerm(i)->getType() == TermType::VARIABLE ) {
//                auto it = sigma.find(a1->getTerm(i));
//                if( it != sigma.end() && it->second != a2->getTerm(i)) {
//                    trace_msg(grounding,1,"Match FAILED!");
//                    return false;
//                }
//                else if( it == sigma.end() ) {
//                    sigma.insert({a1->getTerm(i),a2->getTerm(i)});
//                    if( delta != nullptr )
//                        delta->push_back(a1->getTerm(i));
//                }
            if( ! matchingTerms(a1->getTerm(i),a2->getTerm(i),sigma,delta) )
                return false;
        }
        trace_msg(grounding,1,"Match OK");
        return true;

    }

    bool
    SubsumptionChecker::matchingTerms(
        Term* t1,
        Term* t2,
        substitution& sigma,
        std::vector<Term*>* delta ) const {

        assert( t1 != nullptr && t2 != nullptr );
        if( (t1->getType() == TermType::NUMERIC_CONSTANT ||
                t1->getType() == TermType::STRING_CONSTANT ||
                t1->getType() == TermType::SYMBOLIC_CONSTANT) &&
                t1 != t2 ) {
            trace_msg(grounding,1,"Match FAILED!");
            return false;
        }
        else if( t1->getType() == TermType::VARIABLE ) {
            auto it = sigma.find(t1);
            if( it != sigma.end() && it->second != t2 ) {
                trace_msg(grounding,1,"Match FAILED!");
                return false;
            }
            else if( it == sigma.end() ) {
                sigma.insert({t1,t2});
                if( delta != nullptr )
                    delta->push_back(t1);
            }
        }
        else if( t1->getType() == TermType::FUNCTION ) {
            if( t2->getType() != TermType::FUNCTION || t1->getName() != t2->getName() || t1->getTermsSize() != t2->getTermsSize() ) {
                trace_msg(grounding,1,"Match FAILED!");
                return false;
            }
            else
                for( unsigned i=0; i<t1->getTermsSize(); i++ )
                    if( ! matchingTerms(t1->getTerms().at(i),t2->getTerms().at(i),sigma,delta) )
                        return false;
        }
        return true;
    }

    bool
    SubsumptionChecker::isPermissible(
        Atom* a ) const
    {
        assert( a != nullptr );
        return a->isClassicalLiteral();
    }

}}

