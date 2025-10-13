from Executors import ExternalCalls
from Option import FILE_UTIL,QUABS_OUTPUT,REGEX_UTIL,RAREQS_OUTPUT,PYQASP_OUTPUT,DEFAULT_PROPERTIES,DEPQBF_OUTPUT
from Structures import SymbolTable
from Converter import QCIRCnfToQDIMACS
import re,sys,shutil,json

class QuabsWithWeakAggr:
    
    def __init__(self,opt_predicate,minimize):
        self.opt_predicate=opt_predicate
        self.minimize=minimize

    def update_qbf(self,clauses,start_symbol,last_symbol):
        
        
        first_cnf = True
        first_exist = True
        with open(FILE_UTIL.QBF_PROGRAM_FILE,"r") as f:
            with open(FILE_UTIL.TMP_QBF_PROGRAM_FILE,"w") as g:
                for line in f:
                    line = line.strip()
                    if "exists(" in line and first_exist:
                        first_exist=False
                        fresh_vars = ",".join([str(var) for var in range(start_symbol,last_symbol)])
                        updated_exists = line[:-1]+","+fresh_vars+")"
                        print(updated_exists,file=g)

                    elif " and(" in line and first_cnf:
                        update_cnf = line[:-1]
                        extra_gate = ",".join([str(last_symbol+i) for i in range(len(clauses))])
                        update_cnf = update_cnf + "," + extra_gate + ")"
                        print(update_cnf,file=g)
                        first_cnf=False
                    else:
                        print(line,file=g)

                for i in range(len(clauses)):
                    gate = last_symbol+i
                    clause = ",".join([str(lit) for lit in clauses[i]])
                    print(f"{gate} = or({clause})",file=g)

    def compute_clauses(self,filename,symbolTable):
        var = symbolTable.getCounter()
        clauses = []

        process = ExternalCalls.callGroundingPipeline(filename,False,True)
        line = process.stdout.readline().decode("UTF-8").strip()
        mapping = {}

        while line:
            if line.startswith("c"):
                prefix, local_var, name = line.split(" ")
                id_,_,_,added = symbolTable.addSymbol(name,-1)
                assert not added
                mapping[int(local_var)]=id_
            elif line.startswith("p"):
                pass
            else:
                lits = line.split(" ")[:-1]
                clause = []
                for lit in lits:
                    lit = int(lit)
                    negated=lit<0
                    var_lit = lit if not negated else -lit
                    if var_lit not in mapping:
                        mapping[var_lit]=var
                        var+=1
                    clause.append(mapping[var_lit] if not negated else -mapping[var_lit])
                clauses.append(clause)
            line = process.stdout.readline().decode("UTF-8").strip()
        process.communicate()
        return clauses,var
        
    def solve(self,symbolTable:SymbolTable,isFirstForall,qcirProps):
        print("Solving ...")

        formula_file = FILE_UTIL.QBF_PROGRAM_FILE
        incoherent=True
        literals = {}
        ground_set = []
        choice = []
        for atom,info in symbolTable.getPredicateDomain(self.opt_predicate):
            terms = atom.split("(")[1].split(")")[0].split(",") 
            literals[info[0]]=[atom,terms]
            terms_str = ",".join(terms)
            ground_set.append(f"{terms_str}:{atom}")
            choice.append(atom)
        choice = "{" + ";".join(choice) + "}."
        constraint = ":-#sum{"+ ";".join(ground_set) +"} "+ (">=" if self.minimize else "<=")
        answers = 0
        while True:
            process = ExternalCalls.callSolver([FILE_UTIL.QUABS_PATH,"--partial-assignment","--preprocessing","0",formula_file])
            line = process.stdout.readline().decode("UTF-8").strip()
            if isFirstForall:
                print("Warning: ignoring model since the most external program is universally quantified")
            
            model = None
            cost = None
            sat=False
            unsat=False
            ok=False
            while line:
                fields = line.split(" ")
                if fields[0] == QUABS_OUTPUT.MODEL_START and model is None and not isFirstForall:
                    model=[]
                    factory = symbolTable.getFactory()
                    cost = 0
                    model_var = []
                    for predicate,domain in factory.items():
                        for atom,data in domain.items():
                            var,level = data
                            if level > 1:
                                continue
                            if str(var) in fields:
                                model.append(f"{atom}")
                                model_var.append(var)
                                if var in literals:
                                    atom,terms = literals[var]
                                    cost+=int(terms[0])
                            else:
                                model_var.append(-var)
                                model.append(f"not {atom}")

                # if fields[0] == QUABS_OUTPUT.MODEL_START and model is None and not isFirstForall:
                #     model=[]
                #     factory = symbolTable.getFactory()
                #     cost = 0
                #     for predicate,domain in factory.items():
                #         for atom,data in domain.items():
                #             var,level = data
                #             if str(var) in fields:
                #                 model.append(atom)
                #                 if var in literals:
                #                     atom,terms = literals[var]
                #                     cost+=int(terms[0])
                
                if line.endswith(QUABS_OUTPUT.UNSAT):
                    unsat=True
                elif line.endswith(QUABS_OUTPUT.SAT):
                    sat=True
                    ok=True

                line = process.stdout.readline().decode("UTF-8").strip()

            
            process.communicate()
            
            # print(f"{PYQASP_OUTPUT.EXTENDED}{process.returncode}")
            if not sat and not unsat:
                print("Unknown results from QUABS")
                FILE_UTIL.cleanup()
                sys.exit(180)

            if unsat:
                if incoherent:
                    print("INCOHERENT")
                    FILE_UTIL.cleanup()
                    sys.exit(20)
                else:
                    print("OPTIMUM FOUND:",opt_cost)
                    FILE_UTIL.cleanup()
                    sys.exit(30)
            
            if not model is None:
                answers+=1
                # print(f"Answer {answers}:",". ".join(model))
                json.dump({"literals":model}, sys.stdout)
                print()
            if not cost is None:
                print("OPTMIZATION:",cost)
            else:
                print("Error: Unknown QAnswer Set Cost")
                FILE_UTIL.cleanup()
                sys.exit(180)
            
            with open(FILE_UTIL.TO_GROUND_PROGRAM_FILE,"w") as g:
                print(choice,file=g)
                print(constraint,cost,".",file=g)
            
            clauses,last_symbol = self.compute_clauses(FILE_UTIL.TO_GROUND_PROGRAM_FILE,symbolTable)
            opt_cost=cost
            incoherent = False
            self.update_qbf(clauses,symbolTable.getCounter(),last_symbol)
            formula_file = FILE_UTIL.TMP_QBF_PROGRAM_FILE
            
