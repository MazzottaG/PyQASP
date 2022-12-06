input = """
% Only undefined auxiliary atoms.
a(1) | b(1).
a(2) | b(2).
ok1 :- #max{V:a(V)} = 2.
"""
output = """
{a(1), b(2)}
{a(1), a(2), ok1}
{b(1), b(2)}
{a(2), b(1), ok1}
"""
