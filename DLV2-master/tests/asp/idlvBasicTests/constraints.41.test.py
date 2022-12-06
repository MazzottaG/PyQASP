input = """
% Inferences from constraints.

b | a.

x :- a.
y :- x.

:- a, y.
"""
output = """
{b}
"""
