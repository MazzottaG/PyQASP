input = """
#show pippo/1.
a(X) | b(X) :- c(X).

c(1..10).

:- a(X),c(X).
"""
output = """
{}
"""
