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

#ifndef HELP_H
#define HELP_H

#include <iostream>
#include "../idlv/util/Constants.h"
#include "../wasp/util/WaspConstants.h"

namespace DLV2
{

    class Help
    {
    public:
    	static void printVersion()
    	{
             std::cout << DLV2_STRING << std::endl;
             std::cout <<
"Copyright 2021 DLVSYSTEM s.r.l\n\n\
This software is free for academic and non-commerical educational use,\n\
as well as for use by non-profit organisations.\n\n\
For further information (including commercial use and evaluation licenses)\n\
please contact info@dlvsystem.com or visit http://www.dlvsystem.com/license-request/.\n\n\
THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED\n\
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF\n\
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.\n\
IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR\n\
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL\n\
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE\n\
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS\n\
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER\n\
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR\n\
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF\n\
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n" << std::endl;


    		 std::cout << IDLV_STRING << std::endl;
    		 std::cout <<"Copyright 2016 Francesco Calimeri, Davide Fusca', Simona Perri and Jessica Zangari "<<std::endl;
    		 std::cout <<"Licensed under the Apache License, Version 2.0 (the \"License\");"<< std::endl;
			 std::cout <<"you may not use this file except in compliance with the License."<< std::endl;
			 std::cout <<"You may obtain a copy of the License at:"<< std::endl;
			 std::cout << std::endl;
			 std::cout <<"\thttp://www.apache.org/licenses/LICENSE-2.0"<< std::endl;
			 std::cout << std::endl;
			 std::cout <<"Unless required by applicable law or agreed to in writing, software"<< std::endl;
			 std::cout <<"distributed under the License is distributed on an \"AS IS\" BASIS,"<< std::endl;
			 std::cout <<"WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied."<< std::endl;
			 std::cout <<"See the License for the specific language governing permissions and"<< std::endl;
			 std::cout <<"limitations under the License."<<std::endl;
			 std::cout << std::endl;
			 std::cout <<"For further information or bug report, contact i-dlv@googlegroups.com."<<std::endl;

             std::cout << WASP_STRING << std::endl;
             std::cout << "Copyright 2013-2019 Mario Alviano, Carmine Dodaro, and Francesco Ricca." << std::endl;
             std::cout << "Licensed under the Apache License, Version 2.0 (the \"License\");" << std::endl;
             std::cout <<"you may not use this file except in compliance with the License."<< std::endl;
			 std::cout <<"You may obtain a copy of the License at:"<< std::endl;
			 std::cout << std::endl;
			 std::cout <<"\thttp://www.apache.org/licenses/LICENSE-2.0"<< std::endl;
			 std::cout << std::endl;
			 std::cout <<"Unless required by applicable law or agreed to in writing, software"<< std::endl;
			 std::cout <<"distributed under the License is distributed on an \"AS IS\" BASIS,"<< std::endl;
			 std::cout <<"WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied."<< std::endl;
			 std::cout <<"See the License for the specific language governing permissions and"<< std::endl;
			 std::cout <<"limitations under the License."<<std::endl;
			 std::cout << std::endl;
		}

        static void printHelp( const char* exec )
        {
            std::string separator = "\n---------------------------\n";

            std::cout << IDLV_STRING << std::endl;
            std::cout << "usage: " << exec << " {OPTIONS} [filename [filename [...]]]" << std::endl;
            std::cout << separator << std::endl;

//            std::cout << "Input options                         " << std::endl << std::endl;
//            std::cout << "--aspcore2strict                       - Enable \"strict\" AspCore2 format as input." << std::endl;
//            std::cout << "--inmemory                             - Enable the input builder for the in-memory version. (default choice)" << std::endl;
//            std::cout << "--dlv-db                               - Enable the input builder for the DB-based version." << std::endl;
//            std::cout << "--test-parser                          - Enable an empty input builder in order to test just parsing functionalities." << std::endl;
//            std::cout << separator << std::endl;

            std::cout << "Output options                         " << std::endl << std::endl;
            std::cout << "--silent                              - Suppress the startup banner and blank lines." << std::endl;
            std::cout << "--no-facts                            - Suppress the printing of EDB." << std::endl;
            std::cout << "--output                              - Set the output format: "<<std::endl;
                    std::cout << "                                            0: smodels numeric format (Default), " << std::endl;
        			std::cout << "                                            1: plain text format, " << std::endl;
        			std::cout << "                                            2: query format (to be used when the input program contains a query to get the answer(s)," << std::endl;
        			std::cout << "                                            3: intermediate numeric format," << std::endl;
            std::cout << "--t                                   - Print in textual mode." << std::endl;
            std::cout << "--filter                              - Filter the specified predicates with the specified arity. Example: --filter=p1/2,p2/3." << std::endl;
            std::cout << "--print-rewriting                     - Print the rewritten program." << std::endl;
            std::cout << "--query                               - Print the results of the input query." << std::endl;
//            std::cout << "--printprogram                         - Enable the input builder for mock objects, print the input program and exit." << std::endl;
//            std::cout << "--printdepgraph                        - Enable the input builder for the dependency graph, print it and exit." << std::endl;
//            std::cout << "--stats                                - Print final statistics." << std::endl;
            std::cout << separator << std::endl;

            std::cout << "Grounding options                         " << std::endl << std::endl;
//            std::cout << "--hash-type                           - Set the hash type of objects. Possible values: STL_HASH=0, BOOST_HASH=1, JAVA_HASH=2, MUR_HASH=3, PERL_DJ=4, PERL_B=5." << std::endl;
//            std::cout << "--index-type                          - Set the index table of object. Possible values: DEFAULT=0, MAP=1, MULTIMAP=2, HASHSET=3, DOUBLEMAP=4." << std::endl;
//            std::cout << "--pred-index-type                     - Set the index type for the given predicates. Example: --pred-index-type=p=1,a=2" << std::endl;
            std::cout << "--check-edb-duplication               - If present, remove duplicate EDB." << std::endl;
            std::cout << "--choice-rewriting                    - Set the rewriting strategy to adopt for choice rules: "<< std::endl;
            std::cout << "                                            0: By means of disjunction, " <<std::endl;
			std::cout << "                                            1: By rewriting choice rules into smaller choice rules (Default)," << std::endl;
			std::cout << "                                            2: By rewriting choice rules in a compact way," << std::endl;
            std::cout << "--rewrite-arith-terms                 - Enable the rewriting of arithmetic terms into assignment built-in atoms." << std::endl;
            std::cout << "--align-dictionaries                  - Enable the dictionaries alignment mechanism." << std::endl;
            std::cout << "--look-ahead                          - Enable the look ahead mechanism." << std::endl;
            std::cout << "--ordering                            - Set the body ordering strategy: " << std::endl;
            std::cout << "                                            0=BASE ORDERING," << std::endl;
            std::cout << "                                            1=DLV COMBINED CRITERION," << std::endl;
            std::cout << "                                            2=COMBINED+ CRITERION (Default)," << std::endl;
            std::cout << "                                            3=FUNCTIONAL TERMS CRITERION," << std::endl;
            std::cout << "                                            4=INDEXING CRITERION," << std::endl;
            std::cout << "                                            5=BACKJUMPING CRITERION," << std::endl;
            std::cout << "                                            6=COMBINATION OF 4 and 5 CRITERIA." << std::endl;
            std::cout << "--no-isolated-filter                  - Disable the filtering mechanism of isolated variables." << std::endl;
            std::cout << "--no-projection                       - Configure the projection rewriting of irrelevant variables and/or functions:" << std::endl;
            std::cout << "                                            0=Project nothing," << std::endl;
            std::cout << "                                            1=Project functional terms only," << std::endl;
            std::cout << "                                            2=Project irrelevant variables only," << std::endl;
            std::cout << "                                            3=Project both irrelevant variables and functional terms." << std::endl;
            std::cout << "--indexing                            - Set the indexing terms for the specified predicates. Example: --indexing=p/3={0,1};p/2={1}." << std::endl;
            std::cout << "--FC                                  - Enable cautious reasoning." << std::endl;
            std::cout << "--FB                                  - Enable brave reasoning." << std::endl;
            std::cout << "--no-magic-sets                       - Disable the Magic Sets rewriting." << std::endl;
#ifdef HTD
            std::cout << "--no-decomp                           - Disable the decomposition rewriting." << std::endl;
    		std::cout << "--decomp-algorithm                    - Set the decomposition algorithm: " << std::endl;
            std::cout << "                                            0=MINIMUM DEGREE (Default)," << std::endl;
            std::cout << "                                            1=MAXIMUM CARDINALITY SEARCH," << std::endl;
            std::cout << "                                            2=MINIMUM FILL," << std::endl;
            std::cout << "                                            3=NATURAL." << std::endl;
//            std::cout << "--decomp-s                            - Set the random seed for the decomposition rewriting (Default 0). " << std::endl;
            std::cout << "--decomp                              - Configure the decomposition rewriting: " << std::endl;
            std::cout << "                                            0=For every rule, the best estimated decomposition is determined "<< std::endl;
			std::cout << "                                              and applied even if not estimated as convenient;" << std::endl;
            std::cout << "                                            1=For every rule, the best estimated decomposition is determined "<< std::endl;
			std::cout << "                                              and applied only if estimated as convenient (Default)." << std::endl;
            std::cout << "--decomp-threshold                    - Set the threshold value for decomposition rewriting (Allowed values: [0,1], Default 0.5). " << std::endl;
            std::cout << "--decomp-iterations                   - Set the decomposition tentative iterations (Allowed values: [0,20], Default 5). " << std::endl;
            std::cout << "--decomp-limit                        - Set the decomposition not improvement limits " << std::endl;
			std::cout << "                                        (Allowed values: [0,20] and <= of decomposition iterations, Default 3)." << std::endl;
            std::cout << "--decomp-no-fitness                   - Disable the fitness mechanism of decomposition rewriting." << std::endl;
            std::cout << "--decomp-size-body-limit              - Set the maximum allowed number of literals in rules body "<< std::endl;
			std::cout << "                                        for which the fitness mechanism has be enabled (Default 10)." << std::endl;
#endif
#ifdef ENABLE_PYTHON
            std::cout << "--enable-external-warnings             - Print warnings about Python external atoms." << std::endl;
			std::cout << "--default-external-atoms              - Set the preference for the internally implemented external atom when there is an external "<<std::endl;
			std::cout << "                                        one with the same name (Default disabled)." << std::endl;
#endif
			std::cout << "--no-string-conversion                - Disable the conversion of UTF-8 to UTF-16 strings in default external atoms that handle "<< std::endl;
			std::cout << "                                        strings (Default active)." << std::endl;
			//          std::cout << "--compact-facts                       - Assign the fact atom a unique index" << std::endl;
            std::cout << separator << std::endl;

            std::cout << "Statistics options                         " << std::endl << std::endl;
            std::cout << "--time                                - Print the grounding time of each rule." << std::endl;
            std::cout << "--gstats                              - Display grounding statistics." << std::endl;
            std::cout << separator << std::endl;

//            std::cout << "DB options                             " << std::endl << std::endl;
//            std::cout << "--db-source=[db_name]                  - Set the name of the database where input data are stored." << std::endl;
//            std::cout << "--db-user=[db_username]                - Set the username to have access to the database." << std::endl;
//            std::cout << "--db-pwd=[db_password]                 - Set the password to have access to the database." << std::endl;
//            std::cout << separator << std::endl;

            std::cout << "General options                     " << std::endl << std::endl;
            std::cout << "--help                                - Print this guide and exit." << std::endl;
            std::cout << "--version                             - Print version information and exit." << std::endl;
            std::cout << "--license                             - Print license information and exit." << std::endl;
            std::cout << "--stdin                               - Read input from standard input." << std::endl;
            std::cout << separator << std::endl;

            printHelpWasp();

            std::cout << "--mode                                 - Select usage mode" << std::endl;
            std::cout << "       =idlv                           - Use idlv (ground without solving)" << std::endl;
            std::cout << "       =wasp                           - Use wasp (input in numerical format)" << std::endl;
        }

        static void printHelpWasp()
        {
            string separator = "\n---------------------------\n";

            cout << WASP_STRING << endl;

            cout << separator << endl;

            #ifdef TRACE_ON
            cout << "Trace options                          " << endl << endl;
            cout << "--trace-waspaggregates=<verbosity>     - Trace aggregates propagation." << endl;
            cout << "--trace-multiaggregates=<verbosity>    - Trace multi aggregates propagation." << endl;
            cout << "--trace-disjunction=<verbosity>        - Trace disjunction propagator." << endl;
            cout << "--trace-enumeration=<verbosity>        - Trace enumeration." << endl;
            cout << "--trace-heuristic=<verbosity>          - Trace heuristic." << endl;
            cout << "--trace-learning=<verbosity>           - Trace learning." << endl;
            cout << "--trace-modelchecker=<verbosity>       - Trace model checker." << endl;
            cout << "--trace-parser=<verbosity>             - Trace parser." << endl;
            cout << "--trace-satelite=<verbosity>           - Trace satelite simplifications." << endl;
            cout << "--trace-solver=<verbosity>             - Trace solver." << endl;
            cout << "--trace-unfoundedset=<verbosity>       - Trace computation of unfounded sets." << endl;
            cout << "--trace-optimization=<verbosity>       - Trace weak constraints algorithms." << endl;
            cout << "--trace-predmin=<verbosity>            - Trace minimization of predicate." << endl;
            cout << separator << endl;
            #endif

            cout << "Restarts options                       " << endl << endl;
            cout << "--restarts                             - Specify an option for restarts [default: glucose]" << endl;
            cout << "       =sequence,N                     - Luby et al. sequence using N (optional) as number of conflicts [default/min: 32]" << endl;
            cout << "       =geometric,N                    - Geometric restarts using N (optional) as number of conflicts [default/min: 100]" << endl;
            cout << "       =glucose                        - Glucose restarts" << endl;
            cout << "       =no                             - Disable restarts" << endl;
            cout << separator << endl;
//
//            cout << "Deletion options                       " << endl << endl;
//            cout << "--aggressive-deletion                  - Aggressive variant of minisat deletion policy." << endl;
//            cout << "--restarts-based-deletion              - Restarts-based deletion policy." << endl;
//            cout << "--minisat-deletion                     - Minisat deletion policy." << endl;
//            cout << "--glucose-deletion=<N>                 - Glucose deletion policy. Keep all the clauses with a LBD less or equal than N." << endl;
//            cout << separator << endl;

            cout << "Output options                         " << endl << endl;
            cout << "--silent                               - Set verbosity of output" << endl;
            cout << "       <1                              - Print models with no spaces and lines" << endl;
            cout << "       =1                              - Print only the number of each model" << endl;
            cout << "       >1                              - Print only the number of models" << endl;
            cout << "--third-competition-output             - Print models according to the third competition output" << endl;
            cout << "--competition-output                   - Print models according to the fourth competition output" << endl;
            cout << "--id-output                            - Print models using the internal id of atoms" << endl;
            cout << "--printlatestmodel                     - Print the latest computed model" << endl;
            cout << "--printonlyoptimum                     - Print only optimum answer sets" << endl;
            cout << "--printbounds                          - Print lower and upper bound during optimization of answer sets" << endl;
            cout << "--printatomstable                      - Print internal dictionary for atoms used in wasp" << endl;

            cout << separator << endl;

            cout << "Heuristics options                     " << endl << endl;
            cout << "--minisat-policy                       - Enable minisat policy for deletion" << endl;
            cout << "--init-varinc=N                        - Initial value of variable increment [default: 1.0]" << endl;
            cout << "--init-vardecay=N                      - Initial value of variable decay 1/0.N [default: 95]" << endl;
            cout << "--init-strategy                        - Set the strategy to initialize minisat variable activities. N is the value specified by option --init-value" << endl;
            cout << "       =all-equals                     - All activities are equal to N" << endl;
            cout << "       =moms                           - Use moms (N is added to computed values)" << endl;
            cout << "       =binary                         - Use binary clauses (N is added to computed values)" << endl;
            cout << "       =watches                        - Use watches (N is added to computed values)" << endl;
            cout << "       =propagators                    - Use propagators (N is added to computed values)" << endl;
            cout << "       =visible-atoms                  - The activities of visible atoms are equal to N" << endl;
            cout << "       =hidden-atoms                   - The activities of hidden atoms are equal to N" << endl;
            cout << "       =combination                    - Use a combination of moms, watches and propagators (N is added to computed values)" << endl;
            cout << "--init-value=N                         - Initial value of init-strategy [default: 0]" << endl;
            cout << "--init-sign                            - Set the polarity of selected literals" << endl;
            cout << "       =false                          - Choose always false" << endl;
            cout << "       =true                           - Choose always true" << endl;
            cout << "       =mixed                          - Choose always mixed" << endl;
            cout << separator << endl;
            cout << "Glucose options                        " << endl << endl;
            cout << "--size-lbdqueue=N                      - Initial size of LBD queue [default: 50]" << endl;
            cout << "--size-trailqueue=N                    - Initial size of trail queue [default: 5000]" << endl;
            cout << "--K=N                                  - Value used for restarts, greater values increment restarts [default: 0.8]" << endl;
            cout << "--R=N                                  - Value used for restarts, greater values reduce restarts [default: 1.4]" << endl;
            cout << "--clauses-beforereduce=N               - Number of clauses needed for deletion [default: 2000]" << endl;
            cout << "--inc-reduceDB=N                       - Increment of the previous value after each deletion [default: 300]" << endl;
            cout << "--specialinc-reduceDB=N                - Special increment performed when clauses have low lbd [default: 1000]" << endl;
            cout << "--lbd-frozenclause=N                   - Keep clauses with LBD lower than N [default: 30]" << endl;
            cout << "--lbd-minclause=N                      - Learned clauses with LBD lower than N are minimized [default: 6]" << endl;
            cout << "--lbd-keepclauses=N                    - Learned clauses with LBD <= N are not removed [default: 2]" << endl;

            #if defined(ENABLE_PERL) || defined(ENABLE_PYTHON)
            cout << separator << endl;

            cout << "External scripts options                     " << endl << endl;
            cout << "--interpreter                          - Set the interpreter for heuristics and plugins" << endl;
            #ifdef ENABLE_PERL
            cout << "       =perl                           - Use perl as interpreter" << endl;
            #endif
            #ifdef ENABLE_PYTHON
            cout << "       =python                         - Use python as interpreter" << endl;
            #endif
            cout << "--heuristic-scriptname=file            - Specify the name of the script" << endl;            
            cout << "                                       - add \"file param1 param2\" for parameters (requires an empty list sys_parameters in the python file)" << endl;
            cout << "--plugins-files=file1,file2,...,filen  - Specify a list of plugins" << endl;
            cout << "                                       - add \"file1 param1 param2\" for parameters (requires an empty list sys_parameters in the python file)" << endl;
            cout << "--script-directory=absolutepath        - Specify a different folder for retrieving the python modules (default is .)" << endl;
            cout << "--lazy-weakconstraints                 - Enable the algorithm to lazily add weak constraints" << endl;
            #endif
            cout << separator << endl;

            cout << "Model Checker options                  " << endl << endl;
            cout << "--modelchecker-algorithm               - Compute the unfounded check for HC components using the specified algorithm" << endl;
            cout << "       =reduct                         - Algorithm based on the GL-reduct" << endl;
            cout << "       =unfounded                      - Algorithm based on the unfounded sets conditions" << endl;

            cout << separator << endl;
            cout << "Weak Constraints options               " << endl << endl;
            cout << "--weakconstraints-algorithm            - Compute optimization problems using the specified algorithm" << endl;
            cout << "       =basic                          - Model-guided algorithm basic" << endl;
            cout << "       =opt                            - Model-guided algorithm opt" << endl;
            cout << "       =one                            - Core-guided algorithm one (default)" << endl;
            cout << "       =pmres                          - Core-guided algorithm pmres" << endl;
            cout << "       =k                              - Core-guided algorithm k" << endl;
            cout << "--k-threshold                          - Initialization for algorithm k. Must be >= 0" << endl;
            cout << "       =0                              - Special case for a dynamic choice of k threshold" << endl;
            cout << "       =1                              - Special case for algorithm one with cardinality constraints" << endl;
            cout << "--enable-disjcores                     - Enable disjoint cores preprocessing (only for core-guided)" << endl;
            cout << "--trim-core                            - Enable trim of unsat cores (only for core-guided)" << endl;
            cout << "--disable-stratification               - Disable stratification (only for core-guided)" << endl;
            cout << "--compute-firstmodel=budget            - Compute a model before starting the algorithm. In addition, budget may specify the maximum number of seconds allowed" << endl;
            cout << "--shrinking-strategy                   - Expensive minimization of unsat cores (only for core-guided)" << endl;
            cout << "       =progression                    - Use progression" << endl;
            cout << "       =linearsearch                   - Use linear search" << endl;
            cout << "--shrinking-budget=budget              - Limit the running time (in seconds) of the solver during the minimization" << endl;

            cout << separator << endl;
            cout << "Query options                          " << endl << endl;
            cout << "--query-algorithm                      - Compute cautious consequences according to the specified algorithm" << endl;
            cout << "       =ict                            - Enable iterative coherence testing" << endl;
            cout << "       =or                             - Enable overestimate reduction" << endl;
            cout << "       =cb                             - Enable core-based algorithm (chunk size or percentage can be also specified)" << endl;
            cout << "       =chunk                          - Enable algorithm based on chunks (Default size=2)" << endl;
            cout << "       =preferences                    - Enable algorithm based on preferences" << endl;
            cout << "       =one                            - Enable algorithm based on algorithm one" << endl;
            cout << "       =kdyn                           - Enable algorithm based on algorithm kdyn" << endl;
            cout << "       =pmres                          - Enable algorithm based on algorithm pmres" << endl;
            cout << "       =pi                             - Enable algorithm based on prime implicate" << endl;
            cout << "       =ict-preferences                - Combines algorithm iterative coherence testing with algorithm based on preferences" << endl;
            cout << "--query-verbosity=(0,...,4)            - Enable verbosity of queries algorithms" << endl;
            cout << "--query-chunk-size=n                   - Specify the size of each chunk" << endl;
            cout << "--query-chunk-percentage=(1,...,100)   - The size of each chunk is a percentage of the number of candidates" << endl;
            cout << "--query-enable-core-cache              - Enable caching of cores for algorithms one, kdyn and pmres" << endl;

            cout << separator << endl;
            cout << "Predicates minimization                " << endl << endl;
            cout << "--minimize-predicates=pred1;...;predn  - Find the answer set subset-minimal w.r.t. atoms over specified predicates" << endl;
            cout << "--minimization-algorithm               - Specify the algorithm to use for the minimization" << endl;
            cout << "       =guess-check                    - Guess an answer set and check the minimality" << endl;
            cout << "       =guess-check-minimize           - Guess an answer set and check the minimality. In case of failure reiterate the check" << endl;
            cout << "       =guess-check-split              - Guess an answer set and check the minimality of each atom" << endl;
            cout << "       =preferences                    - Use the always-false heuristic" << endl;
            cout << "       =core-based                     - Use algorithm based on unsatisfiable cores" << endl;
            cout << "--min-chunk-percentage=(1,...,100)     - Core based algorithm can use chunks. The size of each chunk is a percentage of the number of candidates" << endl;

            cout << separator << endl;
            cout << "Paracoherent semantics                 " << endl << endl;
            cout << "--paracoherent                         - Select paracoherent semantics" << endl;
            cout << "       =semantics,algorithm            - Semantics must be semistable, semiequilibrium or split." << endl;
            cout << "                                       - For semistable and semiequilibrium, algorithm must be gc, minimize, split, opt, or weak (default split)." << endl;
            cout << "                                       - For split, algorithm must be weak." << endl;

            cout << separator << endl;
            cout << "Disjunction options                    " << endl << endl;
            cout << "--disjunction                          - Enable options for handling disjunctive rules" << endl;
            cout << "       =propagator                     - Use propagator for inferences" << endl;
            cout << "       =compv                          - Perform the disjunctive completion" << endl;
            cout << "       =auto                           - Perform an automatic choice between propagator and disjunctive completion" << endl;
            //cout << "--enable-shift-onedef                  - Enable shift also for atoms with one defition" << endl;

            cout << separator << endl;
            cout << "General options                        " << endl << endl;
            cout << "--stats=verbosity                      - Print statistics (verbosity is optional 0=basic stats, 1=more stats, 2=dynamic stats)" << endl;
            cout << "-n                                     - Specify the answers to compute" << endl;
            cout << "       <=0                             - To compute all answers" << endl;
            cout << "       =k                              - To compute at most k answers" << endl;
            cout << "--pre=                                 - Preprocessing and print in different format" << endl;
            cout << "       =lparse                         - Print lparse format" << endl;
            cout << "       =lparse-one-level               - Print lparse format where multi-levels weak constraints are compressed into one single level" << endl;
            cout << "       =edimacs                        - Print extended dimacs" << endl;
            cout << "       =wbo                            - Print wbo format" << endl;
            cout << "--pre-verbosity=N                      - Set to N the value of verbosity of preprocessing" << endl;
            cout << "--dimacs                               - Enable dimacs format as input and outputs" << endl;
            cout << "--wellfounded                          - To compute the wellfounded model." << endl;
            cout << "       =cautious                       - To filter only atoms that are true in the wellfounded model." << endl;            
            cout << "--help                                 - Print this guide and exit" << endl;
            cout << "--disable-simplifications              - Disable the simplifications of satelite" << endl;
            cout << "       =N                              - [Optional] disable simplifications if the number of clauses is >= N" << endl;
            cout << "--dynamic-configuration=m1,m2,k,f,s    - Update heuristics parameters when m1 <= learned/choices <= m2 and choices > k" << endl <<
                    "                                           m1 <= m2, m1, m2 > 0, k > 0" << endl <<
                    "                                           f is the name of the file containing the configuration" << endl <<
                    "                                           s is the strategy to apply if, after the dynamic configuration, learned/choices < m1 or learned/choices > m2 and can be:" << endl <<
                    "                                              keep, to continue with the current configuration" << endl <<
                    "                                              restore, to restore the initial configuration" << endl <<
                    "                                              filename, to apply the configuration specified by the filename" << endl;
            cout << "--enable-multiaggregates               - Share aggregate sets if possible" << endl;
            cout << "--enumeration-strategy                 - Select different strategy for enumeration of answer sets" << endl;
            cout << "       =bt                             - Use backtracking for enumeration" << endl;
            cout << "       =bc                             - Use blocking clause" << endl;
        }
    };

};

#endif /* HELP_H */
