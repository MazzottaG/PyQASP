//============================================================================
// Name        : 1.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdlib>
#include <csignal>
#include "common/DLV2Facade.h"
#include "common/Options.h"
#include "wasp/util/Statistics.h"
#include "wasp/preasp/Options.h"

using namespace std;

int EXIT_CODE = 0;

DLV2::DLV2Facade* dlv2FacadePointer = NULL;

void my_handler( int )
{
    cerr << "Killed: Bye!" << endl;
    if(EXIT_CODE == 0) EXIT_CODE = 1;
    dlv2FacadePointer->onKill();
    delete dlv2FacadePointer;
    Statistics::clean();
    exit(EXIT_CODE);
}

int useWasp(DLV2::DLV2Facade& dlv2Facade) {
    if(DLV2::Options::mode == MODE_IDLV) return -1;
    if(DLV2::Options::pre != PREASP_PRINT_WASP) return dlv2Facade.printPreprocessing();
    if(DLV2::Options::mode != MODE_WASP) dlv2Facade.checkParacoherent();   
    dlv2Facade.solve();
    return -1;
}


int useIDLV(DLV2::DLV2Facade& dlv2Facade) {
    if(DLV2::Options::mode != MODE_IDLV) return -1;
    if(dlv2Facade.ground()) return 0;
    return -1;
}

int main(int argc, char** argv)
{
    dlv2FacadePointer = new DLV2::DLV2Facade();
    DLV2::DLV2Facade& dlv2Facade = *dlv2FacadePointer;
    dlv2Facade.parseOptions(argc,argv);    
    dlv2Facade.greetings();
    signal(SIGINT, my_handler);
    signal(SIGTERM, my_handler);
    #ifndef _WIN32
        signal(SIGXCPU, my_handler);
    #else
        signal(24, my_handler);
    #endif

        dlv2Facade.init();
    int parserExit = 0;
    parserExit = DLV2::Options::mode != MODE_WASP ? dlv2Facade.readInput() : dlv2Facade.readWaspInput();
    if(parserExit == 0) {
        int res = -1;        
        if(DLV2::Options::mode == MODE_IDLV) { res=useIDLV(dlv2Facade); }        
        else if(DLV2::Options::mode == MODE_WASP || dlv2Facade.ground()) res = useWasp(dlv2Facade);

        if(res != -1) EXIT_CODE = res;
        dlv2Facade.onFinish();
    }
    else {
        EXIT_CODE = parserExit;
    }
    delete dlv2FacadePointer;
    Statistics::clean();
    return EXIT_CODE;
}
