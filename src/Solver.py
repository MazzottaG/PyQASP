from Executors import ExternalCalls
from Option import FILE_UTIL,QUABS_OUTPUT,REGEX_UTIL,RAREQS_OUTPUT,PYQASP_OUTPUT,DEFAULT_PROPERTIES,DEPQBF_OUTPUT,QCIR_FORMAT
from Structures import SymbolTable
from Converter import QCIRCnfToQDIMACS
import re,sys

class OutputBuilder:
    
    def printOuput(self,symbolTable:SymbolTable,isFirstForall,process):
        line = process.stdout.readline().decode("UTF-8").strip()
        sat = False
        unsat = False
        while line:
            print(line)
            if DEPQBF_OUTPUT.UNSAT in line:
                unsat = True
            elif DEPQBF_OUTPUT.SAT in line:
                sat=True
            line = process.stdout.readline().decode("UTF-8").strip()
        process.communicate()
        print(f"{PYQASP_OUTPUT.EXTENDED}{process.returncode}")
        sys.exit(process.returncode)

        # if sat:
        #     print(PYQASP_OUTPUT.SAT)
        # if unsat:
        #     print(PYQASP_OUTPUT.UNSAT)
        # if not sat and not unsat:
        #     print("No answer found")

class RareqsOutputBuilder(OutputBuilder):
    
    def printOuput(self,symbolTable:SymbolTable,isFirstForall,process):
        line = process.stdout.readline().decode("UTF-8")
        # if isFirstForall:
        #     print("Warning: ignoring model since the most external program is universally quantified")
        sat=False
        unsat=False
        while line:
            print(line)
            match = re.match(REGEX_UTIL.RAREQS_OUT,line)
            if match:
                if match.group(1) == RAREQS_OUTPUT.SAT:
                    sat=True
                elif match.group(1) == RAREQS_OUTPUT.UNSAT:
                    sat=False
            line = process.stdout.readline().decode("UTF-8")
        process.communicate()
        print(f"{PYQASP_OUTPUT.EXTENDED}{process.returncode}")
        sys.exit(process.returncode)

        # if sat:
        #     print(PYQASP_OUTPUT.SAT)
        # elif unsat:
        #     print(PYQASP_OUTPUT.UNSAT)
        # else:
        #     print("No answer found")

class QuabsOutputBuilder(OutputBuilder):
    
    def printOuput(self,symbolTable:SymbolTable,isFirstForall,process):
        line = process.stdout.readline().decode("UTF-8").strip()
        if isFirstForall:
            print("Warning: ignoring model since the most external program is universally quantified")
        
        model = None
        sat=False
        unsat=False
        model_var = []
        while line:
            #print(line)
            fields = line.split(" ")
            if fields[0] == QUABS_OUTPUT.MODEL_START and model is None and not isFirstForall:
                model=[]
                factory = symbolTable.getFactory()
                for predicate,domain in factory.items():
                    for atom,data in domain.items():
                        var,level = data
                        if level > 1:
                            continue
                        if str(var) in fields:
                            model.append(f"{atom}")
                            model_var.append(var)
                        else:
                            model_var.append(-var)
                            model.append(f"not {atom}")
            
            if line.endswith(QUABS_OUTPUT.UNSAT):
                unsat=True
            elif line.endswith(QUABS_OUTPUT.SAT):
                sat=True

            line = process.stdout.readline().decode("UTF-8").strip()

        if not model is None:
            print(". ".join(model))
        
        process.communicate()
        return process.returncode,model_var

        # if not isFirstForall and (not sat or model is None):
        #     print(f"Warning: unexpected outcome model: {model} and sat: {sat}")
        

        # if sat:
        #     print(PYQASP_OUTPUT.SAT)
        # if unsat:
        #     print(PYQASP_OUTPUT.UNSAT)
        # if not sat and not unsat:
        #     print("No answer found")
        
        

        
class Solver:
    
    def __init__(self):
        self.outputBuilder = OutputBuilder()

    def printOuput(self,symbolTable:SymbolTable,isFirstForall,process):
        print("Output ...")
        self.outputBuilder.printOuput(symbolTable,isFirstForall,process)
    
    def solve(self,symbolTable:SymbolTable,isFirstForall,qcirProps):
        print("Solving ...")
        
class Rareqs(Solver):
    # public final static ShellCommand RARE_QS_COMMAND_TEMPLATE = new ShellCommand(
	# 		"%s $file -prenex -write-gq | %s -",
	# 		new String[] { "qcir-conv.py", "rareqs-nn" });
    def __init__(self):
        super().__init__()
        self.outputBuilder = RareqsOutputBuilder()
    
    def solve(self,symbolTable:SymbolTable,isFirstForall,qcirProps):
        super().solve(symbolTable,isFirstForall,qcirProps)
        cmds = [
            [FILE_UTIL.QCIR_CONV_PATH,FILE_UTIL.QBF_PROGRAM_FILE,"-prenex","-write-gq"],
            [FILE_UTIL.RAREQS_NN_PATH, "-"]
        ]
        pipeline = ExternalCalls.callSolverPipeline(cmds)
        self.outputBuilder.printOuput(symbolTable,isFirstForall,pipeline[-1])

class Depqbf(Solver):
    # 	public final static ShellCommand DEPQBF_COMMAND_TEMPLATE_BLO = new ShellCommand(
    # 			"%s $file -prenex | %s - -read-qcir -write-dimacs | %s | %s",
    # 			new String[] { "qcir-conv.py", "fmla", "bloqqer37", "depqbf" });
    
    def __init__(self):
        super().__init__()
    
    def solve(self,symbolTable:SymbolTable,isFirstForall,qcirProps):
        super().solve(symbolTable,isFirstForall,qcirProps)
        cmds = []
        # if True and qcirProps.isQuiteCnf():
        if DEFAULT_PROPERTIES.SKIP_QCIR_CONV_FOR_QDIMACS and qcirProps.isQuiteCnf():
            print("Mapping directly into cnf")
            QCIRCnfToQDIMACS().translate(qcirProps.getLastSymbol(),qcirProps.getClausesCount(),qcirProps.getLevelsCount())
            cmds = [
                [FILE_UTIL.BLOQQER37_PATH,FILE_UTIL.QDIMACS_PROGRAM_FILE],
                [FILE_UTIL.DEPQBF_PATH]
            ]
        else:
            cmds = [
                [FILE_UTIL.QCIR_CONV_PATH,FILE_UTIL.QBF_PROGRAM_FILE,"-prenex"],
                [FILE_UTIL.FMLA_PATH,"-","-read-qcir","-write-dimacs"],
                [FILE_UTIL.BLOQQER37_PATH],
                [FILE_UTIL.DEPQBF_PATH]
            ]

        pipeline = ExternalCalls.callSolverPipeline(cmds)
        self.outputBuilder.printOuput(symbolTable,isFirstForall,pipeline[-1])

class Quabs(Solver):
    
    def __init__(self):
        super().__init__()
        self.outputBuilder = QuabsOutputBuilder()

    def solve(self,symbolTable:SymbolTable,isFirstForall,qcirProps):
        super().solve(symbolTable,isFirstForall,qcirProps)
        process = ExternalCalls.callSolver([FILE_UTIL.QUABS_PATH,"--partial-assignment",FILE_UTIL.QBF_PROGRAM_FILE])
        exit_code,model = self.outputBuilder.printOuput(symbolTable,isFirstForall,process)
        print(f"{PYQASP_OUTPUT.EXTENDED}{exit_code}")



class QuabsEnumerator(Solver):
    
    def __init__(self):
        super().__init__()
        self.outputBuilder = QuabsOutputBuilder()

    def solve(self,symbolTable:SymbolTable,isFirstForall,qcirProps):
        super().solve(symbolTable,isFirstForall,qcirProps)
        exit_code = 10
        SAT = False
        while exit_code == 10:
            process= ExternalCalls.callSolver([FILE_UTIL.QUABS_PATH,"--partial-assignment",FILE_UTIL.QBF_PROGRAM_FILE])
            exit_code, model = self.outputBuilder.printOuput(symbolTable,isFirstForall,process)
            if exit_code == 10:
                SAT=True
                with open(FILE_UTIL.WORKING_QBF_PROGRAM_FILE,"w") as g:
                    add_gate = True
                    with open(FILE_UTIL.QBF_PROGRAM_FILE, "r") as f:
                        for line in f:
                            line = line.strip()
                            if QCIR_FORMAT.AND_GATE in line and add_gate:
                                add_gate=False

                                model_gate = symbolTable.addExtraSymbol()
                                model_clause = ",".join([str(-v) for v in model])
                                print(f"{model_gate} = {QCIR_FORMAT.OR_GATE}({model_clause})",file=g)
                                print(f"{line[:-1]},{model_gate})",file=g)
                            else:
                                print(line,file=g)

                ExternalCalls.callFileCopy(FILE_UTIL.WORKING_QBF_PROGRAM_FILE,FILE_UTIL.QBF_PROGRAM_FILE)

        if not SAT:
            print(PYQASP_OUTPUT.UNSAT)