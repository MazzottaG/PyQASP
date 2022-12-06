/* 
 * File:   SubsumptionChecker.h
 * Author: cesco
 *
 * Created on 04/ott/2018, 18:23:47
 */

#ifndef SUBSUMPTIONCHECKER_H_
#define SUBSUMPTIONCHECKER_H_

#include <cstddef>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <vector>

#include "RuleHashBuilder.h"

namespace DLV2{ namespace grounder{

    class StatementDependency;
    class RuleHashBuilder;
    class Term;
    class Atom;
    class Rule;

    typedef void (RuleHashBuilder::*onPredicateMethod)(unsigned);
    typedef void (RuleHashBuilder::*onConstantMethod)(unsigned);
    typedef void (RuleHashBuilder::*onFunctionMethod)(unsigned);

    typedef std::unordered_map<Term*,Term*> substitution;
    typedef std::list<Atom*> clause;

    class SubsumptionChecker {
    public:
        SubsumptionChecker(): rhb(nullptr), freshConstantId(0) { }
        virtual ~SubsumptionChecker() { }
        virtual void run( std::vector<Rule*>& rules );

        void setRuleHashBuilder( RuleHashBuilder* r ) { rhb = r; }
        RuleHashBuilder* getRuleHashBuilder() const { return rhb; }

    protected:
        RuleHashBuilder::hash_t buildHashCode( Rule* r );
        void buildHashCodeAtom(const Atom&, onPredicateMethod, onConstantMethod, onFunctionMethod);
        bool isSubsumedBy( Rule*, Rule* );
        Term* getSubstitution( Term* t, substitution& sigma );
        bool simplifySingletonAtomsFromClause( clause& C, const clause& W, substitution& sigma ) const;
        bool checkClause(const clause& C, clause::const_iterator itC, const clause& W, substitution& sigma ) const;
        bool matchingAtoms(Atom* a1, Atom* a2, substitution& sigma, std::vector<Term*>* delta=nullptr) const;
        bool matchingTerms(Term* t1, Term* t2, substitution& sigma, std::vector<Term*>* delta=nullptr) const;
        bool isPermissible( Atom* a ) const;

    private:
        RuleHashBuilder* rhb;
        int freshConstantId;

    };

}}


#endif /* SUBSUMPTIONCHECKER_H_ */
