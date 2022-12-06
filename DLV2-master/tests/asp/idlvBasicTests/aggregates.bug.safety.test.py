input = """
{ hold(P) : wlist(S,_,pos(P),_) } :- #sum{S:b(S)}=3, pos(B), hold(B).
{ hold(P) : wlist(S,_,pos(P),_) } :- #sum{S:b(S)}<3, pos(B), hold(B).
pos(1..3).
hold(1..3).
b(1..2).
wlist(1..2,1..2,pos(1),1).
"""
output = """
{b(1), b(2), hold(1), hold(2), hold(3), pos(1), pos(2), pos(3), wlist(1,1,pos(1),1), wlist(1,2,pos(1),1), wlist(2,1,pos(1),1), wlist(2,2,pos(1),1)}
"""
