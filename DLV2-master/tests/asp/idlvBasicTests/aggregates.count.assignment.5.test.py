input = """
r(Y,Z) :- Z = #count{ X: d(X,Y) }, Y = #count{ X: e(X,Z) }.

d(1,1). e(1,1).

"""
output = """
"""
