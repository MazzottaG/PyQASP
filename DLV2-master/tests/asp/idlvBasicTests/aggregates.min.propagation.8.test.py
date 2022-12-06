input = """
d(0) | b.
d(1).

:- not b.
l_d(M) :- d(M), #min{ D : d(D) } = M.


"""
output = """
{b, d(1), l_d(1)}
"""
