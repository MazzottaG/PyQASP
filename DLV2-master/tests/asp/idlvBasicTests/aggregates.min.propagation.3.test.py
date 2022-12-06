input = """
c | nc.

a(0) :- not c.
a(1) :- c.
good :- #min{ X: a(X) } = 1.

"""
output = """
{a(0), nc}
{a(1), c, good}
"""
