input = """
a(X,Y):- edge(f(X),Y), node(X), not node(f(X)).
b(X,Y):- a(X,Y).
c(X,Y):- b(X,Y).
d(X,Y):- c(X,Y).
e(X,Y):- d(X,Y).

"""
output = """
{}
"""
