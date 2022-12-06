input = """

distanza(1,0).
distanza(2,1).


d(1) | nd(1).
d(2) | nd(2).

:- not #count{D: d(D)}=1.

serve(1,Dist) :- distanza(_,Dist), 
  Dist = #max {Y : d(D1), distanza(D1,Y) }.

"""
output = """
{d(1), distanza(1,0), distanza(2,1), nd(2), serve(1,0)}
{d(2), distanza(1,0), distanza(2,1), nd(1), serve(1,1)}
"""
