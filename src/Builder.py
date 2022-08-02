from Option import FILE_UTIL,DIMACS_FORMAT,QCIR_FORMAT,QASP_FORMAT,DEFAULT_PROPERTIES
from Executors import ExternalCalls
from Structures import SymbolTable
import sys

class QCIRBuilder:
    
    def __init__(self,symbolTable:SymbolTable):
        self.subformulas=[]
        self.quantifiers=[]
        self.symbols = symbolTable
        
        self.qbfFileHandler = open(FILE_UTIL.QBF_PROGRAM_FILE,"w")
        self.qbfFileHandler.write(f"{QCIR_FORMAT.HEADER}\n")
        
        self.gatesFileHandler = open(FILE_UTIL.GATES_PROGRAM_FILE,"w")
    
    def addFalsum(self):
        phi = self.symbols.addExtraSymbol()
        self.subformulas.append(phi)
        self.quantifiers.append(QASP_FORMAT.QEXISTS)
        self.gatesFileHandler.write(f"{phi} = or( )\n")
    
    def addVerum(self,quantifier):
        phi = self.symbols.addExtraSymbol()
        self.subformulas.append(phi)
        self.quantifiers.append(quantifier)
        self.gatesFileHandler.write(f"{phi} = and( )\n")
    
    def addCnf(self,wellfounded,properties,level,quantifier,currentDomainFacts):
        
        # current program is coherent 
        if properties.isEmpty() == False:
            builder = CNFBuilder(wellfounded,properties,self.symbols,level,self.gatesFileHandler)
            built = builder.buildCurrentCNF(currentDomainFacts,quantifier == QASP_FORMAT.QCONSTRAINT)
            currentPhi = None
            if built:
                currentPhi = builder.getPhi()
            self.subformulas.append(currentPhi)
            self.quantifiers.append(quantifier)
            quant = QCIR_FORMAT.EXISTS
            if quantifier != QASP_FORMAT.QCONSTRAINT:
                quant = quantifier
            vars_ = ",".join([str(x) for x in builder.getFreshVariables()])
            if len(vars_)>0:
                self.qbfFileHandler.write(f"{quant}({vars_})\n")
        else:
            print("Warning: empty program found at level",level)
            self.addVerum(quantifier)

    def finalize(self):
        out_var = None
        if len(self.subformulas) == 0:
            print("Error: empty program")
            sys.exit(0)
        if len(self.subformulas) == 1:
            out_var = self.subformulas[-1]
            if self.quantifiers[-1] == QASP_FORMAT.QFORALL:
                out_var = -out_var
        else:
            currentGate = self.subformulas[-1]
            for i in range(len(self.subformulas)-2,-1,-1):
                phi = self.subformulas[i]
                if phi is None:
                    continue
                op = QCIR_FORMAT.AND_GATE
                if self.quantifiers[i] == QASP_FORMAT.QFORALL:
                    phi = -phi
                    op = QCIR_FORMAT.OR_GATE

                gate = self.symbols.addExtraSymbol()
                self.gatesFileHandler.write(f"{gate} = {op}({phi},{currentGate})\n")
                currentGate=gate
            out_var = currentGate
        
        self.gatesFileHandler.close()
        self.qbfFileHandler.write(f"{QCIR_FORMAT.OUTPUT}({out_var})\n")
        self.qbfFileHandler.close()
        ExternalCalls.callFileAppend(FILE_UTIL.GATES_PROGRAM_FILE,FILE_UTIL.QBF_PROGRAM_FILE)
    
    def isFirstForall(self):
        return self.quantifiers[0] == QASP_FORMAT.QFORALL
class CNFBuilder:
    
    def __init__(self,wellfounded,properties,symbolTable: SymbolTable,level,fileHandler):
        self.level = level
        self.model=wellfounded
        self.prop=properties
        self.gatesFileHandler = fileHandler
        self.symbolTable = symbolTable
        self.variableMapping={}
        self.gates = []
        self.freshVariables = set()
        self.phi=None
    
    def getFreshVariables(self):
        return self.freshVariables

    def getPhi(self):
        return self.phi

    def addAtom(self,atom):
        var, lev, truth = self.symbolTable.addSymbol(atom,self.level)
        
        if lev == self.level:
            self.freshVariables.add(str(var))

    def readAtom(self,fields):
        sat_var = int(fields[DIMACS_FORMAT.DIMACS_COMMENT_VAR_INDEX])
        var, lev, truth = self.symbolTable.addSymbol(fields[DIMACS_FORMAT.DIMACS_COMMENT_ATOM_INDEX],self.level)
        
        if sat_var not in self.variableMapping:
            self.variableMapping[sat_var]=var
        
        if lev == self.level:
            self.freshVariables.add(str(var))

    def readClause(self,fields,remap=True):
        clause = []
        for lit in fields[:-1]:
            sat_id = int(lit)
            sat_var = sat_id if sat_id>=0 else -sat_id
            sign = 1 if sat_id >= 0 else -1
            if sat_var not in self.variableMapping:
                sym = self.symbolTable.addExtraSymbol()
                self.variableMapping[sat_var]=sym
                self.freshVariables.add(str(sym))

            clause.append(str(sign * self.variableMapping[sat_var]))
        gate = self.symbolTable.addExtraSymbol()
        clause = ",".join(clause)
        self.gatesFileHandler.write(f"{gate} = {QCIR_FORMAT.OR_GATE}({clause})\n")
        self.gates.append(str(gate))
    
    def addUnitClause(self,lit):
        gate = self.symbolTable.addExtraSymbol()
        self.gatesFileHandler.write(f"{gate} = {QCIR_FORMAT.OR_GATE}({str(lit)})\n")
        self.gates.append(str(gate))
    
    def assign(self,atoms,truth,currentDomainFacts):
        for atom in atoms:
            var,level = self.symbolTable.assign(atom,truth,self.level)
            if level == self.level:
                self.freshVariables.add(str(var) if var >=0 else str(-var))
            if truth != self.symbolTable.TRUE or var >= len(currentDomainFacts) or currentDomainFacts[var]==False:
                self.addUnitClause(var)

    def addWellFoundedClauses(self,currentDomainFacts):
        self.assign(self.model.getTrue(),SymbolTable.TRUE,currentDomainFacts)
        self.assign(self.model.getFalse(),SymbolTable.FALSE,currentDomainFacts)

    def buildCurrentCNF(self,currentDomainFacts,isConstraint):
        disjunctive = self.prop.isDisjunctive()
        if disjunctive == None:
            print("Error: disjunctive properties is missing")
            sys.exit(180)
        if DEFAULT_PROPERTIES.ONLY_CHOICE and not isConstraint:
            onlyChoice = self.prop.isOnlyChoice()
            if onlyChoice == None:
                print("Error: onlyChoice properties is missing")
                sys.exit(180)
            else:
                if onlyChoice and self.model.onlyUndef():
                    for atom in self.model.getUndefined():
                        self.addAtom(atom)
                    return False
        
        stdout = ExternalCalls.callPipeline(FILE_UTIL.GROUND_PROGRAM_FILE,disjunctive)
        line=stdout.readline().decode("UTF-8").strip()
        while line:
            fields = line.split(" ")
            if fields[0] != "p":
                if fields[0] == "c":
                    self.readAtom(fields)
                else:
                    self.readClause(fields)
            line=stdout.readline().decode("UTF-8").strip()
        self.addWellFoundedClauses(currentDomainFacts)

        self.phi = self.symbolTable.addExtraSymbol()
        gates = ",".join(self.gates)
        self.gatesFileHandler.write(f"{self.phi} = {QCIR_FORMAT.AND_GATE}({gates})\n")
        return True
