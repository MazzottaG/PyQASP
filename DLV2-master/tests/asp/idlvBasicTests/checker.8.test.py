input = """
a :- b.
b :- a.
c | d.  
a | c.

a | z.

:- not a.
:- not z.

z :- y.
y :- z.
x | w.
z | x.
"""
output = """
{a, b, d, w, y, z}
"""
