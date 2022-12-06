/* 
 * File:   RuleHashBuilder.h
 * Author: cesco
 *
 * Created on 08/ott/2018, 12:00:08
 */

#ifndef RULEHASHBUILDER_H_
#define RULEHASHBUILDER_H_

#include <cstdint>
using namespace std;

namespace DLV2{ namespace grounder{

    /*
     * The hash is made as follows:
     * - 16 bits for head predicates
     * -  8 bits for head constants+functions
     * - 16 bits for posbody predicates
     * -  8 bits for posbody constants+functions
     * -  8 bits for negbody predicates
     * -  8 bits for negbody constants+functions
     */
class RuleHashBuilder {
    public:
		typedef uint64_t hash_t;

        RuleHashBuilder() :
				hash( 0 ){
		}
        virtual ~RuleHashBuilder() { }

		virtual void newHash(){
			hash = 0;
		}

        virtual void onHeadPredicate( unsigned id ) { hash |= static_cast<hash_t>(id & 0xFFFF) << 48; }
        virtual void onHeadConstant( unsigned id ) { hash |= static_cast<hash_t>(id & 0xFF) << 40; }
        virtual void onHeadFunction( unsigned id ) { onHeadConstant(id); }
        virtual void onBodyPositivePredicate( unsigned id ) { hash |= static_cast<hash_t>(id & 0xFFFF) << 24; }
        virtual void onBodyPositiveConstant( unsigned id ) { hash |= static_cast<hash_t>(id & 0xFF) << 16; }
        virtual void onBodyPositiveFunction( unsigned id ) { onBodyPositiveConstant(id); }
        virtual void onBodyNegativePredicate( unsigned id ) { hash |= static_cast<hash_t>(id & 0xFF) << 8; }
        virtual void onBodyNegativeConstant( unsigned id ) { hash |= static_cast<hash_t>(id & 0xFF); }
        virtual void onBodyNegativeFunction( unsigned id ) { onBodyNegativeConstant(id); }

		virtual hash_t getHash(){
			return hash;
		}
	private:
		hash_t hash;
    };

/*
 * The hash is made as follows:
 * -   1 bit for negativity (1 negative atom, 0 positive atom)
 * -   1 bit for strong negation
 * -   8 bits for guard terms
 * -   4 bits for logical+arithmetic operators (16 possible operator without collision)
 * -   4 bits for aggregation functions (16 possible function without collision)
 * -   8 bits for aggregate elements
 * -   8 bits for aggregation terms
 * -  14 bits for predicates
 * -  16 bits for terms+functions
 */

class AtomHashBuilder {
public:
	typedef uint64_t hash_t;
	AtomHashBuilder(bool isNegative) :
			//TODO CHECK IF 1ULL IS OK!
			isNegative(isNegative), hash(isNegative ? ((1ULL) << 60) : (0)) {
	}
	AtomHashBuilder() :
			isNegative(false), hash(0) {
	}

	void newHash(bool isNegative = false) {
		this->isNegative = isNegative;
		hash = isNegative ? ((1ULL) << 60) : (0);
	}

	void onStrongNegation() {
		hash |= ((1ULL) << 59);
	}
	void onGuardTerms(unsigned id) {
		hash |= static_cast<hash_t>(id & 0xFF) << 54;

	}
	void onLogicalArithmeticOperator(unsigned id) {
		hash |= static_cast<hash_t>(id & 0xF) << 50;

	}
	void onAggregateFunction(unsigned id) {
		hash |= static_cast<hash_t>(id & 0xF) << 46;
	}
	void onAggregateElements(unsigned id) {
		hash |= static_cast<hash_t>(id & 0xFF) << 38;
	}
	void onChoiceElements(unsigned id) {
		onAggregateElements(id);
	}
	void onAggregationTerm(unsigned id) {
		hash |= static_cast<hash_t>(id & 0xFF) << 30;
	}
	void onFirstAtomChoice(unsigned id) {
		onAggregationTerm(id);
	}
	void onPredicate(unsigned id) {
		hash |= static_cast<hash_t>(id & 0x3FFF) << 16;
	}
	void onFunction(unsigned id) {
		hash |= static_cast<hash_t>(id & 0xFFFF);
	}
	void onTerm(unsigned id) {
		hash |= static_cast<hash_t>(id & 0xFFFF);
	}
	hash_t getHash() {
		return hash;
	}
private:
	bool isNegative;
	hash_t hash;
};

}}


#endif /* RULEHASHBUILDER_H_ */
