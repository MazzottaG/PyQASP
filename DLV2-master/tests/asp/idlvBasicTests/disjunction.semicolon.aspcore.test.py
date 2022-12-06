input = """
f(one).

f(two) | f(three) :- f(one).
"""
output = """
{f(one), f(two)}
{f(one), f(three)}
"""
