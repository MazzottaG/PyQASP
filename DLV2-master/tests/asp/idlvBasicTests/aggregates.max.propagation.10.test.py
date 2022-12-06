input = """
c | nc.

a(1) :- not c.
a(0) :- c.
good :- #max{ X: a(X) } = 0.

"""
output = """
{a(1), nc}
{a(0), c, good}
"""
