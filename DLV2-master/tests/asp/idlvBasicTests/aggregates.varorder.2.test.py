input = """

a(V1,V2) :- q(Y), #count{W: p(W,Y,Z)} = V1, #count{X: p(X,Z,Y)} = V2.

q(1).
p(0,1,1).
p(1,1,2).
p(0,2,1).
p(0,2,2).
p(0,3,5).
p(0,6,1).

"""
output = """
{a(2,1), p(0,1,1), p(0,2,1), p(0,2,2), p(0,3,5), p(0,6,1), p(1,1,2), q(1)}
"""