input = """
int(1..2).
n(1).
a(X) :- int(X), X > 0, X = #count{Z : n(Z) }.

"""
output = """
{a(1), int(1), int(2), n(1)}
"""
