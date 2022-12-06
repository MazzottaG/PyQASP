/*
 * StatementAtomMappinga.h
 *
 *  Created on: Mar 26, 2019
 *      Author: jessica
 */

#ifndef SRC_GROUNDER_GROUND_STATEMENTATOMMAPPING_H_
#define SRC_GROUNDER_GROUND_STATEMENTATOMMAPPING_H_

#include "../table/PredicateTable.h"
#include "../statement/Rule.h"
#include "../atom/Predicate.h"
#include "vector"
#include "unordered_map"

namespace DLV2 {
namespace grounder {

/**
 *  This class contains the dependency correlation between predicates and statements
 */
class StatementAtomMapping {
public:
	StatementAtomMapping():completedComputationOfDependencies(false){};

	///This method adds a rule, determining which predicates appear in it
	void addRule(Rule *r);

	///This method removes a rule, determining which predicates appear in it
	void removeRule(Rule *r);

	/// Getter for the rule in which the predicate appear in the head
	/// @param p The index of predicate
	/// @param rules The vector that will be filled in with the rules in which p appear in the head
	void getRuleInHead(index_object p,vector<Rule*> & rules) const;
	void getRuleInHead(index_object p,unordered_set<unsigned>& rules) const;

	/// Getter for the rule in which the predicate appear in the body
	/// @param p The index of predicate
	/// @param rules The vector that will be filled in with the rules in which p appear in the body
	void getRuleInBody(index_object p,unordered_set<unsigned>& rules) const;

	/// Return true if the predicate p appear in the head of at least one rule
	/// @param p The index of predicate
	bool isInHead(index_object p) const;

	/// Return true if the predicate p appear in the body of at least one rule
	/// @param p The index of predicate
	bool isInBody(index_object p) const;

	///Clear all data structures of this class
	void clear(){headMap.clear();bodyMap.clear();resetDependencies();}

	///Debug print
	void print() const;

	///Reset all computed dependencies among predicates
	void resetDependencies();

	///Given a predicate a and a predicate b, set that a depends from b and set all indirect dependencies:
	///	- a depends from predicate b' if b depends from b'
	///	- each predicate a' depends from b if a' depends from a
	///	- each predicate a' depends from each predicate b' if a' depends from a and b depends from b'
	void addPredicateDependencies(Predicate* a, Predicate* b);

	///Given a predicate a and a predicate b, return true if a depends (directly or indirectly) from b
	bool checkIfDepended(Predicate* a, Predicate* b);

	///If the current size of the matrix is less than N*F
	///where N is the number of predicates, and F is a number
	///then the matrix is resized to have N*F rows and N*F columns.
	///New empty cells are set to false.
	void resizeToPredicateSize(unsigned factor=1);

	virtual ~StatementAtomMapping(){}

	void computeIndirectDependencies(unsigned i, unordered_set<unsigned>& dep);
    
    void printWithPredicateNames() const;

	void completeComputationOfDependencies();

	void setCompletedComputationOfDependencies(
			bool completedComputationOfDependencies) {
		this->completedComputationOfDependencies =
				completedComputationOfDependencies;
	}

private:
	//The unordered multimap containing the maps between predicates and rules in which they appear in the head
	unordered_multimap<index_object, Rule*> headMap;
	//The unordered multimap containing the maps between predicates and rules in which they appear in the body
	unordered_multimap<index_object, Rule*> bodyMap;
	//The unordered map containing the mappings between each predicate and the set predicates to which it depends
	vector<vector<unsigned>> predicateDependencies;
	bool completedComputationOfDependencies;

};


} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_GROUND_STATEMENTATOMMAPPING_H_ */
