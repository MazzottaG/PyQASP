input = """
% Only undefined auxiliary atoms.
a(1) | b(1).
a(2) | b(2).
ok1 :- #max{V:a(V)} = 3.

% Only c(0) undefined.
c(0) :- a(1).
ok2 :- #max{V:c(V)} = 0. 


"""
output = """
{a(1), b(2), c(0), ok2}
{a(1), a(2), c(0), ok2}
{b(1), b(2)}
{a(2), b(1)}
"""
