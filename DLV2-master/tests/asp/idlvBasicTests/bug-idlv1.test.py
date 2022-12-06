input = """
c(Y):- b(Y), not a(Y,Y).
c(Y):-a(Y,X), b(X), Y=X-1.
a(X,X) :-b(X), Y=X+1, not b(Y),not c(X).
a(X,Y):-c(X), c(Y), not b(X).
a(1,2).
b(2).
b(3).
"""
output = """
{a(1,1), a(1,2), a(1,3), b(2), b(3), c(1), c(2), c(3)}
{a(1,1), a(1,2), a(3,3), b(2), b(3), c(1), c(2)}
"""
