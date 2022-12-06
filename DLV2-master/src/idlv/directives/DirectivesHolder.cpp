/*******************************************************************************
 * Copyright 2016 Francesco Calimeri, Davide Fusca', Simona Perri and Jessica Zangari
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/
/*
 * DirectivesHolder.cpp
 *
 *  Created on: Jul 13, 2016
 *      Author: jessica
 */

#include "DirectivesHolder.h"

namespace DLV2 {
namespace grounder {

DirectivesHolder* DirectivesHolder::directivesHolder=0;

DirectivesHolder* DirectivesHolder::getInstance(){
	if(directivesHolder==0)
		directivesHolder=new DirectivesHolder();
	return directivesHolder;
}

void DirectivesHolder::freeInstance(){
	if(directivesHolder==0) return;
	for(auto& d:directivesHolder->connections)
		delete d.second;
	for(auto& d:directivesHolder->importDirectives)
		delete d;
	for(auto& d:directivesHolder->exportDirectives)
		delete d;
	delete directivesHolder;
	directivesHolder=0;
}

void DirectivesHolder::applyShows(){
	if(!showDirective.size())return;

	auto output = OutputBuilder::getInstance();
	output->clearStreamAtomTable();
	PredicateTable::getInstance()->setAllPredicateHidden();

	for(auto& sd : showDirective){
		auto p = sd.getPredicate();
		p->setHiddenForPrinting(false);
//		auto pext = PredicateExtTable::getInstance()->getPredicateExt(p);
//		if(pext==nullptr)continue;
//		for(auto atom :*pext->getTable(FACT)){
//			output->onFact(atom);
//			output->appendToStreamAtomTable(atom,atom->isFact());
//		}
	}
}

}}




