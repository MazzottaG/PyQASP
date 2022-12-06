input = """

% No more than bound() Instances of a(X) should be true.
:- bound(N), #sum{V : a(V)} > N.

% Exactly 2 Instances of a(X) are true.
a(first).
a(second).

% There should be no more than 2 Instances of a(X).
bound(2).
% There should be no more than 3 Instances of a(X).
bound(3).

"""
output = """
{a(first), a(second), bound(2), bound(3)}
"""
