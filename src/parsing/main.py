from antlr4 import *
from ASPCore2Lexer import ASPCore2Lexer
from ASPCore2Parser import ASPCore2Parser
from ASPCore2Listener import ASPCore2Listener
import sys

file = sys.argv[1]
HEAD_PREDICATE_PREFIX = 0
NO_HEAD_PREDICATE_PREFIX = 1
TIGHT_PROP_PREFIX = 2
listener = ASPCore2Listener(ASPCore2Parser.NAF)
f=open(file,"r")
for line in f:
    line = line.strip()
    if len(line)>0:
        input_stream = InputStream(line)
        lexer = ASPCore2Lexer(input_stream)
        stream = CommonTokenStream(lexer)
        parser = ASPCore2Parser(stream)
        parser.addParseListener(listener)
        parser.program()
f.close()
inverse={}
for pred,id_ in listener.predicatesToId.items():
    inverse[id_]=pred
    if pred in listener.headPredicates:
        print(f"{HEAD_PREDICATE_PREFIX}:{pred}")
    else:
        print(f"{NO_HEAD_PREDICATE_PREFIX}:{pred}")
    

for pred in listener.graph:
    for adj in listener.graph[pred]:
        print(f"{inverse[pred]}->{inverse[adj]}")

