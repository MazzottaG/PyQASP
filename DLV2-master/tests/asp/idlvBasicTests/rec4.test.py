input = """
a(X,Y):-edge(X,Y),edge(Y,Y).
a(X,X):-a(X,Y).
"""
output = """
{}
"""
