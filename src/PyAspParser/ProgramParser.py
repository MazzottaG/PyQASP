from antlr4 import *
from PyAspParser.ASPCore2Lexer import ASPCore2Lexer
from PyAspParser.ASPCore2Parser import ASPCore2Parser
from PyAspParser.ASPCore2Listener import ASPCore2Listener
from PyAspParser.ProgramBuilder import RuleBuilder
# from ASPCore2Lexer import ASPCore2Lexer
# from ASPCore2Parser import ASPCore2Parser
# from ASPCore2Listener import ASPCore2Listener
# from ProgramBuilder import RuleBuilder
import sys,json,scc,re

class ProgramDependency:
    
    def __init__(self,predicateMapping):
        self.dependencyGraph = {}
        self.predicatesToId = predicateMapping

    def getId(self,pred):
        try:
            return self.predicatesToId[pred]
        except:
            print(f"Error: No id for {pred}")
            return -1

    def addNode(self,pred_id):
        if pred_id not in self.dependencyGraph:
            self.dependencyGraph[pred_id]=[]

    def addDependency(self,from_,to_):
        from_id = self.getId(from_)
        to_id = self.getId(to_)
        if from_id < 0 or to_id < 0:
            print(f"Error: No if for predicate :: unable to add dependency {from_} -> {to}")
            return
        try:
            self.dependencyGraph[from_id].add(to_id)
        except:
            self.dependencyGraph[from_id]=set([to_id])
        
        self.addNode(to_id)

class ParsedProgram:
    OUTPUT_DEFINING = 0
    OUTPUT_BODY = 1

    JSON_RULES = 0
    JSON_PARSED_RULES = 1
    JSON_PARSED_RULES_FOR_PRED = 2
    JSON_PREDICATE_ID = 3
    JSON_PREDICATE_COUNT = 4
    JSON_HEAD_PREDICATES = 5
    JSON_BODY_PREDICATES = 6


    def __init__(self):
        self.ASP_FACTS                   = r'([a-z][a-zA-Z0-9_]*(\s*\((\s*([a-z0-9A-Z][a-z0-9A-Z]*(\s*,\s*[a-z0-9A-Z][a-z0-9A-Z]*)*)\s*){0,1}\)){0,1}.\s*)+'
        
        # program rules as string
        self.rules = []
        self.facts = []

        # parsed rule struct: [head,choiceBodyForHead,body]
        self.parsed_rules = []
        
        # predicate_id: list[parsed_rules_id]
        self.parsed_rules_for_pred = {}

        # predicateName to integer index
        self.predicatesToId = {}
        self.predicateCount = 1
        self.headPredicates = None
        self.bodyPredicates = None  


    
    def writeToFile(self,filename):
        program = {}
        program[ParsedProgram.JSON_RULES] = self.rules 
        program[ParsedProgram.JSON_PARSED_RULES] = self.parsed_rules 
        program[ParsedProgram.JSON_PARSED_RULES_FOR_PRED] = self.parsed_rules_for_pred 
        program[ParsedProgram.JSON_PREDICATE_ID] = self.predicatesToId 
        program[ParsedProgram.JSON_PREDICATE_COUNT] = self.predicateCount 
        program[ParsedProgram.JSON_HEAD_PREDICATES] = self.headPredicates 
        program[ParsedProgram.JSON_BODY_PREDICATES] = self.bodyPredicates
        
        with open(filename,"w") as jsonfile:
            json.dump(program,jsonfile)
    
    def readProgramFromFile(self,filename):

        program = {}
        with open(filename,"r") as jsonfile:
            program = json.load(jsonfile)
        
        self.rules = program[str(ParsedProgram.JSON_RULES)]
        self.parsed_rules = program[str(ParsedProgram.JSON_PARSED_RULES)]
        self.parsed_rules_for_pred = {}
        subdict = program[str(ParsedProgram.JSON_PARSED_RULES_FOR_PRED)]
        for pred_id in subdict:
            self.parsed_rules_for_pred[int(pred_id)]=[int(x) for x in subdict[pred_id]]

        self.predicatesToId = {}
        subdict = program[str(ParsedProgram.JSON_PREDICATE_ID)]
        for pred_name in subdict:
            self.predicatesToId[pred_name]=int(subdict[pred_name])

        self.predicateCount = program[str(ParsedProgram.JSON_PREDICATE_COUNT)]
        self.headPredicates = program[str(ParsedProgram.JSON_HEAD_PREDICATES)]
        self.bodyPredicates = program[str(ParsedProgram.JSON_BODY_PREDICATES)]      
        
    def labelPredicates(self):
        
        self.headPredicates = []
        self.bodyPredicates = []
        first = True
        for pred in self.predicatesToId:
            id_ = self.predicatesToId[pred]
            if id_ in self.parsed_rules_for_pred:
                self.headPredicates.append(pred)
            else:
                self.bodyPredicates.append(pred)

    def printPredicates(self):
        
        self.labelPredicates()

        headPredicates = ",".join(self.headPredicates)
        bodyPredicates = ",".join(self.bodyPredicates)
        
        print(f"{Program.OUTPUT_DEFINING}:{headPredicates}")
        print(f"{Program.OUTPUT_BODY}:{bodyPredicates}")

    def getPredicateId(self,predicate):
        try:
            return self.predicatesToId[predicate]
        except:
            pred_id = self.predicateCount
            self.predicatesToId[predicate] = self.predicateCount
            self.predicateCount+=1
            return pred_id
                
    def addParsedRule(self,head,choiceBodyForHead,body):
        ruleIndex = len(self.parsed_rules)
        # self.parsed_rules.append([head,choiceBodyForHead,body])
        
        if not body is None:
            for pred in body:
                self.getPredicateId(pred[1])
        
        if not choiceBodyForHead is None:
            for h in choiceBodyForHead:
                for pred in choiceBodyForHead[h]:
                    self.getPredicateId(pred[1])
        
        if head is None:
            return
        
        for headAtom in head:
            head_id = self.getPredicateId(headAtom)
            try:
                self.parsed_rules_for_pred[head_id].append(ruleIndex)
            except:
                self.parsed_rules_for_pred[head_id]=[ruleIndex]

    def addRuleAsStr(self,rule_str,fileHandler=None):
        if not fileHandler is None:
            fileHandler.write(f"{rule_str}\n")
        self.rules.append(rule_str)
    
    def printProgram(self):
        for rule in self.rules:
            print(rule)

    def parseRule(self,line):
        # line=line.strip()
        # if re.fullmatch(self.ASP_FACTS,line):
        #     for fact in re.split(r"\.\s*",line):
        #         self.facts.append(f"{fact.strip()}.")
        #     return
        input_stream = InputStream(line)
        lexer = ASPCore2Lexer(input_stream)
        stream = CommonTokenStream(lexer)
        parser = ASPCore2Parser(stream)
        parser.buildParseTrees = False
        builder = RuleBuilder()
        parser.addParseListener(builder)
        parser.program()
        
        # [head,choiceBodyForHead,body]
        for (rule,rulestr) in builder.rules:
            self.addParsedRule(rule[0],rule[1],rule[2])
            self.addRuleAsStr(rulestr)

        self.facts+=builder.facts
        
        if builder.error:
            print("Error during parsing: exiting ...")
            return True
        return False
        
    def parseProgram(self,filename,write=False):
        error = False
        f=open(filename,"r")
        for line in f:
            line = line.strip()
            if len(line)>0:
                if self.parseRule(line):
                    error =True
                    break
        f.close()
        if error:
            exit(180)
        elif write:
            self.writeToFile(filename+".json")

    def findMaximalStratifiedSubProgram(self):

        dependency = ProgramDependency(self.predicatesToId)
        visitedRules=[False for i in range(len(self.rules))]
        vertices=set()
        verticesName={}
        #TODO why you do not iterate over all rules directly
        for pred in self.predicatesToId:

            pred_id = self.predicatesToId[pred]
            dependency.addNode(pred_id)
            vertices.add(pred_id)
            verticesName[pred_id] = pred

            rules = []
            try:
                rules=self.parsed_rules_for_pred[pred_id]
            except:
                pass
                # print(f"Warning: no rule found for {pred}")
        
            for rule_id in rules:
                if not visitedRules[rule_id]:
                    visitedRules[rule_id] = True
                
                    # [head,choiceBodyForHead,body]
                    rule = self.parsed_rules[rule_id]
                    for headAtom in rule[0]:
                        try:
                            for lit in rule[1][pred]:
                                dependency.addDependency(lit[1],headAtom)
                                id_ = dependency.getId(lit[1])
                                vertices.add(id_)
                                verticesName[id_] = lit[1]
                        except:
                            pass

                        try:
                            for lit in rule[2]:
                                dependency.addDependency(lit[1],headAtom)
                                id_ = dependency.getId(lit[1])
                                vertices.add(id_)
                                verticesName[id_] = lit[1]
                        except:
                            pass
        
        for node in dependency.dependencyGraph:
            dependency.dependencyGraph[node]=list(dependency.dependencyGraph[node])

        strat_predicates = []

        for component in scc.strongly_connected_components_path(vertices,dependency.dependencyGraph):
            strat = True
            preds = list(component)
            for i in preds:
                if not strat:
                    break
        
                head = verticesName[i]
                for j in preds:
                    if not strat:
                        break
        
                    body = [True,verticesName[j]]
                    try:
                        for rule_id in self.parsed_rules_for_pred[i]:
                            
                            rule = self.parsed_rules[rule_id]
                            try:
                                if body in rule[1][head]:
                                    strat=False
                                    break
                            except:
                                pass

                            try:
                                if body in rule[2]:
                                    strat=False
                                    break
                            except:
                                pass
                    except:
                        pass
            if strat:
                strat_predicates+=[verticesName[x] for x in preds]
        
        # predicates in the body of remaining rules
        domainPredicates=set()
        subprogram = []
        remaining = [True for x in self.rules]

        # Adding constraint to subprogram
        for i in range(len(self.parsed_rules)):
            add = True

            # choice rules discared from maximal strat subprogram
            choice = self.parsed_rules[i][1]
            if not choice is None:
                add=False
            
            if add:
                head = self.parsed_rules[i][0]
                if head is None or len(head) == 0:
                    pass
                else:
                    #disjunctive rules discarded from maximal strat subprogram
                    if len(head) >= 2:
                        add=False
                    
                    if add:
                        for headPredicate in head:
                            if headPredicate not in strat_predicates:
                                add=False
                                break
            if add:
                subprogram.append(i)
                remaining[i]=False

            else:
                if not choice is None:
                    for head in choice:
                        for lit in choice[head]:
                            domainPredicates.add(lit[1])
                if not self.parsed_rules[i][2] is None:
                    for lit in self.parsed_rules[i][2]:
                        domainPredicates.add(lit[1])
        #TODO: what if {a;b}. a:-e. b:-f.
        removedFromSubProgram=True
        while removedFromSubProgram:

            removedFromSubProgram=False
            resultingSubProgram=[]
            for i in subprogram:
                head = self.parsed_rules[i][0]
                left = True
                if not head is None and len(head)>0 and head[0] in domainPredicates:
                    removedFromSubProgram=True
                    if not self.parsed_rules[i][2] is None:
                        for lit in self.parsed_rules[i][2]:
                            domainPredicates.add(lit[1])
                else:
                    resultingSubProgram.append(i)
            subprogram = resultingSubProgram
        return subprogram
    
    def checkTight(self):

        dependency = ProgramDependency(self.predicatesToId)
        visitedRules=[False for i in range(len(self.rules))]
        vertices=set()
        verticesName={}
        
        for pred in self.predicatesToId:

            pred_id = self.predicatesToId[pred]
            dependency.addNode(pred_id)
            vertices.add(pred_id)
            verticesName[pred_id] = pred
        
        for rule in self.parsed_rules:
            
            if rule[0] is None or len(rule[0])==0:
                continue

            if not rule[1] is None:
                for head in rule[1]:
                    for lit in rule[1][head]:
                        if not lit[0]:
                            dependency.addDependency(lit[1],head)

            if not rule[2] is None:
                for head in rule[0]:
                    for lit in rule[2]:
                        if not lit[0]:
                            dependency.addDependency(lit[1],head)
            
        
        for node in dependency.dependencyGraph:
            dependency.dependencyGraph[node]=list(dependency.dependencyGraph[node])

        strat_predicates = []
        tight = True

        for component in scc.strongly_connected_components_path(vertices,dependency.dependencyGraph):
            if len(component) > 1:
                tight = False
                break

            pred = list(component)[0]
            predName=verticesName[pred]
            try:
                for rule_id in self.parsed_rules_for_pred[pred]:
                    rule = self.parsed_rules[rule_id]
                    choiceBodyDep = not rule[1] is None and pred in rule[1] and [False,predName] in rule[1][pred] # {a(X,Y):a(X,Z)}:-b(Z,Y).
                    bodyDep = not rule[2] is None and [False,predName] in rule[2] # r(X,Y):-r(X,Z),e(Z,Y).
                    if choiceBodyDep or bodyDep:
                        tight=False
                        break
            except:
                # print("No rule for",predName)
                pass
            
            if not tight:
                break
            
        return tight

# p = ParsedProgram()
# p.parseProgram(sys.argv[1])
# p.writeToFile("parsedProgram.json")
# p.printProgram()
# subprogram = p.findMaximalStratifiedSubProgram()
# print("Maximal Stratified SubProgram")
# print("\n".join([p.rules[i] for i in subprogram]))