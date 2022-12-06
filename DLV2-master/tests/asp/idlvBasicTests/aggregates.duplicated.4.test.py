input = """
mysum(X) :- #sum{P: c(P)} = X, dom(X).
mycount(X) :- #count{P: c(P)} = X, dom(X).

dom(0). dom(1).
c(X) | d(X) :- dom(X).

:- not c(0).
:- not c(1).
"""
output = """
{c(0), c(1), dom(0), dom(1), mysum(1)}
"""
