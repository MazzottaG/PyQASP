input = """
a(X):- edge(X,Y), b(X).
b(X):- a(X).
a(X):-node(X).

c(X):-d(X).
d(X):-c(X).

c(X):-edge(X,_).
"""
output = """
{}
"""
