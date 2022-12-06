input = """
a(X):-edge(f(X),_), node(X).
node(X):-b(X).
b(X):-a(X).
"""
output = """
{}
"""
