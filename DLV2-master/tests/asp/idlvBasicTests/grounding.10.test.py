input = """
bottom(b).
time0(0).

pushable_bottom(b,c).

pushdown(B,C) | npushdown(B,C) :- bottom(B), time0(T), box(B,T), pushable_bottom(B,C).


box(B,1) :- pushdown(Z,B).
box(b,0).
"""
output = """
{bottom(b), box(b,0), box(c,1), pushable_bottom(b,c), pushdown(b,c), time0(0)}
{bottom(b), box(b,0), npushdown(b,c), pushable_bottom(b,c), time0(0)}
"""
