input = """
a :- a.
a | b :- not a.

"""
output = """
{b}
"""
