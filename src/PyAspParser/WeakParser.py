
from antlr4 import *
from PyAspParser.ASPCore2Listener import ASPCore2Listener
from PyAspParser.ASPCore2Parser import ASPCore2Parser
from PyAspParser.ASPCore2Lexer import ASPCore2Lexer
# from ASPCore2Listener import ASPCore2Listener
# from ASPCore2Parser import ASPCore2Parser
# from ASPCore2Lexer import ASPCore2Lexer
# from ASPCore2Listener import ASPCore2Listener
# from ASPCore2Parser import ASPCore2Parser
import sys


class RuleBuilder(ASPCore2Listener):
    
    def __init__(self):
        self.WEAK_PRED="weak_viol"
        self.body=None
        self.weights=None
        self.readingWeights=False
        self.weaktokens = []
        self.ignoring_tokens=[ASPCore2Parser.SQUARE_OPEN,ASPCore2Parser.SQUARE_CLOSED,ASPCore2Parser.WCONS]
        self.rules=[]
        self.last_index=[]
        self.foundLevel = False

    def getRuleAsStr(self):
        return " ".join(self.weaktokens)
    
    def getHead(self):
        pass 

    def getChoiceBody(self):
        pass
    
    def getBody(self):
        return self.body

    # Enter a parse tree produced by ASPCore2Parser#program.
    def enterProgram(self, ctx:ASPCore2Parser.ProgramContext):
        pass        
    # Enter a parse tree produced by ASPCore2Parser#simple_rule.
    def enterSimple_rule(self, ctx:ASPCore2Parser.Simple_ruleContext):
        self.body=None
        self.weights=None
        self.weaktokens = []

    # Enter a parse tree produced by ASPCore2Parser#head.
    def enterHead(self, ctx:ASPCore2Parser.HeadContext):
        pass

    # Enter a parse tree produced by ASPCore2Parser#body.
    def enterBody(self, ctx:ASPCore2Parser.BodyContext):
        self.body = []

    # Enter a parse tree produced by ASPCore2Parser#disjunction.
    def enterDisjunction(self, ctx:ASPCore2Parser.DisjunctionContext):
        pass

    def exitDisjunction(self, ctx:ASPCore2Parser.DisjunctionContext):
        pass

    # Enter a parse tree produced by ASPCore2Parser#choice_atom.
    def enterChoice_atom(self, ctx:ASPCore2Parser.Choice_atomContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#choice_element.
    def exitChoice_element(self, ctx:ASPCore2Parser.Choice_elementContext):
        pass

    # Enter a parse tree produced by ASPCore2Parser#choice_element_atom.
    def enterChoice_element_atom(self, ctx:ASPCore2Parser.Choice_element_atomContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#choice_element_atom.
    def exitChoice_element_atom(self, ctx:ASPCore2Parser.Choice_element_atomContext):
        pass
    
    # Enter a parse tree produced by ASPCore2Parser#identifier.
    def enterIdentifier(self, ctx:ASPCore2Parser.IdentifierContext):
        pass
    
    def exitIdentifier(self, ctx:ASPCore2Parser.IdentifierContext):
        pass

    # Enter a parse tree produced by ASPCore2Parser#atom.
    def enterAtom(self, ctx:ASPCore2Parser.AtomContext):
        pass
    
   # Enter a parse tree produced by ASPCore2Parser#naf_literal.
    def enterNaf_literal(self, ctx:ASPCore2Parser.Naf_literalContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#naf_literal.
    def exitNaf_literal(self, ctx:ASPCore2Parser.Naf_literalContext):
        pass

    # Enter a parse tree produced by ASPCore2Parser#weight_at_levels.
    def enterWeight_at_levels(self, ctx:ASPCore2Parser.Weight_at_levelsContext):
        self.weights=[]

    # Exit a parse tree produced by ASPCore2Parser#weight_at_levels.
    def exitWeight_at_levels(self, ctx:ASPCore2Parser.Weight_at_levelsContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#levels_and_terms.
    def enterLevels_and_terms(self, ctx:ASPCore2Parser.Levels_and_termsContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#levels_and_terms.
    def exitLevels_and_terms(self, ctx:ASPCore2Parser.Levels_and_termsContext):
        pass
    # Exit a parse tree produced by ASPCore2Parser#simple_rule.
    def exitSimple_rule(self, ctx:ASPCore2Parser.Simple_ruleContext):
        if not self.weights is None:
            # print("body:"," ".join(self.weaktokens))
            # print("weights:",self.weights)
            
            if not self.foundLevel:
                self.weights = [self.weights[0],"1"]+self.weights[1:]
            terms = ",".join(self.weights)
            body = " ".join(self.weaktokens)
            self.rules.append(f"{self.WEAK_PRED}("+terms+"):-"+body)
            self.weights=None
        else:
            print("Ignored rule:"," ".join(self.weaktokens))
            
    def enterTerm_(self, ctx:ASPCore2Parser.Term_Context):
        if not self.weights is None:
            self.last_index.append(len(self.weights))
        else:
            self.last_index.append(len(self.weaktokens))
        # print("Reading Term_",self.weaktokens,self.weights,self.last_index)

    def enterExpr(self, ctx:ASPCore2Parser.Term_Context):
        if not self.weights is None:
            self.last_index.append(len(self.weights))
        else:
            self.last_index.append(len(self.weaktokens))
        # print("Reading Expr",self.weaktokens,self.weights,self.last_index)
        

    def shrink(self,size,array):
        term = []
        while len(array)>size:
            term.append(array.pop())
        return "".join(reversed(term))

    def exitTerm_(self, ctx:ASPCore2Parser.Term_Context):
        if len(self.last_index)<=0:
            sys.exit(180)

        length = self.last_index.pop()
        if not self.weights is None:
            term = self.shrink(length,self.weights)
            self.weights.append(term)
        else:
            term = self.shrink(length,self.weaktokens)
            self.weaktokens.append(term)
        # print("Read Term_",self.weaktokens,self.weights,self.last_index)
        
    def exitExpr(self, ctx:ASPCore2Parser.Term_Context):
        if len(self.last_index)<=0:
            sys.exit(180)

        length = self.last_index.pop()
        if not self.weights is None:
            term = self.shrink(length,self.weights)
            self.weights.append(term)
        else:
            term = self.shrink(length,self.weaktokens)
            self.weaktokens.append(term)
        # print("Read Expr",self.weaktokens,self.weights,self.last_index)



    def visitErrorNode(self, node:ErrorNode):
        self.error = True

    def visitTerminal(self, node:TerminalNode):
        if node.symbol.type in self.ignoring_tokens:
            return
        if node.symbol.type == ASPCore2Parser.AT:
            self.foundLevel = True
            return

        if self.weights is None:
            self.weaktokens.append(str(node))
        elif node.symbol.type != ASPCore2Parser.COMMA:
            self.weights.append(str(node))

def translate_weak(filename):
    data=None
    with open(filename) as file:
        data = f'{file.read()}\n'
    input_stream = InputStream(data)
    lexer = ASPCore2Lexer(input_stream)
    stream = CommonTokenStream(lexer)
    parser = ASPCore2Parser(stream)
    listener = RuleBuilder() 
    parser.addParseListener(listener)
    parser.program()
    return listener.rules,listener.WEAK_PRED

# print(translate_weak("../../weak.lp"))