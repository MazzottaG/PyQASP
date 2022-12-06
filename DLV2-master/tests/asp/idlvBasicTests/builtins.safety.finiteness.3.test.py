input = """
p(X) :- q(Y). %, #prec(Y,X).
q(X) :- p(X).

q(10).
"""
output = """
"""
