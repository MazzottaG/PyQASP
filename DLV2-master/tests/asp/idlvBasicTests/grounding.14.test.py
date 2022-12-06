input = """
a(0). a(1).
c(0).
b :- a(X), not c(X).
c(2) :- b.
"""
output = """
{a(0), a(1), b, c(0), c(2)}
"""
