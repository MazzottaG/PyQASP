from Executors import ExternalCalls
import sys,re
from Option import LPARSE_FORMAT,FILE_UTIL,REGEX_UTIL,DEFAULT_PROPERTIES

class BasicGrounder:
    zeroLine = r'0'
    
    def __init__(self):
        self.input=FILE_UTIL.TO_GROUND_PROGRAM_FILE
        self.output=FILE_UTIL.GROUND_PROGRAM_FILE
        self.prop = ProgramProperties()
        self.model = WellFoundedModel()
        self.usingWellFounded=False
    
    def reset(self):
        self.input=FILE_UTIL.TO_GROUND_PROGRAM_FILE
        self.output=FILE_UTIL.GROUND_PROGRAM_FILE
        self.prop = ProgramProperties()
        self.model = WellFoundedModel()
    
    def printName(self):
        print("Empty Grounder")

    def ground(self):
        print("DEBUG: Basic Grounder called")

    def readWellFounded(self,stdout):
        return None

    def readLParse(self,stdout,fileHandler):
        self.prop.setCoherent(True)
        line = self.readWellFounded(stdout)
        if line is None:
            if self.usingWellFounded:
                print("No well-found model used")
            line = stdout.readline().decode("UTF-8").strip()
        self.prop.setDisjunctive(False)
        if DEFAULT_PROPERTIES.ONLY_CHOICE:
            self.prop.setOnlyChoice(True)
        programEnded = False
        atomTableEnded = False
        while line:
            match = re.match(BasicGrounder.zeroLine,line)
            if match:
                if programEnded:
                    atomTableEnded = True
                programEnded=True
            elif not programEnded:
                match = re.match(r'1 1 1 0 1\n{0,1}',line)
                if not match:
                    lparseRule = [int(x) for x in line.split(LPARSE_FORMAT.SEPARATOR)]
                    if lparseRule[LPARSE_FORMAT.RULE_TYPE_INDEX] == LPARSE_FORMAT.DISJCUNTIVE_RULE:
                        self.prop.setDisjunctive(True)
                        if DEFAULT_PROPERTIES.ONLY_CHOICE:
                            self.prop.setOnlyChoice(False)
                    elif lparseRule[LPARSE_FORMAT.RULE_TYPE_INDEX] == LPARSE_FORMAT.SIMPLE_RULE and lparseRule[LPARSE_FORMAT.ONE_HEAD_ATOM_INDEX] == 1 and lparseRule[LPARSE_FORMAT.BODY_SIZE_INDEX] == 0:
                        self.prop.setCoherent(False)
                    
                    if lparseRule[LPARSE_FORMAT.RULE_TYPE_INDEX] != LPARSE_FORMAT.CHOICE_RULE or lparseRule[LPARSE_FORMAT.BODY_SIZE_INDEX+lparseRule[LPARSE_FORMAT.HEAD_LENGHT_INDEX]] > 0:
                        if DEFAULT_PROPERTIES.ONLY_CHOICE:
                            self.prop.setOnlyChoice(False)
            elif not atomTableEnded and not self.usingWellFounded:
                varStr, atom = line.split(LPARSE_FORMAT.SEPARATOR)
                self.model.addUndef(atom)
                    

            fileHandler.write(line+"\n")
            line = stdout.readline().decode("UTF-8").strip()
        return

    def printProps(self):
        self.prop.print()

    def getWellFounded(self):
        return self.model
    
    def getProps(self):
        return self.prop

class GringoWapper(BasicGrounder):
    
    def __init__(self):
        super().__init__()

    def ground(self):
        f = open(self.output,"w")
        super().readLParse(ExternalCalls.callGringo(self.input),f)
        f.close()

    def printName(self):
        print("Gringo Grounder")

class IDLVWapper(BasicGrounder):
    
    def __init__(self):
        super().__init__()

    def ground(self):
        f = open(self.output,"w")
        super().readLParse(ExternalCalls.callIDLV(self.input),f)
        f.close()

    def printName(self):
        print("IDLV Grounder")

class ProgramProperties:

    def __init__(self):
        self.disjunctive=None
        self.onlyChoice=None
        self.coherent=None

    def setDisjunctive(self,disj : bool):
        self.disjunctive = disj
    
    def setOnlyChoice(self,choice : bool):
        self.onlyChoice = choice
    
    def setCoherent(self,coherent : bool):
        self.coherent = coherent
    
    def isCoherent(self):
        if self.coherent is None:
            print("Warning: coherence properties not defined")
        return self.coherent

    def isDisjunctive(self):
        if self.disjunctive is None:
            print("Warning: disjunctive properties not defined")
        return self.disjunctive

    def isOnlyChoice(self):
        if self.onlyChoice is None:
            print("Warning: onlyChoice properties not defined")
        return self.onlyChoice
    def print(self):
        print(f"disjunctive: {self.disjunctive}")
        print(f"coherent: {self.coherent}")
        print(f"onlyChoice: {self.onlyChoice}")

class WellFoundedModel:
    
    TRUE = 0
    UNDEFINED = 1
    FALSE = 2
    LABELS = ["True","Undef","False"]
    def __init__(self):
        self.model=[[],[],[]]
    
    def save(self,truth,atoms):
        if truth not in [WellFoundedModel.TRUE,WellFoundedModel.UNDEFINED,WellFoundedModel.FALSE]:
            print(f"Error unable to saver {atoms} for truth value {truth}")
            return
        self.model[truth]=atoms

    def addUndef(self,atom):
        self.model[WellFoundedModel.UNDEFINED].append(atom)

    def onlyUndef(self):
        return len(self.model[WellFoundedModel.TRUE]) == 0 and len(self.model[WellFoundedModel.FALSE])==0

    def getTrue(self):
        return self.model[WellFoundedModel.TRUE]
    
    def getUndefined(self):
        return self.model[WellFoundedModel.UNDEFINED]
    
    def getFalse(self):
        return self.model[WellFoundedModel.FALSE]

    def __str__(self):
        out=[]
        for i in [WellFoundedModel.TRUE,WellFoundedModel.UNDEFINED,WellFoundedModel.FALSE]:
            out.append(f"{WellFoundedModel.LABELS[i]} {self.model[i]}")
        return "\n".join(out)
    
        
class DLV2Wapper(BasicGrounder):
    
    def __init__(self):
        super().__init__()
        self.usingWellFounded=True

    preaspIncoherent = r'1 1 0 0\s*\n'
    wellfoundedFormat = [[r'Start WellFounded Model\n',False],[r'True: \{(.*)\}\n',True],[r'Undefined: \{(.*)\}\n',True],[r'False: \{(.*)\}\n',True],[r'End WellFounded Model\n',False]]
    wellfoundedOrder = [WellFoundedModel.TRUE,WellFoundedModel.UNDEFINED,WellFoundedModel.FALSE]

    def readWellFounded(self,stdout):
        line = stdout.readline().decode("UTF-8")
        if not line:
            print("Error empty stdout reading wellfounded model")
            sys.exit(180)

        match_incoherent = re.match(DLV2Wapper.preaspIncoherent,line)
        if match_incoherent:
            self.prop.setCoherent(False)
            return line

        readLines=0
        for reg,read in DLV2Wapper.wellfoundedFormat:
            match = re.match(reg,line)
            if match:
                if read:
                    group = match.group(1)
                    atoms =  []
                    if len(group) > 0:
                        atoms = group.split(", ")

                    self.model.save(DLV2Wapper.wellfoundedOrder[readLines],atoms)
                    readLines+=1
            else:
                print(f"Error: not expected line reading wellfounded {line.strip()}")
                sys.exit(180)
    
            line = stdout.readline().decode("UTF-8")
            if not line:
                print("Error: missing lines reading wellfounded")
                sys.exit(180)
        return line
    
    def ground(self):
        stdout = ExternalCalls.callDLV2(self.input)
        f=open(self.output,"w")
        super().readLParse(stdout,f)
        f.close()

    def printName(self):
        print("IDLV + WellFounded")
