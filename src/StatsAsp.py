from Option import LPARSE_FORMAT
import subprocess,math

class EmptyStats:
    def analyzeRule(self,rule):
        pass
    def getFeature(self):
        pass
    def getPredicationFeature(self):
        pass
    def setAtomSize(self,atoms):
        pass
    def print(self):
        pass
    def setExistentials(stats,v):
        pass
    def setUniversals(stats,v):
        pass
    def setQUniversal(stats,v):
        pass
    def setQExistential(stats,v):
        pass
    def setQLevels(stats,v):
        pass

class StatsAsp(EmptyStats):
    
    HELP={
        "R"         :"Rule count                                        :",
        "A"         :"Number of atoms                                   :",
        "(R/A)"     :"Ratio between rules count and atoms count         :",
        "(R/A)^2"   :"Squared ratio between rules count and atoms count :",
        "(R/A)^3"   :"Cube ratio between rules count and atoms count    :",
        "(A/R)"     :"Ratio between atoms count and rules count         :",
        "(A/R)^2"   :"Squared ratio between atoms count and rules count :",
        "(A/R)^3"   :"Cube ratio between atoms count and rules count    :",
        "R1"        :"Rule with body of length 1                        :",
        "R2"        :"Rule with body of length 2                        :",
        "R3"        :"Rule with body of length 3                        :",
        "PR"        :"Positive rule count                               :",
        "F"         :"Normal facts count                                :",
        "DF"        :"Disjunctive facts count                           :",
        "NR"        :"Normal rule count                                 :",
        "NC"        :"Constraint count                                  :",
        "VF"        :"Universal atoms count                             :",
        "VE"        :"Existantial atoms count                           :",
        "QF"        :"Universial levels count                           :",
        "QE"        :"Existantial levels count                          :",
        "QL"        :"Quantification levels count                       :"
    }
    HEADER=[
            "R"      ,
            "A"      ,
            "(R/A)"  ,
            "(R/A)^2",
            "(R/A)^3",
            "(A/R)"  ,
            "(A/R)^2",
            "(A/R)^3",
            "R1"     ,
            "R2"     ,
            "R3"     ,
            "PR"     ,
            "F"      ,
            "DF"     ,
            "NR"     ,
            "NC"     ,
            "VF"     ,
            "VE"     ,
            "QF"     ,
            "QE"     ,
            "QL"     
        ] 
    def getHelp():
        metrics=[]
        for metric in StatsAsp.HELP:
            metrics.append(StatsAsp.HELP[metric]+" "+metric)
        return "\n".join(metrics)

    def getHeader():
        return ",".join(StatsAsp.HEADER)

    def getR(stats):
        return stats.ruleCount

    def getA(stats):
        return stats.atomsCount
    
    def getRatioRA(stats):
        try:
            return stats.ruleCount/stats.atomsCount
        except:
            return float('NaN')
    
    def getRatioRA2(stats):
        try:
            return math.pow(stats.ruleCount/stats.atomsCount,2)
        except:
            return float('NaN')
    
    def getRatioRA3(stats):
        try:
            return math.pow(stats.ruleCount/stats.atomsCount,3)
        except:
            return float('NaN')
    def getRatioAR(stats):
        try:
            return stats.atomsCount/stats.ruleCount
        except:
            return float('NaN')
    def getRatioAR2(stats):
        try:
            return math.pow(stats.atomsCount/stats.ruleCount,2)
        except:
            return float('NaN')
    def getRatioAR3(stats):
        try:
            return math.pow(stats.atomsCount/stats.ruleCount,3)
        except:
            return float('NaN')
    def getL1(stats):
        return stats.ruleForBodyLen[1]
    def getL2(stats):
        return stats.ruleForBodyLen[2]
    def getL3(stats):
        return stats.ruleForBodyLen[3]
    def getPR(stats):
        return stats.positiveRuleCount
    def getF(stats):
        return stats.trueFacts
    def getDF(stats):
        return stats.disjunciveFacts
    def getNR(stats):
        return stats.normalRuleCount
    def getC(stats):
        return stats.constraintCount
    def getVE(stats):
        return stats.existentials
    def getVF(stats):
        return stats.universals
    def getQF(stats):
        return stats.universalLevels
    def getQE(stats):
        return stats.existentialLevels
    def getQL(stats):
        return stats.levels

    FUNC={
        "R"         : getR,
        "A"         : getA,
        "(R/A)"     : getRatioRA,
        "(R/A)^2"   : getRatioRA2,
        "(R/A)^3"   : getRatioRA3,
        "(A/R)"     : getRatioAR,
        "(A/R)^2"   : getRatioAR2,
        "(A/R)^3"   : getRatioAR3,
        "R1"        : getL1,
        "R2"        : getL2,
        "R3"        : getL3,
        "PR"        : getPR,
        "F"         : getF,
        "DF"        : getDF,
        "NR"        : getNR,
        "NC"        : getC,
        "VF"        : getVF,
        "VE"        : getVE,
        "QF"        : getQF,
        "QE"        : getQE,
        "QL"        : getQL
    }
    def __init__(self):
        
        # problem size feature
        self.ruleCount              = 0
        self.atomsCount             = 1
        
        # computed metrics
        # ra^i: ratio rules/atoms
        # ar^i: ratio atoms/rules
        
        # balance feature
        self.minBodyLen             = 1
        self.maxBodyLen             = 3 
        self.ruleForBodyLen         = [0]
        
        for i in range(self.minBodyLen,self.maxBodyLen+1):
            self.ruleForBodyLen.append(0)

        # proximity to horn
        self.positiveRuleCount      = 0

        # pecurial feature
        self.trueFacts              = 0
        self.disjunciveFacts        = 0
        self.normalRuleCount        = 0
        self.constraintCount        = 0

        # quantifier stats
        self.existentials           = 0
        self.universals             = 0
        self.existentialLevels      = 0
        self.universalLevels        = 0
        self.levels                 = 0

    def getFeature(self):
        row=[]
        for feature in StatsAsp.HEADER:
            f = StatsAsp.FUNC[feature]
            row.append(str(f(self)))
        return ",".join(row)

    def getPredicationFeature(self):
        row=[]
        for feature in StatsAsp.HEADER:
            f = StatsAsp.FUNC[feature]
            row.append(f(self))
        return row

    def print(self):
        for feature in StatsAsp.HEADER:
            print(StatsAsp.HELP[feature],StatsAsp.FUNC[feature](self))

    def setExistentials(self,v):
        self.existentials=v

    def setUniversals(self,v):
        self.universals=v

    def setQUniversal(self,v):
        self.universalLevels=v

    def setQExistential(self,v):
        self.existentialLevels=v

    def setQLevels(self,v):
        self.levels=v

    def analyzeBody(self,rule,bodyLen,negativebodyLen):
        
        if negativebodyLen == 0:
            self.positiveRuleCount+=1

        if bodyLen >= self.minBodyLen and bodyLen <= self.maxBodyLen:
            try:
                self.ruleForBodyLen[bodyLen]+=1
            except:
                self.ruleForBodyLen[bodyLen]=1
    
    def isConstraint(self,rule):
        return rule[LPARSE_FORMAT.RULE_TYPE_INDEX+1] == 1

    def setAtomSize(self,atoms):
        self.atomsCount=atoms
        
    def analyzeRule(self,rule):        
        
        ruleType = rule[LPARSE_FORMAT.RULE_TYPE_INDEX]
        self.ruleCount+=1
            
        if ruleType == LPARSE_FORMAT.CHOICE_RULE:
            head_len = rule[LPARSE_FORMAT.HEAD_LENGHT_INDEX]
            bodyLen = rule[LPARSE_FORMAT.HEAD_LENGHT_INDEX+head_len+1]
            negativebodyLen = rule[LPARSE_FORMAT.HEAD_LENGHT_INDEX+head_len+2]
            
            self.analyzeBody(rule,bodyLen,negativebodyLen)
                
        elif ruleType == LPARSE_FORMAT.DISJCUNTIVE_RULE:
            head_len = rule[LPARSE_FORMAT.HEAD_LENGHT_INDEX]
            bodyLen = rule[LPARSE_FORMAT.HEAD_LENGHT_INDEX+head_len+1]
            negativebodyLen = rule[LPARSE_FORMAT.HEAD_LENGHT_INDEX+head_len+2]
            if bodyLen == 0:
                self.disjunciveFacts+=1

            self.analyzeBody(rule,bodyLen,negativebodyLen)

        else:
            # normal rule, constraint rule or weight rule 

            if self.isConstraint(rule):
                self.constraintCount+=1

            bodyLen=0
            negativebodyLen=0

            if ruleType == LPARSE_FORMAT.SIMPLE_RULE:
                
                self.normalRuleCount+=1
                bodyLen = rule[LPARSE_FORMAT.RULE_TYPE_INDEX+2]
                negativebodyLen = rule[LPARSE_FORMAT.RULE_TYPE_INDEX+3]
                if bodyLen == 0:
                    self.trueFacts+=1

            elif ruleType == LPARSE_FORMAT.CONSTRAINT_RULE:
                
                bodyLen = rule[LPARSE_FORMAT.RULE_TYPE_INDEX+2]
                negativebodyLen = rule[LPARSE_FORMAT.RULE_TYPE_INDEX+3]
            
            else:
                # weigth rule
                bodyLen = rule[LPARSE_FORMAT.RULE_TYPE_INDEX+3]
                negativebodyLen = rule[LPARSE_FORMAT.RULE_TYPE_INDEX+4]
            
            self.analyzeBody(rule,bodyLen,negativebodyLen)
        
        
# programs=[
#     "a(1).a(2).b(1).b(2).{c(X,Y):b(Y)}:-a(X).",
#     "{a(1);a(2);b(1);b(2)}.{c(X,Y):b(Y)}:-a(X).",
#     "a(1).a(2).b(1).b(2).{c(X,Y):b(Y)}:-a(X). :-c(X,X).",
#     "{a(1);b(1);b(2)}.{c(X,Y):b(Y);d(X,Y):b(Y)}:-a(X). :-c(X,X).",
#     "{color(X,red);color(X,blue);color(X,green)}=1:-node(X). :-link(X,Y),color(X,C),color(Y,C),X!=Y. node(1). link(1,1). link(1,2). link(1,3). node(2). link(2,1). link(2,2). link(2,3). node(3). link(3,1). link(3,2). link(3,3).",
#     "color(X,red)|color(X,blue)|color(X,green):-node(X). :-link(X,Y),color(X,C),color(Y,C),X!=Y. node(1). link(1,1). link(1,2). link(1,3). node(2). link(2,1). link(2,2). link(2,3). node(3). link(3,1). link(3,2). link(3,3)."
# ]
# for program in programs:
#     childProcess = subprocess.Popen(["resources/tools/dlv2","--mode=idlv"],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
#     stdout,stderr = childProcess.communicate(program.encode("utf-8"))
#     print(program)
#     stats = StatsAsp()
#     for line in stdout.decode("utf-8").split("\n"):
#         line = line.strip()
#         if line.startswith("0"):
#             break
#         stats.analyzeRule([int(x) for x in line.split(" ")])
#     stats.print()        
# print(StatsAsp.getHeader())
# print(StatsAsp.getHelp())