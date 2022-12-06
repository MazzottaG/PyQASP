input = """
a | c :- d, e.

d :- a.
d :- b.
b :- a.

e :- f, g, i.
f :- g, e, i.
i :- e, f, h.

g :- f, h, e, i.

c :- h, g.

h :- i.

a | e.
"""
output = """
{a, b, d}
{e}
"""
