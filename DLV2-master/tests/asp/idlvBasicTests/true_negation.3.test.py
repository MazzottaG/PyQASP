input = """
f(X) | f(Y) :- -g(X,Y), -h(Y).

-f(Y) :- g(Y,Y).

-g(a,b).
g(b,b).
-g(c,c).

-h(a).
-h(b).
"""
output = """
{-f(b), -g(a,b), -g(c,c), -h(a), -h(b), f(a), g(b,b)}
"""
