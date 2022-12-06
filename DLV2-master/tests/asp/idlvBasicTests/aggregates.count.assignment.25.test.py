input = """
q(1,2).
p(1..2).
s(1..3).

r(Z) :- Y=X+1, #count{H: s(H), not q(H,Y)}=Z, #count{F: p(F)}=G, X=G*3."""
output = """
{p(1), p(2), q(1,2), r(3), s(1), s(2), s(3)}
"""
