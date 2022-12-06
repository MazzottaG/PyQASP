input = """
f(1).
a(X) :- 1=2+x, f(X). %+(1,2,x), f(X).
"""
output = """
{f(1)}
"""
