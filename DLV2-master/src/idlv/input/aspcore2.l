%{
/////////////////////////////////////////////////////////////////////////////
// scanner_asp_rfc.l

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

#include "aspcore2_parser.h"
#include "InputDirector.h"

#define YY_DECL int yylex(DLV2::InputDirector& director)
#define OPT_PARSER 1

#ifndef OPT_PARSER
#define YY_INPUT(buf,result,max_size)  {\
    result = GetNextChar(buf, max_size); \
    if (  result <= 0  ) \
      result = YY_NULL; \
    }
#endif

%}


%option nounput

BLANK               ([ \t]+)
NEWLINE             \15?\n
DOT 				\.

ANNOTATION_RULE_ALIGN_SUBSTITUTIONS     \%@rule_align_substitutions
ANNOTATION_RULE_LOOK_AHEAD              \%@rule_look_ahead
ANNOTATION_RULE_PROJECTION              \%@rule_projection
ANNOTATION_RULE_REWRITING_ARITH			\%@rule_rewriting_arith
ANNOTATION_RULE_ORDERING			\%@rule_ordering
ANNOTATION_ORDERING_VALUE			@value

ANNOTATION_RULE_ATOM_INDEXED		\%@rule_atom_indexed
ANNOTATION_ATOM_INDEXED_ATOM		@atom
ANNOTATION_ATOM_INDEXED_ARGUMENTS	@arguments

ANNOTATION_RULE_PARTIAL_ORDER		\%@rule_partial_order
ANNOTATION_PARTIAL_ORDER_BEFORE		@before
ANNOTATION_PARTIAL_ORDER_AFTER		@after

ANNOTATION_EXTATOM_PREDICATE	@predicate
ANNOTATION_EXTATOM_TYPE		@type
ANNOTATION_EXTATOM_TYPE_QCONST	@Q_CONST
ANNOTATION_EXTATOM_TYPE_CONST	@CONST
ANNOTATION_EXTATOM_TYPE_U_INT	@U_INT
ANNOTATION_EXTATOM_TYPE_UR_INT	@UR_INT
ANNOTATION_EXTATOM_TYPE_UT_INT	@UT_INT
ANNOTATION_EXTATOM_TYPE_R_INT	@R_INT
ANNOTATION_EXTATOM_TYPE_T_INT	@T_INT

ANNOTATION_GLOBAL_ORDERING			\%@global_ordering
ANNOTATION_GLOBAL_ATOM_INDEXED		\%@global_atom_indexed
ANNOTATION_GLOBAL_PARTIAL_ORDER		\%@global_partial_order
ANNOTATION_GLOBAL_EXTATOM_CONVERSION	\%@global_external_predicate_conversion

ANNOTATION_RULE_TO_DECOMPOSE			\%@rule_to_decompose
ANNOTATION_RULE_TO_NOT_DECOMPOSE		\%@rule_to_not_decompose

ANNOTATION_GLOBAL_WASP_HEURISTIC 		\%@global_heuristic
ANNOTATION_GLOBAL_WASP_HEURISTIC_FILE	@file 
ANNOTATION_GLOBAL_WASP_HEURISTIC_ELEMENTS	@elements

COMMENT_EMPTY       \%\n
COMMENT             \%[^@\n].*

AGGR_COUNT          #count
AGGR_MAX            #max
AGGR_MIN            #min
AGGR_SUM            #sum
ANNOTATION_GLOBAL_WASP_PROPAGATOR 		#propagator
DIRECTIVE_NAME      #([A-Za-z_]*)
DIRECTIVE_VALUE     .*

SYMBOLIC_CONSTANT   ([a-z][A-Za-z_0-9]*)
VARIABLE            ([A-Z][A-Za-z_0-9]*)
STRING              (\"[^\"]*\")
NUMBER              ([0-9]+)

%x ANNOTATION

%x DIRECTIVE_NAME_READ    
                      // Only in state DIRECTIVE_NAME_READ a directive value 
                      // token could be generated, while normally that token 
                      // is not considered because it is too generic and, 
                      // consequently, it might be matched anywhere.

%START WANT_NEWLINE
                      // Only in state WANT_NEWLINE a newline
                      // token for the parser is generated, while
                      // normally newlines are simply skipped.
%%
        // Newline is normally skipped, except when explicitly
        // requested with WANT_NEWLINE. (e.g. by a construct that is
        // terminated by a newline.)

<WANT_NEWLINE,DIRECTIVE_NAME_READ>{NEWLINE} {
                            director.onNewLine();
                            BEGIN(INITIAL);
                            return NEWLINE;
}

{NEWLINE}               { director.onNewLine(); }
"."                     { return DOT; }
".."                    { return DDOT; }
","                     { return COMMA; }
"|"                     { return VEL; }
";"                     { return SEMICOLON; }
":"                     { return COLON; }
"@"                     { return AT; }
"\\E"                   { return EXISTS; }

"not"                   { return NAF; }

":-"                    { return CONS; }
":~"                    { return WCONS; }

"+"                     { return PLUS; }
"-"                     { return DASH; }
"*"                     { return TIMES; }
"/"                     { return SLASH; }
"\\"                   	{ return BACK_SLASH; }

"("                     { return PARAM_OPEN; }
")"                     { return PARAM_CLOSE; }

"["                     { return SQUARE_OPEN; }
"]"                     { return SQUARE_CLOSE; }

"{"                     { return CURLY_OPEN; }
"}"                     { return CURLY_CLOSE; }

"?"                     { return QUERY_MARK; }

"_"                     { return ANON_VAR; }

"="     | 
"=="          			{ return EQUAL; }
"<>"    |
"!="                    { return UNEQUAL; }
"<"                     { return LESS; }
">"                     { return GREATER; }
"<="                    { return LESS_OR_EQ; }
">="                    { return GREATER_OR_EQ; }
"&"                     { return AMPERSAND; }

{AGGR_COUNT}            { 
                            yylval.string=new char[strlen(yytext)+1];
                            strcpy(yylval.string,yytext);
                            return AGGR_COUNT;
                        }

{AGGR_MAX}              { 
                            yylval.string=new char[strlen(yytext)+1];
                            strcpy(yylval.string,yytext);
                            return AGGR_MAX;
                        }

{AGGR_MIN}              { 
                            yylval.string=new char[strlen(yytext)+1];
                            strcpy(yylval.string,yytext);
                            return AGGR_MIN;
                        }

{AGGR_SUM}              { 
                            yylval.string=new char[strlen(yytext)+1];
                            strcpy(yylval.string,yytext);
                            return AGGR_SUM;
                        }
{ANNOTATION_GLOBAL_WASP_PROPAGATOR} {
									return ANNOTATION_GLOBAL_WASP_PROPAGATOR;
}                     

{DIRECTIVE_NAME}        {
                            BEGIN(DIRECTIVE_NAME_READ);
                            yylval.string=new char[strlen(yytext)+1];
                            strcpy(yylval.string,yytext);
                            return DIRECTIVE_NAME;
                        }

<DIRECTIVE_NAME_READ>{DIRECTIVE_VALUE} {
                            BEGIN(INITIAL);
                            yylval.string=new char[strlen(yytext)+1];
                            strcpy(yylval.string,yytext);
                            return DIRECTIVE_VALUE;
                        }

{SYMBOLIC_CONSTANT}     {
                            yylval.string=new char[strlen(yytext)+1];
                            strcpy(yylval.string,yytext);
                            return SYMBOLIC_CONSTANT;
                        }

{VARIABLE}				{
                            yylval.string=new char[strlen(yytext)+1];
                            strcpy(yylval.string,yytext);
                            return VARIABLE;
                        }

{STRING}                {
                            yylval.string=new char[strlen(yytext)+1];
                            strcpy(yylval.string,yytext);
                            return STRING;
                        }

{NUMBER}                { 
                            yylval.string=new char[strlen(yytext)+1];
                            strcpy(yylval.string,yytext);
                            return NUMBER; 
                        }
                       
{ANNOTATION_RULE_ORDERING}	{
								return ANNOTATION_RULE_ORDERING;
							}
							
{ANNOTATION_ORDERING_VALUE}	{
								return ANNOTATION_ORDERING_VALUE;
							}							
														
{ANNOTATION_RULE_ATOM_INDEXED}	{
									return ANNOTATION_RULE_ATOM_INDEXED;
								}

{ANNOTATION_ATOM_INDEXED_ATOM}	{
								return ANNOTATION_ATOM_INDEXED_ATOM;
							}
							
{ANNOTATION_ATOM_INDEXED_ARGUMENTS}	{
								return ANNOTATION_ATOM_INDEXED_ARGUMENTS;
							}
							
{ANNOTATION_RULE_PARTIAL_ORDER}	{
									return ANNOTATION_RULE_PARTIAL_ORDER;
								}
								
{ANNOTATION_PARTIAL_ORDER_BEFORE}	{
									return ANNOTATION_PARTIAL_ORDER_BEFORE;
								}
								
{ANNOTATION_PARTIAL_ORDER_AFTER}	{
									return ANNOTATION_PARTIAL_ORDER_AFTER;
								}	
															
{ANNOTATION_GLOBAL_ORDERING}	{
									return ANNOTATION_GLOBAL_ORDERING;
								}

{ANNOTATION_GLOBAL_ATOM_INDEXED} {
									return ANNOTATION_GLOBAL_ATOM_INDEXED;
								  }

{ANNOTATION_GLOBAL_PARTIAL_ORDER} {
									return ANNOTATION_GLOBAL_PARTIAL_ORDER;
								  }
								  

{ANNOTATION_GLOBAL_EXTATOM_CONVERSION} {
									return ANNOTATION_GLOBAL_EXTATOM_CONVERSION;
								  }

{ANNOTATION_EXTATOM_PREDICATE} {
									return ANNOTATION_EXTATOM_PREDICATE;
								  }
{ANNOTATION_EXTATOM_TYPE} {
									return ANNOTATION_EXTATOM_TYPE;
								  }

{ANNOTATION_EXTATOM_TYPE_QCONST} {
									return ANNOTATION_EXTATOM_TYPE_QCONST;
								  }
{ANNOTATION_EXTATOM_TYPE_CONST} {
									return ANNOTATION_EXTATOM_TYPE_CONST;
								  }								 								  
{ANNOTATION_EXTATOM_TYPE_U_INT} {
									return ANNOTATION_EXTATOM_TYPE_U_INT;
								  }		
{ANNOTATION_EXTATOM_TYPE_UR_INT} {
									return ANNOTATION_EXTATOM_TYPE_UR_INT;
								  }		
{ANNOTATION_EXTATOM_TYPE_UT_INT} {
									return ANNOTATION_EXTATOM_TYPE_UT_INT;
								  }										  								 
{ANNOTATION_EXTATOM_TYPE_R_INT} {
									return ANNOTATION_EXTATOM_TYPE_R_INT;
								  }		
{ANNOTATION_EXTATOM_TYPE_T_INT} {
									return ANNOTATION_EXTATOM_TYPE_T_INT;
								  }		

{ANNOTATION_RULE_PROJECTION} {
									return ANNOTATION_RULE_PROJECTION;
								  }		

{ANNOTATION_RULE_REWRITING_ARITH} {
									return ANNOTATION_RULE_REWRITING_ARITH;
								  }		
{ANNOTATION_RULE_ALIGN_SUBSTITUTIONS} {
										return ANNOTATION_RULE_ALIGN_SUBSTITUTIONS;
								  	}		

{ANNOTATION_RULE_LOOK_AHEAD} {
								return ANNOTATION_RULE_LOOK_AHEAD;
							 }
{ANNOTATION_GLOBAL_WASP_HEURISTIC_FILE} {
									return ANNOTATION_GLOBAL_WASP_HEURISTIC_FILE;
							}
{ANNOTATION_GLOBAL_WASP_HEURISTIC_ELEMENTS} {
									return ANNOTATION_GLOBAL_WASP_HEURISTIC_ELEMENTS;
							}
{ANNOTATION_GLOBAL_WASP_HEURISTIC} {
									return ANNOTATION_GLOBAL_WASP_HEURISTIC;
}
{ANNOTATION_RULE_TO_DECOMPOSE}   {
                                    return ANNOTATION_RULE_TO_DECOMPOSE;
                                }
{ANNOTATION_RULE_TO_NOT_DECOMPOSE}   {
                                    return ANNOTATION_RULE_TO_NOT_DECOMPOSE;
                                }

								  
{COMMENT_EMPTY}			{ /* Skip comments */}
{COMMENT}				{ /* Skip comments */ }
{BLANK}					{ /* Skip blanks */ }

.                       {   
                            // Anything not matching one of the patterns
                            // given above is considered an error.
                            char s[1024];
                            assert(yytext[1]==0);
                            sprintf(s,"Lexical syntax error (%s)",yytext); 
                            yyerror(director,s);

                            return ERROR;
                        }
                        
                        
                        
                        
