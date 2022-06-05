
// Generated from ASPCore2.g4 by ANTLR 4.10.1

#pragma once


#include "antlr4-runtime.h"
#include "ASPCore2Parser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by ASPCore2Parser.
 */
class  ASPCore2Listener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterProgram(ASPCore2Parser::ProgramContext *ctx) = 0;
  virtual void exitProgram(ASPCore2Parser::ProgramContext *ctx) = 0;

  virtual void enterRule(ASPCore2Parser::RuleContext *ctx) = 0;
  virtual void exitRule(ASPCore2Parser::RuleContext *ctx) = 0;

  virtual void enterSimple_rule(ASPCore2Parser::Simple_ruleContext *ctx) = 0;
  virtual void exitSimple_rule(ASPCore2Parser::Simple_ruleContext *ctx) = 0;

  virtual void enterHead(ASPCore2Parser::HeadContext *ctx) = 0;
  virtual void exitHead(ASPCore2Parser::HeadContext *ctx) = 0;

  virtual void enterBody(ASPCore2Parser::BodyContext *ctx) = 0;
  virtual void exitBody(ASPCore2Parser::BodyContext *ctx) = 0;

  virtual void enterWeight_at_levels(ASPCore2Parser::Weight_at_levelsContext *ctx) = 0;
  virtual void exitWeight_at_levels(ASPCore2Parser::Weight_at_levelsContext *ctx) = 0;

  virtual void enterLevels_and_terms(ASPCore2Parser::Levels_and_termsContext *ctx) = 0;
  virtual void exitLevels_and_terms(ASPCore2Parser::Levels_and_termsContext *ctx) = 0;

  virtual void enterDisjunction(ASPCore2Parser::DisjunctionContext *ctx) = 0;
  virtual void exitDisjunction(ASPCore2Parser::DisjunctionContext *ctx) = 0;

  virtual void enterConjunction(ASPCore2Parser::ConjunctionContext *ctx) = 0;
  virtual void exitConjunction(ASPCore2Parser::ConjunctionContext *ctx) = 0;

  virtual void enterChoice_atom(ASPCore2Parser::Choice_atomContext *ctx) = 0;
  virtual void exitChoice_atom(ASPCore2Parser::Choice_atomContext *ctx) = 0;

  virtual void enterLower_guard(ASPCore2Parser::Lower_guardContext *ctx) = 0;
  virtual void exitLower_guard(ASPCore2Parser::Lower_guardContext *ctx) = 0;

  virtual void enterUpper_guard(ASPCore2Parser::Upper_guardContext *ctx) = 0;
  virtual void exitUpper_guard(ASPCore2Parser::Upper_guardContext *ctx) = 0;

  virtual void enterChoice_elements(ASPCore2Parser::Choice_elementsContext *ctx) = 0;
  virtual void exitChoice_elements(ASPCore2Parser::Choice_elementsContext *ctx) = 0;

  virtual void enterChoice_element(ASPCore2Parser::Choice_elementContext *ctx) = 0;
  virtual void exitChoice_element(ASPCore2Parser::Choice_elementContext *ctx) = 0;

  virtual void enterChoice_element_atom(ASPCore2Parser::Choice_element_atomContext *ctx) = 0;
  virtual void exitChoice_element_atom(ASPCore2Parser::Choice_element_atomContext *ctx) = 0;

  virtual void enterChoice_elements_literals(ASPCore2Parser::Choice_elements_literalsContext *ctx) = 0;
  virtual void exitChoice_elements_literals(ASPCore2Parser::Choice_elements_literalsContext *ctx) = 0;

  virtual void enterNaf_literals(ASPCore2Parser::Naf_literalsContext *ctx) = 0;
  virtual void exitNaf_literals(ASPCore2Parser::Naf_literalsContext *ctx) = 0;

  virtual void enterNaf_literal(ASPCore2Parser::Naf_literalContext *ctx) = 0;
  virtual void exitNaf_literal(ASPCore2Parser::Naf_literalContext *ctx) = 0;

  virtual void enterNaf_literal_aggregate(ASPCore2Parser::Naf_literal_aggregateContext *ctx) = 0;
  virtual void exitNaf_literal_aggregate(ASPCore2Parser::Naf_literal_aggregateContext *ctx) = 0;

  virtual void enterExistential_atom(ASPCore2Parser::Existential_atomContext *ctx) = 0;
  virtual void exitExistential_atom(ASPCore2Parser::Existential_atomContext *ctx) = 0;

  virtual void enterClassic_literal(ASPCore2Parser::Classic_literalContext *ctx) = 0;
  virtual void exitClassic_literal(ASPCore2Parser::Classic_literalContext *ctx) = 0;

  virtual void enterAtom(ASPCore2Parser::AtomContext *ctx) = 0;
  virtual void exitAtom(ASPCore2Parser::AtomContext *ctx) = 0;

  virtual void enterExtAtom(ASPCore2Parser::ExtAtomContext *ctx) = 0;
  virtual void exitExtAtom(ASPCore2Parser::ExtAtomContext *ctx) = 0;

  virtual void enterExtSemicol(ASPCore2Parser::ExtSemicolContext *ctx) = 0;
  virtual void exitExtSemicol(ASPCore2Parser::ExtSemicolContext *ctx) = 0;

  virtual void enterTerms(ASPCore2Parser::TermsContext *ctx) = 0;
  virtual void exitTerms(ASPCore2Parser::TermsContext *ctx) = 0;

  virtual void enterBasic_terms(ASPCore2Parser::Basic_termsContext *ctx) = 0;
  virtual void exitBasic_terms(ASPCore2Parser::Basic_termsContext *ctx) = 0;

  virtual void enterBuiltin_atom(ASPCore2Parser::Builtin_atomContext *ctx) = 0;
  virtual void exitBuiltin_atom(ASPCore2Parser::Builtin_atomContext *ctx) = 0;

  virtual void enterCompareop(ASPCore2Parser::CompareopContext *ctx) = 0;
  virtual void exitCompareop(ASPCore2Parser::CompareopContext *ctx) = 0;

  virtual void enterBinop(ASPCore2Parser::BinopContext *ctx) = 0;
  virtual void exitBinop(ASPCore2Parser::BinopContext *ctx) = 0;

  virtual void enterArithop1(ASPCore2Parser::Arithop1Context *ctx) = 0;
  virtual void exitArithop1(ASPCore2Parser::Arithop1Context *ctx) = 0;

  virtual void enterArithop2(ASPCore2Parser::Arithop2Context *ctx) = 0;
  virtual void exitArithop2(ASPCore2Parser::Arithop2Context *ctx) = 0;

  virtual void enterTerm_(ASPCore2Parser::Term_Context *ctx) = 0;
  virtual void exitTerm_(ASPCore2Parser::Term_Context *ctx) = 0;

  virtual void enterTerm__(ASPCore2Parser::Term__Context *ctx) = 0;
  virtual void exitTerm__(ASPCore2Parser::Term__Context *ctx) = 0;

  virtual void enterTerm(ASPCore2Parser::TermContext *ctx) = 0;
  virtual void exitTerm(ASPCore2Parser::TermContext *ctx) = 0;

  virtual void enterExpr(ASPCore2Parser::ExprContext *ctx) = 0;
  virtual void exitExpr(ASPCore2Parser::ExprContext *ctx) = 0;

  virtual void enterFactor(ASPCore2Parser::FactorContext *ctx) = 0;
  virtual void exitFactor(ASPCore2Parser::FactorContext *ctx) = 0;

  virtual void enterBasic_term(ASPCore2Parser::Basic_termContext *ctx) = 0;
  virtual void exitBasic_term(ASPCore2Parser::Basic_termContext *ctx) = 0;

  virtual void enterGround_term(ASPCore2Parser::Ground_termContext *ctx) = 0;
  virtual void exitGround_term(ASPCore2Parser::Ground_termContext *ctx) = 0;

  virtual void enterVariable_term(ASPCore2Parser::Variable_termContext *ctx) = 0;
  virtual void exitVariable_term(ASPCore2Parser::Variable_termContext *ctx) = 0;

  virtual void enterFunctional_term(ASPCore2Parser::Functional_termContext *ctx) = 0;
  virtual void exitFunctional_term(ASPCore2Parser::Functional_termContext *ctx) = 0;

  virtual void enterVars(ASPCore2Parser::VarsContext *ctx) = 0;
  virtual void exitVars(ASPCore2Parser::VarsContext *ctx) = 0;

  virtual void enterIdentifier(ASPCore2Parser::IdentifierContext *ctx) = 0;
  virtual void exitIdentifier(ASPCore2Parser::IdentifierContext *ctx) = 0;

  virtual void enterDirective(ASPCore2Parser::DirectiveContext *ctx) = 0;
  virtual void exitDirective(ASPCore2Parser::DirectiveContext *ctx) = 0;

  virtual void enterQuery(ASPCore2Parser::QueryContext *ctx) = 0;
  virtual void exitQuery(ASPCore2Parser::QueryContext *ctx) = 0;

  virtual void enterLower_guard_compare_aggregate(ASPCore2Parser::Lower_guard_compare_aggregateContext *ctx) = 0;
  virtual void exitLower_guard_compare_aggregate(ASPCore2Parser::Lower_guard_compare_aggregateContext *ctx) = 0;

  virtual void enterUpper_guard_compare_aggregate(ASPCore2Parser::Upper_guard_compare_aggregateContext *ctx) = 0;
  virtual void exitUpper_guard_compare_aggregate(ASPCore2Parser::Upper_guard_compare_aggregateContext *ctx) = 0;

  virtual void enterCompare_aggregate(ASPCore2Parser::Compare_aggregateContext *ctx) = 0;
  virtual void exitCompare_aggregate(ASPCore2Parser::Compare_aggregateContext *ctx) = 0;

  virtual void enterLower_guard_leftward_left_aggregate(ASPCore2Parser::Lower_guard_leftward_left_aggregateContext *ctx) = 0;
  virtual void exitLower_guard_leftward_left_aggregate(ASPCore2Parser::Lower_guard_leftward_left_aggregateContext *ctx) = 0;

  virtual void enterLeftward_left_aggregate(ASPCore2Parser::Leftward_left_aggregateContext *ctx) = 0;
  virtual void exitLeftward_left_aggregate(ASPCore2Parser::Leftward_left_aggregateContext *ctx) = 0;

  virtual void enterLeft_aggregate(ASPCore2Parser::Left_aggregateContext *ctx) = 0;
  virtual void exitLeft_aggregate(ASPCore2Parser::Left_aggregateContext *ctx) = 0;

  virtual void enterLower_guard_rightward_left_aggregate(ASPCore2Parser::Lower_guard_rightward_left_aggregateContext *ctx) = 0;
  virtual void exitLower_guard_rightward_left_aggregate(ASPCore2Parser::Lower_guard_rightward_left_aggregateContext *ctx) = 0;

  virtual void enterRightward_left_aggregate(ASPCore2Parser::Rightward_left_aggregateContext *ctx) = 0;
  virtual void exitRightward_left_aggregate(ASPCore2Parser::Rightward_left_aggregateContext *ctx) = 0;

  virtual void enterUpper_guard_leftward_right_aggregate(ASPCore2Parser::Upper_guard_leftward_right_aggregateContext *ctx) = 0;
  virtual void exitUpper_guard_leftward_right_aggregate(ASPCore2Parser::Upper_guard_leftward_right_aggregateContext *ctx) = 0;

  virtual void enterUpper_guard_rightward_right_aggregate(ASPCore2Parser::Upper_guard_rightward_right_aggregateContext *ctx) = 0;
  virtual void exitUpper_guard_rightward_right_aggregate(ASPCore2Parser::Upper_guard_rightward_right_aggregateContext *ctx) = 0;

  virtual void enterRight_aggregate(ASPCore2Parser::Right_aggregateContext *ctx) = 0;
  virtual void exitRight_aggregate(ASPCore2Parser::Right_aggregateContext *ctx) = 0;

  virtual void enterAggregate_atom(ASPCore2Parser::Aggregate_atomContext *ctx) = 0;
  virtual void exitAggregate_atom(ASPCore2Parser::Aggregate_atomContext *ctx) = 0;

  virtual void enterLeftwardop(ASPCore2Parser::LeftwardopContext *ctx) = 0;
  virtual void exitLeftwardop(ASPCore2Parser::LeftwardopContext *ctx) = 0;

  virtual void enterRightwardop(ASPCore2Parser::RightwardopContext *ctx) = 0;
  virtual void exitRightwardop(ASPCore2Parser::RightwardopContext *ctx) = 0;

  virtual void enterAggregate(ASPCore2Parser::AggregateContext *ctx) = 0;
  virtual void exitAggregate(ASPCore2Parser::AggregateContext *ctx) = 0;

  virtual void enterAggregate_elements(ASPCore2Parser::Aggregate_elementsContext *ctx) = 0;
  virtual void exitAggregate_elements(ASPCore2Parser::Aggregate_elementsContext *ctx) = 0;

  virtual void enterAggregate_element(ASPCore2Parser::Aggregate_elementContext *ctx) = 0;
  virtual void exitAggregate_element(ASPCore2Parser::Aggregate_elementContext *ctx) = 0;

  virtual void enterAggregate_function(ASPCore2Parser::Aggregate_functionContext *ctx) = 0;
  virtual void exitAggregate_function(ASPCore2Parser::Aggregate_functionContext *ctx) = 0;

  virtual void enterAnnotation_global(ASPCore2Parser::Annotation_globalContext *ctx) = 0;
  virtual void exitAnnotation_global(ASPCore2Parser::Annotation_globalContext *ctx) = 0;

  virtual void enterAnnotations_rule(ASPCore2Parser::Annotations_ruleContext *ctx) = 0;
  virtual void exitAnnotations_rule(ASPCore2Parser::Annotations_ruleContext *ctx) = 0;

  virtual void enterAnnotation_rule(ASPCore2Parser::Annotation_ruleContext *ctx) = 0;
  virtual void exitAnnotation_rule(ASPCore2Parser::Annotation_ruleContext *ctx) = 0;

  virtual void enterAnnotation_rule_to_decompose(ASPCore2Parser::Annotation_rule_to_decomposeContext *ctx) = 0;
  virtual void exitAnnotation_rule_to_decompose(ASPCore2Parser::Annotation_rule_to_decomposeContext *ctx) = 0;

  virtual void enterAnnotation_rule_to_not_decompose(ASPCore2Parser::Annotation_rule_to_not_decomposeContext *ctx) = 0;
  virtual void exitAnnotation_rule_to_not_decompose(ASPCore2Parser::Annotation_rule_to_not_decomposeContext *ctx) = 0;

  virtual void enterAnnotation_rule_ordering(ASPCore2Parser::Annotation_rule_orderingContext *ctx) = 0;
  virtual void exitAnnotation_rule_ordering(ASPCore2Parser::Annotation_rule_orderingContext *ctx) = 0;

  virtual void enterAnnotation_global_ordering(ASPCore2Parser::Annotation_global_orderingContext *ctx) = 0;
  virtual void exitAnnotation_global_ordering(ASPCore2Parser::Annotation_global_orderingContext *ctx) = 0;

  virtual void enterAnnotation_global_extatom_conversion(ASPCore2Parser::Annotation_global_extatom_conversionContext *ctx) = 0;
  virtual void exitAnnotation_global_extatom_conversion(ASPCore2Parser::Annotation_global_extatom_conversionContext *ctx) = 0;

  virtual void enterAnnotation_extpredicate(ASPCore2Parser::Annotation_extpredicateContext *ctx) = 0;
  virtual void exitAnnotation_extpredicate(ASPCore2Parser::Annotation_extpredicateContext *ctx) = 0;

  virtual void enterExtatom_conv_types(ASPCore2Parser::Extatom_conv_typesContext *ctx) = 0;
  virtual void exitExtatom_conv_types(ASPCore2Parser::Extatom_conv_typesContext *ctx) = 0;

  virtual void enterExtatom_conv_type(ASPCore2Parser::Extatom_conv_typeContext *ctx) = 0;
  virtual void exitExtatom_conv_type(ASPCore2Parser::Extatom_conv_typeContext *ctx) = 0;

  virtual void enterRule_ordering_type(ASPCore2Parser::Rule_ordering_typeContext *ctx) = 0;
  virtual void exitRule_ordering_type(ASPCore2Parser::Rule_ordering_typeContext *ctx) = 0;

  virtual void enterGlobal_ordering_type(ASPCore2Parser::Global_ordering_typeContext *ctx) = 0;
  virtual void exitGlobal_ordering_type(ASPCore2Parser::Global_ordering_typeContext *ctx) = 0;

  virtual void enterAnnotation_rule_atom_indexed(ASPCore2Parser::Annotation_rule_atom_indexedContext *ctx) = 0;
  virtual void exitAnnotation_rule_atom_indexed(ASPCore2Parser::Annotation_rule_atom_indexedContext *ctx) = 0;

  virtual void enterAnnotation_global_atom_indexed(ASPCore2Parser::Annotation_global_atom_indexedContext *ctx) = 0;
  virtual void exitAnnotation_global_atom_indexed(ASPCore2Parser::Annotation_global_atom_indexedContext *ctx) = 0;

  virtual void enterRule_indexing_arguments(ASPCore2Parser::Rule_indexing_argumentsContext *ctx) = 0;
  virtual void exitRule_indexing_arguments(ASPCore2Parser::Rule_indexing_argumentsContext *ctx) = 0;

  virtual void enterRule_naf_literal_annotation(ASPCore2Parser::Rule_naf_literal_annotationContext *ctx) = 0;
  virtual void exitRule_naf_literal_annotation(ASPCore2Parser::Rule_naf_literal_annotationContext *ctx) = 0;

  virtual void enterGlobal_indexing_arguments(ASPCore2Parser::Global_indexing_argumentsContext *ctx) = 0;
  virtual void exitGlobal_indexing_arguments(ASPCore2Parser::Global_indexing_argumentsContext *ctx) = 0;

  virtual void enterGlobal_naf_literal_annotation(ASPCore2Parser::Global_naf_literal_annotationContext *ctx) = 0;
  virtual void exitGlobal_naf_literal_annotation(ASPCore2Parser::Global_naf_literal_annotationContext *ctx) = 0;

  virtual void enterRule_atom_annotation_before(ASPCore2Parser::Rule_atom_annotation_beforeContext *ctx) = 0;
  virtual void exitRule_atom_annotation_before(ASPCore2Parser::Rule_atom_annotation_beforeContext *ctx) = 0;

  virtual void enterGlobal_atom_annotation_before(ASPCore2Parser::Global_atom_annotation_beforeContext *ctx) = 0;
  virtual void exitGlobal_atom_annotation_before(ASPCore2Parser::Global_atom_annotation_beforeContext *ctx) = 0;

  virtual void enterRule_atom_annotation_after(ASPCore2Parser::Rule_atom_annotation_afterContext *ctx) = 0;
  virtual void exitRule_atom_annotation_after(ASPCore2Parser::Rule_atom_annotation_afterContext *ctx) = 0;

  virtual void enterGlobal_atom_annotation_after(ASPCore2Parser::Global_atom_annotation_afterContext *ctx) = 0;
  virtual void exitGlobal_atom_annotation_after(ASPCore2Parser::Global_atom_annotation_afterContext *ctx) = 0;

  virtual void enterRule_atoms_annotation_before(ASPCore2Parser::Rule_atoms_annotation_beforeContext *ctx) = 0;
  virtual void exitRule_atoms_annotation_before(ASPCore2Parser::Rule_atoms_annotation_beforeContext *ctx) = 0;

  virtual void enterGlobal_atoms_annotation_before(ASPCore2Parser::Global_atoms_annotation_beforeContext *ctx) = 0;
  virtual void exitGlobal_atoms_annotation_before(ASPCore2Parser::Global_atoms_annotation_beforeContext *ctx) = 0;

  virtual void enterRule_atoms_annotation_after(ASPCore2Parser::Rule_atoms_annotation_afterContext *ctx) = 0;
  virtual void exitRule_atoms_annotation_after(ASPCore2Parser::Rule_atoms_annotation_afterContext *ctx) = 0;

  virtual void enterGlobal_atoms_annotation_after(ASPCore2Parser::Global_atoms_annotation_afterContext *ctx) = 0;
  virtual void exitGlobal_atoms_annotation_after(ASPCore2Parser::Global_atoms_annotation_afterContext *ctx) = 0;

  virtual void enterAnnotation_rule_partial_order_begin(ASPCore2Parser::Annotation_rule_partial_order_beginContext *ctx) = 0;
  virtual void exitAnnotation_rule_partial_order_begin(ASPCore2Parser::Annotation_rule_partial_order_beginContext *ctx) = 0;

  virtual void enterAnnotation_global_partial_order_begin(ASPCore2Parser::Annotation_global_partial_order_beginContext *ctx) = 0;
  virtual void exitAnnotation_global_partial_order_begin(ASPCore2Parser::Annotation_global_partial_order_beginContext *ctx) = 0;

  virtual void enterAnnotation_rule_partial_order(ASPCore2Parser::Annotation_rule_partial_orderContext *ctx) = 0;
  virtual void exitAnnotation_rule_partial_order(ASPCore2Parser::Annotation_rule_partial_orderContext *ctx) = 0;

  virtual void enterAnnotation_global_partial_order(ASPCore2Parser::Annotation_global_partial_orderContext *ctx) = 0;
  virtual void exitAnnotation_global_partial_order(ASPCore2Parser::Annotation_global_partial_orderContext *ctx) = 0;

  virtual void enterAnnotation_rule_projection(ASPCore2Parser::Annotation_rule_projectionContext *ctx) = 0;
  virtual void exitAnnotation_rule_projection(ASPCore2Parser::Annotation_rule_projectionContext *ctx) = 0;

  virtual void enterAnnotation_rule_projection_value(ASPCore2Parser::Annotation_rule_projection_valueContext *ctx) = 0;
  virtual void exitAnnotation_rule_projection_value(ASPCore2Parser::Annotation_rule_projection_valueContext *ctx) = 0;

  virtual void enterAnnotation_rule_rewriting_arith(ASPCore2Parser::Annotation_rule_rewriting_arithContext *ctx) = 0;
  virtual void exitAnnotation_rule_rewriting_arith(ASPCore2Parser::Annotation_rule_rewriting_arithContext *ctx) = 0;

  virtual void enterAnnotation_rule_align_substitutions(ASPCore2Parser::Annotation_rule_align_substitutionsContext *ctx) = 0;
  virtual void exitAnnotation_rule_align_substitutions(ASPCore2Parser::Annotation_rule_align_substitutionsContext *ctx) = 0;

  virtual void enterAnnotation_rule_look_ahead(ASPCore2Parser::Annotation_rule_look_aheadContext *ctx) = 0;
  virtual void exitAnnotation_rule_look_ahead(ASPCore2Parser::Annotation_rule_look_aheadContext *ctx) = 0;

  virtual void enterAnnotation_global_wasp_propagator(ASPCore2Parser::Annotation_global_wasp_propagatorContext *ctx) = 0;
  virtual void exitAnnotation_global_wasp_propagator(ASPCore2Parser::Annotation_global_wasp_propagatorContext *ctx) = 0;

  virtual void enterAnnotation_global_wasp_propagator_begin(ASPCore2Parser::Annotation_global_wasp_propagator_beginContext *ctx) = 0;
  virtual void exitAnnotation_global_wasp_propagator_begin(ASPCore2Parser::Annotation_global_wasp_propagator_beginContext *ctx) = 0;

  virtual void enterAnnotation_global_wasp_heuristic(ASPCore2Parser::Annotation_global_wasp_heuristicContext *ctx) = 0;
  virtual void exitAnnotation_global_wasp_heuristic(ASPCore2Parser::Annotation_global_wasp_heuristicContext *ctx) = 0;

  virtual void enterAnnotation_global_wasp_heuristic_begin(ASPCore2Parser::Annotation_global_wasp_heuristic_beginContext *ctx) = 0;
  virtual void exitAnnotation_global_wasp_heuristic_begin(ASPCore2Parser::Annotation_global_wasp_heuristic_beginContext *ctx) = 0;

  virtual void enterHeuristic_python_file(ASPCore2Parser::Heuristic_python_fileContext *ctx) = 0;
  virtual void exitHeuristic_python_file(ASPCore2Parser::Heuristic_python_fileContext *ctx) = 0;

  virtual void enterAggregate_element_heuristic(ASPCore2Parser::Aggregate_element_heuristicContext *ctx) = 0;
  virtual void exitAggregate_element_heuristic(ASPCore2Parser::Aggregate_element_heuristicContext *ctx) = 0;

  virtual void enterAggregate_elements_heuristic(ASPCore2Parser::Aggregate_elements_heuristicContext *ctx) = 0;
  virtual void exitAggregate_elements_heuristic(ASPCore2Parser::Aggregate_elements_heuristicContext *ctx) = 0;


};

