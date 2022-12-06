input = """
a | b.
foo(X,Y) :- a,  X1 = Y + 1, X = X1 + 0. %#int(Y), X1 = Y + 1, X = X1 + 0.
bar(X,Y) :-  X1 = Y + 1, X = X1 + 0, b. %#int(Y), X1 = Y + 1, X = X1 + 0, b.
"""
output = """
"""
