input = """
r(Y,Z) :- Z = #count{ X: d(X,Y) }, Y = #count{ A: e(A,Z) }.

d(1,1). e(1,1).
f(0).
"""
output = """
"""
