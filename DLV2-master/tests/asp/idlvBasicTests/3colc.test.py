input = """
node(X) :- edge(X,Y).
node(Y) :- edge(X,Y).

colored(X,r) :- node(X), not colored(X,g), not colored(X,b).
colored(X,g) :- node(X), not colored(X,r), not colored(X,b).
colored(X,b) :- node(X), not colored(X,r), not colored(X,g).

:- edge(X,Y), colored(X,C), colored(Y,C).
"""
output = """
{}
"""
