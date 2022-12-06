input = """
% run with option --depgraph
% the component of a must be cyclic

b(1).
a(X, Y) :- a(X, X), b(Y).

"""
output = """
{b(1)}
"""
