input = """
bla(bla).
d(1).

% Guards should be integers, so after the instantiation of X,
% the evaluation of the aggregate atom should return false and
% ouch should not be derived.
ouch :- bla(X), #count{V : d(V)} < X.
"""
output = """
{bla(bla), d(1), ouch}
"""
