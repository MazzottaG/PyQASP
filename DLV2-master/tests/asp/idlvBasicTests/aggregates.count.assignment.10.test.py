input = """
a(Z) :- Y = #count{X:c(X,V)}, Z = #count{W:b(W,Y)}, p(V).

b(2,1).
c(1,a).
c(2,a).
p(a).
"""
output = """
{a(0), b(2,1), c(1,a), c(2,a), p(a)}
"""
