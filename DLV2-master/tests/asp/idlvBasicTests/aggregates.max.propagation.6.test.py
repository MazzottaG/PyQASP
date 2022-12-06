input = """
% Only undefined auxiliary atoms.
a(1) | b(1).
a(2) | b(2).
ok1 :- #max{V:a(V)} = 3.

x | -x.

% Only c(0) undefined.
c(0) :- a(1).
ok2 :- #max{V:c(V)} = 0.
		
"""
output = """
{-x, a(1), b(2), c(0), ok2}
{-x, a(1), a(2), c(0), ok2}
{a(1), b(2), c(0), ok2, x}
{a(1), a(2), c(0), ok2, x}
{-x, b(1), b(2)}
{b(1), b(2), x}
{-x, a(2), b(1)}
{a(2), b(1), x}
"""
