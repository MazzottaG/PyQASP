input = """
int(1).
n(1).
a(X) :- int(X), X > 0, X < #count{Z : n(Z) }.

"""
output = """
{int(1), n(1)}
"""
