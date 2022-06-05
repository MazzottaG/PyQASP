#ifndef CONCRETE_LISTENER
#define CONCRETE_LISTENER
#include "ASPCore2Listener.h"
class ConcreteListener : public ASPCore2Listener{

    virtual void enterProgram(ASPCore2Parser::ProgramContext *ctx){
        std::cout << "enterProgram" <<std::endl;
        
    }
  virtual void exitProgram(ASPCore2Parser::ProgramContext *ctx){
        std::cout << "exitProgram" <<std::endl;
	}

  virtual void enterRule(ASPCore2Parser::RuleContext *ctx){
        std::cout << "enterRule" <<std::endl;
	}
  virtual void exitRule(ASPCore2Parser::RuleContext *ctx){
        std::cout << "exitRule" <<std::endl;
	}

  virtual void enterSimple_rule(ASPCore2Parser::Simple_ruleContext *ctx){
        std::cout << "enterSimple_rule" <<std::endl;
	}
  virtual void exitSimple_rule(ASPCore2Parser::Simple_ruleContext *ctx){
        std::cout << "exitSimple_rule" <<std::endl;
	}

  virtual void enterHead(ASPCore2Parser::HeadContext *ctx){
	}
  virtual void exitHead(ASPCore2Parser::HeadContext *ctx){
	}

  virtual void enterBody(ASPCore2Parser::BodyContext *ctx){
	}
  virtual void exitBody(ASPCore2Parser::BodyContext *ctx){
	}

  virtual void enterWeight_at_levels(ASPCore2Parser::Weight_at_levelsContext *ctx){
	}
  virtual void exitWeight_at_levels(ASPCore2Parser::Weight_at_levelsContext *ctx){
	}

  virtual void enterLevels_and_terms(ASPCore2Parser::Levels_and_termsContext *ctx){
	}
  virtual void exitLevels_and_terms(ASPCore2Parser::Levels_and_termsContext *ctx){
	}

  virtual void enterDisjunction(ASPCore2Parser::DisjunctionContext *ctx){
	}
  virtual void exitDisjunction(ASPCore2Parser::DisjunctionContext *ctx){
	}

  virtual void enterConjunction(ASPCore2Parser::ConjunctionContext *ctx){
	}
  virtual void exitConjunction(ASPCore2Parser::ConjunctionContext *ctx){
	}

  virtual void enterClassic_literal(ASPCore2Parser::Classic_literalContext *ctx){
	}
  virtual void exitClassic_literal(ASPCore2Parser::Classic_literalContext *ctx){
	}

  virtual void enterNaf_literals(ASPCore2Parser::Naf_literalsContext *ctx){
	}
  virtual void exitNaf_literals(ASPCore2Parser::Naf_literalsContext *ctx){
	}

  virtual void enterNaf_literal(ASPCore2Parser::Naf_literalContext *ctx){
	}
  virtual void exitNaf_literal(ASPCore2Parser::Naf_literalContext *ctx){
	}

  virtual void enterNaf_literal_aggregate(ASPCore2Parser::Naf_literal_aggregateContext *ctx){
	}
  virtual void exitNaf_literal_aggregate(ASPCore2Parser::Naf_literal_aggregateContext *ctx){
	}

  virtual void enterExistential_atom(ASPCore2Parser::Existential_atomContext *ctx){
	}
  virtual void exitExistential_atom(ASPCore2Parser::Existential_atomContext *ctx){
	}

  virtual void enterAtom(ASPCore2Parser::AtomContext *ctx){
	}
  virtual void exitAtom(ASPCore2Parser::AtomContext *ctx){
	}

  virtual void enterExtAtom(ASPCore2Parser::ExtAtomContext *ctx){
	}
  virtual void exitExtAtom(ASPCore2Parser::ExtAtomContext *ctx){
	}

  virtual void enterExtSemicol(ASPCore2Parser::ExtSemicolContext *ctx){
	}
  virtual void exitExtSemicol(ASPCore2Parser::ExtSemicolContext *ctx){
	}

  virtual void enterIdentifier(ASPCore2Parser::IdentifierContext *ctx){
	}
  virtual void exitIdentifier(ASPCore2Parser::IdentifierContext *ctx){
	}

  virtual void enterQuery(ASPCore2Parser::QueryContext *ctx){
	}
  virtual void exitQuery(ASPCore2Parser::QueryContext *ctx){
	}

  virtual void enterLower_guard_compare_aggregate(ASPCore2Parser::Lower_guard_compare_aggregateContext *ctx){
	}
  virtual void exitLower_guard_compare_aggregate(ASPCore2Parser::Lower_guard_compare_aggregateContext *ctx){
	}

  virtual void enterUpper_guard_compare_aggregate(ASPCore2Parser::Upper_guard_compare_aggregateContext *ctx){
	}
  virtual void exitUpper_guard_compare_aggregate(ASPCore2Parser::Upper_guard_compare_aggregateContext *ctx){
	}

  virtual void enterCompare_aggregate(ASPCore2Parser::Compare_aggregateContext *ctx){
	}
  virtual void exitCompare_aggregate(ASPCore2Parser::Compare_aggregateContext *ctx){
	}

  virtual void enterLower_guard_leftward_left_aggregate(ASPCore2Parser::Lower_guard_leftward_left_aggregateContext *ctx){
	}
  virtual void exitLower_guard_leftward_left_aggregate(ASPCore2Parser::Lower_guard_leftward_left_aggregateContext *ctx){
	}

  virtual void enterLeftward_left_aggregate(ASPCore2Parser::Leftward_left_aggregateContext *ctx){
	}
  virtual void exitLeftward_left_aggregate(ASPCore2Parser::Leftward_left_aggregateContext *ctx){
	}

  virtual void enterLeft_aggregate(ASPCore2Parser::Left_aggregateContext *ctx){
	}
  virtual void exitLeft_aggregate(ASPCore2Parser::Left_aggregateContext *ctx){
	}

  virtual void enterLower_guard_rightward_left_aggregate(ASPCore2Parser::Lower_guard_rightward_left_aggregateContext *ctx){
	}
  virtual void exitLower_guard_rightward_left_aggregate(ASPCore2Parser::Lower_guard_rightward_left_aggregateContext *ctx){
	}

  virtual void enterRightward_left_aggregate(ASPCore2Parser::Rightward_left_aggregateContext *ctx){
	}
  virtual void exitRightward_left_aggregate(ASPCore2Parser::Rightward_left_aggregateContext *ctx){
	}

  virtual void enterUpper_guard_leftward_right_aggregate(ASPCore2Parser::Upper_guard_leftward_right_aggregateContext *ctx){
	}
  virtual void exitUpper_guard_leftward_right_aggregate(ASPCore2Parser::Upper_guard_leftward_right_aggregateContext *ctx){
	}

  virtual void enterUpper_guard_rightward_right_aggregate(ASPCore2Parser::Upper_guard_rightward_right_aggregateContext *ctx){
	}
  virtual void exitUpper_guard_rightward_right_aggregate(ASPCore2Parser::Upper_guard_rightward_right_aggregateContext *ctx){
	}

  virtual void enterRight_aggregate(ASPCore2Parser::Right_aggregateContext *ctx){
	}
  virtual void exitRight_aggregate(ASPCore2Parser::Right_aggregateContext *ctx){
	}

  virtual void enterAggregate_atom(ASPCore2Parser::Aggregate_atomContext *ctx){
	}
  virtual void exitAggregate_atom(ASPCore2Parser::Aggregate_atomContext *ctx){
	}

  virtual void enterLeftwardop(ASPCore2Parser::LeftwardopContext *ctx){
	}
  virtual void exitLeftwardop(ASPCore2Parser::LeftwardopContext *ctx){
	}

  virtual void enterRightwardop(ASPCore2Parser::RightwardopContext *ctx){
	}
  virtual void exitRightwardop(ASPCore2Parser::RightwardopContext *ctx){
	}

  virtual void enterAggregate(ASPCore2Parser::AggregateContext *ctx){
	}
  virtual void exitAggregate(ASPCore2Parser::AggregateContext *ctx){
	}

  virtual void enterAggregate_elements(ASPCore2Parser::Aggregate_elementsContext *ctx){
	}
  virtual void exitAggregate_elements(ASPCore2Parser::Aggregate_elementsContext *ctx){
	}

  virtual void enterAggregate_element(ASPCore2Parser::Aggregate_elementContext *ctx){
	}
  virtual void exitAggregate_element(ASPCore2Parser::Aggregate_elementContext *ctx){
	}

  virtual void enterAggregate_function(ASPCore2Parser::Aggregate_functionContext *ctx){
	}
  virtual void exitAggregate_function(ASPCore2Parser::Aggregate_functionContext *ctx){
	}

  virtual void enterTerms(ASPCore2Parser::TermsContext *ctx){
	}
  virtual void exitTerms(ASPCore2Parser::TermsContext *ctx){
	}

  virtual void enterBasic_terms(ASPCore2Parser::Basic_termsContext *ctx){
	}
  virtual void exitBasic_terms(ASPCore2Parser::Basic_termsContext *ctx){
	}

  virtual void enterBuiltin_atom(ASPCore2Parser::Builtin_atomContext *ctx){
	}
  virtual void exitBuiltin_atom(ASPCore2Parser::Builtin_atomContext *ctx){
	}

  virtual void enterCompareop(ASPCore2Parser::CompareopContext *ctx){
	}
  virtual void exitCompareop(ASPCore2Parser::CompareopContext *ctx){
	}

  virtual void enterBinop(ASPCore2Parser::BinopContext *ctx){
	}
  virtual void exitBinop(ASPCore2Parser::BinopContext *ctx){
	}

  virtual void enterFactor(ASPCore2Parser::FactorContext *ctx){
	}
  virtual void exitFactor(ASPCore2Parser::FactorContext *ctx){
	}

  virtual void enterBasic_term(ASPCore2Parser::Basic_termContext *ctx){
	}
  virtual void exitBasic_term(ASPCore2Parser::Basic_termContext *ctx){
	}

  virtual void enterGround_term(ASPCore2Parser::Ground_termContext *ctx){
	}
  virtual void exitGround_term(ASPCore2Parser::Ground_termContext *ctx){
	}

  virtual void enterVariable_term(ASPCore2Parser::Variable_termContext *ctx){
	}
  virtual void exitVariable_term(ASPCore2Parser::Variable_termContext *ctx){
	}

  virtual void enterFunctional_term(ASPCore2Parser::Functional_termContext *ctx){
	}
  virtual void exitFunctional_term(ASPCore2Parser::Functional_termContext *ctx){
	}

  virtual void enterExpr(ASPCore2Parser::ExprContext *ctx){
	}
  virtual void exitExpr(ASPCore2Parser::ExprContext *ctx){
	}

  virtual void enterArithop2(ASPCore2Parser::Arithop2Context *ctx){
	}
  virtual void exitArithop2(ASPCore2Parser::Arithop2Context *ctx){
	}

  virtual void enterArithop1(ASPCore2Parser::Arithop1Context *ctx){
	}
  virtual void exitArithop1(ASPCore2Parser::Arithop1Context *ctx){
	}

  virtual void enterTerm_(ASPCore2Parser::Term_Context *ctx){
	}
  virtual void exitTerm_(ASPCore2Parser::Term_Context *ctx){
	}

  virtual void enterTerm__(ASPCore2Parser::Term__Context *ctx){
	}
  virtual void exitTerm__(ASPCore2Parser::Term__Context *ctx){
	}

  virtual void enterTerm(ASPCore2Parser::TermContext *ctx){
	}
  virtual void exitTerm(ASPCore2Parser::TermContext *ctx){
	}

  virtual void enterVars(ASPCore2Parser::VarsContext *ctx){
	}
  virtual void exitVars(ASPCore2Parser::VarsContext *ctx){
	}
    virtual void visitTerminal(antlr4::tree::TerminalNode *node) {
	}
    virtual void visitErrorNode(antlr4::tree::ErrorNode *node) {
	}
    virtual void enterEveryRule(antlr4::ParserRuleContext *ctx) {
	}
    virtual void exitEveryRule(antlr4::ParserRuleContext *ctx) {
	}
};
#endif