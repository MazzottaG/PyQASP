input = """
a(X,Y):- colour(X), node(Y), not node(X).
b(X,Y,X):- a(X,Y).
c(X,Y):- a(f(1),Y),b(X,Y,X).
"""
output = """
{}
"""
