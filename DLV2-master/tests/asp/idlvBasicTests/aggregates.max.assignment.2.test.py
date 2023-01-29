input = """
r(W) :- W = #max{X: a(X,Y)}, c(Y).

a(1,2).
a(3,2).
a(2,1).
a(5,1).

c(1) :- not c(2).
c(2) :- not c(1).

"""
output = """
{a(1,2), a(2,1), a(3,2), a(5,1), c(1), r(5)}
{a(1,2), a(2,1), a(3,2), a(5,1), c(2), r(3)}
"""