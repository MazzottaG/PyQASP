input = """
a(X) :- f(X), X<Y, g(Y).
a(X) :- f(X), X<Y.
a(X) :- X<Y, g(Y).
"""
output = """
"""
