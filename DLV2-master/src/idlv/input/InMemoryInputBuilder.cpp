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
 * InMemoryInputBuilder.cpp
 *
 *  Created on: Jan 19, 2015
 *      Author: davide
 */

#include "InMemoryInputBuilder.h"
#include "../grounder/atom/ClassicalLiteral.h"
#include "../grounder/atom/BuiltInAtom.h"
#include "../grounder/ground/ProgramGrounder.h"
#include "../grounder/atom/AggregateAtom.h"
#include "../grounder/atom/Choice.h"
#include "../util/Utils.h"
#include "../grounder/statement/GroundingPreferences.h"
#include "../grounder/exception/ExtAtomException.h"
#include "ExtAtomFactory.h"
#include "../../common/ErrorMessage.h"
#include "../util/WarningMessage.h"
#include "../directives/DirectiveFactory.h"
#include "../directives/ShowDirective.h"
#include "../grounder/term/ListTerm.h"


#include <string>
#include <regex>
#include <algorithm>

namespace DLV2 {
namespace grounder {

bool InMemoryInputBuilder::foundASafetyOrParsingError=false;
bool InMemoryInputBuilder::currentRuleIsUnsafe=false;

string InMemoryInputBuilder::errorMessage="";

InMemoryInputBuilder::InMemoryInputBuilder() :
	SPARQL_PYTHON_PREDICATE_NAME("sparql"),
	SPARQL_ENDPOINT_PYTHON_PREDICATE_NAME("sparqlEndpoint"),
	termTable(TermTable::getInstance()),
	predicateTable(PredicateTable::getInstance()),
	statementDependency(StatementDependency::getInstance()),
	instancesTable(PredicateExtTable::getInstance()),
	foundARangeAtomInCurrentRule(false),
	currentRule(new Rule()),
	currentAtom(nullptr),
	currentBinop(Binop::NONE_OP),
	currentAggregate(nullptr),
	currentAggregateElement(new AggregateElement),
	currentChoice(nullptr),
	currentChoiceElement(new ChoiceElement),
	weight(nullptr),
	level(nullptr),
	hiddenNewPredicate(false),
	currentRuleOrdering(-1),
	currentProjection(-1),
	currentRewriteArith(false),
	projectAtom(false),
	currentLookAhead(false),
	currentAlignSubstitutions(false),
    	currentRuleToDecompose(false),
    	currentRuleToNotDecompose(false),
	extSeparator(0), 
	onHeadTail( false )
{

	Options * opt=Options::globalOptions();

	switch (opt->getRewritingType()) {
		case DISJUNCTION:
			inputRewriter=new BaseInputRewriter();
			break;
		case COMPACT_NATIVE_CHOICE:
			inputRewriter=new AdvancedChoiceBaseInputRewriter();
			break;
		default:
			inputRewriter=new ChoiceBaseInputRewriter();
			break;
	}

	string filter=opt->getPredicateToFilter();
	if(filter.size()>0){
		//If exist a filter hidden all new predicate and create the predicate to show and set hidden to this predicate to false
		hiddenNewPredicate=true;
		vector<string> splitString=Utils::split(filter,',');
		for(auto predicateArity:splitString){
			std::regex ree("(.*?)/(\\d+)");
			smatch m;
			regex_search(predicateArity,m,ree);
			if(m.size()<2){
				ErrorMessage::errorDuringParsing("--> Invalid filter provided: " + predicateArity);
				return;
			}
			predicateArity.append(".");
			DirectiveFactory::createShowDirective(predicateArity.c_str());
		}
	}
	projectAtom = (opt->getRewProject());
#ifdef PYTHON
	extAtomFactory=PythonExtAtomFactory::getInstance();
#else
	extAtomFactory = ExtAtomFactory::getInstance();
#endif
}

InMemoryInputBuilder::~InMemoryInputBuilder(){
	delete currentRule;
	delete currentAtom;
	delete inputRewriter;
	delete currentAggregateElement;
	delete currentChoiceElement;
}

void InMemoryInputBuilder::onDirective(char* directiveName,char* directiveValue) {
	if(foundASafetyOrParsingError) return;
	if(strcmp(directiveName,"#show")==0){
		DirectiveFactory::createShowDirective(directiveValue);
	}
	else if(strcmp(directiveName, "#import_local_sparql") == 0) {
		DirectiveFactory::createSparqlImportDirective(directiveValue);
	}else if(strcmp(directiveName,"#import_remote_sparql")==0){
		string dataset = "";
		string query = "";
		string predicateName = "";
		unsigned arity = 0;
		vector<TypeConv> typeConvs;

		grounder::SparqlImportDirective directive(directiveValue);
		directive.split();
		dataset=directive.getURL();
		query=directive.getQuery();
		predicateName=directive.predicateName();
		arity=directive.predArity();
		typeConvs=directive.getTypeConv();

		Atom* sparqlExtAtom = nullptr;
		sparqlExtAtom = makeSparqlExtAtom(dataset, query, arity, typeConvs);

		Atom* atomInHead = nullptr;
		atomInHead = makeAtomInHead(predicateName, arity);

		vector<Atom*> head(1, atomInHead);
		vector<Atom*> body(1, sparqlExtAtom);
		Rule* r=createRule(&head, &body);
		RewriteMagicExternalSources::getInstance()->addMapping(atomInHead->getPredicate(),r);

	}else if(strcmp(directiveName,"#import_sql")==0){
		DirectiveFactory::createImportDirective(directiveValue);
	}else if(strcmp(directiveName,"#export_sql")==0){
		DirectiveFactory::createExportDirective(directiveValue);
	}else if(strcmp(directiveName,ExtPredConversionDirective::DIR_NAME)==0){
		DirectiveFactory::createExtPredTypeDirective(directiveValue);
	}else{
		string s1(directiveName);
		string s2(directiveValue);
		WarningMessage::warningDuringParsing("--> Provided a not supported directive. It will be ignored: "+s1+s2);
	}
}


Atom* InMemoryInputBuilder::makeSparqlExtAtom(const string& dataset,
		const string& query, unsigned arity, vector<TypeConv>& typeConvs) {
	vector<Term*> terms;

	Term* dsTerm = new StringConstantTerm(false, dataset);
	termTable->addTerm(dsTerm);
	terms.push_back(dsTerm);
	Term* qTerm = new StringConstantTerm(false, query);
	termTable->addTerm(qTerm);
	terms.push_back(qTerm);

	for (unsigned i = 0; i < arity; i++) {
		string name = "T";
		name += to_string(i + 1);
		Term* term = new VariableTerm(false, name);
		termTable->addTerm(term);
		terms.push_back(term);
	}

	Atom* sparqlExtAtom;
//	regex e("(^http://)||(^https://)");
//	smatch m;
//	if(regex_search(dataset,m,e)){
	string name = SPARQL_ENDPOINT_PYTHON_PREDICATE_NAME;
	Predicate * predicate = new Predicate( name, terms.size() );
	predicate->setHiddenForPrinting( hiddenNewPredicate );
	predicateTable->getInstance()->insertPredicate( predicate );
	instancesTable->addPredicateExt( predicate );
	sparqlExtAtom = extAtomFactory->makeExtAtom( predicate, terms, 2, false );
	//todo
	if( sparqlExtAtom == nullptr )
		throw new ExtAtomException( "External atom not found: " + name );
//	}
//	else{
//		string name="sparql";
//		Predicate * predicate = new Predicate(name,terms.size());
//		predicate->setHiddenForPrinting(hiddenNewPredicate);
//		predicateTable->getInstance()->insertPredicate(predicate);
//		instancesTable->addPredicateExt(predicate);
//		sparqlExtAtom = ExtAtomFactory::getInstance()->makeExtAtom(predicate, terms, 2, false);
//	}
	sparqlExtAtom->setTypeConvs( typeConvs );
	return sparqlExtAtom;
}

void InMemoryInputBuilder::onAnnotationGlobalExternal(){
	extAtomFactory->addTypeConvOfExtAtom( predicateExtAtomTypeConv, extAtomTypeConv );
	extAtomTypeConv.clear();
}

void InMemoryInputBuilder::onAnnotationGlobalExternalPredicateName(char* name) {
	predicateExtAtomTypeConv=name;
}

void InMemoryInputBuilder::onAnnotationGlobalExternalType(char* type) {
	//TODO
}

void InMemoryInputBuilder::onAnnotationGlobalExternalTypeQC() {
	extAtomTypeConv.push_back(TypeConv::Q_CONST);
}

void InMemoryInputBuilder::onAnnotationGlobalExternalTypeC() {
	extAtomTypeConv.push_back(TypeConv::_CONST);
}

void InMemoryInputBuilder::onAnnotationGlobalExternalTypeU() {
	extAtomTypeConv.push_back(TypeConv::U_INT);
}

void InMemoryInputBuilder::onAnnotationGlobalExternalTypeUT() {
	extAtomTypeConv.push_back(TypeConv::UT_INT);
}

void InMemoryInputBuilder::onAnnotationGlobalExternalTypeUR() {
	extAtomTypeConv.push_back(TypeConv::UR_INT);
}

void InMemoryInputBuilder::onAnnotationGlobalExternalTypeT() {
	extAtomTypeConv.push_back(TypeConv::T_INT);
}

void InMemoryInputBuilder::onAnnotationGlobalExternalTypeR() {
	extAtomTypeConv.push_back(TypeConv::R_INT);
}

Atom* InMemoryInputBuilder::makeAtomInHead(string& predicateName, unsigned arity) {
	Predicate* predicate = new Predicate(predicateName, arity);
	predicate->setHiddenForPrinting(hiddenNewPredicate);
	predicateTable->getInstance()->insertPredicate(predicate);
	instancesTable->addPredicateExt(predicate);

	vector<Term*> terms;
	for (unsigned i = 0; i < arity; i++) {
		string name = "T";
		name += to_string(i + 1);
		Term* term = new VariableTerm(false, name);
		termTable->addTerm(term);
		terms.push_back(term);
	}

	Atom* atomInHead = new ClassicalLiteral(predicate, terms, false, false);
	return atomInHead;
}

void InMemoryInputBuilder::onAnnotationPartialOrdering(bool global) {
	if(!global){
		currentRuleAtomsAfter.push_back(vector<Atom*>());
		currentRuleAtomsBefore.push_back(vector<Atom*>());
	}
	else{
		GroundingPreferences::getGroundingPreferences()->addGlobalPartialOrder();
	}
}

void InMemoryInputBuilder::clearAnnotationsSetting(){
	currentRuleOrdering = -1;
	currentProjection = -1;
	currentRewriteArith = false;
	currentLookAhead = false;
	currentAlignSubstitutions = false;
	for(auto atom:currentRuleAtomsIndexed)
		delete atom;
	currentRuleAtomsIndexed.clear();
	currentRuleAtomsIndexedArguments.clear();
	for(unsigned i=0;i<currentRuleAtomsBefore.size();++i){
		for(auto atom:currentRuleAtomsBefore[i]){
			if(atom->isAggregateAtom()) continue;
			atom->deleteAtoms();
			delete atom;
		}
		for(auto atom:currentRuleAtomsAfter[i]){
			if(atom->isAggregateAtom()) continue;
			atom->deleteAtoms();
			delete atom;
		}
	}
	currentRuleAtomsBefore.clear();
	currentRuleAtomsAfter.clear();
	for(auto atom:annotationsAtomsToDelete){
		atom->deleteAtoms();
		delete atom;
	}
}

void InMemoryInputBuilder::manageBeforeRuleAnnotations(Rule* currentRule){
        if(currentProjection != -1)
                GroundingPreferences::getGroundingPreferences()->addRuleProjectionType(currentRule, currentProjection);
        if(currentRewriteArith)
                GroundingPreferences::getGroundingPreferences()->addRuleRewArith(currentRule);
}

void InMemoryInputBuilder::manageRuleAnnotations(Rule* currentRule, bool rewritedRule) {
	if (currentRuleOrdering != -1)
		if(!GroundingPreferences::getGroundingPreferences()->addRuleOrderingType(
			currentRule, currentRuleOrdering))
		if(!rewritedRule)
			WarningMessage::warningDuringParsing("The ordering type "+to_string(currentRuleOrdering)+" is not valid. It will be ignored.");

	for (unsigned i = 0; i < currentRuleAtomsIndexed.size(); ++i) {
		AnnotationsError error=GroundingPreferences::getGroundingPreferences()->addRuleAtomIndexingSetting(
				currentRule, currentRuleAtomsIndexed[i],
				currentRuleAtomsIndexedArguments[i]);
		if(!rewritedRule && error==ATOM_NOT_PRESENT){
			WarningMessage::warningDuringParsing("The atom "+currentRuleAtomsIndexed[i]->toString()+" is not present in the following rule: ");
			currentRule->print(cerr);
		}
	}

	for(unsigned i=0;i<currentRuleAtomsBefore.size();++i){
		GroundingPreferences::getGroundingPreferences()->addRulePartialOrder(currentRule);
		for(auto& atom:currentRuleAtomsBefore[i]){
			AnnotationsError error=GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom(currentRule, atom);
			if(!rewritedRule && error==ATOM_NOT_PRESENT){
				WarningMessage::warningDuringParsing("The atom "+atom->toString()+" is not present in the following rule: ");
				currentRule->print(cerr);
			}
		}
		for(auto& atom:currentRuleAtomsAfter[i]){
			AnnotationsError error=GroundingPreferences::getGroundingPreferences()->addRulePartialOrderAtom(currentRule, atom);
			if(!rewritedRule && error==ATOM_NOT_PRESENT){
				WarningMessage::warningDuringParsing("The atom "+atom->toString()+" is not present in the following rule: ");
				currentRule->print(cerr);
			}
		}
		if(!rewritedRule && GroundingPreferences::getGroundingPreferences()->checkRulePartialOrderConflicts(currentRule)==CONFLICT_FOUND){
			WarningMessage::warningDuringParsing("In the following rule the partial ordering specified cannot be applied: ");
			currentRule->print(cerr);
		}
	}
    if(currentLookAhead)
		GroundingPreferences::getGroundingPreferences()->addRuleLookAhead(currentRule);
    if(currentAlignSubstitutions)
    	GroundingPreferences::getGroundingPreferences()->addRuleAlignSubstitutions(currentRule);
}

void InMemoryInputBuilder::onRule() {
	if(foundASafetyOrParsingError) return;
	if(currentRule->isAFact()){
		Atom *fact=*currentRule->getBeginHead();
		if(fact->containsVariablesOrAnonymous()){
			safetyError(false,currentRule);
			foundASafetyOrParsingError=true;
			currentRuleIsUnsafe=true;
			return;
		}
		bool range=fact->containsRangeTerms();
		if(range){
			for(auto t:fact->getTerms())
				if(t->getType()==TermType::FUNCTION && t->contain(RANGE)){
					errorMessage="--> Error: range terms are not supported inside functional terms";
					foundASafetyOrParsingError=true;
					currentRuleIsUnsafe=true;
					return;
				}
			vector<Atom*> atomExpanded;
			expandRangeAtom(fact,atomExpanded);
			for (auto atom : atomExpanded)
				createFact(atom);
		}
		else
			createFact(fact);
		currentRule->clear();
	}else{
		if(foundARangeAtomInCurrentRule){
			vector<vector<Atom*>> headExpanded; expandRulePart(currentRule->getBeginHead(),currentRule->getEndHead(), headExpanded);
			vector<vector<Atom*>> bodyExpanded;
			if(currentRule->getSizeBody()>0) expandRulePart(currentRule->getBeginBody(),currentRule->getEndBody(), bodyExpanded);
			for(auto head: headExpanded){
				if(bodyExpanded.size()>0){
					for(auto body: bodyExpanded)
						createRule(&head,&body);
				}
				else
					createRule(&head);
			}
			for(auto body: bodyExpanded)
				for(auto a:body)
					delete a;
			for(auto head: headExpanded)
				for(auto a:head)
					delete a;
			currentRule->clear();
		}
		else{
			addRule(currentRule);
			currentRule = new Rule;
		}
	}

	foundARangeAtomInCurrentRule=false;
}

void InMemoryInputBuilder::onConstraint() {
	if(foundASafetyOrParsingError) return;
	if(foundARangeAtomInCurrentRule){
		vector<vector<Atom*>> bodyExpanded;
		expandRulePart(currentRule->getBeginBody(),currentRule->getEndBody(), bodyExpanded);
		for(auto body: bodyExpanded)
			createRule(0,&body);
		currentRule->clear();
	}
	else{
		addRule(currentRule);
		currentRule = new Rule;
	}
	foundARangeAtomInCurrentRule=false;
}

void InMemoryInputBuilder::onWeakConstraint() {
	if(foundASafetyOrParsingError) return;
	Rule * weakRule=new WeakConstraint(currentRule->getBody(),weight,level,terms_parsered);
	weakRule->setMustBeRewritedForAggregates(currentRule->isMustBeRewritedForAggregates());
	addRule(weakRule);
	delete currentRule;
	currentRule = new Rule;
	terms_parsered.clear();
	weight=nullptr;
	level=nullptr;

//	if(currentRuleIsUnsafe){
//		safetyError(false,weakRule);
//		return;
//	}
//
//	statementDependency->assignRuleIndex(weakRule);
//	manageBeforeRuleAnnotations(weakRule);
//	if(!weakRule->hasOnlyPositiveClassicalLitsAndNoArithTerms()){
//		OrderRule orderRule(weakRule);
//		bool isSafe = orderRule.order();
//		safetyError(isSafe,weakRule);
//	}
//	statementDependency->addRuleMapping(weakRule);
//
//	manageRuleAnnotations(weakRule);
//	clearAnnotationsSetting();

//	delete currentRule;
//	currentRule = new Rule;
//	terms_parsered.clear();
//	weight=nullptr;
//	level=nullptr;
}

void InMemoryInputBuilder::onQuery() {
	if(foundASafetyOrParsingError) return;
	if(!Options::globalOptions()->rewriteMagic()) return;

	if(Options::globalOptions()->getOutputFormat()!=OUTPUT_TEXTUAL)
		predicateTable->setAllPredicateHidden();
	bool isGround=currentAtom->isGround();
	if(isGround){
		currentAtom->getPredicate()->setHiddenForPrinting(false);
		OutputBuilder::getInstance()->setAtomToFilter(currentAtom);
	}
	if(Options::globalOptions()->getOutputFormat()!=OUTPUT_TEXTUAL)
		OutputBuilder::getInstance()->clearStreamAtomTable();

	statementDependency->addQueryAtom(currentAtom,isGround);
	currentAtom=nullptr;
	if(Options::globalOptions()->getOutputFormat()!=OUTPUT_TEXTUAL)
		hiddenNewPredicate=true;
}

void InMemoryInputBuilder::onHeadAtom() {
	if(foundASafetyOrParsingError) return;
	currentRule->addInHead(currentAtom);
	if(currentAtom->containsAnonymous()){
		currentRuleIsUnsafe=true;
	}
	currentAtom = nullptr;
}

void InMemoryInputBuilder::onHead() {
	if(foundASafetyOrParsingError) return;
}

void InMemoryInputBuilder::onBodyLiteral() {
	if(foundASafetyOrParsingError) return;
	currentRule->addInBody(currentAtom);
	currentAtom=nullptr;
}

void InMemoryInputBuilder::onBody() {
	if(foundASafetyOrParsingError) return;
}

void InMemoryInputBuilder::onNafLiteral(bool naf) {
	if(foundASafetyOrParsingError) return;
	if(naf) statementDependency->negativeAtomFound();
	currentAtom->setNegative(naf);
	if(naf && currentAtom->containsAnonymous()){
		currentRuleIsUnsafe=true;

	}
}

void InMemoryInputBuilder::onAtom(bool isStrongNeg) {
	if(foundASafetyOrParsingError) return;
	if(isStrongNeg){
		//Create a new predicate like the old predicate but with trueNegative = true
		Predicate * oldPredicate=currentAtom->getPredicate();
		string predicatename=oldPredicate->getName();
		Predicate *strongPredicate=new Predicate(predicatename,oldPredicate->getArity(),oldPredicate->isEdb());
		strongPredicate->setTrueNegated(true);
		strongPredicate->setHiddenForPrinting(hiddenNewPredicate);
		predicateTable->getInstance()->insertPredicate(strongPredicate);
		instancesTable->addPredicateExt(strongPredicate);
		currentAtom->setPredicate(strongPredicate);
	}
}

void InMemoryInputBuilder::onExtAtom( bool naf ){
	if( foundASafetyOrParsingError )
		return;
	try{
		Predicate * predicate = new Predicate( externalPredicateName, terms_parsered.size() );
		predicate->setHiddenForPrinting( hiddenNewPredicate );
		predicateTable->getInstance()->insertPredicate( predicate );
		instancesTable->addPredicateExt( predicate );
		if( terms_parsered.size() == 0 )
			extSeparator = 0;
		currentAtom = extAtomFactory->makeExtAtom( predicate, terms_parsered, extSeparator, naf );
		if( currentAtom == nullptr )
			throw ExtAtomException( externalPredicateName + " not found" );

		if( naf )
			statementDependency->negativeAtomFound();
		statementDependency->externalLiteralFound();
	} catch ( ExtAtomException& e ){
		extAtomError( currentRule, e.getMessage() );
	}
	terms_parsered.clear();
}

void InMemoryInputBuilder::onSemicolon() {
	extSeparator = terms_parsered.size();
}

void InMemoryInputBuilder::onExternalPredicateName( char* name ){
	if( foundASafetyOrParsingError )
		return;
	externalPredicateName = name;
}

void InMemoryInputBuilder::onExistentialAtom() {
	if(foundASafetyOrParsingError) return;
}

void InMemoryInputBuilder::onPredicateName(char* name) {
	if(foundASafetyOrParsingError) return;
	string name_predicate(name);
	Predicate *predicate = new Predicate(name_predicate,terms_parsered.size());
	predicate->setHiddenForPrinting(hiddenNewPredicate);
	predicateTable->getInstance()->insertPredicate(predicate);
	instancesTable->addPredicateExt(predicate);
	currentAtom = new ClassicalLiteral(predicate,terms_parsered,false,false);
	terms_parsered.clear();
}

void InMemoryInputBuilder::onExistentialVariable(char* var) {
	if(foundASafetyOrParsingError) return;
}

void InMemoryInputBuilder::onEqualOperator() {
	if(foundASafetyOrParsingError) return;
	currentBinop = Binop::EQUAL;
}

void InMemoryInputBuilder::onUnequalOperator() {
	if(foundASafetyOrParsingError) return;
	currentBinop = Binop::UNEQUAL;
}

void InMemoryInputBuilder::onLessOperator() {
	if(foundASafetyOrParsingError) return;
	currentBinop = Binop::LESS;
}

void InMemoryInputBuilder::onLessOrEqualOperator() {
	if(foundASafetyOrParsingError) return;
	currentBinop = Binop::LESS_OR_EQ;
}

void InMemoryInputBuilder::onGreaterOperator() {
	if(foundASafetyOrParsingError) return;
	currentBinop = Binop::GREATER;
}

void InMemoryInputBuilder::onGreaterOrEqualOperator() {
	if(foundASafetyOrParsingError) return;
	currentBinop = Binop::GREATER_OR_EQ;
}

bool isNumeric(const char* pszInput, int nNumberBase )
{
    istringstream iss( pszInput );

    if ( nNumberBase == 10 )
    {
        double dTestSink;
        iss >> dTestSink;
    }
    else if ( nNumberBase == 8 || nNumberBase == 16 )
    {
        int nTestSink;
        iss >> ( ( nNumberBase == 8 ) ? oct : hex ) >> nTestSink;
    }
    else
        return false;

    // Was any input successfully consumed/converted?
    if ( !iss )
        return false;

    // Was all the input successfully consumed/converted?
    return ( iss.rdbuf()->in_avail() == 0 );
}

void InMemoryInputBuilder::newTerm(char* value)
{
    if( value[0] >= 'A' && value[0] <='Z' ) // Variable
    {
    	string name(value);
		Term *term=new VariableTerm(false,name);
		termTable->addTerm(term);
		terms_parsered.push_back(term);
    }
    else if( (value[0] == '\"' && value[strlen(value)-1] == '\"') ||
            (value[0] >= 'a' && value[0] <='z') )   // String constant
    {
    	string name(value);
    	Term *term=new StringConstantTerm(false,name);
		termTable->addTerm(term);
		terms_parsered.push_back(term);
    }
    else //if( isNumeric( value, 10 ) ) // Numeric constant
    {
    	int val = atoi(value);
    	onTerm(val);
    }
}

void InMemoryInputBuilder::onTerm(char* value) {
	if(foundASafetyOrParsingError) return;
	newTerm(value);
}

void InMemoryInputBuilder::onTerm(int value) {
	if(foundASafetyOrParsingError) return;
	Term *term=new NumericConstantTerm(false,value);
	termTable->addTerm(term);
	terms_parsered.push_back(term);
}

void InMemoryInputBuilder::onUnknownVariable() {
	if(foundASafetyOrParsingError) return;
	string name("_");
	Term *term=new VariableTerm(false,name);
	termTable->addTerm(term);
	terms_parsered.push_back(term);
}

void InMemoryInputBuilder::onFunction(char* functionSymbol, int nTerms) {
	if(foundASafetyOrParsingError) return;
	string name(functionSymbol);
	vector<Term*> termsInFunction(nTerms);

	for(int i=0;i<nTerms;i++){
		termsInFunction[nTerms-i-1]=terms_parsered.back();
		terms_parsered.pop_back();
	}

	Term *term=new FunctionTerm(name,false,termsInFunction);
	termTable->addTerm(term);
	terms_parsered.push_back(term);
}

void InMemoryInputBuilder::onHeadTailList(){
	if( foundASafetyOrParsingError )
		return;
	onHeadTail = true;
}

void InMemoryInputBuilder::onListTerm( int nTerms ){
	if( foundASafetyOrParsingError )
		return;
	unsigned start = terms_parsered.size() - nTerms;

	vector< Term* > termsInList( terms_parsered.begin() + start, terms_parsered.end() );

	terms_parsered.resize( terms_parsered.size() - nTerms );

	Term *term = new ListTerm( false, onHeadTail, termsInList );
	termTable->addTerm( term );

	if( term->isGround() )
		term = term->toList();

	terms_parsered.push_back( term );

	onHeadTail = false;
}

void InMemoryInputBuilder::onTermDash(){
	if( foundASafetyOrParsingError )
		return;
	Term *oldTerm = terms_parsered.back();
	if( oldTerm->isRange() ){
		oldTerm->setNegative( true );
	} else{
		Term *newTerm = nullptr;
		if(oldTerm->getType()==NUMERIC_CONSTANT)
			newTerm=new NumericConstantTerm(false,oldTerm->getConstantValue()*(-1));
		else if(oldTerm->getType()==ARITH){
			ArithTerm *arithCasted=dynamic_cast<ArithTerm*>(oldTerm);
			newTerm = new ArithTerm(*arithCasted);
			newTerm-> addTerm(termTable->term_minus_one);
			newTerm->addOperator(Operator::TIMES);
		}else{
			newTerm = new ArithTerm(false,oldTerm,termTable->term_minus_one,TIMES);
		}
		TermTable::getInstance()->addTerm(newTerm);
		terms_parsered.pop_back();
		terms_parsered.push_back(newTerm);
	}
}

void InMemoryInputBuilder::onTermParams() {
	if(foundASafetyOrParsingError) return;

}

void InMemoryInputBuilder::onTermRange(char* lowerBound, char* upperBound) {
	if(foundASafetyOrParsingError) return;
	foundARangeAtomInCurrentRule=true;
	if(isNumeric(lowerBound,10) && isNumeric(upperBound,10)){
		Term* rangeTerm=new RangeTerm(atoi(lowerBound),atoi(upperBound));
		terms_parsered.push_back(rangeTerm);
	}
}

void InMemoryInputBuilder::onArithmeticOperation(char arithOperator) {
	if(foundASafetyOrParsingError) return;
	auto last=terms_parsered[terms_parsered.size()-1];
	auto secondLast=terms_parsered[terms_parsered.size()-2];
	Term *arithTerm=nullptr;
	if(secondLast->getType()!=TermType::ARITH && last->getType()!=TermType::ARITH){
		//First occurrence of arithmetic
		arithTerm=new ArithTerm;
		arithTerm->addTerm(secondLast);
		arithTerm->addTerm(last);

		arithTerm->addOperator(ArithTerm::getOperatorName(arithOperator));
	}else{
		if(secondLast->getType()==TermType::ARITH){
			ArithTerm * arithCasted=dynamic_cast<ArithTerm*>(secondLast);
			arithTerm = new ArithTerm(*arithCasted);
		}else{
			arithTerm=new ArithTerm;
			arithTerm->addTerm(secondLast);
		}
		ArithTerm::addArithTerm(arithTerm,last,ArithTerm::getOperatorName(arithOperator));
	}
	termTable->addTerm(arithTerm);

	terms_parsered.pop_back();
	terms_parsered.pop_back();
	terms_parsered.push_back(arithTerm);

}

void InMemoryInputBuilder::onWeightAtLevels(int nWeight, int nLevel,
		int nTerm) {
	if(foundASafetyOrParsingError) return;
	//TODO ERROR IN PARSER
	if(nWeight==0){
		nTerm--;
		nWeight=1;
	}
	weight=terms_parsered[0];
	if(nLevel>0){
		level=terms_parsered[nWeight];
	}
	if(nLevel+nWeight>0)
		terms_parsered.erase(terms_parsered.begin(),terms_parsered.begin()+nLevel+nWeight);
}

void InMemoryInputBuilder::onChoiceLowerGuard() {
	if(foundASafetyOrParsingError) return;
	if(currentChoice==nullptr)
		currentChoice = new Choice;
	Term* firstGuard=terms_parsered.back();
	currentChoice->setFirstGuard(firstGuard);
	currentChoice->setFirstBinop(currentBinop);
	if(firstGuard->contain(TermType::ANONYMOUS)){
		currentRuleIsUnsafe=true;
//		currentChoice->print();cout<<" ";
	}
	terms_parsered.pop_back();
}

void InMemoryInputBuilder::onChoiceUpperGuard() {
	if(foundASafetyOrParsingError) return;
	if(currentChoice==nullptr)
		currentChoice = new Choice;
	Term* secondGuard=terms_parsered.back();
	currentChoice->setSecondGuard(secondGuard);
	currentChoice->setSecondBinop(currentBinop);
	if(secondGuard->contain(TermType::ANONYMOUS)){
		currentRuleIsUnsafe=true;
//		currentChoice->print();cout<<" ";
//		safetyError(false,"ATOM IS UNSAFE");
	}
	terms_parsered.pop_back();
}

void InMemoryInputBuilder::onChoiceElementAtom() {
	if(foundASafetyOrParsingError) return;
	if(currentChoice==nullptr)
		currentChoice = new Choice;
	currentChoiceElement->add(currentAtom);
	currentAtom=nullptr;
}

void InMemoryInputBuilder::onChoiceElementLiteral() {
	if(foundASafetyOrParsingError) return;
	currentChoiceElement->add(currentAtom);
	currentAtom=nullptr;
}

void InMemoryInputBuilder::onChoiceElement() {
	if(foundASafetyOrParsingError) return;
	currentChoice->addChoiceElement(currentChoiceElement);
	currentChoiceElement=new ChoiceElement;
}

void InMemoryInputBuilder::onChoiceAtom() {
	if(foundASafetyOrParsingError) return;
	statementDependency->choiceFound();
	if(currentChoice->getFirstBinop()==NONE_OP && currentChoice->getSecondBinop()==NONE_OP){
		currentChoice->setSecondBinop(GREATER_OR_EQ);
		currentChoice->setSecondGuard(termTable->term_zero);
	}
	currentAtom=currentChoice;
	currentRule->addInHead(currentAtom);
	currentChoice=nullptr;
	currentAtom=nullptr;
}

void InMemoryInputBuilder::onBuiltinAtom() {
	if(foundASafetyOrParsingError) return;
	currentAtom = new BuiltInAtom(currentBinop,false,terms_parsered);
	if(currentAtom->containsAnonymous()){
		currentRuleIsUnsafe=true;
//		currentAtom->print();cout<<" ";
//		safetyError(false,"ATOM IS UNSAFE");
	}
}

void InMemoryInputBuilder::onAggregateLowerGuard() {
	if(foundASafetyOrParsingError) return;
	if(currentAggregate==nullptr)
		currentAggregate = new AggregateAtom;
	Term* firstGuard=terms_parsered.back();
	currentAggregate->setFirstGuard(firstGuard);
	currentAggregate->setFirstBinop(currentBinop);
	if(firstGuard->contain(TermType::ANONYMOUS)){
		currentRuleIsUnsafe=true;
//		currentAggregate->print();cout<<" ";
//		safetyError(false,"ATOM IS UNSAFE");
	}
	terms_parsered.pop_back();
}

void InMemoryInputBuilder::onAggregateUpperGuard() {
	if(foundASafetyOrParsingError) return;
	if(currentAggregate==nullptr)
		currentAggregate = new AggregateAtom;
	Term* secondGuard=terms_parsered.back();
	currentAggregate->setSecondGuard(secondGuard);
	currentAggregate->setSecondBinop(currentBinop);
	if(secondGuard->contain(TermType::ANONYMOUS)){
		currentRuleIsUnsafe=true;
//		currentAggregate->print();cout<<" ";
//		safetyError(false,"ATOM IS UNSAFE");
	}
	terms_parsered.pop_back();
}

void InMemoryInputBuilder::onAggregateFunction(char* functionSymbol) {
	if(foundASafetyOrParsingError) return;
	currentRule->setMustBeRewritedForAggregates(true);
	if(currentAggregate==nullptr)
		currentAggregate = new AggregateAtom;
	if(strcmp(functionSymbol,"#count")==0){
		currentAggregate->setAggregateFunction(AggregateFunction::COUNT);
	}else if(strcmp(functionSymbol,"#sum")==0){
		currentAggregate->setAggregateFunction(AggregateFunction::SUM);
	}else if(strcmp(functionSymbol,"#min")==0){
		currentAggregate->setAggregateFunction(AggregateFunction::MIN);
	}else if(strcmp(functionSymbol,"#max")==0){
		currentAggregate->setAggregateFunction(AggregateFunction::MAX);
	}
}

void InMemoryInputBuilder::onAggregateGroundTerm(char* value, bool dash) {
	if(foundASafetyOrParsingError) return;
	newTerm(value);
	currentAggregateElement->addTerm(terms_parsered.back());
	terms_parsered.pop_back();
}

void InMemoryInputBuilder::onAggregateVariableTerm(char* value) {
	if(foundASafetyOrParsingError) return;
	string value_string(value);
	Term *term=new VariableTerm(false,value_string);
	termTable->addTerm(term);

	currentAggregateElement->addTerm(term);
}

void InMemoryInputBuilder::onAggregateUnknownVariable() {
	if(foundASafetyOrParsingError) return;
	currentRuleIsUnsafe=true;
//	currentAggregate->print();cout<<" ";
//	safetyError(false,"ATOM IS UNSAFE");
}

void InMemoryInputBuilder::onAggregateNafLiteral() {
	if(foundASafetyOrParsingError) return;
	currentAggregateElement->addNafLiterals(currentAtom);
	currentAtom=nullptr;
}

void InMemoryInputBuilder::onAggregateElement() {
	if(foundASafetyOrParsingError) return;
	currentAggregate->addAggregateElement(currentAggregateElement);
	if(!currentAggregateElement->areAggregationTermsSafe())
	{
		currentRuleIsUnsafe=true;
//		currentAggregate->print();cout<<" ";
//		safetyError(false,"ATOM IS UNSAFE");
	}
	currentAggregateElement=new AggregateElement;
}

void InMemoryInputBuilder::onAggregateFunctionalTerm( char* functionSymbol, int nTerms) {
	if(foundASafetyOrParsingError) return;
	string name(functionSymbol);
	vector<Term*> termsInFunction(nTerms);

	for(int i=0;i<nTerms;i++){
		if(terms_parsered.back()->getType()==ANONYMOUS)
			currentRuleIsUnsafe=true;
		termsInFunction[nTerms-i-1]=terms_parsered.back();
		terms_parsered.pop_back();
	}

	Term *term=new FunctionTerm(name,false,termsInFunction);
	termTable->addTerm(term);
	currentAggregateElement->addTerm(term);
}

void InMemoryInputBuilder::onAggregate(bool naf) {
	if(foundASafetyOrParsingError) return;
	statementDependency->aggregateFound();
	currentAggregate->setNegative(naf);
	currentAtom = currentAggregate;
	currentAtom->changeInStandardFormat();
	currentAggregate = nullptr;
}

void InMemoryInputBuilder::rewriteAggregate(Rule* rule,bool clear) {
	//Sort the rule and check for safety
	if(currentRuleIsUnsafe){
		safetyError(false,rule);
		statementDependency->assignRuleIndex(rule);
		statementDependency->addRuleMapping(rule);
		return;
	}

	if(rule->getSizeHead()>1)
		statementDependency->disjunctionFound();
        
	statementDependency->assignRuleIndex(rule);
	manageBeforeRuleAnnotations(rule);
        
	OrderRule orderRule(rule);
	bool isSafe = orderRule.order();
	safetyError(isSafe,rule);

	projectionRewrite(rule);

	for(auto& v:currentRuleAtomsBefore){
		for(unsigned i=0;i<v.size();++i){
			Atom*& atom=v[i];
			if(atom->isAggregateAtom()){
				vector<unsigned> positions;
				GroundingPreferences::checkIfAtomIsPresentInRule(rule,atom,positions);
				if(positions.size()>0){
					atom->deleteAtoms();
					delete atom;
					atom=rule->getAtomInBody(positions[0]);
				}
				else
					annotationsAtomsToDelete.push_back(atom);
			}
		}
	}
	for(auto& v:currentRuleAtomsAfter){
		for(unsigned i=0;i<v.size();++i){
			Atom*& atom=v[i];
			if(atom->isAggregateAtom()){
				vector<unsigned> positions;
				GroundingPreferences::checkIfAtomIsPresentInRule(rule,atom,positions);
				if(positions.size()>0){
					atom->deleteAtoms();
					delete atom;
					atom=rule->getAtomInBody(positions[0]);
				}
				else
					annotationsAtomsToDelete.push_back(atom);
			}
		}
	}

	//Translate the rule
	vector<Rule*> rules;
	inputRewriter->translateAggregate(rule, rules, &orderRule);

	for (auto r : rules) {
		statementDependency->assignRuleIndex(r);
                manageBeforeRuleAnnotations(r);
		OrderRule orderR(r);
		isSafe = orderR.order();
		if (!isSafe)
			safetyError(isSafe,r);

		projectionRewrite(r);
		statementDependency->addRuleMapping(r);
		manageRuleAnnotations(r);
	}

	statementDependency->addRuleMapping(rule);
	manageRuleAnnotations(rule);
	if(clear)
		clearAnnotationsSetting();
}

void InMemoryInputBuilder::rewriteChoice(Rule* rule) {

	OrderRule orderRule(rule);
	orderRule.order();

	vector<Rule*> rules;
	inputRewriter->translateChoice(rule, rules,&orderRule);
	for (auto r : rules){
		if(r->isMustBeRewritedForAggregates()){
			rewriteAggregate(r,false);
		}else{
			manageSimpleRule(r,false);
		}
	}
	clearAnnotationsSetting();
}

void InMemoryInputBuilder::onEnd() {
//	OutputBuilder::getInstance()->onEndParsing(); TODO Remove
	DirectivesHolder::getInstance()->applyShows();
}

void InMemoryInputBuilder::projectionRewrite(Rule* rule){
        //	if(!projectAtom)return;
	vector<Rule*> rules;
	inputRewriter->projectAtoms(rule,rules);
	for(auto r:rules){
		statementDependency->assignRuleIndex(r);
		statementDependency->addRuleMapping(r);
		r->setUnsolvedPredicates();
	}
}


void InMemoryInputBuilder::manageSimpleRule(Rule* rule,bool clear) {
	if(currentRuleIsUnsafe){
		safetyError(false,rule);
		statementDependency->assignRuleIndex(rule);
		statementDependency->addRuleMapping(rule);
		return;
	}

	if(rule->getSizeHead()>1)
		statementDependency->disjunctionFound();

	//First push in the atom assignment builtin that not contain arithmetic terms
	inputRewriter->pushBuiltin(rule);

	statementDependency->assignRuleIndex(rule);
        manageBeforeRuleAnnotations(rule);
	if(!rule->hasOnlyPositiveClassicalLitsAndNoArithTerms()){
		OrderRule orderRule(rule);
		bool isSafe = orderRule.order();
		safetyError(isSafe,rule);
	}
	else{
		set_term headTerms;
		rule->getVariablesInHead(headTerms);
		if(!headTerms.empty()){
			set_term bodyTerms;
			rule->getVariablesInBody(bodyTerms);
			bool isSafe=Utils::isContained(headTerms,bodyTerms);
			safetyError(isSafe,rule);
		}
	}

	//Do projection rewriting for each rule parsered
	projectionRewrite(rule);

	statementDependency->addRuleMapping(rule);
	rule->setUnsolvedPredicates();
	manageRuleAnnotations(rule);
	if(clear)
		clearAnnotationsSetting();
}

void InMemoryInputBuilder::addRule(Rule* rule) {
	set_term global;
	for(auto atom:rule->getBody())
		if(atom->isClassicalLiteral() && !atom->isNegative())
			atom->getVariable(global);
	for(auto atom:rule->getBody()){
		set_term localVariables,guardVariables,allInternalVariables;
		if(atom->isAggregateAtom()){
			allInternalVariables=atom->getVariable(false);
			guardVariables=atom->getGuardVariable();
			atom->getVariablesInAggregationTerms(localVariables);
		}
		if(!Utils::isDisjoint(localVariables,guardVariables)
			|| (!Utils::isDisjoint(allInternalVariables,guardVariables) && Utils::isDisjoint(guardVariables,global))){
			if(localVariables.size()==1)
				WarningMessage::warningDuringParsing("The local variable "+(*localVariables.begin())->toString()+" appears also as global variable.");
			else
				WarningMessage::warningDuringParsing("There are local variables in aggregate literals appearing also as global variables.");
			foundASafetyOrParsingError=true;
		}
		if(foundASafetyOrParsingError){
			safetyError(false,rule);
			return;
		}
	}

	if(rule->isChoiceRule()){
		rewriteChoice(rule);
		statementDependency->disjunctionFound();
	}else if(rule->isMustBeRewritedForAggregates())
		rewriteAggregate(rule);
	else
		manageSimpleRule(rule);
}

Rule* InMemoryInputBuilder::createRule(vector<Atom*>* head, vector<Atom*>* body) {
	Rule* rule=new Rule;
	if(head!=0){
		for(auto a:*head)
			rule->addInHead(a->clone());
	}
	if(body!=0){
		for(auto a:*body)
			rule->addInBody(a->clone());
	}
	addRule(rule);
	return rule;
}

void InMemoryInputBuilder::createFact(Atom* fact) {
	fact->setFact(true);
	Predicate* predicate = fact->getPredicate();
//	if (!(instancesTable->getPredicateExt(predicate)->addAtom(FACT, fact)))
	IndexingStructure* atomSearcher=nullptr;
	instancesTable->getPredicateExt(predicate)->getAtomSearcher(FACT);
	if(Options::globalOptions()->getCheckFactDuplicate())
		atomSearcher=instancesTable->getPredicateExt(predicate)->addAtomSearcher(FACT,HASHSET,nullptr);

	if(atomSearcher==nullptr || atomSearcher->find(fact)==nullptr){
		instancesTable->getPredicateExt(predicate)->addAtom(fact,FACT);
	}else
		delete fact;
}

void InMemoryInputBuilder::expandTermsRecursive(Atom* atom, vector<Term*>& currentTerms, vector<Atom*>& atomExpanded,unsigned currentPosition){
	if(currentPosition<atom->getTermsSize()-1){
		currentPosition++;
		expandTerms(atom,currentTerms,atomExpanded,currentPosition);
		currentTerms.pop_back();
	}
	else{
		ClassicalLiteral* newAtom = new ClassicalLiteral(atom->getPredicate(),atom->isHasMinus(),atom->isNegative());
		newAtom->setTerms(currentTerms);
		atomExpanded.push_back(newAtom);
		currentTerms.pop_back();
	}
}

void InMemoryInputBuilder::expandTerms(Atom* atom, vector<Term*>& currentTerms, vector<Atom*>& atomExpanded, unsigned currentPosition){
	Term* t=atom->getTerm(currentPosition);
	if(!t->isRange()){
		currentTerms.push_back(t);
		expandTermsRecursive(atom,currentTerms,atomExpanded,currentPosition);
		return;
	}
	RangeTerm* rt=dynamic_cast<RangeTerm*>(t);
	for(int i=rt->getLowerBound();i<=rt->getUpperBound();i++){
		Term* t = new NumericConstantTerm(rt->isNegative(),i);
		termTable->addTerm(t);
		currentTerms.push_back(t);
		expandTermsRecursive(atom,currentTerms,atomExpanded,currentPosition);
	}
}

void InMemoryInputBuilder::expandRangeAtom(Atom* atom, vector<Atom*>& atomExpanded) {
	vector<Term*> currentTerms;
	expandTerms(atom, currentTerms, atomExpanded, 0);
	for (auto t : atom->getTerms())
		if(t->isRange())
			delete t;
	delete atom;
}

void InMemoryInputBuilder::expandAtoms(const vector<vector<Atom*>>& atoms, vector<Atom*>& currentAtoms, vector<vector<Atom*>>& atomsExpanded, unsigned currentPosition){
	for(auto it=atoms[currentPosition].begin();it!=atoms[currentPosition].end();it++){
		Atom* atom = (*it)->clone();
		currentAtoms.push_back(atom);
		if(currentPosition<atoms.size()-1){
			currentPosition++;
			expandAtoms(atoms,currentAtoms,atomsExpanded,currentPosition);
			currentPosition--;
		}
		else
			atomsExpanded.push_back(currentAtoms);
		currentAtoms.pop_back();
	}
}

void InMemoryInputBuilder::expandRulePart(vector<Atom*>::const_iterator start, vector<Atom*>::const_iterator end, vector<vector<Atom*> >& atomsExpanded) {
	vector<vector<Atom*>> atoms;
	for (auto it = start; it != end; it++) {
		vector<Atom*> atomExpanded;
		if((*it)->containsRangeTerms())
			expandRangeAtom(*it,atomExpanded);
		else
			atomExpanded.push_back(*it);
		atoms.push_back(atomExpanded);
	}
	vector<Atom*> currentAtoms;
	expandAtoms(atoms,currentAtoms,atomsExpanded,0);
	for(auto vectorAtom:atoms)
		for(auto atom: vectorAtom)
			delete atom;
}

void InMemoryInputBuilder::onAnnotationRuleOrdering(char* annotation) {
	if(isNumeric(annotation,10))
		currentRuleOrdering = atoi(annotation);
	//FIXME check that the number is a val ordering type
}

void InMemoryInputBuilder::onAnnotationRuleProjection(char* annotation) {
	if(isNumeric(annotation,10))
		currentProjection = atoi(annotation);
	//FIXME check that the number is a val ordering type
}


void InMemoryInputBuilder::onAnnotationRuleRewArith() {
	currentRewriteArith = true;
}


void InMemoryInputBuilder::onAnnotationRuleAtomIndexedArgument(char* annotation) {
	int argument=0;
	if(!isNumeric(annotation,10)){
		//FIXME warning
		return;
	}
	argument=atoi(annotation);
	if(argument>=0 && unsigned(argument)<currentRuleAtomsIndexed.back()->getPredicate()->getArity())
		currentRuleAtomsIndexedArguments.back().push_back(argument);
	else{
		WarningMessage::warningDuringParsing("The arguments specified for the atom "+currentRuleAtomsIndexed.back()->toString()+" are not valid in the following rule:");
		currentRule->print(cerr);
	}
}

void InMemoryInputBuilder::onAnnotationRuleAtomIndexedLiteral(bool naf) {
	currentAtom->setNegative(naf);
	currentRuleAtomsIndexed.push_back(currentAtom);
	currentRuleAtomsIndexedArguments.push_back(vector<unsigned>());
	currentAtom= nullptr;
}

void InMemoryInputBuilder::onAnnotationRulePartialOrderingBefore(bool naf) {
	currentAtom->setNegative(naf);
	currentRuleAtomsBefore.back().push_back(currentAtom);
}

void InMemoryInputBuilder::onAnnotationRulePartialOrderingAfter(bool naf) {
	currentAtom->setNegative(naf);
	currentRuleAtomsAfter.back().push_back(currentAtom);
}

void InMemoryInputBuilder::onAnnotationAggregateRulePartialOrderingAfter(bool naf) {
	currentAggregate->setNegative(naf);
	currentRuleAtomsAfter.back().push_back(currentAggregate);
	currentAggregate = nullptr;
}

void InMemoryInputBuilder::onAnnotationAggregateRulePartialOrderingBefore(bool naf) {
	currentAggregate->setNegative(naf);
	currentRuleAtomsBefore.back().push_back(currentAggregate);
	currentAggregate = nullptr;
}

void InMemoryInputBuilder::onAnnotationGlobalOrdering(char* annotation) {
	if(isNumeric(annotation,10)){
		int globalOrdering = atoi(annotation);
		if(!GroundingPreferences::getGroundingPreferences()->addGlobalOrderingType(globalOrdering))
			WarningMessage::warningDuringParsing("The ordering type "+to_string(globalOrdering)+" is not valid. It will be ignored.");
	}
}

void InMemoryInputBuilder::onAnnotationGlobalAtomIndexedArgument(char* annotation) {
	int argument=0;
	if(!isNumeric(annotation,10)){
		//FIXME warning
		return;
	}
	argument=atoi(annotation);
	if(argument>=0 && unsigned(argument)<globalAtomsIndexed.back()->getPredicate()->getArity()){
		globalAtomsIndexedArguments.back().push_back(argument);
		GroundingPreferences::getGroundingPreferences()->addGlobalAtomIndexingSetting(globalAtomsIndexed.back(),globalAtomsIndexedArguments.back());
	}
	//else	FIXME WARNING
}

void InMemoryInputBuilder::onAnnotationGlobalAtomIndexedLiteral(bool naf) {
	currentAtom->setNegative(naf);
	globalAtomsIndexed.push_back(currentAtom);
	globalAtomsIndexedArguments.push_back(vector<unsigned>());
	currentAtom= nullptr;
}

void InMemoryInputBuilder::onAnnotationGlobalPartialOrderingBefore(bool naf) {
	currentAtom->setNegative(naf);
	GroundingPreferences::getGroundingPreferences()->addGlobalPartialOrderAtomStart(currentAtom);
	currentAtom= nullptr;
}

void InMemoryInputBuilder::onAnnotationGlobalPartialOrderingAfter(bool naf) {
	currentAtom->setNegative(naf);
	GroundingPreferences::getGroundingPreferences()->addGlobalPartialOrderAtomEnd(currentAtom);
	currentAtom= nullptr;
}

void InMemoryInputBuilder::onAnnotationAggregateGlobalPartialOrderingAfter(bool naf) {
	currentAggregate->setNegative(naf);
	GroundingPreferences::getGroundingPreferences()->addGlobalPartialOrderAtomEnd(currentAggregate);
	currentAggregate = nullptr;
}

void InMemoryInputBuilder::onAnnotationAggregateGlobalPartialOrderingBefore(bool naf) {
	currentAggregate->setNegative(naf);
	GroundingPreferences::getGroundingPreferences()->addGlobalPartialOrderAtomStart(currentAggregate);
	currentAggregate = nullptr;
}

void InMemoryInputBuilder::onAnnotationGlobalWaspAggregateElement(){
	currentAggregateElement=new AggregateElement;
}

void InMemoryInputBuilder::onAnnotationGlobalWaspLiteral(){
	AggregateElement* age=new AggregateElement;
	age->addNafLiterals(currentAtom);
	currentAggregate->addAggregateElement(age);
	currentAtom=nullptr;
}

void InMemoryInputBuilder::onAnnotationGlobalWaspPyFile(char* f){
	if(foundASafetyOrParsingError) return;

	pythonFileWasp=f;
}

void InMemoryInputBuilder::onAnnotationGlobalWaspPropagatorBegin(){
	errorMessage="--> Error: propagators are not supported by such version.\n    For a DLV version supporting propagators, please contact us at:\n    dodaro@mat.unical.it, zangari@mat.unical.it";
	foundASafetyOrParsingError=true;

	if(currentAggregate==nullptr){
		pythonFileWasp="";
		currentAggregate = new AggregateAtom;
	}
}

void InMemoryInputBuilder::onAnnotationGlobalWaspPropagatorEnd(){
	if(foundASafetyOrParsingError) return;
	PropagatorsHeuristicsHolder::getInstance()->addWaspProp({currentAggregate,pythonFileWasp});
	currentAggregate = nullptr;
	pythonFileWasp="";
}

void InMemoryInputBuilder::onAnnotationGlobalWaspHeuristicsBegin(){
	errorMessage="--> Error: custom solving heuristics are not supported by such version.\n    For a DLV version supporting custom solving heuristics, please contact us at:\n    dodaro@mat.unical.it, zangari@mat.unical.it";
	foundASafetyOrParsingError=true;

	if(currentAggregate==nullptr){
		pythonFileWasp="";
		currentAggregate = new AggregateAtom;
	}
}

void InMemoryInputBuilder::onAnnotationGlobalWaspHeuristicsEnd(){
	if(foundASafetyOrParsingError) return;
	PropagatorsHeuristicsHolder::getInstance()->addWaspHeuristics({currentAggregate,pythonFileWasp});
	currentAggregate = nullptr;
	pythonFileWasp="";
}


void InMemoryInputBuilder::safetyError(bool condition, Rule* rule) {
	if(!condition){
		stringstream tmp;
		rule->print(tmp);
		errorMessage="--> Safety Error: "+tmp.str();
		foundASafetyOrParsingError=true;
	}
}

void InMemoryInputBuilder::extAtomError(Rule* rule, string message) {
	stringstream tmp;
	rule->print(tmp);
	errorMessage="--> External atom error: " + message;
	foundASafetyOrParsingError=true;
}

void InMemoryInputBuilder::onAnnotationRuleLookAhead() {
	currentLookAhead=true;
}

void InMemoryInputBuilder::onAnnotationRuleAlignSubstitutions() {
	currentAlignSubstitutions=true;
}

void InMemoryInputBuilder::onAnnotationRuleToDecompose(){
    currentRuleToDecompose=true;
}

void InMemoryInputBuilder::onAnnotationRuleToNotDecompose(){
    currentRuleToNotDecompose=true;
}

} /* namespace grounder */
} /* namespace DLV2 */

