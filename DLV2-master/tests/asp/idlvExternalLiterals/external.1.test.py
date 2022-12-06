input="""
a(X):-b(X).
b(1..2).
"""
output="""
{a(1), a(2), b(1), b(2)}
"""
