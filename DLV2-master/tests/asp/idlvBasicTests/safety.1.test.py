input = """
true.

% safe rules
f(X) | g(Y) :- h(X,Y), not i(Y).
f(X) | g(Y) :- i(Y), i(Z), i(X), not h(Z,Y).
% unsafe rules
f(X) | g(Y) :- h(X,Y), not i(Z).
f(X) | g(Y) :- not h(X,Y).
f(X) | g(Y) :- i(X), not h(X,Y).
f(X) | g(Y) :- i(Y), not h(X,Y).
f(X) | g(Y) :- i(Y), i(X), not h(Z,Y).
f(X) | g(Y) :- true.
"""
output = """
"""
