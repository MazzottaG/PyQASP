input = """
a | a :- b.

b | c.

"""
output = """
{a, b}
{c}
"""
