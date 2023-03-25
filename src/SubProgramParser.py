from Executors import ExternalCalls
from Option import FILE_UTIL,REGEX_UTIL,QASP_FORMAT,LPARSE_FORMAT,QCIR_FORMAT,DEFAULT_PROPERTIES,PYQASP_OUTPUT
from PyAspParser.ProgramParserLight import *
from Structures import SymbolTable
from Builder import QCIRProps
import os,re,sys,subprocess,time,json

ASP_TO_JSON_RULE_PREFIX=0
ASP_TO_JSON_FACT_PREFIX=1
ASP_TO_JSON_SEPARATOR="@"

# class RuleListener(Thread):
    
#     def __init__(self,proc,bar):
#         super().__init__()
#         self.process = proc
#         self.result=0
        
#     def run(self):
#         for line in self.process.stdout:
#             print(line.decode("utf-8"))
        
class EmptyDebugger:
    def printMessage(self,message):
        return

class Debugger:
    def printMessage(self,message):
        print(message)
        return

class EmptyDebugCommand:
    def getOutput(self,cmd):
        return ""

class DebugCommand(EmptyDebugCommand):
    def getOutput(self,cmd):
        return subprocess.getoutput(cmd)

class PredicateMap:
    
    def __init__(self):
        self.predicatesToId={}
        self.idToPred=[]
        self.predicateCount=0
        
    def getPredicateId(self,predicate):
        try:
            id_=self.predicatesToId[predicate]
            return self.predicatesToId[predicate]
        except:
            pred_id = self.predicateCount
            self.predicatesToId[predicate] = self.predicateCount
            self.predicateCount+=1
            self.idToPred.append(predicate)
            return pred_id
    def getPredicate(pred_id):
        try:
            return self.idToPred[pred_id]
        except:
            return ""
            
class SubProgramParser:

    AUX_PREDICATE = "unsat_forall_"
    SIMPLIFIED_FORALL_FILE = os.path.join(FILE_UTIL.FILES_FOLDER_PATH,"simplified_forall.asp")
    OMITTED_FORALL_FILE = os.path.join(FILE_UTIL.FILES_FOLDER_PATH,"omitted_forall.asp")
    
    INCOHERENT_F = 1
    INCOHERENT_E = 2
    INCOHERENT_C = 3
    SKIPPED      = 4
    EMPTY        = 5
    ENCODED_C    = 6
    ENCODED_F    = 7
    ENCODED_E    = 8
    
    def __init__(self,qaspFilename,debug,cmd,grounder):
        self.predicateMap = PredicateMap()
        self.grounder_backend=grounder
        self.programCount = 0
        self.currentQuantifier = None
        self.qaspFile = qaspFilename
        self.stopEncoding=False
        
        self.pushSubProgram = None    
        self.forallLevel = None
        self.addUnsat = None
        self.movedSymbols = []
        self.movedInterface = []
        
        self.encodedLevel = [None]

        self.symbols = SymbolTable()
        
        self.gates=[]
        self.clauseCount = 0
        self.gatesCount = 0

        self.debugger=debug
        self.debugcmd=cmd
    
    def parseRule(self,head,choiceBodyForHead,body):
        
        body_numeric=None
        if not body is None:
            body_numeric=[]
            for pred in body:
                pred_id = self.predicateMap.getPredicateId(pred[1])
                body_numeric.append([pred[0],pred_id])
        
        head_numeric = None
        if not head is None:
            head_numeric=[]
            for pred in head:
                head_numeric.append(self.predicateMap.getPredicateId(pred))

        choiceBodyForHead_numeric=None 
        if not choiceBodyForHead is None:
            choiceBodyForHead_numeric={}
            for h in choiceBodyForHead:
                pred_h = self.predicateMap.getPredicateId(h)
                for pred in choiceBodyForHead[h]:
                    pred_b = self.predicateMap.getPredicateId(pred[1])
                    try:
                        choiceBodyForHead_numeric[pred_h].append([pred[0],pred_b])
                    except:
                        choiceBodyForHead_numeric[pred_h]=[[pred[0],pred_b]]
        return head_numeric,choiceBodyForHead_numeric,body_numeric

    def parseProgram(self,filename):
        start=time.time()
        parsedProgram=ParsedProgram()
        parser=ExternalCalls.callProgramParser(filename)                
        for line in parser.stdout:
            field=line.decode("UTF-8").strip().split(ASP_TO_JSON_SEPARATOR)
            if int(field[0]) == ASP_TO_JSON_FACT_PREFIX:
                parsedProgram.facts.append(field[1])
            else:
                rule=json.loads("{\"0\":"+field[1]+"}")["0"]
                head,choiceForHead,body=self.parseRule(rule[0],rule[1],rule[2])
                parsedProgram.addParsedRule(head,choiceForHead,body)
                parsedProgram.addRuleAsStr(rule[3])
        parser.communicate()
        result = parser.returncode
        self.debugger.printMessage(f"Result parsing level {self.programCount}: {result}")
        self.debugger.printMessage(f"Parsing level {self.programCount} time  : {time.time()-start}")
        return parsedProgram if result==180 else None

    def buildPushSubprogram(self, program:ParsedProgram):
    
        #spi
        start_time=time.time()
        spi = program.findMaximalStratifiedSubProgram()
        end_time=time.time()
        
        #prints Pi\Spi on file and compute wellfounded model
        i=0
        pi_handler = open(SubProgramParser.SIMPLIFIED_FORALL_FILE,"w")
        spi_handler = open(SubProgramParser.OMITTED_FORALL_FILE,"w")
        for j in range(len(program.rules)):
            if i>=len(spi) or j < spi[i]:
                pi_handler.write(f"{program.rules[j]}\n")
            else:
                spi_handler.write(f"{program.rules[spi[i]]}\n")
                i+=1

        for fact in program.facts:
            pi_handler.write(f"{fact}\n")

        pi_handler.close() 
        spi_handler.close()
        
        self.addInterface(program,SubProgramParser.SIMPLIFIED_FORALL_FILE)

        #evaluate if it is the case to push subprogram one level below or not
        simplify = True
        
        self.debugger.printMessage("-------------------------------------DLV2 simplified forall "+str(self.programCount)+" ... -------------------------------------\n\n"+self.debugcmd.getOutput(f"cat {SubProgramParser.SIMPLIFIED_FORALL_FILE}")+" \n")
        self.debugger.printMessage("-----------------------------------------------------------------------------------------------------------")
        childProcess = ExternalCalls.callDLV2(SubProgramParser.SIMPLIFIED_FORALL_FILE)
        coherent, stream,true,undef,false = self.wellfounded(childProcess.stdout)
        found_zero = False
        for line in stream:
            if not simplify or found_zero:
                continue
            line = line.decode("UTF-8") 
            
            if line.strip() == "0":
                found_zero=True
                continue
            
            data = [int(x) for x in line.split(" ")]

            #is choice fact
            if data[LPARSE_FORMAT.RULE_TYPE_INDEX] == LPARSE_FORMAT.CHOICE_RULE:
                head_len = data[LPARSE_FORMAT.HEAD_LENGHT_INDEX]
                if data[LPARSE_FORMAT.HEAD_LENGHT_INDEX+head_len+1] == 0:
                    continue

            elif data[LPARSE_FORMAT.RULE_TYPE_INDEX] == LPARSE_FORMAT.SIMPLE_RULE:
                if data[LPARSE_FORMAT.ONE_HEAD_ATOM_INDEX] == 1:
                    #is a 1 1 1 0 1
                    if data[LPARSE_FORMAT.BODY_SIZE_INDEX] == 1 and data[LPARSE_FORMAT.NEG_BODY_SIZE_INDEX]==0 and data[LPARSE_FORMAT.NEG_BODY_SIZE_INDEX+1]==1:
                        continue
                else:
                    #is a fact 
                    if data[LPARSE_FORMAT.BODY_SIZE_INDEX] == 0:
                        continue
            print("Forall cannot be simplified:",line.strip())
            simplify=False
        
        childProcess.communicate()
        if childProcess.returncode != 0:
            print(subprocess.getoutput(f"cat {FILE_UTIL.SIMPLIFIED_FORALL_FILE}"))
            print("Error happened computing well founded",childProcess.returncode)
            sys.exit(180)
        
        if simplify:
            # guess only choice after well founded 
            current_symbols=[]
            moved_symbols=[]
            moved_interface=[]

            pushSubProgram = ParsedProgram()
            i=0
            #each rule in program is either a normal rule or choice rule
            for j in range(len(program.rules)):
                if i>=len(spi) or j < spi[i]:
                    pass
                else:
                    rule = program.parsed_rules[spi[i]]
                    pushSubProgram.addParsedRule(rule[0],rule[1],rule[2])
                    pushSubProgram.addRuleAsStr(program.rules[spi[i]])
                    i+=1

            spi_handler = open(SubProgramParser.OMITTED_FORALL_FILE,"a")

            #contains true supported atoms in Pi\SPi
            #moved subprogram contains atom. for atom in true no need to add to the interface
            for atom in true:
                id_,lev,truth,added = self.symbols.addSymbol(atom,self.programCount+1)
                if added:
                    #atoms of guess subrogram always true
                    moved_symbols.append(str(id_))
                    pushSubProgram.addRuleAsStr(f"{atom}.")
                    pushSubProgram.addParsedRule([self.predicateMap.getPredicateId(atom.split("(")[0])],None,None)
                    self.symbols.assign(atom,SymbolTable.TRUE,self.programCount+1)

                spi_handler.write(f"{atom}.\n")
            
            for atom in undef:
                id_,lev,truthValue,added = self.symbols.addSymbol(atom,self.programCount)
                if added:
                    current_symbols.append(str(id_))
                spi_handler.write("{"+atom+"}.\n")
            
            spi_handler.close()
            spi_true = true
            spi_undef = undef
            spi_false = []

            if len(spi) > 0:
                self.debugger.printMessage("-------------------------------------DLV2 moved forall "+str(self.programCount)+" ... -------------------------------------\n\n"+self.debugcmd.getOutput(f"cat {SubProgramParser.OMITTED_FORALL_FILE}")+" \n")
                self.debugger.printMessage("-----------------------------------------------------------------------------------------------------------")
                childProcess = ExternalCalls.callDLV2(SubProgramParser.OMITTED_FORALL_FILE)
                coherent, stream,spi_true,spi_undef,spi_false = self.wellfounded(childProcess.stdout)
                childProcess.communicate()
                if childProcess.returncode != 0:
                    print(subprocess.getoutput(f"cat {FILE_UTIL.OMITTED_FORALL_FILE}"))
                    print("Error happened computing well founded",childProcess.returncode)
                    sys.exit(180)
            
            if coherent:
                #adding constraint to pushed subprogram
                moved_interface.append("%True")
                for atom in spi_true:
                    pred=self.predicateMap.getPredicateId(atom.split("(")[0])
                    #every atom that is not in the factory is an atom defined in SPi so it is moved
                    id_,lev,truth,added = self.symbols.addSymbol(atom,self.programCount+1)
                    pushSubProgram.addRuleAsStr(f":- not {atom}.")
                    pushSubProgram.addParsedRule(None,None,[[True,pred]])
                        
                    if lev == self.programCount:
                        moved_interface.append("{"+atom+"}.")
                    
                    if added:
                        #level of atom if programCount+1
                        #if it is false Pi is not satisfied
                        moved_symbols.append(str(id_))

                    # self.symbols.assign(atom,SymbolTable.TRUE,self.programCount+1)

                moved_interface.append("%False")
                for atom in spi_false:
                    
                    pred=self.predicateMap.getPredicateId(atom.split("(")[0])
                    id_,lev,truth,added = self.symbols.addSymbol(atom,self.programCount+1)
                    pushSubProgram.addRuleAsStr(f":- {atom}.")
                    pushSubProgram.addParsedRule(None,None,[[False,pred]])
                    if lev == self.programCount:
                        moved_interface.append("{"+atom+"}.")

                    if added:
                        #level of atom if programCount+1
                        #if it is true Pi is not satisfied
                        moved_symbols.append(str(id_))

                    # self.symbols.assign(atom,SymbolTable.FALSE,self.programCount+1)

                moved_interface.append("%Undef")
                for atom in spi_undef:
                    pred=self.predicateMap.getPredicateId(atom.split("(")[0])
                    id_,lev,truth,added = self.symbols.addSymbol(atom,self.programCount+1)
                    if added or lev == self.programCount:
                        if lev == self.programCount:
                            moved_interface.append("{"+atom+"}.")
                        if added:
                            moved_symbols.append(str(id_))

                return True,pushSubProgram,current_symbols,None,moved_symbols,moved_interface

            return False,None,None,None,moved_symbols,moved_interface

        #compute well founded model of Pi
        phi_i = None
        
        self.debugger.printMessage("-------------------------------------DLV2 entire forall "+str(self.programCount)+" ... -------------------------------------\n\n"+self.debugcmd.getOutput(f"cat {SubProgramParser.SIMPLIFIED_FORALL_FILE} {SubProgramParser.OMITTED_FORALL_FILE}")+" \n")
        self.debugger.printMessage("-----------------------------------------------------------------------------------------------------------")

        childProcess = ExternalCalls.callDLV2MultipleInput([SubProgramParser.SIMPLIFIED_FORALL_FILE,SubProgramParser.OMITTED_FORALL_FILE])
        coherent, stream,true,undef,false = self.wellfounded(childProcess.stdout)
        
        if stream is None:
            print("Error reading wellfounded model")
            sys.exit(180)

        if coherent:
            
            wellfounded_clauses=[]
            current_symbols=[]
            for atom in true:
                id_,lev,truth,added = self.symbols.addSymbol(atom,self.programCount)
                if added:
                    current_symbols.append(str(id_))
                    # wellfounded_clauses.append(f"or({id_})")
                self.symbols.assign(atom,SymbolTable.TRUE,self.programCount)    
                    
            for atom in undef:
                id_,lev,truth,added = self.symbols.addSymbol(atom,self.programCount)
                if added:
                    current_symbols.append(str(id_))
                
            for atom in false:
                id_,lev,truth,added = self.symbols.addSymbol(atom,self.programCount)
                if added:
                    current_symbols.append(str(id_))
                wellfounded_clauses.append(f"or({-id_})")
                self.symbols.assign(atom,SymbolTable.FALSE,self.programCount)    

            # print("Encoding ...\n\n",subprocess.getoutput(f"cat {SubProgramParser.SIMPLIFIED_FORALL_FILE} {SubProgramParser.OMITTED_FORALL_FILE}")," \n")
            phi_i,extras = self.encodeResidual(stream,program.checkTight(),wellfounded_clauses)    
            childProcess.communicate()
            if childProcess.returncode != 0:
                print("Error happened computing well founded",childProcess.returncode)
                sys.exit(180)

            current_symbols+=extras
            symbols = ",".join(current_symbols) 
            f = open(FILE_UTIL.QBF_PROGRAM_FILE,"a")
            f.write(f"{QCIR_FORMAT.FORALL}({symbols})\n")
            f.close()
            return coherent,None,current_symbols,phi_i,[],[]
        
        return coherent,None,None,phi_i,[],[]

    def residualToCnf(self,disjunction,tight,wellfoundedClauses=[]):
        self.debugger.printMessage(f"Writing intermediated cnf on {FILE_UTIL.QCIR_SUB_FORMULA_PREFIX}_{self.programCount}.qcir")
        f = open(f"{FILE_UTIL.QCIR_SUB_FORMULA_PREFIX}_{self.programCount}.qcir","w")
        childProcess = ExternalCalls.callPipeline(FILE_UTIL.GROUND_PROGRAM_FILE,disjunction,tight)
        variableMapping = {}
        gates=[]
        extras=[]

        for clause in wellfoundedClauses:
            gate = self.symbols.addExtraSymbol()
            f.write(f"{gate} = {clause}\n")
            gates.append(str(gate))
            self.gatesCount+=1
            self.clauseCount+=1
        for line in childProcess.stdout:
            line = line.decode("UTF-8").strip()
            self.debugger.printMessage(line)
            fields = line.split(" ")
            if fields[0] != "p":
                if fields[0] == "c":
                    #propositional atoms not present in the factory, at this point, are meant to be atoms added by external pipeline (_acyc_*)
                    addExtra = "(" not in fields[2] and len(self.symbols.getPredicateDomain(fields[2]))==0
                    
                    id_,level,truth,added = self.symbols.addSymbol(fields[2],self.programCount)
                    current_id = int(fields[1])
                    variableMapping[current_id]=id_
                    if added and addExtra:
                        extras.append(str(id_))                            
                    
                    # if added:
                        # if truth == SymbolTable.TRUE:
                        #     gate = self.symbols.addExtraSymbol()
                        #     f.write(f"{gate}=or({id_})\n")
                        #     gates.append(str(gate))
                        #     self.gatesCount+=1
                        #     self.clauseCount+=1
                        # elif truth != SymbolTable.UNDEF:
                        #     gate = self.symbols.addExtraSymbol()
                        #     f.write(f"{gate}=or({-id_})\n")
                        #     gates.append(str(gate))
                        #     self.gatesCount+=1
                        #     self.clauseCount+=1
                            
                else:
                    rewrittenClause=[]
                    for id_str in fields[:-1]:
                        id_ = int(id_str)
                        var = id_ if id_>=0 else -id_
                        try:
                            rewrittenClause.append(str(variableMapping[var]) if id_ >=0 else str(-variableMapping[var]))
                        except:
                            v=self.symbols.addExtraSymbol()
                            extras.append(str(v))
                            variableMapping[var]=v
                            rewrittenClause.append(str(v) if id_ >=0 else str(-v))
                    clause=",".join(rewrittenClause)
                    gate = self.symbols.addExtraSymbol()
                    f.write(f"{gate} = or({clause})\n")
                    gates.append(str(gate))
                    self.gatesCount+=1
                    self.clauseCount+=1
        
        phi_i = self.symbols.addExtraSymbol()
        self.gatesCount+=1
        conjunction = ",".join(gates)
        f.write(f"{phi_i} = and({conjunction})")
        f.close()
        childProcess.communicate()
        if childProcess.returncode != 0:
            print("Error calling external pipeline")
            sys.exit(180)
        return phi_i,extras
        
    def encodeResidual(self,stream,tight,wellfoundedClauses=[]):
        f=open(FILE_UTIL.GROUND_PROGRAM_FILE,"w")
        disjunction = False
        end=False
        for line in stream:
            line = line.decode("UTF-8").strip()
            if line == "0":
                end=True
            elif not end:
                data = [int(x) for x in line.split(" ")]
                if data[LPARSE_FORMAT.RULE_TYPE_INDEX] == LPARSE_FORMAT.DISJCUNTIVE_RULE:
                    disjunction = True
            f.write(f"{line}\n")
        f.close()
        
        # print("Encoding ...\n\n",subprocess.getoutput(f"cat {FILE_UTIL.GROUND_PROGRAM_FILE}"),"\n")
            
        return self.residualToCnf(disjunction,tight,wellfoundedClauses)
        # print("\n")        

    def encodeSmodels(self,stream):
        f=open(FILE_UTIL.GROUND_PROGRAM_FILE,"w")
        disjunction = False
        endProgram  = False
        endTable    = False
        coherent    = True
        current_symbols = []
        for line in stream:
            line = line.decode("UTF-8").strip()
            if line == "0":
                if not endProgram:
                    endProgram=True
                else:
                    endTable=True
            elif not endProgram:
                if line.strip() == "1 1 0 0":
                    coherent=False
                else:
                    data = [int(x) for x in line.split(" ")]
                    if data[LPARSE_FORMAT.RULE_TYPE_INDEX] == LPARSE_FORMAT.DISJCUNTIVE_RULE:
                        disjunction = True
            elif not endTable:
                id_,lev,truth,added = self.symbols.addSymbol(line.split(" ")[1],self.programCount)
                if added:
                    current_symbols.append(str(id_))                

            f.write(f"{line}\n")
        f.close()
        # print("----------------------------- Grounded level -----------------------------")
        # print(subprocess.getoutput(f"cat {FILE_UTIL.GROUND_PROGRAM_FILE} > ground.{self.programCount}"))
        # print("--------------------------------------------------------------------------")
        return coherent,disjunction,current_symbols

    def wellfounded(self,stream):
        #reading well founded model
        true=None
        undef=None
        false=None
        coherent = True
        for line in stream:
            line = line.decode("UTF-8")
            if line.strip() == "1 1 0 0":
                coherent=False
                break
            elif line.startswith("S"):
                continue
            elif line.startswith("T"):
                true = line.split("{")[1].split("}")[0].strip().split(", ")
                if true[-1]=="":
                    true=true[:-1]
            elif line.startswith("U"):
                undef = line.split("{")[1].split("}")[0].strip().split(", ")
                if undef[-1]=="":
                    undef=undef[:-1]
            elif line.startswith("F"):
                false = line.split("{")[1].split("}")[0].strip().split(", ")
                if false[-1]=="":
                    false=false[:-1]
            elif line.startswith("E"):
                return coherent,stream,true,undef,false

        return coherent,stream,None,None,None

    def addInterface(self,program,filename,movedInterface=[]):
        f=open(filename,"a")
        program.labelPredicates(self.predicateMap)
        
        #TODO add check previously defined predicate: problem while reading programs with subprogram from previous level
        f.write("%Previous level.\n")
        for predicate in program.bodyPredicates+program.headPredicates:
            domain = self.symbols.getPredicateDomain(predicate)
            for atom,data in domain:
                id_,level = data
                if level >= self.programCount or (not self.pushSubProgram is None and level == self.forallLevel):
                    continue 
                truth = self.symbols.getTruthValue(id_)
                if truth == SymbolTable.UNDEF:
                    f.write("{"+atom+"}.\n")
                elif truth == SymbolTable.TRUE:
                    f.write(f"{atom}.\n")
        f.write("%Moved interface.\n")
        f.write("\n".join(movedInterface)+"\n")
        f.close()
    
    def ground(self,program,filename,addMovedSymbols,addMovedInterface):
        
        self.addInterface(program,filename,self.movedInterface if addMovedInterface else [])
        self.debugger.printMessage("-------------------------------------DLV2 "+str(self.programCount)+" ... -------------------------------------\n\n"+self.debugcmd.getOutput(f"cat {filename}")+" \n")
        self.debugger.printMessage("------------------------------------------------------------------------------------------")
        
        if not DEFAULT_PROPERTIES.NO_WF:
            childProcess = ExternalCalls.callDLV2(filename)
            coherent,stream,true,undef,false = self.wellfounded(childProcess.stdout)
            wellfounded_clauses=[]
            current_symbols = []
            # print("WF_T",true)
            # print("WF_U",undef)
            # print("WF_F",false)
            if coherent:
                for atom in true:
                    id_,lev,truth,added = self.symbols.addSymbol(atom,self.programCount)
                    if added:
                        #wellfounded_clauses.append(f"or({id_})")
                        current_symbols.append(str(id_))
                    self.symbols.assign(atom,SymbolTable.TRUE,self.programCount)
                        
                for atom in false:
                    id_,lev,truth,added = self.symbols.addSymbol(atom,self.programCount)
                    if added:
                        current_symbols.append(str(id_))
                    self.symbols.assign(atom,SymbolTable.FALSE,self.programCount)
                    wellfounded_clauses.append(f"or({-id_})")
                        
                for atom in undef:
                    id_,lev,truth,added = self.symbols.addSymbol(atom,self.programCount)
                    if added:
                        current_symbols.append(str(id_))
            
            
            phi_i = None
            if coherent:
                if stream is None:
                    print("Error reading stream after wellfounded model")
                    sys.exit(180)
                phi_i,extras=self.encodeResidual(stream,program.checkTight(),wellfounded_clauses)
                symbols = ",".join(current_symbols+extras if not addMovedSymbols else current_symbols+extras+self.movedSymbols) 
                f = open(FILE_UTIL.QBF_PROGRAM_FILE,"a")
                quant = QCIR_FORMAT.EXISTS if self.currentQuantifier != QASP_FORMAT.QFORALL else QCIR_FORMAT.FORALL
                f.write(f"{quant}({symbols})\n")
                f.close()
            childProcess.communicate() 
            if childProcess.returncode != 0:
                print("Error happened computing well founded",childProcess.returncode)
                sys.exit(180)
            return coherent,phi_i
        else:

            childProcess=None
            if self.grounder_backend=="gringo":
                print("Grounding using gringo")
                childProcess = ExternalCalls.callGringo(filename)
            else:
                print("Grounding using idlv")
                childProcess = ExternalCalls.callIDLV(filename)
                
            coherent, disjunction, current_symbols = self.encodeSmodels(childProcess.stdout)
            
            childProcess.communicate() 
            if childProcess.returncode != 0:
                print("Error happened during grounding",childProcess.returncode)
                sys.exit(180)
            
            phi_i = None
            if coherent:
                phi_i,extras=self.residualToCnf(disjunction,program.checkTight(),[])
                symbols = ",".join(current_symbols+extras if not addMovedSymbols else current_symbols+extras+self.movedSymbols) 
                f = open(FILE_UTIL.QBF_PROGRAM_FILE,"a")
                quant = QCIR_FORMAT.EXISTS if self.currentQuantifier != QASP_FORMAT.QFORALL else QCIR_FORMAT.FORALL
                f.write(f"{quant}({symbols})\n")
                f.close()
            return coherent,phi_i
        
    def buildSubPrograms(self):
        print("Parsing ...",self.qaspFile)
        self.stopEncoding=False
        self.encodedLevel = [None]
        f = open(self.qaspFile,"r")
        self.currentQuantifier = None
        parsedProgram = None
        self.gates=[]
        
        finalQBF = open(FILE_UTIL.QBF_PROGRAM_FILE,"w")
        finalQBF.write(f"{QCIR_FORMAT.HEADER}\n")
        finalQBF.close()
        
        parserFileHandler=None
        for line in f:
            if len(line.strip()) == 0:
                continue
            
            matchQuantifier = re.match(REGEX_UTIL.QASP_QUANTIFIER,line)
            
            if matchQuantifier:
                newQuantifier = matchQuantifier.group(1)
                self.debugger.printMessage(f"Found quantifier {newQuantifier}")
                if not self.currentQuantifier is None:
                    if newQuantifier == self.currentQuantifier:
                        continue
        
                    parserFileHandler.close()
                    parsedProgram = self.parseProgram(FILE_UTIL.ASP_PARSER_FILE)
                    if parsedProgram is None:
                        print("Error during parsing level",self.programCount)
                        sys.exit(180)
                    start_encoding_time=time.time()
                    if self.currentQuantifier == QASP_FORMAT.QFORALL:
                        start_time=time.time()
                        if not self.addUnsat is None:
                            parsedProgram.addParsedRule(None,None,[[False,self.predicateMap.getPredicateId(SubProgramParser.AUX_PREDICATE+str(self.forallLevel))]])
                            parsedProgram.addRuleAsStr(f":- {SubProgramParser.AUX_PREDICATE+str(self.forallLevel)}.")

                        self.pushSubProgram = None
                        self.addUnsat = None
                        self.movedSymbols = []

                        self.isLastQuantifierExists = False
                        if DEFAULT_PROPERTIES.GUESS_CHECK:
                            coherent, subProgram, currentSymbols, phi_i,movedSymbols,movedInterface = self.buildPushSubprogram(parsedProgram)
                            
                            if coherent:
                                self.gates.append(phi_i)
                                if subProgram is None:
                                    # Pi already encoded in cnf
                                    self.encodedLevel.append(SubProgramParser.ENCODED_F)
                                else:
                                    #quantifies symbols
                                    if len(currentSymbols) > 0:
                                        symbols = ",".join(currentSymbols) 
                                        f = open(FILE_UTIL.QBF_PROGRAM_FILE,"a")
                                        f.write(f"{self.currentQuantifier}({symbols})\n")
                                        f.close()
                                        self.encodedLevel.append(SubProgramParser.SKIPPED)
                                        if len(subProgram.rules) > 0:
                                            self.pushSubProgram = subProgram    
                                            self.forallLevel = self.programCount
                                            self.addUnsat = True
                                            self.movedSymbols=movedSymbols
                                            self.movedInterface=movedInterface
                                    else:
                                        self.encodedLevel.append(SubProgramParser.EMPTY)
                                        
                            else:
                                print("Incoherent forall",self.programCount)
                                phi_i=self.symbols.addExtraSymbol()
                                self.gates.append(phi_i)
                                self.gatesCount+=1
                                f = open(f"{FILE_UTIL.QCIR_SUB_FORMULA_PREFIX}_{self.programCount}.qcir","w")
                                f.write(f"{phi_i} = and()\n")
                                f.close()
                                self.encodedLevel.append(SubProgramParser.INCOHERENT_F)
                                self.stopEncoding=True
                                break
                        else:
                            f = open(FILE_UTIL.TO_GROUND_PROGRAM_FILE,"w")
                            for fact in parsedProgram.facts:
                                f.write(f"{fact}\n")
                            for rulestr in parsedProgram.rules:
                                f.write(f"{rulestr}\n")
                            f.close()
                            coherent,phi_i = self.ground(parsedProgram,FILE_UTIL.TO_GROUND_PROGRAM_FILE,False,False)
                            if not coherent:
                                print("Incoherent forall",self.programCount)
                                phi_i=self.symbols.addExtraSymbol()
                                self.gates.append(phi_i)
                                self.gatesCount+=1
                                f = open(f"{FILE_UTIL.QCIR_SUB_FORMULA_PREFIX}_{self.programCount}.qcir","w")
                                f.write(f"{phi_i} = and( )\n")
                                f.close()
                                self.encodedLevel.append(SubProgramParser.INCOHERENT_F)
                                self.stopEncoding=True
                                break
                            else:
                                self.gates.append(phi_i)
                                self.encodedLevel.append(SubProgramParser.ENCODED_F)
                            
                    elif self.currentQuantifier == QASP_FORMAT.QEXISTS:
                        
                        start_time=time.time()
                        self.isLastQuantifierExists = True
                        addMovedSymbols=False
                        addMovedInterface=False
                        toGround=None
                        #Pi is exists
                        if not self.pushSubProgram is None:
                            
                            f = open(FILE_UTIL.TO_GROUND_PROGRAM_FILE,"w")
                            #Pi' 
                            augmentedExists = ParsedProgram()
                            
                            # SPi
                            existsSubprogram = parsedProgram.findMaximalStratifiedSubProgram()
                            
                            i=0
                            for j in range(len(parsedProgram.rules)):
                                if i >= len(existsSubprogram) or j < existsSubprogram[i]:
                                    #rule in Pi\SPi
                                    rule = parsedProgram.parsed_rules[j]
                                    rulestr = parsedProgram.rules[j]
                                    # Hr:-Br. in SPi -> :-Br, not unsat.
                                    lit_str=[True,SubProgramParser.AUX_PREDICATE+str(self.forallLevel)]
                                    lit = [True,self.predicateMap.getPredicateId(SubProgramParser.AUX_PREDICATE+str(self.forallLevel))]
                                    sep=","
                                    if rule[2] is None:
                                        rule[2] = []
                                        sep = ":-"

                                    rule[2].append(lit)
                                    rulestr = f"{rulestr[:-1]}{sep} not {lit_str[1]}."
                                    augmentedExists.addParsedRule(rule[0],rule[1],rule[2])
                                    # augmentedExists.addRuleAsStr(rulestr,f)
                                    f.write(f"{rulestr}\n")
                                else:
                                    #rule in SPi
                                    ruleid = existsSubprogram[i]
                                    rule = parsedProgram.parsed_rules[ruleid]
                                    rulestr = parsedProgram.rules[ruleid]
                                    head = rule[0]
                                    if head is None or len(head) == 0:
                                        #:-Br. in SPi -> :-Br, not unsat.
                                        lit = [True,self.predicateMap.getPredicateId(SubProgramParser.AUX_PREDICATE+str(self.forallLevel))]
                                        lit_str = [True,SubProgramParser.AUX_PREDICATE+str(self.forallLevel)]
                                        rule[2].append(lit)
                                        rulestr = f"{rulestr[:-1]}, not {lit_str[1]}."
                                    
                                    augmentedExists.addParsedRule(rule[0],rule[1],rule[2])
                                    # augmentedExists.addRuleAsStr(rulestr,f)
                                    f.write(f"{rulestr}\n")
                                    i+=1
                            
                            ruleid = 0
                            for rule in self.pushSubProgram.parsed_rules:
                                rulestr = self.pushSubProgram.rules[ruleid]
                                #:-Br. in SPi -> unsat :-Br.
                                if rule[0] is None:
                                    rule[0]=[]
                                if len(rule[0]) == 0:
                                    rule[0].append(self.predicateMap.getPredicateId(SubProgramParser.AUX_PREDICATE+str(self.forallLevel)))
                                    rulestr = SubProgramParser.AUX_PREDICATE+str(self.forallLevel)+rulestr
                                augmentedExists.addParsedRule(rule[0],rule[1],rule[2])
                                # augmentedExists.addRuleAsStr(rulestr,f)
                                f.write(f"{rulestr}\n")

                                ruleid+=1
                        
                            for fact in parsedProgram.facts:
                                f.write(f"{fact}\n")

                            toGround=augmentedExists
                            addMovedSymbols=True
                            addMovedInterface=True
                            f.close()
                            # print("Augmented exists",self.programCount,"with previous forall")
                            # toGround.printProgram()
                            # print("---------------------------------------------------------------")

                        else:
                            f = open(FILE_UTIL.TO_GROUND_PROGRAM_FILE,"w")
                            for rulestr in parsedProgram.rules:
                                f.write(f"{rulestr}\n")
                            for fact in parsedProgram.facts:
                                f.write(f"{fact}\n")
                            f.close()
                            toGround=parsedProgram
                            # print("Exists",self.programCount)
                            # toGround.printProgram()
                            # print("---------------------------------------------------------------")

                        
                        coherent,phi_i = self.ground(toGround,FILE_UTIL.TO_GROUND_PROGRAM_FILE,addMovedSymbols,addMovedInterface)
                        self.pushSubProgram = None
                        if not coherent:
                            self.encodedLevel.append(SubProgramParser.INCOHERENT_E)
                            phi_i=self.symbols.addExtraSymbol()
                            self.gates.append(phi_i)
                            self.gatesCount+=1
                            f = open(f"{FILE_UTIL.QCIR_SUB_FORMULA_PREFIX}_{self.programCount}.qcir","w")
                            f.write(f"{phi_i} = or( )\n")
                            f.close()
                            self.clauseCount+=1

                            self.stopEncoding=True
                            print("Incoherent Exists",self.programCount)
                            break
                        else:
                            self.gates.append(phi_i)
                            self.encodedLevel.append(SubProgramParser.ENCODED_E)

                    else:
                        if self.currentQuantifier == QASP_FORMAT.QCONSTRAINT:
                            print("Error: New quantifier found after constraint")
                            sys.exit(180)
                        else:
                            print("Error: Unknown quantifier",self.currentQuantifier)
                            sys.exit(180)

                    # print(f"Encoding time level {self.programCount}:{time.time()-start_encoding_time}")  
                parserFileHandler=open(FILE_UTIL.ASP_PARSER_FILE,"w")
                parsedProgram = ParsedProgram()
                self.currentQuantifier = newQuantifier
                self.programCount += 1
                continue
            
            #current line does not match quantifier
            if self.currentQuantifier is None:
                print("No Quantifier found yet: Skipping",line)
            else:
                parserFileHandler.write(line)
                
        
        if not self.stopEncoding:
            parsedProgram=None
            if parserFileHandler!=None:
                parserFileHandler.close()
                parsedProgram=self.parseProgram(FILE_UTIL.ASP_PARSER_FILE)
                if parsedProgram is None:
                    print("Error during parsing level",self.programCount)
                    sys.exit(180)
            start_encoding_time=time.time()
            if self.currentQuantifier == QASP_FORMAT.QCONSTRAINT:
                start_time=time.time()
                #encodedLevel contains the encodings strategy of previous level: 0 is a fake level
                lastForall=None
                isLastForallEncoded=False
                toGround=None
                for i in range(len(self.encodedLevel)-1,0,-1):
                    if self.encodedLevel[i] == SubProgramParser.SKIPPED:
                        lastForall=i
                        isLastForallEncoded=False
                        break
                    elif self.encodedLevel[i] == SubProgramParser.ENCODED_F:
                        lastForall=i
                        isLastForallEncoded=True
                        break
                addMovedSymbols=False
                addMovedInterface=False
                if lastForall is None or isLastForallEncoded:
                    f = open(FILE_UTIL.TO_GROUND_PROGRAM_FILE,"w")
                    for rule in parsedProgram.rules:
                        f.write(rule+"\n")
                    for fact in parsedProgram.facts:
                        f.write(f"{fact}\n")
                    f.close()
                    toGround=parsedProgram
                    # print("Constraint")
                    # parsedProgram.printProgram()
                    # print("---------------------------------------------------------------")

                else:
                    f = open(FILE_UTIL.TO_GROUND_PROGRAM_FILE,"w")
                    augmentedExists = ParsedProgram()
                    for fact in parsedProgram.facts:
                        f.write(f"{fact}\n")
                    for j in range(len(parsedProgram.rules)):
                        rule = parsedProgram.parsed_rules[j]
                        rulestr = parsedProgram.rules[j]
                        head = rule[0]
                        if head is None or len(head) == 0:
                            #:-Br. in SPi -> :-Br, not unsat.
                            lit = [True,self.predicateMap.getPredicateId(SubProgramParser.AUX_PREDICATE+str(lastForall))]
                            lit_str = [True,SubProgramParser.AUX_PREDICATE+str(lastForall)]
                            rule[2].append(lit)
                            rulestr = f"{rulestr[:-1]}, not {lit_str[1]}."
                        augmentedExists.addParsedRule(rule[0],rule[1],rule[2])
                        # augmentedExists.addRuleAsStr(rulestr,f)
                        f.write(f"{rulestr}\n")
                    #By definition C is a stratified normal program with constraint
                    if not self.pushSubProgram is None:
                        addMovedSymbols=True
                        addMovedInterface=True
                        for j in range(len(self.pushSubProgram.parsed_rules)):
                            rule = self.pushSubProgram.parsed_rules[j]
                            rulestr = self.pushSubProgram.rules[j]
                        
                            #:-Br. in SPi -> unsat :-Br.
                            if rule[0] is None:
                                rule[0]=[]
                            if len(rule[0]) == 0:
                                rule[0].append(self.predicateMap.getPredicateId(SubProgramParser.AUX_PREDICATE+str(self.forallLevel)))
                                rulestr = f"{SubProgramParser.AUX_PREDICATE+str(self.forallLevel)}{rulestr}"
                            augmentedExists.addParsedRule(rule[0],rule[1],rule[2])
                            # augmentedExists.addRuleAsStr(rulestr,f)
                            f.write(f"{rulestr}\n")
                    toGround=augmentedExists
                    f.close()
                    

                coherent,phi_i = self.ground(toGround,FILE_UTIL.TO_GROUND_PROGRAM_FILE,addMovedSymbols,addMovedInterface)
                if not coherent:
                    phi_i = self.symbols.addExtraSymbol()
                    self.gatesCount+=1
                    f = open(f"{FILE_UTIL.QCIR_SUB_FORMULA_PREFIX}_{self.programCount}.qcir","w")
                    f.write(f"{phi_i} = or( )\n")
                    f.close()
                    self.clauseCount+=1
                    self.encodedLevel.append(SubProgramParser.INCOHERENT_C)
                    print("Incoherent constraint")
                else:
                    self.encodedLevel.append(SubProgramParser.ENCODED_C)
                self.gates.append(phi_i)

            # print("Encoding time constraint:",time.time()-start_encoding_time)  
        variablesCount=self.symbols.idCounter-self.gatesCount

        phi_c=""
        quiteCNF=True
        finalizing=[]
        
        
        for i in range(len(self.encodedLevel)-1,0,-1):
            if self.encodedLevel[i] in [SubProgramParser.INCOHERENT_C,SubProgramParser.INCOHERENT_E,SubProgramParser.INCOHERENT_F]:
                phi_c = f"{self.gates[i-1]}"
            elif self.encodedLevel[i] in [SubProgramParser.ENCODED_C,SubProgramParser.ENCODED_E] :
                if phi_c == "":
                    phi_c = self.gates[i-1]  
                else:
                    next_=self.symbols.addExtraSymbol()
                    finalizing.append(f"{next_} = and({self.gates[i-1]},{phi_c})")
                    phi_c = f"{next_}"
            elif self.encodedLevel[i] == SubProgramParser.ENCODED_F:

                if phi_c == "":
                    phi_c = -self.gates[i-1]
                else:
                    next_=self.symbols.addExtraSymbol()
                    finalizing.append(f"{next_} = or(-{self.gates[i-1]},{phi_c})")
                    phi_c = f"{next_}"
                quiteCNF=False
        if len(self.encodedLevel) == 2:
            if self.encodedLevel[1] == SubProgramParser.INCOHERENT_F:
                print(f"{PYQASP_OUTPUT.EXTENDED}10")
                sys.exit(10)
            elif self.encodedLevel[1] == SubProgramParser.INCOHERENT_E:
                print(f"{PYQASP_OUTPUT.EXTENDED}20")
                sys.exit(20)
            # else:
            #     print("Error: found only one level not incoherent")
            #     sys.exit(180)

        finalQBF=open(FILE_UTIL.QBF_PROGRAM_FILE,"a")
        finalQBF.write(f"{QCIR_FORMAT.OUTPUT}({phi_c})\n")
        for i in range(1,len(self.encodedLevel)):
            if self.encodedLevel[i] == SubProgramParser.SKIPPED:
                continue
            f=open(f"{FILE_UTIL.QCIR_SUB_FORMULA_PREFIX}_{i}.qcir","r")
            for line in f:
                finalQBF.write(f"{line.strip()}\n")
            f.close()
        for line in finalizing:
            finalQBF.write(f"{line.strip()}\n")
        finalQBF.close()
        
        props = QCIRProps()
        props.clauseCount = self.clauseCount
        props.variablesCount = variablesCount
        props.quiteCNF = quiteCNF
        props.levelsCount=self.programCount
        props.lastSymbol=self.symbols.idCounter-1
        return props

