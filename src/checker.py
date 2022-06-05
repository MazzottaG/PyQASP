import re,argparse,sys

dimacsCommentMatch = r'^c\s*(\d+)\s*(.*)\n'
dimacsHeaderMatch = r'^p\scnf\s*\d+\s*\d*\n'

class CheckerCNF:

    def __init__(self):
        self.substitution = {}

    # def addClause(self,c1,c2):
    #     literals = [[int(v) for v in c1.split(" ")],[int(v) for v in c2.split(" ")]]
    #     for i in range(len(literals[0])):
    #         if literals[0][i] == 0:
    #             continue
    #         v1 = literals[0][i] if literals[0][i] >= 0 else -literals[0][i]
    #         v2 = literals[1][i] if literals[1][i] >= 0 else -literals[1][i]
            
    #         if v1 not in self.substitution:
    #             self.substitution[v1]=v2
            
    #         subv2= self.substitution[v1] if literals[0][i] >=0 else -self.substitution[v1]

    #         if subv2 != literals[1][i]:
    #             print(f"Error in clause remapping: expected={subv2} real={literals[1][i]}")
    #             print(f"Error in clause remapping: actual subtitution={self.subtitution}")
    #             exit(180)
    #     return
    # def checkAndClear(self):
    #     for v in self.substitution:
    #         for v1 in self.substitution:
    #             if v != v1 and self.substitution[v1]==self.substitution[v]:
    #                 print(f"Error in clause remapping: {v} and {v1} mapped to same variable = {self.substitution[v]}")
    #                 print(f"Error in clause remapping: actual subtitution={self.subtitution}")
    #                 exit(180)
    #     self.clear()
        
    # def getMismatch(self):
    #     return self.cnf1[self.clauseMismatch]+" "+self.cnf2[self.clauseMismatch]
    # def clear(self):
    #     self.substitution = {}
    
