input = """
p(a) | p(b).
q(X) :- #count{ Y:p(Y) } = X.

"""
output = """
{p(a), q(1)}
{p(b), q(1)}
"""
