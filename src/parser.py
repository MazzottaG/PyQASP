import re
import subprocess
import os,sys,argparse
import logging

from Executors import ExternalCalls
from Structures import SymbolTable,QCIRProgram,Quantifier
from checker import CheckerCNF

class Parser:

    quantifiersMatching = r'\s*%@(forall|exists|constraint)\s*\n'
    dimacsCommentMatch = r'^c\s*(\d+)\s*(.*)\n'
    dimacsHeaderMatch = r'^p\s(wcnf|cnf)\s*\d+\s*\d*\s*\d*\n'
    temporaryFilename=os.path.join(sys._MEIPASS,"resources","files","tmp.lp")
    qbfFile=os.path.join(sys._MEIPASS,"resources","files","formula.qbf")
    DISJUNCTIVE_OPTION="--disjunctive"

    def __init__(self, filename):
        self.QASP_program = filename
        self.disjunctive = False
        self.table=SymbolTable()
        self.qbfFormula=QCIRProgram(Parser.qbfFile)

    def getPredicates(self,outstream):
        line=outstream.readline()
        predicates=set()
        self.disjunctive=False
        while line:
            predicate = line.decode("UTF-8").rstrip()
            if Parser.DISJUNCTIVE_OPTION not in predicate:
                predicates.add(predicate)
            else:
                self.disjunctive=True
            line=outstream.readline()
        logging.info(f"\tCurrent program predicate: {predicates}")
        logging.info(f"\tDisjunctive: {self.disjunctive}")
            
        return predicates

    def computeASPProgram(self,currentRules):
        #get predicate names in currentRules
        self.saveProgramOnTmpFile(currentRules)
        predicates = self.getPredicates(ExternalCalls.callProgramParser(self.temporaryFilename))
        choicerule = "{"
        countDomainPredicate=0
        #adding existing predicate's domain
        for predicate in predicates:
            for atom in self.table.getPredicateDomain(predicate):
                if countDomainPredicate>0:
                   choicerule+=";"
                choicerule+=atom
                countDomainPredicate+=1
        choicerule+="}.\n"
        logging.info(f"\tAdded previous domain choice: {choicerule}")
        return currentRules if countDomainPredicate == 0 else currentRules+choicerule

    def saveProgramOnTmpFile(self,program):
        f=open(Parser.temporaryFilename,"w")
        f.write(program)
        f.close()

    def remapCNF(self,outstream,currentQuantifier):
        streamline=outstream.readline()
        substitution_function={}
        quantifier=Quantifier(currentQuantifier)
        self.qbfFormula.buildCNF()
        variables=""
        logging.info(f"\tRemapping CNF")

        while streamline:
            line = streamline.decode("UTF-8")
            # check whether current line is a comment containing mapping from asp symbol to sat variable
            match = re.match(Parser.dimacsCommentMatch,line)
            if match:
                sat_variable = int(match.group(1))
                atom = match.group(2)
                #returns new id if it is a new atoms otherwise already assigned id
                variable,new = self.table.addSymbol(atom)
                #sat_variable has to be replaced with id stored in the symbol table (it could be also the same)
                substitution_function[sat_variable]=variable
                if new:
                    #adding new variable to quantified variables
                    variables+=str(variable)+","
            else:
                match = re.match(Parser.dimacsHeaderMatch,line)
                if not match:
                    #line is neither cnf format header neither a comment so it is a clause
                    #computing new clause according to variable substitution
                    buildClause=""
                    for lit in line.split(" "):
                        id_ = int(lit)
                        if id_ == 0:
                            #skipping 0 delimiter in clauses
                            continue
                        currentVar = id_ if id_ >= 0 else -id_
                        if currentVar not in substitution_function:
                            #auxiliary sat variable introduced by lp2sat and so we generate a new id for it
                            new_symbol=self.table.addExtraSymbol()
                            substitution_function[currentVar]=new_symbol
                            variables+=str(new_symbol)+","
                        
                        # substitution_function is defined forall symbols seen so far 
                        trueid = substitution_function[currentVar] if id_ >= 0 else -substitution_function[currentVar] 
                        buildClause += ","+str(trueid) if len(buildClause)>0 else str(trueid)

                    # if buildClause!="":
                    #     debugf.write(buildClause+"\n")
                    #     self.checker.addClause(line,buildClause)
                    # else:
                    #     warnings.warn(f"Warning: built empty clause")
                    #for each clause an or gate is added to final qbf formula
                    
                    self.qbfFormula.addOrGate(buildClause,self.table)

            streamline=outstream.readline()

        #finalizing current cnf
        if len(variables)>0:
            variables=variables[:-1]

        self.qbfFormula.builtCNF(self.table,quantifier,variables)

    def computeCNF(self,program,currentQuantifier):
        self.saveProgramOnTmpFile(program)
        self.remapCNF(ExternalCalls.callGringoLp2Sat(Parser.temporaryFilename,self.disjunctive),currentQuantifier)

    def encodeProgram(self,currentRules,currentQuantifier):
        ASP_program = self.computeASPProgram(currentRules)
        self.computeCNF(ASP_program,currentQuantifier)

    def parse(self):
        print(f"Parsing {self.QASP_program} ...")
        f=open(self.QASP_program,"r")
        currentQuantifier=None
        currentRules=""
        programLevel=1
        for line in f:
            match = re.match(Parser.quantifiersMatching,line)
            if match:
                if currentQuantifier!=None:
                    if currentQuantifier == Quantifier.QConstraint:
                        print("Error: found new quantifier after constraint statement")
                        exit(180)
                    logging.info(f"\tEncoding {currentQuantifier} program at level {programLevel}")
                    programLevel+=1
                    self.encodeProgram(currentRules,currentQuantifier)
                currentQuantifier=match.group(1)
                currentRules=""
            else:
                if currentQuantifier != None:
                    currentRules+=line

        #should it be necessarely a contraint ?
        if currentQuantifier != None:
            if currentQuantifier != Quantifier.QConstraint:
                print("Error: last quantified statement is not a constraint")
                exit(180)
            logging.info(f"\tEncoding {currentQuantifier} program at level {programLevel}")
            programLevel+=1
            self.encodeProgram(currentRules,currentQuantifier)
        f.close()
        #finalize qcir file
        logging.info(f"\tFinalizing QBF formula")
        self.qbfFormula.close(self.table)

# error in warning in info in debug

parser = argparse.ArgumentParser(description='QBF encoder')
parser.add_argument('filename', metavar='file', type=str, help='Path to QASP file')
parser.add_argument('-v','--verbose', dest="debuglevel",  type=int, help='verbosity levels: 1 or 2')
parser.add_argument('-err','--errorfile', dest="logfile",  type=str, help='Path of file used for collecting stderr')
parser.add_argument('-s','--solver', dest="solvername",  type=str, help='available solvers : '+str(list(ExternalCalls.qbf_solvers.keys())))
parser.add_argument('-pq','--print-qcir', dest="qcir_file",  type=str, help='output qcir filename')

ns = parser.parse_args()

qasp_file = ns.filename

if ns.qcir_file:
    Parser.qbfFile=ns.qcir_file

solver = "quabs"
if ns.solvername:
    solver = ns.solvername
LEVELS = {1:logging.INFO,2:logging.DEBUG}
DEBUG_LEVEL = logging.WARNING
if ns.debuglevel:
    DEBUG_LEVEL = LEVELS[ns.debuglevel]
logging.basicConfig(format='%(message)s',level=DEBUG_LEVEL)
if ns.logfile:
    ExternalCalls.error_log_file=ns.logfile

p=Parser(qasp_file)
p.parse()
print(f"Start solving with {solver} ...")
outstream = ExternalCalls.callSolver(Parser.qbfFile,solver)
if not outstream is None: 
    line = outstream.readline()
    unsat=False
    while line:
        line = line.decode("utf-8").rstrip()

        if line.startswith("V"):
            model =[int(v) for v in line.split(" ")[1:-1]] 
            qasp_models=[]

            for name,predSet in p.table.factory.items():
                for atom,var in predSet.items():
                    if var in model:
                        qasp_models.append(atom)
            print("Answer:")
            print(". ".join(qasp_models))

        elif line.endswith("UNSAT"):
            unsat=True

        line = outstream.readline()
    
    if unsat:
        print("UNSATISFIABLE")
    else:
        print("SATISFIABLE")

    