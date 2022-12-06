input = """
node(X) :- edge(X,Y).
node(Y) :- edge(X,Y).

colored(X,r) | colored(X,g) | colored(X,b) :- node(X).

:- edge(X,Y), colored(X,C), colored(Y,C).
"""
output = """
{}
"""
