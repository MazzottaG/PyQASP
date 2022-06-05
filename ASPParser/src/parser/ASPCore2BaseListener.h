
// Generated from ASPCore2.g4 by ANTLR 4.10.1

#pragma once


#include "antlr4-runtime.h"
#include "ASPCore2Listener.h"


/**
 * This class provides an empty implementation of ASPCore2Listener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  ASPCore2BaseListener : public ASPCore2Listener {
public:

  virtual void enterProgram(ASPCore2Parser::ProgramContext * /*ctx*/) override { }
  virtual void exitProgram(ASPCore2Parser::ProgramContext * /*ctx*/) override { }

  virtual void enterRule(ASPCore2Parser::RuleContext * /*ctx*/) override { }
  virtual void exitRule(ASPCore2Parser::RuleContext * /*ctx*/) override { }

  virtual void enterSimple_rule(ASPCore2Parser::Simple_ruleContext * /*ctx*/) override { }
  virtual void exitSimple_rule(ASPCore2Parser::Simple_ruleContext * /*ctx*/) override { }

  virtual void enterHead(ASPCore2Parser::HeadContext * /*ctx*/) override { }
  virtual void exitHead(ASPCore2Parser::HeadContext * /*ctx*/) override { }

  virtual void enterBody(ASPCore2Parser::BodyContext * /*ctx*/) override { }
  virtual void exitBody(ASPCore2Parser::BodyContext * /*ctx*/) override { }

  virtual void enterWeight_at_levels(ASPCore2Parser::Weight_at_levelsContext * /*ctx*/) override { }
  virtual void exitWeight_at_levels(ASPCore2Parser::Weight_at_levelsContext * /*ctx*/) override { }

  virtual void enterLevels_and_terms(ASPCore2Parser::Levels_and_termsContext * /*ctx*/) override { }
  virtual void exitLevels_and_terms(ASPCore2Parser::Levels_and_termsContext * /*ctx*/) override { }

  virtual void enterDisjunction(ASPCore2Parser::DisjunctionContext * /*ctx*/) override { }
  virtual void exitDisjunction(ASPCore2Parser::DisjunctionContext * /*ctx*/) override { }

  virtual void enterConjunction(ASPCore2Parser::ConjunctionContext * /*ctx*/) override { }
  virtual void exitConjunction(ASPCore2Parser::ConjunctionContext * /*ctx*/) override { }

  virtual void enterChoice_atom(ASPCore2Parser::Choice_atomContext * /*ctx*/) override { }
  virtual void exitChoice_atom(ASPCore2Parser::Choice_atomContext * /*ctx*/) override { }

  virtual void enterLower_guard(ASPCore2Parser::Lower_guardContext * /*ctx*/) override { }
  virtual void exitLower_guard(ASPCore2Parser::Lower_guardContext * /*ctx*/) override { }

  virtual void enterUpper_guard(ASPCore2Parser::Upper_guardContext * /*ctx*/) override { }
  virtual void exitUpper_guard(ASPCore2Parser::Upper_guardContext * /*ctx*/) override { }

  virtual void enterChoice_elements(ASPCore2Parser::Choice_elementsContext * /*ctx*/) override { }
  virtual void exitChoice_elements(ASPCore2Parser::Choice_elementsContext * /*ctx*/) override { }

  virtual void enterChoice_element(ASPCore2Parser::Choice_elementContext * /*ctx*/) override { }
  virtual void exitChoice_element(ASPCore2Parser::Choice_elementContext * /*ctx*/) override { }

  virtual void enterChoice_element_atom(ASPCore2Parser::Choice_element_atomContext * /*ctx*/) override { }
  virtual void exitChoice_element_atom(ASPCore2Parser::Choice_element_atomContext * /*ctx*/) override { }

  virtual void enterChoice_elements_literals(ASPCore2Parser::Choice_elements_literalsContext * /*ctx*/) override { }
  virtual void exitChoice_elements_literals(ASPCore2Parser::Choice_elements_literalsContext * /*ctx*/) override { }

  virtual void enterNaf_literals(ASPCore2Parser::Naf_literalsContext * /*ctx*/) override { }
  virtual void exitNaf_literals(ASPCore2Parser::Naf_literalsContext * /*ctx*/) override { }

  virtual void enterNaf_literal(ASPCore2Parser::Naf_literalContext * /*ctx*/) override { }
  virtual void exitNaf_literal(ASPCore2Parser::Naf_literalContext * /*ctx*/) override { }

  virtual void enterNaf_literal_aggregate(ASPCore2Parser::Naf_literal_aggregateContext * /*ctx*/) override { }
  virtual void exitNaf_literal_aggregate(ASPCore2Parser::Naf_literal_aggregateContext * /*ctx*/) override { }

  virtual void enterExistential_atom(ASPCore2Parser::Existential_atomContext * /*ctx*/) override { }
  virtual void exitExistential_atom(ASPCore2Parser::Existential_atomContext * /*ctx*/) override { }

  virtual void enterClassic_literal(ASPCore2Parser::Classic_literalContext * /*ctx*/) override { }
  virtual void exitClassic_literal(ASPCore2Parser::Classic_literalContext * /*ctx*/) override { }

  virtual void enterAtom(ASPCore2Parser::AtomContext * /*ctx*/) override { }
  virtual void exitAtom(ASPCore2Parser::AtomContext * /*ctx*/) override { }

  virtual void enterExtAtom(ASPCore2Parser::ExtAtomContext * /*ctx*/) override { }
  virtual void exitExtAtom(ASPCore2Parser::ExtAtomContext * /*ctx*/) override { }

  virtual void enterExtSemicol(ASPCore2Parser::ExtSemicolContext * /*ctx*/) override { }
  virtual void exitExtSemicol(ASPCore2Parser::ExtSemicolContext * /*ctx*/) override { }

  virtual void enterTerms(ASPCore2Parser::TermsContext * /*ctx*/) override { }
  virtual void exitTerms(ASPCore2Parser::TermsContext * /*ctx*/) override { }

  virtual void enterBasic_terms(ASPCore2Parser::Basic_termsContext * /*ctx*/) override { }
  virtual void exitBasic_terms(ASPCore2Parser::Basic_termsContext * /*ctx*/) override { }

  virtual void enterBuiltin_atom(ASPCore2Parser::Builtin_atomContext * /*ctx*/) override { }
  virtual void exitBuiltin_atom(ASPCore2Parser::Builtin_atomContext * /*ctx*/) override { }

  virtual void enterCompareop(ASPCore2Parser::CompareopContext * /*ctx*/) override { }
  virtual void exitCompareop(ASPCore2Parser::CompareopContext * /*ctx*/) override { }

  virtual void enterBinop(ASPCore2Parser::BinopContext * /*ctx*/) override { }
  virtual void exitBinop(ASPCore2Parser::BinopContext * /*ctx*/) override { }

  virtual void enterArithop1(ASPCore2Parser::Arithop1Context * /*ctx*/) override { }
  virtual void exitArithop1(ASPCore2Parser::Arithop1Context * /*ctx*/) override { }

  virtual void enterArithop2(ASPCore2Parser::Arithop2Context * /*ctx*/) override { }
  virtual void exitArithop2(ASPCore2Parser::Arithop2Context * /*ctx*/) override { }

  virtual void enterTerm_(ASPCore2Parser::Term_Context * /*ctx*/) override { }
  virtual void exitTerm_(ASPCore2Parser::Term_Context * /*ctx*/) override { }

  virtual void enterTerm__(ASPCore2Parser::Term__Context * /*ctx*/) override { }
  virtual void exitTerm__(ASPCore2Parser::Term__Context * /*ctx*/) override { }

  virtual void enterTerm(ASPCore2Parser::TermContext * /*ctx*/) override { }
  virtual void exitTerm(ASPCore2Parser::TermContext * /*ctx*/) override { }

  virtual void enterExpr(ASPCore2Parser::ExprContext * /*ctx*/) override { }
  virtual void exitExpr(ASPCore2Parser::ExprContext * /*ctx*/) override { }

  virtual void enterFactor(ASPCore2Parser::FactorContext * /*ctx*/) override { }
  virtual void exitFactor(ASPCore2Parser::FactorContext * /*ctx*/) override { }

  virtual void enterBasic_term(ASPCore2Parser::Basic_termContext * /*ctx*/) override { }
  virtual void exitBasic_term(ASPCore2Parser::Basic_termContext * /*ctx*/) override { }

  virtual void enterGround_term(ASPCore2Parser::Ground_termContext * /*ctx*/) override { }
  virtual void exitGround_term(ASPCore2Parser::Ground_termContext * /*ctx*/) override { }

  virtual void enterVariable_term(ASPCore2Parser::Variable_termContext * /*ctx*/) override { }
  virtual void exitVariable_term(ASPCore2Parser::Variable_termContext * /*ctx*/) override { }

  virtual void enterFunctional_term(ASPCore2Parser::Functional_termContext * /*ctx*/) override { }
  virtual void exitFunctional_term(ASPCore2Parser::Functional_termContext * /*ctx*/) override { }

  virtual void enterVars(ASPCore2Parser::VarsContext * /*ctx*/) override { }
  virtual void exitVars(ASPCore2Parser::VarsContext * /*ctx*/) override { }

  virtual void enterIdentifier(ASPCore2Parser::IdentifierContext * /*ctx*/) override { }
  virtual void exitIdentifier(ASPCore2Parser::IdentifierContext * /*ctx*/) override { }

  virtual void enterDirective(ASPCore2Parser::DirectiveContext * /*ctx*/) override { }
  virtual void exitDirective(ASPCore2Parser::DirectiveContext * /*ctx*/) override { }

  virtual void enterQuery(ASPCore2Parser::QueryContext * /*ctx*/) override { }
  virtual void exitQuery(ASPCore2Parser::QueryContext * /*ctx*/) override { }

  virtual void enterLower_guard_compare_aggregate(ASPCore2Parser::Lower_guard_compare_aggregateContext * /*ctx*/) override { }
  virtual void exitLower_guard_compare_aggregate(ASPCore2Parser::Lower_guard_compare_aggregateContext * /*ctx*/) override { }

  virtual void enterUpper_guard_compare_aggregate(ASPCore2Parser::Upper_guard_compare_aggregateContext * /*ctx*/) override { }
  virtual void exitUpper_guard_compare_aggregate(ASPCore2Parser::Upper_guard_compare_aggregateContext * /*ctx*/) override { }

  virtual void enterCompare_aggregate(ASPCore2Parser::Compare_aggregateContext * /*ctx*/) override { }
  virtual void exitCompare_aggregate(ASPCore2Parser::Compare_aggregateContext * /*ctx*/) override { }

  virtual void enterLower_guard_leftward_left_aggregate(ASPCore2Parser::Lower_guard_leftward_left_aggregateContext * /*ctx*/) override { }
  virtual void exitLower_guard_leftward_left_aggregate(ASPCore2Parser::Lower_guard_leftward_left_aggregateContext * /*ctx*/) override { }

  virtual void enterLeftward_left_aggregate(ASPCore2Parser::Leftward_left_aggregateContext * /*ctx*/) override { }
  virtual void exitLeftward_left_aggregate(ASPCore2Parser::Leftward_left_aggregateContext * /*ctx*/) override { }

  virtual void enterLeft_aggregate(ASPCore2Parser::Left_aggregateContext * /*ctx*/) override { }
  virtual void exitLeft_aggregate(ASPCore2Parser::Left_aggregateContext * /*ctx*/) override { }

  virtual void enterLower_guard_rightward_left_aggregate(ASPCore2Parser::Lower_guard_rightward_left_aggregateContext * /*ctx*/) override { }
  virtual void exitLower_guard_rightward_left_aggregate(ASPCore2Parser::Lower_guard_rightward_left_aggregateContext * /*ctx*/) override { }

  virtual void enterRightward_left_aggregate(ASPCore2Parser::Rightward_left_aggregateContext * /*ctx*/) override { }
  virtual void exitRightward_left_aggregate(ASPCore2Parser::Rightward_left_aggregateContext * /*ctx*/) override { }

  virtual void enterUpper_guard_leftward_right_aggregate(ASPCore2Parser::Upper_guard_leftward_right_aggregateContext * /*ctx*/) override { }
  virtual void exitUpper_guard_leftward_right_aggregate(ASPCore2Parser::Upper_guard_leftward_right_aggregateContext * /*ctx*/) override { }

  virtual void enterUpper_guard_rightward_right_aggregate(ASPCore2Parser::Upper_guard_rightward_right_aggregateContext * /*ctx*/) override { }
  virtual void exitUpper_guard_rightward_right_aggregate(ASPCore2Parser::Upper_guard_rightward_right_aggregateContext * /*ctx*/) override { }

  virtual void enterRight_aggregate(ASPCore2Parser::Right_aggregateContext * /*ctx*/) override { }
  virtual void exitRight_aggregate(ASPCore2Parser::Right_aggregateContext * /*ctx*/) override { }

  virtual void enterAggregate_atom(ASPCore2Parser::Aggregate_atomContext * /*ctx*/) override { }
  virtual void exitAggregate_atom(ASPCore2Parser::Aggregate_atomContext * /*ctx*/) override { }

  virtual void enterLeftwardop(ASPCore2Parser::LeftwardopContext * /*ctx*/) override { }
  virtual void exitLeftwardop(ASPCore2Parser::LeftwardopContext * /*ctx*/) override { }

  virtual void enterRightwardop(ASPCore2Parser::RightwardopContext * /*ctx*/) override { }
  virtual void exitRightwardop(ASPCore2Parser::RightwardopContext * /*ctx*/) override { }

  virtual void enterAggregate(ASPCore2Parser::AggregateContext * /*ctx*/) override { }
  virtual void exitAggregate(ASPCore2Parser::AggregateContext * /*ctx*/) override { }

  virtual void enterAggregate_elements(ASPCore2Parser::Aggregate_elementsContext * /*ctx*/) override { }
  virtual void exitAggregate_elements(ASPCore2Parser::Aggregate_elementsContext * /*ctx*/) override { }

  virtual void enterAggregate_element(ASPCore2Parser::Aggregate_elementContext * /*ctx*/) override { }
  virtual void exitAggregate_element(ASPCore2Parser::Aggregate_elementContext * /*ctx*/) override { }

  virtual void enterAggregate_function(ASPCore2Parser::Aggregate_functionContext * /*ctx*/) override { }
  virtual void exitAggregate_function(ASPCore2Parser::Aggregate_functionContext * /*ctx*/) override { }

  virtual void enterAnnotation_global(ASPCore2Parser::Annotation_globalContext * /*ctx*/) override { }
  virtual void exitAnnotation_global(ASPCore2Parser::Annotation_globalContext * /*ctx*/) override { }

  virtual void enterAnnotations_rule(ASPCore2Parser::Annotations_ruleContext * /*ctx*/) override { }
  virtual void exitAnnotations_rule(ASPCore2Parser::Annotations_ruleContext * /*ctx*/) override { }

  virtual void enterAnnotation_rule(ASPCore2Parser::Annotation_ruleContext * /*ctx*/) override { }
  virtual void exitAnnotation_rule(ASPCore2Parser::Annotation_ruleContext * /*ctx*/) override { }

  virtual void enterAnnotation_rule_to_decompose(ASPCore2Parser::Annotation_rule_to_decomposeContext * /*ctx*/) override { }
  virtual void exitAnnotation_rule_to_decompose(ASPCore2Parser::Annotation_rule_to_decomposeContext * /*ctx*/) override { }

  virtual void enterAnnotation_rule_to_not_decompose(ASPCore2Parser::Annotation_rule_to_not_decomposeContext * /*ctx*/) override { }
  virtual void exitAnnotation_rule_to_not_decompose(ASPCore2Parser::Annotation_rule_to_not_decomposeContext * /*ctx*/) override { }

  virtual void enterAnnotation_rule_ordering(ASPCore2Parser::Annotation_rule_orderingContext * /*ctx*/) override { }
  virtual void exitAnnotation_rule_ordering(ASPCore2Parser::Annotation_rule_orderingContext * /*ctx*/) override { }

  virtual void enterAnnotation_global_ordering(ASPCore2Parser::Annotation_global_orderingContext * /*ctx*/) override { }
  virtual void exitAnnotation_global_ordering(ASPCore2Parser::Annotation_global_orderingContext * /*ctx*/) override { }

  virtual void enterAnnotation_global_extatom_conversion(ASPCore2Parser::Annotation_global_extatom_conversionContext * /*ctx*/) override { }
  virtual void exitAnnotation_global_extatom_conversion(ASPCore2Parser::Annotation_global_extatom_conversionContext * /*ctx*/) override { }

  virtual void enterAnnotation_extpredicate(ASPCore2Parser::Annotation_extpredicateContext * /*ctx*/) override { }
  virtual void exitAnnotation_extpredicate(ASPCore2Parser::Annotation_extpredicateContext * /*ctx*/) override { }

  virtual void enterExtatom_conv_types(ASPCore2Parser::Extatom_conv_typesContext * /*ctx*/) override { }
  virtual void exitExtatom_conv_types(ASPCore2Parser::Extatom_conv_typesContext * /*ctx*/) override { }

  virtual void enterExtatom_conv_type(ASPCore2Parser::Extatom_conv_typeContext * /*ctx*/) override { }
  virtual void exitExtatom_conv_type(ASPCore2Parser::Extatom_conv_typeContext * /*ctx*/) override { }

  virtual void enterRule_ordering_type(ASPCore2Parser::Rule_ordering_typeContext * /*ctx*/) override { }
  virtual void exitRule_ordering_type(ASPCore2Parser::Rule_ordering_typeContext * /*ctx*/) override { }

  virtual void enterGlobal_ordering_type(ASPCore2Parser::Global_ordering_typeContext * /*ctx*/) override { }
  virtual void exitGlobal_ordering_type(ASPCore2Parser::Global_ordering_typeContext * /*ctx*/) override { }

  virtual void enterAnnotation_rule_atom_indexed(ASPCore2Parser::Annotation_rule_atom_indexedContext * /*ctx*/) override { }
  virtual void exitAnnotation_rule_atom_indexed(ASPCore2Parser::Annotation_rule_atom_indexedContext * /*ctx*/) override { }

  virtual void enterAnnotation_global_atom_indexed(ASPCore2Parser::Annotation_global_atom_indexedContext * /*ctx*/) override { }
  virtual void exitAnnotation_global_atom_indexed(ASPCore2Parser::Annotation_global_atom_indexedContext * /*ctx*/) override { }

  virtual void enterRule_indexing_arguments(ASPCore2Parser::Rule_indexing_argumentsContext * /*ctx*/) override { }
  virtual void exitRule_indexing_arguments(ASPCore2Parser::Rule_indexing_argumentsContext * /*ctx*/) override { }

  virtual void enterRule_naf_literal_annotation(ASPCore2Parser::Rule_naf_literal_annotationContext * /*ctx*/) override { }
  virtual void exitRule_naf_literal_annotation(ASPCore2Parser::Rule_naf_literal_annotationContext * /*ctx*/) override { }

  virtual void enterGlobal_indexing_arguments(ASPCore2Parser::Global_indexing_argumentsContext * /*ctx*/) override { }
  virtual void exitGlobal_indexing_arguments(ASPCore2Parser::Global_indexing_argumentsContext * /*ctx*/) override { }

  virtual void enterGlobal_naf_literal_annotation(ASPCore2Parser::Global_naf_literal_annotationContext * /*ctx*/) override { }
  virtual void exitGlobal_naf_literal_annotation(ASPCore2Parser::Global_naf_literal_annotationContext * /*ctx*/) override { }

  virtual void enterRule_atom_annotation_before(ASPCore2Parser::Rule_atom_annotation_beforeContext * /*ctx*/) override { }
  virtual void exitRule_atom_annotation_before(ASPCore2Parser::Rule_atom_annotation_beforeContext * /*ctx*/) override { }

  virtual void enterGlobal_atom_annotation_before(ASPCore2Parser::Global_atom_annotation_beforeContext * /*ctx*/) override { }
  virtual void exitGlobal_atom_annotation_before(ASPCore2Parser::Global_atom_annotation_beforeContext * /*ctx*/) override { }

  virtual void enterRule_atom_annotation_after(ASPCore2Parser::Rule_atom_annotation_afterContext * /*ctx*/) override { }
  virtual void exitRule_atom_annotation_after(ASPCore2Parser::Rule_atom_annotation_afterContext * /*ctx*/) override { }

  virtual void enterGlobal_atom_annotation_after(ASPCore2Parser::Global_atom_annotation_afterContext * /*ctx*/) override { }
  virtual void exitGlobal_atom_annotation_after(ASPCore2Parser::Global_atom_annotation_afterContext * /*ctx*/) override { }

  virtual void enterRule_atoms_annotation_before(ASPCore2Parser::Rule_atoms_annotation_beforeContext * /*ctx*/) override { }
  virtual void exitRule_atoms_annotation_before(ASPCore2Parser::Rule_atoms_annotation_beforeContext * /*ctx*/) override { }

  virtual void enterGlobal_atoms_annotation_before(ASPCore2Parser::Global_atoms_annotation_beforeContext * /*ctx*/) override { }
  virtual void exitGlobal_atoms_annotation_before(ASPCore2Parser::Global_atoms_annotation_beforeContext * /*ctx*/) override { }

  virtual void enterRule_atoms_annotation_after(ASPCore2Parser::Rule_atoms_annotation_afterContext * /*ctx*/) override { }
  virtual void exitRule_atoms_annotation_after(ASPCore2Parser::Rule_atoms_annotation_afterContext * /*ctx*/) override { }

  virtual void enterGlobal_atoms_annotation_after(ASPCore2Parser::Global_atoms_annotation_afterContext * /*ctx*/) override { }
  virtual void exitGlobal_atoms_annotation_after(ASPCore2Parser::Global_atoms_annotation_afterContext * /*ctx*/) override { }

  virtual void enterAnnotation_rule_partial_order_begin(ASPCore2Parser::Annotation_rule_partial_order_beginContext * /*ctx*/) override { }
  virtual void exitAnnotation_rule_partial_order_begin(ASPCore2Parser::Annotation_rule_partial_order_beginContext * /*ctx*/) override { }

  virtual void enterAnnotation_global_partial_order_begin(ASPCore2Parser::Annotation_global_partial_order_beginContext * /*ctx*/) override { }
  virtual void exitAnnotation_global_partial_order_begin(ASPCore2Parser::Annotation_global_partial_order_beginContext * /*ctx*/) override { }

  virtual void enterAnnotation_rule_partial_order(ASPCore2Parser::Annotation_rule_partial_orderContext * /*ctx*/) override { }
  virtual void exitAnnotation_rule_partial_order(ASPCore2Parser::Annotation_rule_partial_orderContext * /*ctx*/) override { }

  virtual void enterAnnotation_global_partial_order(ASPCore2Parser::Annotation_global_partial_orderContext * /*ctx*/) override { }
  virtual void exitAnnotation_global_partial_order(ASPCore2Parser::Annotation_global_partial_orderContext * /*ctx*/) override { }

  virtual void enterAnnotation_rule_projection(ASPCore2Parser::Annotation_rule_projectionContext * /*ctx*/) override { }
  virtual void exitAnnotation_rule_projection(ASPCore2Parser::Annotation_rule_projectionContext * /*ctx*/) override { }

  virtual void enterAnnotation_rule_projection_value(ASPCore2Parser::Annotation_rule_projection_valueContext * /*ctx*/) override { }
  virtual void exitAnnotation_rule_projection_value(ASPCore2Parser::Annotation_rule_projection_valueContext * /*ctx*/) override { }

  virtual void enterAnnotation_rule_rewriting_arith(ASPCore2Parser::Annotation_rule_rewriting_arithContext * /*ctx*/) override { }
  virtual void exitAnnotation_rule_rewriting_arith(ASPCore2Parser::Annotation_rule_rewriting_arithContext * /*ctx*/) override { }

  virtual void enterAnnotation_rule_align_substitutions(ASPCore2Parser::Annotation_rule_align_substitutionsContext * /*ctx*/) override { }
  virtual void exitAnnotation_rule_align_substitutions(ASPCore2Parser::Annotation_rule_align_substitutionsContext * /*ctx*/) override { }

  virtual void enterAnnotation_rule_look_ahead(ASPCore2Parser::Annotation_rule_look_aheadContext * /*ctx*/) override { }
  virtual void exitAnnotation_rule_look_ahead(ASPCore2Parser::Annotation_rule_look_aheadContext * /*ctx*/) override { }

  virtual void enterAnnotation_global_wasp_propagator(ASPCore2Parser::Annotation_global_wasp_propagatorContext * /*ctx*/) override { }
  virtual void exitAnnotation_global_wasp_propagator(ASPCore2Parser::Annotation_global_wasp_propagatorContext * /*ctx*/) override { }

  virtual void enterAnnotation_global_wasp_propagator_begin(ASPCore2Parser::Annotation_global_wasp_propagator_beginContext * /*ctx*/) override { }
  virtual void exitAnnotation_global_wasp_propagator_begin(ASPCore2Parser::Annotation_global_wasp_propagator_beginContext * /*ctx*/) override { }

  virtual void enterAnnotation_global_wasp_heuristic(ASPCore2Parser::Annotation_global_wasp_heuristicContext * /*ctx*/) override { }
  virtual void exitAnnotation_global_wasp_heuristic(ASPCore2Parser::Annotation_global_wasp_heuristicContext * /*ctx*/) override { }

  virtual void enterAnnotation_global_wasp_heuristic_begin(ASPCore2Parser::Annotation_global_wasp_heuristic_beginContext * /*ctx*/) override { }
  virtual void exitAnnotation_global_wasp_heuristic_begin(ASPCore2Parser::Annotation_global_wasp_heuristic_beginContext * /*ctx*/) override { }

  virtual void enterHeuristic_python_file(ASPCore2Parser::Heuristic_python_fileContext * /*ctx*/) override { }
  virtual void exitHeuristic_python_file(ASPCore2Parser::Heuristic_python_fileContext * /*ctx*/) override { }

  virtual void enterAggregate_element_heuristic(ASPCore2Parser::Aggregate_element_heuristicContext * /*ctx*/) override { }
  virtual void exitAggregate_element_heuristic(ASPCore2Parser::Aggregate_element_heuristicContext * /*ctx*/) override { }

  virtual void enterAggregate_elements_heuristic(ASPCore2Parser::Aggregate_elements_heuristicContext * /*ctx*/) override { }
  virtual void exitAggregate_elements_heuristic(ASPCore2Parser::Aggregate_elements_heuristicContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

