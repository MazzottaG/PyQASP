input = """
a(Z) :- Z = #count{X:b(X,Y)}, Y = #count{W:c(W)}.

b(2,1).
c(1).
c(2).
"""
output = """
{a(0), b(2,1), c(1), c(2)}
"""
