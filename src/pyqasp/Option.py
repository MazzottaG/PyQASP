import sys,os,subprocess
from importlib.resources import files, as_file

class PyQASPOptions:
    DLV2 = 0
    DLV2_WELL_FOUNDED = 1
    GRINGO = 2

class LPARSE_FORMAT:
    SIMPLE_RULE=1
    CONSTRAINT_RULE=2
    CHOICE_RULE=3
    WEIGHT_RULE=5
    DISJCUNTIVE_RULE=8

    RULE_TYPE_INDEX=0
    HEAD_LENGHT_INDEX=1
    ONE_HEAD_ATOM_INDEX=1
    BODY_SIZE_INDEX=2
    NEG_BODY_SIZE_INDEX=3

    SEPARATOR = " "

class FILE_UTIL:
    # DEFAULT_PATH                = sys._MEIPASS
    DEFAULT_PATH                = files("pyqasp")
    ASP_PARSER_PATH             = DEFAULT_PATH.joinpath("resources","asp-parser","aspToJson") 
    # ASP_PARSER_PATH             = DEFAULT_PATH.joinpath("resources","asp-parser","ruleToJson") 
    ASP_RULE_PARSER_PATH        = DEFAULT_PATH.joinpath("resources","asp-parser","ParserByRule") 
    ASP_PROGRAM_PARSER_PATH     = DEFAULT_PATH.joinpath("resources","asp-parser","ProgramParser") 
    OLD_TOOL_FOLDER_PATH        = DEFAULT_PATH.joinpath("resources","old-tool")
    TOOL_FOLDER_PATH            = DEFAULT_PATH.joinpath("resources","tools")
    MODELS_FOLDER               = DEFAULT_PATH.joinpath("resources","selector_model")

    ESTIMATOR_FILE              = MODELS_FOLDER.joinpath("uniquely-augmented_random-forest.joblib")
    
    QUABS_PATH                  = TOOL_FOLDER_PATH.joinpath("quabs")
    DEPQBF_PATH                 = TOOL_FOLDER_PATH.joinpath("depqbf")
    RAREQS_NN_PATH              = TOOL_FOLDER_PATH.joinpath("rareqs-nn")
    
    QCIR_CONV_PATH              = TOOL_FOLDER_PATH.joinpath("qcir-conv.py")
    BLOQQER37_PATH              = TOOL_FOLDER_PATH.joinpath("bloqqer37")
    FMLA_PATH                   = TOOL_FOLDER_PATH.joinpath("fmla")
    
    GRINGO_PATH                 = TOOL_FOLDER_PATH.joinpath("gringo")        
    DLV2_PATH                   = TOOL_FOLDER_PATH.joinpath("dlv2")          
    
    OLD_LPSHIFT_PATH            = TOOL_FOLDER_PATH.joinpath("lpshift-1.4")   
    OLD_LP2NORMAL_PATH          = TOOL_FOLDER_PATH.joinpath("lp2normal-2.27")
    OLD_LP2SAT_PATH             = TOOL_FOLDER_PATH.joinpath("lp2sat-1.24")   

    LPSHIFT_PATH                = TOOL_FOLDER_PATH.joinpath("lpshift")   
    LP2NORMAL_PATH              = TOOL_FOLDER_PATH.joinpath("lp2normal2")
    LP2SAT_PATH                 = TOOL_FOLDER_PATH.joinpath("lp2sat")   
    LP2ACYC_PATH                = TOOL_FOLDER_PATH.joinpath("lp2acyc")   
    LP2ATOMIC_PATH                = TOOL_FOLDER_PATH.joinpath("lp2atomic")   

    FILES_FOLDER_PATH           = DEFAULT_PATH.joinpath("resources","files")
    FACTORY_DUMP                = FILES_FOLDER_PATH.joinpath("factory.json")
    ASP_PARSER_FILE             = FILES_FOLDER_PATH.joinpath("parsing.asp")
    TO_GROUND_PROGRAM_FILE      = FILES_FOLDER_PATH.joinpath("subprogram.asp")
    GROUND_PROGRAM_FILE         = FILES_FOLDER_PATH.joinpath("subprogram.lparse")
    QBF_PROGRAM_FILE            = FILES_FOLDER_PATH.joinpath("formula.qcir")
    WORKING_QBF_PROGRAM_FILE    = FILES_FOLDER_PATH.joinpath("working_formula.qcir")
    TMP_QBF_PROGRAM_FILE        = FILES_FOLDER_PATH.joinpath("tmp_formula.qcir")
    QDIMACS_PROGRAM_FILE        = FILES_FOLDER_PATH.joinpath("formula.qdimacs")
    GATES_PROGRAM_FILE          = FILES_FOLDER_PATH.joinpath("builder.qcir")
    QCIR_SUB_FORMULA_PREFIX     = FILES_FOLDER_PATH.joinpath("subformula")
    
    LOG_ERROR                   = FILES_FOLDER_PATH.joinpath("log.err")
    def cleanup():
        print("Cleaning up working directory")
        for file in [FILE_UTIL.FACTORY_DUMP, FILE_UTIL.ASP_PARSER_FILE, FILE_UTIL.TO_GROUND_PROGRAM_FILE, FILE_UTIL.GROUND_PROGRAM_FILE, FILE_UTIL.QBF_PROGRAM_FILE, FILE_UTIL.TMP_QBF_PROGRAM_FILE, FILE_UTIL.QDIMACS_PROGRAM_FILE, FILE_UTIL.GATES_PROGRAM_FILE, FILE_UTIL.QCIR_SUB_FORMULA_PREFIX]:
            if os.path.exists(file):
                os.remove(file)
    
class DIMACS_FORMAT:
    DIMACS_COMMENT_VAR_INDEX    = 1
    DIMACS_COMMENT_ATOM_INDEX   = 2

class QCIR_FORMAT:
    OR_GATE                     = "or"
    AND_GATE                    = "and"
    FORALL                      = "forall"
    EXISTS                      = "exists"
    OUTPUT                      = "output"
    HEADER                      = "#QCIR-G14"

class QDIMACS_FORMAT:
    FORALL                      = "a"
    EXISTS                      = "e"

class QASP_FORMAT:
    QFORALL                     = QCIR_FORMAT.FORALL
    QEXISTS                     = QCIR_FORMAT.EXISTS
    QCONSTRAINT                 = "constraint"

class REGEX_UTIL:

    QASP_QUANTIFIER             = r'\s*%@(forall|exists|constraint)\s*\n'
    QCIR_QUANTIFIER             = r'(forall|exists|constraint)\((.*)\)\n'
    QCIR_GATE                   = r'\d+ = (or|and)\((.*)\)\n'
    RAREQS_OUT                  = r's cnf (0|1)\n'

class ASP_PARSER_FORMAT:
    HEAD_PREDICATE_PREFIX       = 0
    NO_HEAD_PREDICATE_PREFIX    = 1
    TIGHT_PROP_PREFIX           = 2
    SEPARATOR                   = ":"

class QUABS_OUTPUT:
    SAT                         = "SAT"
    UNSAT                       = "UNSAT"
    MODEL_START                 = "V"

class RAREQS_OUTPUT:
    SAT                         = "1"
    UNSAT                       = "0"

class DEPQBF_OUTPUT:
    SAT                         = "SAT"
    UNSAT                       = "UNSAT"

class PYQASP_OUTPUT:
    SAT                         = "SATISFIABLE"
    UNSAT                       = "UNSATISFIABLE"
    EXTENDED                    = "PyQasp::"

class EmptyDebugger:
    def printMessage(self,message):
        return

class Debugger:
    def printMessage(self,message):
        print(message)
        return

class EmptyDebugCommand:
    def getOutput(self,cmd):
        return ""

class DebugCommand(EmptyDebugCommand):
    def getOutput(self,cmd):
        return subprocess.getoutput(cmd)

class DEFAULT_PROPERTIES:
    GUESS_CHECK                 = False
    COUNTING                    = False
    NO_WF                       = False
    ONLY_CHOICE                 = True
    SKIP_QCIR_CONV_FOR_QDIMACS  = True
    PRINT_STATS                 = False
    PRINT_ASPSTATS              = False
    SATISFIABILITY              = False
    debug                       = EmptyDebugger()
    debugcmd                    = EmptyDebugCommand()
    