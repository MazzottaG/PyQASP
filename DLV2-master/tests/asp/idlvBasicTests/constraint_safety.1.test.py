input = """
true.
i(a).
h(a,a).

% safe constraints
:- i(Y), i(X), not h(X,Y).
:- i(X), not h(X,X).
:- not i(a).
% unsafe constraints
:- not h(X,Y).
:- i(X1), not h(X1,X2).
:- i(X2), not h(X1,X2).
"""
output = """
"""
