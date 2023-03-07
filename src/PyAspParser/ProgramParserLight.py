import sys,json,scc,re

class ProgramDependency:
    
    def __init__(self):
        self.dependencyGraph = {}

    def addNode(self,pred_id):
        if pred_id not in self.dependencyGraph:
            self.dependencyGraph[pred_id]=[]

    def addDependency(self,from_id,to_id):
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
        
        # program rules as string
        self.rules = []
        self.facts = []
        
        # parsed rule struct: [head,choiceBodyForHead,body]
        self.parsed_rules = []
        
        # predicate_id: list[parsed_rules_id]
        self.parsed_rules_for_pred = {}

        self.currentPreds = set()
        self.headPredicates = None
        self.bodyPredicates = None  

    def labelPredicates(self,predicateMap):
        
        self.headPredicates = []
        self.bodyPredicates = []
        
        first = True
        for pred in predicateMap.predicatesToId:
            id_ = predicateMap.predicatesToId[pred]
            if id_ in self.parsed_rules_for_pred:
                self.headPredicates.append(pred)
            elif id_ in self.currentPreds:
                self.bodyPredicates.append(pred)

    def printPredicates(self):
        
        self.labelPredicates()

        headPredicates = ",".join(self.headPredicates)
        bodyPredicates = ",".join(self.bodyPredicates)
        
        print(f"{Program.OUTPUT_DEFINING}:{headPredicates}")
        print(f"{Program.OUTPUT_BODY}:{bodyPredicates}")

    def addParsedRule(self,head,choiceBodyForHead,body):
        ruleIndex = len(self.parsed_rules)
        self.parsed_rules.append([head,choiceBodyForHead,body])
        if not body is None:
            for lit in body:
                self.currentPreds.add(lit[1])
        
        if not choiceBodyForHead is None:
            for h in choiceBodyForHead:
                self.currentPreds.add(h)
                for pred in choiceBodyForHead[h]:
                    self.currentPreds.add(pred)
        if not choiceBodyForHead is None:
            for h in choiceBodyForHead:
                for pred in choiceBodyForHead[h]:
                    self.currentPreds.add(pred[1])
        if not head is None:
            for h in head:
                self.currentPreds.add(h)
        if head is None or len(head)==0:
            return
        
        for head_id in head:
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

    def findMaximalStratifiedSubProgram(self):

        dependency = ProgramDependency()
        visitedRules=[False for i in range(len(self.rules))]
        vertices=set()

        for pred_id in self.currentPreds:
            dependency.addNode(pred_id)
            vertices.add(pred_id)

        #TODO why you do not iterate over all rules directly
        for pred_id in self.currentPreds:
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
                            for lit in rule[1][pred_id]:
                                dependency.addDependency(lit[1],headAtom)
                        except:
                            pass

                        try:
                            for lit in rule[2]:
                                dependency.addDependency(lit[1],headAtom)
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
        
                head = i
                for j in preds:
                    if not strat:
                        break
        
                    body = [True,j]
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
                strat_predicates+=preds
        
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

        dependency = ProgramDependency()
        visitedRules=[False for i in range(len(self.rules))]
        vertices=set()

        for pred_id in self.currentPreds:
            dependency.addNode(pred_id)
            vertices.add(pred_id)
        
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

        tight = True

        for component in scc.strongly_connected_components_path(vertices,dependency.dependencyGraph):
            if len(component) > 1:
                tight = False
                break

            pred = list(component)[0]
            try:
                for rule_id in self.parsed_rules_for_pred[pred]:
                    rule = self.parsed_rules[rule_id]
                    choiceBodyDep = not rule[1] is None and pred in rule[1] and [False,pred] in rule[1][pred] # {a(X,Y):a(X,Z)}:-b(Z,Y).
                    bodyDep = not rule[2] is None and [False,pred] in rule[2] # r(X,Y):-r(X,Z),e(Z,Y).
                    if choiceBodyDep or bodyDep:
                        tight=False
                        break
            except:
                # print("No rule for",predName)
                pass
            
            if not tight:
                break
            
        return tight



