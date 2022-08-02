from Executors import ExternalCalls
import os,sys,logging

class SymbolTable:
    TRUE=1
    UNDEF=0
    FALSE=-1

    def __init__(self):
        self.factory={}
        self.idCounter=2
        self.idToAtom=[SymbolTable.UNDEF,SymbolTable.UNDEF]
        
    def addExtraSymbol(self):
        self.idCounter+=1
        self.idToAtom.append(SymbolTable.UNDEF)
        return self.idCounter-1
    
    def assign(self,atom,value,level):
        predicate = atom.split("(")[0]
        added=False
        var=0
        lev=0
        if predicate not in self.factory:
            self.factory[predicate]={}
        if atom not in self.factory[predicate]:
            self.factory[predicate][atom]=[self.idCounter,level]
            added=True
            self.idToAtom.append(value)
            var = self.idCounter
            lev = level
            self.idCounter+=1
        else:
            var,lev = self.factory[predicate][atom]
            self.idToAtom[var]=value

        return [var,lev] if value == SymbolTable.TRUE else [-var,lev]

    def addSymbol(self,atom,level):
        
        predicate = atom.split("(")[0]
        if predicate not in self.factory:
            self.factory[predicate]={}
        if atom not in self.factory[predicate]:
            self.factory[predicate][atom] = [self.idCounter,level]
            self.idToAtom.append(SymbolTable.UNDEF)
            self.idCounter+=1
        
        id_,lev = self.factory[predicate][atom]
        return (id_,lev,self.idToAtom[id_])

    def getPredicateDomain(self,predicate):
        if predicate in self.factory:
            return self.factory[predicate].items()
        return {}
    
    def getTruthValue(self,id_):
        return self.idToAtom[id_]
    
    def containsPredicate(self,predicate):
        return predicate in self.factory
    
    def getFactory(self):
        return self.factory
    
    def getSymbolsCount(self):
        return self.idCounter - 2
    
    def getCounter(self):
        return self.idCounter
    def print(self):
        print("Factory: ",self.factory)
