# Generated from ASPCore2.g4 by ANTLR 4.11.1
from antlr4 import *
if __name__ is not None and "." in __name__:
    from .ASPCore2Parser import ASPCore2Parser
else:
    from ASPCore2Parser import ASPCore2Parser

# This class defines a complete listener for a parse tree produced by ASPCore2Parser.
class ASPCore2Listener(ParseTreeListener):
    
    def __init__(self,naf):
        super().__init__()
        self.NAF=naf
        self.reset()
    
    def printGraph(self):
        for pred in self.predicatesToId:
            print(pred,self.predicatesToId[pred])

        for node in self.graph:
            for adj in self.graph[node]:
                print(node,"->",adj)

    def reset(self):
        self.headPredicates=set()
        self.readingPredicate=False
        self.readingIdentifier=False
        self.readingDisjunction=False
        self.readingChoiceHead=False
        self.ruleCount=0
        self.predicateCount=1
        self.predicatesToId={}
        self.foundNegation=False
        self.head=[]
        self.body=[]
        self.graph={}

    def visitTerminal(self, node:TerminalNode):
        
        if node.symbol.type == self.NAF:
            self.foundNegation = True

        if self.readingPredicate and self.readingIdentifier:
            predicate=str(node)
            predId=None
            try:
                predId = self.predicatesToId[predicate]
            except KeyError:
                self.predicatesToId[predicate]=self.predicateCount
                predId = self.predicateCount
                self.predicateCount+=1

            if self.readingDisjunction or self.readingChoiceHead:
                self.headPredicates.add(str(node))
                self.head.append(predId)
            elif not self.foundNegation:
                self.body.append(predId)

    # Enter a parse tree produced by ASPCore2Parser#program.
    def enterProgram(self, ctx:ASPCore2Parser.ProgramContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#program.
    def exitProgram(self, ctx:ASPCore2Parser.ProgramContext):
        pass

    # Enter a parse tree produced by ASPCore2Parser#rule.
    def enterRule(self, ctx:ASPCore2Parser.RuleContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#rule.
    def exitRule(self, ctx:ASPCore2Parser.RuleContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#simple_rule.
    def enterSimple_rule(self, ctx:ASPCore2Parser.Simple_ruleContext):
        self.ruleCount+=1
        self.head=[]
        self.body=[]

    # Exit a parse tree produced by ASPCore2Parser#simple_rule.
    def exitSimple_rule(self, ctx:ASPCore2Parser.Simple_ruleContext):
        for bodyPred in self.body:
            for headPred in self.head:
                try:
                    self.graph[bodyPred].add(headPred)
                except KeyError:
                    self.graph[bodyPred]={headPred}


    # Enter a parse tree produced by ASPCore2Parser#head.
    def enterHead(self, ctx:ASPCore2Parser.HeadContext):
        self.readingHead=True

    # Exit a parse tree produced by ASPCore2Parser#head.
    def exitHead(self, ctx:ASPCore2Parser.HeadContext):
        self.readingHead=False


    # Enter a parse tree produced by ASPCore2Parser#body.
    def enterBody(self, ctx:ASPCore2Parser.BodyContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#body.
    def exitBody(self, ctx:ASPCore2Parser.BodyContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#weight_at_levels.
    def enterWeight_at_levels(self, ctx:ASPCore2Parser.Weight_at_levelsContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#weight_at_levels.
    def exitWeight_at_levels(self, ctx:ASPCore2Parser.Weight_at_levelsContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#levels_and_terms.
    def enterLevels_and_terms(self, ctx:ASPCore2Parser.Levels_and_termsContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#levels_and_terms.
    def exitLevels_and_terms(self, ctx:ASPCore2Parser.Levels_and_termsContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#disjunction.
    def enterDisjunction(self, ctx:ASPCore2Parser.DisjunctionContext):
        self.readingDisjunction=True

    # Exit a parse tree produced by ASPCore2Parser#disjunction.
    def exitDisjunction(self, ctx:ASPCore2Parser.DisjunctionContext):
        self.readingDisjunction=False


    # Enter a parse tree produced by ASPCore2Parser#conjunction.
    def enterConjunction(self, ctx:ASPCore2Parser.ConjunctionContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#conjunction.
    def exitConjunction(self, ctx:ASPCore2Parser.ConjunctionContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#choice_atom.
    def enterChoice_atom(self, ctx:ASPCore2Parser.Choice_atomContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#choice_atom.
    def exitChoice_atom(self, ctx:ASPCore2Parser.Choice_atomContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#lower_guard.
    def enterLower_guard(self, ctx:ASPCore2Parser.Lower_guardContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#lower_guard.
    def exitLower_guard(self, ctx:ASPCore2Parser.Lower_guardContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#upper_guard.
    def enterUpper_guard(self, ctx:ASPCore2Parser.Upper_guardContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#upper_guard.
    def exitUpper_guard(self, ctx:ASPCore2Parser.Upper_guardContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#choice_elements.
    def enterChoice_elements(self, ctx:ASPCore2Parser.Choice_elementsContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#choice_elements.
    def exitChoice_elements(self, ctx:ASPCore2Parser.Choice_elementsContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#choice_element.
    def enterChoice_element(self, ctx:ASPCore2Parser.Choice_elementContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#choice_element.
    def exitChoice_element(self, ctx:ASPCore2Parser.Choice_elementContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#choice_element_atom.
    def enterChoice_element_atom(self, ctx:ASPCore2Parser.Choice_element_atomContext):
        self.readingChoiceHead=True

    # Exit a parse tree produced by ASPCore2Parser#choice_element_atom.
    def exitChoice_element_atom(self, ctx:ASPCore2Parser.Choice_element_atomContext):
        self.readingChoiceHead=False


    # Enter a parse tree produced by ASPCore2Parser#choice_elements_literals.
    def enterChoice_elements_literals(self, ctx:ASPCore2Parser.Choice_elements_literalsContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#choice_elements_literals.
    def exitChoice_elements_literals(self, ctx:ASPCore2Parser.Choice_elements_literalsContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#naf_literals.
    def enterNaf_literals(self, ctx:ASPCore2Parser.Naf_literalsContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#naf_literals.
    def exitNaf_literals(self, ctx:ASPCore2Parser.Naf_literalsContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#naf_literal.
    def enterNaf_literal(self, ctx:ASPCore2Parser.Naf_literalContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#naf_literal.
    def exitNaf_literal(self, ctx:ASPCore2Parser.Naf_literalContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#naf_literal_aggregate.
    def enterNaf_literal_aggregate(self, ctx:ASPCore2Parser.Naf_literal_aggregateContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#naf_literal_aggregate.
    def exitNaf_literal_aggregate(self, ctx:ASPCore2Parser.Naf_literal_aggregateContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#existential_atom.
    def enterExistential_atom(self, ctx:ASPCore2Parser.Existential_atomContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#existential_atom.
    def exitExistential_atom(self, ctx:ASPCore2Parser.Existential_atomContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#classic_literal.
    def enterClassic_literal(self, ctx:ASPCore2Parser.Classic_literalContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#classic_literal.
    def exitClassic_literal(self, ctx:ASPCore2Parser.Classic_literalContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#atom.
    def enterAtom(self, ctx:ASPCore2Parser.AtomContext):
        self.readingPredicate=True

    # Exit a parse tree produced by ASPCore2Parser#atom.
    def exitAtom(self, ctx:ASPCore2Parser.AtomContext):
        pass

    # Enter a parse tree produced by ASPCore2Parser#extAtom.
    def enterExtAtom(self, ctx:ASPCore2Parser.ExtAtomContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#extAtom.
    def exitExtAtom(self, ctx:ASPCore2Parser.ExtAtomContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#extSemicol.
    def enterExtSemicol(self, ctx:ASPCore2Parser.ExtSemicolContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#extSemicol.
    def exitExtSemicol(self, ctx:ASPCore2Parser.ExtSemicolContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#terms.
    def enterTerms(self, ctx:ASPCore2Parser.TermsContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#terms.
    def exitTerms(self, ctx:ASPCore2Parser.TermsContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#basic_terms.
    def enterBasic_terms(self, ctx:ASPCore2Parser.Basic_termsContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#basic_terms.
    def exitBasic_terms(self, ctx:ASPCore2Parser.Basic_termsContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#builtin_atom.
    def enterBuiltin_atom(self, ctx:ASPCore2Parser.Builtin_atomContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#builtin_atom.
    def exitBuiltin_atom(self, ctx:ASPCore2Parser.Builtin_atomContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#compareop.
    def enterCompareop(self, ctx:ASPCore2Parser.CompareopContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#compareop.
    def exitCompareop(self, ctx:ASPCore2Parser.CompareopContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#binop.
    def enterBinop(self, ctx:ASPCore2Parser.BinopContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#binop.
    def exitBinop(self, ctx:ASPCore2Parser.BinopContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#arithop1.
    def enterArithop1(self, ctx:ASPCore2Parser.Arithop1Context):
        pass

    # Exit a parse tree produced by ASPCore2Parser#arithop1.
    def exitArithop1(self, ctx:ASPCore2Parser.Arithop1Context):
        pass


    # Enter a parse tree produced by ASPCore2Parser#arithop2.
    def enterArithop2(self, ctx:ASPCore2Parser.Arithop2Context):
        pass

    # Exit a parse tree produced by ASPCore2Parser#arithop2.
    def exitArithop2(self, ctx:ASPCore2Parser.Arithop2Context):
        pass


    # Enter a parse tree produced by ASPCore2Parser#term_.
    def enterTerm_(self, ctx:ASPCore2Parser.Term_Context):
        pass

    # Exit a parse tree produced by ASPCore2Parser#term_.
    def exitTerm_(self, ctx:ASPCore2Parser.Term_Context):
        pass


    # Enter a parse tree produced by ASPCore2Parser#term__.
    def enterTerm__(self, ctx:ASPCore2Parser.Term__Context):
        pass

    # Exit a parse tree produced by ASPCore2Parser#term__.
    def exitTerm__(self, ctx:ASPCore2Parser.Term__Context):
        pass


    # Enter a parse tree produced by ASPCore2Parser#term.
    def enterTerm(self, ctx:ASPCore2Parser.TermContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#term.
    def exitTerm(self, ctx:ASPCore2Parser.TermContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#expr.
    def enterExpr(self, ctx:ASPCore2Parser.ExprContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#expr.
    def exitExpr(self, ctx:ASPCore2Parser.ExprContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#factor.
    def enterFactor(self, ctx:ASPCore2Parser.FactorContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#factor.
    def exitFactor(self, ctx:ASPCore2Parser.FactorContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#basic_term.
    def enterBasic_term(self, ctx:ASPCore2Parser.Basic_termContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#basic_term.
    def exitBasic_term(self, ctx:ASPCore2Parser.Basic_termContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#ground_term.
    def enterGround_term(self, ctx:ASPCore2Parser.Ground_termContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#ground_term.
    def exitGround_term(self, ctx:ASPCore2Parser.Ground_termContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#variable_term.
    def enterVariable_term(self, ctx:ASPCore2Parser.Variable_termContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#variable_term.
    def exitVariable_term(self, ctx:ASPCore2Parser.Variable_termContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#functional_term.
    def enterFunctional_term(self, ctx:ASPCore2Parser.Functional_termContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#functional_term.
    def exitFunctional_term(self, ctx:ASPCore2Parser.Functional_termContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#vars.
    def enterVars(self, ctx:ASPCore2Parser.VarsContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#vars.
    def exitVars(self, ctx:ASPCore2Parser.VarsContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#identifier.
    def enterIdentifier(self, ctx:ASPCore2Parser.IdentifierContext):
        self.readingIdentifier=True

    # Exit a parse tree produced by ASPCore2Parser#identifier.
    def exitIdentifier(self, ctx:ASPCore2Parser.IdentifierContext):
        self.readingIdentifier=False
        if self.readingPredicate:
            self.readingPredicate=False
            self.foundNegation=False



    # Enter a parse tree produced by ASPCore2Parser#directive.
    def enterDirective(self, ctx:ASPCore2Parser.DirectiveContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#directive.
    def exitDirective(self, ctx:ASPCore2Parser.DirectiveContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#query.
    def enterQuery(self, ctx:ASPCore2Parser.QueryContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#query.
    def exitQuery(self, ctx:ASPCore2Parser.QueryContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#lower_guard_compare_aggregate.
    def enterLower_guard_compare_aggregate(self, ctx:ASPCore2Parser.Lower_guard_compare_aggregateContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#lower_guard_compare_aggregate.
    def exitLower_guard_compare_aggregate(self, ctx:ASPCore2Parser.Lower_guard_compare_aggregateContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#upper_guard_compare_aggregate.
    def enterUpper_guard_compare_aggregate(self, ctx:ASPCore2Parser.Upper_guard_compare_aggregateContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#upper_guard_compare_aggregate.
    def exitUpper_guard_compare_aggregate(self, ctx:ASPCore2Parser.Upper_guard_compare_aggregateContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#compare_aggregate.
    def enterCompare_aggregate(self, ctx:ASPCore2Parser.Compare_aggregateContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#compare_aggregate.
    def exitCompare_aggregate(self, ctx:ASPCore2Parser.Compare_aggregateContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#lower_guard_leftward_left_aggregate.
    def enterLower_guard_leftward_left_aggregate(self, ctx:ASPCore2Parser.Lower_guard_leftward_left_aggregateContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#lower_guard_leftward_left_aggregate.
    def exitLower_guard_leftward_left_aggregate(self, ctx:ASPCore2Parser.Lower_guard_leftward_left_aggregateContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#leftward_left_aggregate.
    def enterLeftward_left_aggregate(self, ctx:ASPCore2Parser.Leftward_left_aggregateContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#leftward_left_aggregate.
    def exitLeftward_left_aggregate(self, ctx:ASPCore2Parser.Leftward_left_aggregateContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#left_aggregate.
    def enterLeft_aggregate(self, ctx:ASPCore2Parser.Left_aggregateContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#left_aggregate.
    def exitLeft_aggregate(self, ctx:ASPCore2Parser.Left_aggregateContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#lower_guard_rightward_left_aggregate.
    def enterLower_guard_rightward_left_aggregate(self, ctx:ASPCore2Parser.Lower_guard_rightward_left_aggregateContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#lower_guard_rightward_left_aggregate.
    def exitLower_guard_rightward_left_aggregate(self, ctx:ASPCore2Parser.Lower_guard_rightward_left_aggregateContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#rightward_left_aggregate.
    def enterRightward_left_aggregate(self, ctx:ASPCore2Parser.Rightward_left_aggregateContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#rightward_left_aggregate.
    def exitRightward_left_aggregate(self, ctx:ASPCore2Parser.Rightward_left_aggregateContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#upper_guard_leftward_right_aggregate.
    def enterUpper_guard_leftward_right_aggregate(self, ctx:ASPCore2Parser.Upper_guard_leftward_right_aggregateContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#upper_guard_leftward_right_aggregate.
    def exitUpper_guard_leftward_right_aggregate(self, ctx:ASPCore2Parser.Upper_guard_leftward_right_aggregateContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#upper_guard_rightward_right_aggregate.
    def enterUpper_guard_rightward_right_aggregate(self, ctx:ASPCore2Parser.Upper_guard_rightward_right_aggregateContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#upper_guard_rightward_right_aggregate.
    def exitUpper_guard_rightward_right_aggregate(self, ctx:ASPCore2Parser.Upper_guard_rightward_right_aggregateContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#right_aggregate.
    def enterRight_aggregate(self, ctx:ASPCore2Parser.Right_aggregateContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#right_aggregate.
    def exitRight_aggregate(self, ctx:ASPCore2Parser.Right_aggregateContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#aggregate_atom.
    def enterAggregate_atom(self, ctx:ASPCore2Parser.Aggregate_atomContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#aggregate_atom.
    def exitAggregate_atom(self, ctx:ASPCore2Parser.Aggregate_atomContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#leftwardop.
    def enterLeftwardop(self, ctx:ASPCore2Parser.LeftwardopContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#leftwardop.
    def exitLeftwardop(self, ctx:ASPCore2Parser.LeftwardopContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#rightwardop.
    def enterRightwardop(self, ctx:ASPCore2Parser.RightwardopContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#rightwardop.
    def exitRightwardop(self, ctx:ASPCore2Parser.RightwardopContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#aggregate.
    def enterAggregate(self, ctx:ASPCore2Parser.AggregateContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#aggregate.
    def exitAggregate(self, ctx:ASPCore2Parser.AggregateContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#aggregate_elements.
    def enterAggregate_elements(self, ctx:ASPCore2Parser.Aggregate_elementsContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#aggregate_elements.
    def exitAggregate_elements(self, ctx:ASPCore2Parser.Aggregate_elementsContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#aggregate_element.
    def enterAggregate_element(self, ctx:ASPCore2Parser.Aggregate_elementContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#aggregate_element.
    def exitAggregate_element(self, ctx:ASPCore2Parser.Aggregate_elementContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#aggregate_function.
    def enterAggregate_function(self, ctx:ASPCore2Parser.Aggregate_functionContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#aggregate_function.
    def exitAggregate_function(self, ctx:ASPCore2Parser.Aggregate_functionContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_global.
    def enterAnnotation_global(self, ctx:ASPCore2Parser.Annotation_globalContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_global.
    def exitAnnotation_global(self, ctx:ASPCore2Parser.Annotation_globalContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotations_rule.
    def enterAnnotations_rule(self, ctx:ASPCore2Parser.Annotations_ruleContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotations_rule.
    def exitAnnotations_rule(self, ctx:ASPCore2Parser.Annotations_ruleContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_rule.
    def enterAnnotation_rule(self, ctx:ASPCore2Parser.Annotation_ruleContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_rule.
    def exitAnnotation_rule(self, ctx:ASPCore2Parser.Annotation_ruleContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_rule_to_decompose.
    def enterAnnotation_rule_to_decompose(self, ctx:ASPCore2Parser.Annotation_rule_to_decomposeContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_rule_to_decompose.
    def exitAnnotation_rule_to_decompose(self, ctx:ASPCore2Parser.Annotation_rule_to_decomposeContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_rule_to_not_decompose.
    def enterAnnotation_rule_to_not_decompose(self, ctx:ASPCore2Parser.Annotation_rule_to_not_decomposeContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_rule_to_not_decompose.
    def exitAnnotation_rule_to_not_decompose(self, ctx:ASPCore2Parser.Annotation_rule_to_not_decomposeContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_rule_ordering.
    def enterAnnotation_rule_ordering(self, ctx:ASPCore2Parser.Annotation_rule_orderingContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_rule_ordering.
    def exitAnnotation_rule_ordering(self, ctx:ASPCore2Parser.Annotation_rule_orderingContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_global_ordering.
    def enterAnnotation_global_ordering(self, ctx:ASPCore2Parser.Annotation_global_orderingContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_global_ordering.
    def exitAnnotation_global_ordering(self, ctx:ASPCore2Parser.Annotation_global_orderingContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_global_extatom_conversion.
    def enterAnnotation_global_extatom_conversion(self, ctx:ASPCore2Parser.Annotation_global_extatom_conversionContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_global_extatom_conversion.
    def exitAnnotation_global_extatom_conversion(self, ctx:ASPCore2Parser.Annotation_global_extatom_conversionContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_extpredicate.
    def enterAnnotation_extpredicate(self, ctx:ASPCore2Parser.Annotation_extpredicateContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_extpredicate.
    def exitAnnotation_extpredicate(self, ctx:ASPCore2Parser.Annotation_extpredicateContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#extatom_conv_types.
    def enterExtatom_conv_types(self, ctx:ASPCore2Parser.Extatom_conv_typesContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#extatom_conv_types.
    def exitExtatom_conv_types(self, ctx:ASPCore2Parser.Extatom_conv_typesContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#extatom_conv_type.
    def enterExtatom_conv_type(self, ctx:ASPCore2Parser.Extatom_conv_typeContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#extatom_conv_type.
    def exitExtatom_conv_type(self, ctx:ASPCore2Parser.Extatom_conv_typeContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#rule_ordering_type.
    def enterRule_ordering_type(self, ctx:ASPCore2Parser.Rule_ordering_typeContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#rule_ordering_type.
    def exitRule_ordering_type(self, ctx:ASPCore2Parser.Rule_ordering_typeContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#global_ordering_type.
    def enterGlobal_ordering_type(self, ctx:ASPCore2Parser.Global_ordering_typeContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#global_ordering_type.
    def exitGlobal_ordering_type(self, ctx:ASPCore2Parser.Global_ordering_typeContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_rule_atom_indexed.
    def enterAnnotation_rule_atom_indexed(self, ctx:ASPCore2Parser.Annotation_rule_atom_indexedContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_rule_atom_indexed.
    def exitAnnotation_rule_atom_indexed(self, ctx:ASPCore2Parser.Annotation_rule_atom_indexedContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_global_atom_indexed.
    def enterAnnotation_global_atom_indexed(self, ctx:ASPCore2Parser.Annotation_global_atom_indexedContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_global_atom_indexed.
    def exitAnnotation_global_atom_indexed(self, ctx:ASPCore2Parser.Annotation_global_atom_indexedContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#rule_indexing_arguments.
    def enterRule_indexing_arguments(self, ctx:ASPCore2Parser.Rule_indexing_argumentsContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#rule_indexing_arguments.
    def exitRule_indexing_arguments(self, ctx:ASPCore2Parser.Rule_indexing_argumentsContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#rule_naf_literal_annotation.
    def enterRule_naf_literal_annotation(self, ctx:ASPCore2Parser.Rule_naf_literal_annotationContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#rule_naf_literal_annotation.
    def exitRule_naf_literal_annotation(self, ctx:ASPCore2Parser.Rule_naf_literal_annotationContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#global_indexing_arguments.
    def enterGlobal_indexing_arguments(self, ctx:ASPCore2Parser.Global_indexing_argumentsContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#global_indexing_arguments.
    def exitGlobal_indexing_arguments(self, ctx:ASPCore2Parser.Global_indexing_argumentsContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#global_naf_literal_annotation.
    def enterGlobal_naf_literal_annotation(self, ctx:ASPCore2Parser.Global_naf_literal_annotationContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#global_naf_literal_annotation.
    def exitGlobal_naf_literal_annotation(self, ctx:ASPCore2Parser.Global_naf_literal_annotationContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#rule_atom_annotation_before.
    def enterRule_atom_annotation_before(self, ctx:ASPCore2Parser.Rule_atom_annotation_beforeContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#rule_atom_annotation_before.
    def exitRule_atom_annotation_before(self, ctx:ASPCore2Parser.Rule_atom_annotation_beforeContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#global_atom_annotation_before.
    def enterGlobal_atom_annotation_before(self, ctx:ASPCore2Parser.Global_atom_annotation_beforeContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#global_atom_annotation_before.
    def exitGlobal_atom_annotation_before(self, ctx:ASPCore2Parser.Global_atom_annotation_beforeContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#rule_atom_annotation_after.
    def enterRule_atom_annotation_after(self, ctx:ASPCore2Parser.Rule_atom_annotation_afterContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#rule_atom_annotation_after.
    def exitRule_atom_annotation_after(self, ctx:ASPCore2Parser.Rule_atom_annotation_afterContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#global_atom_annotation_after.
    def enterGlobal_atom_annotation_after(self, ctx:ASPCore2Parser.Global_atom_annotation_afterContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#global_atom_annotation_after.
    def exitGlobal_atom_annotation_after(self, ctx:ASPCore2Parser.Global_atom_annotation_afterContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#rule_atoms_annotation_before.
    def enterRule_atoms_annotation_before(self, ctx:ASPCore2Parser.Rule_atoms_annotation_beforeContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#rule_atoms_annotation_before.
    def exitRule_atoms_annotation_before(self, ctx:ASPCore2Parser.Rule_atoms_annotation_beforeContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#global_atoms_annotation_before.
    def enterGlobal_atoms_annotation_before(self, ctx:ASPCore2Parser.Global_atoms_annotation_beforeContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#global_atoms_annotation_before.
    def exitGlobal_atoms_annotation_before(self, ctx:ASPCore2Parser.Global_atoms_annotation_beforeContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#rule_atoms_annotation_after.
    def enterRule_atoms_annotation_after(self, ctx:ASPCore2Parser.Rule_atoms_annotation_afterContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#rule_atoms_annotation_after.
    def exitRule_atoms_annotation_after(self, ctx:ASPCore2Parser.Rule_atoms_annotation_afterContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#global_atoms_annotation_after.
    def enterGlobal_atoms_annotation_after(self, ctx:ASPCore2Parser.Global_atoms_annotation_afterContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#global_atoms_annotation_after.
    def exitGlobal_atoms_annotation_after(self, ctx:ASPCore2Parser.Global_atoms_annotation_afterContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_rule_partial_order_begin.
    def enterAnnotation_rule_partial_order_begin(self, ctx:ASPCore2Parser.Annotation_rule_partial_order_beginContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_rule_partial_order_begin.
    def exitAnnotation_rule_partial_order_begin(self, ctx:ASPCore2Parser.Annotation_rule_partial_order_beginContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_global_partial_order_begin.
    def enterAnnotation_global_partial_order_begin(self, ctx:ASPCore2Parser.Annotation_global_partial_order_beginContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_global_partial_order_begin.
    def exitAnnotation_global_partial_order_begin(self, ctx:ASPCore2Parser.Annotation_global_partial_order_beginContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_rule_partial_order.
    def enterAnnotation_rule_partial_order(self, ctx:ASPCore2Parser.Annotation_rule_partial_orderContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_rule_partial_order.
    def exitAnnotation_rule_partial_order(self, ctx:ASPCore2Parser.Annotation_rule_partial_orderContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_global_partial_order.
    def enterAnnotation_global_partial_order(self, ctx:ASPCore2Parser.Annotation_global_partial_orderContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_global_partial_order.
    def exitAnnotation_global_partial_order(self, ctx:ASPCore2Parser.Annotation_global_partial_orderContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_rule_projection.
    def enterAnnotation_rule_projection(self, ctx:ASPCore2Parser.Annotation_rule_projectionContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_rule_projection.
    def exitAnnotation_rule_projection(self, ctx:ASPCore2Parser.Annotation_rule_projectionContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_rule_projection_value.
    def enterAnnotation_rule_projection_value(self, ctx:ASPCore2Parser.Annotation_rule_projection_valueContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_rule_projection_value.
    def exitAnnotation_rule_projection_value(self, ctx:ASPCore2Parser.Annotation_rule_projection_valueContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_rule_rewriting_arith.
    def enterAnnotation_rule_rewriting_arith(self, ctx:ASPCore2Parser.Annotation_rule_rewriting_arithContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_rule_rewriting_arith.
    def exitAnnotation_rule_rewriting_arith(self, ctx:ASPCore2Parser.Annotation_rule_rewriting_arithContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_rule_align_substitutions.
    def enterAnnotation_rule_align_substitutions(self, ctx:ASPCore2Parser.Annotation_rule_align_substitutionsContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_rule_align_substitutions.
    def exitAnnotation_rule_align_substitutions(self, ctx:ASPCore2Parser.Annotation_rule_align_substitutionsContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_rule_look_ahead.
    def enterAnnotation_rule_look_ahead(self, ctx:ASPCore2Parser.Annotation_rule_look_aheadContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_rule_look_ahead.
    def exitAnnotation_rule_look_ahead(self, ctx:ASPCore2Parser.Annotation_rule_look_aheadContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_global_wasp_propagator.
    def enterAnnotation_global_wasp_propagator(self, ctx:ASPCore2Parser.Annotation_global_wasp_propagatorContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_global_wasp_propagator.
    def exitAnnotation_global_wasp_propagator(self, ctx:ASPCore2Parser.Annotation_global_wasp_propagatorContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_global_wasp_propagator_begin.
    def enterAnnotation_global_wasp_propagator_begin(self, ctx:ASPCore2Parser.Annotation_global_wasp_propagator_beginContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_global_wasp_propagator_begin.
    def exitAnnotation_global_wasp_propagator_begin(self, ctx:ASPCore2Parser.Annotation_global_wasp_propagator_beginContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_global_wasp_heuristic.
    def enterAnnotation_global_wasp_heuristic(self, ctx:ASPCore2Parser.Annotation_global_wasp_heuristicContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_global_wasp_heuristic.
    def exitAnnotation_global_wasp_heuristic(self, ctx:ASPCore2Parser.Annotation_global_wasp_heuristicContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#annotation_global_wasp_heuristic_begin.
    def enterAnnotation_global_wasp_heuristic_begin(self, ctx:ASPCore2Parser.Annotation_global_wasp_heuristic_beginContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#annotation_global_wasp_heuristic_begin.
    def exitAnnotation_global_wasp_heuristic_begin(self, ctx:ASPCore2Parser.Annotation_global_wasp_heuristic_beginContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#heuristic_python_file.
    def enterHeuristic_python_file(self, ctx:ASPCore2Parser.Heuristic_python_fileContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#heuristic_python_file.
    def exitHeuristic_python_file(self, ctx:ASPCore2Parser.Heuristic_python_fileContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#aggregate_element_heuristic.
    def enterAggregate_element_heuristic(self, ctx:ASPCore2Parser.Aggregate_element_heuristicContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#aggregate_element_heuristic.
    def exitAggregate_element_heuristic(self, ctx:ASPCore2Parser.Aggregate_element_heuristicContext):
        pass


    # Enter a parse tree produced by ASPCore2Parser#aggregate_elements_heuristic.
    def enterAggregate_elements_heuristic(self, ctx:ASPCore2Parser.Aggregate_elements_heuristicContext):
        pass

    # Exit a parse tree produced by ASPCore2Parser#aggregate_elements_heuristic.
    def exitAggregate_elements_heuristic(self, ctx:ASPCore2Parser.Aggregate_elements_heuristicContext):
        pass



del ASPCore2Parser