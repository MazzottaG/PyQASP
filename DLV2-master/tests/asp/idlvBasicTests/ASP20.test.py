input = """
% instance

vertex(0). vertex(1). vertex(2). vertex(3). vertex(4). 
obs_vlabel(1,p). obs_vlabel(3,p).

edge(0,1).         edge(0,3).         edge(0,4).
obs_elabel(0,1,p). obs_elabel(0,3,m). obs_elabel(0,4,m).

edge(1,0).
obs_elabel(1,0,p).

edge(1,2).
obs_elabel(1,2,p).

edge(2,4).
obs_elabel(2,4,m).

edge(3,1).         edge(3,2).         edge(3,4).
obs_elabel(3,1,p). obs_elabel(3,2,p). obs_elabel(3,4,p).

% encoding

%%%%%%%%%%%%%%%%%
% Preprocessing %
%%%%%%%%%%%%%%%%%

sign(m). sign(p).

diff(V,V)  :- edge(V,V), obs_elabel(V,V,m), not obs_elabel(V,V, p).
diff(U,V)  :- edge(U,V), obs_elabel(U,V,m), not obs_elabel(U,V, p), obs_vlabel(U,S), obs_vlabel(V,S).
diff(U,V)  :- edge(U,V), obs_elabel(U,V, p), not obs_elabel(U,V,m), obs_vlabel(U,S), obs_vlabel(V,T), S != T.

nontriv(V) :- vertex(V), not input(V), edge(U,V), not diff(U,V).
trivial(V) :- vertex(V), not input(V), not nontriv(V).

btedge(W,U,V) :- vertex(V), not input(V), not trivial(V), edge(W,V), edge(U,V), edge(Z,V), W < Z, Z < U.
ntedge(W,U,V) :- vertex(V), not input(V), not trivial(V), edge(W,V), edge(U,V), W < U, not btedge(W,U,V).
nfirst(U,V)   :- ntedge(W,U,V).
nlast(W,V)    :- ntedge(W,U,V).
first(U,V)    :- vertex(V), not input(V), not trivial(V), edge(U,V), not nfirst(U,V).
last(U,V)     :- vertex(V), not input(V), not trivial(V), edge(U,V), not nlast(U,V).


%%%%%%%%%%%%%
% Generator %
%%%%%%%%%%%%%

active(V) | inactive(V) :- vertex(V), not input(V).
inactive(V)             :- vertex(V), not input(V), active(W), trivial(W), V != W.
singleton               :- active(V), trivial(V).

reach(U,V) :- edge(U,V), active(V), not trivial(V).
reach(V,U) :- edge(U,V), active(V), not trivial(V),                        not obs_vlabel(U,p), not obs_vlabel(U,m).
reach(U,W) :- edge(U,V), active(V), not trivial(V), reach(V,W), vertex(W).
reach(V,W) :- edge(U,V), active(V), not trivial(V), reach(U,W), vertex(W), not obs_vlabel(U,p), not obs_vlabel(U,m).

aedge(V) :- vertex(V), not input(V), not trivial(V), not obs_vlabel(V,p), not obs_vlabel(V,m), active(W), edge(V,W).

:- active(V), not trivial(V), active(W), not trivial(W), not reach(V,W).
:- active(V), not trivial(V), not obs_vlabel(V,p), not obs_vlabel(V,m), not aedge(V).


%%%%%%%%%%%%%%%%%%%%%%
% Inconsistency Test %
%%%%%%%%%%%%%%%%%%%%%%

vlabel(V,p)   | vlabel(V,m)   :- active(V),    not trivial(V),               not obs_vlabel(V,p),   not obs_vlabel(V,m).
vlabel(U,p)   | vlabel(U,m)   :- active(V),    not trivial(V), edge(U,V),    not obs_vlabel(U,p),   not obs_vlabel(U,m).
llabel(U,V,p) | llabel(U,V,m) :- active(V),    not trivial(V), edge(U,V),    not obs_elabel(U,V,p), not obs_elabel(U,V,m).

vlabel(V,S)   :- vertex(V), obs_vlabel(V,S),   not trivial(V), not input(V).
vlabel(U,S)   :- edge(U,V), obs_vlabel(U,S),   not trivial(V), not input(V).
llabel(U,V,S) :- edge(U,V), obs_elabel(U,V,S), not trivial(V), not input(V).

oppo(U,V)     :- llabel(U,V,m), vlabel(U,S),   not trivial(V), not input(V), not obs_elabel(U,V,p), active(V), vlabel(V,S).
oppo(U,V)     :- llabel(U,V,p), vlabel(U,S),   not trivial(V), not input(V), not obs_elabel(U,V,m), active(V), vlabel(V,T), S != T.

coppo(U,V)    :- oppo(U,V), first(U,V).
coppo(U,V)    :- oppo(U,V), coppo(W,V), ntedge(W,U,V).

bot           :- singleton.
bot           :- active(V), coppo(U,V), last(U,V).

vlabel(V,S)   :- bot, vertex(V), sign(S),      not trivial(V), not input(V), not obs_vlabel(V,p),   not obs_vlabel(V,m).
vlabel(U,S)   :- bot, edge(U,V), sign(S),      not trivial(V), not input(V), not obs_vlabel(U,p),   not obs_vlabel(U,m).
llabel(U,V,S) :- bot, edge(U,V), sign(S),      not trivial(V), not input(V), not obs_elabel(U,V,p), not obs_elabel(U,V,m).

:- not bot.


%%%%%%%%%%%%%%%%%%%
% Minimality Test %
%%%%%%%%%%%%%%%%%%%

mvlabel(W,V,p)   | mvlabel(W,V,m)   :-                active(W), not trivial(V), not trivial(W), active(V), reach(V,W), reach(W,V), W != V.
mvlabel(W,U,p)   | mvlabel(W,U,m)   :- edge(U,V),     active(W), not trivial(V), not trivial(W), active(V), reach(V,W), reach(W,V), W != V.
mllabel(W,U,V,p) | mllabel(W,U,V,m) :- edge(U,V),     active(W), not trivial(V), not trivial(W), active(V), reach(V,W), reach(W,V), W != V.

mvlabel(W,V,S)   :- obs_vlabel(V,S),                  vertex(W), not trivial(V), not trivial(W), not input(V), not input(W),        W != V.
mvlabel(W,U,S)   :- obs_vlabel(U,S),    edge(U,V),    vertex(W), not trivial(V), not trivial(W), not input(V), not input(W),        W != V.
mllabel(W,U,V,S) :- obs_elabel(U,V,S),  edge(U,V),    vertex(W), not trivial(V), not trivial(W), not input(V), not input(W),        W != V.

minfl(W,V,p)     :- mvlabel(W,U,S), mllabel(W,U,V,S), active(W), not trivial(V), not trivial(W), active(V), reach(V,W), reach(W,V), W != V.
minfl(W,V,m)     :- mvlabel(W,U,S), mllabel(W,U,V,T), active(W), not trivial(V), not trivial(W), active(V), reach(V,W), reach(W,V), W != V, S != T.

:- active(V), active(W), not trivial(V), not trivial(W), W != V, mvlabel(W,V,S), not minfl(W,V,S).


%%%%%%%%%%
% Output %
%%%%%%%%%%

% #hide.
% #show active(V).

"""
output = """
{active(0), active(3), aedge(0), bot, btedge(0,3,4), coppo(0,3), coppo(1,0), edge(0,1), edge(0,3), edge(0,4), edge(1,0), edge(1,2), edge(2,4), edge(3,1), edge(3,2), edge(3,4), first(0,1), first(0,3), first(0,4), first(1,0), first(1,2), inactive(1), inactive(2), inactive(4), last(0,3), last(1,0), last(3,1), last(3,2), last(3,4), llabel(0,1,p), llabel(0,3,m), llabel(0,4,m), llabel(1,0,p), llabel(1,2,p), llabel(2,4,m), llabel(3,1,p), llabel(3,2,p), llabel(3,4,p), minfl(0,3,p), minfl(3,0,p), mllabel(0,0,1,p), mllabel(0,0,3,m), mllabel(0,0,4,m), mllabel(0,1,2,p), mllabel(0,2,4,m), mllabel(0,3,1,p), mllabel(0,3,2,p), mllabel(0,3,4,p), mllabel(1,0,3,m), mllabel(1,0,4,m), mllabel(1,1,0,p), mllabel(1,1,2,p), mllabel(1,2,4,m), mllabel(1,3,2,p), mllabel(1,3,4,p), mllabel(2,0,1,p), mllabel(2,0,3,m), mllabel(2,0,4,m), mllabel(2,1,0,p), mllabel(2,2,4,m), mllabel(2,3,1,p), mllabel(2,3,4,p), mllabel(3,0,1,p), mllabel(3,0,4,m), mllabel(3,1,0,p), mllabel(3,1,2,p), mllabel(3,2,4,m), mllabel(3,3,1,p), mllabel(3,3,2,p), mllabel(3,3,4,p), mllabel(4,0,1,p), mllabel(4,0,3,m), mllabel(4,1,0,p), mllabel(4,1,2,p), mllabel(4,3,1,p), mllabel(4,3,2,p), mvlabel(0,0,m), mvlabel(0,1,p), mvlabel(0,3,p), mvlabel(1,1,p), mvlabel(1,3,p), mvlabel(2,1,p), mvlabel(2,3,p), mvlabel(3,0,p), mvlabel(3,1,p), mvlabel(3,3,p), mvlabel(4,1,p), mvlabel(4,3,p), nfirst(2,4), nfirst(3,1), nfirst(3,2), nfirst(3,4), nlast(0,1), nlast(0,4), nlast(1,2), nlast(2,4), nontriv(0), nontriv(1), nontriv(2), nontriv(3), nontriv(4), ntedge(0,2,4), ntedge(0,3,1), ntedge(1,3,2), ntedge(2,3,4), obs_elabel(0,1,p), obs_elabel(0,3,m), obs_elabel(0,4,m), obs_elabel(1,0,p), obs_elabel(1,2,p), obs_elabel(2,4,m), obs_elabel(3,1,p), obs_elabel(3,2,p), obs_elabel(3,4,p), obs_vlabel(1,p), obs_vlabel(3,p), oppo(0,3), oppo(1,0), reach(0,0), reach(0,3), reach(1,0), reach(1,3), reach(3,0), reach(3,3), sign(m), sign(p), vertex(0), vertex(1), vertex(2), vertex(3), vertex(4), vlabel(0,m), vlabel(0,p), vlabel(1,p), vlabel(2,m), vlabel(2,p), vlabel(3,p), vlabel(4,m), vlabel(4,p)}
"""
