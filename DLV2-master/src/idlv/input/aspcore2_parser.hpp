/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "src/idlv/input/aspcore2.y" /* yacc.c:339  */

//////////////////////////////////////////////////////////////////////////////
// aspcore2.y

//////////////////////////////////////////////////////////////////////////////
/*
This file is part of the ASPCOMP2013 ASP-Core-2 validator (validator in the following)

    The validator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The validator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the validator.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "InputDirector.h"
#include <iostream>

bool queryFound=false;



#line 96 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif


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
#line 33 "src/idlv/input/aspcore2.y" /* yacc.c:355  */

    char* string;
    char single_char;
    int integer;

#line 293 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (DLV2::InputDirector& director);



/* Copy the second part of user declarations.  */

#line 310 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  134
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   939

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  78
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  112
/* YYNRULES -- Number of rules.  */
#define YYNRULES  259
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  538

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   332

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    86,    86,    88,    90,    91,    96,    97,   101,   102,
     103,   104,   108,   112,   116,   120,   124,   128,   138,   142,
     150,   157,   162,   170,   175,   179,   189,   193,   197,   201,
     208,   212,   219,   220,   221,   222,   226,   233,   240,   241,
     245,   249,   256,   263,   267,   274,   278,   285,   289,   293,
     297,   301,   308,   309,   313,   320,   326,   327,   331,   336,
     341,   349,   354,   359,   364,   372,   378,   379,   383,   384,
     388,   395,   399,   406,   407,   408,   413,   414,   418,   419,
     420,   424,   428,   433,   440,   444,   449,   454,   459,   466,
     467,   473,   474,   482,   483,   489,   490,   491,   495,   500,
     505,   510,   518,   523,   530,   538,   542,   549,   553,   560,
     569,   574,   578,   583,   591,   592,   593,   597,   607,   614,
     621,   628,   629,   633,   640,   644,   645,   649,   656,   660,
     667,   674,   675,   679,   680,   681,   682,   683,   687,   691,
     698,   702,   709,   710,   714,   715,   720,   727,   732,   737,
     742,   750,   751,   752,   753,   754,   755,   759,   760,   764,
     765,   766,   767,   768,   769,   770,   771,   772,   777,   777,
     785,   785,   792,   793,   797,   798,   802,   812,   821,   822,
     827,   831,   835,   839,   843,   847,   851,   858,   865,   872,
     877,   882,   886,   893,   897,   904,   908,   915,   919,   926,
     930,   934,   938,   942,   946,   951,   959,   963,   967,   971,
     975,   979,   984,   991,   995,   999,  1003,  1007,  1011,  1016,
    1023,  1027,  1031,  1035,  1039,  1043,  1048,  1055,  1056,  1060,
    1061,  1065,  1066,  1070,  1071,  1075,  1081,  1087,  1089,  1094,
    1096,  1101,  1105,  1113,  1113,  1120,  1120,  1127,  1127,  1134,
    1139,  1146,  1153,  1158,  1165,  1173,  1181,  1185,  1192,  1193
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SYMBOLIC_CONSTANT", "NUMBER",
  "VARIABLE", "STRING", "DIRECTIVE_NAME", "DIRECTIVE_VALUE", "AGGR_COUNT",
  "AGGR_MAX", "AGGR_MIN", "AGGR_SUM", "ANNOTATION_RULE_ORDERING",
  "ANNOTATION_ORDERING_VALUE", "NUMBER_ANNOTATION",
  "ANNOTATION_RULE_PROJECTION", "ANNOTATION_RULE_LOOK_AHEAD",
  "ANNOTATION_RULE_ALIGN_SUBSTITUTIONS", "ANNOTATION_RULE_REWRITING_ARITH",
  "ANNOTATION_RULE_ATOM_INDEXED", "ANNOTATION_ATOM_INDEXED_ATOM",
  "ANNOTATION_ATOM_INDEXED_ARGUMENTS", "ANNOTATION_RULE_PARTIAL_ORDER",
  "ANNOTATION_PARTIAL_ORDER_BEFORE", "ANNOTATION_PARTIAL_ORDER_AFTER",
  "ANNOTATION_GLOBAL_ORDERING", "ANNOTATION_GLOBAL_ATOM_INDEXED",
  "ANNOTATION_GLOBAL_PARTIAL_ORDER",
  "ANNOTATION_GLOBAL_EXTATOM_CONVERSION", "ANNOTATION_EXTATOM_PREDICATE",
  "ANNOTATION_EXTATOM_TYPE", "ANNOTATION_EXTATOM_TYPE_QCONST",
  "ANNOTATION_EXTATOM_TYPE_CONST", "ANNOTATION_EXTATOM_TYPE_U_INT",
  "ANNOTATION_EXTATOM_TYPE_UT_INT", "ANNOTATION_EXTATOM_TYPE_UR_INT",
  "ANNOTATION_EXTATOM_TYPE_T_INT", "ANNOTATION_EXTATOM_TYPE_R_INT",
  "ANNOTATION_GLOBAL_WASP_PROPAGATOR", "ANNOTATION_GLOBAL_WASP_HEURISTIC",
  "ANNOTATION_GLOBAL_WASP_HEURISTIC_FILE",
  "ANNOTATION_GLOBAL_WASP_HEURISTIC_ELEMENTS",
  "ANNOTATION_RULE_TO_DECOMPOSE", "ANNOTATION_RULE_TO_NOT_DECOMPOSE",
  "ERROR", "NEWLINE", "DOT", "DDOT", "SEMICOLON", "COLON", "CONS",
  "QUERY_MARK", "PLUS", "TIMES", "SLASH", "BACK_SLASH", "ANON_VAR",
  "PARAM_OPEN", "PARAM_CLOSE", "SQUARE_OPEN", "SQUARE_CLOSE", "CURLY_OPEN",
  "CURLY_CLOSE", "EQUAL", "UNEQUAL", "LESS", "GREATER", "LESS_OR_EQ",
  "GREATER_OR_EQ", "DASH", "COMMA", "NAF", "AT", "WCONS", "AMPERSAND",
  "VEL", "EXISTS", "$accept", "HEAD_SEPARATOR", "program", "rules", "rule",
  "simple_rule", "head", "body", "weight_at_levels", "levels_and_terms",
  "disjunction", "conjunction", "choice_atom", "lower_guard",
  "upper_guard", "choice_elements", "choice_element",
  "choice_element_atom", "choice_elements_literals", "naf_literals",
  "naf_literal", "naf_literal_aggregate", "existential_atom",
  "classic_literal", "atom", "extAtom", "extSemicol", "terms",
  "basic_terms", "builtin_atom", "compareop", "binop", "arithop1",
  "arithop2", "term_", "term__", "term", "expr", "factor", "basic_term",
  "ground_term", "variable_term", "functional_term", "list_term",
  "list_terms_term", "list_head_tail_term", "list_tail_term", "vars",
  "identifier", "directive", "query", "lower_guard_compare_aggregate",
  "upper_guard_compare_aggregate", "compare_aggregate",
  "lower_guard_leftward_left_aggregate", "leftward_left_aggregate",
  "left_aggregate", "lower_guard_rightward_left_aggregate",
  "rightward_left_aggregate", "upper_guard_leftward_right_aggregate",
  "upper_guard_rightward_right_aggregate", "right_aggregate",
  "aggregate_atom", "leftwardop", "rightwardop", "aggregate",
  "aggregate_elements", "aggregate_element", "aggregate_function",
  "annotation_global", "annotations_rule", "annotation_rule",
  "annotation_rule_to_decompose", "annotation_rule_to_not_decompose",
  "annotation_rule_ordering", "annotation_global_ordering",
  "annotation_global_extatom_conversion", "annotation_extpredicate",
  "extatom_conv_types", "extatom_conv_type", "rule_ordering_type",
  "global_ordering_type", "annotation_rule_atom_indexed",
  "annotation_global_atom_indexed", "rule_indexing_arguments",
  "rule_naf_literal_annotation", "global_indexing_arguments",
  "global_naf_literal_annotation", "rule_atom_annotation_before",
  "global_atom_annotation_before", "rule_atom_annotation_after",
  "global_atom_annotation_after", "rule_atoms_annotation_before",
  "global_atoms_annotation_before", "rule_atoms_annotation_after",
  "global_atoms_annotation_after", "annotation_rule_partial_order_begin",
  "annotation_global_partial_order_begin", "annotation_rule_partial_order",
  "annotation_global_partial_order", "annotation_rule_projection",
  "annotation_rule_projection_value", "annotation_rule_rewriting_arith",
  "annotation_rule_align_substitutions", "annotation_rule_look_ahead",
  "annotation_global_wasp_propagator",
  "annotation_global_wasp_propagator_begin",
  "annotation_global_wasp_heuristic",
  "annotation_global_wasp_heuristic_begin", "heuristic_python_file",
  "aggregate_element_heuristic", "aggregate_elements_heuristic", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332
};
# endif

#define YYPACT_NINF -411

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-411)))

#define YYTABLE_NINF -128

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     597,  -411,  -411,     2,  -411,  -411,   104,     9,    74,   103,
     155,   172,    91,  -411,   134,   138,  -411,   158,  -411,  -411,
     202,   207,   763,  -411,   185,   176,   279,   185,   763,   114,
     220,   659,  -411,  -411,   -10,   145,  -411,   173,  -411,  -411,
     205,  -411,  -411,   396,   -13,   249,  -411,   335,  -411,  -411,
    -411,   688,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,
     204,   208,  -411,  -411,  -411,  -411,  -411,  -411,  -411,   223,
    -411,   230,   303,  -411,    69,   294,  -411,   253,  -411,   254,
    -411,   260,   293,   113,   299,   291,  -411,   265,  -411,   266,
    -411,  -411,  -411,  -411,    65,   279,   282,   268,  -411,  -411,
    -411,  -411,  -411,  -411,   396,   259,  -411,   259,   195,  -411,
     259,   222,  -411,  -411,   396,   269,   185,   278,   283,  -411,
     280,   274,   288,  -411,    61,  -411,   304,  -411,   300,  -411,
    -411,   306,  -411,    55,  -411,  -411,  -411,   481,  -411,    15,
     279,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,
    -411,  -411,  -411,  -411,    50,  -411,  -411,  -411,    50,   529,
    -411,  -411,   162,   215,   252,   255,  -411,  -411,   308,   298,
     310,   312,   320,   321,   311,  -411,   317,   326,   324,   325,
     323,   343,  -411,  -411,   396,  -411,   334,  -411,   763,   368,
     185,   408,   418,  -411,  -411,  -411,   185,  -411,  -411,   185,
     185,  -411,  -411,  -411,   273,  -411,   185,   185,    18,  -411,
     279,   396,    76,   529,   336,   389,  -411,  -411,   355,   279,
    -411,  -411,    67,  -411,   249,  -411,  -411,  -411,   -24,  -411,
     340,   341,   345,   346,   351,   369,   370,   372,   428,   390,
     445,  -411,  -411,  -411,    59,   446,   404,    83,   378,  -411,
    -411,  -411,  -411,  -411,   509,  -411,  -411,  -411,  -411,  -411,
      16,  -411,    22,    34,  -411,  -411,   450,    57,  -411,  -411,
    -411,  -411,   398,    93,  -411,    95,  -411,  -411,  -411,  -411,
    -411,  -411,   185,    39,   386,  -411,   396,   147,   185,  -411,
    -411,  -411,   396,   403,   412,   414,   415,   419,   452,   426,
     452,   432,   421,  -411,  -411,   436,    43,  -411,   425,   438,
    -411,    43,  -411,   427,   279,   433,  -411,  -411,   185,   -35,
    -411,    76,   295,   185,   295,  -411,  -411,  -411,    76,  -411,
     122,  -411,   774,   785,   796,   807,  -411,   434,   342,   437,
     342,   460,   462,  -411,   477,   463,  -411,   495,  -411,   487,
     153,   869,   449,  -411,  -411,   156,  -411,  -411,  -411,   185,
     185,   464,    65,  -411,  -411,  -411,  -411,   455,   467,    65,
    -411,  -411,  -411,  -411,   456,   468,    65,  -411,  -411,  -411,
    -411,   465,   474,    65,  -411,  -411,  -411,  -411,   469,   486,
     508,   441,  -411,   496,  -411,  -411,   108,   513,   110,  -411,
    -411,   459,  -411,   493,   498,  -411,  -411,   170,    76,  -411,
     280,   488,  -411,  -411,  -411,  -411,   774,   494,  -411,  -411,
    -411,   785,   500,  -411,  -411,  -411,   796,   501,  -411,  -411,
    -411,   807,   502,   511,   529,   342,   503,   512,   507,   518,
     519,   510,  -411,  -411,   185,  -411,   539,  -411,   558,  -411,
     560,  -411,   559,   532,   179,  -411,   554,   534,   564,   602,
     603,  -411,  -411,  -411,  -411,  -411,  -411,  -411,   549,   540,
     280,   548,   555,   557,   563,   342,    97,   565,   342,   566,
    -411,   193,  -411,   196,   571,   510,   569,   570,   572,   576,
     111,   452,   136,   452,   574,   618,   580,   624,  -411,  -411,
     785,   774,   807,   796,   583,   584,   585,   587,   600,  -411,
     604,  -411,   586,   589,   590,   593,   611,   613,   614,   621,
    -411,  -411,   591,   610,   622,   625,  -411,  -411,  -411,  -411,
     623,   626,   633,   636,  -411,  -411,  -411,  -411
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     5,   114,    87,   116,   115,     0,     0,     0,     0,
       0,     0,     0,   235,     0,     0,   236,     0,   251,   254,
       0,     0,     0,    81,     0,     0,     0,     0,     0,     0,
       0,     4,     6,     8,     0,    18,    19,     0,    28,    26,
      56,    89,    93,     0,    90,    91,    85,    86,    11,    17,
      10,     0,   157,   166,   167,   159,   151,   154,   160,   152,
       0,     0,   161,   153,   162,   163,   164,   165,   156,     0,
     155,     0,     0,   117,     0,     0,   248,     0,   246,     0,
     244,     0,     0,     0,     0,     0,   169,     0,   171,     0,
     147,   148,   149,   150,     0,     0,     0,    20,    52,    30,
      47,    56,    50,    49,     0,     0,   135,     0,   125,   133,
       0,   126,   134,    53,     0,     0,     0,     0,    86,   105,
     107,    66,     0,   108,     0,    39,    40,    42,    58,    57,
      84,     0,   112,     0,     1,     7,    12,     0,     2,     0,
       0,   118,    71,    72,   138,   140,   139,   141,    73,    36,
      74,    75,    76,    77,     0,    78,    79,    80,     0,     0,
       9,   158,     0,     0,     0,     0,    83,   187,     0,     0,
       0,     0,     0,     0,     0,   188,     0,     0,     0,     0,
       0,     0,    48,    51,     0,    54,     0,    15,     0,    73,
       0,    74,    75,   121,   124,   136,     0,   128,   137,     0,
       0,   122,   131,   132,     0,    88,     0,     0,     0,   106,
       0,    34,     0,     0,     0,     0,    55,    13,     0,     0,
      29,    27,     0,    87,    92,    86,    94,    60,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   247,   245,   243,     0,     0,     0,     0,     0,   168,
     170,   119,   123,   127,     0,    31,    70,   129,   130,   120,
     114,   100,   116,   115,   103,   143,     0,     0,    68,    95,
      96,    97,     0,     0,   145,     0,    67,   110,   111,   109,
      38,    35,     0,     0,    41,    43,     0,     0,     0,    16,
     113,    14,    33,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   173,   242,     0,     0,   193,     0,     0,
     175,     0,   197,     0,     0,     0,    65,    64,     0,     0,
     101,     0,     0,     0,     0,   142,    82,    37,     0,    59,
       0,    32,     0,     0,     0,     0,   255,     0,     0,     0,
       0,     0,     0,   194,     0,     0,   198,     0,   177,     0,
       0,     0,   146,    45,    69,     0,   144,    44,    21,     0,
       0,     0,     0,   199,   204,   201,   202,   227,     0,     0,
     213,   218,   215,   216,   231,     0,     0,   206,   211,   208,
     209,   229,     0,     0,   220,   225,   222,   223,   233,     0,
       0,     0,   257,    58,   256,   258,     0,     0,     0,   172,
     241,     0,   174,     0,     0,    62,    63,     0,     0,   104,
      25,    23,    22,   200,   205,   203,     0,     0,   214,   219,
     217,     0,     0,   207,   212,   210,     0,     0,   221,   226,
     224,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    46,     0,   228,     0,   232,     0,   230,
       0,   234,     0,     0,     0,   259,     0,     0,     0,     0,
       0,   180,   181,   182,   183,   184,   185,   186,     0,   178,
      24,     0,     0,     0,     0,     0,    59,     0,     0,     0,
     191,     0,   195,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   176,   179,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   192,
       0,   196,     0,     0,     0,     0,     0,     0,     0,     0,
     189,   190,     0,     0,     0,     0,   249,   250,   252,   253,
       0,     0,     0,     0,   237,   238,   239,   240
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -411,  -411,  -411,  -411,   658,   639,  -411,   -22,  -411,  -411,
    -411,  -411,  -411,  -411,   405,   556,   485,  -411,  -411,  -411,
    -210,   521,   561,     0,    12,    90,   393,  -154,  -411,  -325,
     -82,   -39,  -411,  -411,  -411,   562,   201,  -411,   568,   391,
    -411,  -411,  -411,   506,  -411,  -411,  -411,  -411,     4,  -411,
    -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,   601,
     609,  -411,   -60,   -89,   -87,    60,  -411,  -203,  -411,  -411,
    -411,   667,  -411,  -411,  -411,  -411,  -411,  -411,   239,  -411,
     489,   480,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,
    -411,  -411,  -403,  -400,  -410,  -398,  -411,  -411,  -411,  -411,
    -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -284,
     302,  -337
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,   139,    30,    31,    32,    33,    34,    96,   289,   361,
      35,    97,    36,    37,   281,   124,   125,   126,   284,   352,
      98,    99,    38,   100,   101,   102,   318,   120,   267,   103,
     148,   190,   154,   158,    41,    42,   104,    44,    45,   268,
     269,   270,   271,    46,   122,   123,   279,   133,    47,    48,
      49,   105,   201,   106,   107,   108,   109,   110,   111,   195,
     198,   112,   113,   150,   151,   114,   273,   394,   115,    50,
      51,    52,    53,    54,    55,    56,    57,   315,   468,   469,
     169,   177,    58,    59,   481,   308,   483,   313,   367,   381,
     374,   388,   368,   382,   375,   389,    60,    61,    62,    63,
      64,   305,    65,    66,    67,    68,    69,    70,    71,   337,
     395,   396
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      39,   274,   285,   398,   149,   228,   131,   365,   372,   379,
     386,   447,    40,   445,   316,   191,   339,   192,     2,   196,
       4,     5,   189,   277,   199,   196,   449,   199,   118,   118,
     128,    39,   200,   451,   185,   293,   207,   136,   127,   129,
     152,   137,     2,    40,     4,     5,     2,   207,     4,     5,
      72,    39,   275,     2,   223,     4,     5,   153,     2,   287,
       4,     5,     2,    40,     4,     5,   -98,    74,     2,     3,
       4,     5,  -102,   167,    90,    91,    92,    93,    25,     2,
       3,     4,     5,   168,   -99,   219,     2,   -98,     4,     5,
     512,   365,    29,  -102,   182,   252,   372,   253,   513,   186,
     319,   379,   251,   515,   514,   -99,   386,   321,    24,   219,
     210,   353,    73,   219,    95,   218,   210,   175,   357,   132,
     118,   356,    23,    24,   211,    25,   215,   176,   322,   219,
     292,   306,    75,    23,    24,    27,    25,   128,   490,   221,
      95,   492,   324,   128,   128,   216,    27,  -104,   283,    82,
      76,    95,   127,   219,   326,   311,   325,   435,   225,   435,
     435,    77,   225,   118,   350,   193,   207,   194,  -104,   355,
     197,   436,   282,   438,   504,   372,   365,   386,   379,     2,
       3,     4,     5,   358,   183,   435,   230,   231,     2,     3,
       4,     5,    83,   359,   118,   360,    84,   407,   443,   506,
     118,    43,    78,   118,   118,   410,   329,   505,   272,   507,
     118,   118,   405,    79,   128,   409,    85,   118,   207,    80,
     134,   138,   127,   128,   207,   117,   121,   207,   130,   442,
      81,   129,    43,    23,    24,   140,    25,   119,   476,   232,
     233,   207,    23,    24,   307,    25,   116,   312,   128,    86,
     207,   128,    43,   282,    88,   116,   494,   141,   118,   496,
      87,   144,   162,   146,   495,    89,   163,   497,    90,    91,
      92,    93,   366,   373,   380,   387,   260,   261,   262,   263,
     454,   164,     2,   182,     4,     5,   118,   128,   165,   145,
     470,   147,   118,   234,   235,   184,   236,   237,   260,   261,
     262,   263,   415,   155,   156,   157,   343,   166,   170,   420,
     128,   346,   171,   172,   174,   128,   425,   130,   348,   173,
     178,   179,   118,   430,   180,   181,   272,   118,   272,   187,
     264,   204,   363,   370,   377,   384,   265,   205,   392,   188,
     392,   206,   393,   266,   393,   260,   261,   262,   263,   209,
     208,   207,   264,   214,   212,   118,   366,   239,   213,   241,
     229,   373,   413,   118,   118,   266,   380,   242,   243,   418,
     249,   387,   238,   183,   240,   244,   423,  -119,  -119,  -119,
    -119,   245,   -58,   428,   -58,   246,   -58,   -58,   247,   248,
     250,   256,   254,   159,   290,   128,   288,   257,   -58,   264,
     258,   259,   291,   129,   294,   295,   -58,   229,   276,   296,
     297,   -58,   391,   286,   229,   298,   363,  -123,  -123,  -123,
    -123,   370,   364,   371,   378,   385,   377,  -127,  -127,  -127,
    -127,   384,   167,   299,   300,   392,   301,   303,   118,   393,
     373,   366,   387,   380,     2,   320,     4,     5,   118,   304,
     175,   310,   414,   314,   320,   229,   323,   328,   336,   419,
     142,   143,   144,   145,   146,   147,   424,   -82,   -82,   -82,
     -82,   -82,   -82,   429,   332,   392,   333,   334,   392,   393,
     341,   335,   393,   327,     2,     3,     4,     5,   338,   330,
      90,    91,    92,    93,   340,   342,   344,   345,   347,   401,
     370,   363,   384,   377,   349,   390,   364,   399,   397,   400,
     402,   371,     2,     3,     4,     5,   378,   403,   404,   229,
     408,   385,   286,   439,   229,   412,   416,   421,   217,   286,
     417,   422,     2,     3,     4,     5,   426,   427,    23,    24,
     431,    25,   461,   462,   463,   464,   465,   466,   467,   432,
     433,    27,   229,    94,   434,   437,    95,   440,   316,   444,
     229,   411,   441,   184,   471,   446,    23,    24,   317,    25,
     184,   448,   450,   452,   456,   453,   457,   184,   458,   116,
     459,   460,   472,   474,   184,   473,    23,    24,   227,    25,
     371,   364,   385,   378,   475,   477,   478,    -3,     1,   116,
       2,     3,     4,     5,     6,   479,   480,   482,   484,   286,
       7,   485,   486,     8,     9,    10,    11,    12,   498,   487,
      13,   488,   509,    14,    15,    16,    17,   489,   511,   491,
     493,   500,   501,   508,   502,   229,    18,    19,   503,   510,
      20,    21,   516,   517,   518,   229,   519,   520,    22,   522,
     530,   521,   523,   524,    23,    24,   525,    25,   526,    26,
     527,   528,     2,     3,     4,     5,     6,    27,   529,   531,
     534,    28,     7,   535,    29,     8,     9,    10,    11,    12,
     536,   532,    13,   537,   533,    14,    15,    16,    17,   135,
     160,     2,     3,     4,     5,   280,   222,   331,    18,    19,
     220,     7,    20,    21,     8,     9,    10,    11,    12,   255,
      22,    13,   351,   354,   278,   202,    23,    24,   161,    25,
     226,    26,   224,   203,   499,   309,     0,   302,     0,    27,
       0,    20,    21,    28,     0,     0,    29,   455,     0,    22,
       0,     0,     0,     0,     0,    23,    24,     0,    25,     0,
      26,     0,     0,     0,     0,     0,     0,     0,    27,     0,
       0,     0,    28,     0,     0,    29,     2,     3,     4,     5,
       0,     0,    90,    91,    92,    93,     0,     2,     3,     4,
       5,     0,     0,    90,    91,    92,    93,     0,     2,     3,
       4,     5,     0,     0,    90,    91,    92,    93,     0,     2,
       3,     4,     5,     0,     0,    90,    91,    92,    93,     0,
       2,     3,     4,     5,     0,     0,    90,    91,    92,    93,
      23,    24,     0,    25,     0,     0,     0,     0,     0,     0,
       0,    23,    24,    27,    25,    94,     0,     0,    95,     0,
       0,     0,    23,    24,    27,    25,   362,     0,     0,    95,
       0,     0,     0,    23,    24,    27,    25,   369,     0,     0,
      95,     0,     0,     0,    23,    24,    27,    25,   376,     0,
       0,    95,     2,     3,     4,     5,     0,    27,     0,   383,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    23,    24,   406,    25,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   116
};

static const yytype_int16 yycheck[] =
{
       0,   204,   212,   340,    43,   159,    28,   332,   333,   334,
     335,   421,     0,   416,    49,   104,   300,   104,     3,   108,
       5,     6,   104,     5,   111,   114,   426,   114,    24,    25,
      26,    31,   114,   431,    94,    59,    71,    47,    26,    27,
      53,    51,     3,    31,     5,     6,     3,    71,     5,     6,
      48,    51,   206,     3,     4,     5,     6,    70,     3,   213,
       5,     6,     3,    51,     5,     6,    50,    58,     3,     4,
       5,     6,    50,     4,     9,    10,    11,    12,    60,     3,
       4,     5,     6,    14,    50,    70,     3,    71,     5,     6,
     500,   416,    77,    71,    94,   184,   421,   184,   501,    95,
     254,   426,   184,   503,   502,    71,   431,    50,    58,    70,
      49,   321,     8,    70,    75,   137,    49,     4,   328,     5,
     116,   324,    57,    58,    63,    60,    71,    14,    71,    70,
      63,    72,    58,    57,    58,    70,    60,   133,   475,   139,
      75,   478,    49,   139,   140,   133,    70,    50,    72,    58,
      47,    75,   140,    70,    59,    72,    63,    49,   154,    49,
      49,    58,   158,   159,   318,   105,    71,   107,    71,   323,
     110,    63,   211,    63,    63,   500,   501,   502,   503,     3,
       4,     5,     6,    61,    94,    49,    24,    25,     3,     4,
       5,     6,    58,    71,   190,    73,    58,   351,   408,    63,
     196,     0,    47,   199,   200,   359,    59,   491,   204,   493,
     206,   207,    59,    58,   210,    59,    58,   213,    71,    47,
       0,    76,   210,   219,    71,    24,    25,    71,    27,    59,
      58,   219,    31,    57,    58,    62,    60,    61,    59,    24,
      25,    71,    57,    58,   244,    60,    70,   247,   244,    47,
      71,   247,    51,   292,    47,    70,    63,    52,   254,    63,
      58,    66,    58,    68,    71,    58,    58,    71,     9,    10,
      11,    12,   332,   333,   334,   335,     3,     4,     5,     6,
     434,    58,     3,   283,     5,     6,   282,   283,    58,    67,
     444,    69,   288,    41,    42,    94,    41,    42,     3,     4,
       5,     6,   362,    54,    55,    56,   306,     4,    14,   369,
     306,   311,    59,    59,    21,   311,   376,   116,   314,    59,
      21,    30,   318,   383,    59,    59,   322,   323,   324,    47,
      57,    62,   332,   333,   334,   335,    63,    59,   338,    71,
     340,    58,   338,    70,   340,     3,     4,     5,     6,    61,
      76,    71,    57,    47,    50,   351,   416,    59,    58,    47,
     159,   421,   362,   359,   360,    70,   426,    47,    47,   369,
      47,   431,    64,   283,    64,    64,   376,     9,    10,    11,
      12,    64,    47,   383,    49,    59,    51,    52,    64,    64,
      47,   190,    58,    58,     5,   391,    60,   196,    63,    57,
     199,   200,    47,   391,    64,    64,    71,   206,   207,    64,
      64,    76,    70,   212,   213,    64,   416,     9,    10,    11,
      12,   421,   332,   333,   334,   335,   426,     9,    10,    11,
      12,   431,     4,    64,    64,   435,    64,    47,   434,   435,
     500,   501,   502,   503,     3,     4,     5,     6,   444,     4,
       4,    47,   362,    75,     4,   254,    58,    71,     6,   369,
      64,    65,    66,    67,    68,    69,   376,    64,    65,    66,
      67,    68,    69,   383,    62,   475,    62,    62,   478,   475,
      59,    62,   478,   282,     3,     4,     5,     6,    62,   288,
       9,    10,    11,    12,    62,    59,    71,    59,    71,    22,
     500,   501,   502,   503,    71,    71,   416,    47,    71,    47,
      47,   421,     3,     4,     5,     6,   426,    22,    31,   318,
      71,   431,   321,    64,   323,    61,    71,    71,    47,   328,
      63,    63,     3,     4,     5,     6,    71,    63,    57,    58,
      71,    60,    32,    33,    34,    35,    36,    37,    38,    63,
      42,    70,   351,    72,    58,    42,    75,    64,    49,    71,
     359,   360,    64,   362,    25,    71,    57,    58,    59,    60,
     369,    71,    71,    71,    71,    64,    64,   376,    71,    70,
      62,    62,    24,    24,   383,    25,    57,    58,    59,    60,
     500,   501,   502,   503,    62,    41,    62,     0,     1,    70,
       3,     4,     5,     6,     7,    41,     4,     4,    59,   408,
      13,    71,    64,    16,    17,    18,    19,    20,    47,    64,
      23,    64,     4,    26,    27,    28,    29,    64,     4,    64,
      64,    62,    62,    59,    62,   434,    39,    40,    62,    59,
      43,    44,    59,    59,    59,   444,    59,    47,    51,    63,
      59,    47,    63,    63,    57,    58,    63,    60,    47,    62,
      47,    47,     3,     4,     5,     6,     7,    70,    47,    59,
      47,    74,    13,    47,    77,    16,    17,    18,    19,    20,
      47,    59,    23,    47,    59,    26,    27,    28,    29,    31,
      51,     3,     4,     5,     6,   210,   140,   292,    39,    40,
     139,    13,    43,    44,    16,    17,    18,    19,    20,   188,
      51,    23,   319,   322,   208,   114,    57,    58,    51,    60,
     158,    62,   154,   114,   485,   245,    -1,   238,    -1,    70,
      -1,    43,    44,    74,    -1,    -1,    77,   435,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    57,    58,    -1,    60,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    74,    -1,    -1,    77,     3,     4,     5,     6,
      -1,    -1,     9,    10,    11,    12,    -1,     3,     4,     5,
       6,    -1,    -1,     9,    10,    11,    12,    -1,     3,     4,
       5,     6,    -1,    -1,     9,    10,    11,    12,    -1,     3,
       4,     5,     6,    -1,    -1,     9,    10,    11,    12,    -1,
       3,     4,     5,     6,    -1,    -1,     9,    10,    11,    12,
      57,    58,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    58,    70,    60,    72,    -1,    -1,    75,    -1,
      -1,    -1,    57,    58,    70,    60,    72,    -1,    -1,    75,
      -1,    -1,    -1,    57,    58,    70,    60,    72,    -1,    -1,
      75,    -1,    -1,    -1,    57,    58,    70,    60,    72,    -1,
      -1,    75,     3,     4,     5,     6,    -1,    70,    -1,    72,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,     5,     6,     7,    13,    16,    17,
      18,    19,    20,    23,    26,    27,    28,    29,    39,    40,
      43,    44,    51,    57,    58,    60,    62,    70,    74,    77,
      80,    81,    82,    83,    84,    88,    90,    91,   100,   101,
     102,   112,   113,   114,   115,   116,   121,   126,   127,   128,
     147,   148,   149,   150,   151,   152,   153,   154,   160,   161,
     174,   175,   176,   177,   178,   180,   181,   182,   183,   184,
     185,   186,    48,     8,    58,    58,    47,    58,    47,    58,
      47,    58,    58,    58,    58,    58,    47,    58,    47,    58,
       9,    10,    11,    12,    72,    75,    85,    89,    98,    99,
     101,   102,   103,   107,   114,   129,   131,   132,   133,   134,
     135,   136,   139,   140,   143,   146,    70,   114,   126,    61,
     105,   114,   122,   123,    93,    94,    95,   102,   126,   102,
     114,    85,     5,   125,     0,    82,    47,    51,    76,    79,
      62,    52,    64,    65,    66,    67,    68,    69,   108,   109,
     141,   142,    53,    70,   110,    54,    55,    56,   111,    58,
      83,   149,    58,    58,    58,    58,     4,     4,    14,   158,
      14,    59,    59,    59,    21,     4,    14,   159,    21,    30,
      59,    59,   101,   103,   114,   140,   126,    47,    71,   108,
     109,   141,   142,   143,   143,   137,   141,   143,   138,   142,
     108,   130,   137,   138,    62,    59,    58,    71,    76,    61,
      49,    63,    50,    58,    47,    71,   102,    47,    85,    70,
     100,   101,    93,     4,   116,   126,   113,    59,   105,   114,
      24,    25,    24,    25,    41,    42,    41,    42,    64,    59,
      64,    47,    47,    47,    64,    64,    59,    64,    64,    47,
      47,   108,   141,   142,    58,    99,   114,   114,   114,   114,
       3,     4,     5,     6,    57,    63,    70,   106,   117,   118,
     119,   120,   126,   144,   145,   105,   114,     5,   121,   124,
      94,    92,   109,    72,    96,    98,   114,   105,    60,    86,
       5,    47,    63,    59,    64,    64,    64,    64,    64,    64,
      64,    64,   158,    47,     4,   179,    72,   101,   163,   159,
      47,    72,   101,   165,    75,   155,    49,    59,   104,   105,
       4,    50,    71,    58,    49,    63,    59,   114,    71,    59,
     114,    92,    62,    62,    62,    62,     6,   187,    62,   187,
      62,    59,    59,   101,    71,    59,   101,    71,   126,    71,
     105,   104,    97,    98,   117,   105,   145,    98,    61,    71,
      73,    87,    72,   101,   103,   107,   140,   166,   170,    72,
     101,   103,   107,   140,   168,   172,    72,   101,   103,   107,
     140,   167,   171,    72,   101,   103,   107,   140,   169,   173,
      71,    70,   101,   126,   145,   188,   189,    71,   189,    47,
      47,    22,    47,    22,    31,    59,    59,   105,    71,    59,
     105,   114,    61,   101,   103,   140,    71,    63,   101,   103,
     140,    71,    63,   101,   103,   140,    71,    63,   101,   103,
     140,    71,    63,    42,    58,    49,    63,    42,    63,    64,
      64,    64,    59,    98,    71,   170,    71,   172,    71,   171,
      71,   173,    71,    64,   105,   188,    71,    64,    71,    62,
      62,    32,    33,    34,    35,    36,    37,    38,   156,   157,
     105,    25,    24,    25,    24,    62,    59,    41,    62,    41,
       4,   162,     4,   164,    59,    71,    64,    64,    64,    64,
     189,    64,   189,    64,    63,    71,    63,    71,    47,   156,
      62,    62,    62,    62,    63,   187,    63,   187,    59,     4,
      59,     4,   172,   170,   173,   171,    59,    59,    59,    59,
      47,    47,    63,    63,    63,    63,    47,    47,    47,    47,
      59,    59,    59,    59,    47,    47,    47,    47
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    78,    79,    80,    80,    80,    81,    81,    82,    82,
      82,    82,    83,    83,    83,    83,    83,    83,    84,    84,
      85,    86,    86,    87,    87,    87,    88,    88,    88,    88,
      89,    89,    90,    90,    90,    90,    91,    92,    93,    93,
      94,    94,    95,    96,    96,    97,    97,    98,    98,    98,
      98,    98,    99,    99,    99,   100,   101,   101,   102,   102,
     102,   103,   103,   103,   103,   104,   105,   105,   106,   106,
     107,   108,   108,   109,   109,   109,   110,   110,   111,   111,
     111,   112,   112,   112,   112,   112,   113,   113,   113,   114,
     114,   115,   115,   116,   116,   117,   117,   117,   118,   118,
     118,   118,   119,   119,   120,   121,   121,   122,   122,   123,
     124,   124,   125,   125,   126,   126,   126,   127,   128,   129,
     130,   131,   131,   132,   133,   134,   134,   135,   136,   137,
     138,   139,   139,   140,   140,   140,   140,   140,   141,   141,
     142,   142,   143,   143,   144,   144,   145,   146,   146,   146,
     146,   147,   147,   147,   147,   147,   147,   148,   148,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   150,   150,
     151,   151,   152,   152,   153,   153,   154,   155,   156,   156,
     157,   157,   157,   157,   157,   157,   157,   158,   159,   160,
     161,   162,   162,   163,   163,   164,   164,   165,   165,   166,
     166,   166,   166,   166,   166,   166,   167,   167,   167,   167,
     167,   167,   167,   168,   168,   168,   168,   168,   168,   168,
     169,   169,   169,   169,   169,   169,   169,   170,   170,   171,
     171,   172,   172,   173,   173,   174,   175,   176,   176,   177,
     177,   178,   179,   180,   180,   181,   181,   182,   182,   183,
     183,   184,   185,   185,   186,   187,   188,   188,   189,   189
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     1,     1,     1,     2,     1,     2,
       1,     1,     2,     3,     4,     3,     4,     1,     1,     1,
       1,     3,     4,     2,     4,     2,     1,     3,     1,     3,
       1,     3,     5,     4,     3,     4,     2,     2,     3,     1,
       1,     3,     1,     1,     3,     1,     3,     1,     2,     1,
       1,     2,     1,     1,     2,     3,     1,     2,     1,     4,
       3,     7,     6,     6,     4,     1,     1,     3,     1,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     2,     1,     1,     1,     3,     1,
       1,     1,     3,     1,     3,     1,     1,     1,     1,     1,
       1,     2,     1,     1,     4,     2,     3,     1,     1,     3,
       1,     1,     1,     3,     1,     1,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     1,     1,     2,     2,     2,
       2,     2,     2,     1,     1,     1,     2,     2,     1,     1,
       1,     1,     4,     3,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     2,
       4,     2,     7,     5,     7,     5,    11,     2,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,    13,
      13,     1,     3,     1,     2,     1,     3,     1,     2,     1,
       2,     1,     1,     2,     1,     2,     1,     2,     1,     1,
       2,     1,     2,     1,     2,     1,     1,     2,     1,     2,
       1,     2,     1,     1,     2,     1,     2,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     1,    15,    15,    15,
      15,     7,     1,     4,     2,     4,     2,     4,     2,    13,
      13,     1,    13,    13,     1,     1,     1,     1,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (director, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, director); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, DLV2::InputDirector& director)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (director);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, DLV2::InputDirector& director)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, director);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule, DLV2::InputDirector& director)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              , director);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, director); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, DLV2::InputDirector& director)
{
  YYUSE (yyvaluep);
  YYUSE (director);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (DLV2::InputDirector& director)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (director);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
#line 90 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { }
#line 1898 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 5:
#line 91 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { yyerror(director,"Generic error"); }
#line 1904 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 12:
#line 109 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onRule(); 
        }
#line 1912 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 13:
#line 113 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onRule(); 
        }
#line 1920 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 14:
#line 117 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onRule(); 
        }
#line 1928 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 15:
#line 121 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onConstraint(); 
        }
#line 1936 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 16:
#line 125 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onWeakConstraint(); 
        }
#line 1944 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 17:
#line 129 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
			if(!queryFound) 
				{ queryFound=true; director.getBuilder()->onQuery(); } 
			else 
				{ yyerror(director,"A query has been already found"); }
		}
#line 1955 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 18:
#line 139 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onHead();
        }
#line 1963 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 19:
#line 143 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onChoiceAtom();
            director.getBuilder()->onHead();
        }
#line 1972 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 20:
#line 151 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onBody();
        }
#line 1980 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 21:
#line 158 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            // There is only the weight. No level and terms.
            director.getBuilder()->onWeightAtLevels(1,0,0); 
        }
#line 1989 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 22:
#line 163 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            // There are also a level and/or some terms.
            // The finalization has been postponed to "level_and_terms".
        }
#line 1998 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 23:
#line 171 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            // There is no terms following the level.
            director.getBuilder()->onWeightAtLevels(1,1,0); 
        }
#line 2007 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 24:
#line 176 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onWeightAtLevels(1,1,(yyvsp[0].integer)); 
        }
#line 2015 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 25:
#line 180 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            // The level is omitted.
            // Thus, the first term, recognized as the
            // weight, should be a term of this list.
            director.getBuilder()->onWeightAtLevels(0,0,(yyvsp[0].integer)+1); 
        }
#line 2026 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 26:
#line 190 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onHeadAtom(); 
        }
#line 2034 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 27:
#line 194 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onHeadAtom(); 
        }
#line 2042 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 28:
#line 198 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onHeadAtom(); 
        }
#line 2050 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 29:
#line 202 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onHeadAtom(); 
        }
#line 2058 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 30:
#line 209 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onBodyLiteral(); 
        }
#line 2066 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 31:
#line 213 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onBodyLiteral(); 
        }
#line 2074 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 36:
#line 227 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onChoiceLowerGuard();
        }
#line 2082 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 37:
#line 234 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onChoiceUpperGuard();
        }
#line 2090 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 40:
#line 246 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onChoiceElement(); 
        }
#line 2098 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 41:
#line 250 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onChoiceElement(); 
        }
#line 2106 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 42:
#line 257 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onChoiceElementAtom(); 
        }
#line 2114 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 43:
#line 264 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onChoiceElementLiteral(); 
        }
#line 2122 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 44:
#line 268 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onChoiceElementLiteral();
        }
#line 2130 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 45:
#line 275 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAggregateNafLiteral();
        }
#line 2138 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 46:
#line 279 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAggregateNafLiteral();
        }
#line 2146 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 47:
#line 286 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onNafLiteral();
        }
#line 2154 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 48:
#line 290 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onNafLiteral(true);
        }
#line 2162 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 49:
#line 294 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onNafLiteral();
        }
#line 2170 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 50:
#line 298 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
    		director.getBuilder()->onExtAtom();
    	}
#line 2178 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 51:
#line 302 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
			director.getBuilder()->onExtAtom(true);
		}
#line 2186 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 53:
#line 310 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAggregate();
        }
#line 2194 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 54:
#line 314 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAggregate(true);
        }
#line 2202 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 55:
#line 321 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onExistentialAtom(); 
        }
#line 2210 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 56:
#line 326 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { director.getBuilder()->onAtom(); }
#line 2216 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 57:
#line 327 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { director.getBuilder()->onAtom(true); }
#line 2222 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 58:
#line 332 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onPredicateName((yyvsp[0].string)); 
            delete[] (yyvsp[0].string);
        }
#line 2231 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 59:
#line 337 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onPredicateName((yyvsp[-3].string)); 
            delete[] (yyvsp[-3].string);
        }
#line 2240 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 60:
#line 342 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onPredicateName((yyvsp[-2].string)); 
            delete[] (yyvsp[-2].string);
        }
#line 2249 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 61:
#line 350 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onExternalPredicateName((yyvsp[-5].string));
            delete[] (yyvsp[-5].string);
        }
#line 2258 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 62:
#line 355 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onExternalPredicateName((yyvsp[-4].string));
            delete[] (yyvsp[-4].string);
        }
#line 2267 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 63:
#line 360 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onExternalPredicateName((yyvsp[-4].string));
            delete[] (yyvsp[-4].string);
        }
#line 2276 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 64:
#line 365 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
        	director.getBuilder()->onExternalPredicateName((yyvsp[-2].string));
        	delete[] (yyvsp[-2].string);
        }
#line 2285 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 65:
#line 373 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
	     	director.getBuilder()->onSemicolon();                  
        }
#line 2293 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 66:
#line 378 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { (yyval.integer) = 1; }
#line 2299 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 67:
#line 379 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { (yyval.integer) = (yyvsp[-2].integer) + 1; }
#line 2305 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 70:
#line 389 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onBuiltinAtom();
        }
#line 2313 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 71:
#line 396 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onEqualOperator();
        }
#line 2321 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 72:
#line 400 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onUnequalOperator(); 
        }
#line 2329 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 76:
#line 413 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { (yyval.single_char) = '+'; }
#line 2335 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 77:
#line 414 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { (yyval.single_char) = '-'; }
#line 2341 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 78:
#line 418 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { (yyval.single_char) = '*'; }
#line 2347 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 79:
#line 419 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { (yyval.single_char) = '/'; }
#line 2353 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 80:
#line 420 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { (yyval.single_char) = '\\'; }
#line 2359 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 81:
#line 425 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onUnknownVariable(); 
        }
#line 2367 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 82:
#line 429 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onFunction((yyvsp[-3].string), (yyvsp[-1].integer)); 
            delete[] (yyvsp[-3].string);
        }
#line 2376 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 83:
#line 434 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getParserConstraint()->rangeFacts();
            director.getBuilder()->onTermRange((yyvsp[-2].string), (yyvsp[0].string));
            delete[] (yyvsp[-2].string);
            delete[] (yyvsp[0].string);
        }
#line 2387 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 84:
#line 441 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onTermDash(); 
        }
#line 2395 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 86:
#line 450 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onTerm((yyvsp[0].string)); 
            delete[] (yyvsp[0].string);
        }
#line 2404 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 87:
#line 455 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onTerm((yyvsp[0].string)); 
            delete[] (yyvsp[0].string);
        }
#line 2413 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 88:
#line 460 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onTermParams(); 
        }
#line 2421 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 89:
#line 466 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {}
#line 2427 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 92:
#line 475 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onArithmeticOperation((yyvsp[-1].single_char)); 
        }
#line 2435 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 93:
#line 482 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {}
#line 2441 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 94:
#line 484 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onArithmeticOperation((yyvsp[-1].single_char)); 
        }
#line 2449 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 98:
#line 496 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAggregateGroundTerm((yyvsp[0].string));
            delete[] (yyvsp[0].string);
        }
#line 2458 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 99:
#line 501 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAggregateGroundTerm((yyvsp[0].string));
            delete[] (yyvsp[0].string);
        }
#line 2467 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 100:
#line 506 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAggregateGroundTerm((yyvsp[0].string));
            delete[] (yyvsp[0].string);
        }
#line 2476 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 101:
#line 511 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAggregateGroundTerm((yyvsp[0].string),true);
            delete[] (yyvsp[0].string);
        }
#line 2485 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 102:
#line 519 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onAggregateVariableTerm((yyvsp[0].string));
            delete[] (yyvsp[0].string);
        }
#line 2494 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 103:
#line 524 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAggregateUnknownVariable();
        }
#line 2502 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 104:
#line 531 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onAggregateFunctionalTerm((yyvsp[-3].string),(yyvsp[-1].integer)); 
            delete[] (yyvsp[-3].string);
       }
#line 2511 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 105:
#line 539 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
               director.getBuilder()->onListTerm(0);
       }
#line 2519 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 106:
#line 543 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
               director.getBuilder()->onListTerm((yyvsp[-1].integer));
       }
#line 2527 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 107:
#line 550 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
                (yyval.integer) = (yyvsp[0].integer);
        }
#line 2535 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 108:
#line 554 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
                (yyval.integer) = 2;
        }
#line 2543 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 109:
#line 561 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {

                director.getBuilder()->onHeadTailList();
            
        }
#line 2553 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 110:
#line 570 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
                director.getBuilder()->onTerm((yyvsp[0].string)); 
                delete[] (yyvsp[0].string);
        }
#line 2562 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 112:
#line 579 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onExistentialVariable((yyvsp[0].string)); 
            delete[] (yyvsp[0].string);
        }
#line 2571 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 113:
#line 584 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onExistentialVariable((yyvsp[0].string));
            delete[] (yyvsp[0].string);
        }
#line 2580 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 114:
#line 591 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { (yyval.string) = (yyvsp[0].string); }
#line 2586 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 115:
#line 592 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { (yyval.string) = (yyvsp[0].string); }
#line 2592 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 116:
#line 593 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { (yyval.string) = (yyvsp[0].string); }
#line 2598 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 117:
#line 598 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getParserConstraint()->directives();
            director.getBuilder()->onDirective((yyvsp[-1].string),(yyvsp[0].string));
            delete[] (yyvsp[-1].string);
            delete[] (yyvsp[0].string);
        }
#line 2609 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 118:
#line 608 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onAtom(); 
        }
#line 2617 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 119:
#line 615 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAggregateLowerGuard();
        }
#line 2625 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 120:
#line 622 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAggregateUpperGuard();
        }
#line 2633 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 123:
#line 634 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAggregateLowerGuard();
        }
#line 2641 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 127:
#line 650 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAggregateLowerGuard();
        }
#line 2649 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 129:
#line 661 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAggregateUpperGuard();
        }
#line 2657 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 130:
#line 668 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAggregateUpperGuard();
        }
#line 2665 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 138:
#line 688 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onLessOperator(); 
        }
#line 2673 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 139:
#line 692 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onLessOrEqualOperator(); 
        }
#line 2681 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 140:
#line 699 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onGreaterOperator();
        }
#line 2689 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 141:
#line 703 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onGreaterOrEqualOperator();
        }
#line 2697 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 146:
#line 721 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAggregateElement();
        }
#line 2705 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 147:
#line 728 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAggregateFunction((yyvsp[0].string));
            delete[] (yyvsp[0].string);
        }
#line 2714 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 148:
#line 733 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAggregateFunction((yyvsp[0].string));
            delete[] (yyvsp[0].string);
        }
#line 2723 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 149:
#line 738 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAggregateFunction((yyvsp[0].string));
            delete[] (yyvsp[0].string);
        }
#line 2732 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 150:
#line 743 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAggregateFunction((yyvsp[0].string));
            delete[] (yyvsp[0].string);
        }
#line 2741 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 169:
#line 778 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationRuleToDecompose();
	}
#line 2749 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 171:
#line 786 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationRuleToNotDecompose();
	}
#line 2757 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 176:
#line 803 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationGlobalExternal();
	}
#line 2765 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 177:
#line 813 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
	        director.getBuilder()->onAnnotationGlobalExternalPredicateName((yyvsp[0].string));
            delete[] (yyvsp[0].string);
	}
#line 2774 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 180:
#line 828 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationGlobalExternalTypeQC();
	}
#line 2782 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 181:
#line 832 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationGlobalExternalTypeC();
	}
#line 2790 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 182:
#line 836 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationGlobalExternalTypeU();
	}
#line 2798 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 183:
#line 840 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationGlobalExternalTypeUT();
	}
#line 2806 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 184:
#line 844 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationGlobalExternalTypeUR();
	}
#line 2814 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 185:
#line 848 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationGlobalExternalTypeT();
	}
#line 2822 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 186:
#line 852 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationGlobalExternalTypeR();
	}
#line 2830 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 187:
#line 858 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationRuleOrdering((yyvsp[0].string));
        delete[] (yyvsp[0].string);
	}
#line 2839 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 188:
#line 865 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationGlobalOrdering((yyvsp[0].string));
        delete[] (yyvsp[0].string);
	}
#line 2848 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 191:
#line 882 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationRuleAtomIndexedArgument((yyvsp[0].string));
        delete[] (yyvsp[0].string);
	}
#line 2857 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 192:
#line 886 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationRuleAtomIndexedArgument((yyvsp[0].string));
        delete[] (yyvsp[0].string);
	}
#line 2866 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 193:
#line 894 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onAnnotationRuleAtomIndexedLiteral();
        }
#line 2874 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 194:
#line 898 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onAnnotationRuleAtomIndexedLiteral(true);
        }
#line 2882 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 195:
#line 904 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationGlobalAtomIndexedArgument((yyvsp[0].string));
        delete[] (yyvsp[0].string);
	}
#line 2891 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 196:
#line 908 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationGlobalAtomIndexedArgument((yyvsp[0].string));
        delete[] (yyvsp[0].string);
	}
#line 2900 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 197:
#line 916 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onAnnotationGlobalAtomIndexedLiteral();
        }
#line 2908 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 198:
#line 920 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onAnnotationGlobalAtomIndexedLiteral(true);
        }
#line 2916 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 199:
#line 927 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onAnnotationRulePartialOrderingBefore();
        }
#line 2924 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 200:
#line 931 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onAnnotationRulePartialOrderingBefore(true);
        }
#line 2932 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 201:
#line 935 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAnnotationRulePartialOrderingBefore();
        }
#line 2940 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 202:
#line 939 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
           director.getBuilder()->onAnnotationAggregateRulePartialOrderingBefore();
        }
#line 2948 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 203:
#line 943 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAnnotationAggregateRulePartialOrderingBefore(true);
        }
#line 2956 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 204:
#line 947 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
        	director.getBuilder()->onExtAtom();
            director.getBuilder()->onAnnotationRulePartialOrderingBefore();
        }
#line 2965 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 205:
#line 952 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
			director.getBuilder()->onExtAtom(true);
		}
#line 2973 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 206:
#line 960 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onAnnotationGlobalPartialOrderingBefore();
        }
#line 2981 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 207:
#line 964 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onAnnotationGlobalPartialOrderingBefore(true);
        }
#line 2989 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 208:
#line 968 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAnnotationGlobalPartialOrderingBefore();
        }
#line 2997 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 209:
#line 972 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
           director.getBuilder()->onAnnotationAggregateGlobalPartialOrderingBefore();
        }
#line 3005 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 210:
#line 976 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAnnotationAggregateGlobalPartialOrderingBefore(true);
        }
#line 3013 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 211:
#line 980 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
        	director.getBuilder()->onExtAtom();
            director.getBuilder()->onAnnotationGlobalPartialOrderingBefore();
        }
#line 3022 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 212:
#line 985 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
			director.getBuilder()->onExtAtom(true);
		}
#line 3030 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 213:
#line 992 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onAnnotationRulePartialOrderingAfter();
        }
#line 3038 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 214:
#line 996 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onAnnotationRulePartialOrderingAfter(true);
        }
#line 3046 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 215:
#line 1000 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAnnotationRulePartialOrderingAfter();
        }
#line 3054 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 216:
#line 1004 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
           director.getBuilder()->onAnnotationAggregateRulePartialOrderingAfter();
        }
#line 3062 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 217:
#line 1008 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAnnotationAggregateRulePartialOrderingAfter(true);
        }
#line 3070 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 218:
#line 1012 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
			director.getBuilder()->onExtAtom();
            director.getBuilder()->onAnnotationRulePartialOrderingAfter();
        }
#line 3079 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 219:
#line 1017 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
			director.getBuilder()->onExtAtom(true);
		}
#line 3087 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 220:
#line 1024 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onAnnotationGlobalPartialOrderingAfter();
        }
#line 3095 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 221:
#line 1028 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    { 
            director.getBuilder()->onAnnotationGlobalPartialOrderingAfter(true);
        }
#line 3103 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 222:
#line 1032 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAnnotationGlobalPartialOrderingAfter();
        }
#line 3111 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 223:
#line 1036 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
           director.getBuilder()->onAnnotationAggregateGlobalPartialOrderingAfter();
        }
#line 3119 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 224:
#line 1040 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
            director.getBuilder()->onAnnotationAggregateGlobalPartialOrderingAfter(true);
        }
#line 3127 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 225:
#line 1044 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
        	director.getBuilder()->onExtAtom();
            director.getBuilder()->onAnnotationGlobalPartialOrderingAfter();
        }
#line 3136 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 226:
#line 1049 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
			director.getBuilder()->onExtAtom(true);
		}
#line 3144 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 235:
#line 1076 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationPartialOrdering();
	}
#line 3152 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 236:
#line 1082 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationPartialOrdering(true);
	}
#line 3160 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 242:
#line 1106 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
        		director.getBuilder()->onAnnotationRuleProjection((yyvsp[0].string));
                        delete[] (yyvsp[0].string);
        }
#line 3169 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 244:
#line 1114 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
                director.getBuilder()->onAnnotationRuleRewArith();
        }
#line 3177 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 246:
#line 1121 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
                director.getBuilder()->onAnnotationRuleAlignSubstitutions();
        }
#line 3185 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 248:
#line 1128 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
                director.getBuilder()->onAnnotationRuleLookAhead();
        }
#line 3193 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 249:
#line 1136 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationGlobalWaspPropagatorEnd();
	}
#line 3201 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 250:
#line 1141 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationGlobalWaspPropagatorEnd();
	}
#line 3209 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 251:
#line 1147 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationGlobalWaspPropagatorBegin();
	}
#line 3217 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 252:
#line 1155 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationGlobalWaspHeuristicsEnd();
	}
#line 3225 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 253:
#line 1160 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationGlobalWaspHeuristicsEnd();
	}
#line 3233 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 254:
#line 1166 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationGlobalWaspHeuristicsBegin();
	}
#line 3241 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 255:
#line 1174 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationGlobalWaspPyFile((yyvsp[0].string));
		delete[] (yyvsp[0].string);
	}
#line 3250 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 256:
#line 1182 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationGlobalWaspAggregateElement();
	}
#line 3258 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;

  case 257:
#line 1186 "src/idlv/input/aspcore2.y" /* yacc.c:1646  */
    {
		director.getBuilder()->onAnnotationGlobalWaspLiteral();
	}
#line 3266 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
    break;


#line 3270 "src/idlv/input/aspcore2_parser.hpp" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (director, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (director, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, director);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, director);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (director, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, director);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, director);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
