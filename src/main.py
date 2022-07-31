from grounder import *
from Option import FILE_UTIL,QASP_FORMAT
from Builder import QCIRBuilder
from Structures import SymbolTable
from AspParser import QASPParser
from Solver import *
import argparse

SOLVERS = {
    "quabs":Quabs(),
    "depqbf":Depqbf(),
    "rareqs":Rareqs()
}
GROUNDERS = {
    "gringo":GringoWapper(),
    "idlv":IDLVWapper(),
    "dlv2":DLV2Wapper()
}
GROUNDERS_DESC = {
    "gringo":"Plain version of gringo grounder",
    "idlv":"Plain version of idlv grounder",
    "dlv2":"idlv + well founded model"
}
argparser = argparse.ArgumentParser(description='QBF encoder')
argparser.add_argument('filename', metavar='file', type=str, help='Path to QASP file')
# argparser.add_argument('-v','--verbose', dest="debuglevel",  type=int, help='verbosity levels: 1 or 2')
argparser.add_argument('-err','--errorfile', dest="logfile",  type=str, help='Path of file used for collecting stderr')
argparser.add_argument('-g','--grounder', dest="groundername",  type=str, help='available grounders : '+str(GROUNDERS_DESC))
argparser.add_argument('-s','--solver', dest="solvername",  type=str, help='available solvers : '+str(list(SOLVERS.keys())))
argparser.add_argument('-pq','--print-qcir', dest="qcir_file",  type=str, help='output qcir filename')

ns = argparser.parse_args()
if ns.qcir_file:
    FILE_UTIL.QBF_PROGRAM_FILE=ns.qcir_file

if ns.logfile:
    FILE_UTIL.LOG_ERROR = ns.logfile

grounder = GROUNDERS["dlv2"]
if ns.groundername:
    if ns.groundername not in GROUNDERS:
        print("Error: Unable to find grounder")
        sys.exit(180)
    grounder = GROUNDERS[ns.groundername]

grounder.printName()

ExternalCalls.LOG_FILE_HANDLER = open(FILE_UTIL.LOG_ERROR,"w")
symbols = SymbolTable()
parser = QASPParser(ns.filename,symbols,grounder)
parser.parse()

solver = SOLVERS["quabs"]
if ns.solvername:
    if ns.solvername not in SOLVERS:
        print("Error: Unable to find solver")
        sys.exit(180)
    solver = SOLVERS[ns.solvername]

solver.solve(symbols,parser.isFirstForall())
ExternalCalls.LOG_FILE_HANDLER.close()