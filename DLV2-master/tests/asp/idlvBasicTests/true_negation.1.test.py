input = """
true | -a :- not b.
true |  b :- not -a.

-true.
"""
output = """
{-a, -true}
{-true, b}
"""
