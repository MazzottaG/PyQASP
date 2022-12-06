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

#include "WaspFacade.h"

#include "Restart.h"

#include "Enumeration.h"

#include "preasp/Parser.h"
#include "preasp/Builder.h"
#include "preasp/Completion.h"
#include "preasp/Program.h"
#include "preasp/Options.h"
#include "preasp/output/StreamCreator.h"
#include "preasp/output/GringoOutput.h"
#include "preasp/output/WBOOutput.h"
#include "WaspCreator.h"
#include <zlib.h>

#include "PredicateMinimization.h"
#include "CautiousReasoning.h"
#include "ParacoherentSemantics.h"

void
WaspFacade::readInput(
    istream& )
{
//    char tmp;
//    in >> tmp;
//
//    if( !in.good() && !in.eof() )
//        DLV2::ErrorMessage::errorDuringParsing( "Unexpected symbol." );
//
//    in.putback( tmp );
    char tmp = 'k';
    switch ( tmp )
    {
//        case COMMENT_DIMACS:
//        case FORMULA_INFO_DIMACS:
//        {
//            DimacsOutputBuilder* d = new DimacsOutputBuilder();
//            solver.setOutputBuilder( d );
//            Dimacs dimacs( solver );
//            dimacs.parse();
//            if( dimacs.isMaxsat() )
//                d->setMaxsat();
//            delete outputBuilder;
//            outputBuilder = d;
//            greetings();
//            break;
//        }

        default:
        {
            //GringoNumericFormat gringo( solver );
            //gringo.parse( in );
            gzFile input_stream = gzdopen(0, "rb");
            preasp::StreamBuffer in(input_stream);
            if( DLV2::Options::paracoherentSemantics.size() > 0 ) program.options.disableRuleChecks();
            program.options.setVerbosity(DLV2::Options::preVerbosity);
            preasp::Parser parser(in, *builder);
            program.options.setCompletionMode(DLV2::Options::shiftStrategy);
            program.options.setPrintOption(PREASP_PRINT_WASP);
            statistics( NULL, startParsing() );
            parser.parseGringo();
            statistics( NULL, endParsing() );
            gzclose(input_stream);            
            if(DLV2::Options::pre == PREASP_PRINT_WASP) greetings();
            if( DLV2::Options::paracoherentSemantics.size() > 0 ) {
                paracoherentRewriting();
            }
            break;
        }
    }
}

void
WaspFacade::paracoherentRewriting() {
    if( DLV2::Options::paracoherentSemantics.size() == 0 )
        return;
    assert(DLV2::Options::paracoherentSemantics[0] == "semistable" || DLV2::Options::paracoherentSemantics[0] == "semiequilibrium" || DLV2::Options::paracoherentSemantics[0] == "split");
    ParacoherentSemantics paracoherent;
    paracoherent.rewrite(*builder, DLV2::Options::paracoherentSemantics[0]);
}

void
WaspFacade::solve()
{
    if(runtime_) { DLV2::ErrorMessage::errorGeneric("Calling method solve twice."); return; }
    init();
    if(!ok_) { printIncoherence(); return; }
    if( DLV2::Options::predMinimizationAlgorithm != NO_PREDMINIMIZATION ) { PredicateMinimization p(*this); p.solve(); return; }
    if( DLV2::Options::queryAlgorithm != NO_QUERY ) { CautiousReasoning c(*this); c.solve(); return; }
    runtime_ = true;

    if(disableVE_) solver.disableVariableElimination();
    if(!solver.preprocessing()) { solver.printIncoherence(); return; }

    statistics( &solver, startSolving() );
    solver.onStartingSolver();

    if(weakFacade.numberOfLevels()==0) { Enumeration enumeration(solver); enumeration.solve(); }
    else handleWeakConstraints();    

    return;
}

void WaspFacade::handleWeakConstraints() {    
    unsigned int result = solveWithWeakConstraints();
    switch( result ) {
        case COHERENT:
            cerr << "ERROR";
            break;

        case INCOHERENT:            
            solver.printIncoherence();
            break;

        case OPTIMUM_FOUND:
        default:
            if( DLV2::Options::maxModels > 1 ) {
                solver.unrollToZero();
                solver.clearConflictStatus();
                DLV2::Options::minimizeUnsatCore=false;
                solver.getWaspOutput().printOptimum();
                Enumeration enumeration(solver);                
                enumeration.solve();       
                solver.optimumFound();                
            }
            else
                solver.optimumFound();
            break;
    }
}

void merge(int left,int center,int right,vector<Literal>& literals,vector<uint64_t>& weights)
{
    Literal* auxLiterals = new Literal[right+1];
    uint64_t* auxWeights = new uint64_t[right+1];

    int i, j;
    for(i=center+1; i>left; i--) { auxLiterals[i-1]=literals[i-1]; auxWeights[i-1]=weights[i-1]; }
    for(j=center; j<right; j++) { auxLiterals[right+center-j]=literals[j+1]; auxWeights[right+center-j]=weights[j+1]; }
    for( int k = left; k <= right; k++ ) {
        if(auxWeights[j]>auxWeights[i]) { literals[k]=auxLiterals[j]; weights[k]=auxWeights[j--]; }
        else { literals[k]=auxLiterals[i]; weights[k]=auxWeights[i++]; }
    }

    delete [] auxWeights; delete [] auxLiterals;
}

void mergesort(int left,int right,vector<Literal>& literals,vector<uint64_t>& weights )
{
    if( left < right ) {
        int center = (left+right)/2;
        mergesort(left,center,literals,weights);
        mergesort(center+1,right,literals,weights);
        merge(left,center,right,literals,weights);
    }
}

bool
WaspFacade::addPseudoBooleanConstraint(
    vector<Literal>& lits,
    vector<uint64_t>& weights,
    uint64_t bound)
{
    if(solver.conflictDetected() || !ok_) return false;
    solver.unrollToZero();
    if(lits.size() != weights.size())
        DLV2::ErrorMessage::errorGeneric("The size of literals must be equal to the size of weights in pseudoBoolean constraints.");

    if(bound == 0) return true;
    uint64_t differentWeights = UINT64_MAX;
    uint64_t sumOfWeights = 0;
    unsigned int j = 0;
    for(unsigned int i = 0; i < lits.size(); i++) {
        lits[j]=lits[i];    weights[j]=weights[i];
        addVariables(lits[i].getVariable());
        if(solver.hasBeenEliminated(lits[i].getVariable())) DLV2::ErrorMessage::errorGeneric("Trying to add a deleted variable to aggregate.");
        solver.setFrozen(lits[i].getVariable());
        if(solver.isFalse(lits[i]) || weights[i] == 0) continue;
        if(solver.isTrue(lits[i])) {
            if(bound <= weights[i]) return true;
            bound -= weights[i];
            continue;
        }

        if(weights[i] > bound) { weights[i] = bound; }
        if(differentWeights == UINT64_MAX)
            differentWeights = weights[i];
        else if(differentWeights != weights[i])
            differentWeights = UINT64_MAX-1;
        sumOfWeights += weights[i];
        j++;
    }
    lits.resize(j);    weights.resize(j);
    if(sumOfWeights < bound) { ok_ = false; return false; }
    if(differentWeights < UINT64_MAX-1 && differentWeights >= bound) { return addClause(lits); }
    if(sumOfWeights > bound) mergesort(0, lits.size()-1, lits, weights); //SORT ONLY IF NOT CARDINALITY CONSTRAINT
    Aggregate* aggregate = new Aggregate();
    Literal aggregateLiteral(1, NEGATIVE);
    aggregate->addLiteral(aggregateLiteral.getOppositeLiteral(), 0);
    #ifndef NDEBUG
    uint64_t previousWeight = UINT64_MAX;
    #endif
    for(unsigned int i = 0; i < lits.size(); i++) {
        assert(lits[i].getVariable() <= solver.numberOfVariables());
        assert(solver.isUndefined(lits[i].getVariable()));
        assert(!solver.hasBeenEliminated(lits[i].getVariable()));
        aggregate->addLiteral(lits[i], weights[i]);
        solver.setFrozen(lits[i].getVariable());
        assert_msg(previousWeight >= weights[i], "Literals must be sorted in increasing order");
        assert(previousWeight=weights[i]); //previousWeight is updated
    }
    assert(aggregate->size() >= 1);
    if(!aggregate->updateBound(solver, bound)) { delete aggregate; ok_ = false; return false; }

    solver.attachAggregate(*aggregate);
    solver.addAggregate(aggregate);

    assert(solver.isTrue(aggregateLiteral));
    aggregate->onLiteralFalse(solver,aggregateLiteral.getOppositeLiteral(),1);    

    if(solver.conflictDetected()) { ok_ = false; return false; }
    return ok_;
}

unsigned int
WaspFacade::solve(const vector<Literal>& assumptions, vector<Literal>& conflict)
{
    if(!ok_) { return INCOHERENT; }
    //First call
    if(!runtime_) {
        init();
        runtime_ = true;
        if(disableVE_) solver.disableVariableElimination();
        if(!solver.preprocessing()) { ok_ = false; return INCOHERENT; }
        solver.turnOffSimplifications();
        solver.setComputeUnsatCores(true);
        statistics(&solver, startSolving());
        solver.onStartingSolver();
    }
    conflict.clear();
    solver.unrollToZero();
    vector<Literal> assumptions_;
    for(unsigned int i = 0; i < assumptions.size(); i++) {
        if(solver.hasBeenEliminated(assumptions[i].getVariable())) DLV2::ErrorMessage::errorGeneric("Assumption literal has been eliminated. (Assumptions must be frozen).");
        assumptions_.push_back(assumptions[i]);
    }

    unsigned int res = solver.solve(assumptions_);
    if( res == INCOHERENT ) {
        assert(solver.getUnsatCore() != NULL);
        const Clause& core = *solver.getUnsatCore();
        for(unsigned int i = 0; i < core.size(); i++)
            conflict.push_back(core.getAt(i));
        
        if(DLV2::Options::minimizeUnsatCore && !isChecker_) trimCore(conflict);
        shrinkUnsatCore(conflict);
        if(conflict.size() == 0) ok_ = false;
        solver.unrollToZero();
        solver.clearConflictStatus();
    }
    return res;
}

void
WaspFacade::addAggregate(vector<Literal>& lits, vector<uint64_t>& weights, Var id, uint64_t bound) {
    if(runtime_) DLV2::ErrorMessage::errorGeneric("Aggregates can only be added before search starts.");

    Aggregate* aggregate = new Aggregate();
    assert(lits.size() == weights.size());
    assert(id != 0);
    assert(bound != 0);
    solver.setFrozen(id);
    Literal aggregateLiteral = Literal(id, POSITIVE);

    aggregate->addLiteral(aggregateLiteral.getOppositeLiteral(),0);
    for(unsigned int i=0; i < lits.size(); i++) {
        if(weights[i] > bound) weights[i] = bound;
        assert(solver.isUndefined(lits[i]));
        solver.setFrozen(lits[i].getVariable());
        aggregate->addLiteral(lits[i], weights[i]);
    }
    aggregate->sort();
    solver.attachAggregate(*aggregate);
    assert(aggregate->size() > 0);
    aggregate->updateBound(solver, bound);

    if(solver.isFalse(aggregateLiteral)) aggregate->onLiteralFalse(solver, aggregateLiteral, -1);
    else if(solver.isTrue(aggregateLiteral)) aggregate->onLiteralFalse(solver, aggregateLiteral.getOppositeLiteral(), 1);
    solver.addAggregate(aggregate);
}


void
WaspFacade::addMultiAggregate(vector<Literal>& lits, vector<uint64_t>& weights, vector<Literal>& ids, vector<uint64_t>& bounds, bool addBinaryClauses) {
    if(runtime_) DLV2::ErrorMessage::errorGeneric("Multi aggregates can only be added before search starts.");

    MultiAggregate* multi = new MultiAggregate();
    assert(lits.size() == weights.size());
    assert(ids.size() == bounds.size());
    for(unsigned int i = 0; i < lits.size(); i++) multi->addLiteral(lits[i], weights[i]);
    for(unsigned int i = 0; i < ids.size(); i++) multi->addBound(ids[i], bounds[i]);

    multi->finalize(solver, addBinaryClauses);
    solver.attachMultiAggregate(*multi);
    solver.addMultiAggregate(multi);
}

void
WaspFacade::init() {
    assert(!runtime_);
    if(isChecker_) return;    
    if(DLV2::Options::minisatPolicy) setMinisatPolicy();
    solver.setRestart();
    assert(DLV2::Options::pre == PREASP_PRINT_WASP);
    program.options.setPrintOption(PREASP_PRINT_EXTENDED_DIMACS);
    if(!builder->simplify()) { std::cout << "simplify from init" <<std::endl;ok_ = false; return; }
    builder->computeSCC();
//            preasp::Completion c1(new preasp::StreamCreator(cout));
//            c1.execute(program, false);

    preasp::Completion c(new WaspCreator(*this));
    c.execute(program, true);
}

unsigned int
WaspFacade::printPre() {
    assert(DLV2::Options::pre != PREASP_PRINT_WASP);
    program.options.setPrintOption(DLV2::Options::pre);
    if(!builder->simplify()) {
        ok_ = false;
        switch( program.options.printOption()) {
            case PREASP_PRINT_GRINGO: {
                preasp::GringoOutput g;
                g.printIncoherentProgram(cout);
            }
            break;

            case PREASP_PRINT_GRINGO_ONE_LEVEL: {
                preasp::GringoOutput g(true);
                g.printIncoherentProgram(cout);
            }
            break;

            case PREASP_PRINT_EXTENDED_DIMACS: {
                preasp::StreamCreator s(cout);
                s.printIncoherence();
            }
            break;

            case PREASP_PRINT_WBO: {
                preasp::WBOOutput o(cout);
                o.printIncoherence();
            }
            break;
            
            case PREASP_WELL_FOUNDED_MODEL:
            case PREASP_WELL_FOUNDED_MODEL_CAUTIOUS:                
                //NO BREAK HERE
            default:
            break;
        }
        return 0;
    }
    switch(program.options.printOption()) {
        case PREASP_PRINT_GRINGO: {            
            preasp::GringoOutput g;
            g.printCoherentProgramAndClean(program, cout);
        }
        break;

        case PREASP_PRINT_GRINGO_ONE_LEVEL: {            
            preasp::GringoOutput g(true);
            g.printCoherentProgramAndClean(program, cout);
        }
        break;

        case PREASP_PRINT_EXTENDED_DIMACS: {
            builder->computeSCC();
            preasp::Completion c(new preasp::StreamCreator(cout));
            c.execute(program, true);
        }
        break;

        case PREASP_PRINT_WBO: {
            builder->computeSCC();
            if(!program.tight()) { cerr << "Error: only tight programs are supported for opb/wbo format." << endl; return -1; }
            preasp::Completion c(new preasp::WBOOutput(cout));
            c.execute(program, true);
        }
        break;
        case PREASP_WELL_FOUNDED_MODEL:
        case PREASP_WELL_FOUNDED_MODEL_CAUTIOUS:  
            //NO BREAK HERE
        default:
        break;
    }
    return 0;
}

void WaspFacade::shrinkUnsatCore(vector<Literal>& unsatCore) {
    if(unsatCore.size() <= 1 || isChecker_) return;
    switch(DLV2::Options::minimizationStrategy) {
        case MINIMIZATION_PROGRESSION:
            minimizeUnsatCoreWithProgression(unsatCore);
            return;
            
        case MINIMIZATION_LINEARSEARCH:
            minimizeUnsatCoreWithLinearSearch(unsatCore);
            return;
    }
}

void WaspFacade::minimizeUnsatCoreWithProgression(vector<Literal>& unsatCore) {
    unsigned int max = 1;    
    unsigned int otherMax = 1;
    
    begin:;
    vector<Literal> assumptions;
    vector<Literal> conflict;
    for(unsigned int i = 0; i < max && i < unsatCore.size(); i++) assumptions.push_back(unsatCore[i]);    

    setBudget(BUDGET_TIME,DLV2::Options::minimizationBudget);
    isChecker_ = true; //DISABLE MINIMIZATION
    unsigned int result = solve(assumptions, conflict);
    isChecker_ = false;
    budgetOff();
    if(result == INCOHERENT) { assert(conflict.size() <= unsatCore.size()); unsatCore.swap(conflict); return; }
    
    if(max+otherMax > unsatCore.size()) otherMax=1;
    max += otherMax;
    otherMax *= 2;
    if(max >= unsatCore.size()) return;    
    goto begin;
}

void WaspFacade::minimizeUnsatCoreWithLinearSearch(vector<Literal>& unsatCore) {
    unsigned int max = 1;    
    
    begin:;
    vector<Literal> assumptions;
    vector<Literal> conflict;
    for(unsigned int i = 0; i < max && i < unsatCore.size(); i++) assumptions.push_back(unsatCore[i]);    

    setBudget(BUDGET_TIME,DLV2::Options::minimizationBudget);
    isChecker_ = true;
    unsigned int result = solve(assumptions, conflict);
    isChecker_ = false;
    budgetOff();
    if(result == INCOHERENT) { unsatCore.swap(conflict); return; }
    
    max++;
    if(max >= unsatCore.size()) return;    
    goto begin;    
}

void WaspFacade::trimCore(vector<Literal>& unsatCore) {
    unsigned int cho = solver.getMaxNumberOfChoices();
    unsigned int res = solver.getMaxNumberOfRestarts();
    unsigned int sec = solver.getMaxNumberOfSeconds();
    
    budgetOff();
    vector<Literal> conflict;    
    begin:;    
    if(unsatCore.size() <= 1 || conflict.size()==unsatCore.size()) { setBudget(BUDGET_CHOICES, cho); setBudget(BUDGET_RESTARTS, res); setBudget(BUDGET_TIME, sec); return; }    
    vector<Literal> localAssumptions;
    for(unsigned int i = 0; i < unsatCore.size(); i++) localAssumptions.push_back(unsatCore[i]);    
    
    isChecker_ = true;
    #ifndef NDEBUG
    unsigned int result =
    #endif
    solve(localAssumptions, conflict);
    isChecker_ = false;
    assert(result==INCOHERENT);
    assert(conflict.size() <= unsatCore.size());
    if(conflict.size() < unsatCore.size()) unsatCore.swap(conflict);
    goto begin;
}