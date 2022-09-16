from Structures import SymbolTable
from Builder import QCIRBuilder
from grounder import *
from Option import FILE_UTIL,REGEX_UTIL,QASP_FORMAT,ASP_PARSER_FORMAT
import subprocess
import time
from antlr4 import *
from parsing.ASPCore2Lexer import ASPCore2Lexer
from parsing.ASPCore2Parser import ASPCore2Parser
from parsing.ASPCore2Listener import ASPCore2Listener
import scc,re

class QASPParser:
    FACT_REGEX = r'^([a-z][a-zA-Z0-9]*)\(.*\)\.'
    PROPOSITION_CHOICE_REGEX = r'^\{([a-z][a-zA-Z0-9]*(;[a-z][a-zA-Z0-9]*)*)\}\.'
    def __init__(self,filename,symbolTable: SymbolTable,grounder):
        self.qasp_filename = filename
        self.symbols = symbolTable
        self.qcirBuilder = QCIRBuilder(self.symbols)
        self.grounder = grounder
        self.currentDomainFacts = []
        self.currentTightProgram = None
        self.listener = ASPCore2Listener(ASPCore2Parser.NAF)
        self.program=[]
        self.threshold=100

    def parseRule(self,rule):
        if len(rule.strip())>0:

            match = re.match(QASPParser.PROPOSITION_CHOICE_REGEX,rule)
            if match:
                for prop_atom in match.group(1).split(";"):
                    self.listener.addPredicate(prop_atom,True)
                return

            match = re.match(QASPParser.FACT_REGEX,rule)
            if match:
                self.listener.addPredicate(match.group(1),True)
                return

            self.program.append(rule)
            if len(self.program) == self.threshold:
                self.parseProgram()
                

    
    def parseProgram(self):
        if len(self.program) > 0:
            input_stream = InputStream("\n".join(self.program))
            lexer = ASPCore2Lexer(input_stream)
            stream = CommonTokenStream(lexer)
            parser = ASPCore2Parser(stream)
            parser.addParseListener(self.listener)
            parser.program()
            self.program = []

    def addDomainsInternal(self,fileHandler):
        currentPredicates=[]
        self.checkTight()
        for predicate in self.listener.predicatesToId:
            currentPredicates.append(predicate)
            domain = self.symbols.getPredicateDomain(predicate)

            if predicate in self.listener.headPredicates and len(domain)>0:
                print(f"Error: {predicate} already defined")
                sys.exit(180)

            for atom,data in domain:
                var,level = data
                truth = self.symbols.getTruthValue(var)
                if truth == SymbolTable.TRUE:
                    fileHandler.write(f"{atom}.\n")
                    while len(self.currentDomainFacts)<=var:
                        self.currentDomainFacts.append(False)
                    self.currentDomainFacts[var] = True
                elif truth == SymbolTable.UNDEF:
                    fileHandler.write("{"+atom+"}.\n")
        self.listener.reset()
        return currentPredicates
    
    def checkTight(self):
        self.currentTightProgram = True
        vertices=range(1,self.listener.predicateCount+1)
        for i in vertices:
            if i not in self.listener.graph:
                self.listener.graph[i]=[]
        for component in scc.strongly_connected_components_path(vertices,self.listener.graph):
            pred = list(component)[0]
            if len(component)>1 or pred in self.listener.graph[pred]:
                self.currentTightProgram = False
                return
        # print("Tight:",self.currentTightProgram)

    def transform(self,level,quantifier,predicates):
    
        self.grounder.reset()
        self.grounder.getProps().setTight(self.currentTightProgram)
        self.grounder.ground()
        if not self.grounder.prop.isCoherent():
            if quantifier != QASP_FORMAT.QFORALL:
                self.qcirBuilder.addFalsum()
            return False
        self.qcirBuilder.addCnf(self.grounder.getWellFounded(),self.grounder.getProps(),level,quantifier,self.currentDomainFacts,predicates)
        return True    

    def getQcirProps(self):
        return self.qcirBuilder.getProps()
    
    def parse(self):
        print(f"Parsing {self.qasp_filename} ...")

        fileHandler = open(self.qasp_filename,"r")
        
        toGroundFileHandler = None
        currentQuantifier=None
        level = 0
        stop = False
        for line in fileHandler:
            match = re.match(REGEX_UTIL.QASP_QUANTIFIER,line)
            if match:
                newQuantifier = match.group(1)
                if currentQuantifier != None:
                    if currentQuantifier == QASP_FORMAT.QCONSTRAINT:
                        print("Error: found new quantifier after constraint statement")
                        exit(180)
                    if currentQuantifier == newQuantifier:
                        continue
                    
                    self.parseProgram()
                    # print("Encoding Level",level)
                    predicates = self.addDomainsInternal(toGroundFileHandler)
                    toGroundFileHandler.close()
                    if not self.transform(level,currentQuantifier,predicates):
                        stop=True
                        break
                    level+=1
                toGroundFileHandler = open(FILE_UTIL.TO_GROUND_PROGRAM_FILE,"w")
                currentQuantifier=newQuantifier
                self.currentDomainFacts=[]
                self.currentTightProgram = None
                
            elif currentQuantifier != None:
                self.parseRule(line)
                toGroundFileHandler.write(f"{line}")
            else:
                print(f"Skipping not quantifed rule {line}")

            
        if not stop:
            if currentQuantifier == QASP_FORMAT.QCONSTRAINT:
                self.parseProgram()
                print("Encoding Level",level)
                predicates=self.addDomainsInternal(toGroundFileHandler)
                toGroundFileHandler.close()
                self.transform(level,currentQuantifier,predicates)
                self.currentDomainFacts=[]
                self.currentTightProgram = None
            else:
                print("Error: Missing Constraint")
                sys.exit(180)

        fileHandler.close()
        self.qcirBuilder.finalize()
    
    def isFirstForall(self):
        return self.qcirBuilder.isFirstForall()


    def addDomainsExternal(self,fileHandler):
        fileHandler.close()
        stdout = ExternalCalls.callProgramParser()
        fileHandler = open(FILE_UTIL.TO_GROUND_PROGRAM_FILE,"a")
        line = stdout.readline().decode("UTF-8").strip()
        currentPredicates=[]
        while line:
            prefix, predicate = line.split(ASP_PARSER_FORMAT.SEPARATOR)
            if int(prefix) == ASP_PARSER_FORMAT.TIGHT_PROP_PREFIX:
                self.currentTightProgram = bool(int(predicate))
            else:
                currentPredicates.append(predicate)
                domain = self.symbols.getPredicateDomain(predicate)
                if int(prefix) == ASP_PARSER_FORMAT.HEAD_PREDICATE_PREFIX and len(domain)>0:
                    print(f"Error: {predicate} already defined")
                    sys.exit(180)

                for atom,data in domain:
                    var,level = data
                    truth = self.symbols.getTruthValue(var)
                    if truth == SymbolTable.TRUE:
                        fileHandler.write(f"{atom}.\n")
                        while len(self.currentDomainFacts)<=var:
                            self.currentDomainFacts.append(False)
                        self.currentDomainFacts[var] = True
                    elif truth == SymbolTable.UNDEF:
                        fileHandler.write("{"+atom+"}.\n")
            line = stdout.readline().decode("UTF-8").strip()
        return currentPredicates
