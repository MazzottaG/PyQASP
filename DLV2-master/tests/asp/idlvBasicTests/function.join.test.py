input = """
% instance

wlist(0,0,pos(atom(q)),1). wlist(0,1,pos(atom(r)),1). 
rule(pos(sum(0,0,2)),pos(conjunction(0))).
rule(pos(atom(a)),pos(conjunction(1))).
rule(pos(sum(0,0,2)),pos(conjunction(2))).
rule(pos(atom(d)),pos(conjunction(3))).
rule(pos(sum(0,1,2)),pos(conjunction(4))).

wlist(2,0,pos(atom(q)),1). wlist(2,1,pos(atom(r)),1).
wlist(2,2,pos(atom(p)),1). wlist(2,3,pos(atom(s)),1).

elem(P) :- rule(pos(sum(_,S,_)),_), wlist(S,_,pos(P),_).
"""
output = """
{elem(atom(q)), elem(atom(r)), rule(pos(atom(a)),pos(conjunction(1))), rule(pos(atom(d)),pos(conjunction(3))), rule(pos(sum(0,0,2)),pos(conjunction(0))), rule(pos(sum(0,0,2)),pos(conjunction(2))), rule(pos(sum(0,1,2)),pos(conjunction(4))), wlist(0,0,pos(atom(q)),1), wlist(0,1,pos(atom(r)),1), wlist(2,0,pos(atom(q)),1), wlist(2,1,pos(atom(r)),1), wlist(2,2,pos(atom(p)),1), wlist(2,3,pos(atom(s)),1)}
"""
