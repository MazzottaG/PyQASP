input = """
r(Y,Z) :- Z = #count{ X: d(X,Y) }, Y = #count{ A: e(A,Z) }.

d(a,1). d(b,1).
e(a,2).
d(a,3).
e(a,1). e(b,1). e(c,1).

"""
output = """
"""
