input = """
% a and b are the top-level PTs.
a | b.

% After taking a, c and d are the PTs and each of them leads to a model.
c | d :- a.

% After backtracking, not a is propagated and, among others, b is derived.
% So g and h are additional PTs in the top level now. 
g | h :- b, not a.

% g directly leads to an inconsistency, and so does h.
% Also not g and not h lead to an inconsistency!
% So, we should immediately backtrack.
:- f.
f :- g, not h.
f :- h, not g.

% i and j should never be derived, since then also one of g or h must be true.
i | j :- b, not a.

"""
output = """
{a, d}
{a, c}
"""
