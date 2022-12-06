input = """
c(1) | c(2) | c(3).
a(X) :- c(X).

okay :- 0 < #count{V :a(V)} < 3. 
ouch :- 1 < #count{V :a(V)} < 3.
"""
output = """
{a(1), c(1), okay}
{a(3), c(3), okay}
{a(2), c(2), okay}
"""
