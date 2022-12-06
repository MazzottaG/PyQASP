#include "WaspConfiguration.h"
#include "Solver.h"

#include "../common/Options.h"
#include "../common/ErrorMessage.h"
#include "../common/Utility.h"
#include "util/WaspConstants.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void WaspConfiguration::updateConfiguration() {
    assert(DLV2::Options::waspDynamicConfigurationK != 0);
    if(count > 1 || choices < DLV2::Options::waspDynamicConfigurationK) return;

    assert(choices != 0);
    float ratio = (float) learned/(float) choices;
    float v1 = DLV2::Options::waspDynamicConfigurationMin;
    float v2 = DLV2::Options::waspDynamicConfigurationMax;
    if(count == 0 && ratio >= v1 && ratio <= v2) {
        count++;        
        readConfiguration(DLV2::Options::waspDynamicConfigurationFile);
        return;
    }
    
    if(count == 1 && (ratio < v1 || ratio > v2)) {
        count++;
        if(DLV2::Options::waspDynamicConfigurationStrategy=="keep") return;
        if(DLV2::Options::waspDynamicConfigurationStrategy=="restore") { restoreOriginalParameters(); return; }
        readConfiguration(DLV2::Options::waspDynamicConfigurationStrategy);            
    }    
}

void WaspConfiguration::restoreOriginalParameters() {
    assert(DLV2::Options::waspDynamicConfigurationStrategy=="restore");
    for(int i = 0; i < updatedParameters.size(); i++) {
        string option = updatedParameters[i];
        string value = "";
        if(option=="--init-varinc") value = to_string(DLV2::Options::initVariableIncrement);
        else if(option=="--init-vardecay") value = to_string(DLV2::Options::initVariableDecay);
        else if(option=="--restarts") {
            if(DLV2::Options::restartsPolicy == SEQUENCE_BASED_RESTARTS_POLICY) value="sequence," + to_string(DLV2::Options::restartsThreshold);                
            else if(DLV2::Options::restartsPolicy == GEOMETRIC_RESTARTS_POLICY) value="geometric," + to_string(DLV2::Options::restartsThreshold);
            else if(DLV2::Options::restartsPolicy == GLUCOSE_RESTARTS_POLICY) value="glucose";
            else value = "no";            
        }
        else if(option=="--K") value = to_string(DLV2::Options::K);
        else if(option=="--R") value = to_string(DLV2::Options::R);
        else if(option=="--inc-reduceDB") value = to_string(DLV2::Options::incReduceDB);
        else if(option=="--specialinc-reduceDB") value = to_string(DLV2::Options::specialIncReduceDB);
        else if(option=="--lbd-frozenclause") value = to_string(DLV2::Options::lbLBDFrozenClause);
        else if(option=="--lbd-minclause") value = to_string(DLV2::Options::lbLBDMinimizingClause);
        else if(option=="--lbd-keepclauses") value = to_string(DLV2::Options::lbdKeepClauses);
        assert(value != "");
        setOption(option, value);
    }
    updatedParameters.clear();
}

void WaspConfiguration::readConfiguration(string filename) {
    string line;
    ifstream configurationFile (filename);
    if (!configurationFile.is_open()) {
        DLV2::ErrorMessage::errorGeneric("Unable to open configuration file " + filename);
        return;
    }
        
    while (getline(configurationFile,line)) {
        string l = line;
        Utility::trim(l);
        if(l.size() == 0) continue; 
        vector<string> output;
        Utility::split(l, '=', output);        
        if(output.size() != 2) DLV2::ErrorMessage::errorGeneric("Configuration file is not well-formed, expected --option=value for each line");
        
        Utility::trim_right(output[0]);
        Utility::trim_left(output[1]);
        if(output[0].size() == 0 || output[1].size() == 0) DLV2::ErrorMessage::errorGeneric("Configuration file is not well-formed, expected --option=value for each line");
        if(DLV2::Options::waspDynamicConfigurationStrategy=="restore") updatedParameters.push_back(output[0]);
        setOption(output[0], output[1]);
    }
    configurationFile.close();    
}

void WaspConfiguration::setOption(string option, string value) {
    if(option=="--init-varinc") { solver.choiceHeuristic->resetVariableIncrement(atof(value.c_str())); return; }
    if(option=="--init-vardecay") { solver.choiceHeuristic->resetVariableDecay(1/(double) (atoi(value.c_str())/100)); return; }        
    if(option=="--restarts") {
        vector<string> elems;
        Utility::split(value, ',', elems );
        if(elems.size() > 2) DLV2::ErrorMessage::errorGeneric("Unexpected value for restarts option.");
        solver.restartsPolicy = DLV2::Options::getRestartsPolicy(elems[0]);
        solver.restartsThreshold = elems.size() == 1 ? 0 : atoi(elems[1].c_str());
        if(solver.restartsPolicy == SEQUENCE_BASED_RESTARTS_POLICY) { if(atoi(elems[1].c_str()) < 32) invalidOption("sequence-based restarts", "32"); }
        else if(solver.restartsPolicy == GEOMETRIC_RESTARTS_POLICY) { if(atoi(elems[1].c_str()) < 100) invalidOption("sequence-based restarts", "100"); }
        else if(elems.size() != 1) DLV2::ErrorMessage::errorGeneric( "Unexpected value for restarts option." );
        solver.setRestart();        
        return;    
    }
    if(option=="--K") { solver.glucoseData.K = atof(value.c_str()); return; }
    if(option=="--R") { solver.glucoseData.R = atof(value.c_str()); return; }
    if(option=="--inc-reduceDB") { solver.glucoseData.incReduceDB = atoi(value.c_str()); return; }
    if(option=="--specialinc-reduceDB") { solver.glucoseData.specialIncReduceDB = atoi(value.c_str()); return; }
    if(option=="--lbd-frozenclause") { solver.glucoseData.lbLBDFrozenClause = atoi(value.c_str()); if(DLV2::Options::lbLBDFrozenClause < 2) invalidOption("--lbd-frozenclause", "2"); return; }
    if(option=="--lbd-minclause") { solver.glucoseData.lbLBDMinimizingClause = atoi(value.c_str()); if(DLV2::Options::lbLBDMinimizingClause < 2) invalidOption("--lbd-minclause", "2"); return; }
    if(option=="--lbd-keepclauses") { solver.glucoseData.lbdKeepClauses = atoi(value.c_str()); if(DLV2::Options::lbdKeepClauses < 2) invalidOption("--lbd-keepclauses", "2"); return; }
    DLV2::ErrorMessage::errorGeneric("Unexpected option " + option);
}

void WaspConfiguration::invalidOption(string option, string value) { DLV2::ErrorMessage::errorGeneric("Invalid value for " + option + ". Cannot be less than " + value); }