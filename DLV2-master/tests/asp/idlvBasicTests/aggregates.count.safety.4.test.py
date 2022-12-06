input = """
a(1). a(2).
b :- #count{ X : a(X) } = 2,  #count{ Y : a(Y) } = X.

"""
output = """
{a(1), a(2)}
"""
