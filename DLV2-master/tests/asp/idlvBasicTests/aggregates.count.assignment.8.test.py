input = """
a(Z) :- Y = #count{X:c(X)}, Z = #count{W:b(W,Y)}.

b(2,1).
c(1).
c(2).
"""
output = """
{a(0), b(2,1), c(1), c(2)}
"""
