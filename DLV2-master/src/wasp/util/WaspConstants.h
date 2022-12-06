/*
 *
 *  Copyright 2013 Mario Alviano, Carmine Dodaro, and Francesco Ricca.
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

#ifndef WASP_CONSTANTS_H
#define WASP_CONSTANTS_H

#include <climits>

//enum TruthValue { UNDEFINED = 0, FALSE = 1, TRUE = 2 };
#define UNDEFINED 0
#define FALSE 1
#define TRUE 2
#define CACHE_FALSE 8
#define CACHE_TRUE 16
#define UNROLL_MASK 3

#define POSITIVE 0
#define NEGATIVE 1
#define NOT_ELIMINATED 2

#define HEURISTIC_BERKMIN 0
#define HEURISTIC_FIRST_UNDEFINED 1
#define HEURISTIC_MINISAT 2
#define HEURISTIC_BERKMIN_CACHE 3

#define WASP_OUTPUT 0
#define COMPETITION_OUTPUT 1
#define DIMACS_OUTPUT 2
#define SILENT_OUTPUT 3
#define THIRD_COMPETITION_OUTPUT 4
#define MULTI 5
#define ID_OUTPUT 6
#define NO_OUTPUT 7

#define SEQUENCE_BASED_RESTARTS_POLICY 0
#define GEOMETRIC_RESTARTS_POLICY 1
#define GLUCOSE_RESTARTS_POLICY 2
#define NO_RESTARTS_POLICY 3

#define NO_QUERY 0
#define OVERESTIMATE_REDUCTION 1
#define ITERATIVE_COHERENCE_TESTING 2
#define CHUNK_DYNAMIC 3
#define COREBASED_QUERIES 4
#define PREFERENCE_QUERIES 5
#define ONE_QUERIES 6
#define KDYN_QUERIES 7
#define PMRES_QUERIES 8
#define ITERATIVE_COHERENCE_TESTING_PREFERENCES 9
#define PRIME_IMPLICATE 10

#define BUDGET_CHOICES 0
#define BUDGET_RESTARTS 1
#define BUDGET_TIME 2

#define NO_PREDMINIMIZATION 0
#define PREDMIN_GUESS_AND_CHECK 1
#define PREDMIN_GUESS_AND_CHECK_AND_MINIMIZE 2
#define PREDMIN_GUESS_AND_CHECK_AND_SPLIT 3
#define PREDMIN_PREFERENCES 4
#define PREDMIN_CORE 5
#define PREDMIN_WEAK 6

/*
 * Weak constraints
 */
#define OPT 0
#define ONE 1
#define BB 2
#define PMRES 3
#define KALG 4

#define MINIMIZATION_OFF 0
#define MINIMIZATION_PROGRESSION 1
#define MINIMIZATION_LINEARSEARCH 2

#define INCOHERENT 0
#define COHERENT 1
#define OPTIMUM_FOUND 2
#define OPTIMUM_FOUND_STOP 3
#define INTERRUPTED 4

#if defined(ENABLE_PYTHON) && defined(ENABLE_PERL)
    #define SCRIPT_STRINGS " [scripts: PERL/PYTHON]"
#else
    #if defined(ENABLE_PYTHON)
        #define SCRIPT_STRINGS " [script: PYTHON]"
    #else
        #if defined(ENABLE_PERL)
            #define SCRIPT_STRINGS " [script: PERL]"
        #else
            #define SCRIPT_STRINGS ""
        #endif
    #endif
#endif

#define ENUMERATION_BC 0
#define ENUMERATION_BT 1

#define REDUCT_BASED 0
#define UNFOUNDED_BASED 1

#define WASP_VERSION "3.0.2"

/*
 * Wasp constants
 */
#define WASP_STRING "WASP " WASP_VERSION SCRIPT_STRINGS "\n"
#define QUERY_FALSE_OUTPUT "no."
#define QUERY_TRUE_OUTPUT "yes."
#define MAXUNSIGNEDINT UINT_MAX

/*
 * Error messages
 */
#define ERRORPARSING "Error during parsing"
#define ERRORGENERIC "Generic error"
#define ERRORPARSINGCODE 100
#define ERRORGENERICCODE 110

/*
 * DIMACS Format
 */
#define COMMENT_DIMACS 'c'
#define FORMULA_INFO_DIMACS 'p'
#define SOLUTION_DIMACS 's'
#define OPTIMUM_DIMACS 'o'
#define VALUE_DIMACS 'v'
#define OPTIMUM_FOUND_DIMACS "OPTIMUM FOUND"
#define UNSAT "UNSATISFIABLE"
#define SAT "SATISFIABLE"

/*
 * Gringo Numeric Format
 */
#define GRINGO_NORMAL_RULE_ID 1
#define GRINGO_COUNT_CONSTRAINT_RULE_ID 2
#define GRINGO_CHOICE_RULE_ID 3
#define GRINGO_SUM_CONSTRAINT_RULE_ID 5
#define GRINGO_OPTIMIZATION_RULE_ID 6
#define GRINGO_DISJUNCTIVE_RULE_ID 8
#define GRINGO_LINE_SEPARATOR 0
#define GRINGO_BPLUS "B+"
#define GRINGO_BMINUS "B-"

/*
 * Interpreters
 */
#define NO_INTERPRETER 0
#define PYTHON_INTERPRETER 1
#define PERL_INTERPRETER 2

/*
 * Init minisat heuristic
 */
#define INIT_MINISAT_ALL_EQUALS 0
#define INIT_MINISAT_MOMS 1
#define INIT_MINISAT_BINARY 2
#define INIT_MINISAT_WATCHES 3
#define INIT_MINISAT_PROPAGATORS 4
#define INIT_MINISAT_VISIBLE_ATOMS 5
#define INIT_MINISAT_HIDDEN_ATOMS 6
#define INIT_MINISAT_COMBINATION 7

#define INIT_SIGN_MINISAT_ALLFALSE 0
#define INIT_SIGN_MINISAT_ALLTRUE 1
#define INIT_SIGN_MINISAT_MIXED 2

/*
 * New types
 */
typedef double Activity;
typedef unsigned int Var;
typedef unsigned int TruthValue;
typedef unsigned int DELETION_POLICY;
typedef unsigned int DECISION_POLICY;
typedef unsigned int OUTPUT_POLICY;
typedef unsigned int RESTARTS_POLICY;
typedef unsigned int WEAK_CONSTRAINTS_ALG;

#endif
