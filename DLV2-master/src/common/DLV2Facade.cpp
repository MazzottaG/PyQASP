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

#include "DLV2Facade.h"
#include "Assert.h"
#include "ErrorMessage.h"
#include "../idlv/input/SelectorBuilder.h"
#include "../idlv/input/Buffer.h"
#include "../idlv/input/EmptyInputBuilder.h"
#include "../idlv/input/PrinterInputBuilder.h"
#include "../idlv/input/InMemoryInputBuilder.h"
#include "../idlv/grounder/ground/BackTrackingGrounder.h"
#include "../idlv/grounder/ground/BackJumpingGrounder.h"
#include "../idlv/directives/DirectivesHolder.h"
#include "../idlv/grounder/ground/PropositionalGrounder.h"
#include "../idlv/grounder/output/PreaspOutputBuilder.h"


//extern Buffer theBuffer;

#include <cstdio>
#include <ctime>
#include <fstream>
#include <sstream>

using namespace std;
using namespace DLV2;
using namespace DLV2::DB;

void 
DLV2Facade::parseOptions(
    int argc, 
    char* const* argv )
{
    getOptions().init(argc,argv);
    waspFacade.initOptions();
}

void
DLV2Facade::greetings()
{
    if( getOptions().getOutputPolicy() != OUTPUT_SILENT && Options::waspOutputPolicy == WASP_OUTPUT && Options::pre == PREASP_PRINT_WASP && Options::mode == MODE_DLV2)
        cout << DLV2_STRING << endl;
}

int
DLV2Facade::readInput()
{
    if( getOptions().isAspCore2Strict() )
        director.setParserConstraint(new ParserConstraintAspCore2Strict());
        
    switch( getOptions().getInputBuilderPolicy() )
    {

        case BUILDER_IN_MEMORY:
        	builder =new DLV2::grounder::InMemoryInputBuilder();
            break;
            
        case BUILDER_EMPTY:
            builder = new EmptyInputBuilder();
            break;

        default:
            ErrorMessage::errorGeneric( "WARNING: No such builder available!" );
    }

    assert_msg( director.getBuilder() == NULL,
            "Null input-builder, cannot start the parsing process.");
    director.configureBuilder(builder);
           
    clock_t start = clock();
    int error = director.parse(getOptions().getInputFiles());
    parserDuration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

    if(error == 0)
    	director.getBuilder()->onEnd();

    return error;
}

bool DLV2Facade::ground()
{
    if( getOptions().getPrintProgram() )
    {
//        if( getOptions().getInputBuilderPolicy() == BUILDER_SELECTOR )
//        {
//            SelectorBuilder* selectorBuilder = static_cast<SelectorBuilder*>(builder);
//
//            ostringstream s1;
//            s1 << "mkfifo /tmp/wasppipe_" << getOptions().getNamedpipe();
//            system(s1.str().c_str());
//
//            int ecode = selectorBuilder->getSolverToCall();
//
//            if(ecode == 0)
//            {
//                //cerr << "WASP" << endl;
//                cout<<"0"<<endl;
//            }
//            else
//            {
//                //cerr << "DLV" << endl;
//                cout<<"1"<<endl;
//                //system("cat - < /tmp/wasppipe");
//            }
//            cout.flush();
//
//            ostringstream s;
//            s << "/tmp/wasppipe_" << getOptions().getNamedpipe();
//            fstream o(s.str(), std::ios::out);
//            //cerr << "scrivo" << endl;
//            //theBuffer.flushOn(o);
//            o.flush();
//            //cerr << "finito" << endl;
//            o.close();
//
//            //delete selectorBuilder;
//            //free();
//
//            //exit(ecode);
//        }        
            ErrorMessage::errorGeneric( "Not valid solver to print the input program! Bye" );
    }
        
    if( getOptions().getPrintDepGraph() )
    {
            ErrorMessage::errorGeneric( "Not valid solver to print the dependency graph! Bye" );
    }
    
    clock_t start = clock();
    if( getOptions().getInputBuilderPolicy() == BUILDER_IN_MEMORY )
    {             
//    	if(getOptions().isEnabledLpopt()){
//    		LpoptFacade lpopt;
//    		lpopt.decomposeProgram();
//    	}
    	DLV2::grounder::ProgramGrounder *grounder;

        if(DLV2::grounder::PredicateTable::getInstance()->isPropositionalProgram() || !DLV2::grounder::TermTable::getInstance()->containsVariable())

                grounder = new DLV2::grounder::PropositionalGrounder();

        else if(getOptions().getInstantiationProcedure()==BACKTRACKING)

                grounder = new DLV2::grounder::BackTrackingGrounder();

        else
    		grounder =  new DLV2::grounder::BackJumpingGrounder();

    	if(!DLV2::grounder::InMemoryInputBuilder::isFoundASafetyError()){
    		grounder->ground();            
    	    delete grounder;                    
    	}
    	else{
			delete grounder;
			cout<<DLV2::grounder::InMemoryInputBuilder::getSafetyErrorMessage()<<endl;
            idlvTime = ( clock() - start ) / (double) CLOCKS_PER_SEC;
            return false;
//    		throw DLV2::grounder::SafetyExceptionGrounder(DLV2::grounder::InMemoryInputBuilder::getSafetyErrorMessage());
    	}
    }

    if( getOptions().getPrintStatistics() )
    {
        cerr << endl << "***FINAL STATISTICS***" << endl;
        cerr << "Parsing time: " << parserDuration << "s" << endl;        
    }
    idlvTime = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    return true;    
}

void
DLV2Facade::solve() {
    waspInitTime = clock();
    waspFacade.solve();
    waspTime = ( clock() - waspInitTime ) / (double) CLOCKS_PER_SEC;    
}

void
DLV2Facade::init() {
    if(Options::mode == MODE_IDLV && getOptions().getOutputFormat() == OUTPUT_PREASP)
    	DLV2::Options::globalOptions()->setOutputFormat(0);
    else{
    	DLV2::grounder::OutputBuilder* o1 = DLV2::grounder::OutputBuilder::getInstance();
    	DLV2::grounder::PreaspOutputBuilder* o = (DLV2::grounder::PreaspOutputBuilder*)(o1);
    	o->setPreaspBuilder(waspFacade.getBuilder());
        if( DLV2::Options::paracoherentSemantics.size() > 0 ) waspFacade.getProgramOptions().disableRuleChecks();
    }
}

DLV2Facade::~DLV2Facade()
{
    DLV2::grounder::OutputBuilder::freeInstance();
    getOptions().finalizeGlobalOptions();
    if( builder != NULL ) 
        delete builder;
}

void
DLV2Facade::checkParacoherent() {
    if( DLV2::Options::paracoherentSemantics.size() > 0 ) {
        waspFacade.paracoherentRewriting();
    }
}