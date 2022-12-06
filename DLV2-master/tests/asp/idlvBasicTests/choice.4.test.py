input = """
a | b :- w(X).
w(1) | w(2).
"""
output = """
{b, w(2)}
{b, w(1)}
{a, w(1)}
{a, w(2)}
"""
