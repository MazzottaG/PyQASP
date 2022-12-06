input = """
c | d :- not a, not g.
f :- a, not f.
a | b.
g :- c.
"""
output = """
{b, d}
"""
