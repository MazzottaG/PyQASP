from pyqasp.grounder import *
from pyqasp.Option import FILE_UTIL,QASP_FORMAT,DEFAULT_PROPERTIES,Debugger,DebugCommand
from pyqasp.Solver import *
from pyqasp.SubProgramParser import *
import argparse,signal,subprocess,json,sys
import tempfile
import shutil


class PyQASPSolver:
    GROUNDER="gringo"
    def __init__(self,filename,tempdir=None):
        ExternalCalls.LOG_FILE_HANDLER = None
        DEFAULT_PROPERTIES.NO_WF = True
        DEFAULT_PROPERTIES.SATISFIABILITY = False
        ExternalCalls.LOG_FILE_HANDLER = open(FILE_UTIL.LOG_ERROR,"w")
        ExternalCalls.debugger = DEFAULT_PROPERTIES.debug
        ExternalCalls.debuggercmd = DEFAULT_PROPERTIES.debugcmd
        self.working_dir = tempfile.mkdtemp() if tempdir is None else tempdir
        self.set_working_dir()
        self.filename = filename
        self.parser = None
        self.symbols = None
        self.solver = None
        self.isFirstForall=None
        self.props = None
        self.closed = False
        self.choice_rule = ""
        self.shot_1_qbf = FILE_UTIL.QBF_PROGRAM_FILE
        self.shot_2_qbf = FILE_UTIL.WORKING_QBF_PROGRAM_FILE
        self.read_from_shot_1 = True
    
    def set_working_dir(self):
        FILE_UTIL.rename(self.working_dir)
    
    def compute_clauses(self):
        clauses = []

        process = ExternalCalls.callGroundingPipeline(FILE_UTIL.TO_GROUND_PROGRAM_FILE,False,True)
        line = process.stdout.readline().decode("UTF-8").strip()
        mapping = {}
        new_atoms = []
        while line:
            if line.startswith("c"):
                prefix, local_var, name = line.split(" ")
                id_,lev,_,added = self.symbols.addSymbol(name,1)
                if added:
                    new_atoms.append(id_)   
                mapping[int(local_var)]=id_
            elif line.startswith("p"):
                pass
            else:
                lits = line.split(" ")[:-1]
                clause = []
                for lit in lits:
                    lit = int(lit)
                    negated=lit<0
                    var_lit = lit if not negated else -lit
                    if var_lit not in mapping:
                        var = self.symbols.addExtraSymbol()
                        mapping[var_lit] = var
                        new_atoms.append(var)

                    clause.append(mapping[var_lit] if not negated else -mapping[var_lit])
                clauses.append(clause)
            line = process.stdout.readline().decode("UTF-8").strip()
        process.communicate()
        return clauses,new_atoms
    
    def update_qbf(self,clauses,new_atoms):
        first_cnf = True
        first_exist = True
        with open(FILE_UTIL.QBF_PROGRAM_FILE if self.read_from_shot_1 else FILE_UTIL.WORKING_QBF_PROGRAM_FILE,"r") as f:
            with open(FILE_UTIL.QBF_PROGRAM_FILE if not self.read_from_shot_1 else FILE_UTIL.WORKING_QBF_PROGRAM_FILE,"w") as g:
                for line in f:
                    line = line.strip()
                    if "exists(" in line and first_exist:
                        first_exist=False
                        fresh_vars = ",".join([str(var) for var in new_atoms])
                        updated_exists = line[:-1]+","+fresh_vars+")"
                        print(updated_exists,file=g)

                    elif " and(" in line and first_cnf:
                        update_cnf = line[:-1]
                        extra_gate = ",".join([str(self.symbols.getCounter()+i) for i in range(len(clauses))])
                        update_cnf = update_cnf + "," + extra_gate + ")"
                        print(update_cnf,file=g)
                        first_cnf=False
                    else:
                        print(line,file=g)

                for i in range(len(clauses)):
                    gate = self.symbols.addExtraSymbol()
                    clause = ",".join([str(lit) for lit in clauses[i]])
                    print(f"{gate} = or({clause})",file=g)
        self.read_from_shot_1 = not self.read_from_shot_1
        if self.read_from_shot_1:
            self.solver.set_qbf_file(self.shot_1_qbf)
        else:
            self.solver.set_qbf_file(self.shot_2_qbf)
        
    def add_rules_to_first_program(self,program_rules):
        self.set_working_dir()
        with open(FILE_UTIL.TO_GROUND_PROGRAM_FILE,"w") as g:
            print(self.choice_rule,file=g)
            print(program_rules,file=g)
        clauses,new_atoms = self.compute_clauses()
        self.update_qbf(clauses,new_atoms)

    def ground(self):
        self.set_working_dir()
        if self.closed:
            raise Exception("Solver is closed cannot be used any more")
        self.parser = SubProgramParser(self.filename,PyQASPSolver.GROUNDER)
        props,aspstats = self.parser.buildSubPrograms()
        self.props = props
        self.symbols=self.parser.symbols
        self.isFirstForall=self.parser.encodedLevel[1] in [self.parser.ENCODED_F,self.parser.SKIPPED]
        self.solver = QuabsShot()
        first_program_atoms = []
        factory = self.symbols.getFactory()
        for predicate in factory:
            predicate_set = factory[predicate]
            first_program_atoms += [atom for atom in predicate_set if predicate_set[atom][1] == 1]
        self.choice_rule = "{"+";".join(first_program_atoms)+"}."
        
    def get_choice_rule(self):
        return self.choice_rule
    
    def solve(self,assumption):
        self.set_working_dir()
        if self.closed:
            raise Exception("Solver is closed cannot be used any more")
        if self.solver is None:
            raise Exception("Program has not been grounded yet")
        return self.solver.solve(self.symbols,self.isFirstForall,self.props,assumption)
    
    def close(self):
        shutil.rmtree(self.working_dir)
        self.closed=True

