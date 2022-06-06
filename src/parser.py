import re,warnings
import subprocess
from Executors import ExternalCalls
from Structures import SymbolTable,QCIRProgram,Quantifier
from checker import CheckerCNF
import os,sys,argparse

class Parser:

    quantifiersMatching = r'\s*%@(forall|exists|constraint)\s*\n'
    dimacsCommentMatch = r'^c\s*(\d+)\s*(.*)\n'
    dimacsHeaderMatch = r'^p\scnf\s*\d+\s*\d*\n'
    temporaryFilename=os.path.join(sys._MEIPASS,"resources","files","tmp.lp")
    # qbfFile=os.path.join(sys._MEIPASS,"resources","files","formula.qbf")
    output_folder="PyQASP-OUT"
    qbfFile=os.path.join(output_folder,"formula.qbf")
    debugCNFFile = os.path.join(sys._MEIPASS,"resources","files","cnf-debug.txt")

    def __init__(self, args):
        self.QASP_program = args.filename
        self.disjunctive = args.disjunctive
        self.table=SymbolTable()
        if not os.path.exists(Parser.output_folder):
            os.mkdir(Parser.output_folder)
        self.qbfFormula=QCIRProgram(Parser.qbfFile)
        # self.checker = CheckerCNF()

    def getPredicates(self,outstream):
        line=outstream.readline()
        predicates=[]
        while line:
            predicate = line.decode("utf-8").rstrip()
            if predicate not in predicates:
                predicates.append(predicate)
            line=outstream.readline()
        return predicates

    def computeASPProgram(self,currentRules):
        #get predicate names in currentRules
        predicates = self.getPredicates(ExternalCalls.callProgramParser(currentRules))
        choicerule = "{"
        countDomainPredicate=0

        #adding existing predicate's domain
        for predicate in predicates:
            for atom in self.table.getPredicateDomain(predicate):
                if countDomainPredicate>0:
                    choicerule+=";"
                choicerule+=atom
                countDomainPredicate+=1
        choicerule+="}."
        return currentRules if countDomainPredicate == 0 else currentRules+choicerule
    def saveProgramOnTmpFile(self,program):
        f=open(Parser.temporaryFilename,"w")
        f.write(program)
        f.close()

    def remapCNF(self,outstream,currentQuantifier):
        streamline=outstream.readline()
        substitution_function={}
        #used to capture last id used by lp2sat
        count=2
        quantifier=Quantifier(currentQuantifier)
        self.qbfFormula.buildCNF()
        debugf=open(Parser.debugCNFFile,"a")
        while streamline:

            line = streamline.decode("utf-8")
            # print(line.rstrip())
            #check whether current line is a comment containing mapping from asp symbol to sat variable
            match = re.match(Parser.dimacsCommentMatch,line)
            if match:
                sat_variable = int(match.group(1))
                atom = match.group(2)
                #returns new id if it is a new atoms or already assigned id
                variable = self.table.addSymbol(atom)
                if variable != sat_variable:
                    #sat_variable has to be replaced id stored in the symbol table
                    substitution_function[sat_variable]=variable
                count+=1
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
                        if (currentVar >= count or currentVar==1) and currentVar not in substitution_function:
                            #auxiliary sat variable introduced by lp2sat and so we generate a new id for it
                            substitution_function[currentVar]=self.table.addExtraSymbol()

                        trueVar = currentVar if currentVar not in substitution_function else substitution_function[currentVar]
                        #variables in the whole cnf will be under current qualifier
                        self.qbfFormula.addQuantifiedVar(trueVar)
                        trueVar = trueVar if id_ >= 0 else -trueVar
                        buildClause += " "+str(trueVar) if buildClause!="" else str(trueVar)
                    # if buildClause!="":
                    #     debugf.write(buildClause+"\n")
                    #     self.checker.addClause(line,buildClause)
                    # else:
                    #     warnings.warn(f"Warning: built empty clause")
                    #for each clause an or gate is added to final qbf formula
                    self.qbfFormula.addOrGate(buildClause,self.table)

            streamline=outstream.readline()
        debugf.close()
        # self.checker.checkAndClear()
        #finalizing current cnf
        self.qbfFormula.builtCNF(self.table,quantifier)

    def computeCNF(self,program,currentQuantifier):
        self.saveProgramOnTmpFile(program)
        self.remapCNF(ExternalCalls.callGringoLp2Sat(Parser.temporaryFilename,self.disjunctive),currentQuantifier)

    def encodeProgram(self,currentRules,currentQuantifier):
        ASP_program = self.computeASPProgram(currentRules)
        self.computeCNF(ASP_program,currentQuantifier)

    def parse(self):
        f=open(self.QASP_program,"r")
        print(f"Parsing {self.QASP_program}")
        currentQuantifier=None
        currentRules=""
        for line in f:
            match = re.match(Parser.quantifiersMatching,line)
            if match:
                if currentQuantifier!=None:
                    if currentQuantifier == Quantifier.QConstraint:
                        print("Error: found new quantifier after constraint statement")
                        exit(180)
                    self.encodeProgram(currentRules,currentQuantifier)
                currentQuantifier=match.group(1)
                currentRules=""
            else:
                if currentQuantifier != None:
                    currentRules+=line.rstrip('\n')

        #should it be necessarely a contraint ?
        if currentQuantifier != None:
            if currentQuantifier != Quantifier.QConstraint:
                print("Error: last quantified statement is not a constraint")
                exit(180)
            self.encodeProgram(currentRules,currentQuantifier)
        f.close()
        #finalize qcir file
        self.qbfFormula.close(self.table)

    def printPrograms(self):
        counter=1
        for p in self.programs:
            print(f"program {counter}")
            print(f"   {p.getQuantifier()}")
            print(f"      {p.getPredicates()}")
            counter+=1



parser = argparse.ArgumentParser(description='QBF encoder')
parser.add_argument('filename', metavar='file', type=str, help='Path to QASP file')
parser.add_argument('-d','--disjunctive', action="store_true", help='sum the integers (default: find the max)')
ns = parser.parse_args(sys.argv[1:])
p=Parser(ns)
p.parse()
print(f"QCIR file built: {Parser.qbfFile}")
ExternalCalls.callQuabs(Parser.qbfFile)