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
 * InMemoryInputBuilder.h
 *
 *  Created on: Jan 19, 2015
 *      Author: davide
 */

#ifndef SRC_INPUT_INMEMORYINPUTBUILDER_H_
#define SRC_INPUT_INMEMORYINPUTBUILDER_H_

#include <list>

#include "InputBuilder.h"
#include "../grounder/atom/Atom.h"
#include "../grounder/ground/StatementDependency.h"
#include "../grounder/table/PredicateExtension.h"
#include "../grounder/term/ConstantTerm.h"
#include "../grounder/term/VariableTerm.h"
#include "../grounder/term/ArithTerm.h"
#include "../grounder/term/FunctionTerm.h"
#include "../grounder/term/RangeTerm.h"
#include "../grounder/table/TermTable.h"
#include "../grounder/table/PredicateTable.h"
#include "../grounder/statement/InputRewriter.h"
#include "../grounder/statement/Rule.h"
#include "../grounder/exception/SafetyExceptionGrounder.h"
#include "ExtAtomFactory.h"
#include "../util/Utils.h"

using namespace std;

namespace DLV2 {
namespace grounder {

class InMemoryInputBuilder: public InputBuilder {
public:
	InMemoryInputBuilder();
	virtual ~InMemoryInputBuilder();

	virtual void onDirective( char* directiveName, char* directiveValue );
	virtual void onRule();
	virtual void onConstraint();
	virtual void onWeakConstraint();
	virtual void onQuery();
	virtual void onHeadAtom();
	virtual void onHead();
	virtual void onBodyLiteral();
	virtual void onBody();
	virtual void onNafLiteral( bool naf = false );
	virtual void onAtom( bool isStrongNeg = false );
	virtual void onExistentialAtom();
	virtual void onPredicateName( char* name );
	virtual void onExistentialVariable( char* var );
	virtual void onEqualOperator();
	virtual void onUnequalOperator();
	virtual void onLessOperator();
	virtual void onLessOrEqualOperator();
	virtual void onGreaterOperator();
	virtual void onGreaterOrEqualOperator();
	virtual void onTerm( char* value );
	virtual void onTerm( int value );
	virtual void onUnknownVariable();
	virtual void onFunction( char* functionSymbol, int nTerms );
	virtual void onHeadTailList();
	virtual void onListTerm( int nTerms );
	virtual void onTermDash();
	virtual void onTermParams();
	virtual void onTermRange( char* lowerBound, char* upperBound );
	virtual void onArithmeticOperation( char arithOperator );
	virtual void onWeightAtLevels( int nWeight, int nLevel, int nTerm );
	virtual void onChoiceLowerGuard();
	virtual void onChoiceUpperGuard();
	virtual void onChoiceElementAtom();
	virtual void onChoiceElementLiteral();
	virtual void onChoiceElement();
	virtual void onChoiceAtom();
	virtual void onBuiltinAtom();
	virtual void onAggregateLowerGuard();
	virtual void onAggregateUpperGuard();
	virtual void onAggregateFunction( char* functionSymbol );
	virtual void onAggregateGroundTerm( char* value, bool dash = false );
	virtual void onAggregateVariableTerm( char* value );
	virtual void onAggregateUnknownVariable();
	virtual void onAggregateFunctionalTerm( char* value, int nTerms );
	virtual void onAggregateNafLiteral();
	virtual void onAggregateElement();
	virtual void onAggregate( bool naf = false );

	virtual void onAnnotationRuleOrdering(char* annotation);
	virtual void onAnnotationRuleProjection(char* annotation);
	virtual void onAnnotationRuleRewArith();
	virtual void onAnnotationRuleLookAhead();
	virtual void onAnnotationRuleAlignSubstitutions();
	virtual void onAnnotationRuleAtomIndexedArgument(char* annotation);
	virtual void onAnnotationRuleAtomIndexedLiteral(bool naf = false);
	virtual void onAnnotationPartialOrdering(bool global=false);
	virtual void onAnnotationRulePartialOrderingBefore(bool naf = false);
	virtual void onAnnotationRulePartialOrderingAfter(bool naf = false);
	virtual void onAnnotationAggregateRulePartialOrderingAfter(bool naf = false);
	virtual void onAnnotationAggregateRulePartialOrderingBefore(bool naf = false);

	virtual void onAnnotationGlobalOrdering( char* annotation );
	virtual void onAnnotationGlobalAtomIndexedArgument( char* annotation );
	virtual void onAnnotationGlobalAtomIndexedLiteral( bool naf = false );
	virtual void onAnnotationGlobalPartialOrderingBefore( bool naf = false );
	virtual void onAnnotationGlobalPartialOrderingAfter( bool naf = false );

	virtual void onAnnotationGlobalExternal();
	virtual void onAnnotationGlobalExternalPredicateName( char* name );
	virtual void onAnnotationGlobalExternalType( char* type );
	virtual void onAnnotationGlobalExternalTypeQC();
	virtual void onAnnotationGlobalExternalTypeC();
	virtual void onAnnotationGlobalExternalTypeU();
	virtual void onAnnotationGlobalExternalTypeUT();
	virtual void onAnnotationGlobalExternalTypeUR();
	virtual void onAnnotationGlobalExternalTypeT();
	virtual void onAnnotationGlobalExternalTypeR();

	virtual void onAnnotationAggregateGlobalPartialOrderingAfter( bool naf = false );
	virtual void onAnnotationAggregateGlobalPartialOrderingBefore( bool naf = false );
        virtual void onAnnotationRuleToDecompose();
        virtual void onAnnotationRuleToNotDecompose();

	virtual void onAnnotationGlobalWaspAggregateElement();
	virtual void onAnnotationGlobalWaspLiteral();
	virtual void onAnnotationGlobalWaspPyFile(char* name);
	virtual void onAnnotationGlobalWaspPropagatorBegin();
	virtual void onAnnotationGlobalWaspPropagatorEnd();
	virtual void onAnnotationGlobalWaspHeuristicsBegin();
	virtual void onAnnotationGlobalWaspHeuristicsEnd();

	/************* useful for ExtAtom *******************/
	virtual void onExtAtom( bool naf = false );
	virtual void onSemicolon();
	virtual void onExternalPredicateName( char* name );
	/**************************************************/

	virtual void onEnd();

    	void newTerm(char*);

	static bool isFoundASafetyError() { return foundASafetyOrParsingError; }
	static const string& getSafetyErrorMessage() { return errorMessage; }

	const string SPARQL_PYTHON_PREDICATE_NAME;
	const string SPARQL_ENDPOINT_PYTHON_PREDICATE_NAME;

protected:

    TermTable *termTable;

    PredicateTable *predicateTable;

    StatementDependency* statementDependency;

    PredicateExtTable* instancesTable;

	InputRewriter* inputRewriter;

	vector<Term*> terms_parsered;

	bool foundARangeAtomInCurrentRule;

	Rule *currentRule;

	Atom *currentAtom;

	Binop currentBinop;

	Atom *currentAggregate;

	AggregateElement *currentAggregateElement;

	Atom* currentChoice;

	ChoiceElement* currentChoiceElement;

	Term* weight;

	Term* level;

	vector<TypeConv> extAtomTypeConv;
	string predicateExtAtomTypeConv;

	bool hiddenNewPredicate;

	int currentRuleOrdering;
	int currentProjection;
	bool currentRewriteArith;
	vector<Atom*> currentRuleAtomsIndexed;
	vector<vector<unsigned>> currentRuleAtomsIndexedArguments;
	vector<vector<Atom*>> currentRuleAtomsBefore;
	vector<vector<Atom*>> currentRuleAtomsAfter;
	vector<Atom*> annotationsAtomsToDelete;

	vector<Atom*> globalAtomsIndexed;
	vector<vector<unsigned>> globalAtomsIndexedArguments;

	Rule* createRule(vector<Atom*>* head, vector<Atom*>* body=0);
	void createFact(Atom* fact);

	void expandTermsRecursive(Atom* atom, vector<Term*>& currentTerms, vector<Atom*>& atomExpanded, unsigned currentPosition);
	void expandTerms(Atom* atom, vector<Term*>& currentTerms, vector<Atom*>& atomExpanded, unsigned currentPosition);

	void expandAtoms(const vector<vector<Atom*>>& atoms, vector<Atom*>& currentAtoms, vector<vector<Atom*>>& atomsExpanded, unsigned currentPosition);
	void expandRulePart(vector<Atom*>::const_iterator start, vector<Atom*>::const_iterator end, vector<vector<Atom*> >& atomsExpanded);
	void expandRangeAtom(Atom* fact, vector<Atom*>& atomExpanded);

	void rewriteAggregate(Rule* rule,bool clear=true);
	void rewriteChoice(Rule* rule);

	static string errorMessage;
	static bool currentRuleIsUnsafe;
	static bool foundASafetyOrParsingError;
	static void safetyError(bool condition, Rule* rule);
	static void extAtomError(Rule* rule, string msg);

	void clearAnnotationsSetting();
	void manageBeforeRuleAnnotations(Rule* currentRule);
	void manageRuleAnnotations(Rule* currentRule,bool rewritedRule=false);

	bool projectAtom;
	bool currentLookAhead;
	bool currentAlignSubstitutions;
	bool currentRuleToDecompose;
   	bool currentRuleToNotDecompose;

	Atom* makeSparqlExtAtom( const string& dataset, const string& query, unsigned arity, vector< TypeConv >& typeConvs );
	Atom* makeAtomInHead( string& predicateName, unsigned arity );

	/************* useful for ExtAtom *******************/
	unsigned extSeparator;
	string externalPredicateName;
	ExtAtomFactory* extAtomFactory;
	/**************************************************/

	bool onHeadTail;
	virtual void addRule(Rule* rule);
	virtual void manageSimpleRule(Rule* rule,bool clear=true);
	virtual void projectionRewrite(Rule* rule);

	string pythonFileWasp;

};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_INPUT_INMEMORYINPUTBUILDER_H_ */
