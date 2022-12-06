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
 * File:   Options.h
 * Author: cesco
 *
 * Created on 4 aprile 2014, 12.52
 */

#ifndef IDLVOPTIONS_H
#define	IDLVOPTIONS_H

#include <vector>
#include <string>
#include <unordered_map>
#include "../idlv/util/Constants.h"
#include "../wasp/util/WaspConstants.h"
#include "Trace.h"
#include "../idlv/grounder/atom/Predicate.h"
#include "../idlv/input/ExtAtomFactory.h"

using namespace std;

namespace DLV2
{


    class Options{
    public:

		#ifdef TRACE_ON
			static TraceLevels traceLevels;
		#endif

        void init( int argc, char* const* argv );

        static Options* globalOptions();
        
        static void finalizeGlobalOptions();

        bool isAspCore2Strict() const { return aspCore2Strict; }

        const std::vector<const char*>& getInputFiles() const { return inputFiles; }
        
        bool getPrintProgram() const { return printProgram; }
        
        bool getPrintDepGraph() const { return printDepGraph; }
        
        bool getPrintStatistics() const { return printStatistics; }

        INPUT_BUILDER_POLICY getInputBuilderPolicy() const { return inputPolicy; }

        OUTPUT_POLICY getOutputPolicy() const { return outputPolicy; }

        unsigned getNamedpipe() const { return namedpipe; }
        
        const std::string& getDBDataSource() const { return dbDataSource; }
        
        const std::string& getDBUsername() const { return dbUsername; }
        
        const std::string& getDBPassword() const { return dbPassword; }

        unsigned getIndexType() const {return this->indexType;}

    	void setIndexType(unsigned indexType) {this->indexType=indexType;}

    	unsigned getHashType() const {return this->hashType;}

    	void setHashType(unsigned hashType) {this->hashType = hashType;}

    	const std::string& getPredicatesIndexTerm() const {return predicatesIndexTerm;}

//		void setPredicateIndexTerm(const std::string& predIndexTerm) {this->predicatesIndexTerm = predIndexTerm; this->splitOption(predicatesIndexTerm, predicatesIndexTermMap);}

    	const std::string& getPredicatesIndexType() const {return predicatesIndexType;}

//		void setPredicateIndexType(const std::string& predIndexType) {this->predicatesIndexType = predIndexType; this->splitOption(predicatesIndexType, predicatesIndexTypeMap);}

    	vector<unsigned> getPredicateIndexTerm(grounder::Predicate* predicate);

//    	int getPredicateIndexType(const std::string& predicate);

    	void splitOption();

        bool isNofacts() const {return nofacts;}

		bool isPrintRewrittenProgram() const {return printRewrittenProgram;}

		unsigned getRewritingType() const {	return rewritingType;}

		unsigned getOutputFormat() const {return outputFormat;}

        ~Options() { /*if (instance != NULL) delete instance;*/ }

		unsigned getInstantiationProcedure() const {
			return instantiationProcedure;
		}

		unsigned getPositiveOrderingProcedure() const {
			return positiveOrderingProcedure;
		}

		bool isEnabledDictionaryIntersection() const {
			return enabledDictionaryIntersection;
		}

		void setEnabledDictionaryIntersection(bool enableDictionaryIntersection) {
			this->enabledDictionaryIntersection = enableDictionaryIntersection;
		}

		bool getRuleTime(){
			return ruleTime;
		}

		bool getCheckFactDuplicate(){
			return checkFactDuplicate;
		}

		bool getRewriteArith(){
			return rewriteArith;
		}

		string getPredicateToFilter(){return predicateToFilter;}

		bool isDisabledAnonymousFilter() const {
			return disabledAnonymousFilter;
		}

		void setDisabledAnonymousFilter(bool disabledAnonymousFilter) {
			this->disabledAnonymousFilter = disabledAnonymousFilter;
		}

		unsigned getPrintGroundStats(){
			return printGroundStats;
		}

		bool isCompactFacts() const {
			return compactFacts;
		}

		void setCompactFacts(bool compactFacts) {
			this->compactFacts = compactFacts;
		}

		unsigned getRewProject(){
			return rewProject;
		}

		unsigned getOptionFrontend() const{
			return optionFrontend;
		}

		bool rewriteMagic(){
			return magicRewriting;
		}

		bool isEvaluateQuery(){
			return evaluateQuery;
		}

		const string& getExtFunctionPythonFile(){
			return extFunctionPythonFile;
		}

		unsigned getDecompositionAlgorithm() const {
			return decompositionAlgorithm;
		}

		unsigned getDecompositionFormula() const {
			return decompositionFormula;
		}

		unsigned getDecompositionIterations() const {
			return decompositionIterations;
		}

		unsigned getDecompositionNotImprovementLimit() const {
			return decompositionNotImprovementLimit;
		}

		unsigned int getDecompositionSeed() const {
			return decompositionSeed;
		}

		double getDecompositionThreshold() const {
			return decompositionThreshold;
		}
		bool noStringConversion() const{
			return noStrConversion;
		}

		bool useDefaultExternalAtoms() const{
			return defaultExternalAtoms;
		}
		bool isEnabledDecomposition() const {
			return enabledDecomposition;
		}

        bool isDisabledDecompFitness() const {
            return disabledDecompFitness;
        }


		bool isLookAhead() const {
			return lookAhead;
		}

		bool isEnableExternalAtomsWarnings() const{
			return enableExternalAtomsWarnings;
		}

        static unsigned int maxCost;

        static unsigned int queryAlgorithm;
        static unsigned int queryVerbosity;
        static bool computeFirstModel;
        static unsigned int budget;
        static bool printLastModelOnly;
        static bool printBounds;
        static bool printAtomTable;
        static bool stratification;

        static unsigned int interpreter;
        static char* heuristic_scriptname;
        static vector< string > pluginsFilenames;
        static unsigned int shiftStrategy;
        static string scriptDirectory;

        static bool oneDefShift;

        static bool simplifications;            

        static unsigned int minimizationStrategy;
        static unsigned int minimizationBudget;

        static unsigned int enumerationStrategy;            
        static WEAK_CONSTRAINTS_ALG weakConstraintsAlg;

        static unsigned int kthreshold;

        static unsigned int silent;
        static bool printOnlyOptimum;

        static unsigned int chunkPercentage;
        static unsigned int chunkSize;

        static unsigned int minimizePredicateChunkPercentage;

        static unsigned int modelcheckerAlgorithm;

        static bool compactReasonsForHCC;

        static DELETION_POLICY deletionPolicy;
        static unsigned int deletionThreshold;

        static unsigned int maxModels;

        static OUTPUT_POLICY waspOutputPolicy;

        static bool minisatPolicy;

        static RESTARTS_POLICY restartsPolicy;

        static unsigned int restartsThreshold;

        static unsigned int simplificationsThreshold;            

        static unsigned int timeLimit;                        

        static bool disjCoresPreprocessing;

        static bool minimizeUnsatCore;     

        static bool callPyFinalize;

        static double sizeLBDQueue;
        static double sizeTrailQueue;
        static double K;
        static double R;

        static int nbclausesBeforeReduce;
        static int incReduceDB;
        static int specialIncReduceDB;
        static unsigned int lbLBDFrozenClause;
        static unsigned int lbLBDMinimizingClause;
        static unsigned int lbdKeepClauses;

        static bool stats;

        static unsigned int statsVerbosity;

        static double initVariableIncrement;
        static double initVariableDecay;
        static unsigned int initValue;
        static unsigned int initMinisatHeuristic;
        static unsigned int initSign;

        static bool multiAggregates;
        static bool queryCoreCache;

        static unsigned int predMinimizationAlgorithm;
        static vector< string > predicatesToMinimize;
        
        static vector< string > paracoherentSemantics;
        
        static unsigned int pre;

        static unsigned int preVerbosity;

        static unsigned mode;
        
        static unordered_map< string, WEAK_CONSTRAINTS_ALG > stringToWeak;

        static unordered_map< string, unsigned int > stringToShift;

        static unordered_map< string, unsigned int > stringToMinimization;
        
        static unordered_map< string, unsigned int > stringToParacoherent;

        static unordered_map< string, unsigned int > stringToQueryAlgorithms;

        static unordered_map< string, unsigned int > stringToInitMinisatHeuristic;

        static unordered_map< string, unsigned int > stringToPredMinimization;        

        static WEAK_CONSTRAINTS_ALG getAlgorithm( const string& s );

        static unsigned int getShiftStrategy( const string& s );

        static unsigned int getMinimizationStrategy( const string& s );

        static unsigned int getEnumerationStrategy( const string& s );

        static unsigned int getQueryAlgorithm( const string& s );

        static unsigned int getInitMinisatHeuristic( const string& s );

        static unsigned int getRestartsPolicy( const string& s );

        static bool waspDynamicConfiguration;
        static float waspDynamicConfigurationMin;
        static float waspDynamicConfigurationMax;
        static unsigned int waspDynamicConfigurationK;
        static string waspDynamicConfigurationFile;
        static string waspDynamicConfigurationStrategy;

        static void initMap();       

void setOutputFormat(unsigned outputFormat) {
		this->outputFormat = outputFormat;
	}

	unsigned getDecompLimitSizeBodyFitness() const {
		return decompLimitSizeBodyFitness;
	}

	void setDecompLimitSizeBodyFitness(int decompLimitSizeBodyFitness) {
		this->decompLimitSizeBodyFitness = decompLimitSizeBodyFitness;
	}

    private:

        static Options* instance;

        Options();

        Options(const Options& o);

        void setOutputPolicy(OUTPUT_POLICY outPolicy) { outputPolicy = outPolicy; }

        void setInputBuilderPolicy(INPUT_BUILDER_POLICY inPolicy) { inputPolicy = inPolicy; }

        void setInputFiles(const std::vector<const char*>& inFiles) { inputFiles = inFiles; }

        void setAspCore2Strict(bool strict) { aspCore2Strict = strict;	}

        std::vector< const char* > inputFiles;

        bool aspCore2Strict;
        
        bool printProgram;
        
        bool printDepGraph;
        
        bool printStatistics;
        
        INPUT_BUILDER_POLICY inputPolicy;
        
        OUTPUT_POLICY outputPolicy;
        
        unsigned namedpipe;
        
        std::string dbDataSource;

        std::string dbUsername;
        
        std::string dbPassword;

        unsigned hashType;

        unsigned indexType;

    	std::string predicatesIndexTerm;

    	std::string predicatesIndexType;

    	///The map containing the mapping between a predicate and the position of its indexing term
    	DLV2::grounder::mapIndexingPredicates predicatesIndexTermMap;

    	///The map containing the mapping between a predicate and the kind of index to be used
    	std::unordered_map<std::string, unsigned int> predicatesIndexTypeMap;

    	bool nofacts;

    	bool printRewrittenProgram;

    	unsigned rewritingType;

    	unsigned outputFormat;

    	unsigned instantiationProcedure;

    	unsigned positiveOrderingProcedure;

    	bool enabledDictionaryIntersection;

    	bool ruleTime;

    	bool checkFactDuplicate;

    	bool rewriteArith;

    	bool disabledAnonymousFilter;

    	string predicateToFilter;

    	bool compactFacts;

    	unsigned printGroundStats;

    	unsigned rewProject;

        unsigned optionFrontend;

        bool magicRewriting;

        bool evaluateQuery;

        string extFunctionPythonFile;
	
	grounder::ExtAtomFactory* extAtomFactory;

        bool enabledDecomposition;

        unsigned decompositionAlgorithm;

        unsigned int decompositionSeed;

        unsigned decompositionFormula;

        double decompositionThreshold;

        unsigned decompositionIterations;

        unsigned decompositionNotImprovementLimit;

        bool disabledDecompFitness;

        unsigned decompLimitSizeBodyFitness;

        bool lookAhead;     
        bool defaultExternalAtoms;
		bool noStrConversion;
		bool enableExternalAtomsWarnings;


        static unordered_map< string, RESTARTS_POLICY > stringToRestarts;
    };
    
};

#endif	/* OPTIONS_H */
