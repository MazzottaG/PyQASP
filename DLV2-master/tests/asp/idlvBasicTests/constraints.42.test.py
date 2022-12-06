input = """
% Inferences from constraints.

b | a.

x :- a.
y :- x.

:- not a, y.
"""
output = """
{b}
{a, x, y}
"""
