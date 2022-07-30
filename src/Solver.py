from Executors import ExternalCalls
from Option import FILE_UTIL,QUABS_OUTPUT
from Structures import SymbolTable

class Solver:
    def solve(self,symbolTable:SymbolTable,isFirstForall):
        print("Solving ...")

class Quabs(Solver):
    
    def solve(self,symbolTable:SymbolTable,isFirstForall):
        super().solve(symbolTable,isFirstForall)
        stdout = ExternalCalls.callSolver([FILE_UTIL.QUABS_PATH,"--partial-assignment",FILE_UTIL.QBF_PROGRAM_FILE])
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
                print("UNSATISFIABLE")
                return
            if line.endswith(QUABS_OUTPUT.SAT):
                sat=True

            line = stdout.readline().decode("UTF-8").strip()

        if not isFirstForall and (not sat or model is None):
            print(f"Warning: unexpected outcome model: {model} and sat: {sat}")
        
        if sat:
            print("SATISFIABLE")
        if not model is None:
            print(". ".join(model))
