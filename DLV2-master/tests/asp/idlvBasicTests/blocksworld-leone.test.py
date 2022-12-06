input = """
% invocation:
% dlv <name of this file> <problem instance> -silent -N=k -pfilter=move 
%
% where 
% <problem instance> is the file which contains the specification of the 
%                    planning problem (i.e. initial state and goal state)
% -silent            suppresses various version informations
% -N=k               k specifies the number of timeslots
% -pfilter=move      displays only atoms with predicate "move" in models 

% This will compute models which correspond to valid plans within k 
% timeslots. Only the "move" actions are displayed in the output.

% If -n=1 is added to the commandline, stop the computation after the 
% first model is found.

% blocks world domain

% block(B)  refers to a block. This relation should be defined in a 
%           seperate file in order to be able to specify problem 
%           instances of varying size.

% init(B,L) defines the initial state (block B is on location L) and
%           should also be defined in the separate file.

% a location may be any block or the table

% on(B,L,T) holds iff block B is on location L during timeslot T

% move(B,L,T) holds iff block B is moved to location L 'at the end' of 
%                       timeslot T (taking effect in the next timeslot)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% define all timeslots by specifying -N=k during the invocation



% #maxint is a special constant and refers to k, if -N=k is specified 
% on the commandline.



% no action (move) may take place at the end of the last timeslot

actiontime(T) :- time(T), not lasttime(T).

% Define the table.

table(table).

% Since the number of blocks varies from problem to problem, the blocks 
% should be defined in a separate file.
% This allows us to use the same program for different problem instances.

location(B) :- block(B).
location(T) :- table(T).

% set the initial conditions

on(B,L,0) :- init(B,L).

% describe the effect of moving a block
% #succ(X,Y) is a builtin and holds if X is the successor of Y, and
% (if -N=k is specified) 0 <= X <= k and 0 <= Y <= k hold.

on(B,L,T1) :- block(B), location(L), move(B,L,T).

% a block can be moved only when it's clear and the goal location is clear

:- move(B,L,T),on(B1,B,T).
:- block(B1),move(B,B1,T),on(B2,B1,T).

% a block cannot be on two different locations

:- on(B,L,T), on(B,L1,T), L != L1.

% a block is never on top of itself

:- block(B), time(T), on(B,B,T).

% do not allow concurrent moves

:- move(B,L,T), move(B1,L1,T), B != B1.
:- move(B,L,T), move(B1,L1,T), L != L1.

% inertia

on(B,L,T1) :- on(B,L,T), not moved(B,T).

moved(B,T) :- move(B,_,T).

% initial values and actions are exogeneous

on(B,L,0) | -on(B,L,0) :- block(B), location(L).

% at some time (when an action may take place) a block may or may not 
% be moved (but never onto itself).

move(B,L,T) | -move(B,L,T) :- block(B), location(L), actiontime(T), B != L.

"""
output = """
"""
