
from antlr4 import *
from PyAspParser.ASPCore2Listener import ASPCore2Listener
from PyAspParser.ASPCore2Parser import ASPCore2Parser
# from ASPCore2Listener import ASPCore2Listener
# from ASPCore2Parser import ASPCore2Parser
import sys


class RuleBuilder(ASPCore2Listener):
    
    def __init__(self):
        
        self.rules = []
        self.facts = []

        # list of predicate name appearing in the head of normal, disjunctive or choice rules
        self.head=None
        
        # list of pair (negated:bool, predicate:string) appearing in the body of normal, disjunctive or choice rules
        self.body=None
        
        # map of (head_predicate:string, list[(negated:bool,body_predicate string)]) for each choice rules
        self.choiceBodyForHead=None
        
        # unused
        self.weights=None
        
        self.naf=None
        self.disjunction = None
        self.ruleTokens = []
        self.error = False

    
    def getRuleAsStr(self):
        return " ".join(self.ruleTokens)
    
    def getHead(self):
        return self.head
    
    def getChoiceBody(self):
        return self.choiceBodyForHead
    
    def getBody(self):
        return self.body

    # Enter a parse tree produced by ASPCore2Parser#program.
    def enterProgram(self, ctx:ASPCore2Parser.ProgramContext):
        self.rules = []
        self.facts = []
        
    # Enter a parse tree produced by ASPCore2Parser#simple_rule.
    def enterSimple_rule(self, ctx:ASPCore2Parser.Simple_ruleContext):
        self.head=None
        self.body=None
        self.weights=None
        self.naf=None
        self.disjunction = None
        self.choiceBodyForHead=None
        
        self.choiceHeadAtom = None
        self.readingAtomIdentifier = False
        self.readingIdentifier = False
        self.buildingChoiceHead = None
        self.ruleTokens = []

    
        
    # Enter a parse tree produced by ASPCore2Parser#head.
    def enterHead(self, ctx:ASPCore2Parser.HeadContext):
        self.head = []

    # Enter a parse tree produced by ASPCore2Parser#body.
    def enterBody(self, ctx:ASPCore2Parser.BodyContext):
        self.body = []

    # Enter a parse tree produced by ASPCore2Parser#disjunction.
    def enterDisjunction(self, ctx:ASPCore2Parser.DisjunctionContext):
        self.disjunction = True

    def exitDisjunction(self, ctx:ASPCore2Parser.DisjunctionContext):
        self.disjunction = False

    # Enter a parse tree produced by ASPCore2Parser#choice_atom.
    def enterChoice_atom(self, ctx:ASPCore2Parser.Choice_atomContext):
        self.choiceBodyForHead = {}

    # Exit a parse tree produced by ASPCore2Parser#choice_element.
    def exitChoice_element(self, ctx:ASPCore2Parser.Choice_elementContext):
        self.buildingChoiceHead = None

    # Enter a parse tree produced by ASPCore2Parser#choice_element_atom.
    def enterChoice_element_atom(self, ctx:ASPCore2Parser.Choice_element_atomContext):
        self.choiceHeadAtom = True

    # Exit a parse tree produced by ASPCore2Parser#choice_element_atom.
    def exitChoice_element_atom(self, ctx:ASPCore2Parser.Choice_element_atomContext):
        self.choiceHeadAtom = False
    
    # Enter a parse tree produced by ASPCore2Parser#identifier.
    def enterIdentifier(self, ctx:ASPCore2Parser.IdentifierContext):
        self.readingIdentifier = True
    
    def exitIdentifier(self, ctx:ASPCore2Parser.IdentifierContext):
        self.readingIdentifier = False

    # Enter a parse tree produced by ASPCore2Parser#atom.
    def enterAtom(self, ctx:ASPCore2Parser.AtomContext):
        self.readingAtomIdentifier = True
    
   # Enter a parse tree produced by ASPCore2Parser#naf_literal.
    def enterNaf_literal(self, ctx:ASPCore2Parser.Naf_literalContext):
        self.naf = False

    # Exit a parse tree produced by ASPCore2Parser#naf_literal.
    def exitNaf_literal(self, ctx:ASPCore2Parser.Naf_literalContext):
        self.naf = None

    # Exit a parse tree produced by ASPCore2Parser#simple_rule.
    def exitSimple_rule(self, ctx:ASPCore2Parser.Simple_ruleContext):
        # if self.choiceBodyForHead is None:
        #     print("Head:",self.head,"Body:",self.body)
        # else:
        #     for head in self.head:
        #         print("Head:",head,end=" Body: ")
        #         try:
        #             for lit in self.choiceBodyForHead[head]:
        #                 print(lit,end=" ")
        #             for lit in self.body:
        #                 print(lit,end=" ")
        #         except:
        #             pass
        #         finally:
        #             print()
        if self.choiceBodyForHead is None and (self.body is None or len(self.body)==0):
            self.facts.append(self.getRuleAsStr())
        else:
            self.rules.append(([self.head,self.choiceBodyForHead,self.body],self.getRuleAsStr()))
        
    def visitErrorNode(self, node:ErrorNode):
        self.error = True

    def visitTerminal(self, node:TerminalNode):
        self.ruleTokens.append(str(node))
        if node.symbol.type == ASPCore2Parser.NAF:
            self.naf = True
        
        if self.readingAtomIdentifier and self.readingIdentifier:
            predicate = [self.naf, str(node)]
            self.readingAtomIdentifier = False

            if not self.naf is None:
                self.naf = False 

            if not self.choiceHeadAtom is None and self.choiceHeadAtom:
                self.buildingChoiceHead = predicate[1]
                self.head.append(predicate[1])

            elif (not self.disjunction is None and self.disjunction):
                self.head.append(predicate[1])

            else:
                if not self.buildingChoiceHead is None:
                    try:
                        self.choiceBodyForHead[self.buildingChoiceHead].append(predicate)
                    except:
                        self.choiceBodyForHead[self.buildingChoiceHead]=[predicate]
                else: 
                    self.body.append(predicate)   


