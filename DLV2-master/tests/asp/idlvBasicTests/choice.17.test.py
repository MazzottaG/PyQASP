input = """
a | b.
c | d.
a | c.

:- not c, a.
"""
output = """
{b, c}
{a, c}
"""
