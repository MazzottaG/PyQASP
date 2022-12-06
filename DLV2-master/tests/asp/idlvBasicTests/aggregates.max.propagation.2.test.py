input = """

a(1) | b(1).

c(X) :- b(X).

ok :- #max{V:c(V)} = 1. 
"""
output = """
{a(1)}
{b(1), c(1), ok}
"""
