input = """
c | d.
a | c.

a | z.

:- not a.
:- not z.

x | w.
z | x.

"""
output = """
{a, d, w, z}
"""
