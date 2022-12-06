input = """
const(a).

h(X) :- f1(X), f3(X). % non f2(X), f3(X), non f4(X).
h(X) :- f2(X), f3(X). %non f0(X), non f1(X), f2(X), f3(X).


"""
output = """
{const(a)}
"""
