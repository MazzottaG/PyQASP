input = """
a(X):- edge(X,Y), b(X), node(Y).
b(X):- a(X).

a(X):- node(X).
"""
output = """
{}
"""
