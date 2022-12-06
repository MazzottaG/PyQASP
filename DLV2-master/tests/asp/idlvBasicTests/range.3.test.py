input = """


f(a).

% intuitively, one could expect: g(4). g(3). g(2). here
% but this does not produce any g():
%g(4..2).
%h(1..3).

f(b).

intersect(X) :- g(X), h(X).
"""
output = """
{f(a), f(b)}
"""
