input = """
p :- not q.
q :- not p.
r :- p.
r :- not r.
"""
output = """
{p, r}
"""
