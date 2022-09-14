grammar ASPCore2;

program: 					rule*;
     	

	
rule: 						simple_rule
						| annotations_rule simple_rule
						| annotation_global
						| directive;
     		
simple_rule:					head DOT
     						| head CONS DOT
     						| head CONS body DOT
     						| CONS body DOT
     						| WCONS body DOT weight_at_levels
     						| query;
     						
		
head:						disjunction
						| choice_atom;
							
body:						conjunction;

weight_at_levels:				SQUARE_OPEN term SQUARE_CLOSED
						| SQUARE_OPEN term levels_and_terms SQUARE_CLOSED;

levels_and_terms:				AT term
						| AT term COMMA terms
						| COMMA terms;

disjunction: 					classic_literal
						| classic_literal HEAD_SEPARATOR disjunction
						| existential_atom
						| existential_atom HEAD_SEPARATOR disjunction;
	
conjunction:					naf_literal_aggregate
						| naf_literal_aggregate COMMA conjunction;
	
choice_atom:					lower_guard CURLY_OPEN choice_elements CURLY_CLOSE upper_guard 
    						| lower_guard CURLY_OPEN choice_elements CURLY_CLOSE 
    						| CURLY_OPEN choice_elements CURLY_CLOSE 
    						| CURLY_OPEN choice_elements CURLY_CLOSE upper_guard;

lower_guard: 					term binop;

upper_guard:					binop term;



choice_elements:				choice_element SEMICOLON choice_elements
    						| choice_element;

choice_element:				choice_element_atom
						| choice_element_atom COLON choice_elements_literals;

choice_element_atom: 				atom;

choice_elements_literals:			naf_literal
						| naf_literals COMMA naf_literal;

naf_literals:					naf_literal
						| naf_literal COMMA naf_literals;

naf_literal:					classic_literal
						| NAF classic_literal
						| builtin_atom
						| extAtom
						| NAF extAtom;

naf_literal_aggregate:				naf_literal
						| aggregate_atom
						| NAF aggregate_atom;

existential_atom:				EXISTS vars atom;

classic_literal:				atom
						| DASH atom;

atom:						identifier
						| identifier PARAM_OPEN terms PARAM_CLOSE
						| identifier PARAM_OPEN PARAM_CLOSE;
						
extAtom:					AMPERSAND identifier PARAM_OPEN terms extSemicol terms PARAM_CLOSE
						| AMPERSAND identifier PARAM_OPEN extSemicol terms PARAM_CLOSE
						| AMPERSAND identifier PARAM_OPEN terms extSemicol PARAM_CLOSE
						| AMPERSAND identifier PARAM_OPEN PARAM_CLOSE;

extSemicol:					SEMICOLON;

terms:						term
						| term COMMA terms;
						
basic_terms:					basic_term
						| basic_term COMMA basic_terms;

builtin_atom:					term binop term;

compareop:					EQUAL
						| UNEQUAL;

binop:						compareop
						| compareop
						| leftwardop
						| rightwardop;

arithop1: 					PLUS
						| DASH;

arithop2:					TIMES
						| SLASH
						| BACK_SLASH;

term_:						ANON_VAR
						| identifier PARAM_OPEN terms PARAM_CLOSE
						| NUMBER DDOT NUMBER
						| DASH term;

term__:					identifier
						| NUMBER
						| PARAM_OPEN term PARAM_CLOSE;
			
term:						term_
						| identifier
						| expr;

expr:						factor
						| factor arithop1 expr;

factor:					term__
						| term__ arithop2 factor;

basic_term:					ground_term
						| variable_term
						| functional_term;
	
ground_term:					SYMBOLIC_CONSTANT
						| STRING
						| NUMBER
						| DASH NUMBER;
	
variable_term:					VARIABLE
						| ANON_VAR;
			
functional_term:				identifier PARAM_OPEN terms PARAM_CLOSE;

vars:						VARIABLE
						| VARIABLE COMMA vars;

identifier:					SYMBOLIC_CONSTANT
						| STRING
						| VARIABLE;
						
directive:					DIRECTIVE_NAME DIRECTIVE_VALUE;						

query: 					atom QUERY_MARK;

lower_guard_compare_aggregate:		term compareop;

upper_guard_compare_aggregate:		compareop term;

compare_aggregate:				lower_guard_compare_aggregate aggregate
						|aggregate upper_guard_compare_aggregate;

lower_guard_leftward_left_aggregate:		term leftwardop;

leftward_left_aggregate:			lower_guard_leftward_left_aggregate aggregate;
	
left_aggregate:				leftward_left_aggregate
						| rightward_left_aggregate;

lower_guard_rightward_left_aggregate:		term rightwardop;

rightward_left_aggregate:			lower_guard_rightward_left_aggregate aggregate;

upper_guard_leftward_right_aggregate:		leftwardop term;
	
upper_guard_rightward_right_aggregate:	rightwardop term;

right_aggregate:				aggregate upper_guard_leftward_right_aggregate
						| aggregate upper_guard_rightward_right_aggregate;

aggregate_atom:				left_aggregate
						| right_aggregate
						| compare_aggregate
						| leftward_left_aggregate upper_guard_leftward_right_aggregate
						| rightward_left_aggregate upper_guard_rightward_right_aggregate;

leftwardop:					LESS
						| LESS_OR_EQ;
					
rightwardop:					GREATER
						| GREATER_OR_EQ;

aggregate:					aggregate_function CURLY_OPEN aggregate_elements CURLY_CLOSE
						|aggregate_function CURLY_OPEN CURLY_CLOSE;
					
aggregate_elements:				aggregate_element SEMICOLON aggregate_elements
						| aggregate_element;

aggregate_element:				basic_terms COLON naf_literals;

aggregate_function:				AGGR_COUNT
						| AGGR_MAX
						| AGGR_MIN
						| AGGR_SUM;

annotation_global:				 annotation_global_ordering 
						| annotation_global_atom_indexed
						| annotation_global_partial_order
						| annotation_global_extatom_conversion
						| annotation_global_wasp_heuristic
						| annotation_global_wasp_propagator;

annotations_rule: 				annotation_rule
						| annotation_rule annotations_rule;
						
annotation_rule: 				annotation_rule_ordering 
						| annotation_rule_atom_indexed
						| annotation_rule_partial_order
    						| annotation_rule_projection
    						| annotation_rule_rewriting_arith
						| annotation_rule_align_substitutions
						| annotation_rule_look_ahead
						| annotation_rule_to_decompose
						| annotation_rule_to_not_decompose;						

annotation_rule_to_decompose:			ANNOTATION_RULE_TO_DECOMPOSE PARAM_OPEN PARAM_CLOSE DOT 							| ANNOTATION_RULE_TO_DECOMPOSE DOT;

annotation_rule_to_not_decompose:		ANNOTATION_RULE_TO_NOT_DECOMPOSE PARAM_OPEN PARAM_CLOSE DOT
						| ANNOTATION_RULE_TO_NOT_DECOMPOSE DOT;

annotation_rule_ordering: 			ANNOTATION_RULE_ORDERING PARAM_OPEN ANNOTATION_ORDERING_VALUE EQUAL rule_ordering_type PARAM_CLOSE DOT
						| ANNOTATION_RULE_ORDERING PARAM_OPEN rule_ordering_type PARAM_CLOSE DOT;


annotation_global_ordering: 			ANNOTATION_GLOBAL_ORDERING PARAM_OPEN ANNOTATION_ORDERING_VALUE EQUAL global_ordering_type PARAM_CLOSE DOT
						| ANNOTATION_GLOBAL_ORDERING PARAM_OPEN global_ordering_type PARAM_CLOSE DOT;

annotation_global_extatom_conversion: 	ANNOTATION_GLOBAL_EXTATOM_CONVERSION PARAM_OPEN ANNOTATION_EXTATOM_PREDICATE EQUAL annotation_extpredicate COMMA ANNOTATION_EXTATOM_TYPE EQUAL extatom_conv_types PARAM_CLOSE DOT;

annotation_extpredicate: 			AMPERSAND identifier;

extatom_conv_types:				extatom_conv_type
						| extatom_conv_type COMMA extatom_conv_types;

extatom_conv_type:				ANNOTATION_EXTATOM_TYPE_QCONST
						| ANNOTATION_EXTATOM_TYPE_CONST
						| ANNOTATION_EXTATOM_TYPE_U_INT
						| ANNOTATION_EXTATOM_TYPE_UT_INT
						| ANNOTATION_EXTATOM_TYPE_UR_INT
						| ANNOTATION_EXTATOM_TYPE_T_INT
						| ANNOTATION_EXTATOM_TYPE_R_INT;
					
rule_ordering_type:  				NUMBER;


global_ordering_type:  			NUMBER; 

annotation_rule_atom_indexed: 		ANNOTATION_RULE_ATOM_INDEXED PARAM_OPEN ANNOTATION_ATOM_INDEXED_ATOM EQUAL rule_naf_literal_annotation COMMA ANNOTATION_ATOM_INDEXED_ARGUMENTS EQUAL CURLY_OPEN rule_indexing_arguments CURLY_CLOSE PARAM_CLOSE DOT;

annotation_global_atom_indexed: 		ANNOTATION_GLOBAL_ATOM_INDEXED PARAM_OPEN ANNOTATION_ATOM_INDEXED_ATOM EQUAL global_naf_literal_annotation COMMA ANNOTATION_ATOM_INDEXED_ARGUMENTS EQUAL CURLY_OPEN global_indexing_arguments CURLY_CLOSE PARAM_CLOSE DOT;

rule_indexing_arguments:  			NUMBER
						| NUMBER COMMA rule_indexing_arguments;

rule_naf_literal_annotation: 			classic_literal
						| NAF classic_literal;
						
global_indexing_arguments: 			NUMBER
						| NUMBER COMMA global_indexing_arguments;
						
global_naf_literal_annotation: 		classic_literal
						| NAF classic_literal;
						
rule_atom_annotation_before:			classic_literal
						| NAF classic_literal
						| builtin_atom
						| aggregate_atom
						| NAF aggregate_atom
						| extAtom
						| NAF extAtom;
						
global_atom_annotation_before: 		classic_literal
						| NAF classic_literal
						| builtin_atom
						| aggregate_atom
						| NAF aggregate_atom
						| extAtom
						| NAF extAtom;
						
rule_atom_annotation_after:			classic_literal
						| NAF classic_literal
						| builtin_atom
						| aggregate_atom
						| NAF aggregate_atom
						| extAtom
						| NAF extAtom;
						
global_atom_annotation_after:			classic_literal
						| NAF classic_literal
						| builtin_atom
						| aggregate_atom
						| NAF aggregate_atom
						| extAtom
						| NAF extAtom;
						
rule_atoms_annotation_before: 		rule_atom_annotation_before
						| rule_atom_annotation_before COMMA rule_atoms_annotation_before;

global_atoms_annotation_before: 		global_atom_annotation_before
						| global_atom_annotation_before COMMA global_atoms_annotation_before;

rule_atoms_annotation_after: 			rule_atom_annotation_after
						| rule_atom_annotation_after COMMA rule_atoms_annotation_after;

global_atoms_annotation_after: 		global_atom_annotation_after
						| global_atom_annotation_after COMMA global_atoms_annotation_after global_atoms_annotation_after;
annotation_rule_partial_order_begin: 		ANNOTATION_RULE_PARTIAL_ORDER;

annotation_global_partial_order_begin: 	ANNOTATION_GLOBAL_PARTIAL_ORDER;


annotation_rule_partial_order: 		annotation_rule_partial_order_begin PARAM_OPEN ANNOTATION_PARTIAL_ORDER_BEFORE EQUAL CURLY_OPEN rule_atoms_annotation_before CURLY_CLOSE COMMA ANNOTATION_PARTIAL_ORDER_AFTER EQUAL CURLY_OPEN rule_atoms_annotation_after CURLY_CLOSE PARAM_CLOSE DOT
						| annotation_rule_partial_order_begin PARAM_OPEN ANNOTATION_PARTIAL_ORDER_AFTER EQUAL CURLY_OPEN rule_atoms_annotation_after CURLY_CLOSE COMMA ANNOTATION_PARTIAL_ORDER_BEFORE EQUAL CURLY_OPEN rule_atoms_annotation_before CURLY_CLOSE PARAM_CLOSE DOT;

annotation_global_partial_order: 		annotation_global_partial_order_begin PARAM_OPEN ANNOTATION_PARTIAL_ORDER_BEFORE EQUAL CURLY_OPEN global_atoms_annotation_before CURLY_CLOSE COMMA ANNOTATION_PARTIAL_ORDER_AFTER EQUAL CURLY_OPEN global_atoms_annotation_after CURLY_CLOSE PARAM_CLOSE DOT
						| annotation_global_partial_order_begin PARAM_OPEN ANNOTATION_PARTIAL_ORDER_AFTER EQUAL CURLY_OPEN global_atoms_annotation_after CURLY_CLOSE COMMA ANNOTATION_PARTIAL_ORDER_BEFORE EQUAL CURLY_OPEN global_atoms_annotation_before CURLY_CLOSE PARAM_CLOSE DOT;

annotation_rule_projection: 			ANNOTATION_RULE_PROJECTION PARAM_OPEN ANNOTATION_ORDERING_VALUE EQUAL annotation_rule_projection_value PARAM_CLOSE DOT;

annotation_rule_projection_value: 		NUMBER;

annotation_rule_rewriting_arith: 		ANNOTATION_RULE_REWRITING_ARITH PARAM_OPEN  PARAM_CLOSE DOT
						| ANNOTATION_RULE_REWRITING_ARITH DOT;
						
annotation_rule_align_substitutions: 		ANNOTATION_RULE_ALIGN_SUBSTITUTIONS PARAM_OPEN PARAM_CLOSE DOT
						| ANNOTATION_RULE_ALIGN_SUBSTITUTIONS DOT;
						
annotation_rule_look_ahead: 			ANNOTATION_RULE_LOOK_AHEAD PARAM_OPEN PARAM_CLOSE DOT
						| ANNOTATION_RULE_LOOK_AHEAD DOT;
												
annotation_global_wasp_propagator: 		annotation_global_wasp_propagator_begin PARAM_OPEN ANNOTATION_GLOBAL_WASP_HEURISTIC_FILE EQUAL heuristic_python_file COMMA ANNOTATION_GLOBAL_WASP_HEURISTIC_ELEMENTS EQUAL CURLY_OPEN aggregate_elements_heuristic CURLY_CLOSE PARAM_CLOSE DOT
						| annotation_global_wasp_propagator_begin PARAM_OPEN ANNOTATION_GLOBAL_WASP_HEURISTIC_ELEMENTS EQUAL CURLY_OPEN aggregate_elements_heuristic CURLY_CLOSE COMMA ANNOTATION_GLOBAL_WASP_HEURISTIC_FILE EQUAL heuristic_python_file PARAM_CLOSE DOT;
						
annotation_global_wasp_propagator_begin: 	ANNOTATION_GLOBAL_WASP_PROPAGATOR;

annotation_global_wasp_heuristic: 		annotation_global_wasp_heuristic_begin PARAM_OPEN ANNOTATION_GLOBAL_WASP_HEURISTIC_FILE EQUAL heuristic_python_file COMMA ANNOTATION_GLOBAL_WASP_HEURISTIC_ELEMENTS EQUAL CURLY_OPEN aggregate_elements_heuristic CURLY_CLOSE PARAM_CLOSE DOT
						| annotation_global_wasp_heuristic_begin PARAM_OPEN ANNOTATION_GLOBAL_WASP_HEURISTIC_ELEMENTS EQUAL CURLY_OPEN aggregate_elements_heuristic CURLY_CLOSE COMMA ANNOTATION_GLOBAL_WASP_HEURISTIC_FILE EQUAL heuristic_python_file PARAM_CLOSE DOT;

annotation_global_wasp_heuristic_begin: 	ANNOTATION_GLOBAL_WASP_HEURISTIC;

heuristic_python_file:				STRING;

aggregate_element_heuristic:  		aggregate_element
						| classic_literal;
						
aggregate_elements_heuristic: 		aggregate_element_heuristic
						| aggregate_element_heuristic SEMICOLON aggregate_elements_heuristic;
			
NAF:						'not' ;




SYMBOLIC_CONSTANT:   				[a-z][A-Za-z_0-9]* ;
VARIABLE:           				[A-Z][A-Za-z_0-9]* ;
STRING:					'"' ('\\"' | . )*? '"' ;
NUMBER:					[0-9]+ ;

WS:			 			[ \t\n]+ ->skip ;


DOT: 						'.' ;
DDOT: 						'..' ;
COMMA:						',' ;
HEAD_SEPARATOR:				'|' ;
SEMICOLON:					';' ;
COLON: 					':' ;
AT: 						'@' ;
CONS:						':-' ;
WCONS: 					':~' ;
PLUS:						'+' ;
DASH:						'-' ;
TIMES: 					'*' ;
SLASH: 					'/' ;
BACK_SLASH:					'\\' ;
PARAM_OPEN:					'(' ;
PARAM_CLOSE:					')' ;
SQUARE_OPEN: 					'[' ;
SQUARE_CLOSED:					']' ;
CURLY_OPEN: 					'{' ;
CURLY_CLOSE:					'}' ;
QUERY_MARK:					'?' ;	
ANON_VAR:					'_' ;
EQUAL:						'=' | '==' ;
UNEQUAL: 					'<>' | '!=' ;
LESS: 						'<' ;
GREATER: 					'>' ;
LESS_OR_EQ:					'<=' ;
GREATER_OR_EQ:			 		'>=' ;
AMPERSAND: 					'&' ;	
EXISTS:					'\\E' ;


AGGR_COUNT:          				'#count' ;
AGGR_MAX:            				'#max' ;
AGGR_MIN:            				'#min' ;
AGGR_SUM:            				'#sum' ;
ANNOTATION_GLOBAL_WASP_PROPAGATOR: 		'#propagator' ;
DIRECTIVE_NAME:      				'#([A-Za-z_]*)' ;
DIRECTIVE_VALUE:     				'.*' ;


ANNOTATION_RULE_ALIGN_SUBSTITUTIONS:   	'%@rule_align_substitutions' ;
ANNOTATION_RULE_LOOK_AHEAD:              	'%@rule_look_ahead' ;
ANNOTATION_RULE_PROJECTION:              	'%@rule_projection' ;
ANNOTATION_RULE_REWRITING_ARITH:		'%@rule_rewriting_arith' ;
ANNOTATION_RULE_ORDERING:			'%@rule_ordering' ;
ANNOTATION_ORDERING_VALUE:			'@value' ;

ANNOTATION_RULE_ATOM_INDEXED:			'%@rule_atom_indexed' ;
ANNOTATION_ATOM_INDEXED_ATOM:			'@atom' ;
ANNOTATION_ATOM_INDEXED_ARGUMENTS:		'@arguments' ;

ANNOTATION_RULE_PARTIAL_ORDER:		'%@rule_partial_order' ;
ANNOTATION_PARTIAL_ORDER_BEFORE:		'@before' ;
ANNOTATION_PARTIAL_ORDER_AFTER:		'@after' ;

ANNOTATION_EXTATOM_PREDICATE:			'@predicate' ;
ANNOTATION_EXTATOM_TYPE:			'@type' ;
ANNOTATION_EXTATOM_TYPE_QCONST:		'@Q_CONST' ;
ANNOTATION_EXTATOM_TYPE_CONST:		'@CONST' ;
ANNOTATION_EXTATOM_TYPE_U_INT:		'@U_INT' ;
ANNOTATION_EXTATOM_TYPE_UR_INT:		'@UR_INT' ;
ANNOTATION_EXTATOM_TYPE_UT_INT:		'@UT_INT' ;
ANNOTATION_EXTATOM_TYPE_R_INT:		'@R_INT' ;
ANNOTATION_EXTATOM_TYPE_T_INT:		'@T_INT' ;

ANNOTATION_GLOBAL_ORDERING:			'%@global_ordering' ;
ANNOTATION_GLOBAL_ATOM_INDEXED:		'%@global_atom_indexed' ;
ANNOTATION_GLOBAL_PARTIAL_ORDER:		'%@global_partial_order' ;
ANNOTATION_GLOBAL_EXTATOM_CONVERSION:		'%@global_external_predicate_conversion' ;

ANNOTATION_RULE_TO_DECOMPOSE:			'%@rule_to_decompose' ;
ANNOTATION_RULE_TO_NOT_DECOMPOSE:		'%@rule_to_not_decompose' ;

ANNOTATION_GLOBAL_WASP_HEURISTIC: 		'%@global_heuristic' ;
ANNOTATION_GLOBAL_WASP_HEURISTIC_FILE:	'@file'  ;
ANNOTATION_GLOBAL_WASP_HEURISTIC_ELEMENTS:	'@elements' ;

EMPTY_COMMENT:					'%''\n' ->skip;
COMMENT: 					'%'~('@' | '\n')(.)*?'\n' ->skip;










