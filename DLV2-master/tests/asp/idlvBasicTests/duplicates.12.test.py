input = """
a | b.

s :- not a.
s :- not b.
"""
output = """
{a, s}
{b, s}
"""
