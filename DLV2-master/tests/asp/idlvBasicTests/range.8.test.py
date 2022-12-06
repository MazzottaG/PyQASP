input = """

f(a).

%g(2..4).
%h(1..3).

f(b).

intersect(X) :- g(X), h(X).
"""
output = """
{f(a), f(b)}
"""
