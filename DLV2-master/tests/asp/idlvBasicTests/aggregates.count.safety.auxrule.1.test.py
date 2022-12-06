input = """
:- a(X), #count{ Y: b(Y), c(Y), not d(V,X) } < 1.
bug :- a(X), #count{ Y: b(Y), c(Y), not d(V,X) }  < 1.
"""
output = """
"""
