input = """
a(2)|z.
a(-1)|w.
a(-4)|r.
ok :- #sum{X:a(X)}<=1.
"""
output = """
{ok, r, w, z}
{a(-4), ok, w, z}
{a(-1), ok, r, z}
{a(-1), a(-4), ok, z}
{a(2), r, w}
{a(-1), a(2), ok, r}
{a(-4), a(2), ok, w}
{a(-1), a(-4), a(2), ok}
"""
