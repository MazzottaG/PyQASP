input = """
% instance

steps(3). time(0). time(1). time(2). time(3). disk(1). disk(2). disk(3). disk(4). disk(5). disk(6). disk(7). disk(8). disk(9). on0(5,1). on0(6,5). on0(7,6). on0(8,7). on0(9,8). ongoal(5,1). ongoal(6,5). ongoal(7,6). ongoal(8,2). ongoal(9,8).

% encoding

% The meaning of the time predicate is self-evident. As for the disk
% predicate, there are k disks 1,2,...,k. Disks 1, 2, 3, 4 denote pegs. 
% Disks 5, ... are movable. The larger the number of the disk, 
% the smaller it is.
%
% The program uses additional predicates:
% on(T,N,M), which is true iff at time T, disk M is on disk N
% move(t,N), which is true iff at time T, it is disk N that will be
% moved
% where(T,N), which is true iff at time T, the disk to be moved is moved
% on top of the disk N.
% goal, which is true iff the goal state is reached at time t
% steps(T), which is the number of time steps T, required to reach the goal (provided part of Input data)

% Read in data 
 	on(0,N1,N) :- on0(N,N1).
    onG(K,N1,N) :- ongoal(N,N1), steps(K).
		   
% Specify valid arrangements of disks
 	% Basic condition. Smaller disks are on larger ones
 	:- time(T), on(T,N1,N), N1>=N.
 	
% Specify a valid move (only for T<t)
 	% pick a disk to move
    move(T,N) | noMove(T,N) :- disk(N), time(T), steps(K), T<K.
    :- move(T,N1), move(T,N2), N1 != N2.
    :- time(T), steps(K), T<K, not diskMoved(T).
    diskMoved(T) :- move(T,Fv1).

 	% pick a disk onto which to move
    where(T,N) | noWhere(T,N) :- disk(N), time(T), steps(K), T<K.
    :- where(T,N1), where(T,N2), N1 != N2.
    :- time(T), steps(K), T<K, not diskWhere(T).
    diskWhere(T) :- where(T,Fv1).

 	% pegs cannot be moved
 	:- move(T,N), N<5.

 	% only top disk can be moved
 	:- on(T,N,N1), move(T,N).

 	% a disk can be placed on top only.
 	:- on(T,N,N1), where(T,N).

 	% no disk is moved in two consecutive moves
	%@rule_partial_order(@before={TM1=T-1},@after={move(TM1,N)}).
 	:- move(T,N), move(TM1,N), TM1=T-1.

% Specify effects of a move
on(TP1,N1,N) :- move(T,N), where(T,N1), TP1=T+1.

on(TP1,N,N1) :- time(T), steps(K), T<K,
                on(T,N,N1), not move(T,N1), TP1=T+1.

% Goal description
%@rule_ordering(@value=0).
	 :- not on(K,N,N1), onG(K,N,N1), steps(K).
	 :- on(K,N,N1), not onG(K,N,N1),steps(K).

% Solution
	 put(T,M,N) :- move(T,N), where(T,M), steps(K), T<K.

"""
output = """
{disk(1), disk(2), disk(3), disk(4), disk(5), disk(6), disk(7), disk(8), disk(9), diskMoved(0), diskMoved(1), diskMoved(2), diskWhere(0), diskWhere(1), diskWhere(2), move(0,9), move(1,8), move(2,9), noMove(0,1), noMove(0,2), noMove(0,3), noMove(0,4), noMove(0,5), noMove(0,6), noMove(0,7), noMove(0,8), noMove(1,1), noMove(1,2), noMove(1,3), noMove(1,4), noMove(1,5), noMove(1,6), noMove(1,7), noMove(1,9), noMove(2,1), noMove(2,2), noMove(2,3), noMove(2,4), noMove(2,5), noMove(2,6), noMove(2,7), noMove(2,8), noWhere(0,1), noWhere(0,2), noWhere(0,3), noWhere(0,5), noWhere(0,6), noWhere(0,7), noWhere(0,8), noWhere(0,9), noWhere(1,1), noWhere(1,3), noWhere(1,4), noWhere(1,5), noWhere(1,6), noWhere(1,7), noWhere(1,8), noWhere(1,9), noWhere(2,1), noWhere(2,2), noWhere(2,3), noWhere(2,4), noWhere(2,5), noWhere(2,6), noWhere(2,7), noWhere(2,9), on(0,1,5), on(0,5,6), on(0,6,7), on(0,7,8), on(0,8,9), on(1,1,5), on(1,4,9), on(1,5,6), on(1,6,7), on(1,7,8), on(2,1,5), on(2,2,8), on(2,4,9), on(2,5,6), on(2,6,7), on(3,1,5), on(3,2,8), on(3,5,6), on(3,6,7), on(3,8,9), on0(5,1), on0(6,5), on0(7,6), on0(8,7), on0(9,8), onG(3,1,5), onG(3,2,8), onG(3,5,6), onG(3,6,7), onG(3,8,9), ongoal(5,1), ongoal(6,5), ongoal(7,6), ongoal(8,2), ongoal(9,8), put(0,4,9), put(1,2,8), put(2,8,9), steps(3), time(0), time(1), time(2), time(3), where(0,4), where(1,2), where(2,8)}
{disk(1), disk(2), disk(3), disk(4), disk(5), disk(6), disk(7), disk(8), disk(9), diskMoved(0), diskMoved(1), diskMoved(2), diskWhere(0), diskWhere(1), diskWhere(2), move(0,9), move(1,8), move(2,9), noMove(0,1), noMove(0,2), noMove(0,3), noMove(0,4), noMove(0,5), noMove(0,6), noMove(0,7), noMove(0,8), noMove(1,1), noMove(1,2), noMove(1,3), noMove(1,4), noMove(1,5), noMove(1,6), noMove(1,7), noMove(1,9), noMove(2,1), noMove(2,2), noMove(2,3), noMove(2,4), noMove(2,5), noMove(2,6), noMove(2,7), noMove(2,8), noWhere(0,1), noWhere(0,2), noWhere(0,4), noWhere(0,5), noWhere(0,6), noWhere(0,7), noWhere(0,8), noWhere(0,9), noWhere(1,1), noWhere(1,3), noWhere(1,4), noWhere(1,5), noWhere(1,6), noWhere(1,7), noWhere(1,8), noWhere(1,9), noWhere(2,1), noWhere(2,2), noWhere(2,3), noWhere(2,4), noWhere(2,5), noWhere(2,6), noWhere(2,7), noWhere(2,9), on(0,1,5), on(0,5,6), on(0,6,7), on(0,7,8), on(0,8,9), on(1,1,5), on(1,3,9), on(1,5,6), on(1,6,7), on(1,7,8), on(2,1,5), on(2,2,8), on(2,3,9), on(2,5,6), on(2,6,7), on(3,1,5), on(3,2,8), on(3,5,6), on(3,6,7), on(3,8,9), on0(5,1), on0(6,5), on0(7,6), on0(8,7), on0(9,8), onG(3,1,5), onG(3,2,8), onG(3,5,6), onG(3,6,7), onG(3,8,9), ongoal(5,1), ongoal(6,5), ongoal(7,6), ongoal(8,2), ongoal(9,8), put(0,3,9), put(1,2,8), put(2,8,9), steps(3), time(0), time(1), time(2), time(3), where(0,3), where(1,2), where(2,8)}
"""
