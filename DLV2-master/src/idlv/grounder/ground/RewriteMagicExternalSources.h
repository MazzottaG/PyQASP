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
 * RewriteMagicExternalSources.h
 *
 *  Created on: 04 gen 2017
 *      Author: jessica
 */

#ifndef SRC_GROUNDER_GROUND_REWRITEMAGICEXTERNALSOURCES_H_
#define SRC_GROUNDER_GROUND_REWRITEMAGICEXTERNALSOURCES_H_

#include "StatementDependency.h"
#include "../atom/Predicate.h"
#include "../../directives/Query.h"

namespace DLV2 {
namespace grounder {

/// This class is responsible of applying magic sets technique within external sources. It is a Singleton.
class RewriteMagicExternalSources {
public:
	///Facade method that applies syntacting rewriting on queries over heterogeneous external sources.
	///To be invoked after that magic sets rewriting is applied.
	void applyMagicToExternalSources();
	static RewriteMagicExternalSources* getInstance(){
		if(rewriteMagicExternalSources == nullptr) rewriteMagicExternalSources = new RewriteMagicExternalSources;
		return rewriteMagicExternalSources;
	}
	void freeInstance(){delete rewriteMagicExternalSources;}
	/// Inform this class of the presence of an import directive acting on the given predicate with the specified query
	void addMapping(Predicate* p,Query* q){predicatesInImportQueries.insert({p,q});};
	/// Inform this class of the presence of an import directive applied by means of an external sparql atom,
	/// acting on the given predicate with the specified auxiliary rule
	void addMapping(Predicate* p,Rule* q){predicatesInSparqlPythonQueries.insert({p,q});};
private:
	static RewriteMagicExternalSources* rewriteMagicExternalSources;
	RewriteMagicExternalSources(){}

	/// Determine the variables present in magic atoms,
	/// storing the involved predicates and the appearing positions.
	void determineInvolvedVariablesInMagicAtoms() {
		variablesInMagicAtoms.resize(
				StatementDependency::getInstance()->getRulesSize());
		for (auto it = StatementDependency::getInstance()->getBeginRules();
				it != StatementDependency::getInstance()->getEndRules(); ++it) {
			for (auto it2 = (*it)->getBeginBody(); it2 != (*it)->getEndBody();
					++it2) {
				Atom* atom = (*it2);
				if (!atom->isNegative() && atom->getPredicate() != nullptr
						&& atom->getPredicate()->getName().find("magic#")
								!= string::npos) {
					for (unsigned i = 0; i < atom->getTermsSize(); ++i)
						variablesInMagicAtoms[(*it)->getIndex()].insert(
								{ atom->getTerm(i), { atom->getPredicate(), i } });
				}
			}
		}
//		for(auto e:variablesInMagicAtoms){
//			for(auto ee:e){
//				cerr<<"Term: "<<endl;
//				ee.first->print(cerr);
//				cerr<<"Predicate: "<<ee.second.first->getName()<<endl;
//				cerr<<"Pos: "<<ee.second.second<<endl;
//			}
//		}
	}

	///Map each variable appearing in a magic atom with its predicate and its position.
	vector<map_term<pair<Predicate*,unsigned>>> variablesInMagicAtoms;

	///Map each predicate involved in an import directive with the specified query
	unordered_map<Predicate*,Query*,IndexForTable<Predicate>,IndexForTable<Predicate>> predicatesInImportQueries;
	///Map each predicate involved in an import sparql python external atom with the auxiliary rule created for its management
	unordered_map<Predicate*,Rule*,IndexForTable<Predicate>,IndexForTable<Predicate>> predicatesInSparqlPythonQueries;

};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_GROUND_REWRITEMAGICEXTERNALSOURCES_H_ */
