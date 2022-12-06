input = """
% instance

field(1,1).
field(1,2).
field(1,3).
field(1,4).
field(2,1).
field(2,2).
field(2,3).
field(2,4).
field(3,1).
field(3,2).
field(3,3).
field(3,4).
field(4,1).
field(4,2).
field(4,3).
field(4,4).
init_on(3,2).
goal_on(1,4).
connect(1,1,s).
connect(1,1,w).
connect(1,2,n).
connect(1,2,e).
connect(1,2,w).
connect(1,3,e).
connect(1,4,n).
connect(1,4,w).
connect(2,1,n).
connect(2,1,w).
connect(2,2,n).
connect(2,2,w).
connect(2,3,n).
connect(2,3,s).
connect(2,4,n).
connect(2,4,e).
connect(2,4,w).
connect(3,1,n).
connect(3,1,s).
connect(3,2,n).
connect(3,2,w).
connect(3,3,e).
connect(3,4,n).
connect(3,4,s).
connect(3,4,w).
connect(4,1,n).
connect(4,1,w).
connect(4,2,n).
connect(4,2,s).
connect(4,2,e).
connect(4,2,w).
connect(4,3,n).
connect(4,3,e).
connect(4,4,w).
max_steps(2).

% encoding

dir(e). dir(w). dir(n). dir(s).
inverse(e,w). inverse(w,e).
inverse(n,s). inverse(s,n).

row(X) :- field(X,Y).
col(Y) :- field(X,Y).

num_rows(X) :- row(X), not row(XX), XX = X+1.
num_cols(Y) :- col(Y), not col(YY), YY = Y+1.

goal(X,Y,0)   :- goal_on(X,Y).
reach(X,Y,0)  :- init_on(X,Y).
conn(X,Y,D,0) :- connect(X,Y,D).

step(S) :- max_steps(S),     0 < S.
step(T) :- step(S), T = S-1, 1 < S.

%%  Direct neighbors

dneighbor(n,X,Y,XX,Y) :- field(X,Y), field(XX,Y), XX = X+1.
dneighbor(s,X,Y,XX,Y) :- field(X,Y), field(XX,Y), XX = X-1.
dneighbor(e,X,Y,X,YY) :- field(X,Y), field(X,YY), YY = Y+1.
dneighbor(w,X,Y,X,YY) :- field(X,Y), field(X,YY), YY = Y-1.

%%  All neighboring fields

neighbor(D,X,Y,XX,YY) :- dneighbor(D,X,Y,XX,YY).
neighbor(n,X,Y, 1, Y) :- field(X,Y), num_rows(X).
neighbor(s,1,Y, X, Y) :- field(X,Y), num_rows(X).
neighbor(e,X,Y, X, 1) :- field(X,Y), num_cols(Y).
neighbor(w,X,1, X, Y) :- field(X,Y), num_cols(Y).

%%  Select a row or column to push

neg_goal(T) :- goal(X,Y,T), not reach(X,Y,T).

rrpush(T)   :- step(T), neg_goal(S), S = T-1, not ccpush(T).
ccpush(T)   :- step(T), neg_goal(S), S = T-1, not rrpush(T).

orpush(X,T) :- row(X), row(XX), rpush(XX,T), X != XX.
ocpush(Y,T) :- col(Y), col(YY), cpush(YY,T), Y != YY.

rpush(X,T)  :- row(X), rrpush(T), not orpush(X,T).
cpush(Y,T)  :- col(Y), ccpush(T), not ocpush(Y,T).

push(X,e,T) :- rpush(X,T), not push(X,w,T).
push(X,w,T) :- rpush(X,T), not push(X,e,T).
push(Y,n,T) :- cpush(Y,T), not push(Y,s,T).
push(Y,s,T) :- cpush(Y,T), not push(Y,n,T).

%%  Determine new position of a (pushed) field

shift(XX,YY,X,Y,T) :- neighbor(e,XX,YY,X,Y), push(XX,e,T), step(T).
shift(XX,YY,X,Y,T) :- neighbor(w,XX,YY,X,Y), push(XX,w,T), step(T).
shift(XX,YY,X,Y,T) :- neighbor(n,XX,YY,X,Y), push(YY,n,T), step(T).
shift(XX,YY,X,Y,T) :- neighbor(s,XX,YY,X,Y), push(YY,s,T), step(T).
shift( X, Y,X,Y,T) :- field(X,Y), not push(X,e,T), not push(X,w,T), not push(Y,n,T), not push(Y,s,T), step(T).

%%  Move connections around

conn(X,Y,D,T) :- conn(XX,YY,D,S), S = T-1, dir(D), shift(XX,YY,X,Y,T), step(T).

%%  Location of goal after pushing

goal(X,Y,T) :- goal(XX,YY,S), S = T-1, shift(XX,YY,X,Y,T), step(T).

%%  Locations reachable from new position

reach(X,Y,T) :- reach(XX,YY,S), S = T-1, shift(XX,YY,X,Y,T), step(T).
reach(X,Y,T) :- reach(XX,YY,T), dneighbor(D,XX,YY,X,Y), conn(XX,YY,D,T), conn(X,Y,E,T), inverse(D,E), step(T).

%%  Goal must be reached

:- neg_goal(S), max_steps(S).

%% Project output

% #hide.
% #show push(Z,D,T).


"""
output = """
{ccpush(2), col(1), col(2), col(3), col(4), conn(1,1,e,1), conn(1,1,e,2), conn(1,1,n,1), conn(1,1,n,2), conn(1,1,s,0), conn(1,1,w,0), conn(1,1,w,1), conn(1,1,w,2), conn(1,2,e,0), conn(1,2,e,1), conn(1,2,e,2), conn(1,2,n,0), conn(1,2,w,0), conn(1,3,e,0), conn(1,3,n,1), conn(1,3,n,2), conn(1,3,s,2), conn(1,3,w,1), conn(1,4,n,0), conn(1,4,s,1), conn(1,4,s,2), conn(1,4,w,0), conn(1,4,w,1), conn(1,4,w,2), conn(2,1,n,0), conn(2,1,n,1), conn(2,1,n,2), conn(2,1,w,0), conn(2,1,w,1), conn(2,1,w,2), conn(2,2,n,0), conn(2,2,n,1), conn(2,2,n,2), conn(2,2,w,0), conn(2,2,w,1), conn(2,2,w,2), conn(2,3,e,2), conn(2,3,n,0), conn(2,3,n,1), conn(2,3,s,0), conn(2,3,s,1), conn(2,4,e,0), conn(2,4,e,1), conn(2,4,e,2), conn(2,4,n,0), conn(2,4,n,1), conn(2,4,n,2), conn(2,4,w,0), conn(2,4,w,1), conn(2,4,w,2), conn(3,1,n,0), conn(3,1,n,1), conn(3,1,n,2), conn(3,1,s,0), conn(3,1,s,1), conn(3,1,s,2), conn(3,2,n,0), conn(3,2,n,1), conn(3,2,n,2), conn(3,2,w,0), conn(3,2,w,1), conn(3,2,w,2), conn(3,3,e,0), conn(3,3,e,1), conn(3,3,e,2), conn(3,3,n,2), conn(3,4,n,0), conn(3,4,n,1), conn(3,4,n,2), conn(3,4,s,0), conn(3,4,s,1), conn(3,4,s,2), conn(3,4,w,0), conn(3,4,w,1), conn(3,4,w,2), conn(4,1,n,0), conn(4,1,n,1), conn(4,1,n,2), conn(4,1,w,0), conn(4,1,w,1), conn(4,1,w,2), conn(4,2,e,0), conn(4,2,e,1), conn(4,2,e,2), conn(4,2,n,0), conn(4,2,n,1), conn(4,2,n,2), conn(4,2,s,0), conn(4,2,s,1), conn(4,2,s,2), conn(4,2,w,0), conn(4,2,w,1), conn(4,2,w,2), conn(4,3,e,0), conn(4,3,e,1), conn(4,3,n,0), conn(4,3,n,1), conn(4,3,n,2), conn(4,3,w,2), conn(4,4,w,0), conn(4,4,w,1), conn(4,4,w,2), connect(1,1,s), connect(1,1,w), connect(1,2,e), connect(1,2,n), connect(1,2,w), connect(1,3,e), connect(1,4,n), connect(1,4,w), connect(2,1,n), connect(2,1,w), connect(2,2,n), connect(2,2,w), connect(2,3,n), connect(2,3,s), connect(2,4,e), connect(2,4,n), connect(2,4,w), connect(3,1,n), connect(3,1,s), connect(3,2,n), connect(3,2,w), connect(3,3,e), connect(3,4,n), connect(3,4,s), connect(3,4,w), connect(4,1,n), connect(4,1,w), connect(4,2,e), connect(4,2,n), connect(4,2,s), connect(4,2,w), connect(4,3,e), connect(4,3,n), connect(4,4,w), cpush(3,2), dir(e), dir(n), dir(s), dir(w), dneighbor(e,1,1,1,2), dneighbor(e,1,2,1,3), dneighbor(e,1,3,1,4), dneighbor(e,2,1,2,2), dneighbor(e,2,2,2,3), dneighbor(e,2,3,2,4), dneighbor(e,3,1,3,2), dneighbor(e,3,2,3,3), dneighbor(e,3,3,3,4), dneighbor(e,4,1,4,2), dneighbor(e,4,2,4,3), dneighbor(e,4,3,4,4), dneighbor(n,1,1,2,1), dneighbor(n,1,2,2,2), dneighbor(n,1,3,2,3), dneighbor(n,1,4,2,4), dneighbor(n,2,1,3,1), dneighbor(n,2,2,3,2), dneighbor(n,2,3,3,3), dneighbor(n,2,4,3,4), dneighbor(n,3,1,4,1), dneighbor(n,3,2,4,2), dneighbor(n,3,3,4,3), dneighbor(n,3,4,4,4), dneighbor(s,2,1,1,1), dneighbor(s,2,2,1,2), dneighbor(s,2,3,1,3), dneighbor(s,2,4,1,4), dneighbor(s,3,1,2,1), dneighbor(s,3,2,2,2), dneighbor(s,3,3,2,3), dneighbor(s,3,4,2,4), dneighbor(s,4,1,3,1), dneighbor(s,4,2,3,2), dneighbor(s,4,3,3,3), dneighbor(s,4,4,3,4), dneighbor(w,1,2,1,1), dneighbor(w,1,3,1,2), dneighbor(w,1,4,1,3), dneighbor(w,2,2,2,1), dneighbor(w,2,3,2,2), dneighbor(w,2,4,2,3), dneighbor(w,3,2,3,1), dneighbor(w,3,3,3,2), dneighbor(w,3,4,3,3), dneighbor(w,4,2,4,1), dneighbor(w,4,3,4,2), dneighbor(w,4,4,4,3), field(1,1), field(1,2), field(1,3), field(1,4), field(2,1), field(2,2), field(2,3), field(2,4), field(3,1), field(3,2), field(3,3), field(3,4), field(4,1), field(4,2), field(4,3), field(4,4), goal(1,3,1), goal(1,4,0), goal(4,3,2), goal_on(1,4), init_on(3,2), inverse(e,w), inverse(n,s), inverse(s,n), inverse(w,e), max_steps(2), neg_goal(0), neg_goal(1), neighbor(e,1,1,1,2), neighbor(e,1,2,1,3), neighbor(e,1,3,1,4), neighbor(e,1,4,1,1), neighbor(e,2,1,2,2), neighbor(e,2,2,2,3), neighbor(e,2,3,2,4), neighbor(e,2,4,2,1), neighbor(e,3,1,3,2), neighbor(e,3,2,3,3), neighbor(e,3,3,3,4), neighbor(e,3,4,3,1), neighbor(e,4,1,4,2), neighbor(e,4,2,4,3), neighbor(e,4,3,4,4), neighbor(e,4,4,4,1), neighbor(n,1,1,2,1), neighbor(n,1,2,2,2), neighbor(n,1,3,2,3), neighbor(n,1,4,2,4), neighbor(n,2,1,3,1), neighbor(n,2,2,3,2), neighbor(n,2,3,3,3), neighbor(n,2,4,3,4), neighbor(n,3,1,4,1), neighbor(n,3,2,4,2), neighbor(n,3,3,4,3), neighbor(n,3,4,4,4), neighbor(n,4,1,1,1), neighbor(n,4,2,1,2), neighbor(n,4,3,1,3), neighbor(n,4,4,1,4), neighbor(s,1,1,4,1), neighbor(s,1,2,4,2), neighbor(s,1,3,4,3), neighbor(s,1,4,4,4), neighbor(s,2,1,1,1), neighbor(s,2,2,1,2), neighbor(s,2,3,1,3), neighbor(s,2,4,1,4), neighbor(s,3,1,2,1), neighbor(s,3,2,2,2), neighbor(s,3,3,2,3), neighbor(s,3,4,2,4), neighbor(s,4,1,3,1), neighbor(s,4,2,3,2), neighbor(s,4,3,3,3), neighbor(s,4,4,3,4), neighbor(w,1,1,1,4), neighbor(w,1,2,1,1), neighbor(w,1,3,1,2), neighbor(w,1,4,1,3), neighbor(w,2,1,2,4), neighbor(w,2,2,2,1), neighbor(w,2,3,2,2), neighbor(w,2,4,2,3), neighbor(w,3,1,3,4), neighbor(w,3,2,3,1), neighbor(w,3,3,3,2), neighbor(w,3,4,3,3), neighbor(w,4,1,4,4), neighbor(w,4,2,4,1), neighbor(w,4,3,4,2), neighbor(w,4,4,4,3), num_cols(4), num_rows(4), ocpush(1,2), ocpush(2,2), ocpush(4,2), orpush(2,1), orpush(3,1), orpush(4,1), push(1,w,1), push(3,s,2), reach(3,2,0), reach(3,2,1), reach(3,2,2), reach(4,2,1), reach(4,2,2), reach(4,3,2), row(1), row(2), row(3), row(4), rpush(1,1), rrpush(1), shift(1,1,1,1,2), shift(1,1,1,4,1), shift(1,2,1,1,1), shift(1,2,1,2,2), shift(1,3,1,2,1), shift(1,3,4,3,2), shift(1,4,1,3,1), shift(1,4,1,4,2), shift(2,1,2,1,1), shift(2,1,2,1,2), shift(2,2,2,2,1), shift(2,2,2,2,2), shift(2,3,1,3,2), shift(2,3,2,3,1), shift(2,4,2,4,1), shift(2,4,2,4,2), shift(3,1,3,1,1), shift(3,1,3,1,2), shift(3,2,3,2,1), shift(3,2,3,2,2), shift(3,3,2,3,2), shift(3,3,3,3,1), shift(3,4,3,4,1), shift(3,4,3,4,2), shift(4,1,4,1,1), shift(4,1,4,1,2), shift(4,2,4,2,1), shift(4,2,4,2,2), shift(4,3,3,3,2), shift(4,3,4,3,1), shift(4,4,4,4,1), shift(4,4,4,4,2), step(1), step(2)}
{ccpush(2), col(1), col(2), col(3), col(4), conn(1,1,e,1), conn(1,1,e,2), conn(1,1,n,1), conn(1,1,n,2), conn(1,1,s,0), conn(1,1,w,0), conn(1,1,w,1), conn(1,1,w,2), conn(1,2,e,0), conn(1,2,e,1), conn(1,2,e,2), conn(1,2,n,0), conn(1,2,n,2), conn(1,2,s,2), conn(1,2,w,0), conn(1,2,w,2), conn(1,3,e,0), conn(1,3,n,1), conn(1,3,n,2), conn(1,3,w,1), conn(1,3,w,2), conn(1,4,n,0), conn(1,4,s,1), conn(1,4,s,2), conn(1,4,w,0), conn(1,4,w,1), conn(1,4,w,2), conn(2,1,n,0), conn(2,1,n,1), conn(2,1,n,2), conn(2,1,w,0), conn(2,1,w,1), conn(2,1,w,2), conn(2,2,e,2), conn(2,2,n,0), conn(2,2,n,1), conn(2,2,w,0), conn(2,2,w,1), conn(2,3,n,0), conn(2,3,n,1), conn(2,3,n,2), conn(2,3,s,0), conn(2,3,s,1), conn(2,3,s,2), conn(2,4,e,0), conn(2,4,e,1), conn(2,4,e,2), conn(2,4,n,0), conn(2,4,n,1), conn(2,4,n,2), conn(2,4,w,0), conn(2,4,w,1), conn(2,4,w,2), conn(3,1,n,0), conn(3,1,n,1), conn(3,1,n,2), conn(3,1,s,0), conn(3,1,s,1), conn(3,1,s,2), conn(3,2,n,0), conn(3,2,n,1), conn(3,2,n,2), conn(3,2,w,0), conn(3,2,w,1), conn(3,2,w,2), conn(3,3,e,0), conn(3,3,e,1), conn(3,3,e,2), conn(3,4,n,0), conn(3,4,n,1), conn(3,4,n,2), conn(3,4,s,0), conn(3,4,s,1), conn(3,4,s,2), conn(3,4,w,0), conn(3,4,w,1), conn(3,4,w,2), conn(4,1,n,0), conn(4,1,n,1), conn(4,1,n,2), conn(4,1,w,0), conn(4,1,w,1), conn(4,1,w,2), conn(4,2,e,0), conn(4,2,e,1), conn(4,2,n,0), conn(4,2,n,1), conn(4,2,n,2), conn(4,2,s,0), conn(4,2,s,1), conn(4,2,w,0), conn(4,2,w,1), conn(4,2,w,2), conn(4,3,e,0), conn(4,3,e,1), conn(4,3,e,2), conn(4,3,n,0), conn(4,3,n,1), conn(4,3,n,2), conn(4,4,w,0), conn(4,4,w,1), conn(4,4,w,2), connect(1,1,s), connect(1,1,w), connect(1,2,e), connect(1,2,n), connect(1,2,w), connect(1,3,e), connect(1,4,n), connect(1,4,w), connect(2,1,n), connect(2,1,w), connect(2,2,n), connect(2,2,w), connect(2,3,n), connect(2,3,s), connect(2,4,e), connect(2,4,n), connect(2,4,w), connect(3,1,n), connect(3,1,s), connect(3,2,n), connect(3,2,w), connect(3,3,e), connect(3,4,n), connect(3,4,s), connect(3,4,w), connect(4,1,n), connect(4,1,w), connect(4,2,e), connect(4,2,n), connect(4,2,s), connect(4,2,w), connect(4,3,e), connect(4,3,n), connect(4,4,w), cpush(2,2), dir(e), dir(n), dir(s), dir(w), dneighbor(e,1,1,1,2), dneighbor(e,1,2,1,3), dneighbor(e,1,3,1,4), dneighbor(e,2,1,2,2), dneighbor(e,2,2,2,3), dneighbor(e,2,3,2,4), dneighbor(e,3,1,3,2), dneighbor(e,3,2,3,3), dneighbor(e,3,3,3,4), dneighbor(e,4,1,4,2), dneighbor(e,4,2,4,3), dneighbor(e,4,3,4,4), dneighbor(n,1,1,2,1), dneighbor(n,1,2,2,2), dneighbor(n,1,3,2,3), dneighbor(n,1,4,2,4), dneighbor(n,2,1,3,1), dneighbor(n,2,2,3,2), dneighbor(n,2,3,3,3), dneighbor(n,2,4,3,4), dneighbor(n,3,1,4,1), dneighbor(n,3,2,4,2), dneighbor(n,3,3,4,3), dneighbor(n,3,4,4,4), dneighbor(s,2,1,1,1), dneighbor(s,2,2,1,2), dneighbor(s,2,3,1,3), dneighbor(s,2,4,1,4), dneighbor(s,3,1,2,1), dneighbor(s,3,2,2,2), dneighbor(s,3,3,2,3), dneighbor(s,3,4,2,4), dneighbor(s,4,1,3,1), dneighbor(s,4,2,3,2), dneighbor(s,4,3,3,3), dneighbor(s,4,4,3,4), dneighbor(w,1,2,1,1), dneighbor(w,1,3,1,2), dneighbor(w,1,4,1,3), dneighbor(w,2,2,2,1), dneighbor(w,2,3,2,2), dneighbor(w,2,4,2,3), dneighbor(w,3,2,3,1), dneighbor(w,3,3,3,2), dneighbor(w,3,4,3,3), dneighbor(w,4,2,4,1), dneighbor(w,4,3,4,2), dneighbor(w,4,4,4,3), field(1,1), field(1,2), field(1,3), field(1,4), field(2,1), field(2,2), field(2,3), field(2,4), field(3,1), field(3,2), field(3,3), field(3,4), field(4,1), field(4,2), field(4,3), field(4,4), goal(1,3,1), goal(1,3,2), goal(1,4,0), goal_on(1,4), init_on(3,2), inverse(e,w), inverse(n,s), inverse(s,n), inverse(w,e), max_steps(2), neg_goal(0), neg_goal(1), neighbor(e,1,1,1,2), neighbor(e,1,2,1,3), neighbor(e,1,3,1,4), neighbor(e,1,4,1,1), neighbor(e,2,1,2,2), neighbor(e,2,2,2,3), neighbor(e,2,3,2,4), neighbor(e,2,4,2,1), neighbor(e,3,1,3,2), neighbor(e,3,2,3,3), neighbor(e,3,3,3,4), neighbor(e,3,4,3,1), neighbor(e,4,1,4,2), neighbor(e,4,2,4,3), neighbor(e,4,3,4,4), neighbor(e,4,4,4,1), neighbor(n,1,1,2,1), neighbor(n,1,2,2,2), neighbor(n,1,3,2,3), neighbor(n,1,4,2,4), neighbor(n,2,1,3,1), neighbor(n,2,2,3,2), neighbor(n,2,3,3,3), neighbor(n,2,4,3,4), neighbor(n,3,1,4,1), neighbor(n,3,2,4,2), neighbor(n,3,3,4,3), neighbor(n,3,4,4,4), neighbor(n,4,1,1,1), neighbor(n,4,2,1,2), neighbor(n,4,3,1,3), neighbor(n,4,4,1,4), neighbor(s,1,1,4,1), neighbor(s,1,2,4,2), neighbor(s,1,3,4,3), neighbor(s,1,4,4,4), neighbor(s,2,1,1,1), neighbor(s,2,2,1,2), neighbor(s,2,3,1,3), neighbor(s,2,4,1,4), neighbor(s,3,1,2,1), neighbor(s,3,2,2,2), neighbor(s,3,3,2,3), neighbor(s,3,4,2,4), neighbor(s,4,1,3,1), neighbor(s,4,2,3,2), neighbor(s,4,3,3,3), neighbor(s,4,4,3,4), neighbor(w,1,1,1,4), neighbor(w,1,2,1,1), neighbor(w,1,3,1,2), neighbor(w,1,4,1,3), neighbor(w,2,1,2,4), neighbor(w,2,2,2,1), neighbor(w,2,3,2,2), neighbor(w,2,4,2,3), neighbor(w,3,1,3,4), neighbor(w,3,2,3,1), neighbor(w,3,3,3,2), neighbor(w,3,4,3,3), neighbor(w,4,1,4,4), neighbor(w,4,2,4,1), neighbor(w,4,3,4,2), neighbor(w,4,4,4,3), num_cols(4), num_rows(4), ocpush(1,2), ocpush(3,2), ocpush(4,2), orpush(2,1), orpush(3,1), orpush(4,1), push(1,w,1), push(2,n,2), reach(1,1,2), reach(1,2,2), reach(1,3,2), reach(2,3,2), reach(3,2,0), reach(3,2,1), reach(4,2,1), reach(4,2,2), row(1), row(2), row(3), row(4), rpush(1,1), rrpush(1), shift(1,1,1,1,2), shift(1,1,1,4,1), shift(1,2,1,1,1), shift(1,2,2,2,2), shift(1,3,1,2,1), shift(1,3,1,3,2), shift(1,4,1,3,1), shift(1,4,1,4,2), shift(2,1,2,1,1), shift(2,1,2,1,2), shift(2,2,2,2,1), shift(2,2,3,2,2), shift(2,3,2,3,1), shift(2,3,2,3,2), shift(2,4,2,4,1), shift(2,4,2,4,2), shift(3,1,3,1,1), shift(3,1,3,1,2), shift(3,2,3,2,1), shift(3,2,4,2,2), shift(3,3,3,3,1), shift(3,3,3,3,2), shift(3,4,3,4,1), shift(3,4,3,4,2), shift(4,1,4,1,1), shift(4,1,4,1,2), shift(4,2,1,2,2), shift(4,2,4,2,1), shift(4,3,4,3,1), shift(4,3,4,3,2), shift(4,4,4,4,1), shift(4,4,4,4,2), step(1), step(2)}
"""
