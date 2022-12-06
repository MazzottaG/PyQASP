input = """
% instance

wlist(0,0,pos(atom(q)),1). wlist(0,1,pos(atom(r)),1). 
set(0,neg(atom(c))). rule(pos(sum(0,0,2)),pos(conjunction(0))).
set(1,pos(atom(r))). rule(pos(atom(a)),pos(conjunction(1))).
set(2,neg(atom(d))). rule(pos(sum(0,0,2)),pos(conjunction(2))).
set(3,neg(atom(t))). rule(pos(atom(d)),pos(conjunction(3))).
wlist(1,0,pos(atom(p)),1). wlist(1,1,pos(atom(t)),1).
set(4,pos(atom(a))). set(4,neg(atom(b))).
rule(pos(sum(0,1,2)),pos(conjunction(4))). set(5,pos(atom(t))).
set(5,neg(atom(r))). set(5,neg(atom(s))).
rule(pos(atom(b)),pos(conjunction(5))). set(6,neg(atom(r))).
set(6,neg(atom(q))). rule(pos(atom(s)),pos(conjunction(6))).
set(7,pos(atom(s))). rule(pos(atom(a)),pos(conjunction(7))).
rule(pos(atom(a)),pos(conjunction(3))). set(8,neg(atom(p))).
rule(pos(atom(c)),pos(conjunction(8))). set(9,pos(atom(a))).
set(9,neg(atom(t))). set(9,neg(atom(b))).
set(9,neg(atom(p))). rule(pos(false),pos(conjunction(9))).
set(10,pos(atom(q))). set(10,pos(atom(r))).
set(10,neg(atom(c))). rule(pos(false),pos(conjunction(10))).
set(11,pos(atom(q))). set(11,pos(atom(r))). set(11,neg(atom(d))).
rule(pos(false),pos(conjunction(11))). set(12,pos(atom(r))).
set(12,pos(atom(t))). set(12,neg(atom(b))). set(12,neg(atom(q))).
rule(pos(false),pos(conjunction(12))).
wlist(2,0,pos(atom(q)),1). wlist(2,1,pos(atom(r)),1).
wlist(2,2,pos(atom(p)),1). wlist(2,3,pos(atom(s)),1).

minimize(1,2). optimize(1,1,incl).

:- not hold(atom(r)), not hold(atom(t)).

% encoding

eleb(P) :- rule(_,pos(P)).

nhold(conjunction(S)) :- eleb(conjunction(S)), not hold(P), set(S,pos(P)).
nhold(conjunction(S)) :- eleb(conjunction(S)),     hold(P), set(S,neg(P)).
 hold(conjunction(S)) :- eleb(conjunction(S)), not nhold(conjunction(S)).

hold(atom(A))                     :- rule(pos(atom(A)),   pos(B)), hold(B).
                                  :- rule(pos(false),     pos(B)), hold(B).
%@rule_ordering(@value=0).
{ hold(P) : wlist(S,_,pos(P),_) } :- rule(pos(sum(_,S,_)),pos(B)), hold(B).

elem(E) :- eleb(E).
elem(E) :- rule(pos(E),_).
elem(P) :- rule(pos(sum(_,S,_)),_), wlist(S,_,pos(P),_).
elem(P) :- minimize(J,S),           wlist(S,_,pos(P),W).

supp(atom(A),B) :- rule(pos(atom(A)),   pos(B)).
%@rule_partial_order(@before={wlist(S,_,pos(atom(A)),_)},@after={rule(pos(sum(_,S,_)),pos(B))}).
supp(atom(A),B) :- rule(pos(sum(_,S,_)),pos(B)), wlist(S,_,pos(atom(A)),_).

supp(atom(A))   :- supp(atom(A),B).

set(S) :- set(S,E).
fact(atom(A))   :- rule(pos(atom(A)),pos(conjunction(S))), not set(S).

true(atom(A))                 :- fact(atom(A)).
true(atom(A)) | fail(atom(A)) :- supp(atom(A)), not fact(atom(A)).
                fail(atom(A)) :- elem(atom(A)), not supp(atom(A)).

fail(false).

sett(S,0,P)    :- set(S,P).
sett(S,N+1,P2) :- sett(S,N,P1), sett(S,N,P2), P1 < P2.

setn(S,N,P) :- sett(S,N,P), not sett(S,N+1,P).
setn(S,N)  :- setn(S,N,_).

true(conjunction(S),N+1) :- elem(conjunction(S)), setn(S,N), not setn(S,N+1).
true(conjunction(S),N)   :- elem(conjunction(S)), true(P), setn(S,N,pos(P)), true(conjunction(S),N+1).
true(conjunction(S),N)   :- elem(conjunction(S)), fail(P), setn(S,N,neg(P)), true(conjunction(S),N+1).

true(conjunction(S)) :- true(conjunction(S),0).
fail(conjunction(S)) :- elem(conjunction(S)), set(S,pos(P)), fail(P).
fail(conjunction(S)) :- elem(conjunction(S)), set(S,neg(N)), true(N).

suppt(S,0,P)    :- supp(S,P).
suppt(S,N+1,P2) :- suppt(S,N,P1), suppt(S,N,P2), P1 < P2.

suppn(S,N,P) :- suppt(S,N,P), not suppt(S,N+1,P).
suppn(S,N)  :- suppn(S,N,_).

suppf(S,N+1) :- suppn(S,N), not suppn(S,N+1).
suppf(S,N)   :- fail(P), suppn(S,N,P), suppf(S,N+1).

bot :- true(atom(A)), suppf(atom(A),0).
bot :- rule(pos(H),pos(B)), true(B), fail(H).

true(atom(A)) :- supp(atom(A)), not fact(atom(A)), bot.
fail(atom(A)) :- supp(atom(A)), not fact(atom(A)), bot.

target(P,N) :- minimize(_,S), wlist(S,N,pos(P),_).
target(N) :- target(P,N).

equal(N+1) :- target(N), not target(N+1).
equal(N) :- target(P,N), true(P),     hold(P), equal(N+1).
equal(N) :- target(P,N), fail(P), not hold(P), equal(N+1).

bot :- equal(0).
bot :- target(P,_), true(P), not hold(P).

:- not bot.


"""
output = """
{wlist(0,0,pos(atom(q)),1), wlist(0,1,pos(atom(r)),1), set(0,neg(atom(c))), rule(pos(sum(0,0,2)),pos(conjunction(0))), set(1,pos(atom(r))), rule(pos(atom(a)),pos(conjunction(1))), set(2,neg(atom(d))), rule(pos(sum(0,0,2)),pos(conjunction(2))), set(3,neg(atom(t))), rule(pos(atom(d)),pos(conjunction(3))), wlist(1,0,pos(atom(p)),1), wlist(1,1,pos(atom(t)),1), set(4,pos(atom(a))), set(4,neg(atom(b))), rule(pos(sum(0,1,2)),pos(conjunction(4))), set(5,pos(atom(t))), set(5,neg(atom(r))), set(5,neg(atom(s))), rule(pos(atom(b)),pos(conjunction(5))), set(6,neg(atom(r))), set(6,neg(atom(q))), rule(pos(atom(s)),pos(conjunction(6))), set(7,pos(atom(s))), rule(pos(atom(a)),pos(conjunction(7))), rule(pos(atom(a)),pos(conjunction(3))), set(8,neg(atom(p))), rule(pos(atom(c)),pos(conjunction(8))), set(9,pos(atom(a))), set(9,neg(atom(t))), set(9,neg(atom(b))), set(9,neg(atom(p))), rule(pos(false),pos(conjunction(9))), set(10,pos(atom(q))), set(10,pos(atom(r))), set(10,neg(atom(c))), rule(pos(false),pos(conjunction(10))), set(11,pos(atom(q))), set(11,pos(atom(r))), set(11,neg(atom(d))), rule(pos(false),pos(conjunction(11))), set(12,pos(atom(r))), set(12,pos(atom(t))), set(12,neg(atom(b))), set(12,neg(atom(q))), rule(pos(false),pos(conjunction(12))), wlist(2,0,pos(atom(q)),1), wlist(2,1,pos(atom(r)),1), wlist(2,2,pos(atom(p)),1), wlist(2,3,pos(atom(s)),1), minimize(1,2), optimize(1,1,incl), fail(false), supp(atom(q),conjunction(2)), supp(atom(q),conjunction(0)), supp(atom(r),conjunction(2)), supp(atom(r),conjunction(0)), supp(atom(p),conjunction(4)), supp(atom(t),conjunction(4)), supp(atom(a),conjunction(1)), supp(atom(d),conjunction(3)), supp(atom(b),conjunction(5)), supp(atom(s),conjunction(6)), supp(atom(a),conjunction(7)), supp(atom(a),conjunction(3)), supp(atom(c),conjunction(8)), suppt(atom(q),0,conjunction(2)), suppt(atom(q),0,conjunction(0)), suppt(atom(r),0,conjunction(2)), suppt(atom(r),0,conjunction(0)), suppt(atom(p),0,conjunction(4)), suppt(atom(t),0,conjunction(4)), suppt(atom(a),0,conjunction(1)), suppt(atom(d),0,conjunction(3)), suppt(atom(b),0,conjunction(5)), suppt(atom(s),0,conjunction(6)), suppt(atom(a),0,conjunction(7)), suppt(atom(a),0,conjunction(3)), suppt(atom(c),0,conjunction(8)), suppt(atom(q),1,conjunction(2)), suppt(atom(r),1,conjunction(2)), suppt(atom(a),1,conjunction(7)), suppt(atom(a),1,conjunction(3)), suppt(atom(a),2,conjunction(7)), suppn(atom(q),0,conjunction(0)), suppn(atom(r),0,conjunction(0)), suppn(atom(p),0,conjunction(4)), suppn(atom(t),0,conjunction(4)), suppn(atom(a),0,conjunction(1)), suppn(atom(d),0,conjunction(3)), suppn(atom(b),0,conjunction(5)), suppn(atom(s),0,conjunction(6)), suppn(atom(c),0,conjunction(8)), suppn(atom(q),1,conjunction(2)), suppn(atom(r),1,conjunction(2)), suppn(atom(a),1,conjunction(3)), suppn(atom(a),2,conjunction(7)), suppn(atom(q),0), suppn(atom(r),0), suppn(atom(p),0), suppn(atom(t),0), suppn(atom(a),0), suppn(atom(d),0), suppn(atom(b),0), suppn(atom(s),0), suppn(atom(c),0), suppn(atom(q),1), suppn(atom(r),1), suppn(atom(a),1), suppn(atom(a),2), supp(atom(q)), supp(atom(r)), supp(atom(p)), supp(atom(t)), supp(atom(a)), supp(atom(d)), supp(atom(b)), supp(atom(s)), supp(atom(c)), eleb(conjunction(0)), eleb(conjunction(1)), eleb(conjunction(2)), eleb(conjunction(3)), eleb(conjunction(4)), eleb(conjunction(5)), eleb(conjunction(6)), eleb(conjunction(7)), eleb(conjunction(8)), eleb(conjunction(9)), eleb(conjunction(10)), eleb(conjunction(11)), eleb(conjunction(12)), elem(atom(q)), elem(atom(r)), elem(atom(p)), elem(atom(s)), elem(atom(t)), elem(sum(0,0,2)), elem(atom(a)), elem(atom(d)), elem(sum(0,1,2)), elem(atom(b)), elem(atom(c)), elem(false), elem(conjunction(0)), elem(conjunction(1)), elem(conjunction(2)), elem(conjunction(3)), elem(conjunction(4)), elem(conjunction(5)), elem(conjunction(6)), elem(conjunction(7)), elem(conjunction(8)), elem(conjunction(9)), elem(conjunction(10)), elem(conjunction(11)), elem(conjunction(12)), set(0), set(1), set(2), set(3), set(4), set(5), set(6), set(7), set(8), set(9), set(10), set(11), set(12), sett(0,0,neg(atom(c))), sett(1,0,pos(atom(r))), sett(2,0,neg(atom(d))), sett(3,0,neg(atom(t))), sett(4,0,pos(atom(a))), sett(4,0,neg(atom(b))), sett(5,0,pos(atom(t))), sett(5,0,neg(atom(r))), sett(5,0,neg(atom(s))), sett(6,0,neg(atom(r))), sett(6,0,neg(atom(q))), sett(7,0,pos(atom(s))), sett(8,0,neg(atom(p))), sett(9,0,pos(atom(a))), sett(9,0,neg(atom(t))), sett(9,0,neg(atom(b))), sett(9,0,neg(atom(p))), sett(10,0,pos(atom(q))), sett(10,0,pos(atom(r))), sett(10,0,neg(atom(c))), sett(11,0,pos(atom(q))), sett(11,0,pos(atom(r))), sett(11,0,neg(atom(d))), sett(12,0,pos(atom(r))), sett(12,0,pos(atom(t))), sett(12,0,neg(atom(b))), sett(12,0,neg(atom(q))), sett(4,1,pos(atom(a))), sett(5,1,pos(atom(t))), sett(5,1,neg(atom(s))), sett(6,1,neg(atom(r))), sett(9,1,pos(atom(a))), sett(9,1,neg(atom(t))), sett(9,1,neg(atom(p))), sett(10,1,pos(atom(r))), sett(10,1,pos(atom(q))), sett(11,1,pos(atom(r))), sett(11,1,pos(atom(q))), sett(12,1,pos(atom(t))), sett(12,1,pos(atom(r))), sett(12,1,neg(atom(q))), sett(5,2,pos(atom(t))), sett(9,2,pos(atom(a))), sett(9,2,neg(atom(t))), sett(10,2,pos(atom(r))), sett(11,2,pos(atom(r))), sett(12,2,pos(atom(t))), sett(12,2,pos(atom(r))), sett(9,3,pos(atom(a))), sett(12,3,pos(atom(t))), setn(0,0,neg(atom(c))), setn(1,0,pos(atom(r))), setn(2,0,neg(atom(d))), setn(3,0,neg(atom(t))), setn(4,0,neg(atom(b))), setn(5,0,neg(atom(r))), setn(6,0,neg(atom(q))), setn(7,0,pos(atom(s))), setn(8,0,neg(atom(p))), setn(9,0,neg(atom(b))), setn(10,0,neg(atom(c))), setn(11,0,neg(atom(d))), setn(12,0,neg(atom(b))), setn(4,1,pos(atom(a))), setn(5,1,neg(atom(s))), setn(6,1,neg(atom(r))), setn(9,1,neg(atom(p))), setn(10,1,pos(atom(q))), setn(11,1,pos(atom(q))), setn(12,1,neg(atom(q))), setn(5,2,pos(atom(t))), setn(9,2,neg(atom(t))), setn(10,2,pos(atom(r))), setn(11,2,pos(atom(r))), setn(12,2,pos(atom(r))), setn(9,3,pos(atom(a))), setn(12,3,pos(atom(t))), setn(0,0), setn(1,0), setn(2,0), setn(3,0), setn(4,0), setn(5,0), setn(6,0), setn(7,0), setn(8,0), setn(9,0), setn(10,0), setn(11,0), setn(12,0), setn(4,1), setn(5,1), setn(6,1), setn(9,1), setn(10,1), setn(11,1), setn(12,1), setn(5,2), setn(9,2), setn(10,2), setn(11,2), setn(12,2), setn(9,3), setn(12,3), target(atom(q),0), target(atom(r),1), target(atom(p),2), target(atom(s),3), target(0), target(1), target(2), target(3), hold(conjunction(2)), hold(conjunction(4)), hold(conjunction(6)), hold(conjunction(7)), hold(conjunction(8)), hold(atom(a)), hold(atom(s)), hold(atom(c)), hold(atom(t)), equal(4), true(atom(q)), fail(atom(q)), true(atom(r)), fail(atom(r)), true(atom(p)), fail(atom(p)), true(atom(t)), fail(atom(t)), true(atom(a)), fail(atom(a)), true(atom(d)), fail(atom(d)), true(atom(b)), fail(atom(b)), true(atom(s)), fail(atom(s)), true(atom(c)), fail(atom(c)), suppf(atom(p),1), suppf(atom(t),1), suppf(atom(d),1), suppf(atom(b),1), suppf(atom(s),1), suppf(atom(c),1), suppf(atom(q),2), suppf(atom(r),2), suppf(atom(a),3), true(conjunction(0),1), true(conjunction(1),1), true(conjunction(2),1), true(conjunction(3),1), true(conjunction(7),1), true(conjunction(8),1), true(conjunction(4),2), true(conjunction(6),2), true(conjunction(5),3), true(conjunction(10),3), true(conjunction(11),3), true(conjunction(9),4), true(conjunction(12),4), true(conjunction(0),0), true(conjunction(2),0), true(conjunction(3),0), true(conjunction(8),0), true(conjunction(6),1), true(conjunction(1),0), true(conjunction(7),0), true(conjunction(4),1), true(conjunction(5),2), true(conjunction(10),2), true(conjunction(11),2), true(conjunction(9),3), true(conjunction(12),3), bot, fail(conjunction(0)), fail(conjunction(2)), fail(conjunction(3)), fail(conjunction(4)), fail(conjunction(5)), fail(conjunction(6)), fail(conjunction(8)), fail(conjunction(9)), fail(conjunction(10)), fail(conjunction(11)), fail(conjunction(12)), fail(conjunction(1)), fail(conjunction(7)), equal(3), true(conjunction(4),0), true(conjunction(6),0), true(conjunction(5),1), true(conjunction(9),2), true(conjunction(10),1), true(conjunction(11),1), true(conjunction(12),2), suppf(atom(p),0), suppf(atom(t),0), suppf(atom(d),0), suppf(atom(b),0), suppf(atom(s),0), suppf(atom(c),0), suppf(atom(q),1), suppf(atom(r),1), suppf(atom(a),2), true(conjunction(0)), true(conjunction(2)), true(conjunction(3)), true(conjunction(8)), true(conjunction(1)), true(conjunction(7)), equal(2), true(conjunction(5),0), true(conjunction(10),0), true(conjunction(11),0), true(conjunction(9),1), true(conjunction(12),1), suppf(atom(q),0), suppf(atom(r),0), suppf(atom(a),1), true(conjunction(4)), true(conjunction(6)), equal(1), true(conjunction(9),0), true(conjunction(12),0), suppf(atom(a),0), true(conjunction(5)), true(conjunction(10)), true(conjunction(11)), equal(0), true(conjunction(9)), true(conjunction(12)), nhold(conjunction(0)), nhold(conjunction(3)), nhold(conjunction(5)), nhold(conjunction(9)), nhold(conjunction(10)), nhold(conjunction(11)), nhold(conjunction(12)), nhold(conjunction(1))}
{wlist(0,0,pos(atom(q)),1), wlist(0,1,pos(atom(r)),1), set(0,neg(atom(c))), rule(pos(sum(0,0,2)),pos(conjunction(0))), set(1,pos(atom(r))), rule(pos(atom(a)),pos(conjunction(1))), set(2,neg(atom(d))), rule(pos(sum(0,0,2)),pos(conjunction(2))), set(3,neg(atom(t))), rule(pos(atom(d)),pos(conjunction(3))), wlist(1,0,pos(atom(p)),1), wlist(1,1,pos(atom(t)),1), set(4,pos(atom(a))), set(4,neg(atom(b))), rule(pos(sum(0,1,2)),pos(conjunction(4))), set(5,pos(atom(t))), set(5,neg(atom(r))), set(5,neg(atom(s))), rule(pos(atom(b)),pos(conjunction(5))), set(6,neg(atom(r))), set(6,neg(atom(q))), rule(pos(atom(s)),pos(conjunction(6))), set(7,pos(atom(s))), rule(pos(atom(a)),pos(conjunction(7))), rule(pos(atom(a)),pos(conjunction(3))), set(8,neg(atom(p))), rule(pos(atom(c)),pos(conjunction(8))), set(9,pos(atom(a))), set(9,neg(atom(t))), set(9,neg(atom(b))), set(9,neg(atom(p))), rule(pos(false),pos(conjunction(9))), set(10,pos(atom(q))), set(10,pos(atom(r))), set(10,neg(atom(c))), rule(pos(false),pos(conjunction(10))), set(11,pos(atom(q))), set(11,pos(atom(r))), set(11,neg(atom(d))), rule(pos(false),pos(conjunction(11))), set(12,pos(atom(r))), set(12,pos(atom(t))), set(12,neg(atom(b))), set(12,neg(atom(q))), rule(pos(false),pos(conjunction(12))), wlist(2,0,pos(atom(q)),1), wlist(2,1,pos(atom(r)),1), wlist(2,2,pos(atom(p)),1), wlist(2,3,pos(atom(s)),1), minimize(1,2), optimize(1,1,incl), fail(false), supp(atom(q),conjunction(2)), supp(atom(q),conjunction(0)), supp(atom(r),conjunction(2)), supp(atom(r),conjunction(0)), supp(atom(p),conjunction(4)), supp(atom(t),conjunction(4)), supp(atom(a),conjunction(1)), supp(atom(d),conjunction(3)), supp(atom(b),conjunction(5)), supp(atom(s),conjunction(6)), supp(atom(a),conjunction(7)), supp(atom(a),conjunction(3)), supp(atom(c),conjunction(8)), suppt(atom(q),0,conjunction(2)), suppt(atom(q),0,conjunction(0)), suppt(atom(r),0,conjunction(2)), suppt(atom(r),0,conjunction(0)), suppt(atom(p),0,conjunction(4)), suppt(atom(t),0,conjunction(4)), suppt(atom(a),0,conjunction(1)), suppt(atom(d),0,conjunction(3)), suppt(atom(b),0,conjunction(5)), suppt(atom(s),0,conjunction(6)), suppt(atom(a),0,conjunction(7)), suppt(atom(a),0,conjunction(3)), suppt(atom(c),0,conjunction(8)), suppt(atom(q),1,conjunction(2)), suppt(atom(r),1,conjunction(2)), suppt(atom(a),1,conjunction(7)), suppt(atom(a),1,conjunction(3)), suppt(atom(a),2,conjunction(7)), suppn(atom(q),0,conjunction(0)), suppn(atom(r),0,conjunction(0)), suppn(atom(p),0,conjunction(4)), suppn(atom(t),0,conjunction(4)), suppn(atom(a),0,conjunction(1)), suppn(atom(d),0,conjunction(3)), suppn(atom(b),0,conjunction(5)), suppn(atom(s),0,conjunction(6)), suppn(atom(c),0,conjunction(8)), suppn(atom(q),1,conjunction(2)), suppn(atom(r),1,conjunction(2)), suppn(atom(a),1,conjunction(3)), suppn(atom(a),2,conjunction(7)), suppn(atom(q),0), suppn(atom(r),0), suppn(atom(p),0), suppn(atom(t),0), suppn(atom(a),0), suppn(atom(d),0), suppn(atom(b),0), suppn(atom(s),0), suppn(atom(c),0), suppn(atom(q),1), suppn(atom(r),1), suppn(atom(a),1), suppn(atom(a),2), supp(atom(q)), supp(atom(r)), supp(atom(p)), supp(atom(t)), supp(atom(a)), supp(atom(d)), supp(atom(b)), supp(atom(s)), supp(atom(c)), eleb(conjunction(0)), eleb(conjunction(1)), eleb(conjunction(2)), eleb(conjunction(3)), eleb(conjunction(4)), eleb(conjunction(5)), eleb(conjunction(6)), eleb(conjunction(7)), eleb(conjunction(8)), eleb(conjunction(9)), eleb(conjunction(10)), eleb(conjunction(11)), eleb(conjunction(12)), elem(atom(q)), elem(atom(r)), elem(atom(p)), elem(atom(s)), elem(atom(t)), elem(sum(0,0,2)), elem(atom(a)), elem(atom(d)), elem(sum(0,1,2)), elem(atom(b)), elem(atom(c)), elem(false), elem(conjunction(0)), elem(conjunction(1)), elem(conjunction(2)), elem(conjunction(3)), elem(conjunction(4)), elem(conjunction(5)), elem(conjunction(6)), elem(conjunction(7)), elem(conjunction(8)), elem(conjunction(9)), elem(conjunction(10)), elem(conjunction(11)), elem(conjunction(12)), set(0), set(1), set(2), set(3), set(4), set(5), set(6), set(7), set(8), set(9), set(10), set(11), set(12), sett(0,0,neg(atom(c))), sett(1,0,pos(atom(r))), sett(2,0,neg(atom(d))), sett(3,0,neg(atom(t))), sett(4,0,pos(atom(a))), sett(4,0,neg(atom(b))), sett(5,0,pos(atom(t))), sett(5,0,neg(atom(r))), sett(5,0,neg(atom(s))), sett(6,0,neg(atom(r))), sett(6,0,neg(atom(q))), sett(7,0,pos(atom(s))), sett(8,0,neg(atom(p))), sett(9,0,pos(atom(a))), sett(9,0,neg(atom(t))), sett(9,0,neg(atom(b))), sett(9,0,neg(atom(p))), sett(10,0,pos(atom(q))), sett(10,0,pos(atom(r))), sett(10,0,neg(atom(c))), sett(11,0,pos(atom(q))), sett(11,0,pos(atom(r))), sett(11,0,neg(atom(d))), sett(12,0,pos(atom(r))), sett(12,0,pos(atom(t))), sett(12,0,neg(atom(b))), sett(12,0,neg(atom(q))), sett(4,1,pos(atom(a))), sett(5,1,pos(atom(t))), sett(5,1,neg(atom(s))), sett(6,1,neg(atom(r))), sett(9,1,pos(atom(a))), sett(9,1,neg(atom(t))), sett(9,1,neg(atom(p))), sett(10,1,pos(atom(r))), sett(10,1,pos(atom(q))), sett(11,1,pos(atom(r))), sett(11,1,pos(atom(q))), sett(12,1,pos(atom(t))), sett(12,1,pos(atom(r))), sett(12,1,neg(atom(q))), sett(5,2,pos(atom(t))), sett(9,2,pos(atom(a))), sett(9,2,neg(atom(t))), sett(10,2,pos(atom(r))), sett(11,2,pos(atom(r))), sett(12,2,pos(atom(t))), sett(12,2,pos(atom(r))), sett(9,3,pos(atom(a))), sett(12,3,pos(atom(t))), setn(0,0,neg(atom(c))), setn(1,0,pos(atom(r))), setn(2,0,neg(atom(d))), setn(3,0,neg(atom(t))), setn(4,0,neg(atom(b))), setn(5,0,neg(atom(r))), setn(6,0,neg(atom(q))), setn(7,0,pos(atom(s))), setn(8,0,neg(atom(p))), setn(9,0,neg(atom(b))), setn(10,0,neg(atom(c))), setn(11,0,neg(atom(d))), setn(12,0,neg(atom(b))), setn(4,1,pos(atom(a))), setn(5,1,neg(atom(s))), setn(6,1,neg(atom(r))), setn(9,1,neg(atom(p))), setn(10,1,pos(atom(q))), setn(11,1,pos(atom(q))), setn(12,1,neg(atom(q))), setn(5,2,pos(atom(t))), setn(9,2,neg(atom(t))), setn(10,2,pos(atom(r))), setn(11,2,pos(atom(r))), setn(12,2,pos(atom(r))), setn(9,3,pos(atom(a))), setn(12,3,pos(atom(t))), setn(0,0), setn(1,0), setn(2,0), setn(3,0), setn(4,0), setn(5,0), setn(6,0), setn(7,0), setn(8,0), setn(9,0), setn(10,0), setn(11,0), setn(12,0), setn(4,1), setn(5,1), setn(6,1), setn(9,1), setn(10,1), setn(11,1), setn(12,1), setn(5,2), setn(9,2), setn(10,2), setn(11,2), setn(12,2), setn(9,3), setn(12,3), target(atom(q),0), target(atom(r),1), target(atom(p),2), target(atom(s),3), target(0), target(1), target(2), target(3), hold(conjunction(0)), hold(conjunction(1)), hold(conjunction(3)), hold(conjunction(4)), hold(atom(a)), hold(atom(d)), hold(atom(r)), hold(atom(p)), equal(4), true(atom(q)), fail(atom(q)), true(atom(r)), fail(atom(r)), true(atom(p)), fail(atom(p)), true(atom(t)), fail(atom(t)), true(atom(a)), fail(atom(a)), true(atom(d)), fail(atom(d)), true(atom(b)), fail(atom(b)), true(atom(s)), fail(atom(s)), true(atom(c)), fail(atom(c)), suppf(atom(p),1), suppf(atom(t),1), suppf(atom(d),1), suppf(atom(b),1), suppf(atom(s),1), suppf(atom(c),1), suppf(atom(q),2), suppf(atom(r),2), suppf(atom(a),3), true(conjunction(0),1), true(conjunction(1),1), true(conjunction(2),1), true(conjunction(3),1), true(conjunction(7),1), true(conjunction(8),1), true(conjunction(4),2), true(conjunction(6),2), true(conjunction(5),3), true(conjunction(10),3), true(conjunction(11),3), true(conjunction(9),4), true(conjunction(12),4), true(conjunction(0),0), true(conjunction(2),0), true(conjunction(3),0), true(conjunction(8),0), true(conjunction(6),1), true(conjunction(1),0), true(conjunction(7),0), true(conjunction(4),1), true(conjunction(5),2), true(conjunction(10),2), true(conjunction(11),2), true(conjunction(9),3), true(conjunction(12),3), bot, fail(conjunction(0)), fail(conjunction(2)), fail(conjunction(3)), fail(conjunction(4)), fail(conjunction(5)), fail(conjunction(6)), fail(conjunction(8)), fail(conjunction(9)), fail(conjunction(10)), fail(conjunction(11)), fail(conjunction(12)), fail(conjunction(1)), fail(conjunction(7)), equal(3), true(conjunction(4),0), true(conjunction(6),0), true(conjunction(5),1), true(conjunction(9),2), true(conjunction(10),1), true(conjunction(11),1), true(conjunction(12),2), suppf(atom(p),0), suppf(atom(t),0), suppf(atom(d),0), suppf(atom(b),0), suppf(atom(s),0), suppf(atom(c),0), suppf(atom(q),1), suppf(atom(r),1), suppf(atom(a),2), true(conjunction(0)), true(conjunction(2)), true(conjunction(3)), true(conjunction(8)), true(conjunction(1)), true(conjunction(7)), equal(2), true(conjunction(5),0), true(conjunction(10),0), true(conjunction(11),0), true(conjunction(9),1), true(conjunction(12),1), suppf(atom(q),0), suppf(atom(r),0), suppf(atom(a),1), true(conjunction(4)), true(conjunction(6)), equal(1), true(conjunction(9),0), true(conjunction(12),0), suppf(atom(a),0), true(conjunction(5)), true(conjunction(10)), true(conjunction(11)), equal(0), true(conjunction(9)), true(conjunction(12)), nhold(conjunction(2)), nhold(conjunction(5)), nhold(conjunction(6)), nhold(conjunction(8)), nhold(conjunction(9)), nhold(conjunction(10)), nhold(conjunction(11)), nhold(conjunction(12)), nhold(conjunction(7))}
"""
