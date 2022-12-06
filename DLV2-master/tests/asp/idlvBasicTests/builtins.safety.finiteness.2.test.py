input = """
p(X) :- q(Y), r(Y). %, #succ(Y,X).
q(X) :- p(X).
p(0).
r(0).
"""
output = """
"""
