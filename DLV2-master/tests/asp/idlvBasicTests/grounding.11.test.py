input = """
bottom(b).
time0(0).
time1(1).

pushable_bottom(b,c).

pushdown(B,C) | npushdown(B,C) :- bottom(B), time0(T), box(B,T), pushable_bottom(B,C).


box(B,T) :- pushdown(Z,B), time1(T).
box(B,T) :- boxinit(B,T).
boxinit(b,0).


good_pushlocation(c).

"""
output = """
{bottom(b), box(b,0), box(c,1), boxinit(b,0), good_pushlocation(c), pushable_bottom(b,c), pushdown(b,c), time0(0), time1(1)}
{bottom(b), box(b,0), boxinit(b,0), good_pushlocation(c), npushdown(b,c), pushable_bottom(b,c), time0(0), time1(1)}
"""
