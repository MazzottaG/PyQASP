from Structures import SymbolTable
from Builder import QCIRBuilder
from grounder import *
from Option import FILE_UTIL,REGEX_UTIL,QASP_FORMAT,ASP_PARSER_FORMAT

class QASPParser:

    def __init__(self,filename,symbolTable: SymbolTable,grounder):
        self.qasp_filename = filename
        self.symbols = symbolTable
        self.qcirBuilder = QCIRBuilder(self.symbols)
        self.grounder = grounder
        self.currentDomainFacts = []
    
    def addDomains(self,fileHandler):
        fileHandler.close()
        stdout = ExternalCalls.callProgramParser()
        fileHandler = open(FILE_UTIL.TO_GROUND_PROGRAM_FILE,"a")
        line = stdout.readline().decode("UTF-8").strip()
        while line:
            prefix, predicate = line.split(ASP_PARSER_FORMAT.SEPARATOR)
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

    def transform(self,level,quantifier):
    
        self.grounder.reset()
        self.grounder.ground()
        if not self.grounder.prop.isCoherent():
            if quantifier != QASP_FORMAT.QFORALL:
                self.qcirBuilder.addFalsum()
            return False
        self.qcirBuilder.addCnf(self.grounder.getWellFounded(),self.grounder.getProps(),level,quantifier,self.currentDomainFacts)
        return True    
    
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

                    self.addDomains(toGroundFileHandler)
                    toGroundFileHandler.close()
                    if not self.transform(level,currentQuantifier):
                        stop=True
                        break
                    level+=1
                toGroundFileHandler = open(FILE_UTIL.TO_GROUND_PROGRAM_FILE,"w")
                currentQuantifier=newQuantifier
                self.currentDomainFacts=[]
                
            elif currentQuantifier != None:
                toGroundFileHandler.write(f"{line}")
            else:
                print(f"Skipping not quantifed rule {line}")

            
        if not stop:
            if currentQuantifier == QASP_FORMAT.QCONSTRAINT:
                self.addDomains(toGroundFileHandler)
                toGroundFileHandler.close()
                self.transform(level,currentQuantifier)
                self.currentDomainFacts=[]
            else:
                print("Error: Missing Constraint")
                sys.exit(180)

        fileHandler.close()
        self.qcirBuilder.finalize()
    
    def isFirstForall(self):
        return self.qcirBuilder.isFirstForall()