input = """
a | b :- d.

c | d.

:- c, not d.
"""
output = """
{b, d}
{a, d}
"""
