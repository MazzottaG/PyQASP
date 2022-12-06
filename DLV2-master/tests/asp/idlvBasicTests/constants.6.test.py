input = """
%#const a = 10.
a(a).

a(X) :- p(X).
p(a).
"""
output = """
{a(a), p(a)}
"""
