input = """
a(1) | a(2).
b(1).
:- b(N), not #count{ V : a(V) } = N.
"""
output = """
{a(1), b(1)}
{a(2), b(1)}
"""
