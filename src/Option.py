import sys,os

class PyQASPOptions:
    DLV2 = 0
    DLV2_WELL_FOUNDED = 1
    GRINGO = 2

class LPARSE_FORMAT:
    SIMPLE_RULE=1
    CHOICE_RULE=3
    DISJCUNTIVE_RULE=8

    RULE_TYPE_INDEX=0
    HEAD_LENGHT_INDEX=1
    ONE_HEAD_ATOM_INDEX=1
    BODY_SIZE_INDEX=2
    NEG_BODY_SIZE_INDEX=3

    SEPARATOR = " "

class FILE_UTIL:
    DEFAULT_PATH                = sys._MEIPASS
    # DEFAULT_PATH                = ""
    ASP_PARSER_PATH             = os.path.join(DEFAULT_PATH,"resources","asp-parser","aspToJson") 
    # ASP_PARSER_PATH             = os.path.join(DEFAULT_PATH,"resources","asp-parser","ruleToJson") 
    ASP_RULE_PARSER_PATH        = os.path.join(DEFAULT_PATH,"resources","asp-parser","ParserByRule") 
    ASP_PROGRAM_PARSER_PATH     = os.path.join(DEFAULT_PATH,"resources","asp-parser","ProgramParser") 
    OLD_TOOL_FOLDER_PATH        = os.path.join(DEFAULT_PATH,"resources","old-tool")
    TOOL_FOLDER_PATH            = os.path.join(DEFAULT_PATH,"resources","tools")
    
    QUABS_PATH                  = os.path.join(TOOL_FOLDER_PATH,"quabs")
    DEPQBF_PATH                 = os.path.join(TOOL_FOLDER_PATH,"depqbf")
    RAREQS_NN_PATH              = os.path.join(TOOL_FOLDER_PATH,"rareqs-nn")
    
    QCIR_CONV_PATH              = os.path.join(TOOL_FOLDER_PATH,"qcir-conv.py")
    BLOQQER37_PATH              = os.path.join(TOOL_FOLDER_PATH,"bloqqer37")
    FMLA_PATH                   = os.path.join(TOOL_FOLDER_PATH,"fmla")
    
    GRINGO_PATH                 = os.path.join(TOOL_FOLDER_PATH,"gringo")        
    DLV2_PATH                   = os.path.join(TOOL_FOLDER_PATH,"dlv2")          
    
    OLD_LPSHIFT_PATH                = os.path.join(TOOL_FOLDER_PATH,"lpshift-1.4")   
    OLD_LP2NORMAL_PATH              = os.path.join(TOOL_FOLDER_PATH,"lp2normal-2.27")
    OLD_LP2SAT_PATH                 = os.path.join(TOOL_FOLDER_PATH,"lp2sat-1.24")   

    LPSHIFT_PATH                = os.path.join(TOOL_FOLDER_PATH,"lpshift")   
    LP2NORMAL_PATH              = os.path.join(TOOL_FOLDER_PATH,"lp2normal2")
    LP2SAT_PATH                 = os.path.join(TOOL_FOLDER_PATH,"lp2sat")   
    LP2ACYC_PATH                = os.path.join(TOOL_FOLDER_PATH,"lp2acyc")   

    FILES_FOLDER_PATH           = os.path.join(DEFAULT_PATH,"resources","files")
    FACTORY_DUMP                = os.path.join(FILES_FOLDER_PATH,"factory.json")
    ASP_PARSER_FILE             = os.path.join(FILES_FOLDER_PATH,"parsing.asp")
    TO_GROUND_PROGRAM_FILE      = os.path.join(FILES_FOLDER_PATH,"subprogram.asp")
    GROUND_PROGRAM_FILE         = os.path.join(FILES_FOLDER_PATH,"subprogram.lparse")
    QBF_PROGRAM_FILE            = os.path.join(FILES_FOLDER_PATH,"formula.qcir")
    QDIMACS_PROGRAM_FILE        = os.path.join(FILES_FOLDER_PATH,"formula.qdimacs")
    GATES_PROGRAM_FILE          = os.path.join(FILES_FOLDER_PATH,"builder.qcir")
    QCIR_SUB_FORMULA_PREFIX     = os.path.join(FILES_FOLDER_PATH,"subformula")
    
    LOG_ERROR                   = os.path.join(FILES_FOLDER_PATH,"log.err")

    
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

class DEFAULT_PROPERTIES:
    GUESS_CHECK                 = False
    ONLY_CHOICE                 = True
    SKIP_QCIR_CONV_FOR_QDIMACS  = True
    PRINT_STATS                 = False