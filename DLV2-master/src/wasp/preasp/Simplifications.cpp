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

#include "Simplifications.h"
#include "Program.h"
#include "output/GringoOutput.h"
#include "Literal.h"
#include "../../common/Options.h"
#include "../../common/Trace.h"

namespace preasp {

Simplifications::Simplifications(Program& d) : ok(true), program(d) {}

void Simplifications::checkEquivalences(vector<pair<Literal,Literal> >& equivalences) {    
    for(unsigned int i = 0; i < program.aggregates.size(); i++) {
        Aggregate& aggr = *program.aggregates[i];
        aggr.removeUselessIdsAndBounds(equivalences);
        int size = aggr.numberOfIdBounds() - 1;
        for(int j = 0; j < size; j++) {            
            trace_msg(simplifications, 1, "Adding " << to_string(aggr.getId(j+1)) << "->" << to_string(aggr.getId(j)));
            program.binaryConstraints.push_back(new BinaryConstraint(Literal(aggr.getId(j+1), false), Literal(aggr.getId(j), true)));
        }        
    }
    
    trace_msg(simplifications, 1, "Checking equivalences...");    
    vector<Literal> toReplace;
    vector<Literal> replacement;
    unordered_map<int, int> allReplacements;
    for(unsigned int i = 0; i < equivalences.size(); i++) {
        if(allReplacements.find(equivalences[i].second.toInt()) == allReplacements.end())
            allReplacements[equivalences[i].second.toInt()] = equivalences[i].first.toInt();
        toReplace.push_back(equivalences[i].second);
        replacement.push_back(equivalences[i].first);
    }
    
    trace_msg(simplifications, 1, "Number of elements to replace: " << toReplace.size());
    //for(unsigned j = 0; j < toReplace.size(); j++) {
        for(unsigned int i = 0; i < program.binaryConstraints.size(); i++) {            
            program.binaryConstraints[i]->replaceLiterals(allReplacements);        
        }
    
        for(unsigned int i = 0; i < program.rules.size(); i++)
            program.rules[i]->replaceLiterals(allReplacements);
     
        for(unsigned int i = 0; i < program.aggregates.size(); i++)
            program.aggregates[i]->replaceLiterals(allReplacements);
        
        for(unsigned int i = 0; i < program.minimizeConstraints.size(); i++)
            program.minimizeConstraints[i]->replaceLiterals(allReplacements);
    //}
    
    for(unsigned int i = 0; i < toReplace.size(); i++) {
        unsigned int toReplaceVar = toReplace[i].var();
        unsigned int replacementVar = replacement[i].var();
        assert(toReplaceVar > 0 && toReplaceVar <= program.numberOfVars);
        assert(replacementVar > 0 && replacementVar <= program.numberOfVars);
        trace_msg(simplifications, 2, "Replacing " << toReplace[i].toInt() << " with " << replacement[i].toInt());
        atomData.setEliminated(toReplaceVar);
        if(!program.isHidden(toReplaceVar)) {
            if(program.isHidden(replacementVar)) program.setVarName(replacementVar, program.getName(toReplaceVar));
            program.addName(replacementVar, program.getName(toReplaceVar));            
        }
        if(interpretation.isUndefined(toReplaceVar)) {
            interpretation.setFalse(toReplaceVar);
            atomData.setPropagated(toReplaceVar);
        }
        else if(interpretation.isTrueSupported(toReplaceVar)) {            
            if(interpretation.isTrueSupported(replacementVar)) continue;
            if(!interpretation.setTrueSupported(replacementVar)) { ok = false; return; }
            atomData.setPropagated(replacementVar);
        }
        else if(interpretation.isTrue(toReplace[i])) {
            if(interpretation.isTrue(replacement[i])) continue;
            //This work only with aggregates
            bool res = replacement[i].sign() ? interpretation.setFalse(replacementVar) : interpretation.setTrueSupported(replacementVar);
            if(!res) { ok = false; return; }
            atomData.setPropagated(replacementVar);
        }
        else if(interpretation.isFalse(toReplace[i])) {
            if(interpretation.isFalse(replacement[i])) continue;
            //This work only with aggregates
            bool res = !replacement[i].sign() ? interpretation.setFalse(replacementVar) : interpretation.setTrueSupported(replacementVar);
            if(!res) { ok = false; return; }
            atomData.setPropagated(replacementVar);
        }        
    }
    trace_msg(simplifications, 1, "...done!");
}

bool Simplifications::simplify(vector<pair<Literal,Literal>>& equivalences, vector<Literal>& aggregatesIds) {
    if(program.options.verbosity() >= 1) cerr << "starting simplifications..." << endl;
    createDataStructures();
    for(unsigned int i = 0; i < aggregatesIds.size(); i++) if(interpretation.isTrue(aggregatesIds[i])) interpretation.setTrueSupported(aggregatesIds[i].toInt());
    if(program.options.verbosity() >= 1) cerr << "checking equivalences..." << endl;
    checkEquivalences(equivalences);
    if(program.options.verbosity() >= 1) cerr << "starting analysis of binary constraints..." << endl;
    analyzeBinaryConstraints();
    if(program.options.verbosity() >= 1) cerr << "starting analysis of rules..." << endl;
    analyzeRules();
    if(program.options.verbosity() >= 1) cerr << "starting analysis of aggregates..." << endl;
    analyzeAggregates();    
    if(program.options.verbosity() >= 1) cerr << "starting propagation..." << endl;
    propagate();
    if(program.options.verbosity() >= 1) cerr << "...end propagation" << endl;
    if(program.options.printOption() == DLV2::Options::pre || program.options.printOption() == PREASP_WELL_FOUNDED_MODEL || program.options.printOption() == PREASP_WELL_FOUNDED_MODEL_CAUTIOUS) {
        if(!ok) {
            cerr << "Error: program is incoherent." << endl;
            return ok;
        }
        cout << "Start WellFounded Model"<<endl;
        bool first = true;
        cout << "True: {";
        for(unsigned int i = 2; i < interpretation.size(); i++) {
            if(!program.isHidden(i) && interpretation.isTrue(i)) {
                if(!first)
                    cout << ", ";                
                else
                    first = false;
                cout << program.getName(i);                
            }
        }
        cout << "}" << endl;
        first = true;
        if(program.options.printOption() == PREASP_WELL_FOUNDED_MODEL || program.options.printOption()==DLV2::Options::pre) {
            cout << "Undefined: {";
            for(unsigned int i = 2; i < interpretation.size(); i++) {
                if(!program.isHidden(i) && interpretation.isUndefined(i)){
                    if(!first)
                        cout << ", ";                
                    else
                        first = false;
                    cout << program.getName(i);
                }
            }
            cout << "}" << endl;
            first=true;
            cout << "False: {";
            for(unsigned int i = 2; i < interpretation.size(); i++) {
                if(!program.isHidden(i) && interpretation.isFalse(i)){
                    if(!first)
                        cout << ", ";                
                    else
                        first = false;
                    cout << program.getName(i);
                }
            }
            cout << "}" << endl;
            cout << "End WellFounded Model"<<endl;
        }
    }
    if(!(program.options.printOption() == PREASP_WELL_FOUNDED_MODEL || program.options.printOption() == PREASP_WELL_FOUNDED_MODEL_CAUTIOUS))
        finalize();
    if(program.options.verbosity() >= 1) cerr << "...end simplifications" << endl;    
    return ok;
}

void Simplifications::analyzeBinaryConstraints() {
    if(!ok) return;
    trace_msg(simplifications, 1, "Considering binary constraints");
    unsigned int j = 0;
    for(unsigned int i = 0; i < program.binaryConstraints.size() && ok; i++) {
        program.binaryConstraints[j] = program.binaryConstraints[i];
        BinaryConstraint& b = *program.binaryConstraints[i];
        unsigned int res = b.simplify(interpretation);
        if(res == CONSTRAINT_UNDEFINED) {
            Literal first = b.getLiteral(0);
            Literal second = b.getLiteral(1);
            first.sign() ? atomData.addNegativeBinaryConstraint(first.var(), j) : atomData.addPositiveBinaryConstraint(first.var(), j);
            second.sign() ? atomData.addNegativeBinaryConstraint(second.var(), j) : atomData.addPositiveBinaryConstraint(second.var(), j);
            j++; continue;
        }
        if(res == CONSTRAINT_VIOLATED) ok = false;
        else if(res != CONSTRAINT_SATISFIED) { inferFalseLiteral(b.getLiteral(res)); }
        delete program.binaryConstraints[i];
    }
    program.binaryConstraints.resize(j);
}

void Simplifications::analyzeRules() {
    if(!ok) return;
    trace_msg(simplifications, 1, "Considering rules");
    unsigned int j = 0;
    for(unsigned int i = 0; i < program.rules.size() && ok; i++) {
        program.rules[j] = program.rules[i];
        Rule& r = *program.rules[i];
        unsigned int numberOfTrue = r.simplify(interpretation);
        if(r.bodySize()==0 && r.isConstraint()) { ok = false; break; }
        if(r.isRemoved()) { delete program.rules[j]; continue; }
        if(!r.isConstraint()) rulesNotConstraint.push_back(program.rules[i]);
        createDataStructuresForRule(r, j, numberOfTrue);
        j++;        
    }
    program.rules.resize(j);
}

void Simplifications::analyzeAggregates() {
    if(!ok) return;
    trace_msg(simplifications, 1, "Considering aggregates");
    unsigned int j = 0;
    for(unsigned int i = 0; i < program.aggregates.size() && ok; i++) {
        program.aggregates[j] = program.aggregates[i];
        Aggregate& aggr = *program.aggregates[i];
        vector<unsigned int> trueIds;
        aggr.simplify(interpretation, trueIds);
        for(unsigned int i = 0; i < trueIds.size(); i++) { trace_msg(simplifications, 2, "Aggregate with id " << trueIds[i] << " is true"); inferFact(trueIds[i]); }
        if(aggr.isRemoved()) {
            delete program.aggregates[j];
            continue;
        }        
        for(unsigned int i = 0; i < aggr.numberOfIdBounds(); i++) atomData.setAggregateAtom(aggr.getId(i), j);        
        createDataStructuresForAggregate(aggr, j);
        j++;
    }
    program.aggregates.resize(j);
}

void Simplifications::createDataStructures() {
    trace_msg(simplifications, 1, "Creating data structures");
    atomData.init(program.numberOfVars);
    interpretation.init(program.numberOfVars);
    
    for(unsigned int i = 0; i < program.facts.size(); i++ ) {
        trace_msg(simplifications, 1, program.facts[i] << " is a fact");
        if(interpretation.isTrueSupported(program.facts[i])) continue;
        if(!interpretation.setTrueSupported(program.facts[i])) { ok = false; return; } 
        atomData.setPropagated(program.facts[i]);
    }
    
    for(unsigned int i = 0; i < program.trueAtoms.size(); i++ ) {
        trace_msg(simplifications, 1, program.trueAtoms[i] << " is true");
        if(interpretation.isTrue(program.trueAtoms[i])) continue;
        if(!interpretation.setTrue(program.trueAtoms[i])) { ok = false; return; }
        atomData.setPropagated(program.trueAtoms[i]);
    }
    
    for(unsigned int i = 0; i < program.falseAtoms.size(); i++ ) {
        trace_msg(simplifications, 1, program.falseAtoms[i] << " is false");
        if(interpretation.isFalse(program.falseAtoms[i])) continue;
        if(!interpretation.setFalse(program.falseAtoms[i])) { ok = false; return; }
        atomData.setPropagated(program.falseAtoms[i]);
    }
//    for(unsigned int i = 1; i < interpretation.size(); i++)
//        printf("%d (TRUES,FALSE,TRUE,UND)=(%d,%d,%d,%d)\n", i, interpretation.isTrueSupported(i),
//                interpretation.isFalse(i), interpretation.isTrueNotSupported(i), interpretation.isUndefined(i));
    program.facts.clear();
    program.trueAtoms.clear();
    program.falseAtoms.clear();        
}

void Simplifications::propagate() {
    if(!ok) return;
    trace_msg(simplifications, 1, "Propagation...");
    trace_msg(simplifications, 1, "Starting check of support");
    for(unsigned int i = 2; i < interpretation.size(); i++) checkSupport(i);    
    trace_msg(simplifications, 1, "End check of support");
    
    begin:;
    while(!program.facts.empty() || !program.falseAtoms.empty() || !program.trueAtoms.empty()) {        
        while(!program.facts.empty()) { 
            if(!ok) break;
            unsigned int next = program.facts.back(); program.facts.pop_back();
            propagateFact(next);
        }
        while(program.facts.empty() && !program.falseAtoms.empty()) {
            if(!ok) break;
            unsigned int next = program.falseAtoms.back(); program.falseAtoms.pop_back();
            propagateFalse(next);
        }        
        while(program.facts.empty() && program.falseAtoms.empty() && !program.trueAtoms.empty()) { 
            if(!ok) break;
            unsigned int next = program.trueAtoms.back(); program.trueAtoms.pop_back();
            propagateTrue(next);
        }
        if(!ok) break;        
    }
    if(operatorR()) goto begin;
    if(!ok) { program.facts.clear(); program.falseAtoms.clear(); program.trueAtoms.clear(); }
    assert(program.facts.empty() && program.falseAtoms.empty() && program.trueAtoms.empty());
    trace_msg(simplifications, 1, "...end propagation");
}

void Simplifications::createDataStructuresForRule(Rule& rule, unsigned int id, unsigned int numberOfTrueInTheHead) {
    if(rule.isRemoved()) return;
    assert(id == trueBodyCounter.size());    trueBodyCounter.push_back(0);
    assert(id == trueHeadSpecialCounter.size()); trueHeadSpecialCounter.push_back(numberOfTrueInTheHead);
    
    if(rule.isChoiceRule() || numberOfTrueInTheHead == NO_TRUE) {
        for(unsigned int k = 0; k < rule.headSize(); k++) {
            unsigned int var = rule.getHeadAtom(k);
            assert(!interpretation.isFalse(var) && !interpretation.isTrueSupported(var));
            addSupportingRule(var, id);
        }
    }
    else {
        for(unsigned int k = 0; k < rule.headSize(); k++) {
            unsigned int var = rule.getHeadAtom(k);
            assert(!interpretation.isFalse(var) && !interpretation.isTrueSupported(var));
            addSupportingRule(var, id);
            if(var != numberOfTrueInTheHead ) atomData.removeSupportingRule(var);
        }
    }
    
    unsigned int countTrueBody = 0;
    for( unsigned int k = 0; k < rule.bodySize(); k++ ) {
        Literal bodyLiteral = rule.getBodyLiteral(k);
        assert(!interpretation.isFalse(bodyLiteral) && !interpretation.isTrueSupported(bodyLiteral.var()));
        if(interpretation.isTrue(bodyLiteral) && atomData.isPropagated(bodyLiteral.var())) { countTrueBody++; }
        bodyLiteral.sign() ? addNegativeBody(bodyLiteral.var(), id) : addPositiveBody(bodyLiteral.var(), id);            
    }
    trueBodyCounter[trueBodyCounter.size()-1] = countTrueBody;
    fire(rule, id);
}

void Simplifications::createDataStructuresForAggregate(Aggregate& aggr, unsigned int id) {
    if(aggr.isRemoved()) return;
    assert(id == maxPossibleSum.size());   maxPossibleSum.push_back(0);
    
    for(unsigned int i = 0; i < aggr.size(); i++) {
        Literal lit = aggr.getLiteral(i);
        assert(interpretation.isUndefined(lit));
        lit.sign() ? atomData.addNegativeAggregate(lit.var(), id) : atomData.addPositiveAggregate(lit.var(), id);
        maxPossibleSum[id] += aggr.getWeight(i);
    }
    fire(aggr, id);
}

void Simplifications::removeLiteralFromBody(Literal lit, unsigned int id, bool wasPropagated) {
    assert(id < program.rules.size());
    Rule& r = *program.rules[id];
    if(r.isRemoved()) return;
    r.removeLiteralFromBody(lit);
    if(wasPropagated) {
        assert(id < trueBodyCounter.size());
        assert(trueBodyCounter[id]>0);
        trueBodyCounter[id]--;
    }
    fire(r, id);
}

void Simplifications::removeVarFromHead(unsigned int var, unsigned int id) {
    assert(id < program.rules.size());
    Rule& r = *program.rules[id];
    if(r.isRemoved()) return;
    r.removeVarFromHead(var);
    fire(r, id);
}

void Simplifications::removedSupportingRule(unsigned int var) {
    atomData.removeSupportingRule(var);
    checkSupport(var);    
}

void Simplifications::bodyLiteralIsTrue(unsigned int id) {
    assert(id < program.rules.size());
    Rule& r = *program.rules[id];
    if(r.isRemoved()) return;
    assert(id < trueBodyCounter.size());
    trueBodyCounter[id]++;
    fire(r, id);
}

void Simplifications::headAtomIsTrue(unsigned int var, unsigned int id) {
    trace_msg(simplifications, 2, (var == 0 ? "Removed rule: Updating supporting rule of head atoms " : "Head atom ") << var << " is true");
    assert(id < program.rules.size());
    Rule& r = *program.rules[id];
    assert(!r.isConstraint());    
    if(r.isChoiceRule()) return;
    assert(id < trueHeadSpecialCounter.size());
    unsigned int specialCounter = trueHeadSpecialCounter[id];
    trace_msg(simplifications, 2, "Special counter: " << ((specialCounter==MORE_TRUE) ? "more than one true" : (specialCounter==NO_TRUE) ? "no true" : to_string(specialCounter)));
    if(specialCounter == MORE_TRUE) return;
    if(specialCounter == NO_TRUE) {
        trueHeadSpecialCounter[id] = var;
        for(unsigned int i = 0; i < r.headSize() && ok; i++) {
            unsigned int headAtom = r.getHeadAtom(i);
            if(headAtom == var) continue;
            removedSupportingRule(headAtom);            
        }
    }
    else {
        if(specialCounter == var) return;
        trueHeadSpecialCounter[id]=MORE_TRUE;
        removedSupportingRule(specialCounter);
    }
}

void Simplifications::removeRule(unsigned int id) {
    assert(id < program.rules.size());
    Rule& r = *program.rules[id];
    if(r.isRemoved()) return;
    r.remove();
    if(!r.isConstraint()) headAtomIsTrue(0, id);    
}

void Simplifications::headAtomIsFact(unsigned int var, unsigned int id, bool wasPropagated) {
    assert(id < program.rules.size());
    Rule& r = *program.rules[id];
    if(r.isRemoved()) return;
    if(r.isChoiceRule()) { r.removeVarFromHead(var); return; }    
    r.remove();
    if(!wasPropagated) headAtomIsTrue(var, id); 
}

void Simplifications::propagateFact(unsigned int var) {
    trace_msg(simplifications, 1, "Propagating fact " << var);
    bool wasPropagated = atomData.isPropagated(var);
    atomData.setPropagated(var);
    
    unsigned int sSize = atomData.supportingRuleSize(var);
    for(unsigned int i = 0; i < sSize && ok; i++)
        headAtomIsFact(var, atomData.getSupportingRule(var,i), wasPropagated);
    if(!ok) return;
    if(!atomData.isAggregateAtom(var)) atomData.clearSupportingRules(var);    
    
    unsigned int bSize = atomData.binaryConstraintsSize(var);
    for(unsigned int i = 0; i < bSize && ok; i++) {
        Type t = atomData.getBinaryConstraint(var, i);
        if(t.type() == TYPE_NEGATIVEBODY) continue;
        assert(t.type() == TYPE_POSITIVEBODY);
        assert(t.id() < program.binaryConstraints.size());
        doInferenceOnBinaryConstraint(*program.binaryConstraints[t.id()]);
    }
    atomData.clearBinaryConstraints(var);
    if(!ok) return;    
    
    unsigned int size = atomData.rulesSize(var);
    for(unsigned int i = 0; i < size && ok; i++) {
        Type t = atomData.getRule(var, i);
        switch(t.type()) {
            case TYPE_NEGATIVEBODY:
                if(!wasPropagated)
                    removeRule(t.id());
                break;

            case TYPE_POSITIVEBODY:
                removeLiteralFromBody(Literal(var,false), t.id(), wasPropagated);
                break;
                
            case TYPE_NEGATIVEAGGREGATE:
                if(!wasPropagated)
                    litIsFalseInAggregate(Literal(var,true), *program.aggregates[t.id()], t.id());
                break;
                
            case TYPE_POSITIVEAGGREGATE:
                if(!wasPropagated)
                    litIsTrueInAggregate(Literal(var,false), *program.aggregates[t.id()], t.id()); 
                break;
                
            default:
                assert(0);
                break;
        }
    }
    atomData.clearRules(var);
    
    if(atomData.isAggregateAtom(var) && !wasPropagated) checkAggregateAtom(var);
}

void Simplifications::propagateFalse(unsigned int var) {
    trace_msg(simplifications, 1, "Propagating false " << var);
    assert(!atomData.isPropagated(var));
    atomData.setPropagated(var);
    
    unsigned int sSize = atomData.supportingRuleSize(var);
    for(unsigned int i = 0; i < sSize && ok; i++)
        removeVarFromHead(var, atomData.getSupportingRule(var, i));
    if(!atomData.isAggregateAtom(var)) atomData.clearSupportingRules(var);
    if(!ok) return;    
    
    unsigned int bSize = atomData.binaryConstraintsSize(var);
    for(unsigned int i = 0; i < bSize && ok; i++) {
        Type t = atomData.getBinaryConstraint(var, i);
        if(t.type() == TYPE_POSITIVEBODY) continue;
        assert(t.type() == TYPE_NEGATIVEBODY);
        assert(t.id() < program.binaryConstraints.size());
        doInferenceOnBinaryConstraint(*program.binaryConstraints[t.id()]);
    }
    atomData.clearBinaryConstraints(var);
    if(!ok) return;
    
    unsigned int size = atomData.rulesSize(var);       
    for(unsigned int i = 0; i < size && ok; i++) {
        Type t = atomData.getRule(var, i);
        switch(t.type()) {
            case TYPE_NEGATIVEBODY:
                removeLiteralFromBody(Literal(var,true), t.id(), false);
                break;

            case TYPE_POSITIVEBODY:
                removeRule(t.id());
                break;
                
            case TYPE_NEGATIVEAGGREGATE:
                litIsTrueInAggregate(Literal(var,true), *program.aggregates[t.id()], t.id());
                break;
                
            case TYPE_POSITIVEAGGREGATE:
                litIsFalseInAggregate(Literal(var,false), *program.aggregates[t.id()], t.id());                 
                break;
                
            default:
                assert(0);
                break;
        }
    }
    atomData.clearRules(var);
    
    if(atomData.isAggregateAtom(var))
        checkAggregateAtom(var);
}

void Simplifications::propagateTrue(unsigned int var) {
    trace_msg(simplifications, 1, "Propagating true " << var);
    if(atomData.isPropagated(var)) { assert(interpretation.isTrueSupported(var)); return; }
    atomData.setPropagated(var);
    
    unsigned int sSize = atomData.supportingRuleSize(var);
    for(unsigned int i = 0; i < sSize && ok; i++) {
        unsigned int ruleId = atomData.getSupportingRule(var,i);
        assert(ruleId < program.rules.size());
        if(!program.rules[ruleId]->isRemoved())
            headAtomIsTrue(var, ruleId);
    }
    if(!ok) return;
    
    unsigned int bSize = atomData.binaryConstraintsSize(var);
    for(unsigned int i = 0; i < bSize && ok; i++) {
        Type t = atomData.getBinaryConstraint(var, i);
        if(t.type() == TYPE_NEGATIVEBODY) continue;
        assert(t.type() == TYPE_POSITIVEBODY);
        assert(t.id() < program.binaryConstraints.size());
        doInferenceOnBinaryConstraint(*program.binaryConstraints[t.id()]);
    }
    atomData.clearBinaryConstraints(var);
    
    if(!ok) return;
    
    unsigned int size = atomData.rulesSize(var);
    for(unsigned int i = 0; i < size && ok; i++) {
        Type t = atomData.getRule(var, i);
        switch(t.type()) {
            case TYPE_NEGATIVEBODY:
                removeRule(t.id());
                break;

            case TYPE_POSITIVEBODY:
                bodyLiteralIsTrue(t.id());
                break;
                
            case TYPE_NEGATIVEAGGREGATE:
                litIsFalseInAggregate(Literal(var,true), *program.aggregates[t.id()], t.id());
                break;
                
            case TYPE_POSITIVEAGGREGATE:
                litIsTrueInAggregate(Literal(var,false), *program.aggregates[t.id()], t.id()); 
                break;
                
            default:
                assert(0);
                break;
        }
    }
    
    if(atomData.isAggregateAtom(var))
        checkAggregateAtom(var);
    else
        checkSupport(var);
}

void Simplifications::litIsTrueInAggregate(Literal lit, Aggregate& aggr, unsigned int id) {
    if(aggr.isRemoved()) return;
    trace_msg(simplifications, 2, "Trying to remove true lit " << lit.toInt() << " from aggregate with id " << id);
    vector<unsigned int> trueIds;
    unsigned int weight = aggr.removeTrueLiteral(lit, trueIds);
    for(unsigned int i = 0; i < trueIds.size(); i++) inferFact(trueIds[i]);
    if(aggr.isRemoved()) return;
    assert(id < maxPossibleSum.size());
    assert(maxPossibleSum[id] >= weight);
    maxPossibleSum[id] -= weight;
    fire(aggr, id);    
}

void Simplifications::litIsFalseInAggregate(Literal lit, Aggregate& aggr, unsigned int id) {
    if(aggr.isRemoved()) return;
     trace_msg(simplifications, 2, "Trying to remove false lit " << lit.toInt() << " from aggregate with id " << id);
    unsigned int weight = aggr.removeFalseLiteral(lit);
    assert(id < maxPossibleSum.size());
    assert(maxPossibleSum[id] >= weight);
    maxPossibleSum[id] -= weight;
    fire(aggr, id);
}

void Simplifications::fire(Rule& rule, unsigned int id) {
    trace_msg(simplifications, 2, "Firing rule with id " << id);
    if(rule.isChoiceRule()) {
        if(rule.headSize() == 0) rule.remove();
        return;
    }
    assert(id < trueHeadSpecialCounter.size());
    assert(id < trueBodyCounter.size());
    if(rule.headSize() == 1 && rule.bodySize() == trueBodyCounter[id])
        inferHead(rule);
    else if(rule.headSize() == 0 && rule.bodySize() == trueBodyCounter[id] + 1)
        inferBody(rule);       
}

void Simplifications::fire(Aggregate& aggr, unsigned int id) {    
    trace_msg(simplifications, 2, "Firing aggregate with id " << id);
    assert(id < maxPossibleSum.size());
    unsigned int maxSum = maxPossibleSum[id];
    trace_msg(simplifications, 3, "Max possible sum " << maxSum);
    for(unsigned int j = 0; j < aggr.numberOfIdBounds(); ) {
        unsigned int aggrId = aggr.getId(j);
        unsigned int bound = aggr.getBound(j);
        trace_msg(simplifications, 4, "Considering bound " << bound);
        if(maxSum < bound) { inferFalseLiteral(Literal(aggrId, false)); aggr.removeFalseId(j); }
        else j++;
        if(interpretation.isTrue(aggrId)) {
            for(unsigned int i = 0; i < aggr.size(); i++) {
                assert(maxSum >= aggr.getWeight(i));
                //If this literal is false then the aggregate is also false
                if(maxSum - aggr.getWeight(i) < bound) inferTrueLiteral(aggr.getLiteral(i));
            }
        }
        else if(interpretation.isFalse(aggrId)) {
            for(unsigned int i = 0; i < aggr.size(); i++) {
                //If this literal is true then the aggregate is also true
                if(aggr.getWeight(i) >= bound) inferFalseLiteral(aggr.getLiteral(i));
            }
        }        
    }
}

void Simplifications::inferBody(const Rule& rule) {    
    Literal lit = rule.getUndefinedBodyLiteral(interpretation, atomData);
    assert(lit.var() != 0);
    inferFalseLiteral(lit);
}

void Simplifications::inferHead(const Rule& rule) {
    assert(rule.headSize()==1);
    unsigned int var = rule.getHeadAtom(0);
    rule.bodySize()==0 ? inferFact(var) : inferTrueLiteral(Literal(var, false));
}

bool Simplifications::ruleCanSupportVar(unsigned int var, unsigned int id) const {
    assert(id < program.rules.size());
    assert(id < trueHeadSpecialCounter.size());
    if(program.rules[id]->isRemoved()) return false;
    if(program.rules[id]->isChoiceRule()) return true;
    unsigned int specialCounter = trueHeadSpecialCounter[id];
    if(specialCounter == MORE_TRUE) return false;
    if(specialCounter == NO_TRUE) return true;
    return specialCounter == var;
}

void Simplifications::checkSupport(unsigned int var) {
    trace_msg(simplifications, 1,"Checking support of " << var);
    if(interpretation.isFalse(var)) return;
    if(interpretation.isTrueSupported(var)) return;
    if(atomData.isAggregateAtom(var)) return;
    if(atomData.isEliminated(var)) return;

    if(atomData.numberOfSupportingRule(var) == 0) { inferFalseLiteral(Literal(var, false)); return; }
    if(atomData.numberOfSupportingRule(var) == 1 && interpretation.isTrueNotSupported(var)) {
        for(unsigned int i = 0; i < atomData.supportingRuleSize(var); i++) {
            unsigned int id = atomData.getSupportingRule(var,i);
            assert(id < program.rules.size());
            if(!ruleCanSupportVar(var, id)) continue;
            if(program.rules[id]->isChoiceRule()) { inferAllBodyTrue(id); }
            else { inferAllHeadFalse(var, id); inferAllBodyTrue(id); }
            if(!ok) return;            
            atomData.clearSupportingRules(var);            
            addSupportingRule(var, id);
            return;
        }
        assert(0);
    }
}

void Simplifications::checkAggregateAtom(unsigned int var){
    assert(atomData.isAggregateAtom(var));
    unsigned int id = atomData.getAggregateId(var);
    assert(id < program.aggregates.size());
    Aggregate& aggr = *program.aggregates[id];    
    if(aggr.isRemoved()) return;
    fire(aggr, id);
    if(interpretation.isTrueNotSupported(var)) inferFact(var);
}

void Simplifications::inferAllBodyTrue(unsigned int id) {
    assert(id < program.rules.size());
    Rule& r = *program.rules[id];
    for(unsigned int i = 0; i < r.bodySize(); i++) {
        inferTrueLiteral(r.getBodyLiteral(i));
        if(!ok) return;
    }
}

void Simplifications::inferAllHeadFalse(unsigned int var, unsigned int id) {
    assert(id < program.rules.size());
    Rule& r = *program.rules[id];
    for(unsigned int i = 0; i < r.headSize(); i++) {
        unsigned int head = r.getHeadAtom(i);
        if(head == var) continue;
        inferFalseLiteral(Literal(head, false));
        if(!ok) return;
    }
}

void Simplifications::inferTrueLiteral(Literal lit) {
    if(interpretation.isTrue(lit)) return;
    trace_msg(simplifications, 2, "Inferring true literal " << lit.toInt());
    bool res = lit.sign() ? interpretation.setFalse(lit.var()) : interpretation.setTrue(lit.var());
    if(!res) { ok = false; return; }
    assert(!atomData.isPropagated(lit.var()));
    lit.sign() ? program.falseAtoms.push_back(lit.var()) : program.trueAtoms.push_back(lit.var());
}

void Simplifications::inferFalseLiteral(Literal lit) { inferTrueLiteral(~lit); }

void Simplifications::inferFact(unsigned int var) {
    trace_msg(simplifications, 2, "Inferring fact " << var);
    if(interpretation.isTrueSupported(var)) return;
    if(!interpretation.setTrueSupported(var)) { ok = false; return; }
    program.facts.push_back(var);
}

bool Simplifications::checkFounded(const Rule& r, vector<unsigned int>& foundedVars) {
    bool found = false;
    for(unsigned int k = 0; k < r.bodySize(); k++) {
        Literal lit = r.getBodyLiteral(k);
        if(!lit.sign() && !atomData.isFounded(lit.var())) { found = true; break; }
    }
    if(!found) {
        for(unsigned int k = 0; k < r.headSize(); k++) {
            unsigned int headAtom = r.getHeadAtom(k);
            if(atomData.isFounded(headAtom)) continue;
            atomData.setFounded(headAtom);
            foundedVars.push_back(headAtom);
        }
    }
    return found;
}

bool Simplifications::operatorR() {
    if(!ok) return false;
    trace_msg(simplifications, 1, "Starting operator R" );
    static vector<unsigned int> foundedVars;
    unsigned int j = 0;
    for(unsigned int i = 0; i < rulesNotConstraint.size(); i++) {
        rulesNotConstraint[j] = rulesNotConstraint[i];
        Rule& r = *rulesNotConstraint[j];
        if(r.isRemoved() || r.isConstraint()) continue;
        if(checkFounded(r, foundedVars)) j++;
    }
    rulesNotConstraint.resize(j);
    for(unsigned int i = 0; i < foundedVars.size(); i++) {
        unsigned int var = foundedVars[i];
        unsigned int size = atomData.rulesSize(var);
        for(unsigned int k = 0; k < size; k++) {
            Type rule = atomData.getRule(var, k);
            if(rule.type() != TYPE_POSITIVEBODY) continue;
            assert(rule.id() < program.rules.size());
            checkFounded(*program.rules[rule.id()], foundedVars);
        }
    }
    foundedVars.clear();
    bool retVal = false;
    for(unsigned int i = 2; i < interpretation.size(); i++) {
        if(interpretation.isTrueSupported(i) || interpretation.isFalse(i) || atomData.isAggregateAtom(i) || atomData.isFounded(i) || atomData.isEliminated(i)) continue;
        inferFalseLiteral(Literal(i, false));
        retVal = true;
        if(!ok) return false;
    }
    return retVal;
}

void Simplifications::finalize() {
    if(!ok) return;
    assert(program.facts.empty() && program.falseAtoms.empty() && program.trueAtoms.empty());
    
    for(unsigned int i=0; i < program.rules.size(); i++) {
        assert(program.rules[i] != NULL);
        Rule& r = *program.rules[i];
        if(r.isRemoved() || r.headSize()<=1) continue;
        bool first = false;
        for(unsigned int j=0; j < r.headSize(); j++) {
            if(interpretation.isTrue(r.getHeadAtom(j))) {
                if(first) { r.disableSupportForHead(); break; }
                first = true;
            }
        }
    }
    
    atomData.createRemapping(program.numberOfVars);
    program.falseAtoms.push_back(1);    
    unsigned int nextVar = 2;    
    for(unsigned int i = 2; i < interpretation.size(); i++) {
        if(atomData.isAggregateAtom(i)) {
            trace_msg(simplifications, 1, "Considering aggregate atom " << i);            
            unsigned int aggrId = atomData.getAggregateId(i);
            assert(aggrId < program.aggregates.size());
            if(program.aggregates[aggrId]->isRemoved()) { trace_msg(simplifications, 1, "Aggregate is removed: simplifying " << i); atomData.remap(i, 0); atomData.clearRules(i); continue; }
            else {
                if(program.options.printOption() != PREASP_PRINT_EXTENDED_DIMACS) {
                    if(interpretation.isTrue(i))
                        program.trueAtoms.push_back(nextVar);
                    else if(interpretation.isFalse(i))
                        program.falseAtoms.push_back(nextVar);
                }
            }
            if(program.options.printOption() == PREASP_PRINT_EXTENDED_DIMACS && !interpretation.isUndefined(i)) {
                if(interpretation.isTrue(i) && !program.isHidden(i)) {
                    program.trueAtoms.push_back(nextVar);
                    trace_msg(simplifications, 1, "Remapping " << i << " to " << nextVar);
                    atomData.remap(i, nextVar++);
                }
                else {
                    trace_msg(simplifications, 1, "Removing aggregate " << i);
                    atomData.remap(i, 0);
                }
            }
            else {
                trace_msg(simplifications, 1, "Remapping " << i << " to " << nextVar);
                atomData.remap(i, nextVar++);
            }
            atomData.clearRules(i);
            continue;
        }
            
        if(interpretation.isFalse(i)) { trace_msg(simplifications, 1, "Removing false atom " << i); atomData.remap(i, 0); continue; }
        
        if(interpretation.isTrueSupported(i)) { 
            if(program.isHidden(i)) { trace_msg(simplifications, 1, "Removing fact " << i); atomData.remap(i, 0); }
            else {
                program.facts.push_back(nextVar);
                trace_msg(simplifications, 1, "Remapping " << i << " to " << nextVar);
                atomData.remap(i, nextVar++);
            }
            continue;
        }
                
        if(interpretation.isTrueNotSupported(i)) {
            if(!atomData.isEliminated(nextVar) || !program.isHidden(nextVar))
                program.trueAtoms.push_back(nextVar);
        }
        atomData.clearRules(i);
        trace_msg(simplifications, 1, "Remapping " << i << " to " << nextVar);
        atomData.remap(i, nextVar++);        
    }    
    
    trace_msg(simplifications, 1, "Processing binary constraints");
    unsigned int j = 0;
    for(unsigned int i = 0; i < program.binaryConstraints.size(); i++) {
        program.binaryConstraints[j]=program.binaryConstraints[i];
        assert(!program.binaryConstraints[j]->isViolated(interpretation));
        if(program.binaryConstraints[j]->isSatisfied(interpretation)) delete program.binaryConstraints[i];
        else {
            program.binaryConstraints[i]->remap(atomData);
            j++;
        }
    }
    program.binaryConstraints.resize(j);    
    
    trace_msg(simplifications, 1, "Processing rules");
    j = 0;
    for(unsigned int i = 0; i < program.rules.size(); i++) {
        program.rules[j]=program.rules[i];
        if(program.rules[i]->isRemoved()) delete program.rules[i];
        else {
            program.rules[i]->remap(atomData);
            j++;
        }
    }
    program.rules.resize(j);    
    
    trace_msg(simplifications, 1, "Processing aggregates");
    j = 0;
    for(unsigned int i = 0; i < program.aggregates.size(); i++) {
        program.aggregates[j]=program.aggregates[i];
        Aggregate& aggr = *program.aggregates[i];
        if(aggr.isRemoved()) delete program.aggregates[i];
        else {
            for(unsigned int k = 0; k < aggr.numberOfIdBounds(); k++) {
                unsigned int aggrId = aggr.getId(k);
                unsigned int newVar = atomData.getNewVar(aggrId);
                assert(program.options.printOption() == PREASP_PRINT_EXTENDED_DIMACS || newVar != 0);

                if(interpretation.isTrue(aggrId)) aggr.setIdTrue(k);
                else if(interpretation.isFalse(aggrId)) aggr.setIdFalse(k);
                
                aggr.remapId(k, newVar);
            }
            aggr.remap(atomData);
            j++;
        }
    }
    program.aggregates.resize(j);
    
    for(unsigned int i = 0; i < program.minimizeConstraints.size(); i++) program.minimizeConstraints[i]->simplifyAndRemap(interpretation, atomData);    
    
    
    program.remapAtomTable(atomData);
    program.numberOfVars=nextVar-1;
//    for(unsigned int i = 1; i < interpretation.size(); i++) {
//        if(atomData.isAggregateAtom(i)) {
//            if(interpretation.isTrue(i))
//                data.facts.push_back(i);
//            else if(interpretation.isFalse(i))
//                data.falseAtoms.push_back(i);
//            atomData.clearRules(i);
//            continue;
//        }
//            
//        if(interpretation.isFalse(i)) { data.falseAtoms.push_back(i); continue; }
//        if(interpretation.isTrueSupported(i)) { data.facts.push_back(i); continue; }
//        if(interpretation.isTrueNotSupported(i)) data.trueAtoms.push_back(i);
//        atomData.clearRules(i);
//    }
}

}
