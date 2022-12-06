input = """
p(2) :- #count{Y:q(Y)}=X, p(X).
q(1).
p(1).
p(a).
"""
output = """
{p(1), p(2), p(a), q(1)}
"""
