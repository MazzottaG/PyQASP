input = """
a | b.
c | d :- not a.
"""
output = """
{b, d}
{b, c}
{a}
"""
