from Executors import ExternalCalls
import warnings

class SymbolTable:
    
    def __init__(self):
        self.factory={}
        self.idCounter=2
        
    def addExtraSymbol(self):
        self.idCounter+=1
        return self.idCounter-1

    def addSymbol(self,atom):

        predicate = atom.split("(")[0]
        if predicate not in self.factory:
            self.factory[predicate]={}
        if atom not in self.factory[predicate]:
            self.factory[predicate][atom]=self.idCounter
            self.idCounter+=1
        return self.factory[predicate][atom]

    def getPredicateDomain(self,predicate):
        if predicate in self.factory:
            return self.factory[predicate]
        return {}
    
    def containsPredicate(self,predicate):
        return predicate in self.factory
    
    def getFactory(self):
        return self.factory
    def getCounter(self):
        return self.idCounter

class Quantifier:
    QForall="forall"
    QExists="exists"
    QConstraint="constraint"
    types=[QForall,QExists]
    def __init__(self,quant):
        self.quantifierName=""
        self.isConstraint=False
        if quant == Quantifier.QConstraint:
            quant=Quantifier.QExists
            self.isConstraint=True

        if quant in Quantifier.types:
            self.quantifierName=quant    

class QCIRProgram:

    tempQCIRFile="../resources/files/tmp.qcir"
    AndGate="and"
    OrGate="or"
    def __init__(self,filename):
        self.buildingCNF=False
        self.currentGateIds=""
        self.subformulaIds=[]
        self.quantifiers=[]
        self.quantifiedVars=set()
        self.lastVar=0
        self.previousLastVar=0
        self.equivalenceGates=""
        self.finalQCIRFile=filename
        self.tmpQCIRFileHandler=open(QCIRProgram.tempQCIRFile,"w")
        f=open(self.finalQCIRFile,"w")
        f.write("#QCIR-G14\n")
        f.close()

    def addQuantifiedVar(self,v):
        if v > self.lastVar:
            self.lastVar=v
        if v <= self.previousLastVar:
            return
        self.quantifiedVars.add(v)
    
    def addQuantifier(self,quantifier):
        self.quantifiers.append(quantifier)

    def buildCNF(self):
        self.buildingCNF=True

    def addOrGate(self,clause,table:SymbolTable):

        clause = clause.replace(" ",",")
        gateId = table.addExtraSymbol()
        if self.buildingCNF:
            if self.currentGateIds!="":
                self.currentGateIds+=","
            #storing gate id for each clause of the current cnf
            self.currentGateIds+=str(gateId)
        self.tmpQCIRFileHandler.write(f"{gateId}={QCIRProgram.OrGate}({clause})\n")

    def builtCNF(self,table:SymbolTable,quantifier:Quantifier):
        if self.buildingCNF:
            fi = table.addExtraSymbol()

            if quantifier.quantifierName in Quantifier.types:

                buildingQuantifier=""
                for v in self.quantifiedVars:
                    if buildingQuantifier != "":
                        buildingQuantifier+=","
                    buildingQuantifier+=str(v)
  
                if buildingQuantifier!="":
                    buildingQuantifier=quantifier.quantifierName+"("+buildingQuantifier+")\n"
                    #printing quantifier statement in final qcir file 
                    f=open(self.finalQCIRFile,"a")
                    f.write(buildingQuantifier)
                    f.close()
                    #storing quantifier type
                    self.quantifiers.append(quantifier)
                else:
                    warnings.warn("quantifier with no variable")
            
            if self.equivalenceGates != "":
                self.equivalenceGates+=","
            #and_ is the output variable of the and gate between left and right implication of the equivalence fi_i <-> cnf(G_i)
            self.equivalenceGates+=str(fi)
            #storing variable that represent fi_i
            self.subformulaIds.append(fi)

            self.tmpQCIRFileHandler.write(f"{fi}={QCIRProgram.AndGate}({self.currentGateIds})\n")
            #clear data for next cnf
            self.currentGateIds=""
            self.buildingCNF=False
            self.quantifiedVars=set()
            self.previousLastVar=self.lastVar
            # print(f"last quantified variable {self.lastVar}")
            
    def close(self,table:SymbolTable):
        #all cnf have been processed
        #final_and=table.addExtraSymbol()

        #defining and gate among and gate of all equivalences
        #self.tmpQCIRFileHandler.write(f"{final_and}={QCIRProgram.AndGate}({self.equivalenceGates})\n")
        
        #building fi_c from right to left
        fi_index=len(self.subformulaIds)-1
        currentGate=self.subformulaIds[fi_index]

        for i in range(len(self.quantifiers)-1,-1,-1):
            if self.quantifiers[i].isConstraint:
                continue
            fi_index-=1
            current_fi = self.subformulaIds[fi_index]
            operator = QCIRProgram.AndGate
            if self.quantifiers[i].quantifierName==Quantifier.QForall:
                current_fi=-current_fi
                operator=QCIRProgram.OrGate

            newGate=table.addExtraSymbol()
            self.tmpQCIRFileHandler.write(f"{newGate}={operator}({current_fi},{currentGate})\n")
            currentGate=newGate

        fi_c=currentGate
        #output=table.addExtraSymbol()
        #defining the output variable as the and between equivalence and-gates and fi_c 
        #self.tmpQCIRFileHandler.write(f"{output}={QCIRProgram.AndGate}({final_and},{fi_c})\n")
        self.tmpQCIRFileHandler.close()
        f = open(self.finalQCIRFile,"a")
        #printing output statatment in final qcir file
        f.write(f"output({fi_c})\n")
        f.close()
        #append al gate statments to final qcir file
        ExternalCalls.callFileAppend(QCIRProgram.tempQCIRFile,self.finalQCIRFile)
            

            

