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
 * File:   DirectiveFactory.h
 * Author: cesco
 *
 * Created on 29 maggio 2014, 22.58
 */

#ifndef DIRECTIVEFACTORY_H
#define	DIRECTIVEFACTORY_H

#include "Directive.h"
#include "DBDirective.h"
#include "DirectivesHolder.h"
#include "ImportDirective.h"
#include "ExportDirective.h"
#include "ExtPredConversionDirective.h"

namespace DLV2
{
    
// FIXME
class DirectiveFactory {
public:
	static Directive* createDirective( char* dirName, char* dirValue ) { return new Directive(dirName,dirValue); }
	static Directive* createImportDirective(char* dirValue ) {
		grounder::DBDirective* directive = new grounder::ImportDirective(dirValue);
		if(directive->split()){
			grounder::DirectivesHolder::getInstance()->addImportDirective(directive);
			return directive;
		}
		return 0;
	}
	static Directive* createExportDirective(char* dirValue ) {
		grounder::DBDirective* directive = new grounder::ExportDirective(dirValue);
		if(directive->split()){
			grounder::DirectivesHolder::getInstance()->addExportDirective(directive);
			return directive;
		}
		return 0;
	}

	static void createSparqlImportDirective(char* dirValue ) {
		grounder::SparqlImportDirective directive(dirValue);
		if(directive.split()){
			grounder::DirectivesHolder::getInstance()->addSparqlImportDirective(directive);
		}
	}

	static void createShowDirective(char* dirValue ){
		grounder::ShowDirective directive(dirValue);
		if(directive.split()){
			directive.getPredicate()->setHiddenForPrinting(false);
			grounder::DirectivesHolder::getInstance()->addShowDirective(directive);
		}
	}

	static void createShowDirective(const char* dirValue ){
		grounder::ShowDirective directive(dirValue);
		if(directive.split())
			grounder::DirectivesHolder::getInstance()->addShowDirective(directive);
	}

	static void createExtPredTypeDirective(char* dirValue ){
		grounder::ExtPredConversionDirective directive(dirValue);
		if(directive.split())
			grounder::DirectivesHolder::getInstance()->addExtPredConversionDirective(directive);
	}


private:
	DirectiveFactory() { }
	DirectiveFactory(const DirectiveFactory&) { }
	~DirectiveFactory() { }
        
    };
    
};

#endif	/* DIRECTIVEFACTORY_H */

