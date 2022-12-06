/*
 * File:   MagicSets.h
 * Author: pierfrancesco
 *
 * Created on 30/ott/2018, 16:58:18
 */

#ifndef MAGICSETS_H_
#define MAGICSETS_H_


#include "../table/PredicateExtension.h"

namespace DLV2 { namespace grounder {

#define SEPARATOR_BODY ','
#define SEPARATOR_HEAD '|'
#define MAGIC_PREFIX "magic#"
#define MAGIC_ADORNMENT_SEPARATOR "#"
#define MAGIC_BOUND "b"
#define MAGIC_FREE "f"

class Rule;
class Atom;
class Term;

using binding_map = std::unordered_map<Term*, Predicate*, IndexForTable<Term> >;

class ADORNMENT_ITEM
    {
    friend ostream &operator<<( ostream &out, const ADORNMENT_ITEM &item );
public:
    virtual ~ADORNMENT_ITEM() {}

    virtual ADORNMENT_ITEM* clone() const = 0;

    virtual ostream &print( ostream &out ) const = 0;
    virtual bool isFullyBound() const = 0;
    virtual bool isFullyFree() const = 0;

    virtual void addBoundVariables( Predicate *pred, Term *term, set_term &boundVariables, binding_map &boundVarsToPred, bool comesFromHead ) const = 0;
    virtual void addBoundTerms( Term *termIn, std::vector<Term*> &termsOut ) const = 0;

    virtual size_t hash() const = 0;

private:
    };

class ADORNMENT_FREE : public ADORNMENT_ITEM
    {
public:
    virtual ADORNMENT_ITEM* clone() const { return new ADORNMENT_FREE(); }

    virtual ostream &print( ostream &out ) const { return out << MAGIC_FREE; }
    virtual bool isFullyBound() const { return false; }
    virtual bool isFullyFree() const { return true; }

    virtual void addBoundVariables( Predicate *pred, Term *term, set_term &boundVariables, binding_map &boundVarsToPred, bool comesFromHead ) const;
    virtual void addBoundTerms( Term *termIn, std::vector<Term*> &termsOut ) const;

    virtual size_t hash() const { return 37; }
    };

class ADORNMENT_BOUND : public ADORNMENT_ITEM
    {
public:
    virtual ADORNMENT_ITEM* clone() const { return new ADORNMENT_BOUND(); }

    virtual ostream &print( ostream &out ) const { return out << MAGIC_BOUND; }
    virtual bool isFullyBound() const { return true; }
    virtual bool isFullyFree() const { return false; }

    virtual void addBoundVariables( Predicate *pred, Term *term, set_term &boundVariables, binding_map &boundVarsToPred, bool comesFromHead ) const;
    virtual void addBoundTerms( Term *termIn, std::vector<Term*> &termsOut ) const;

    virtual size_t hash() const { return 19; }
    };

class ADORNMENT
    {
    friend ostream &operator<<( ostream &out, const ADORNMENT &adornment );
public:
    explicit ADORNMENT( const Atom *atom );
    ADORNMENT( const Atom *atom, const set_term &boundVariables, const binding_map &boundVarsToPred, StatementAtomMapping& statementAtomMapping );
    ADORNMENT( const ADORNMENT &init );
    ~ADORNMENT();

    ADORNMENT &operator=( const ADORNMENT &right );
    bool operator==( const ADORNMENT & a ) const;
    bool operator!=( const ADORNMENT & a ) const { return !(*this==a); }

    ADORNMENT_ITEM &operator[]( unsigned i );
    const ADORNMENT_ITEM &operator[]( unsigned i ) const;

    unsigned getSize() const { return size; }

    string toString() const;
    bool isFullyBound() const;
    bool isFullyFree() const;
    bool isSubsumedBy( const ADORNMENT& a ) const;
    bool isStrictlySubsumedBy( const ADORNMENT& a ) const;

    void addBoundVariables( Predicate* pred, const std::vector<Term*> &terms, set_term &boundVariables, binding_map &boundVarsToPred, bool comesFromHead ) const;
    void addBoundTerms( const std::vector<Term*> &termsIn, std::vector<Term*> &out ) const;

    class hasher
        {
    public:
        size_t operator()( const ADORNMENT &adornment ) const;
        };

private:
    ADORNMENT_ITEM** items;
    unsigned size;

    void init( const Atom *atom, const set_term &boundVariables, const binding_map &boundVarsToPred, StatementAtomMapping& statementAtomMapping );
    void freeItems();
    void cloneItems( ADORNMENT_ITEM **const &init, unsigned initSize );
    };

/*
class ADORNMENT_COMPLEX : public ADORNMENT_ITEM
    {
public:
    ADORNMENT_COMPLEX( string functor, ADORNMENT adornment );
    ADORNMENT_COMPLEX( const ADORNMENT_COMPLEX &init );

    virtual ADORNMENT_ITEM* clone() const { return new ADORNMENT_COMPLEX( *this ); }

    virtual ostream &print( ostream &out ) const { return functor << "(" << adornment << ")"; }
    virtual bool isFullyBound() const { return adornment.isFullyBound(); }
    virtual bool isFullyFree() const { return adornment.isFullyFree(); }

    virtual void addBoundVariables( const TERM &term, set< TERM > &boundVariables ) const;
    virtual void addBoundTerms( const TERM &termIn, TERMS &termsOut ) const;

    virtual size_t hash() const;
private:
    string functor;
    ADORNMENT adornment;
    };
*/

class ADORNED_PREDICATE // : public Predicate
    {
    friend ostream &operator<<( ostream &out, const ADORNED_PREDICATE &predicate);
public:
    ADORNED_PREDICATE( Predicate *predicate, const ADORNMENT &adornment );

    Predicate *getPredicate() const { return predicate; }
//    const Predicate &getPredicate() const { return *this; }

    const ADORNMENT &getAdornment() { return adornment; }
    const ADORNMENT &getAdornment() const { return adornment; }

    string toString() const;

    size_t hash() const;
    bool operator==( const ADORNED_PREDICATE &ap ) const;
    bool operator!=( const ADORNED_PREDICATE &ap ) const { return !(*this==ap); }

    class hasher
        {
    public:
        size_t operator()( const ADORNED_PREDICATE &predicate ) const;
        size_t operator()( ADORNED_PREDICATE *predicate ) const;
        bool operator()( const ADORNED_PREDICATE &ap1, const ADORNED_PREDICATE &ap2 ) const;
        bool operator()( ADORNED_PREDICATE *ap1, ADORNED_PREDICATE *ap2 ) const;
        };

private:
    ADORNMENT adornment;
    Predicate* predicate;
    };

class ABSTRACT_ADORNED_ATOM
	{
public:
	ABSTRACT_ADORNED_ATOM(): atom(nullptr) { }
	explicit ABSTRACT_ADORNED_ATOM( Atom* atom );
    virtual ~ABSTRACT_ADORNED_ATOM() { assert( atom != nullptr ); delete atom; }
    Atom* getAtom() const { assert( atom != nullptr ); return atom; }
    virtual void addBoundVariables( set_term &boundVariables, binding_map &boundVarsToPred, bool comesFromHead = false ) const = 0;
    virtual const ADORNMENT &getAdornment(unsigned index = 0) const = 0;
    virtual ADORNED_PREDICATE getAdornedPredicate(unsigned index = 0) const = 0;
    virtual Atom* createMagicAtom(unsigned index = 0) const = 0;

    virtual ABSTRACT_ADORNED_ATOM &operator=( const ABSTRACT_ADORNED_ATOM& right ) = 0;
    virtual bool operator==( const ABSTRACT_ADORNED_ATOM & a ) const = 0;
    virtual bool operator!=( const ABSTRACT_ADORNED_ATOM & a ) const { return !(*this == a); }

    virtual string toString() const = 0;
    virtual bool hasBoundLiterals() const = 0;
    virtual bool isFullyFree() const = 0;
protected:
	Atom* atom;
	};

class ADORNED_ATOM : public ABSTRACT_ADORNED_ATOM
    {
public:
    friend ostream &operator<<( ostream &out, const ADORNED_ATOM &atom );

    explicit ADORNED_ATOM( Atom* atom ): ABSTRACT_ADORNED_ATOM(atom), adornment(atom){}
    ADORNED_ATOM( const ADORNED_PREDICATE &predicate, const std::vector<Term*> &params, bool hasMinus, bool negative );
    ADORNED_ATOM( Atom* atom, const set_term &boundVariables, const binding_map &boundVarsToPred, StatementAtomMapping &statementAtomMapping );
    ADORNED_ATOM( const ADORNED_ATOM & aa );
    ADORNED_ATOM( Atom *tmp, const ADORNMENT &adornment );
    virtual ~ADORNED_ATOM() {}

    virtual ABSTRACT_ADORNED_ATOM &operator=( const ABSTRACT_ADORNED_ATOM& right );
    virtual bool operator==( const ABSTRACT_ADORNED_ATOM & a ) const;

    virtual const ADORNMENT &getAdornment(unsigned index = 0) const { return adornment; }
    virtual ADORNED_PREDICATE getAdornedPredicate(unsigned index = 0) const;

    virtual void addBoundVariables( set_term &boundVariables, binding_map &boundVarsToPred, bool comesFromHead = false ) const;

    virtual Atom* createMagicAtom(unsigned index = 0) const;

    virtual string toString() const;
    virtual bool hasBoundLiterals() const;
    virtual bool isFullyFree() const { return adornment.isFullyFree(); }
private:
    ADORNMENT adornment;

    };

class AGGREGATE_ADORNED_ATOM : public ABSTRACT_ADORNED_ATOM
	{
public:
	friend ostream &operator<<( ostream &out, const AGGREGATE_ADORNED_ATOM &atom );

    explicit AGGREGATE_ADORNED_ATOM( Atom* atom );
//    AGGREGATE_ADORNED_ATOM( const ADORNED_PREDICATE &predicate, const std::vector<Term*> &params, bool hasMinus, bool negative );
    AGGREGATE_ADORNED_ATOM( Atom* atom, const set_term &boundVariables, const binding_map &boundVarsToPred, StatementAtomMapping &statementAtomMapping );
//    AGGREGATE_ADORNED_ATOM( const ADORNED_ATOM & aa );
    virtual ~AGGREGATE_ADORNED_ATOM();

    virtual ABSTRACT_ADORNED_ATOM &operator=( const ABSTRACT_ADORNED_ATOM& right );
    virtual bool operator==( const ABSTRACT_ADORNED_ATOM & a ) const;

    virtual const ADORNMENT &getAdornment(unsigned index = 0) const;
    std::vector<ADORNMENT> &getAdornments() { return adornments; }

    virtual ADORNED_PREDICATE getAdornedPredicate(unsigned index = 0 ) const;
    virtual void addBoundVariables( set_term &boundVariables, binding_map &boundVarsToPred, bool comesFromHead = false ) const;
    virtual Atom* createMagicAtom(unsigned index = 0) const;

    virtual string toString() const;
    virtual bool hasBoundLiterals() const;
    virtual bool isFullyFree() const;
private:
    std::vector<ADORNMENT> adornments;

	};

class ADORNED_RULE
    {
    friend ostream &operator<<( ostream &out, const ADORNED_RULE &rule );
public:
    typedef std::vector<ABSTRACT_ADORNED_ATOM*> HEAD;
    typedef std::vector<ABSTRACT_ADORNED_ATOM*> BODY;

    virtual ~ADORNED_RULE();

    const HEAD &getHead() const { return head; }
    HEAD &getHead() { return head; }

    const BODY &getBody() const { return body; }
    BODY &getBody() { return body; }

    bool isDisjunctive() const { return head.size() > 1; }

    // FIXME!
    // necessary only for non-ground queries (at the moment)
    bool hasBoundLiterals() const;

private:
    HEAD head;
    BODY body;
    };

class DECORATED_RULE
    {
public:
    friend ostream &operator<<( ostream &out, const DECORATED_RULE &dr );

    DECORATED_RULE(): rule(nullptr) {}
    DECORATED_RULE( const DECORATED_RULE &dr );
    virtual ~DECORATED_RULE();

    DECORATED_RULE &operator=( const DECORATED_RULE & dr );
    void pushToHead( Atom *atom, const ADORNMENT *adornment = nullptr );
    void pushToBody( Atom *atom, const ADORNMENT *adornment = nullptr );
    bool hasBodyAnyMagicAtom() const { return ! bodyAtomPositionToAdorment.empty(); }
    bool hasHeadAnyMagicAtom() const { return ! headAtomPositionToAdorment.empty(); }
    bool hasAnyMagicAtom() const { return ! headAtomPositionToAdorment.empty() || ! bodyAtomPositionToAdorment.empty(); }
    bool isHeadAtomAdorned( unsigned pos ) const { return headAtomPositionToAdorment.find(pos) != headAtomPositionToAdorment.end(); }
    bool isBodyAtomAdorned( unsigned pos ) const { return bodyAtomPositionToAdorment.find(pos) != bodyAtomPositionToAdorment.end(); }
    ADORNMENT *getHeadAtomAdornment( unsigned pos );
    ADORNMENT *getBodyAtomAdornment( unsigned pos );
    const std::vector<Atom*> &getHead() const { assert( rule != nullptr ); return rule->getHead(); }
    const std::vector<Atom*> &getBody() const { assert( rule != nullptr ); return rule->getBody(); }
    size_t getHeadSize() const { assert( rule != nullptr ); return rule->getHead().size(); }
    size_t getBodySize() const { assert( rule != nullptr ); return rule->getBody().size(); }
    void setRule( Rule *r ) { assert( r != nullptr ); rule = r; }
    Rule *getRule() const { return rule; }

private:
    Rule *rule;
    std::unordered_map<unsigned,ADORNMENT*> headAtomPositionToAdorment;
    std::unordered_map<unsigned,ADORNMENT*> bodyAtomPositionToAdorment;

    };

class MAGIC_SETS
    {
public:
    MAGIC_SETS(
            std::vector<Rule*> &idb,
            const std::vector<Rule*> &constraints,
            const std::vector<Rule*> &wconstraints,
            std::vector<Atom*> *query,
            bool isqueryground,
			const StatementAtomMapping& statementAtomMapping);

    ~MAGIC_SETS();

//    void normalizeMagicPredicateNames();
    void rewrite();
    static Predicate* getMagicRep( Predicate* p );
    static Predicate* createMagicRep( Predicate* p );

private:
    typedef std::unordered_map<Predicate*, std::vector<std::size_t> > PREDICATES_RULES_INDEX;

    enum TARGET {
        ALL_RULES = 0,
        NORMAL_RULES,
        DISJUNCTIVE_RULES,
        ALL_RULES_DONE,
        NORMAL_RULES_DONE
    };

    std::vector<Rule*> &IDB;
    const std::vector<Rule*> &Constraints;
    const std::vector<Rule*> &WConstraints;
    std::vector<Atom*> *Query;
    bool IsQueryGround;

    std::vector<DECORATED_RULE*> magicRules;
    std::vector<DECORATED_RULE*> modifiedRules;
    std::vector<Rule*> queryRules;

    std::unordered_map< size_t, vector<ADORNMENT*> > predicateMostGeneralAdornments;
    std::unordered_map< Predicate*, Predicate* > magicPredicate2OriginalPredicate;

//    std::set<std::string> magicNames;

    PREDICATES_RULES_INDEX predicatesRulesIndex;

    std::unordered_map<ADORNED_PREDICATE, TARGET, ADORNED_PREDICATE::hasher> adornedPredicatesTargets;
    std::vector<ADORNED_PREDICATE> adornedPredicatesToBeProcessed;

    set_term boundVariables;
    binding_map boundVarsToPred;

    StatementAtomMapping statementAtomMapping;

    bool processQuery();
    int adornRule( Rule *r, ABSTRACT_ADORNED_ATOM *aA, bool originatedFromDisjunctiveRule, ADORNED_RULE &outRule );
    int adornBody( const std::vector<Atom*> &conj, bool originatedFromDisjunctiveRule, ADORNED_RULE &outRule );

    // adorn an atom and add it to the head of the rule to be adorned
    //
    // @param a, the atom to be added
    // @param r, the rule to which head the atom is to be added
    void adornAndAddAtomToHead( Atom *a, bool originatedFromDisjunctiveRule, ADORNED_RULE &r );

    // if a literal is IDB and not propositional adorn it and add it to
    // the body of the rule to be adorned, otherwise simply add it
    //
    // @param L, the literal to be added
    // @param r, the rule to which body the literal is to be added
    void adornAndAddLiteralToBody( Atom *L, bool originatedFromDisjunctiveRule, ADORNED_RULE &r );

    // return true if a literal is permissible w.r.t. the current set of
    // bound variables
    // the condition of permissibility depends on the type
    // of the literal and on the number of bound terms
    // intuitively, a literal is permissible, if the number of tuples it
    // represents is considerably restricted
    // @param L, the literal to be checked
    bool isPermissible( const Atom *L );

    // return true if the variables of a literal are all bound w.r.t.
    // the current set of bound variables
    // @param L, the literal to be checked
    bool fullyBound(const Atom *L );

    // calculate the number of bound terms in atom a w.r.t. the
    // current set of bound variables
    // @param a, the atom whose number of bound terms must be calculated
    unsigned countNumberBoundTerms( const Atom *a );

    // check if an atom has at least one bound term w.r.t. the current set
    // of bound variables or if it has at least one constant term.
    // @param a, the atom to be checked if it has bound terms
    bool hasBoundTerms( const Atom *a ) const;

    // return true if the first literal is preferable compared to the
    // second literal w.r.t. the current set of bound variables
    // the condition of preferability depends on the ability of a literal
    // to propagate the binding
    // @param L1, the literal to be compared
    // @param L2, the literal to be compared
    bool isPreferable(const Atom *L1, const Atom *L2 );

    // update the set of bound variables with all the variables of a
    // literal
    // @param L, the literal whose variables must be added to the set of
    //        bound variables
    void updateBoundVars( const Atom *L );

    void insertPredicate( const ADORNED_PREDICATE &aP, bool fromDisjunctiveRule );

    ////////////////////////////////GENERATE///////////////////////
    //
    // Receive an adorned rule and return the corresponding generated rules.
    // Build a new magic rule for each IDB literal L in the body which is:
    // adorned (the adornment can be either bound or free) or
    // propositional.
    // The head of the magic rule consists of the magic version of L while
    // the body consists of the magic version of the original head and the
    // literals preceding L in the adorned rule, which were useful to
    // propagate some new binding.
    //
    // @param adornedRule, the adorned rule for which the magic rules are
    //              generated
    // @param sourceAtom FIXME
    void generate(
        const ADORNED_RULE &aR,
        ABSTRACT_ADORNED_ATOM *sourceAtom,
        int permissibles );
    void generate(
        const ADORNED_RULE &aR,
        std::vector<Atom*> newMagicRuleBody,
        std::vector<ADORNMENT*> partOfNewMagicRuleBodyAdornment,
		ABSTRACT_ADORNED_ATOM *headAtom,
        ADORNED_RULE::BODY::const_iterator end,
        int permissibles );

    bool isUseful( const ADORNED_ATOM &aL );

    //////////////////////////MODIFY////////////////////////////////////
    //
    // receive an adorned rule and the corresponding original head and
    // return the modified rule, obtained by adding to the body of each
    // adorned rule the magic version of the head predicate, if it has at
    // least one bound term
    //
    ///////////////////DISJUNCTION////////////////////////////
    //
    // Check if the original head of the rule from which the adorn step
    // generated the current adorned rule: if it is disjunctive, first rebuild
    // the disjunctive head of the adorned rule and then insert for each
    // head literal a magic literal in the body.
    //
    // given an adorned normal rule
    // r: p#...#(t):-q1#...#(t),...,not qm#...#(t),not p1#b...b#(t),...,not pn#b...b#(t)
    // and a head h=p(t),p1(t)V...Vpn(t) (with n>=1)
    // by shifting back the head, rebuild the disjunctive rule:
    // p#...#(t) V p1#b...b#(t) V ... V pn#b...b#(t) :- q1#...#(t), ..., qm#...#(t)
    // and finally modify it by adding the magic version of head literals
    // to the body:
    // p#...#(t) V p1#b...b#(t) V ... V pn#b...b#(t) :-
    // m_p#...#(t),m_p1#b...b#(t),...,m_pn#b...b#(t),q1#...#(t),...,qm#...#(t)
    //
    // The modified rule is added to set modifiedRules.
    void modify( const Rule &rule, const ADORNED_RULE &adornedRule );

    ////////////////////////////////////////////////////////////////////////
    // receive an adorned atom aa and return a magic atom with the same
    // predicate name beginning with underscore and with only the bound
    // variables of aa as parameters
    //
    // @param aa, the adorned atom to transform
    Atom *magic( ABSTRACT_ADORNED_ATOM *aa );

    void printBoundVariables( ostream &out ) const;

    string targetToString( TARGET target ) const;

    ////////////////////////////////////////////////////////////////////////
    // MANAGE CASES WHERE THE BINDING IS LOST
    // For every predicate p in IDB that is fully free-adorned:
    // - erase rules where a not fully-free adorned atom of p occurs in the body;
    // - replace every occurrence of not fully-free adorned version of p with a fully-free adorned version of p.
    void manageLostBindings();
    void eraseRulesWithSubsumedMagicAtomsInTheBody( std::vector<DECORATED_RULE*> &rules );
    void updateHeadOfRulesWithSubsumedMagicAtoms( std::vector<DECORATED_RULE*> &rules );
    Atom *createStrictlySubsumedAtom( Atom *atom, ADORNMENT *atomAdornment, ADORNMENT *mga ) const;
//    std::string getOriginalPred( Predicate *p ) const;

    ////////////////////////////////////////////////////////////////////////
    // CYCLE PREVENTION
    // For each predicate p:
    //     - add a unique representative m#p of any possible magified version of p
    //     - add a new dependency p -> m#p
    void generateMagicPredsAndAddDeps();
    };


}}


#endif /* MAGICSETS_H_ */
