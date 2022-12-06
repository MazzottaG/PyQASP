input = """
% instance

connected(x1y1,x2y1). connected(x2y1,x1y1). 
connected(x2y1,x3y1). connected(x3y1,x2y1).

connected(x2y2,x3y2). connected(x3y2,x2y2).

connected(x1y3,x2y3). connected(x2y3,x1y3). 
connected(x2y3,x3y3). connected(x3y3,x2y3).

connected(x2y1,x2y2). connected(x2y2,x2y1).
connected(x2y2,x2y3). connected(x2y3,x2y2).

connected(x3y1,x3y2). connected(x3y2,x3y1).
connected(x3y2,x3y3). connected(x3y3,x3y2).

at(x2y2).

visit(x1y1). visit(x2y1). visit(x3y1).
visit(x2y2). visit(x3y2).
visit(x1y3). visit(x2y3).

step(1). step(2). step(3).
step(4). step(5). step(6).
step(7). step(8).

% encoding

%
% initial status
%
visited(X) :- at(X).
atrobot(X,0) :- at(X).

% GENERATE  >>>>>
1 <= { move( Curpos,Nextpos,T ) : connected( Curpos,Nextpos ) , Curpos != Nextpos } <= 1 :- step(T).
% <<<<<  GENERATE
% 

% 
% 
% EFFECTS APPLY  >>>>>

% move/3, effects
atrobot( Nextpos,T ) :- move( Curpos,Nextpos,T ).
del( atrobot( Curpos ),T ) :- move( Curpos,Nextpos,T ).
visited( Nextpos ) :- move( Curpos,Nextpos,T ).
% <<<<<  EFFECTS APPLY
% 

% 
% 
% INERTIA  >>>>>
atrobot( X,T ) :- step(T), atrobot( X,T-1 ), not del( atrobot( X ) ,T  ).
%
% GB: it is not needed to let 'visited' subject to timestamping 
%
% <<<<<  INERTIA
% 

% 
% 
% PRECONDITIONS HOLD  >>>>>

% move/3, preconditions
 :- move( Curpos,Nextpos,T ), not atrobot(Curpos, T-1), step(T).
% <<<<<  PRECONDITIONS HOLD
% 

goalreached :- N = #count{ X : visited(X) , visit(X) }, N = #count{ X : visit(X) }.
:- not goalreached.


"""
output = """
{at(x2y2), atrobot(x1y1,4), atrobot(x1y3,8), atrobot(x2y1,3), atrobot(x2y1,5), atrobot(x2y2,0), atrobot(x2y2,6), atrobot(x2y3,7), atrobot(x3y1,2), atrobot(x3y2,1), connected(x1y1,x2y1), connected(x1y3,x2y3), connected(x2y1,x1y1), connected(x2y1,x2y2), connected(x2y1,x3y1), connected(x2y2,x2y1), connected(x2y2,x2y3), connected(x2y2,x3y2), connected(x2y3,x1y3), connected(x2y3,x2y2), connected(x2y3,x3y3), connected(x3y1,x2y1), connected(x3y1,x3y2), connected(x3y2,x2y2), connected(x3y2,x3y1), connected(x3y2,x3y3), connected(x3y3,x2y3), connected(x3y3,x3y2), del(atrobot(x1y1),5), del(atrobot(x2y1),4), del(atrobot(x2y1),6), del(atrobot(x2y2),1), del(atrobot(x2y2),7), del(atrobot(x2y3),8), del(atrobot(x3y1),3), del(atrobot(x3y2),2), goalreached, move(x1y1,x2y1,5), move(x2y1,x1y1,4), move(x2y1,x2y2,6), move(x2y2,x2y3,7), move(x2y2,x3y2,1), move(x2y3,x1y3,8), move(x3y1,x2y1,3), move(x3y2,x3y1,2), step(1), step(2), step(3), step(4), step(5), step(6), step(7), step(8), visit(x1y1), visit(x1y3), visit(x2y1), visit(x2y2), visit(x2y3), visit(x3y1), visit(x3y2), visited(x1y1), visited(x1y3), visited(x2y1), visited(x2y2), visited(x2y3), visited(x3y1), visited(x3y2)}
{at(x2y2), atrobot(x1y1,2), atrobot(x1y3,8), atrobot(x2y1,1), atrobot(x2y1,3), atrobot(x2y2,0), atrobot(x2y2,6), atrobot(x2y3,7), atrobot(x3y1,4), atrobot(x3y2,5), connected(x1y1,x2y1), connected(x1y3,x2y3), connected(x2y1,x1y1), connected(x2y1,x2y2), connected(x2y1,x3y1), connected(x2y2,x2y1), connected(x2y2,x2y3), connected(x2y2,x3y2), connected(x2y3,x1y3), connected(x2y3,x2y2), connected(x2y3,x3y3), connected(x3y1,x2y1), connected(x3y1,x3y2), connected(x3y2,x2y2), connected(x3y2,x3y1), connected(x3y2,x3y3), connected(x3y3,x2y3), connected(x3y3,x3y2), del(atrobot(x1y1),3), del(atrobot(x2y1),2), del(atrobot(x2y1),4), del(atrobot(x2y2),1), del(atrobot(x2y2),7), del(atrobot(x2y3),8), del(atrobot(x3y1),5), del(atrobot(x3y2),6), goalreached, move(x1y1,x2y1,3), move(x2y1,x1y1,2), move(x2y1,x3y1,4), move(x2y2,x2y1,1), move(x2y2,x2y3,7), move(x2y3,x1y3,8), move(x3y1,x3y2,5), move(x3y2,x2y2,6), step(1), step(2), step(3), step(4), step(5), step(6), step(7), step(8), visit(x1y1), visit(x1y3), visit(x2y1), visit(x2y2), visit(x2y3), visit(x3y1), visit(x3y2), visited(x1y1), visited(x1y3), visited(x2y1), visited(x2y2), visited(x2y3), visited(x3y1), visited(x3y2)}
{at(x2y2), atrobot(x1y1,2), atrobot(x1y3,8), atrobot(x2y1,1), atrobot(x2y1,3), atrobot(x2y2,0), atrobot(x2y3,7), atrobot(x3y1,4), atrobot(x3y2,5), atrobot(x3y3,6), connected(x1y1,x2y1), connected(x1y3,x2y3), connected(x2y1,x1y1), connected(x2y1,x2y2), connected(x2y1,x3y1), connected(x2y2,x2y1), connected(x2y2,x2y3), connected(x2y2,x3y2), connected(x2y3,x1y3), connected(x2y3,x2y2), connected(x2y3,x3y3), connected(x3y1,x2y1), connected(x3y1,x3y2), connected(x3y2,x2y2), connected(x3y2,x3y1), connected(x3y2,x3y3), connected(x3y3,x2y3), connected(x3y3,x3y2), del(atrobot(x1y1),3), del(atrobot(x2y1),2), del(atrobot(x2y1),4), del(atrobot(x2y2),1), del(atrobot(x2y3),8), del(atrobot(x3y1),5), del(atrobot(x3y2),6), del(atrobot(x3y3),7), goalreached, move(x1y1,x2y1,3), move(x2y1,x1y1,2), move(x2y1,x3y1,4), move(x2y2,x2y1,1), move(x2y3,x1y3,8), move(x3y1,x3y2,5), move(x3y2,x3y3,6), move(x3y3,x2y3,7), step(1), step(2), step(3), step(4), step(5), step(6), step(7), step(8), visit(x1y1), visit(x1y3), visit(x2y1), visit(x2y2), visit(x2y3), visit(x3y1), visit(x3y2), visited(x1y1), visited(x1y3), visited(x2y1), visited(x2y2), visited(x2y3), visited(x3y1), visited(x3y2), visited(x3y3)}
{at(x2y2), atrobot(x1y1,8), atrobot(x1y3,2), atrobot(x2y1,7), atrobot(x2y2,0), atrobot(x2y3,1), atrobot(x2y3,3), atrobot(x3y1,6), atrobot(x3y2,5), atrobot(x3y3,4), connected(x1y1,x2y1), connected(x1y3,x2y3), connected(x2y1,x1y1), connected(x2y1,x2y2), connected(x2y1,x3y1), connected(x2y2,x2y1), connected(x2y2,x2y3), connected(x2y2,x3y2), connected(x2y3,x1y3), connected(x2y3,x2y2), connected(x2y3,x3y3), connected(x3y1,x2y1), connected(x3y1,x3y2), connected(x3y2,x2y2), connected(x3y2,x3y1), connected(x3y2,x3y3), connected(x3y3,x2y3), connected(x3y3,x3y2), del(atrobot(x1y3),3), del(atrobot(x2y1),8), del(atrobot(x2y2),1), del(atrobot(x2y3),2), del(atrobot(x2y3),4), del(atrobot(x3y1),7), del(atrobot(x3y2),6), del(atrobot(x3y3),5), goalreached, move(x1y3,x2y3,3), move(x2y1,x1y1,8), move(x2y2,x2y3,1), move(x2y3,x1y3,2), move(x2y3,x3y3,4), move(x3y1,x2y1,7), move(x3y2,x3y1,6), move(x3y3,x3y2,5), step(1), step(2), step(3), step(4), step(5), step(6), step(7), step(8), visit(x1y1), visit(x1y3), visit(x2y1), visit(x2y2), visit(x2y3), visit(x3y1), visit(x3y2), visited(x1y1), visited(x1y3), visited(x2y1), visited(x2y2), visited(x2y3), visited(x3y1), visited(x3y2), visited(x3y3)}
{at(x2y2), atrobot(x1y1,8), atrobot(x1y3,2), atrobot(x2y1,7), atrobot(x2y2,0), atrobot(x2y2,4), atrobot(x2y3,1), atrobot(x2y3,3), atrobot(x3y1,6), atrobot(x3y2,5), connected(x1y1,x2y1), connected(x1y3,x2y3), connected(x2y1,x1y1), connected(x2y1,x2y2), connected(x2y1,x3y1), connected(x2y2,x2y1), connected(x2y2,x2y3), connected(x2y2,x3y2), connected(x2y3,x1y3), connected(x2y3,x2y2), connected(x2y3,x3y3), connected(x3y1,x2y1), connected(x3y1,x3y2), connected(x3y2,x2y2), connected(x3y2,x3y1), connected(x3y2,x3y3), connected(x3y3,x2y3), connected(x3y3,x3y2), del(atrobot(x1y3),3), del(atrobot(x2y1),8), del(atrobot(x2y2),1), del(atrobot(x2y2),5), del(atrobot(x2y3),2), del(atrobot(x2y3),4), del(atrobot(x3y1),7), del(atrobot(x3y2),6), goalreached, move(x1y3,x2y3,3), move(x2y1,x1y1,8), move(x2y2,x2y3,1), move(x2y2,x3y2,5), move(x2y3,x1y3,2), move(x2y3,x2y2,4), move(x3y1,x2y1,7), move(x3y2,x3y1,6), step(1), step(2), step(3), step(4), step(5), step(6), step(7), step(8), visit(x1y1), visit(x1y3), visit(x2y1), visit(x2y2), visit(x2y3), visit(x3y1), visit(x3y2), visited(x1y1), visited(x1y3), visited(x2y1), visited(x2y2), visited(x2y3), visited(x3y1), visited(x3y2)}
"""
