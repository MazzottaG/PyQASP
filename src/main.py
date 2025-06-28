from grounder import *
from Option import FILE_UTIL,QASP_FORMAT,DEFAULT_PROPERTIES,Debugger,DebugCommand
#from Builder import QCIRBuilder
#from Structures import SymbolTable
#from AspParser import QASPParser
from Solver import *
from SubProgramParser import *
import argparse,signal,subprocess,json,sys

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
    "rareqs":Rareqs(),
    "auto":None
}
GROUNDERS = {
    #"gringo":GringoWapper(),
    #"idlv":IDLVWapper(),
    # "dlv2":DLV2Wapper()
}
GROUNDERS_DESC = {
    "gringo":"Plain version of gringo grounder",
    "idlv":"Plain version of idlv grounder",
    "dlv2":"idlv + well founded model"
}
argparser = argparse.ArgumentParser(description='QBF encoder')
argparser.add_argument('filename', metavar='file', type=str, help='Path to QASP file')
argparser.add_argument('--no-wf', dest="disable_wf", default=False, action='store_true')
argparser.add_argument('--sat', dest="only_sat", default=False, action='store_true')
argparser.add_argument('--enumerate', dest="enum", default=False, action='store_true')
argparser.add_argument('-s','--solver', dest="solvername",  type=str, help='available solvers : '+str(list(SOLVERS.keys())))
argparser.add_argument('-g','--grounder', dest="groundername",  type=str, help='available grounders : '+str(list(GROUNDERS_DESC.keys())))
argparser.add_argument('-pq','--print-qcir', dest="qcir_file",  type=str, help='output qcir filename')
argparser.add_argument('-err','--error-log', dest="log_file",  type=str, help='external tools log filename')
argparser.add_argument('--guess-check', dest="enable_guess_check", default=False, action='store_true')
# argparser.add_argument('--no-direct-cnf', dest="disable_skip_conv", default=False, action='store_true')
argparser.add_argument('--stats', dest="stats", default=False, action='store_true',help="print encoded qbf formula's statistics")
argparser.add_argument('--asp-stats', dest="aspstats", default=False, action='store_true',help="print smodels' statistics")
argparser.add_argument('-e', "--encoder-only", dest="encode", default=False, action='store_true',help="disable solver usage and print qcir on stdout")
argparser.add_argument('-d', "--debug-encoding", dest="debug_print", default=False, action='store_true',help="enable encoding process debug")

ns = argparser.parse_args()

grounder = "dlv2"
if ns.groundername:
    if ns.groundername not in GROUNDERS_DESC:
        print("Error: Unable to find solver")
        sys.exit(180)
    grounder = ns.groundername

if ns.enable_guess_check:
    DEFAULT_PROPERTIES.GUESS_CHECK = True

if ns.disable_wf:
    DEFAULT_PROPERTIES.NO_WF = True
else:
    if grounder != "dlv2":
        print("Error: disable well optimization are not supported with plain gringo and idlv grounder")
        print("run with --no-wf")
        sys.exit(180)

if ns.only_sat:
    DEFAULT_PROPERTIES.SATISFIABILITY = True

if DEFAULT_PROPERTIES.GUESS_CHECK and DEFAULT_PROPERTIES.NO_WF:
    print("Guess&Check optimizations are meant to be used together with well founded opt")
    print("run without --no-wf")
    sys.exit(180)

if ns.aspstats or (not ns.solvername or SOLVERS[ns.solvername] is None):
    DEFAULT_PROPERTIES.PRINT_ASPSTATS = True

if ns.stats:
    DEFAULT_PROPERTIES.PRINT_STATS = True

if ns.qcir_file:
    FILE_UTIL.QBF_PROGRAM_FILE=ns.qcir_file

if ns.log_file:
    FILE_UTIL.LOG_ERROR = ns.log_file

ExternalCalls.LOG_FILE_HANDLER = open(FILE_UTIL.LOG_ERROR,"w")


if ns.debug_print:
    DEFAULT_PROPERTIES.debug=Debugger()
    DEFAULT_PROPERTIES.debugcmd=DebugCommand()

ExternalCalls.debugger = DEFAULT_PROPERTIES.debug
ExternalCalls.debuggercmd = DEFAULT_PROPERTIES.debugcmd

parser = SubProgramParser(ns.filename,grounder)
props,aspstats = parser.buildSubPrograms()
if DEFAULT_PROPERTIES.PRINT_STATS:
    props.printProps()

if ns.aspstats:
    print("@ASPSTATS",aspstats.getFeature())
    aspstats.print()

solver = SOLVERS["quabs"]
if ns.solvername:
    if ns.solvername not in SOLVERS:
        print("Error: Unable to find solver")
        sys.exit(180)
    solver = SOLVERS[ns.solvername]
if solver is None:
    import joblib
    estimator   = joblib.load(FILE_UTIL.ESTIMATOR_FILE)
    backend = estimator.predict([aspstats.getPredicationFeature()])
    solver_name = backend[0].split(".bash")[0]
    if solver_name.endswith("-blo"):
        solver_name = solver_name.split("-blo")[0]
    solver = SOLVERS[solver_name]
    DEFAULT_PROPERTIES.debug.printMessage(solver_name+" selected as backend solver")
else:
    DEFAULT_PROPERTIES.debug.printMessage(ns.solvername+" used as backend solver")
    # backend = loaded_model.predict([row])[0]
symbols=parser.symbols
# json_object = json.dumps(symbols.factory, indent=4)
 
# with open("symbols.json", "w") as outfile:
#     outfile.write(json_object)

if ns.encode:
    sys.exit(0)
isFirstForall=parser.encodedLevel[1] in [parser.ENCODED_F,parser.SKIPPED]
parser=None
if not ns.enum or ns.only_sat:
    solver.solve(symbols,isFirstForall,props)
else:
    solver = QuabsEnumerator()
    solver.solve(symbols,isFirstForall,props)

ExternalCalls.LOG_FILE_HANDLER.close()
