input = """
% Bug.
% line 3: Rule is not safe.
% Aborting due to parser errors.

s(20).
q(X) :- s(X), Y = 5, Y < X.
q1(X) :- s(X), Y = 5, Y < X.

"""
output = """
{q(20), q1(20), s(20)}
"""
