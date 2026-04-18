from pyqasp.grounder import *
from pyqasp.Option import FILE_UTIL,QASP_FORMAT,DEFAULT_PROPERTIES,Debugger,DebugCommand
from pyqasp.Solver import *
from pyqasp.SubProgramParser import *
import argparse,signal,subprocess,json,sys
import tempfile
import shutil


class PyQASPSolver:
    GROUNDER="gringo"
    def __init__(self,filename):
        ExternalCalls.LOG_FILE_HANDLER = None
        DEFAULT_PROPERTIES.NO_WF = True
        DEFAULT_PROPERTIES.SATISFIABILITY = False
        ExternalCalls.LOG_FILE_HANDLER = open(FILE_UTIL.LOG_ERROR,"w")
        ExternalCalls.debugger = DEFAULT_PROPERTIES.debug
        ExternalCalls.debuggercmd = DEFAULT_PROPERTIES.debugcmd
        self.working_dir = tempfile.mkdtemp()
        FILE_UTIL.rename(self.working_dir)
        self.filename = filename
        self.parser = None
        self.symbols = None
        self.solver = None
        self.isFirstForall=None
        self.props = None
        self.closed = False
    
    def ground(self):
        if self.closed:
            raise Exception("Solver is closed cannot be used any more")
        self.parser = SubProgramParser(self.filename,PyQASPSolver.GROUNDER)
        props,aspstats = self.parser.buildSubPrograms()
        self.props = props
        self.symbols=self.parser.symbols
        self.isFirstForall=self.parser.encodedLevel[1] in [self.parser.ENCODED_F,self.parser.SKIPPED]
        self.solver = QuabsShot()

    def solve(self,assumption):
        if self.closed:
            raise Exception("Solver is closed cannot be used any more")
        if self.solver is None:
            raise Exception("Program has not been grounded yet")
        return self.solver.solve(self.symbols,self.isFirstForall,self.props,assumption)
    
    def close(self):
        shutil.rmtree(self.working_dir)
        self.closed=True

