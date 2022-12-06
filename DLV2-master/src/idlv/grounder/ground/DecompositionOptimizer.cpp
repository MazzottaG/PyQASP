/*******************************************************************************
 *   Copyright 2016 Francesco Calimeri, Davide Fusca', Simona Perri and Jessica Zangari
 *  
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *  
 *        http://www.apache.org/licenses/LICENSE-2.0
 *  
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *******************************************************************************/
/*
 * DecompositionOptimizer.cpp
 *
 *  Created on: May 19, 2017
 *      Author: jessica
 */


#include "DecompositionOptimizer.h"
#include "DecompositionMapper.h"

namespace DLV2 {
namespace grounder {

#ifdef HTD

void DecompositionOptimizer::decompose() {
	if(ruleToDecompose->getSizeBody()<=2 || ruleToDecompose->containsExternalLits()) return;
	libraryInstance->treeDecompositionAlgorithmFactory().setConstructionTemplate(new BucketEliminationTreeDecompositionAlgorithm(libraryInstance.get()));
	IOrderingAlgorithm *orderAlg;
	switch(Options::globalOptions()->getDecompositionAlgorithm())
	{
        case 0:
        	orderAlg = new MinDegreeOrderingAlgorithm(libraryInstance.get());
        	break;
        case 1:
        	orderAlg = new MaximumCardinalitySearchOrderingAlgorithm(libraryInstance.get());
			break;
        case 2:
        	orderAlg = new MinFillOrderingAlgorithm(libraryInstance.get());
			break;
        case 3:
        	orderAlg = new NaturalOrderingAlgorithm(libraryInstance.get());
			break;
        default:
        	orderAlg = new MinDegreeOrderingAlgorithm(libraryInstance.get());
			break;
	}
	libraryInstance->orderingAlgorithmFactory().setConstructionTemplate(orderAlg);

	IHypergraph* ruleHypergraph=convertRuleToHypergraph();


	if(Options::globalOptions()->isDisabledDecompFitness() || ruleToDecompose->getSizeBody()>=Options::globalOptions()->getDecompLimitSizeBodyFitness()){
		//NO FITNESS
		RootingOperation* rootingOperation=new RootingOperation(this);
		// OLD HTD VERSION
		// unique_ptr<htd::ITreeDecompositionAlgorithm> treeDecompositionAlgorithm(libraryInstance->treeDecompositionAlgorithmFactory().getTreeDecompositionAlgorithm());
		unique_ptr<ITreeDecompositionAlgorithm> treeDecompositionAlgorithm(libraryInstance->treeDecompositionAlgorithmFactory().createInstance());
		treeDecompositionAlgorithm->addManipulationOperation(rootingOperation);
		unique_ptr<ITreeDecomposition> decomposition(treeDecompositionAlgorithm->computeDecomposition(*ruleHypergraph));
		Rule* ruleDecomposed=0;
		vector<Rule*> decompRules;
		if(convertDecompositionToRules(*decomposition,ruleDecomposed,decompRules) && ruleDecomposed!=0){
			DecompositionMapper::getInstance()->add(ruleDecomposed,decompRules);
		}
	}
	else{
		FitnessFunction* fitnessFunction = new FitnessFunction(this);
		TreeDecompositionOptimizationOperation* operation=new TreeDecompositionOptimizationOperation(libraryInstance.get(), fitnessFunction);
		operation->setManagementInstance(libraryInstance.get());
		RootingOperation* rootingOperation=new RootingOperation(this);
		ITreeDecompositionAlgorithm* treeDecompositionAlgorithm=libraryInstance->treeDecompositionAlgorithmFactory().createInstance();
		IterativeImprovementTreeDecompositionAlgorithm algorithm(libraryInstance.get(),treeDecompositionAlgorithm,fitnessFunction->clone());
		algorithm.addManipulationOperation(operation);
		algorithm.addManipulationOperation(rootingOperation);
		algorithm.setIterationCount(Options::globalOptions()->getDecompositionIterations());
		algorithm.setNonImprovementLimit(Options::globalOptions()->getDecompositionNotImprovementLimit());

		unique_ptr<ITreeDecomposition> d(algorithm.computeDecomposition(*ruleHypergraph,
			[&](const IMultiHypergraph & graph,
			const ITreeDecomposition & decomp,
			const FitnessEvaluation & fitness)
			{
				HTD_UNUSED(graph);
				HTD_UNUSED(decomp);
				double eval=fitness[0];
				if(eval>best)
					best=eval;
			}
		));
		if(!decompositions.empty()){
			Rule* decomposedRule=decompositions.at(best).first;
			vector<Rule*> decompositionResultingRules=decompositions.at(best).second;
			DecompositionMapper::getInstance()->add(decomposedRule,decompositionResultingRules);
		}
	}

	delete ruleHypergraph;
}

bool comp(Term* t1,Term* t2){
	return t1->getName()<t2->getName();
}

Hypergraph* DecompositionOptimizer::convertRuleToHypergraph() {
	Hypergraph* hg = new Hypergraph(libraryInstance.get());
	sort(variablesInRuleToDecompose.begin(),variablesInRuleToDecompose.end(),comp);
	firstVertex = hg->addVertices(variablesInRuleToDecompose.size());
	for (auto it = variablesInRuleToDecompose.begin(); it != variablesInRuleToDecompose.end(); ++it)
		mapVertexVariable.insert({*it, (firstVertex + it - variablesInRuleToDecompose.begin())});
	vector<vertex_t> head;
	for (auto v:variablesInRoot)
		head.push_back(getVariableId(v));
	if(!head.empty())
		hg->addEdge(head);
	for (vector<Atom*>::const_iterator it1 = ruleToDecompose->getBeginBody(); it1 != ruleToDecompose->getEndBody(); ++it1)
	{
		vector<vertex_t> edge;
		set_term vars;
		if(!(*it1)->isAggregateAtom())
			vars=(*it1)->getVariable();
		else{
			vars=(*it1)->getSharedVariable(ruleToDecompose->getBeginBody(),ruleToDecompose->getEndBody());
			set_term guards=(*it1)->getGuardVariable();
			vars.insert(guards.begin(),guards.end());
		}
		edge.reserve(vars.size());
		for(auto v:vars)
			edge.push_back(getVariableId(v));
		if(!edge.empty())
			hg->addEdge(edge);
	}

	return hg;
}

bool DecompositionOptimizer::convertDecompositionToRules(const ITreeDecomposition& decomposition,Rule*& ruleDecomposed,vector<Rule*>& rules){
	vector<vertex_t> verticesRules;
	verticesRules.push_back(decomposition.root());

	set_term root;
	for (auto it1: decomposition.bagContent(decomposition.root()))
		root.insert(getIdVariable(it1));
	if(!Utils::isContained(variablesInRoot,root))
		return false;

	visitNode(decomposition.root(), decomposition, verticesRules);
	unsigned count=0;

	unordered_set<unsigned> usedAtoms;
	unordered_map<vertex_t,vector<Atom*>> temporaryChildAtoms;
	//Build each decomposed rule and ensure its safety
	for(auto it=verticesRules.rbegin();it!=verticesRules.rend();++it,++count){
		vertex_t vertex=*it;
//		cerr<<"VERTEX: "<<vertex<<" "<<decomposition.isRoot(vertex)<<"--> ";
//		for(auto e:decomposition.bagContent(vertex))
//			{getIdVariable(e)->print(cerr);cerr<<" ";}
//		cerr<<endl;
		vertex_t parent=decomposition.parent(vertex);
		set_term varsInBag;
		for (auto it1 : decomposition.bagContent(vertex))
			varsInBag.insert(getIdVariable(it1));
		Rule* newRule=0;
		if(ruleToDecompose->isWeakConstraint() && decomposition.isRoot(vertex))
			newRule=new WeakConstraint;
		else
			newRule=new Rule;
		vector<unsigned> estratedAtomsPos;
		ruleToDecompose->extractAtomsInBodyContainingVariables(varsInBag,estratedAtomsPos);
		for(auto atomPos:estratedAtomsPos){
			if(!usedAtoms.count(atomPos) ){
				newRule->addInBody(ruleToDecompose->getAtomInBody(atomPos)->clone());
				usedAtoms.insert(atomPos);
			}
		}
		for(auto a:temporaryChildAtoms[vertex])
			newRule->addInBody(a->clone());
		if(!decomposition.isRoot(vertex)){
			string headPredName=DECOMPOSITION_PREDICATES_PREFIX+"_"+to_string(ruleToDecompose->getIndex())+"_"+to_string(count);
			auto intersectionVertices = decomposition.rememberedVertices(parent, vertex);
			vector<Term*> headTerms;
			for (auto it2 :intersectionVertices)
				headTerms.push_back(getIdVariable(it2));
			Predicate* predicate=new Predicate(headPredName,headTerms.size());
			predicate->setHiddenForPrinting(true);
			predicate->setIdb();
			PredicateTable::getInstance()->insertPredicate(predicate);
			PredicateExtTable::getInstance()->addPredicateExt(predicate);
			Atom* auxiliaryAtom=new ClassicalLiteral(predicate,headTerms,false,false);
			newRule->setHead({auxiliaryAtom});
			temporaryChildAtoms[parent].push_back(auxiliaryAtom);
		}
		else{
			if(ruleToDecompose->isWeakConstraint()){
				WeakConstraint* w=dynamic_cast<WeakConstraint*>(newRule);
				w->setWeight(ruleToDecompose->getWeight());
				w->setLevel(ruleToDecompose->getLevel());
				w->setLabel(ruleToDecompose->getLabel());
			}
			for(auto atom:ruleToDecompose->getHead())
				newRule->addInHead(atom->clone());
		}

		//Safety check
		OrderRule order(newRule);
		bool safe=order.order();
		set_term safeVars=order.getSafeVariables();
		set_term allVars,unsafeVars; newRule->getGlobalVariables(allVars);
		Utils::differenceSet(allVars,safeVars,unsafeVars);
		if(!safe){
			if(orderRuleToDecompose==nullptr){
				orderRuleToDecompose=new OrderRule(ruleToDecompose);
				orderRuleToDecompose->order();
			}
			Rule* newSafeRule=new Rule;
			string headPredName=DECOMPOSITION_PREDICATES_PREFIX+"_"+DOMAIN_PRED_SUFFIX+"_"+to_string(ruleToDecompose->getIndex())+"_"+to_string(count);
			vector<Term*> headTerms;
			headTerms.insert(headTerms.end(),unsafeVars.begin(),unsafeVars.end());
			Predicate* predicate=new Predicate(headPredName,headTerms.size());
			predicate->setHiddenForPrinting(true);
			predicate->setIdb();
			PredicateTable::getInstance()->insertPredicate(predicate);
			PredicateExtTable::getInstance()->addPredicateExt(predicate);
			Atom* auxiliaryAtom=new ClassicalLiteral(predicate,headTerms,false,false);
			newSafeRule->setHead({auxiliaryAtom});

			unordered_set<unsigned> atomsAdded;
			list<unsigned> toVisit;
			bool safe2=false;
			for(unsigned i=0;i<estratedAtomsPos.size();++i){
				for(auto dep:orderRuleToDecompose->getAtomsFromWhichDepends(estratedAtomsPos[i])){
					Atom* atom=ruleToDecompose->getAtomInBody(dep.first);
					newSafeRule->addInBody(atom->clone());
					OrderRule order2(newSafeRule);
					safe2=order2.order();
					set_term safeVars2=order2.getSafeVariables();
					set_term allVars2,unsafeVars2; newSafeRule->getGlobalVariables(allVars2);
					Utils::differenceSet(allVars2,safeVars2,unsafeVars2);
					set_term atomVars;
					if(atom->isAggregateAtom())
						atomVars=atom->getSharedVariable(newSafeRule->getBeginBody(),newSafeRule->getEndBody());
					else
						atomVars=atom->getVariable();
					if(!Utils::isDisjoint(atomVars,unsafeVars2) || !Utils::isDisjoint(atomVars,unsafeVars)){
						atomsAdded.insert(dep.first);
						toVisit.push_back(dep.first);
					}
					if(safe2)
						break;
				}
			}
			while(!toVisit.empty()){
				unsigned next=toVisit.front();
				toVisit.pop_front();
				for(auto dep:orderRuleToDecompose->getAtomsFromWhichDepends(next)){
//					Atom* atom=ruleToDecompose->getAtomInBody(dep.first);
//					newSafeRule->addInBody(atom->clone());
//					OrderRule order2(newSafeRule);
//					safe2=order2.order();
//					set_term safeVars2=order2.getSafeVariables();
//					set_term allVars2,unsafeVars2; newSafeRule->getGlobalVariables(allVars2);
//					Utils::differenceSet(allVars2,safeVars2,unsafeVars2);
//					set_term atomVars;
//					if(atom->isAggregateAtom())
//						atomVars=atom->getSharedVariable(newSafeRule->getBeginBody(),newSafeRule->getEndBody());
//					else
//						atomVars=atom->getVariable();
//					if(!Utils::isDisjoint(atomVars,unsafeVars2) || !Utils::isDisjoint(atomVars,unsafeVars)){
						atomsAdded.insert(dep.first);
						toVisit.push_back(dep.first);
//					}
//					if(safe2)
//						break;
				}
			}

			newSafeRule->deleteBody([](Atom* atom){
				return 2;
			});

			vector<Atom*> bodyAtoms;
			for(auto atom:atomsAdded)
				bodyAtoms.push_back(ruleToDecompose->getAtomInBody(atom)->clone());

			newSafeRule->setBody(bodyAtoms);

			OrderRule order1(newSafeRule);
			safe2=order1.order();
			if(!safe2){
				rules.push_back(newSafeRule);
				rules.push_back(newRule);
				return false;
			}

			newRule->addInBody(auxiliaryAtom->clone());
			OrderRule order2(newRule);
			safe=order2.order();
			if(!safe){
				rules.push_back(newSafeRule);
				rules.push_back(newRule);
				return false;
			}

			BaseInputRewriter ir;
			ir.projectAtoms(newSafeRule,rules,AUXILIARY_PROJ_DECOMP);
			rules.push_back(newSafeRule);
		}
		if(!decomposition.isRoot(vertex)){
			BaseInputRewriter ir;
			ir.projectAtoms(newRule,rules,AUXILIARY_PROJ_DECOMP);
			rules.push_back(newRule);
		}
		else{
			newRule->setIndex(ruleToDecompose->getIndex());
			BaseInputRewriter ir;
			ir.projectAtoms(newRule,rules,AUXILIARY_PROJ_DECOMP);
			ruleDecomposed=newRule;
		}
	}
	return true;
}

void DecompositionOptimizer::visitNode(vertex_t node,const ITreeDecomposition& decomposition,vector<vertex_t>& verticesRules) const
{
	ConstCollection<vertex_t> children = decomposition.children(node);
	if (!children.empty())
	{
		for (auto it = children.begin(); it != children.end(); ++it)
			verticesRules.push_back(*it);
		for (auto it = children.begin(); it != children.end(); ++it)
			visitNode(*it,decomposition, verticesRules);
	}
}

FitnessEvaluation* FitnessFunction::fitness(const IMultiHypergraph & graph, const ITreeDecomposition & decomposition) const {
    HTD_UNUSED(graph)
	vector<Rule*> decompositionResultingRules;
	Rule* decomposedRule=0;
	double eval=0;
	if(decompositionOptimizer->convertDecompositionToRules(decomposition,decomposedRule,decompositionResultingRules) && decomposedRule!=0){
		eval=IntelligentDecomposer::getInstance()->evaluateDecomposition(decomposedRule,decompositionResultingRules.begin(),decompositionResultingRules.end());
		for(auto r:decompositionResultingRules)
			IntelligentDecomposer::getInstance()->cleanTempPredicatesStatistics(r->getAtomInHead(0)->getPredicate());
		eval=eval*-1;
	}
	else
		eval=numeric_limits<double>::lowest();
	decompositionOptimizer->storeDecomposition(eval,decomposedRule,decompositionResultingRules);
    return new FitnessEvaluation(1,eval);
}

FitnessEvaluation* FitnessFunctionWidth::fitness(const IMultiHypergraph & graph, const ITreeDecomposition & decomposition) const {
    HTD_UNUSED(graph)
	vector<Rule*> decompositionResultingRules;
	Rule* decomposedRule=0;
	double eval=0;
	if(decompositionOptimizer->convertDecompositionToRules(decomposition,decomposedRule,decompositionResultingRules) && decomposedRule!=0){
		eval=decomposition.maximumBagSize();
		eval=eval*-1;
	}
	else
		eval=numeric_limits<double>::lowest();
	decompositionOptimizer->storeDecomposition(eval,decomposedRule,decompositionResultingRules);
    return new FitnessEvaluation(1,eval);
}


#endif




} /* namespace grounder */
} /* namespace DLV2 */

