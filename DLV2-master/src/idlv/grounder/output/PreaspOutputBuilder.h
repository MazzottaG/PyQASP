/*
 * PreaspOutputBuilder.h
 *
 *  Created on: Feb 6, 2018
 *      Author: jessica
 */

#ifndef IDLV_GROUNDER_OUTPUT_PREASPOUTPUTBUILDER_H_
#define IDLV_GROUNDER_OUTPUT_PREASPOUTPUTBUILDER_H_

#include "../../../wasp/preasp/Builder.h"
#include "../../../wasp/preasp/Program.h"
#include "OutputBuilder.h"
#include "../table/IdGenerator.h"
#include <unordered_map>

using namespace std;

namespace DLV2 {
namespace grounder {

class PreaspOutputBuilder : public OutputBuilder {
public:
	PreaspOutputBuilder() { preaspBuilder = NULL; }
	virtual ~PreaspOutputBuilder() {}
    
    void setPreaspBuilder(preasp::Builder* b) { if(preaspBuilder==NULL) preaspBuilder = b; }

    virtual void onRule(Rule *rule) override;
    virtual void onWeakConstraint(Rule* rule) override;
    virtual void onQuery() override {
    	//TODO
    }
    virtual void onConstraint(Rule* rule) override;
    virtual void onHead(const vector<Atom*>& head) override;
    virtual void onChoiceAtom(Atom *atom) override;
    virtual void onBody(Rule *rule) override;
    virtual void onClassicalLiteral(Atom *atom) override;
    virtual unsigned onAggregate(Atom *atom) override;
    virtual void onFact(Atom* atom) override;
    virtual void onEnd();
    virtual void appendToStreamAtomTable(Atom* atom, bool fact=false);
    virtual void clearStreamAtomTable(){ preaspBuilder->getProgram().clearVarNames();}

    virtual void onNormalOrDisjunctiveRule(Rule* rule);
    virtual void onChoiceRule(Rule*);
    unsigned countSumAggregate(Atom* atom);
    unsigned maxMinAggregate(Atom *atom);
    unsigned onConstraintRule(Atom* aggregateAtom, unsigned bound);
    unsigned onWeightRule(Atom* aggregateAtom, int bound);
    void manageWeakConstraints();

    void appendToDictionary(const vector<Term*>& terms, unsigned id);

private:
	preasp::Builder* preaspBuilder;
	vector<preasp::Literal> lits;
    /// id_weight_label represent a tuple of : body of the weak rewrited, the weight and the label of the weak
    /// weakLevelConstraints is a list of list, which the second list group the weak with the same level
    list<pair_level_tuple_list> weakLevelConstraints;
    /// Map for each level return the iterator of the list that group the weak with the level in the key
    unordered_map<unsigned,list_pair_it> levelWeak;
};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* IDLV_GROUNDER_OUTPUT_PREASPOUTPUTBUILDER_H_ */
