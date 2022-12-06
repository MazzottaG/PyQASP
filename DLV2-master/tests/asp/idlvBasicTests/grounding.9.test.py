input = """
bottom(a,b).
bottom(b,c).
bottom(c,d).

pushable_bottom(b,c).

pushdown(B,C) | npushdown(B,C) :- bottom(a,B), box(B,0), pushable_bottom(B,C).


box(B,1) :- pushdown(X,B).
box(b,0).


good_pushlocation(c).
"""
output = """
{bottom(a,b), bottom(b,c), bottom(c,d), box(b,0), box(c,1), good_pushlocation(c), pushable_bottom(b,c), pushdown(b,c)}
{bottom(a,b), bottom(b,c), bottom(c,d), box(b,0), good_pushlocation(c), npushdown(b,c), pushable_bottom(b,c)}
"""
