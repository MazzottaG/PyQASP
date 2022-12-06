input = """
a | na.
b | nb.
c | nc.
d | nd.
x | nx.

x :- v.
nx :- w.

v :- w, nx.

w :- na, nc, nx.
w :- nb, c, nx.
w :- b, nd.
w :- not w.
"""
output = """
{a, b, c, nd, nx, v, w, x}
{b, c, na, nd, nx, v, w, x}
{a, b, nc, nd, nx, v, w, x}
{b, na, nc, nd, nx, v, w, x}
"""
