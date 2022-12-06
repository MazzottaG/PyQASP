input = """
a(1) | a(2).

p(1).

q(X) :- p(X), X = #min{V:a(V)}.

"""
output = """
{a(1), p(1), q(1)}
{a(2), p(1)}
"""
