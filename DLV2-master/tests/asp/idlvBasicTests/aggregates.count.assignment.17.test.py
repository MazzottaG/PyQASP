input = """

a(S,T,Z) :- #count{X: r(T,X)} = Z, #count{W: q(W,S)} = T, #count{K: p(K,Y)} = S.

q(1,1).
q(2,2).

r(1,1).
r(1,2).
r(1,3).
r(2,2).
r(3,3).

p(1,1).
p(2,2).

%out{ a(2,1,3) }

%repository error
"""
output = """
{a(2,1,3), p(1,1), p(2,2), q(1,1), q(2,2), r(1,1), r(1,2), r(1,3), r(2,2), r(3,3)}
"""
