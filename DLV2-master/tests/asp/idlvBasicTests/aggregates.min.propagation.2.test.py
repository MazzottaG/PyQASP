input = """

di(1).
di(2).


d(1) | d(2).
:- not #count{D: d(D)}=1.

serve :- di(Dist), Dist = #min {D1 : d(D1), di(D1) }.

"""
output = """
{d(2), di(1), di(2), serve}
{d(1), di(1), di(2), serve}
"""
