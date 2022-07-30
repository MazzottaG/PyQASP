from Option import FILE_UTIL,QCIR_FORMAT,REGEX_UTIL,QDIMACS_FORMAT
import re

class QCIRAndsToQDIMACS:
    
    def __init__(self):
        self.currentQuantifier = None
        self.currentVariables = None

    def translate(self,variables,clauses,totalFormulas):
        switchQuant = {
            QCIR_FORMAT.FORALL : QDIMACS_FORMAT.FORALL,
            QCIR_FORMAT.EXISTS : QDIMACS_FORMAT.EXISTS
        }
        qcirFileHandler = open(FILE_UTIL.QBF_PROGRAM_FILE,"r")
        qdimacsFileHandler = open(FILE_UTIL.QDIMACS_PROGRAM_FILE,"w")
        qdimacsFileHandler.write(f"p cnf {variables} {clauses}\n")
        outputFound = False
        formulasCount = 0
        for line in qcirFileHandler:
            if not outputFound:
                if QCIR_FORMAT.OUTPUT in line:
                    if not self.currentQuantifier is None:
                        self.currentVariables = " ".join(self.currentVariables)
                        qdimacsFileHandler.write(f"{switchQuant[self.currentQuantifier]} {self.currentVariables} 0\n")
                    outputFound=True
                    continue
                match = re.match(REGEX_UTIL.QCIR_QUANTIFIER,line)
                if match:
                    quantifier = match.group(1)
                    variables = match.group(2).replace(","," ")
                    
                    if self.currentQuantifier == quantifier:
                        self.currentVariables.append(variables)
                    elif not self.currentQuantifier is None:
                        self.currentVariables = " ".join(self.currentVariables)
                        qdimacsFileHandler.write(f"{switchQuant[self.currentQuantifier]} {self.currentVariables} 0\n")
                        
                        self.currentQuantifier = quantifier
                        self.currentVariables = [variables]
                         
                    if self.currentQuantifier == None:
                        self.currentQuantifier = quantifier
                        self.currentVariables = [variables]
            else:
                match = re.match(REGEX_UTIL.QCIR_GATE,line)
                if match:
                    op = match.group(1)
                    if op == QCIR_FORMAT.OR_GATE:
                        clause = match.group(2).replace(","," ")
                        qdimacsFileHandler.write(f"{clause} 0\n")
                    else:
                        formulasCount+=1
            if formulasCount >= totalFormulas:
                break
                        