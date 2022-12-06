input = """
node(N) :- edge(N,_).
node(N) :- edge(_,N).

col(N,X) | -col(N,X) :- node(N), X > 0. %#int(X), X > 0.
:- col(N,X), col(N,Y), X != Y.

:- node(N), not isColored(N).
isColored(N) :- col(N,X).

:- edge(X,Y), col(X,C1), col(Y,C2), C1=C2.
:- edge(X,Y), col(X,C1), col(Y,C2), C1=C2.
"""
output = """
"""
