from Executors import ExternalCalls
from Option import FILE_UTIL,QUABS_OUTPUT,REGEX_UTIL,RAREQS_OUTPUT,PYQASP_OUTPUT,DEFAULT_PROPERTIES,DEPQBF_OUTPUT,QCIR_FORMAT
from Structures import SymbolTable
from Converter import QCIRCnfToQDIMACS
import re,sys,json

class OutputBuilder:

    def __init__(self):
        self.debug = DEFAULT_PROPERTIES.debug
    
    def printOuput(self,symbolTable:SymbolTable,isFirstForall,process):
        line = process.stdout.readline().decode("UTF-8").strip()
        sat = False
        unsat = False
        while line:
            self.debug.printMessage(line)
            if DEPQBF_OUTPUT.UNSAT in line:
                unsat = True
            elif DEPQBF_OUTPUT.SAT in line:
                sat=True
            line = process.stdout.readline().decode("UTF-8").strip()
        process.communicate()
        self.debug.printMessage(f"{PYQASP_OUTPUT.EXTENDED}{process.returncode}")
        return process.returncode,None

        # if sat:
        #     print(PYQASP_OUTPUT.SAT)
        # if unsat:
        #     print(PYQASP_OUTPUT.UNSAT)
        # if not sat and not unsat:
        #     print("No answer found")

class RareqsOutputBuilder(OutputBuilder):
    
    def __init__(self):
        super().__init__()
    
    def printOuput(self,symbolTable:SymbolTable,isFirstForall,process):
        line = process.stdout.readline().decode("UTF-8")
        # if isFirstForall:
        #     print("Warning: ignoring model since the most external program is universally quantified")
        sat=False
        unsat=False
        while line:
            self.debug.printMessage(line)
            match = re.match(REGEX_UTIL.RAREQS_OUT,line)
            if match:
                if match.group(1) == RAREQS_OUTPUT.SAT:
                    sat=True
                elif match.group(1) == RAREQS_OUTPUT.UNSAT:
                    sat=False
            line = process.stdout.readline().decode("UTF-8")
        process.communicate()
        self.debug.printMessage(f"{PYQASP_OUTPUT.EXTENDED}{process.returncode}")
        return process.returncode,None

        # if sat:
        #     print(PYQASP_OUTPUT.SAT)
        # elif unsat:
        #     print(PYQASP_OUTPUT.UNSAT)
        # else:
        #     print("No answer found")

class QuabsOutputBuilder(OutputBuilder):

    def __init__(self):
        super().__init__()
    
    def printOuput(self,symbolTable:SymbolTable,isFirstForall,process):
        line = process.stdout.readline().decode("UTF-8").strip()
        if isFirstForall:
            self.debug.printMessage("Warning: ignoring model since the most external program is universally quantified")
        
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

        if not model is None and not DEFAULT_PROPERTIES.COUNTING:
            json.dump({"literals":model}, sys.stdout)
            print()
            # print(". ".join(model))
        
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
        self.debug=DEFAULT_PROPERTIES.debug

    def printOuput(self,symbolTable:SymbolTable,isFirstForall,process):
        self.debug.printMessage("Output ...")
        return self.outputBuilder.printOuput(symbolTable,isFirstForall,process)
    
    def solve(self,symbolTable:SymbolTable,isFirstForall,qcirProps):
        self.debug.printMessage("Solving ...")
        return None
        
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
        exit_code,model = self.outputBuilder.printOuput(symbolTable,isFirstForall,pipeline[-1])
        return exit_code

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
            self.debug.printMessage("Mapping directly into cnf")
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
        exit_code,model = self.outputBuilder.printOuput(symbolTable,isFirstForall,pipeline[-1])
        return exit_code

class Quabs(Solver):
    
    def __init__(self):
        super().__init__()
        self.outputBuilder = QuabsOutputBuilder()

    def solve(self,symbolTable:SymbolTable,isFirstForall,qcirProps):
        super().solve(symbolTable,isFirstForall,qcirProps)
        command = [FILE_UTIL.QUABS_PATH,"--partial-assignment","--preprocessing","0",FILE_UTIL.QBF_PROGRAM_FILE] 
        if DEFAULT_PROPERTIES.SATISFIABILITY:
            command = [FILE_UTIL.QUABS_PATH,"--partial-assignment",FILE_UTIL.QBF_PROGRAM_FILE] 

        process = ExternalCalls.callSolver(command)
        exit_code,model = self.outputBuilder.printOuput(symbolTable,isFirstForall,process)
        self.debug.printMessage(f"{PYQASP_OUTPUT.EXTENDED}{exit_code}")
        return exit_code



class QuabsEnumerator(Solver):
    
    def __init__(self):
        super().__init__()
        self.outputBuilder = QuabsOutputBuilder()

    def solve(self,symbolTable:SymbolTable,isFirstForall,qcirProps):
        super().solve(symbolTable,isFirstForall,qcirProps)
        exit_code = 10
        SAT = False
        models=[]
        count_models=0
        while exit_code == 10:
            process= ExternalCalls.callSolver([FILE_UTIL.QUABS_PATH,"--partial-assignment","--preprocessing","0",FILE_UTIL.QBF_PROGRAM_FILE])
            exit_code, model = self.outputBuilder.printOuput(symbolTable,isFirstForall,process)
            if exit_code == 10:
                SAT=True
                count_models += 1
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

        if DEFAULT_PROPERTIES.COUNTING:
            suffix = "s" if count_models > 1 else ""
            print(f"Quantified Answer Set{suffix}:",count_models)
        if not SAT:
            self.debug.printMessage(PYQASP_OUTPUT.UNSAT)
            return 20
        else:
            return 10