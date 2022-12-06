input = """

a(Y) :- #count{X: p(X,Z)} = Y, #count{W: p(W,W) } = Z.

p(1,1).
p(1,2).
p(1,3).
p(2,4).

%out{ a(1). }
"""
output = """
{a(1), p(1,1), p(1,2), p(1,3), p(2,4)}
"""
