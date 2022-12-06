input = """
d(1).
p(1).

okay0    :-p(X), X = #count{ V : d(V)}.

okay1(X) :-p(X), X = #count{ V : d(V)}.
"""
output = """
{d(1), okay0, okay1(1), p(1)}
"""
