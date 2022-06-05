
// Generated from ASPCore2.g4 by ANTLR 4.10.1

#pragma once


#include "antlr4-runtime.h"




class  ASPCore2Lexer : public antlr4::Lexer {
public:
  enum {
    NAF = 1, SYMBOLIC_CONSTANT = 2, VARIABLE = 3, STRING = 4, NUMBER = 5, 
    WS = 6, DOT = 7, DDOT = 8, COMMA = 9, HEAD_SEPARATOR = 10, SEMICOLON = 11, 
    COLON = 12, AT = 13, CONS = 14, WCONS = 15, PLUS = 16, DASH = 17, TIMES = 18, 
    SLASH = 19, BACK_SLASH = 20, PARAM_OPEN = 21, PARAM_CLOSE = 22, SQUARE_OPEN = 23, 
    SQUARE_CLOSED = 24, CURLY_OPEN = 25, CURLY_CLOSE = 26, QUERY_MARK = 27, 
    ANON_VAR = 28, EQUAL = 29, UNEQUAL = 30, LESS = 31, GREATER = 32, LESS_OR_EQ = 33, 
    GREATER_OR_EQ = 34, AMPERSAND = 35, EXISTS = 36, AGGR_COUNT = 37, AGGR_MAX = 38, 
    AGGR_MIN = 39, AGGR_SUM = 40, ANNOTATION_GLOBAL_WASP_PROPAGATOR = 41, 
    DIRECTIVE_NAME = 42, DIRECTIVE_VALUE = 43, ANNOTATION_RULE_ALIGN_SUBSTITUTIONS = 44, 
    ANNOTATION_RULE_LOOK_AHEAD = 45, ANNOTATION_RULE_PROJECTION = 46, ANNOTATION_RULE_REWRITING_ARITH = 47, 
    ANNOTATION_RULE_ORDERING = 48, ANNOTATION_ORDERING_VALUE = 49, ANNOTATION_RULE_ATOM_INDEXED = 50, 
    ANNOTATION_ATOM_INDEXED_ATOM = 51, ANNOTATION_ATOM_INDEXED_ARGUMENTS = 52, 
    ANNOTATION_RULE_PARTIAL_ORDER = 53, ANNOTATION_PARTIAL_ORDER_BEFORE = 54, 
    ANNOTATION_PARTIAL_ORDER_AFTER = 55, ANNOTATION_EXTATOM_PREDICATE = 56, 
    ANNOTATION_EXTATOM_TYPE = 57, ANNOTATION_EXTATOM_TYPE_QCONST = 58, ANNOTATION_EXTATOM_TYPE_CONST = 59, 
    ANNOTATION_EXTATOM_TYPE_U_INT = 60, ANNOTATION_EXTATOM_TYPE_UR_INT = 61, 
    ANNOTATION_EXTATOM_TYPE_UT_INT = 62, ANNOTATION_EXTATOM_TYPE_R_INT = 63, 
    ANNOTATION_EXTATOM_TYPE_T_INT = 64, ANNOTATION_GLOBAL_ORDERING = 65, 
    ANNOTATION_GLOBAL_ATOM_INDEXED = 66, ANNOTATION_GLOBAL_PARTIAL_ORDER = 67, 
    ANNOTATION_GLOBAL_EXTATOM_CONVERSION = 68, ANNOTATION_RULE_TO_DECOMPOSE = 69, 
    ANNOTATION_RULE_TO_NOT_DECOMPOSE = 70, ANNOTATION_GLOBAL_WASP_HEURISTIC = 71, 
    ANNOTATION_GLOBAL_WASP_HEURISTIC_FILE = 72, ANNOTATION_GLOBAL_WASP_HEURISTIC_ELEMENTS = 73, 
    EMPTY_COMMENT = 74, COMMENT = 75
  };

  explicit ASPCore2Lexer(antlr4::CharStream *input);

  ~ASPCore2Lexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

