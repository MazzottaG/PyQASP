input = """
a | b.

foo(X,Y) :- X1 = Y + 1, a, X2 = X1 + 0, X = X2 + 0. % #int(Y), X1 = Y + 1, a, X2 = X1 + 0, X = X2 + 0.
"""
output = """
"""
