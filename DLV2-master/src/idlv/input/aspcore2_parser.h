/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_SRC_IDLV_INPUT_ASPCORE2_PARSER_H_INCLUDED
# define YY_YY_SRC_IDLV_INPUT_ASPCORE2_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    SYMBOLIC_CONSTANT = 258,
    NUMBER = 259,
    VARIABLE = 260,
    STRING = 261,
    DIRECTIVE_NAME = 262,
    DIRECTIVE_VALUE = 263,
    AGGR_COUNT = 264,
    AGGR_MAX = 265,
    AGGR_MIN = 266,
    AGGR_SUM = 267,
    ANNOTATION_RULE_ORDERING = 268,
    ANNOTATION_ORDERING_VALUE = 269,
    NUMBER_ANNOTATION = 270,
    ANNOTATION_RULE_PROJECTION = 271,
    ANNOTATION_RULE_LOOK_AHEAD = 272,
    ANNOTATION_RULE_ALIGN_SUBSTITUTIONS = 273,
    ANNOTATION_RULE_REWRITING_ARITH = 274,
    ANNOTATION_RULE_ATOM_INDEXED = 275,
    ANNOTATION_ATOM_INDEXED_ATOM = 276,
    ANNOTATION_ATOM_INDEXED_ARGUMENTS = 277,
    ANNOTATION_RULE_PARTIAL_ORDER = 278,
    ANNOTATION_PARTIAL_ORDER_BEFORE = 279,
    ANNOTATION_PARTIAL_ORDER_AFTER = 280,
    ANNOTATION_GLOBAL_ORDERING = 281,
    ANNOTATION_GLOBAL_ATOM_INDEXED = 282,
    ANNOTATION_GLOBAL_PARTIAL_ORDER = 283,
    ANNOTATION_GLOBAL_EXTATOM_CONVERSION = 284,
    ANNOTATION_EXTATOM_PREDICATE = 285,
    ANNOTATION_EXTATOM_TYPE = 286,
    ANNOTATION_EXTATOM_TYPE_QCONST = 287,
    ANNOTATION_EXTATOM_TYPE_CONST = 288,
    ANNOTATION_EXTATOM_TYPE_U_INT = 289,
    ANNOTATION_EXTATOM_TYPE_UT_INT = 290,
    ANNOTATION_EXTATOM_TYPE_UR_INT = 291,
    ANNOTATION_EXTATOM_TYPE_T_INT = 292,
    ANNOTATION_EXTATOM_TYPE_R_INT = 293,
    ANNOTATION_GLOBAL_WASP_PROPAGATOR = 294,
    ANNOTATION_GLOBAL_WASP_HEURISTIC = 295,
    ANNOTATION_GLOBAL_WASP_HEURISTIC_FILE = 296,
    ANNOTATION_GLOBAL_WASP_HEURISTIC_ELEMENTS = 297,
    ANNOTATION_RULE_TO_DECOMPOSE = 298,
    ANNOTATION_RULE_TO_NOT_DECOMPOSE = 299,
    ERROR = 300,
    NEWLINE = 301,
    DOT = 302,
    DDOT = 303,
    SEMICOLON = 304,
    COLON = 305,
    CONS = 306,
    QUERY_MARK = 307,
    PLUS = 308,
    TIMES = 309,
    SLASH = 310,
    BACK_SLASH = 311,
    ANON_VAR = 312,
    PARAM_OPEN = 313,
    PARAM_CLOSE = 314,
    SQUARE_OPEN = 315,
    SQUARE_CLOSE = 316,
    CURLY_OPEN = 317,
    CURLY_CLOSE = 318,
    EQUAL = 319,
    UNEQUAL = 320,
    LESS = 321,
    GREATER = 322,
    LESS_OR_EQ = 323,
    GREATER_OR_EQ = 324,
    DASH = 325,
    COMMA = 326,
    NAF = 327,
    AT = 328,
    WCONS = 329,
    AMPERSAND = 330,
    VEL = 331,
    EXISTS = 332
  };
#endif
/* Tokens.  */
#define SYMBOLIC_CONSTANT 258
#define NUMBER 259
#define VARIABLE 260
#define STRING 261
#define DIRECTIVE_NAME 262
#define DIRECTIVE_VALUE 263
#define AGGR_COUNT 264
#define AGGR_MAX 265
#define AGGR_MIN 266
#define AGGR_SUM 267
#define ANNOTATION_RULE_ORDERING 268
#define ANNOTATION_ORDERING_VALUE 269
#define NUMBER_ANNOTATION 270
#define ANNOTATION_RULE_PROJECTION 271
#define ANNOTATION_RULE_LOOK_AHEAD 272
#define ANNOTATION_RULE_ALIGN_SUBSTITUTIONS 273
#define ANNOTATION_RULE_REWRITING_ARITH 274
#define ANNOTATION_RULE_ATOM_INDEXED 275
#define ANNOTATION_ATOM_INDEXED_ATOM 276
#define ANNOTATION_ATOM_INDEXED_ARGUMENTS 277
#define ANNOTATION_RULE_PARTIAL_ORDER 278
#define ANNOTATION_PARTIAL_ORDER_BEFORE 279
#define ANNOTATION_PARTIAL_ORDER_AFTER 280
#define ANNOTATION_GLOBAL_ORDERING 281
#define ANNOTATION_GLOBAL_ATOM_INDEXED 282
#define ANNOTATION_GLOBAL_PARTIAL_ORDER 283
#define ANNOTATION_GLOBAL_EXTATOM_CONVERSION 284
#define ANNOTATION_EXTATOM_PREDICATE 285
#define ANNOTATION_EXTATOM_TYPE 286
#define ANNOTATION_EXTATOM_TYPE_QCONST 287
#define ANNOTATION_EXTATOM_TYPE_CONST 288
#define ANNOTATION_EXTATOM_TYPE_U_INT 289
#define ANNOTATION_EXTATOM_TYPE_UT_INT 290
#define ANNOTATION_EXTATOM_TYPE_UR_INT 291
#define ANNOTATION_EXTATOM_TYPE_T_INT 292
#define ANNOTATION_EXTATOM_TYPE_R_INT 293
#define ANNOTATION_GLOBAL_WASP_PROPAGATOR 294
#define ANNOTATION_GLOBAL_WASP_HEURISTIC 295
#define ANNOTATION_GLOBAL_WASP_HEURISTIC_FILE 296
#define ANNOTATION_GLOBAL_WASP_HEURISTIC_ELEMENTS 297
#define ANNOTATION_RULE_TO_DECOMPOSE 298
#define ANNOTATION_RULE_TO_NOT_DECOMPOSE 299
#define ERROR 300
#define NEWLINE 301
#define DOT 302
#define DDOT 303
#define SEMICOLON 304
#define COLON 305
#define CONS 306
#define QUERY_MARK 307
#define PLUS 308
#define TIMES 309
#define SLASH 310
#define BACK_SLASH 311
#define ANON_VAR 312
#define PARAM_OPEN 313
#define PARAM_CLOSE 314
#define SQUARE_OPEN 315
#define SQUARE_CLOSE 316
#define CURLY_OPEN 317
#define CURLY_CLOSE 318
#define EQUAL 319
#define UNEQUAL 320
#define LESS 321
#define GREATER 322
#define LESS_OR_EQ 323
#define GREATER_OR_EQ 324
#define DASH 325
#define COMMA 326
#define NAF 327
#define AT 328
#define WCONS 329
#define AMPERSAND 330
#define VEL 331
#define EXISTS 332

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 33 "src/idlv/input/aspcore2.y" /* yacc.c:1909  */

    char* string;
    char single_char;
    int integer;

#line 214 "src/idlv/input/aspcore2_parser.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (DLV2::InputDirector& director);

#endif /* !YY_YY_SRC_IDLV_INPUT_ASPCORE2_PARSER_H_INCLUDED  */
