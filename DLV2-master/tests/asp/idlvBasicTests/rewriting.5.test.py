input = """
a(c,d).
b(X,Y) :- a(X,Y), not b(X,Y).
c(X) :- a(Z,W), b(X,Y).

"""
output = """
INCOHERENT
"""
