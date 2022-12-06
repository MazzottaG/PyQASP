input = """
:- not a.
a :- c.
c | b | e.
c | b.
d | e.
:- a, d, e.
"""
output = """
{a, c, d}
{a, c, e}
"""
