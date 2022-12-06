input = """
a | b :- d.
a | b :- c.

d :- not c.
c :- not d.

:- d, not c.
"""
output = """
{a, c}
{b, c}
"""
