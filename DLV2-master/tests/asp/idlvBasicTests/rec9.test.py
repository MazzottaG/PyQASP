input = """
b(X,Y,Z):-edge(f(X),X+2), Y=X+3, Z=Y+1, a(X,Y,Z).
a(X,Y,Z):-edge(f(X),f(Y)), node(Z).
"""
output = """
{}
"""
