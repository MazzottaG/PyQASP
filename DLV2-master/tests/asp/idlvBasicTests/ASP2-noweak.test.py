input = """
%instance

size(4).

%encoding

% possible coordinates
value(0).
value(V) :- value(V1), V=V1+1, size(N), V1<=N.

step(-1).
step(1).
diff(X,0) :- step(X).
diff(0,Y) :- step(Y).
diff(X,Y) :- step(X), step(Y).

% a cell may live, except for the ones at the border
{ lives(X,Y) } <= 1 :- value(X), value(Y), X>0, Y>0, size(N), X<=N, Y<=N.

% cells with exactly 3 neighbours must live
:- #sum { 1,DX,DY : lives(X+DX,Y+DY), diff(DX,DY) } = 3, not lives(X,Y), value(X), value(Y).

% living cells must have 2-3 living neighbours
:- lives(X,Y), not 2 <= #sum { 1,DX,DY : lives(X+DX,Y+DY), diff(DX,DY) } <= 3, value(X), value(Y).

% connectedness
reached(XX,YY) :- XX = #min { X : lives(X,Y), value(X), value(Y) }, YY = #min { Y : lives(XX,Y), value(Y) }.
reached(XX,YY) :- reached(X,Y), value(X), value(Y), diff(DX,DY), XX=X+DX, YY=Y+DY, lives(XX,YY).
:- lives(X,Y), not reached(X,Y), value(X), value(Y).

% maximise living cells
%#maximise { 1 : lives(X,Y), value(X), value(Y) }.
%:~ value(X),value(Y), not lives(X,Y). [1,X,Y]

"""
output = """
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), reached(#sup,#sup), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,4), lives(3,1), lives(3,4), lives(4,2), lives(4,3), reached(1,2), reached(1,3), reached(2,1), reached(2,4), reached(3,1), reached(3,4), reached(4,2), reached(4,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,4), lives(3,1), lives(3,3), lives(4,2), reached(1,2), reached(1,3), reached(2,1), reached(2,4), reached(3,1), reached(3,3), reached(4,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,3), lives(1,4), lives(2,4), lives(3,3), lives(4,3), lives(4,4), reached(1,3), reached(1,4), reached(2,4), reached(3,3), reached(4,3), reached(4,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(3,3), lives(3,4), lives(4,3), lives(4,4), reached(3,3), reached(3,4), reached(4,3), reached(4,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(3,1), lives(3,3), lives(3,4), lives(4,1), lives(4,2), lives(4,4), reached(3,1), reached(3,3), reached(3,4), reached(4,1), reached(4,2), reached(4,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,2), lives(2,1), lives(2,3), lives(3,1), lives(3,4), lives(4,2), lives(4,3), reached(1,2), reached(2,1), reached(2,3), reached(3,1), reached(3,4), reached(4,2), reached(4,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,2), lives(2,1), lives(2,3), lives(3,1), lives(3,3), lives(4,2), reached(1,2), reached(2,1), reached(2,3), reached(3,1), reached(3,3), reached(4,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,3), lives(1,4), lives(2,3), lives(3,1), lives(3,3), lives(4,1), lives(4,2), reached(1,3), reached(1,4), reached(2,3), reached(3,1), reached(3,3), reached(4,1), reached(4,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,3), lives(1,4), lives(2,3), lives(3,4), lives(4,3), lives(4,4), reached(1,3), reached(1,4), reached(2,3), reached(3,4), reached(4,3), reached(4,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,1), lives(1,2), lives(1,4), lives(2,1), lives(2,3), lives(2,4), reached(1,1), reached(1,2), reached(1,4), reached(2,1), reached(2,3), reached(2,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,3), lives(1,4), lives(2,3), lives(2,4), reached(1,3), reached(1,4), reached(2,3), reached(2,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(2,3), lives(2,4), lives(3,3), lives(3,4), reached(2,3), reached(2,4), reached(3,3), reached(3,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,1), lives(1,2), lives(2,1), lives(2,3), lives(3,3), lives(4,3), lives(4,4), reached(1,1), reached(1,2), reached(2,1), reached(2,3), reached(3,3), reached(4,3), reached(4,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,1), lives(1,2), lives(2,1), lives(3,2), lives(4,1), lives(4,2), reached(1,1), reached(1,2), reached(2,1), reached(3,2), reached(4,1), reached(4,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,2), lives(2,1), lives(2,3), lives(3,1), lives(3,2), reached(1,2), reached(2,1), reached(2,3), reached(3,1), reached(3,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,1), lives(1,2), lives(2,1), lives(2,3), lives(3,2), reached(1,1), reached(1,2), reached(2,1), reached(2,3), reached(3,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,2), lives(2,1), lives(2,3), lives(3,2), reached(1,2), reached(2,1), reached(2,3), reached(3,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,3), lives(3,1), lives(3,2), reached(1,2), reached(1,3), reached(2,1), reached(2,3), reached(3,1), reached(3,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,3), lives(3,2), reached(1,2), reached(1,3), reached(2,1), reached(2,3), reached(3,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,2), lives(1,3), lives(2,3), lives(3,2), lives(4,2), lives(4,3), reached(1,2), reached(1,3), reached(2,3), reached(3,2), reached(4,2), reached(4,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,1), lives(1,2), lives(2,1), lives(2,3), lives(3,2), lives(3,4), lives(4,3), lives(4,4), reached(1,1), reached(1,2), reached(2,1), reached(2,3), reached(3,2), reached(3,4), reached(4,3), reached(4,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,2), lives(2,1), lives(2,3), lives(3,2), lives(3,4), lives(4,3), lives(4,4), reached(1,2), reached(2,1), reached(2,3), reached(3,2), reached(3,4), reached(4,3), reached(4,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,1), lives(1,2), lives(2,1), lives(2,3), lives(3,2), lives(3,4), lives(4,3), reached(1,1), reached(1,2), reached(2,1), reached(2,3), reached(3,2), reached(3,4), reached(4,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,2), lives(2,1), lives(2,3), lives(3,2), lives(3,4), lives(4,3), reached(1,2), reached(2,1), reached(2,3), reached(3,2), reached(3,4), reached(4,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,4), lives(3,2), lives(3,4), lives(4,3), reached(1,2), reached(1,3), reached(2,1), reached(2,4), reached(3,2), reached(3,4), reached(4,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(3,1), lives(3,2), lives(3,4), lives(4,1), lives(4,3), lives(4,4), reached(3,1), reached(3,2), reached(3,4), reached(4,1), reached(4,3), reached(4,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(2,3), lives(3,2), lives(3,4), lives(4,3), lives(4,4), reached(2,3), reached(3,2), reached(3,4), reached(4,3), reached(4,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(2,3), lives(3,2), lives(3,4), lives(4,3), reached(2,3), reached(3,2), reached(3,4), reached(4,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(2,3), lives(3,2), lives(3,4), lives(4,2), lives(4,3), reached(2,3), reached(3,2), reached(3,4), reached(4,2), reached(4,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(2,3), lives(2,4), lives(3,2), lives(3,4), lives(4,3), reached(2,3), reached(2,4), reached(3,2), reached(3,4), reached(4,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(2,3), lives(2,4), lives(3,2), lives(3,4), lives(4,2), lives(4,3), reached(2,3), reached(2,4), reached(3,2), reached(3,4), reached(4,2), reached(4,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(3,1), lives(3,2), lives(4,1), lives(4,2), reached(3,1), reached(3,2), reached(4,1), reached(4,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(2,1), lives(2,3), lives(2,4), lives(3,1), lives(3,2), lives(3,4), reached(2,1), reached(2,3), reached(2,4), reached(3,1), reached(3,2), reached(3,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,3), lives(1,4), lives(2,4), lives(3,1), lives(3,2), lives(3,3), lives(4,1), reached(1,3), reached(1,4), reached(2,4), reached(3,1), reached(3,2), reached(3,3), reached(4,1), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,4), lives(3,2), lives(3,3), reached(1,2), reached(1,3), reached(2,1), reached(2,4), reached(3,2), reached(3,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,1), lives(1,2), lives(2,1), lives(3,2), lives(3,3), lives(3,4), lives(4,4), reached(1,1), reached(1,2), reached(2,1), reached(3,2), reached(3,3), reached(3,4), reached(4,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(3,2), lives(3,3), lives(4,2), lives(4,3), reached(3,2), reached(3,3), reached(4,2), reached(4,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,1), lives(1,2), lives(2,1), lives(2,3), lives(3,2), lives(3,3), reached(1,1), reached(1,2), reached(2,1), reached(2,3), reached(3,2), reached(3,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,2), lives(2,1), lives(2,3), lives(3,2), lives(3,3), reached(1,2), reached(2,1), reached(2,3), reached(3,2), reached(3,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,2), lives(1,3), lives(2,2), lives(2,4), lives(3,3), lives(3,4), reached(1,2), reached(1,3), reached(2,2), reached(2,4), reached(3,3), reached(3,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,2), lives(1,3), lives(2,2), lives(2,4), lives(3,3), reached(1,2), reached(1,3), reached(2,2), reached(2,4), reached(3,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,3), lives(2,2), lives(2,4), lives(3,3), lives(3,4), reached(1,3), reached(2,2), reached(2,4), reached(3,3), reached(3,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,3), lives(2,2), lives(2,4), lives(3,3), reached(1,3), reached(2,2), reached(2,4), reached(3,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,3), lives(1,4), lives(2,2), lives(2,4), lives(3,3), reached(1,3), reached(1,4), reached(2,2), reached(2,4), reached(3,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(2,1), lives(2,2), lives(2,4), lives(3,1), lives(3,3), lives(3,4), reached(2,1), reached(2,2), reached(2,4), reached(3,1), reached(3,3), reached(3,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,2), lives(1,3), lives(2,2), lives(3,3), lives(4,2), lives(4,3), reached(1,2), reached(1,3), reached(2,2), reached(3,3), reached(4,2), reached(4,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,3), lives(2,2), lives(2,4), lives(3,1), lives(3,3), lives(4,2), reached(1,3), reached(2,2), reached(2,4), reached(3,1), reached(3,3), reached(4,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,3), lives(2,2), lives(2,4), lives(3,1), lives(3,3), lives(4,1), lives(4,2), reached(1,3), reached(2,2), reached(2,4), reached(3,1), reached(3,3), reached(4,1), reached(4,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,3), lives(1,4), lives(2,2), lives(2,4), lives(3,1), lives(3,3), lives(4,2), reached(1,3), reached(1,4), reached(2,2), reached(2,4), reached(3,1), reached(3,3), reached(4,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,3), lives(1,4), lives(2,2), lives(2,4), lives(3,1), lives(3,3), lives(4,1), lives(4,2), reached(1,3), reached(1,4), reached(2,2), reached(2,4), reached(3,1), reached(3,3), reached(4,1), reached(4,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(2,1), lives(2,2), lives(3,1), lives(3,3), lives(4,2), reached(2,1), reached(2,2), reached(3,1), reached(3,3), reached(4,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(2,1), lives(2,2), lives(3,1), lives(3,3), lives(4,2), lives(4,3), reached(2,1), reached(2,2), reached(3,1), reached(3,3), reached(4,2), reached(4,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(2,2), lives(3,1), lives(3,3), lives(4,2), reached(2,2), reached(3,1), reached(3,3), reached(4,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(2,2), lives(3,1), lives(3,3), lives(4,1), lives(4,2), reached(2,2), reached(3,1), reached(3,3), reached(4,1), reached(4,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(2,2), lives(3,1), lives(3,3), lives(4,2), lives(4,3), reached(2,2), reached(3,1), reached(3,3), reached(4,2), reached(4,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(2,2), lives(2,3), lives(3,1), lives(3,3), lives(4,2), reached(2,2), reached(2,3), reached(3,1), reached(3,3), reached(4,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(2,2), lives(2,3), lives(3,1), lives(3,3), lives(4,1), lives(4,2), reached(2,2), reached(2,3), reached(3,1), reached(3,3), reached(4,1), reached(4,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,3), lives(2,2), lives(2,4), lives(3,2), lives(3,3), reached(1,3), reached(2,2), reached(2,4), reached(3,2), reached(3,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,3), lives(1,4), lives(2,2), lives(2,4), lives(3,2), lives(3,3), reached(1,3), reached(1,4), reached(2,2), reached(2,4), reached(3,2), reached(3,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(2,2), lives(2,3), lives(3,2), lives(3,3), reached(2,2), reached(2,3), reached(3,2), reached(3,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,2), lives(1,3), lives(2,2), lives(2,3), reached(1,2), reached(1,3), reached(2,2), reached(2,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,1), lives(1,2), lives(2,2), lives(3,1), lives(4,1), lives(4,2), reached(1,1), reached(1,2), reached(2,2), reached(3,1), reached(4,1), reached(4,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,1), lives(1,2), lives(2,1), lives(2,2), reached(1,1), reached(1,2), reached(2,1), reached(2,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,1), lives(1,2), lives(2,2), lives(3,2), lives(3,4), lives(4,3), lives(4,4), reached(1,1), reached(1,2), reached(2,2), reached(3,2), reached(3,4), reached(4,3), reached(4,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,1), lives(2,1), lives(2,2), lives(2,3), lives(3,4), lives(4,3), lives(4,4), reached(1,1), reached(2,1), reached(2,2), reached(2,3), reached(3,4), reached(4,3), reached(4,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(2,2), lives(2,3), lives(3,1), lives(3,4), lives(4,2), lives(4,3), reached(2,2), reached(2,3), reached(3,1), reached(3,4), reached(4,2), reached(4,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,3), lives(2,2), lives(2,4), lives(3,1), lives(3,4), lives(4,2), lives(4,3), reached(1,3), reached(2,2), reached(2,4), reached(3,1), reached(3,4), reached(4,2), reached(4,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,4), lives(2,2), lives(2,3), lives(2,4), lives(3,1), lives(4,1), lives(4,2), reached(1,4), reached(2,2), reached(2,3), reached(2,4), reached(3,1), reached(4,1), reached(4,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,1), lives(1,3), lives(1,4), lives(2,1), lives(2,2), lives(2,4), reached(1,1), reached(1,3), reached(1,4), reached(2,1), reached(2,2), reached(2,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,3), lives(1,4), lives(2,2), lives(2,4), lives(3,2), lives(4,1), lives(4,2), reached(1,3), reached(1,4), reached(2,2), reached(2,4), reached(3,2), reached(4,1), reached(4,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(1,3), lives(2,2), lives(2,4), lives(3,2), lives(3,4), lives(4,3), reached(1,3), reached(2,2), reached(2,4), reached(3,2), reached(3,4), reached(4,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(2,1), lives(2,2), lives(3,1), lives(3,2), reached(2,1), reached(2,2), reached(3,1), reached(3,2), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(2,2), lives(2,3), lives(3,2), lives(3,4), lives(4,3), lives(4,4), reached(2,2), reached(2,3), reached(3,2), reached(3,4), reached(4,3), reached(4,4), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
{diff(-1,-1), diff(-1,0), diff(-1,1), diff(0,-1), diff(0,1), diff(1,-1), diff(1,0), diff(1,1), lives(2,2), lives(2,3), lives(3,2), lives(3,4), lives(4,3), reached(2,2), reached(2,3), reached(3,2), reached(3,4), reached(4,3), size(4), step(-1), step(1), value(0), value(1), value(2), value(3), value(4), value(5)}
"""
