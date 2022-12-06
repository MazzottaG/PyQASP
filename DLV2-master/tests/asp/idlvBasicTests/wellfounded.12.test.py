input = """
p :- not q.
q :- r.
r :- q.
s :- p.
t :- not s.
l :- u.
u :- t.

"""
output = """
{p, s}
"""
