input = """
a :- b.
b | c.

d.
:- d, a.
"""
output = """
{c, d}
"""
