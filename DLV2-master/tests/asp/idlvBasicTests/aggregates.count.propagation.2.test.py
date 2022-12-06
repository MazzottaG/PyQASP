input = """
% Count up the EDB predicates during grounding.
a(1).
a(2).
a(3).
a(4).

b(5) | b(6).     

% Count up the IDB predicates during the propagation.
a(X) :- b(X).

% If the count value is correct, the 'ok' predicate should be true.
ok :- #count{V : a(V)} = 5.
"""
output = """
{a(1), a(2), a(3), a(4), a(5), b(5), ok}
{a(1), a(2), a(3), a(4), a(6), b(6), ok}
"""
