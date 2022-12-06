input = """
% This bug was found by Wolfgang Faber.
% It involved the grounding and it caused a core dumped inside
% the main program during the initialization of groundingCost.
  
a.
%:-a.

% The output of the result is printed by running dlv
% specifing the -silent option only.
"""
output = """
{a}
"""
