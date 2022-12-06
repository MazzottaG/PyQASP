input = """
a | b :- c.
-b :- a.

a | c.
"""
output = """
{b, c}
{-b, a}
"""
