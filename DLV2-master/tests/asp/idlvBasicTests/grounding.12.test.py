input = """

g(a).
g(c).
h(a) :- i.
i :- g(Lit), not h(Lit).





"""
output = """
{g(a), g(c), h(a), i}
"""
