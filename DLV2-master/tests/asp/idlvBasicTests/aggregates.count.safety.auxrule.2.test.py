input = """
ouch(X) :- #count{V:b(V)} = X,  not p(X,Y).

b(1).
p(2,2).
"""
output = """
"""
