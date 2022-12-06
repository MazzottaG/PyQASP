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
 * DecompositionOptimizer.h
 *
 *  Created on: May 19, 2017
 *      Author: jessica
 */

#ifndef SRC_GROUNDER_GROUND_DECOMPOSITIONOPTIMIZER_H_
#define SRC_GROUNDER_GROUND_DECOMPOSITIONOPTIMIZER_H_



#ifndef HTD

#include "../statement/Rule.h"

namespace DLV2 {
namespace grounder {
class DecompositionOptimizer {
public:
	DecompositionOptimizer(Rule* rule){};
	void decompose(){}
};

}}

#else
#include "../statement/Rule.h"
#include "../table/PredicateExtension.h"
#include "../table/PredicateTable.h"
#include "../statement/OrderRule.h"
#include "../statement/InputRewriter.h"
#include "../statement/OrderRule.h"
#include "IntelligentDecomposer.h"
#include "../../../common/Options.h"
#include <float.h>

#include <htd/main.hpp>
#include <list>

using namespace htd;

namespace DLV2 {
namespace grounder {

class DecompositionOptimizer {
private:
	Rule* ruleToDecompose;
	unique_ptr<LibraryInstance> libraryInstance;
	map_term<vertex_t> mapVertexVariable;
	vertex_t firstVertex;
	vector<Term*> variablesInRuleToDecompose;
	OrderRule* orderRuleToDecompose;
	BaseInputRewriter inputRewriter;
	set_term variablesInRoot;

	unordered_map<double,pair<Rule*,vector<Rule*>>> decompositions;
	double best;

	Hypergraph* convertRuleToHypergraph() ;
	void visitNode(vertex_t node,const ITreeDecomposition& decomposition,vector<vertex_t>& verticesRules) const;

public:
	~DecompositionOptimizer(){
		delete orderRuleToDecompose;
		ruleToDecompose->free();
		delete ruleToDecompose;
		for(auto d:decompositions){
			if(d.first!=best){
				d.second.first->free();
				delete d.second.first;
				for(auto r:d.second.second){
					r->free();
					delete r;
				}
			}
		}
	}
	DecompositionOptimizer(Rule* rule):ruleToDecompose(rule->clone()),libraryInstance(createManagementInstance(Id::FIRST)),
			firstVertex(0),orderRuleToDecompose(nullptr),best(numeric_limits<double>::lowest())
	{
		ruleToDecompose->getVariablesInHead(variablesInRoot);
		if(ruleToDecompose->isWeakConstraint()){
			const WeakConstraint* w=dynamic_cast<const WeakConstraint*>(ruleToDecompose);
			w->getWeightLevelLabeltVariables(variablesInRoot);
		}
		ruleToDecompose->setIndex(rule->getIndex());
		unsigned int seed = Options::globalOptions()->getDecompositionSeed();
		srand(seed);
		set_term ruleVariables;
		ruleToDecompose->getGlobalVariables(ruleVariables);
		for(auto v:ruleVariables)
			variablesInRuleToDecompose.push_back(v);
	}

	void storeDecomposition(double eval,Rule*& ruleDecomposed,vector<Rule*>& decompositionResultingRules){
		if(!std::isnan(eval) && eval>numeric_limits<double>::lowest() && !decompositions.count(eval)){
			decompositions.insert({eval,{ruleDecomposed,decompositionResultingRules}});
		}
		else{
			if(ruleDecomposed!=0){
				ruleDecomposed->free();
				delete ruleDecomposed;
			}
			for(auto& r:decompositionResultingRules){
				if(r!=0){
					r->free();
					delete r;
				}
			}
		}
	}

	void decompose();
	bool convertDecompositionToRules(const ITreeDecomposition& decomposition,Rule*& ruleDecomposed,vector<Rule*>& auxRules);

	vertex_t getVariableId(Term* t) const {
		return mapVertexVariable.at(t);
	}
	Term* getIdVariable(vertex_t vertex) const {
		return variablesInRuleToDecompose[vertex - firstVertex];
	}
	const Rule* getRuleToDecompose() const {
		return ruleToDecompose;
	}

	vertex_t findContainingBag(const ITreeDecomposition& decomp, vertex_t start, list<vertex_t>::const_iterator begin, list<vertex_t>::const_iterator end)
	{
		list<vertex_t> bag(decomp.bagContent(start).begin(), decomp.bagContent(start).end());
		bag.sort();
		if(includes(bag.begin(), bag.end(), begin, end))
			return start;
		ConstCollection<vertex_t> children = decomp.children(start);
		for(ConstIterator<vertex_t> it = children.begin(); it != children.end(); ++it)
		{
			vertex_t recRes = findContainingBag(decomp, *it, begin, end);
			if(recRes != 0)
				return recRes;
		}
		return 0;
	}

	LibraryInstance* getLibraryInstance() const {
		return libraryInstance.get();
	}

	const set_term& getVariablesInRoot() const
	{
		return variablesInRoot;
	}
};

class RootingOperation : public ITreeDecompositionManipulationOperation
{ // operation that searches the decomposition for a node whose bag includes all of a certain statement's head's variables and makes this node the decomp's root node.
public:
	RootingOperation(DecompositionOptimizer* opt):decompositionOptimizer(opt),libraryInstance(opt->getLibraryInstance()){};
	~RootingOperation(){};
	void apply(const IMultiHypergraph & graph, IMutableTreeDecomposition & decomposition) const {
		set_term varsInHead=decompositionOptimizer->getVariablesInRoot();
		list<vertex_t> headVertices;
		for (auto it = varsInHead.begin();  it != varsInHead.end(); ++it)
			headVertices.push_back(decompositionOptimizer->getVariableId(*it));
		headVertices.sort();
		decomposition.makeRoot(decompositionOptimizer->findContainingBag(decomposition, decomposition.root(), headVertices.begin(), headVertices.end()));
	}
	void apply(const IMultiHypergraph & graph, IMutableTreeDecomposition & decomposition, const vector<vertex_t> & relevantVertices, vector<vertex_t> & createdVertices, vector<vertex_t> & removedVertices) const {apply(graph,decomposition);}
	void apply(const IMultiHypergraph & graph, IMutableTreeDecomposition & decomposition, const vector<ILabelingFunction *> & labelingFunctions) const {apply(graph,decomposition);}
	void apply(const IMultiHypergraph & graph, IMutableTreeDecomposition & decomposition, const vector<vertex_t> & relevantVertices, const vector<ILabelingFunction *> & labelingFunctions, vector<vertex_t> & createdVertices, vector<vertex_t> & removedVertices) const {apply(graph,decomposition);}
	bool isLocalOperation(void) const{return false;}
	bool createsTreeNodes(void) const{return false;}
	bool removesTreeNodes(void) const{return false;}
	bool modifiesBagContents(void) const{return false;}
	bool createsSubsetMaximalBags(void) const{return false;}
	bool createsLocationDependendLabels(void) const{return false;}
	RootingOperation * clone(void) const{return new RootingOperation(decompositionOptimizer);}
	RootingOperation * cloneTreeDecompositionManipulationOperation(void) const {return clone();}
	const LibraryInstance * managementInstance(void) const HTD_NOEXCEPT{return libraryInstance;}
	void setManagementInstance(const LibraryInstance * const manager){this->libraryInstance=manager;};
	const RootingOperation * cloneDecompositionManipulationOperation(void) const {return clone();}
private:
	DecompositionOptimizer* decompositionOptimizer;
	const LibraryInstance * libraryInstance;
};

class FitnessFunction : public ITreeDecompositionFitnessFunction
{
    public:
        FitnessFunction(DecompositionOptimizer* opt):decompositionOptimizer(opt){}
        virtual ~FitnessFunction(){}
        virtual FitnessEvaluation * fitness(const IMultiHypergraph & graph, const ITreeDecomposition & decomposition) const;
        virtual FitnessFunction * clone(void) const {return new FitnessFunction(decompositionOptimizer);}
    protected:
        DecompositionOptimizer* decompositionOptimizer;
};

class FitnessFunctionWidth : public FitnessFunction
{
    public:
		FitnessFunctionWidth(DecompositionOptimizer* opt):FitnessFunction(opt){}
		virtual ~FitnessFunctionWidth(){}
		virtual FitnessEvaluation * fitness(const IMultiHypergraph & graph, const ITreeDecomposition & decomposition) const;
		virtual FitnessFunction * clone(void) const {return new FitnessFunctionWidth(decompositionOptimizer);}
};
} /* namespace grounder */
} /* namespace DLV2 */
#endif


#endif /* SRC_GROUNDER_GROUND_DECOMPOSITIONOPTIMIZER_H_ */
