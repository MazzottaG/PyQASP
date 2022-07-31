from Executors import ExternalCalls
from Option import FILE_UTIL,QUABS_OUTPUT,REGEX_UTIL,RAREQS_OUTPUT,PYQASP_OUTPUT
from Structures import SymbolTable
import re

class OutputBuilder:
    
    def printOuput(self,symbolTable:SymbolTable,isFirstForall,stdout):
        line = stdout.readline().decode("UTF-8").strip()
        while line:
            print(line)
            line = stdout.readline().decode("UTF-8").strip()
class RareqsOutputBuilder(OutputBuilder):
    
    def printOuput(self,symbolTable:SymbolTable,isFirstForall,stdout):
        line = stdout.readline().decode("UTF-8")
        # if isFirstForall:
        #     print("Warning: ignoring model since the most external program is universally quantified")
        sat=False
        while line:
            match = re.match(REGEX_UTIL.RAREQS_OUT,line)
            if match:
                if match.group(1) == RAREQS_OUTPUT.SAT:
                    print(PYQASP_OUTPUT.SAT)
                else:
                    print(PYQASP_OUTPUT.UNSAT)
                return
            line = stdout.readline().decode("UTF-8")
            
class QuabsOutputBuilder(OutputBuilder):
    
    def printOuput(self,symbolTable:SymbolTable,isFirstForall,stdout):
        line = stdout.readline().decode("UTF-8").strip()
        if isFirstForall:
            print("Warning: ignoring model since the most external program is universally quantified")
        model = None
        sat=False
        while line:
            fields = line.split(" ")
            if fields[0] == QUABS_OUTPUT.MODEL_START and model is None and not isFirstForall:
                model=[]
                factory = symbolTable.getFactory()
                for predicate,domain in factory.items():
                    for atom,data in domain.items():
                        var,level = data
                        if str(var) in fields:
                            model.append(atom)
            if line.endswith(QUABS_OUTPUT.UNSAT):
                print(PYQASP_OUTPUT.UNSAT)
                return

            if line.endswith(QUABS_OUTPUT.SAT):
                sat=True

            line = stdout.readline().decode("UTF-8").strip()

        if not isFirstForall and (not sat or model is None):
            print(f"Warning: unexpected outcome model: {model} and sat: {sat}")
        
        if sat:
            print(PYQASP_OUTPUT.SAT)

        if not model is None:
            print(". ".join(model))

        
class Solver:
    
    def __init__(self):
        self.outputBuilder = OutputBuilder()

    def printOuput(self,symbolTable:SymbolTable,isFirstForall,stdout):
        print("Output ...")
        self.outputBuilder.printOuput(symbolTable,isFirstForall,stdout)
    
    def solve(self,symbolTable:SymbolTable,isFirstForall):
        print("Solving ...")
class Rareqs(Solver):
    # public final static ShellCommand RARE_QS_COMMAND_TEMPLATE = new ShellCommand(
	# 		"%s $file -prenex -write-gq | %s -",
	# 		new String[] { "qcir-conv.py", "rareqs-nn" });
    def __init__(self):
        super().__init__()
        self.outputBuilder = RareqsOutputBuilder()
    
    def solve(self,symbolTable:SymbolTable,isFirstForall):
        super().solve(symbolTable,isFirstForall)
        cmds = [
            [FILE_UTIL.QCIR_CONV_PATH,FILE_UTIL.QBF_PROGRAM_FILE,"-prenex","-write-gq"],
            [FILE_UTIL.RAREQS_NN_PATH, "-"]]
        stdout = ExternalCalls.callSolverPipeline(cmds)
        self.outputBuilder.printOuput(symbolTable,isFirstForall,stdout)

class Depqbf(Solver):
    # 	public final static ShellCommand DEPQBF_COMMAND_TEMPLATE_BLO = new ShellCommand(
    # 			"%s $file -prenex | %s - -read-qcir -write-dimacs | %s | %s",
    # 			new String[] { "qcir-conv.py", "fmla", "bloqqer37", "depqbf" });
    
    def __init__(self):
        super().__init__()
    
    def solve(self,symbolTable:SymbolTable,isFirstForall):
        super().solve(symbolTable,isFirstForall)
        cmds = [
            [FILE_UTIL.QCIR_CONV_PATH,FILE_UTIL.QBF_PROGRAM_FILE,"-prenex"],
            [FILE_UTIL.FMLA_PATH,"-","-read-qcir","-write-dimacs"],
            [FILE_UTIL.BLOQQER37_PATH],
            [FILE_UTIL.DEPQBF_PATH]]
        stdout = ExternalCalls.callSolverPipeline(cmds)
        self.outputBuilder.printOuput(symbolTable,isFirstForall,stdout)

class Quabs(Solver):
    
    def __init__(self):
        super().__init__()
        self.outputBuilder = QuabsOutputBuilder()

    def solve(self,symbolTable:SymbolTable,isFirstForall):
        super().solve(symbolTable,isFirstForall)
        stdout = ExternalCalls.callSolver([FILE_UTIL.QUABS_PATH,"--partial-assignment",FILE_UTIL.QBF_PROGRAM_FILE])
        self.outputBuilder.printOuput(symbolTable,isFirstForall,stdout)