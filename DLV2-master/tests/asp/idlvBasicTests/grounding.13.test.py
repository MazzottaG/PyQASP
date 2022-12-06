input = """
a(0). a(1).
b :- a(X), not c(X).
c(1) | c(0) :- b.
"""
output = """
{a(0), a(1), b, c(1)}
{a(0), a(1), b, c(0)}
"""
