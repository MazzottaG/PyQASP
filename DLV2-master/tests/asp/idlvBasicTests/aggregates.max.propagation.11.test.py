input = """
d(1) | b.
d(0).

:- not b.
l_d(M) :- d(M), #max{ D : d(D) } = M.


"""
output = """
{b, d(0), l_d(0)}
"""
