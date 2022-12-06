input = """
a(4)|d.
a(-1)|f.
ok :- #sum{X:a(X)}=4.
"""
output = """
{d, f}
{a(-1), d}
{a(4), f, ok}
{a(-1), a(4)}
"""
