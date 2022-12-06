input = """
a(Z) :- Z = #count{X:b(X,Y)}, Y = #count{W:c(W,V)}, p(V).

b(2,1).
c(1,a).
c(2,a).
p(a).
"""
output = """
{a(0), b(2,1), c(1,a), c(2,a), p(a)}
"""
