input = """


f(a).

%g(a..z).
%h(1..3).

f(b).

intersect(X) :- g(X), h(X).
"""
output = """
{f(a), f(b)}
"""
