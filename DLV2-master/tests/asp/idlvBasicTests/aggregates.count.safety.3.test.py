input = """
a(1). a(2).
c(1). c(2).
b :- 2 = #count{X:a(X)}, c(X).
"""
output = """
{a(1), a(2), c(1), c(2)}
"""
