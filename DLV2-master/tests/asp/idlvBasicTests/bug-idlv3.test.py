input = """
bound(1).
{s(1)}.
{s(2)}.
bound(X1) :- sum(X), X1 = X+1.
sum(K) :- K<=#sum{X:s(X)}, bound(K).
"""
output = """
{bound(1)}
{bound(1), s(1), sum(1), bound(2)}
{bound(1), s(2), sum(1), bound(2), sum(2), bound(3)}
{bound(1), s(2), s(1), sum(1), bound(2), sum(2), bound(3), sum(3), bound(4)}
"""
