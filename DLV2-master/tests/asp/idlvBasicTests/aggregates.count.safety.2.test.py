input = """
a(1). a(2).
c(1). c(2).
b :- 2 = #count{X:a(X)}, a(X).
"""
output = """
{a(1), a(2), c(1), c(2)}
"""
