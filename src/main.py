from grounder import *
from Option import FILE_UTIL,QASP_FORMAT,DEFAULT_PROPERTIES
from Builder import QCIRBuilder
from Structures import SymbolTable
from AspParser import QASPParser
from Solver import *
import argparse,signal,subprocess

ExternalCalls.LOG_FILE_HANDLER = None
def handler(signum, frame):
    print('Sig term', signum)
    if not ExternalCalls.LOG_FILE_HANDLER is None:
        ExternalCalls.LOG_FILE_HANDLER.close() 
    sys.exit(180)
    
signal.signal(signal.SIGTERM, handler)
#signal.signal(signal.SIGKILL, handler)

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
argparser.add_argument('--no-choice-opt', dest="disable_choice_check", default=False, action='store_true')
argparser.add_argument('--no-direct-cnf', dest="disable_skip_conv", default=False, action='store_true')
argparser.add_argument('--stats', dest="stats", default=False, action='store_true',help="print encoded qbf formula's statistics")
argparser.add_argument('-e', "--encoder-only", dest="encode", default=False, action='store_true',help="disable solver usage and print qcir on stdout")

ns = argparser.parse_args()

if ns.disable_choice_check:
    DEFAULT_PROPERTIES.ONLY_CHOICE = False

if ns.disable_skip_conv:
    DEFAULT_PROPERTIES.SKIP_QCIR_CONV_FOR_QDIMACS = False

if ns.stats:
    DEFAULT_PROPERTIES.PRINT_STATS = True

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
# print(symbols)
if ns.encode:
    print(subprocess.getoutput(f"cat {FILE_UTIL.QBF_PROGRAM_FILE}"))
    sys.exit(0)
solver = SOLVERS["quabs"]
if ns.solvername:
    if ns.solvername not in SOLVERS:
        print("Error: Unable to find solver")
        sys.exit(180)
    solver = SOLVERS[ns.solvername]
if DEFAULT_PROPERTIES.PRINT_STATS:
    parser.getQcirProps().printProps()
solver.solve(symbols,parser.isFirstForall(),parser.getQcirProps())
ExternalCalls.LOG_FILE_HANDLER.close()
