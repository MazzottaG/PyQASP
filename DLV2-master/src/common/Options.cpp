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

#include "Options.h"
#include "ErrorMessage.h"
#include "Help.h"
#include "Assert.h"
#include "Utility.h"

#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <regex>
#include <stdlib.h>
#include <string>
#include "../idlv/grounder/table/PredicateTable.h"
#include "../idlv/input/ExtAtomFactory.h"
#include "../wasp/preasp/Options.h"
#include "../wasp/util/WaspConstants.h"

using namespace std;

namespace DLV2
{
// Options with a short alias.
#define OPTIONID_help 'h'
#define OPTIONID_models 'n'

// "Only-long" options with their code.

/* INPUT OPTIONS */
#define OPTIONID_aspcore2strict ( 'z' + 1001 )
#define OPTIONID_inmemory ( 'z' + 1002 )
#define OPTIONID_dlvdb ( 'z' + 1003 )
#define OPTIONID_testparser ( 'z' + 1004 )

/* OUTPUT OPTIONS */
#define OPTIONID_printprogram ( 'z' + 1011 )
#define OPTIONID_selector ( 'z' + 1012 )
#define OPTIONID_printdepgraph ( 'z' + 1013 )
#define OPTIONID_printstatistics ( 'z' + 1014 )

/* GENERIC OPTIONS */
#define OPTIONID_stdin ( 'z' + 10020 )

/* DB OPTIONS */
#define OPTIONID_datasource ('z' + 1030 )
#define OPTIONID_username ('z' + 1031 )
#define OPTIONID_password ('z' + 1032 )

/* GROUNDER OPTIONS */
#define OPTIONID_hashType ('z' + 1033)
#define OPTIONID_indexType ('z' + 1034)
#define OPTIONID_predIndexTerm ('z' + 1035)
#define OPTIONID_predIndexType ('z' + 1036)
#define OPTIONID_nofacts ('z' + 1037)
#define OPTIONID_printRewrite ('z' + 1038)
#define OPTIONID_rewritingType ('z' + 1039)
#define OPTIONID_outputFormat ('z' + 1040)
#define OPTIONID_textual ('z' + 1041)
#define OPTIONID_instantiationProcedure ('z' + 1042)
#define OPTIONID_positiveOrderingProcedure ('z' + 1043)
#define OPTIONID_enableDictionaryIntersection ('z' + 1044)
#define OPTIONID_ruleTime ('z' + 1045)
#define OPTIONID_checkFactDuplicate ('z' + 1046)
#define OPTIONID_rewriteArith ('z' + 1047)
#define OPTIONID_anonymousFilter ('z' + 1048)
#define OPTIONID_predicateToFilter ('z' + 1049)
#define OPTIONID_compactFacts ('z' + 1050)
#define OPTIONID_gstats ('z' + 1051)
#define OPTIONID_project ('z' + 1052)
#define OPTIONID_FB ('z' + 1053)
#define OPTIONID_FC ('z' + 1054)
#define OPTIONID_OMSM ('z' + 1055)
#define OPTIONID_EQ ('z' + 1056)
#define OPTIONID_VERSION ('z' + 1057)
#define OPTIONID_PYTHONFILE ('z' + 1058)
#define OPTIONID_DECOMP ('z' + 1059)
#define OPTIONID_DECOMP_H ('z' + 1060)
#define OPTIONID_DECOMP_S ('z' + 1061)
#define OPTIONID_DECOMP_FORMULA ('z' + 1062)
#define OPTIONID_DECOMP_THRESHOLD ('z' + 1063)
#define OPTIONID_DECOMP_ITERATIONS ('z' + 1064)
#define OPTIONID_DECOMP_NOT_IMPROVEMENT_LIMIT ('z' + 1065)
#define OPTIONID_DECOMP_NO_FITNESS ('z' + 1066)
#define OPTIONID_DECOMP_LIMIT_SIZE_BODY_FITNESS ('z' + 1067)
#define OPTIONID_enableLookAhead ('z' + 1068)
#define OPTIONID_NO_STRING_CONVERSION ('z'+1069)
#define OPTIONID_DEFAULT_EXTERNAL_ATOMS ('z'+1070)
#define OPTIONID_NO_EXTERNAL_WARNINGS ('z'+1071)

/* TRACE OPTIONS */
#define OPTIONID_trace_heuristic ( 'z' + 1 )
#define OPTIONID_trace_parser ( 'z' + 2 )
#define OPTIONID_trace_checker ( 'z' + 3 )
#define OPTIONID_trace_solving ( 'z' + 4 )
#define OPTIONID_trace_us ( 'z' + 5 )
#define OPTIONID_trace_learning ( 'z' + 6 )
#define OPTIONID_trace_enumeration ( 'z' + 7 )
#define OPTIONID_trace_satelite ( 'z' + 8 )
#define OPTIONID_trace_aggregates ( 'z' + 9 )
#define OPTIONID_trace_weakconstraints ( 'z' + 10 )
#define OPTIONID_trace_disjunction ( 'z' + 11 )
#define OPTIONID_trace_multiaggregates ( 'z' + 12 )
#define OPTIONID_trace_predmin ( 'z' + 13 )
#define OPTIONID_trace_simplifications ( 'z' + 14 )

/* OUTPUT OPTIONS */
#define OPTIONID_silent ( 'z' + 20 )
#define OPTIONID_competition_output ( 'z' + 21 )
#define OPTIONID_third_competition_output ( 'z' + 22 )
#define OPTIONID_lastModel ( 'z' + 26 )
#define OPTIONID_printbounds ( 'z' + 27 )
#define OPTIONID_printatomtable ( 'z' + 28 )
#define OPTIONID_idOutput ( 'z' + 29 )
#define OPTIONID_onlyOptimum ( 'z' + 30 )
#define OPTIONID_stats ( 'z' + 31 )

#define OPTIONID_minisatheuristic ( 'z' + 40 )
#define OPTIONID_initVariableIncrement ( 'z' + 41 )
#define OPTIONID_initVariableDecay ( 'z' + 42 )
#define OPTIONID_initStrategy ( 'z' + 43 )
#define OPTIONID_initValue ( 'z' + 44 )
#define OPTIONID_initSign ( 'z' + 45 )
#define OPTIONID_dynamicConfiguration ( 'z' + 46 )

/* RESTART OPTIONS */
#define OPTIONID_restarts ( 'z' + 50 )

/* HEURISTIC OPTIONS */
#define OPTIONID_heuristic_interpreter ( 'z' + 60 )
#define OPTIONID_heuristic_scriptname ( 'z' + 61 )
#define OPTIONID_heuristic_plugins ( 'z' + 62 )
#define OPTIONID_heuristic_setscriptdirectory ( 'z' + 63 )


/* GLUCOSE OPTIONS */
#define OPTIONID_glucose_sizeLBDQueue ( 'z' + 80 )
#define OPTIONID_glucose_sizeTrailQueue ( 'z' + 81 )
#define OPTIONID_glucose_K ( 'z' + 82 )
#define OPTIONID_glucose_R ( 'z' + 83 )
#define OPTIONID_glucose_nbclausesBeforeReduce ( 'z' + 84 )
#define OPTIONID_glucose_incReduceDB ( 'z' + 85 )
#define OPTIONID_glucose_specialIncReduceDB ( 'z' + 86 )
#define OPTIONID_glucose_lbLBDFrozenClause ( 'z' + 87 )
#define OPTIONID_glucose_lbLBDMinimizingClause ( 'z' + 88 )
#define OPTIONID_glucose_keepClauses ( 'z' + 89 )

/* INPUT OPTIONS */
#define OPTIONID_dimacs ( 'z' + 90 )

/* GENERIC OPTIONS */
#define OPTIONID_time_limit ( 'z' + 101 )
#define OPTIONID_max_cost ( 'z' + 102 )

#define OPTIONID_modelchecker_algorithm ( 'z' + 104 )
#define OPTIONID_shift_strategy ( 'z' + 120 )
//#define OPTIONID_shift_onedef ( 'z' + 121 )
#define OPTIONID_simplifications ( 'z' + 122 )
#define OPTIONID_enumeration ( 'z' + 123 )
#define OPTIONID_multiaggregates ( 'z' + 124 )
#define OPTIONID_pre ( 'z' + 125 )
#define OPTIONID_wf ( 'z' + 126 )
#define OPTIONID_preverbosity ( 'z' + 127 )

/* WEAK CONSTRAINTS OPTIONS */
#define OPTIONID_weakconstraintsalgorithm ( 'z' + 200 )
#define OPTIONID_kthreshold ( 'z' + 201 )

#define OPTIONID_disjcores ( 'z' + 215 )
#define OPTIONID_minimize ( 'z' + 216 )
#define OPTIONID_stratification ( 'z' + 217 )
#define OPTIONID_firstmodel ( 'z' + 218 )
#define OPTIONID_minimizationstrategy ( 'z' + 219 )
#define OPTIONID_minimizationbudget ( 'z' + 220 )

/* QUERY OPTIONS */
#define OPTIONID_queryalgorithm ( 'z' + 300 )
#define OPTIONID_queryverbosity ( 'z' + 301 )
#define OPTIONID_querychunksize ( 'z' + 302 )
#define OPTIONID_querychunkpercentage ( 'z' + 303 )
#define OPTIONID_querycorecache ( 'z' + 304 )

/* PREDMIN OPTIONS */
#define OPTIONID_predminimizationalgorithm ( 'z' + 350 )
#define OPTIONID_predminimizationpredicate ( 'z' + 351 )
#define OPTIONID_predminimizationchunkpercentage ( 'z' + 352 )
#define OPTIONID_paracoherentsemantics ( 'z' + 353 )

#define OPTIONID_mode ( 'z' + 354 )
};

using namespace std;
using namespace DLV2;


// Initialize singleton
Options* Options::instance = NULL;

#ifdef TRACE_ON
	TraceLevels Options::traceLevels;
#endif



Options*
Options::globalOptions()
{
    if( instance == NULL )
    {
        instance = new Options();
    }
    return instance;
}

void
Options::finalizeGlobalOptions()
{
    if( instance != NULL )
    {
        delete instance;
        instance = NULL;
    }
}


Options::Options():
        aspCore2Strict(false),
        printProgram(false),
        printDepGraph(false),
        printStatistics(false),
        inputPolicy(BUILDER_IN_MEMORY),
        outputPolicy(OUTPUT_ASPCORE2),
		namedpipe(0),
		hashType(STL_HASH),
		indexType(MAP),
		nofacts(false),
		printRewrittenProgram(false),
		rewritingType(NATIVE_CHOICE),
		outputFormat(OUTPUT_PREASP),
		instantiationProcedure(BACKJUMPING),
		positiveOrderingProcedure(DLV_ORDERING_COMPARISONS_ESTIMATION),
		enabledDictionaryIntersection(false),
		ruleTime(false),
		checkFactDuplicate(false),
		rewriteArith(false),
		disabledAnonymousFilter(false),
		compactFacts(false),
		printGroundStats(0),
		rewProject(3),
		optionFrontend(0),
		magicRewriting(true),
		evaluateQuery(false),
		enabledDecomposition(true),
		decompositionAlgorithm(0),
		decompositionSeed(0),
		decompositionFormula(DECOMPOSITION_FORMULA_COMBINED),
		decompositionThreshold(0.5),
		decompositionIterations(DECOMPOSITION_ITERATIONS),
		decompositionNotImprovementLimit(DECOMPOSITION_NOT_IMPROVEMENT_LIMIT),
		disabledDecompFitness(false),
		decompLimitSizeBodyFitness(10),
		lookAhead(false),
		defaultExternalAtoms(false),
		noStrConversion(false),
		enableExternalAtomsWarnings(false)

{
#ifdef PYTHON
	extAtomFactory=grounder::PythonExtAtomFactory::getInstance();
#else
	extAtomFactory = grounder::ExtAtomFactory::getInstance();
#endif
}

Options::Options(
    const Options& o):
        aspCore2Strict(o.aspCore2Strict),
        printProgram(o.printProgram),
        printDepGraph(o.printDepGraph),
        printStatistics(o.printStatistics),
        inputPolicy(o.inputPolicy),
        outputPolicy(o.outputPolicy),
		namedpipe(o.namedpipe),
		hashType(o.hashType),
		indexType(o.indexType),
		predicatesIndexTerm(o.predicatesIndexTerm),
		predicatesIndexType(o.predicatesIndexTerm),
		nofacts(o.nofacts),
		printRewrittenProgram(o.printRewrittenProgram),
		rewritingType(o.rewritingType),
		outputFormat(o.outputFormat),
		instantiationProcedure(o.instantiationProcedure),
		positiveOrderingProcedure(o.positiveOrderingProcedure),
		enabledDictionaryIntersection(o.enabledDictionaryIntersection),
		ruleTime(o.ruleTime),
		checkFactDuplicate(o.checkFactDuplicate),
		rewriteArith(o.rewriteArith),
		disabledAnonymousFilter(o.disabledAnonymousFilter),
		compactFacts(o.compactFacts),
		printGroundStats(o.printGroundStats),
		rewProject(o.rewProject),
		optionFrontend(o.optionFrontend),
		magicRewriting(o.magicRewriting),
		evaluateQuery(o.evaluateQuery),
		enabledDecomposition(o.enabledDecomposition),
		decompositionAlgorithm(o.decompositionAlgorithm),
		decompositionSeed(o.decompositionSeed),
		decompositionFormula(o.decompositionFormula),
		decompositionThreshold(o.decompositionThreshold),
		decompositionIterations(o.decompositionIterations),
		decompositionNotImprovementLimit(o.decompositionNotImprovementLimit),
		disabledDecompFitness(o.disabledDecompFitness),
		decompLimitSizeBodyFitness(o.decompLimitSizeBodyFitness),
		lookAhead(o.lookAhead),
		defaultExternalAtoms( o.defaultExternalAtoms ), 
		noStrConversion( o.noStrConversion ),
		enableExternalAtomsWarnings( o.enableExternalAtomsWarnings )
{

#ifdef PYTHON
	extAtomFactory=grounder::PythonExtAtomFactory::getInstance();
#else
	extAtomFactory = grounder::ExtAtomFactory::getInstance();
#endif
}

OUTPUT_POLICY Options::waspOutputPolicy = WASP_OUTPUT;

bool Options::printLastModelOnly = false;
bool Options::printBounds = false;
bool Options::printAtomTable = false;
bool Options::printOnlyOptimum = false;

bool Options::minisatPolicy = false;

unsigned int Options::interpreter = NO_INTERPRETER;
char* Options::heuristic_scriptname = NULL;

RESTARTS_POLICY Options::restartsPolicy = GLUCOSE_RESTARTS_POLICY;

unsigned int Options::restartsThreshold = 100;

unsigned int Options::simplificationsThreshold = 0;

unsigned int Options::timeLimit = 0;

unsigned int Options::maxModels = 1;

unsigned int Options::maxCost = MAXUNSIGNEDINT;

WEAK_CONSTRAINTS_ALG Options::weakConstraintsAlg = ONE;

unsigned int Options::kthreshold = 0;

bool Options::disjCoresPreprocessing = false;
bool Options::minimizeUnsatCore = false;
bool Options::stratification = true;
bool Options::computeFirstModel = false;
unsigned Options::budget = UINT_MAX;

unsigned int Options::queryAlgorithm = NO_QUERY;
unsigned int Options::queryVerbosity = 0;

bool Options::callPyFinalize = true;

unordered_map< string, WEAK_CONSTRAINTS_ALG > Options::stringToWeak;

vector< string > Options::pluginsFilenames;

string Options::scriptDirectory = "";

unsigned int Options::predMinimizationAlgorithm = NO_PREDMINIMIZATION;
vector< string > Options::predicatesToMinimize;

vector< string > Options::paracoherentSemantics;

bool Options::multiAggregates = false;
bool Options::queryCoreCache = false;

unsigned int Options::shiftStrategy = PREASP_COMPLETION_AUTO;

//bool Options::oneDefShift = false;

unordered_map< string, unsigned int > Options::stringToShift;

unordered_map< string, unsigned int > Options::stringToMinimization;

unordered_map< string, unsigned int > Options::stringToQueryAlgorithms;

unordered_map< string, unsigned int > Options::stringToInitMinisatHeuristic;

unordered_map< string, unsigned int > Options::stringToPredMinimization;

unordered_map< string, unsigned int > Options::stringToParacoherent;

unordered_map< string, RESTARTS_POLICY > Options::stringToRestarts;

bool Options::simplifications = true;

unsigned Options::silent = 0;

unsigned int Options::minimizationStrategy = MINIMIZATION_OFF;

unsigned int Options::minimizationBudget = UINT_MAX;

unsigned int Options::enumerationStrategy = ENUMERATION_BT;

unsigned int Options::chunkSize = UINT_MAX;

unsigned int Options::chunkPercentage = UINT_MAX;

unsigned int Options::modelcheckerAlgorithm = REDUCT_BASED;

unsigned int Options::minimizePredicateChunkPercentage = UINT_MAX;

double Options::sizeLBDQueue = 50;
double Options::sizeTrailQueue = 5000;
double Options::K = 0.8;
double Options::R = 1.4;

int Options::nbclausesBeforeReduce = 2000;
int Options::incReduceDB = 300;
int Options::specialIncReduceDB = 1000;
unsigned int Options::lbLBDFrozenClause = 30;

unsigned int Options::lbLBDMinimizingClause = 6;

unsigned int Options::lbdKeepClauses = 2;

double Options::initVariableIncrement = 1.0;
double Options::initVariableDecay = ( 1 / 0.95 );

bool Options::stats = false;

unsigned Options::statsVerbosity = 0;

unsigned int Options::initMinisatHeuristic = INIT_MINISAT_ALL_EQUALS;
unsigned int Options::initValue = 0;
unsigned int Options::initSign = INIT_SIGN_MINISAT_ALLFALSE;

unsigned int Options::pre = PREASP_PRINT_WASP;
unsigned int Options::preVerbosity = 0;

unsigned int Options::mode = MODE_DLV2;

bool Options::waspDynamicConfiguration = false;
float Options::waspDynamicConfigurationMin = 0.1;
float Options::waspDynamicConfigurationMax = 0.9;
unsigned int Options::waspDynamicConfigurationK = 10000;
string Options::waspDynamicConfigurationFile = "";
string Options::waspDynamicConfigurationStrategy = "keep";

void
Options::init(
    int argc,
    char* const* argv )
{
    // It will store the option code.
    int code;
    initMap();
    // A string listing valid short options letters.
    const char* const shortOptions = "hn:01";

    // An array describing valid long options.
    const struct option longOptions[] =
    {
        /* INPUT OPTIONS */
//        { "aspcore2strict", no_argument, NULL, OPTIONID_aspcore2strict },
//        { "inmemory", no_argument, NULL, OPTIONID_inmemory },
//        { "dlvdb", no_argument, NULL, OPTIONID_dlvdb },
//        { "test-parser", no_argument, NULL, OPTIONID_testparser },
//
//        /* OUTPUT OPTIONS */
//    	  { "silent", no_argument, NULL, OPTIONID_silent },
//        { "printprogram", no_argument, NULL, OPTIONID_printprogram },
//        { "selector", required_argument, NULL, OPTIONID_selector },
//        { "printdepgraph", no_argument, NULL, OPTIONID_printdepgraph },
//        { "stats", no_argument, NULL, OPTIONID_printstatistics },

        /* GENERIC OPTIONS */
        { "help", no_argument, NULL, OPTIONID_help },
        { "stdin", no_argument, NULL, OPTIONID_stdin },

        /* DB OPTIONS */
//        { "db-source", required_argument, NULL, OPTIONID_datasource },
//        { "db-user", required_argument, NULL, OPTIONID_username },
//        { "db-pwd", required_argument, NULL, OPTIONID_password },

		/* GROUNDER */
		{"no-facts",no_argument, NULL, OPTIONID_nofacts},
		{"output",required_argument, NULL, OPTIONID_outputFormat},
		{"print-rewriting",no_argument, NULL, OPTIONID_printRewrite},
//		{"hashtype",required_argument, NULL, OPTIONID_hashType },
//		{"index-type",required_argument,NULL,OPTIONID_indexType},
		{"indexing",required_argument, NULL, OPTIONID_predIndexTerm},
//		{"pred-index-type",required_argument, NULL, OPTIONID_predIndexType},
		{"choice-rewriting",required_argument, NULL, OPTIONID_rewritingType},
		{"t",no_argument, NULL, OPTIONID_textual},
		{"instantiate",required_argument, NULL, OPTIONID_instantiationProcedure},
		{"ordering",required_argument, NULL, OPTIONID_positiveOrderingProcedure},
		{"align-substitutions",no_argument, NULL, OPTIONID_enableDictionaryIntersection},
		{"look-ahead",no_argument, NULL, OPTIONID_enableLookAhead},
		{"time",no_argument, NULL, OPTIONID_ruleTime},
		{"check-edb-duplication",no_argument, NULL, OPTIONID_checkFactDuplicate},
		{"rewrite-arith-terms",no_argument, NULL, OPTIONID_rewriteArith},
		{"no-isolated-filter",no_argument, NULL, OPTIONID_anonymousFilter},
		{"filter",required_argument, NULL, OPTIONID_predicateToFilter},
//		{"compact-facts",required_argument, NULL, OPTIONID_compactFacts},
		{"gstats",optional_argument, NULL, OPTIONID_gstats},
		{"no-projection",optional_argument, NULL, OPTIONID_project},
		{"FC",no_argument, NULL, OPTIONID_FC},
		{"FB",no_argument, NULL, OPTIONID_FB},
		{"no-magic-sets",no_argument, NULL, OPTIONID_OMSM},
		{"query",no_argument, NULL, OPTIONID_EQ},
		{"version",no_argument, NULL, OPTIONID_VERSION},
        {"license",no_argument, NULL, OPTIONID_VERSION},
		{"python-file",required_argument, NULL, OPTIONID_PYTHONFILE},
		{"no-decomp",no_argument, NULL, OPTIONID_DECOMP},
		{"decomp-algorithm",required_argument, NULL, OPTIONID_DECOMP_H},
//		{"decomp-s",required_argument, NULL, OPTIONID_DECOMP_S},
		{"decomp",required_argument, NULL, OPTIONID_DECOMP_FORMULA},
		{"decomp-threshold",required_argument, NULL, OPTIONID_DECOMP_THRESHOLD},
		{"decomp-iterations",required_argument, NULL, OPTIONID_DECOMP_ITERATIONS},
		{"decomp-limit",required_argument, NULL, OPTIONID_DECOMP_NOT_IMPROVEMENT_LIMIT},
		{"decomp-no-fitness",no_argument, NULL, OPTIONID_DECOMP_NO_FITNESS},
		{ "no-string-conversion", no_argument, NULL, OPTIONID_NO_STRING_CONVERSION }, 
		{ "default-external-atoms", no_argument, NULL, OPTIONID_DEFAULT_EXTERNAL_ATOMS },
		{ "enable-external-warnings", no_argument, NULL, OPTIONID_NO_EXTERNAL_WARNINGS },

		
		// Required at end of array.

        /* TRACE OPTIONS */
        { "trace-parser", required_argument, NULL, OPTIONID_trace_parser },
        { "trace-solver", required_argument, NULL, OPTIONID_trace_solving },
        { "trace-modelchecker", required_argument, NULL, OPTIONID_trace_checker },
        { "trace-unfoundedset", required_argument, NULL, OPTIONID_trace_us },
        { "trace-heuristic", required_argument, NULL, OPTIONID_trace_heuristic },
        { "trace-learning", required_argument, NULL, OPTIONID_trace_learning },
        { "trace-enumeration", required_argument, NULL, OPTIONID_trace_enumeration },
        { "trace-satelite", required_argument, NULL, OPTIONID_trace_satelite },
        { "trace-waspaggregates", required_argument, NULL, OPTIONID_trace_aggregates },
        { "trace-optimization", required_argument, NULL, OPTIONID_trace_weakconstraints },
        { "trace-disjunction", required_argument, NULL, OPTIONID_trace_disjunction },
        { "trace-multiaggregates", required_argument, NULL, OPTIONID_trace_multiaggregates },
        { "trace-predmin", required_argument, NULL, OPTIONID_trace_predmin },
        { "trace-simplifications", required_argument, NULL, OPTIONID_trace_simplifications },

        /* OUTPUT OPTIONS */
        { "competition-output", no_argument, NULL, OPTIONID_competition_output },
        { "silent", optional_argument, NULL, OPTIONID_silent },
        { "third-competition-output", no_argument, NULL, OPTIONID_third_competition_output },
        { "printlatestmodel", no_argument, NULL, OPTIONID_lastModel },
        { "printonlyoptimum", no_argument, NULL, OPTIONID_onlyOptimum },
        { "printbounds", no_argument, NULL, OPTIONID_printbounds },
        { "printatomstable", no_argument, NULL, OPTIONID_printatomtable },
        { "id-output", no_argument, NULL, OPTIONID_idOutput },
        { "stats", optional_argument, NULL, OPTIONID_stats },

        /* MINISAT POLICY */
        { "minisat-policy", no_argument, NULL, OPTIONID_minisatheuristic },
        { "init-varinc", required_argument, NULL, OPTIONID_initVariableIncrement },
        { "init-vardecay", required_argument, NULL, OPTIONID_initVariableDecay },
        { "init-strategy", required_argument, NULL, OPTIONID_initStrategy },
        { "init-value", required_argument, NULL, OPTIONID_initValue },
        { "init-sign", required_argument, NULL, OPTIONID_initSign },


        { "minimize-predicates", required_argument, NULL, OPTIONID_predminimizationpredicate },
        { "minimization-algorithm", required_argument, NULL, OPTIONID_predminimizationalgorithm },
        { "min-chunk-percentage", required_argument, NULL, OPTIONID_predminimizationchunkpercentage },
        { "paracoherent", required_argument, NULL, OPTIONID_paracoherentsemantics },

        #if defined(ENABLE_PYTHON) || defined(ENABLE_PERL)
        /* HEURISTIC */
        { "interpreter", required_argument, NULL, OPTIONID_heuristic_interpreter },
        { "heuristic-scriptname", required_argument, NULL, OPTIONID_heuristic_scriptname },
        { "plugins-files", required_argument, NULL, OPTIONID_heuristic_plugins },
        { "script-directory", required_argument, NULL, OPTIONID_heuristic_setscriptdirectory },
        #endif
        /* RESTART OPTIONS */
        { "restarts", required_argument, NULL, OPTIONID_restarts },        

        /* GLUCOSE OPTIONS */
        { "size-lbdqueue", required_argument, NULL, OPTIONID_glucose_sizeLBDQueue },
        { "size-trailqueue", required_argument, NULL, OPTIONID_glucose_sizeTrailQueue },
        { "K", required_argument, NULL, OPTIONID_glucose_K },
        { "R", required_argument, NULL, OPTIONID_glucose_R },
        { "clauses-beforereduce", required_argument, NULL, OPTIONID_glucose_nbclausesBeforeReduce },
        { "inc-reduceDB", required_argument, NULL, OPTIONID_glucose_incReduceDB },
        { "specialinc-reduceDB", required_argument, NULL, OPTIONID_glucose_specialIncReduceDB },
        { "lbd-frozenclause", required_argument, NULL, OPTIONID_glucose_lbLBDFrozenClause },
        { "lbd-minclause", required_argument, NULL, OPTIONID_glucose_lbLBDMinimizingClause },
        { "lbd-keepclauses", required_argument, NULL, OPTIONID_glucose_keepClauses },

        /* INPUT OPTIONS */
        { "dimacs", no_argument, NULL, OPTIONID_dimacs },

        /* GENERIC OPTIONS*/
        { "time-limit", required_argument, NULL, OPTIONID_time_limit },
        { "max-cost", required_argument, NULL, OPTIONID_max_cost },
        { "disable-simplifications", optional_argument, NULL, OPTIONID_simplifications },
        { "enable-multiaggregates", no_argument, NULL, OPTIONID_multiaggregates },
        { "enumeration-strategy", required_argument, NULL, OPTIONID_enumeration },
        { "dynamic-configuration", required_argument, NULL, OPTIONID_dynamicConfiguration },

        { "modelchecker-algorithm", required_argument, NULL, OPTIONID_modelchecker_algorithm },

        /* WEAK CONSTRAINTS */
        { "weakconstraints-algorithm", required_argument, NULL, OPTIONID_weakconstraintsalgorithm },
        { "k-threshold", required_argument, NULL, OPTIONID_kthreshold },
        { "enable-disjcores", no_argument, NULL, OPTIONID_disjcores },
        { "trim-core", no_argument, NULL, OPTIONID_minimize },
        { "disable-stratification", no_argument, NULL, OPTIONID_stratification },
        { "compute-firstmodel", optional_argument, NULL, OPTIONID_firstmodel },
        { "shrinking-strategy", required_argument, NULL, OPTIONID_minimizationstrategy },
        { "shrinking-budget", required_argument, NULL, OPTIONID_minimizationbudget },

        /* SHIFT STRATEGY */
        { "disjunction", required_argument, NULL, OPTIONID_shift_strategy },
        //{ "enable-shift-onedef", no_argument, NULL, OPTIONID_shift_onedef },

        /* QUERY */
        { "query-algorithm", optional_argument, NULL, OPTIONID_queryalgorithm },
        { "query-verbosity", required_argument, NULL, OPTIONID_queryverbosity },
        { "query-chunk-size", required_argument, NULL, OPTIONID_querychunksize },
        { "query-chunk-percentage", required_argument, NULL, OPTIONID_querychunkpercentage },
        { "query-enable-core-cache", no_argument, NULL, OPTIONID_querycorecache },

        { "wellfounded", optional_argument, NULL, OPTIONID_wf },
        { "mode", required_argument, NULL, OPTIONID_mode },
        { "pre", required_argument, NULL, OPTIONID_pre },
        { "pre-verbosity", required_argument, NULL, OPTIONID_preverbosity },
        { NULL, 0, NULL, 0 }
    };

    do{
        // The function getopt_long() stores the opoptionFrontendtion index here.
        int optionIndex = 0;

        code = getopt_long (argc, argv, shortOptions, longOptions, &optionIndex);

        switch (code)
        {
            case -1:
                // All the lines were parsed
                break;
//            case OPTIONID_aspcore2strict:
//                aspCore2Strict = true;
//                break;
//
//            case OPTIONID_inmemory:
//                inputPolicy = BUILDER_IN_MEMORY;
//                break;
//
//            case OPTIONID_dlvdb:
//                inputPolicy = BUILDER_DLV_DB;
//                break;
//
//            case OPTIONID_testparser:
//                inputPolicy = BUILDER_EMPTY;
//                printStatistics = true;
//                break;

            case OPTIONID_printprogram:
                inputPolicy = BUILDER_MOCK_OBJECTS;
                printProgram = true;
                break;

            case OPTIONID_selector:
                inputPolicy = BUILDER_SELECTOR;
                printProgram = true;
                namedpipe = atoi( optarg );
                break;

            case OPTIONID_printdepgraph:
                inputPolicy = BUILDER_DEPGRAPH;
                printDepGraph = true;
                break;

            case OPTIONID_printstatistics:
                printStatistics = true;
                break;

            case OPTIONID_help:
                Help::printHelp(argv[0]);
                exit(0);
                break;

            case OPTIONID_stdin:
                inputFiles.push_back("--");
                break;

            case OPTIONID_datasource:
                dbDataSource.append(optarg);
                break;

            case OPTIONID_username:
                dbUsername.append(optarg);
                break;

            case OPTIONID_password:
                dbPassword.append(optarg);
                break;

            case OPTIONID_hashType:
            	hashType = atoi(optarg);
                assert_msg((hashType>=STL_HASH && hashType<=PERL_B),"Hash type not supported");
            	break;

            case OPTIONID_indexType:
            	indexType = atoi(optarg);
            	if(indexType==MAP_DICTIONARY_INTERSECTION) Options::globalOptions()->setEnabledDictionaryIntersection(true);
                assert_msg((indexType>=DEFAULT && indexType<=MAP_HISTORY_VECTOR),"Index type not supported");
                break;

            case OPTIONID_predIndexTerm:
                predicatesIndexTerm.append(optarg);
//                this->splitOption();
                break;

//            case OPTIONID_predIndexType:
//				predicatesIndexType.append(optarg);
//				this->splitOption(predicatesIndexType, predicatesIndexTypeMap);
//				break;

            case OPTIONID_nofacts:
            	nofacts=true;
				break;

            case OPTIONID_printRewrite:
				printRewrittenProgram=true;
            	break;

            case OPTIONID_rewritingType:
            	rewritingType= atoi(optarg);
				assert_msg((rewritingType>=DISJUNCTION && rewritingType<=COMPACT_NATIVE_CHOICE),"Rewriting type not supported");
            	break;

            case OPTIONID_outputFormat:
            	outputFormat=atoi(optarg);
            	mode = MODE_IDLV;
            	if(outputFormat==OUTPUT_QUERY_TEXTUAL){
                	evaluateQuery = true;
                	outputFormat=OUTPUT_QUERY_TEXTUAL;
            	}
            	break;

            case OPTIONID_textual:
            	outputFormat=OUTPUT_TEXTUAL;
            	mode = MODE_IDLV;
            	break;

            case OPTIONID_instantiationProcedure:
            	instantiationProcedure=atoi(optarg);
				break;

            case OPTIONID_positiveOrderingProcedure:
            	positiveOrderingProcedure=atoi(optarg);
				break;

            case OPTIONID_enableDictionaryIntersection:
            	enabledDictionaryIntersection=true;
				break;

            case OPTIONID_ruleTime:
            	ruleTime=true;
				break;

            case OPTIONID_checkFactDuplicate:
            	checkFactDuplicate=true;
				break;

            case OPTIONID_rewriteArith:
            	rewriteArith=true;
				break;

            case OPTIONID_anonymousFilter:
            	disabledAnonymousFilter=true;
				break;

            case OPTIONID_predicateToFilter:
            	predicateToFilter.append(optarg);
				break;

            case OPTIONID_compactFacts:
            	compactFacts=true;
				break;

            case OPTIONID_gstats:
            	if(optarg)
            		printGroundStats=atoi(optarg);
            	else
            		printGroundStats=BASE_STATS;
				break;

            case OPTIONID_project:
                    rewProject=(optarg)?atoi(optarg):0;
				break;

            case OPTIONID_FB:
            	optionFrontend = FRONTEND_BRAVE;
				break;

            case OPTIONID_FC:
            	optionFrontend = FRONTEND_CAUTIOUS;
				break;
            case OPTIONID_OMSM:
            	magicRewriting = false;
				break;

            case OPTIONID_EQ:
            	evaluateQuery = true;
            	outputFormat=OUTPUT_QUERY_TEXTUAL;
            	mode = MODE_IDLV;
				break;

            case OPTIONID_VERSION:
                Help::printVersion();
                exit(0);
                break;

            case OPTIONID_PYTHONFILE:
            	extFunctionPythonFile.append(optarg);
            	  char * pch;
            	  pch = strtok (optarg,",");
            	  while (pch != NULL)
            	  {
            		extAtomFactory->splitPythonPath(pch);
            	    pch = strtok (NULL, " ,");
            	  }
            	break;

            case OPTIONID_DECOMP:
            	enabledDecomposition=false;
                break;

            case OPTIONID_DECOMP_H:
            	if(atoi(optarg)>=0 && atoi(optarg)<=3)
            		decompositionAlgorithm=atoi(optarg);
            	break;
//            case OPTIONID_DECOMP_S:
//				decompositionSeed=atoi(optarg);
//                break;
            case OPTIONID_DECOMP_FORMULA:
            	if(atoi(optarg)>=DECOMPOSITION_FORMULA_NONE && atoi(optarg)<=DECOMPOSITION_FORMULA_JOIN_ULLMAN)
            		decompositionFormula=atoi(optarg);
            	break;
            case OPTIONID_DECOMP_THRESHOLD:
            	if(atof(optarg)>=0 && atof(optarg)<=1)
            		decompositionThreshold=atof(optarg);
            	break;
            case OPTIONID_DECOMP_ITERATIONS:
            	if(atoi(optarg)>=0 && atoi(optarg)<=20)
            		decompositionIterations=atoi(optarg);
            	break;
            case OPTIONID_DECOMP_NOT_IMPROVEMENT_LIMIT:
            	if(atof(optarg)>=0 && atof(optarg)<=20)
            		decompositionNotImprovementLimit=atoi(optarg);
            	break;
            case OPTIONID_DECOMP_NO_FITNESS:
            	disabledDecompFitness=true;
            	break;
            case OPTIONID_DECOMP_LIMIT_SIZE_BODY_FITNESS:
            	if(atoi(optarg)>=2)
            		decompLimitSizeBodyFitness=atoi(optarg);
            	break;
            case OPTIONID_enableLookAhead:
            	lookAhead=true;
            	break;
            case OPTIONID_NO_STRING_CONVERSION:
		noStrConversion = true;
		break;
	    case OPTIONID_DEFAULT_EXTERNAL_ATOMS:
		defaultExternalAtoms = true;
		break;
			case OPTIONID_NO_EXTERNAL_WARNINGS:
				enableExternalAtomsWarnings = true;
			break;
            case OPTIONID_models:
                if( optarg )
                {
                    maxModels = atoi( optarg );
                    if( maxModels == 0 )
                        maxModels = UINT_MAX;
                }
                break;

            case OPTIONID_trace_parser:
                setTraceLevel( parser, atoi( optarg ) );
                break;

            case OPTIONID_trace_checker:
                setTraceLevel( modelchecker, atoi( optarg ) );
                break;

            case OPTIONID_trace_heuristic:
                setTraceLevel( heuristic, atoi( optarg ) );
                break;

            case OPTIONID_trace_solving:
                setTraceLevel( solving, atoi( optarg ) );
                break;

            case OPTIONID_trace_us:
                setTraceLevel( unfoundedset, atoi( optarg ) );
                break;

            case OPTIONID_trace_learning:
                setTraceLevel( learning, atoi( optarg ) );
                break;

            case OPTIONID_trace_enumeration:
                setTraceLevel( enumeration, atoi( optarg ) );
                break;

            case OPTIONID_trace_satelite:
                setTraceLevel( satelite, atoi( optarg ) );
                break;

            case OPTIONID_trace_aggregates:
                setTraceLevel( waspaggregates, atoi( optarg ) );
                break;

            case OPTIONID_trace_weakconstraints:
                setTraceLevel( optimization, atoi( optarg ) );
                break;

            case OPTIONID_trace_disjunction:
                setTraceLevel( disjunction, atoi( optarg ) );
                break;

            case OPTIONID_trace_multiaggregates:
                setTraceLevel( multiaggregates, atoi( optarg ) );
                break;

            case OPTIONID_trace_predmin:
                setTraceLevel( predmin, atoi( optarg ) );
                break;

            case OPTIONID_trace_simplifications:
                setTraceLevel( simplifications, atoi( optarg ) );
                break;

            case OPTIONID_competition_output:
                waspOutputPolicy = COMPETITION_OUTPUT;
                break;

            case OPTIONID_silent:
                if( optarg )
                {
                    silent = atoi( optarg );
                    if( silent < 1 )
                        silent = 0;
                    else if( silent > 1 )
                        silent = 2;
                }
                waspOutputPolicy = SILENT_OUTPUT;
                outputPolicy = OUTPUT_SILENT; //I-DLV outputpolicy
                break;

            case OPTIONID_third_competition_output:
                waspOutputPolicy = THIRD_COMPETITION_OUTPUT;
                break;

            case OPTIONID_onlyOptimum:
                printOnlyOptimum = true;
                break;

            case OPTIONID_stats:
                stats = true;
                if( optarg )
                {
                    statsVerbosity = atoi( optarg );
                }
                break;

            case OPTIONID_lastModel:
                printLastModelOnly = true;
                break;

            case OPTIONID_minisatheuristic:
                minisatPolicy = true;
                break;

            case OPTIONID_heuristic_interpreter:
                if( optarg )
                {
                    #ifdef ENABLE_PERL
                    if( !strcmp( optarg, "perl" ) )
                        interpreter = PERL_INTERPRETER;
                    else
                    #endif
                    #ifdef ENABLE_PYTHON
                    if( !strcmp( optarg, "python" ) )
                        interpreter = PYTHON_INTERPRETER;
                    else
                    #endif
                        ErrorMessage::errorGeneric( "Unkwown interpreter." );
                }
                break;

            case OPTIONID_printbounds:
                printBounds = true;
                break;

            case OPTIONID_printatomtable:
                printAtomTable = true;
                break;

            case OPTIONID_idOutput:
                waspOutputPolicy = ID_OUTPUT;
                break;

            case OPTIONID_heuristic_scriptname:
                if( optarg )
                    heuristic_scriptname = optarg;
                break;

            case OPTIONID_heuristic_plugins:
                if( optarg )
                    Utility::split( string( optarg ), ',', pluginsFilenames );
                break;

            case OPTIONID_heuristic_setscriptdirectory:
                if( optarg )
                    scriptDirectory = string( optarg );
                break;            

            case OPTIONID_restarts: {
                if( optarg ) {
                    string s( optarg );
                    vector<string> elems;
                    Utility::split( s, ',', elems );
                    if(elems.size() > 2)
                        ErrorMessage::errorGeneric( "Unexpected value for restarts option." );
                    restartsPolicy = getRestartsPolicy(elems[0]);
                    restartsThreshold = elems.size() == 1 ? 0 : atoi( elems[1].c_str() );
                    if(restartsPolicy == SEQUENCE_BASED_RESTARTS_POLICY) {
                        if( restartsThreshold < 32 )
                            restartsThreshold = 32;
                    }
                    else if(restartsPolicy == GEOMETRIC_RESTARTS_POLICY) {
                        if( restartsThreshold < 100 )
                            restartsThreshold = 100;
                    }
                    else {
                        if(elems.size() != 1)
                            ErrorMessage::errorGeneric( "Unexpected value for restarts option." );
                    }                                        
                }
                else
                    ErrorMessage::errorGeneric( "Inserted invalid algorithm for restarts." );
                }
                break;            

            case OPTIONID_dimacs:
                waspOutputPolicy = DIMACS_OUTPUT;
                break;

            case OPTIONID_simplifications:
                {
                    simplifications = false;                
                    if( optarg ) {
                        int value = atoi(optarg);
                        if(value < 0) value = 0;
                        simplificationsThreshold = value;
                    }
                }
                break;

            case OPTIONID_modelchecker_algorithm:
                modelcheckerAlgorithm = REDUCT_BASED;
                if( optarg )
                {
                    if( !strcmp( optarg, "reduct" ) )
                        modelcheckerAlgorithm = REDUCT_BASED;
                    else if( !strcmp( optarg, "unfounded" ) )
                        modelcheckerAlgorithm = UNFOUNDED_BASED;
                    else
                        ErrorMessage::errorGeneric( "Inserted invalid algorithm for model checker." );
                }
                else
                    ErrorMessage::errorGeneric( "Inserted invalid algorithm for model checker." );
                break;

            case OPTIONID_multiaggregates:
                multiAggregates = true;
                break;

            case OPTIONID_time_limit:
                if( optarg )
                    timeLimit = atoi( optarg );
                break;

            case OPTIONID_max_cost:
                if( optarg )
                    maxCost = atoi( optarg );
                break;

            case OPTIONID_glucose_sizeLBDQueue:
                if( optarg )
                    sizeLBDQueue = atof( optarg );
                break;

            case OPTIONID_glucose_sizeTrailQueue:
                if( optarg )
                    sizeTrailQueue = atof( optarg );
                break;

            case OPTIONID_glucose_K:
                if( optarg )
                    K = atof( optarg );
                break;

            case OPTIONID_glucose_R:
                if( optarg )
                    R = atof( optarg );
                break;

            case OPTIONID_glucose_nbclausesBeforeReduce:
                if( optarg )
                    nbclausesBeforeReduce = atoi( optarg );
                break;

            case OPTIONID_glucose_incReduceDB:
                if( optarg )
                    incReduceDB = atoi( optarg );
                break;

            case OPTIONID_glucose_specialIncReduceDB:
                if( optarg )
                    specialIncReduceDB = atoi( optarg );
                break;

            case OPTIONID_glucose_lbLBDFrozenClause:
                if( optarg )
                    lbLBDFrozenClause = atoi( optarg );
                break;

            case OPTIONID_glucose_lbLBDMinimizingClause:
                if( optarg )
                    lbLBDMinimizingClause = atoi( optarg );
                break;

            case OPTIONID_glucose_keepClauses:
                if( optarg )
                    lbdKeepClauses = atoi( optarg );
                break;

            case OPTIONID_initVariableIncrement:
                if( optarg )
                    initVariableIncrement = atof( optarg );
                break;

            case OPTIONID_initVariableDecay:
                if( optarg )
                    initVariableDecay = 1 / ( double ) ( atoi( optarg ) / 100 );
                break;

            case OPTIONID_initStrategy:
                if( optarg )
                    initMinisatHeuristic = getInitMinisatHeuristic( optarg );
                break;

            case OPTIONID_initValue:
                if( optarg )
                    initValue = atoi( optarg );
                break;

            case OPTIONID_initSign:
                if( optarg )
                {
                    if( !strcmp( optarg, "false" ) )
                        initSign = INIT_SIGN_MINISAT_ALLFALSE;
                    else if( !strcmp( optarg, "true" ) )
                        initSign = INIT_SIGN_MINISAT_ALLTRUE;
                    else if( !strcmp( optarg, "mixed" ) )
                        initSign = INIT_SIGN_MINISAT_MIXED;
                }
                break;

            case OPTIONID_weakconstraintsalgorithm:
                if( optarg )
                    weakConstraintsAlg = getAlgorithm( string( optarg ) );
                else
                    ErrorMessage::errorGeneric( "Inserted invalid algorithm for weak constraints." );
                break;

            case OPTIONID_kthreshold:
                if( optarg )
                {
                    int kthreshold_ = atoi( optarg );
                    if( kthreshold_ < 0 )
                        ErrorMessage::errorGeneric( "Threshold for k must be >= 0." );
                    kthreshold = kthreshold_;
                }
                break;

            case OPTIONID_shift_strategy:
                if( optarg )
                    shiftStrategy = getShiftStrategy( string( optarg ) );
                else
                    ErrorMessage::errorGeneric( "Inserted invalid strategy for shift." );
                break;

            case OPTIONID_enumeration:
                if( optarg )
                    enumerationStrategy = getEnumerationStrategy( string( optarg ) );
                else
                    ErrorMessage::errorGeneric( "Inserted invalid strategy for enumeration." );
                break;

            case OPTIONID_dynamicConfiguration:
                if(optarg) {
                    vector<string> elems;
                    Utility::split(string(optarg), ',', elems);
                    if(elems.size() != 5) ErrorMessage::errorGeneric("Expected exactly 5 values for dynamic configuration, got " + to_string(elems.size()));
                    waspDynamicConfiguration = true;
                    waspDynamicConfigurationMin = atof(elems[0].c_str());
                    waspDynamicConfigurationMax = atof(elems[1].c_str());
                    waspDynamicConfigurationK = atoi(elems[2].c_str());
                    waspDynamicConfigurationFile = elems[3];
                    waspDynamicConfigurationStrategy = elems[4];
                    if(waspDynamicConfigurationMin < 0) ErrorMessage::errorGeneric("Min value cannot be less than 0.1");
                    //if(waspDynamicConfigurationMin > 0.8) ErrorMessage::errorGeneric("Min value cannot be greater than 0.8");
                    if(waspDynamicConfigurationMax < 0) ErrorMessage::errorGeneric("Max value cannot be less than 0.1");
                    //if(waspDynamicConfigurationMax > 0.9) ErrorMessage::errorGeneric("Max value cannot be greater than 0.9");
                    if(waspDynamicConfigurationMin > waspDynamicConfigurationMax) ErrorMessage::errorGeneric("Min value cannot be grater than max value");
                    if(atoi(elems[2].c_str()) <= 0) ErrorMessage::errorGeneric("Number of choices must be greater than 0");
                    if(waspDynamicConfigurationFile.size() == 0) ErrorMessage::errorGeneric("Configuration file cannot be empty");
                    if(waspDynamicConfigurationStrategy.size() == 0) ErrorMessage::errorGeneric("Configuration strategy cannot be empty");
                }
                else
                    ErrorMessage::errorGeneric( "Unexpected value for dynamic configuration." );
                break;

            //case OPTIONID_shift_onedef:
            //    oneDefShift = true;
            //    break;

            case OPTIONID_disjcores:
                disjCoresPreprocessing = true;
                break;

            case OPTIONID_minimize:
                minimizeUnsatCore = true;
                break;

            case OPTIONID_stratification:
                stratification = false;
                break;

            case OPTIONID_firstmodel:
                computeFirstModel = true;
                if( optarg )
                    budget = atoi( optarg );
                break;

            case OPTIONID_minimizationstrategy:
                if( optarg )
                    minimizationStrategy = getMinimizationStrategy( string( optarg ) );
                else
                    ErrorMessage::errorGeneric( "Inserted invalid strategy for minimization." );
                break;

            case OPTIONID_minimizationbudget:
                if( optarg )
                    minimizationBudget = atoi( optarg );
                break;

            case OPTIONID_queryalgorithm:
                queryAlgorithm = ITERATIVE_COHERENCE_TESTING;
                if( optarg )
                    queryAlgorithm = getQueryAlgorithm( optarg );
                else
                    ErrorMessage::errorGeneric( "Inserted invalid algorithm for query answering." );
                break;

            case OPTIONID_queryverbosity:
                queryVerbosity = 0;
                if( optarg )
                {
                    unsigned int value = atoi( optarg );
                    if( value <= 4 )
                        queryVerbosity = value;
                    else
                        ErrorMessage::errorGeneric( "Inserted invalid value for query verbosity." );
                }
                else
                        ErrorMessage::errorGeneric( "Inserted invalid value for query verbosity." );
                break;

            case OPTIONID_querycorecache:
                queryCoreCache = true;
                break;

            case OPTIONID_querychunksize:
                if( optarg )
                {
                    chunkSize = atoi( optarg );
                    if( chunkPercentage != UINT_MAX )
                        ErrorMessage::errorGeneric( "Only one option between chunk percentage and chunk size can be specified." );
                }
                else
                    ErrorMessage::errorGeneric( "Only one option between chunk percentage and chunk size can be specified." );
                break;

            case OPTIONID_querychunkpercentage:
                if( optarg )
                {
                    chunkPercentage = atoi( optarg );
                    if( chunkPercentage == 0 || chunkPercentage > 100 )
                        ErrorMessage::errorGeneric( "Inserted invalid value for chunk percentage." );
                    if( chunkSize != UINT_MAX )
                        ErrorMessage::errorGeneric( "Only one option between chunk percentage and chunk size can be specified." );
                }
                else
                    ErrorMessage::errorGeneric( "Inserted invalid value for chunk percentage." );
                break;

            case OPTIONID_predminimizationalgorithm:
                if( optarg )
                {
                    unordered_map< string, unsigned int >::iterator it = stringToPredMinimization.find( optarg );
                    if( it == stringToPredMinimization.end() )
                        ErrorMessage::errorGeneric( "Inserted invalid algorithm for pred minimization." );

                    predMinimizationAlgorithm = it->second;
                }
                break;

            case OPTIONID_predminimizationpredicate:
                if( optarg )
                {
                    string s( optarg );
                    Utility::split( s, ';', predicatesToMinimize );
                }
                break;

            case OPTIONID_predminimizationchunkpercentage:
                if( optarg )
                {
                    minimizePredicateChunkPercentage = atoi( optarg );
                    if( minimizePredicateChunkPercentage == 0 || minimizePredicateChunkPercentage > 100 )
                        ErrorMessage::errorGeneric( "Inserted invalid value for chunk percentage." );
                }
                break;

            case OPTIONID_paracoherentsemantics:
                if( optarg )
                {
                    string s( optarg );
                    Utility::split( s, ',', paracoherentSemantics );
                    if(paracoherentSemantics[0] != "semistable" && paracoherentSemantics[0] != "semiequilibrium" && paracoherentSemantics[0] != "split") {
                        ErrorMessage::errorGeneric( "Semantics can be semistable, semiequilibrium or split" );
                    }
                    predicatesToMinimize.push_back("#gap");
                    if(paracoherentSemantics.size() > 2) cerr << "WARNING: Ignored extra arguments." << endl;
                    string alg = (paracoherentSemantics.size() <= 1) ? "split" : paracoherentSemantics[1];
                    if(paracoherentSemantics[0] == "split") {
                        alg = "weak";
                        if(paracoherentSemantics.size() > 1 && paracoherentSemantics[1] != "weak") cerr << "WARNING: Split semantics only admits algorithm weak." << endl;
                    }
                    unordered_map< string, unsigned int >::iterator it = stringToParacoherent.find( alg );
                    if( it == stringToParacoherent.end() )
                        ErrorMessage::errorGeneric( "Inserted invalid algorithm for paracoherent semantics." );
                    predMinimizationAlgorithm = it->second;
                }
                break;

            case OPTIONID_pre:
                if( optarg ) {
                    unordered_map<string, unsigned int> elems;
                    elems["lparse"] = PREASP_PRINT_GRINGO;
                    elems["lparse-one-level"] = PREASP_PRINT_GRINGO_ONE_LEVEL;
                    elems["edimacs"] = PREASP_PRINT_EXTENDED_DIMACS;
                    elems["wbo"] = PREASP_PRINT_WBO;                    
                    unordered_map< string, unsigned int >::iterator it = elems.find( optarg );
                    if( it == elems.end() )
                        ErrorMessage::errorGeneric( "Inserted invalid strategy for pre." );

                    pre = it->second;
                }
                break;
            
            case OPTIONID_preverbosity:
                if( optarg ) {
                    preVerbosity = atoi( optarg );
                }
                break;
            
            case OPTIONID_wf:
                if(optarg) {
                    string s (optarg);
                    if(s != "cautious")
                        ErrorMessage::errorGeneric( "Inserted invalid option for well founded." );
                    else
                        pre = PREASP_WELL_FOUNDED_MODEL_CAUTIOUS;
                }
                else
                    pre = PREASP_WELL_FOUNDED_MODEL;
                break;

            case OPTIONID_mode:
                if( optarg ) {
                    unordered_map<string, unsigned int> elems;
                    elems["idlv"] = MODE_IDLV;
                    elems["wasp"] = MODE_WASP;
                    unordered_map< string, unsigned int >::iterator it = elems.find( optarg );
                    if( it == stringToPredMinimization.end() )
                        ErrorMessage::errorGeneric( "Inserted invalid option for mode." );

                    mode = it->second;
                }
                break;


            default:
                ErrorMessage::errorGeneric( "This option is not supported." );
                break;
        }
    }while( code != -1 );

    // Manage the reminder options (probably, they are all input files).
    for( int i = optind; i < argc; i++ )
    {
    	string filename(argv[ i ]);
    	if(Utils::hasEnding(filename,".py"))
    		extAtomFactory->splitPythonPath(filename);
    	else
    		inputFiles.push_back( argv[ i ] );
    }
    if(inputFiles.empty())
        inputFiles.push_back("--");
}

void Options::splitOption(){
	// Split the string indexingPreferences and
	// fill in the map with the indexing strategies for the specified
	regex pred_regex("[a-z]([a-zA-Z0-9_])*\\/([0-9])+\\=\\{(([0-9])+((\\,)|(\\}\\,)|(\\}$)))+");
	regex_token_iterator<string::iterator> rend;
	if(predicatesIndexTerm.compare("")!=0){
		regex re(";");
		regex_token_iterator<string::iterator> predicated_indexed(predicatesIndexTerm.begin(),predicatesIndexTerm.end(), re, -1);
		while(predicated_indexed!=rend){
			string s=*predicated_indexed++;
			if(regex_match(s, pred_regex)){
				regex re2("=|,|\\{|\\}");
				regex_token_iterator<string::iterator> tokens(s.begin(),s.end(), re2, -1);
				grounder::Predicate* predicate=nullptr;
				while(tokens!=rend){
					string ss=*tokens++;
					if(ss=="") continue;
					unsigned number=0;
					try
					{
						number = stoi(ss);
						if(predicate!=nullptr && number>=0 && number<predicate->getArity())
							predicatesIndexTermMap[predicate].push_back(number);
					}
					catch(const std::invalid_argument& ia)
					{
						std::string pred=ss;
						regex re3("\\/");
						regex_token_iterator<string::iterator> tokens2(pred.begin(),pred.end(), re3, -1);
						std::string predicateName=*tokens2++;
						std::string predicateArity=*tokens2;
						unsigned arity=0;
						try
						{
							arity=stoi(predicateArity);
							grounder::Predicate* p=new grounder::Predicate(predicateName,arity);
							grounder::PredicateTable::getInstance()->getPredicate(p);
							predicate=p;
							if(predicate!=nullptr)
								predicatesIndexTermMap.insert({predicate,vector<unsigned>()});
							else continue;
						}
						catch(const std::invalid_argument& ia){
							continue;
						}

						}
					}
			}
		}
	}
//		for(auto a:predicatesIndexTermMap){
//			cerr<<a.first->getName()<<" "<<a.first->getArity()<<endl;
//			for(auto b:a.second)
//				cerr<<b<<" ";
//			cerr<<endl;
//		}
}

vector<unsigned> Options::getPredicateIndexTerm(grounder::Predicate* predicate){
	if(!predicatesIndexTermMap.empty() && predicatesIndexTermMap.count(predicate))
		return predicatesIndexTermMap[predicate];
	return vector<unsigned>();
}

//int Options::getPredicateIndexType(const string& predicate){
//	if(predicatesIndexTypeMap.count(predicate)){
//		unsigned type=predicatesIndexTypeMap[predicate];
//		return type;
//	}
//	return -1;
//}

WEAK_CONSTRAINTS_ALG
Options::getAlgorithm(
    const string& s )
{
    unordered_map< string, WEAK_CONSTRAINTS_ALG >::iterator it = stringToWeak.find( s );
    if( it == stringToWeak.end() )
        ErrorMessage::errorGeneric( "Inserted invalid algorithm for weak constraints." );

    return it->second;
}

unsigned int
Options::getShiftStrategy(
    const string& s )
{
    unordered_map< string, unsigned int >::iterator it = stringToShift.find( s );
    if( it == stringToShift.end() )
        ErrorMessage::errorGeneric( "Inserted invalid strategy for shift." );

    return it->second;
}

unsigned int
Options::getMinimizationStrategy(
    const string& s )
{
    unordered_map< string, unsigned int >::iterator it = stringToMinimization.find( s );
    if( it == stringToMinimization.end() )
        ErrorMessage::errorGeneric( "Inserted invalid strategy for minimization." );

    return it->second;
}

unsigned int
Options::getQueryAlgorithm(
    const string& s )
{
    unordered_map< string, unsigned int >::iterator it = stringToQueryAlgorithms.find( s );
    if( it == stringToQueryAlgorithms.end() )
        ErrorMessage::errorGeneric( "Inserted invalid strategy for query algorithm." );

    return it->second;
}

unsigned int
Options::getInitMinisatHeuristic(
    const string& s )
{
    unordered_map< string, unsigned int >::iterator it = stringToInitMinisatHeuristic.find( s );
    if( it == stringToInitMinisatHeuristic.end() )
        ErrorMessage::errorGeneric( "Inserted invalid strategy for initialization of minisat heuristic." );

    return it->second;
}

RESTARTS_POLICY
Options::getRestartsPolicy(
    const string& s )
{
    unordered_map< string, unsigned int >::iterator it = stringToRestarts.find( s );
    if( it == stringToRestarts.end() )
        ErrorMessage::errorGeneric( "Inserted invalid strategy for restarts." );

    return it->second;
}

unsigned int
Options::getEnumerationStrategy(
    const string& s )
{
    if( s == "bt" )
        return ENUMERATION_BT;
    else if( s == "bc" )
        return ENUMERATION_BC;
    ErrorMessage::errorGeneric( "Inserted invalid strategy for enumeration." );
    return ENUMERATION_BT;
}

void
Options::initMap()
{
    stringToWeak[ "one" ] = ONE;
    stringToWeak[ "opt" ] = OPT;
    stringToWeak[ "pmres" ] = PMRES;
    stringToWeak[ "basic" ] = BB;
    stringToWeak[ "k" ] = KALG;

    stringToShift[ "propagator" ] = PREASP_COMPLETION_PROPAGATOR;
    stringToShift[ "compv" ] = PREASP_COMPLETION_DISJUNCTION;
    stringToShift[ "auto" ] = PREASP_COMPLETION_AUTO;

    stringToMinimization[ "progression" ] = MINIMIZATION_PROGRESSION;
    stringToMinimization[ "linearsearch" ] = MINIMIZATION_LINEARSEARCH;

    stringToQueryAlgorithms[ "or" ] = OVERESTIMATE_REDUCTION;
    stringToQueryAlgorithms[ "ict" ] = ITERATIVE_COHERENCE_TESTING;
    stringToQueryAlgorithms[ "cb" ] = COREBASED_QUERIES;
    stringToQueryAlgorithms[ "chunk-dynamic" ] = CHUNK_DYNAMIC;
    stringToQueryAlgorithms[ "preferences" ] = PREFERENCE_QUERIES;
    stringToQueryAlgorithms[ "one" ] = ONE_QUERIES;
    stringToQueryAlgorithms[ "kdyn" ] = KDYN_QUERIES;
    stringToQueryAlgorithms[ "pmres" ] = PMRES_QUERIES;
    stringToQueryAlgorithms[ "ict-preferences" ] = ITERATIVE_COHERENCE_TESTING_PREFERENCES;
    stringToQueryAlgorithms[ "pi" ] = PRIME_IMPLICATE;

    stringToPredMinimization[ "guess-check" ] = PREDMIN_GUESS_AND_CHECK;
    stringToPredMinimization[ "guess-check-minimize" ] = PREDMIN_GUESS_AND_CHECK_AND_MINIMIZE;
    stringToPredMinimization[ "guess-check-split" ] = PREDMIN_GUESS_AND_CHECK_AND_SPLIT;
    stringToPredMinimization[ "preferences" ] = PREDMIN_PREFERENCES;
    stringToPredMinimization[ "core-based"] = PREDMIN_CORE;

    stringToInitMinisatHeuristic[ "all-equals" ] = INIT_MINISAT_ALL_EQUALS;
    stringToInitMinisatHeuristic[ "moms" ] = INIT_MINISAT_MOMS;
    stringToInitMinisatHeuristic[ "binary" ] = INIT_MINISAT_BINARY;
    stringToInitMinisatHeuristic[ "watches" ] = INIT_MINISAT_WATCHES;
    stringToInitMinisatHeuristic[ "propagators" ] = INIT_MINISAT_PROPAGATORS;
    stringToInitMinisatHeuristic[ "visible-atoms" ] = INIT_MINISAT_VISIBLE_ATOMS;
    stringToInitMinisatHeuristic[ "hidden-atoms" ] = INIT_MINISAT_HIDDEN_ATOMS;
    stringToInitMinisatHeuristic[ "combination" ] = INIT_MINISAT_COMBINATION;

    stringToParacoherent[ "gc" ] = PREDMIN_GUESS_AND_CHECK;
    stringToParacoherent[ "minimize" ] = PREDMIN_GUESS_AND_CHECK_AND_MINIMIZE;
    stringToParacoherent[ "split" ] = PREDMIN_GUESS_AND_CHECK_AND_SPLIT;
    stringToParacoherent[ "opt" ] = PREDMIN_PREFERENCES;
    stringToParacoherent[ "weak" ] = PREDMIN_WEAK;

    stringToRestarts["glucose"] = GLUCOSE_RESTARTS_POLICY;
    stringToRestarts["geometric"] = GEOMETRIC_RESTARTS_POLICY;
    stringToRestarts["sequence"] = SEQUENCE_BASED_RESTARTS_POLICY;
    stringToRestarts["no"] = NO_RESTARTS_POLICY;
}
