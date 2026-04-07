from pyqasp.Executors import ExternalCalls
from pyqasp.Option import FILE_UTIL,QUABS_OUTPUT,REGEX_UTIL,RAREQS_OUTPUT,PYQASP_OUTPUT,DEFAULT_PROPERTIES,DEPQBF_OUTPUT
from pyqasp.Structures import SymbolTable
from pyqasp.Converter import QCIRCnfToQDIMACS
import re,sys,shutil,json,subprocess

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
        levels = {}
        ground_set_for_level={}
        for atom,info in symbolTable.getPredicateDomain(self.opt_predicate):
            terms = atom.split("(")[1].split(")")[0].split(",") 
            opt_level = int(terms[1])
            try:
                levels[opt_level].append(atom)
            except:
                levels[opt_level]=[atom]
            literals[info[0]]=[atom,terms]
            terms_str = ",".join(terms)
            try:
                ground_set_for_level[opt_level].append(f"{terms_str}:{atom}")
            except:
                ground_set_for_level[opt_level]=[f"{terms_str}:{atom}"]
            choice.append(atom)
        
        constraint_for_level={}
        for lev in ground_set_for_level:
            constraint = ":-#sum{"+ ";".join(ground_set_for_level[lev]) +"} "
            constraint_for_level[lev] = constraint    
        choice = "{" + ";".join(choice) + "}."
        # constraint = ":-#sum{"+ ";".join(ground_set) +"} "+ (">=" if self.minimize else "<=")
        answers = 0
        
        sorted_levels = sorted([x for x in levels])
        current_level=sorted_levels.pop()
        opt_for_level={}
        prev_cost = None

        while True:
            process = ExternalCalls.callSolver([FILE_UTIL.QUABS_PATH,"--partial-assignment","--preprocessing","0",formula_file])
            line = process.stdout.readline().decode("UTF-8").strip()
            if isFirstForall:
                print("Warning: ignoring model since the most external program is universally quantified")
            
            model = None
            cost  = None
            sat   = False
            unsat = False
            ok    = False
            while line:
                fields = line.split(" ")
                if fields[0] == QUABS_OUTPUT.MODEL_START and model is None and not isFirstForall:
                    model=[]
                    factory = symbolTable.getFactory()
                    cost = {}
                    model_var = []
                    for predicate,domain in factory.items():
                        for atom,data in domain.items():
                            var,level = data
                            if level > 1:
                                continue
                            if str(var) in fields:
                                if predicate != symbolTable.get_weak_pred():
                                    model.append(f"{atom}")
                                model_var.append(var)
                                if var in literals:
                                    atom,terms = literals[var]
                                    # print("Found cost atom",atom)
                                    try:
                                        cost[int(terms[1])]+=int(terms[0])
                                        # print("   Adding cost at level",int(terms[1]),":",int(terms[0]))
                                    except:
                                        cost[int(terms[1])]=int(terms[0])
                                        # print("   Initializing cost at level",int(terms[1]),":",int(terms[0]))
                            else:
                                model_var.append(-var)
                                if predicate != symbolTable.get_weak_pred():
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
                    if len(sorted_levels) == 0:
                        print("OPTIMUM FOUND:",opt_for_level)
                        FILE_UTIL.cleanup()
                        sys.exit(30)
                    else:
                        current_level=sorted_levels.pop()
                        # print("Next level:",current_level)
                        # print("Remaining levels:",sorted_levels)

            
            if not model is None:
                answers+=1
                # print(f"Answer {answers}:",". ".join(model))
                json.dump({"literals":model}, sys.stdout)
                print()
                
            if not cost is None:
                for lev in levels:
                    if lev not in cost:
                        cost[lev]=0
                print("OPTMIZATION:",cost)
            elif not unsat:
                print("Error: Unknown QAnswer Set Cost")
                FILE_UTIL.cleanup()
                sys.exit(180)
            
            with open(FILE_UTIL.TO_GROUND_PROGRAM_FILE,"w") as g:
                print(choice,file=g)
                for lev in opt_for_level:
                    if lev != current_level:
                        print(constraint_for_level[lev],"!=",opt_for_level[lev],".",file=g)
                if not unsat:
                    print(constraint_for_level[current_level],(">=" if self.minimize else "<="),cost[current_level],".",file=g)
                else:
                    print(constraint_for_level[current_level],(">=" if self.minimize else "<="),prev_cost[current_level],".",file=g)
            
            clauses,last_symbol = self.compute_clauses(FILE_UTIL.TO_GROUND_PROGRAM_FILE,symbolTable)
            if not unsat:
                opt_for_level[current_level]=cost[current_level]
                prev_cost=cost
            incoherent = False
            self.update_qbf(clauses,symbolTable.getCounter(),last_symbol)
            formula_file = FILE_UTIL.TMP_QBF_PROGRAM_FILE
            
