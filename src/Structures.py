from Executors import ExternalCalls
import warnings,os,sys

class SymbolTable:
    
    def __init__(self):
        self.factory={}
        self.idCounter=2
        
    def addExtraSymbol(self):
        self.idCounter+=1
        return self.idCounter-1

    def addSymbol(self,atom):

        predicate = atom.split("(")[0]
        added=False
        if predicate not in self.factory:
            self.factory[predicate]={}
        if atom not in self.factory[predicate]:
            self.factory[predicate][atom]=self.idCounter
            added=True
            self.idCounter+=1
        return (self.factory[predicate][atom],added)

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
    def print(self):
        print(self.factory)

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

    tempQCIRFile=os.path.join(sys._MEIPASS,"resources","files","tmp.qcir")
    AndGate="and"
    OrGate="or"
    def __init__(self,filename):
        self.buildingCNF=False
        self.currentGateIds=""
        self.subformulaIds=[]
        self.quantifiers=[]
        self.finalQCIRFile=filename
        self.tmpQCIRFileHandler=open(QCIRProgram.tempQCIRFile,"w")
        f=open(self.finalQCIRFile,"w")
        f.write("#QCIR-G14\n")
        f.close()

    def buildCNF(self):
        self.buildingCNF=True

    def addOrGate(self,clause,table:SymbolTable):

        if self.buildingCNF:
            #clause = clause.replace(" ",",")
            gateId = table.addExtraSymbol()
            if len(self.currentGateIds)>0:
                self.currentGateIds+=","
            #storing gate id for each clause of the current cnf
            self.currentGateIds+=str(gateId)
            self.tmpQCIRFileHandler.write(f"{gateId}={QCIRProgram.OrGate}({clause})\n")

    def builtCNF(self,table:SymbolTable,quantifier:Quantifier,buildingQuantifier):
        if self.buildingCNF:
            if quantifier.quantifierName in Quantifier.types:

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
            
            fi = table.addExtraSymbol()
            #storing variable that represent fi_i
            self.subformulaIds.append(fi)

            self.tmpQCIRFileHandler.write(f"{fi}={QCIRProgram.AndGate}({self.currentGateIds})\n")
            
            #clear data for next cnf
            self.currentGateIds=""
            self.buildingCNF=False
            
    def close(self,table:SymbolTable):
        #all cnf have been processed

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

        self.tmpQCIRFileHandler.close()

        #defining the output variable fi_c 
        fi_c=currentGate
        f = open(self.finalQCIRFile,"a")
        #printing output statatment in final qcir file
        f.write(f"output({fi_c})\n")
        f.close()
        #append al gate statments to final qcir file
        ExternalCalls.callFileAppend(QCIRProgram.tempQCIRFile,self.finalQCIRFile)
            

            

