input = """
% instance

player(player_01).
stone(stone_01).
isgoal(pos_4_2).
isnongoal(pos_1_1).
isnongoal(pos_1_2).
isnongoal(pos_1_3).
isnongoal(pos_2_1).
isnongoal(pos_2_2).
isnongoal(pos_2_3).
isnongoal(pos_3_1).
isnongoal(pos_3_2).
isnongoal(pos_3_3).
isnongoal(pos_4_1).
isnongoal(pos_4_3).
isnongoal(pos_5_1).
isnongoal(pos_5_2).
isnongoal(pos_5_3).
movedir(pos_2_2,pos_3_2,dir_right).
movedir(pos_3_2,pos_2_2,dir_left).
movedir(pos_3_2,pos_4_2,dir_right).
movedir(pos_4_2,pos_3_2,dir_left).
at(player_01,pos_2_2).
at(stone_01,pos_3_2).
clear(pos_4_2).
goal(stone_01).
step(1).
step(2).
step(3).

% encoding

%
% Sokoban domain IPC 2008
%
% Adaptment from IPC 2008 domain description by GB Ianni, using the PDDL2ASP PLASP converter
% http://www.cs.uni-potsdam.de/wv/pdfformat/gekaknsc11a.pdf 
%
% 


% GENERATE  >>>>>
1 <= { pushtonongoal( P,S,Ppos,From,To,Dir,T ) : 
	movedir( Ppos,From,Dir ) ,
	movedir( From,To,Dir ) , 
	isnongoal( To ) , 
	player( P ) , 
	stone( S ) , Ppos != To , Ppos != From , From != To; 
    move( P,From,To,Dir,T ) : 
	movedir( From,To,Dir ) , 
	player( P ) , From != To;
    pushtogoal( P,S,Ppos,From,To,Dir,T ) : 
	movedir( Ppos,From,Dir ) , 
	movedir( From,To,Dir ) , 
	isgoal( To ) , player( P ) , stone( S ) , Ppos != To , Ppos != From , From != To;
    noop(T) } <= 1 :- step(T).

% <<<<<  GENERATE
% 

% 
%
% Initial state
at(P,To,0) :- at(P,To).
clear(P,0) :- clear(P).
atgoal(S,0) :- isgoal(L), stone(S), at(S,L).
 
% EFFECTS APPLY  >>>>>

% push-to-nongoal/7, effects
del( at( P,Ppos ),Ti ) :- pushtonongoal( P,S,Ppos,From,To,Dir,Ti ), 
                          movedir( Ppos,From,Dir ), movedir( From,To,Dir ), isnongoal( To ), player( P ), stone( S ), Ppos != To, Ppos != From, From != To.
del( at( S,From ),Ti ) :- pushtonongoal( P,S,Ppos,From,To,Dir,Ti ), movedir( Ppos,From,Dir ), movedir( From,To,Dir ), isnongoal( To ), player( P ), stone( S ), Ppos != To, Ppos != From, From != To.
del( clear( To ),Ti ) :- pushtonongoal( P,S,Ppos,From,To,Dir,Ti ), movedir( Ppos,From,Dir ), movedir( From,To,Dir ), isnongoal( To ), player( P ), stone( S ), Ppos != To, Ppos != From, From != To.
at( P,From,Ti ) :- pushtonongoal( P,S,Ppos,From,To,Dir,Ti ), movedir( Ppos,From,Dir ), movedir( From,To,Dir ), isnongoal( To ), player( P ), stone( S ), Ppos != To, Ppos != From, From != To.
at( S,To,Ti ) :- pushtonongoal( P,S,Ppos,From,To,Dir,Ti ), movedir( Ppos,From,Dir ), movedir( From,To,Dir ), isnongoal( To ), player( P ), stone( S ), Ppos != To, Ppos != From, From != To.
clear( Ppos,Ti ) :- pushtonongoal( P,S,Ppos,From,To,Dir,Ti ), movedir( Ppos,From,Dir ), movedir( From,To,Dir ), isnongoal( To ), player( P ), stone( S ), Ppos != To, Ppos != From, From != To.
del( atgoal( S ),Ti ) :- pushtonongoal( P,S,Ppos,From,To,Dir,Ti ), movedir( Ppos,From,Dir ), movedir( From,To,Dir ), isnongoal( To ), player( P ), stone( S ), Ppos != To, Ppos != From, From != To.

% move/5, effects
del( at( P,From ),Ti ) :- move( P,From,To,Dir,Ti ), movedir( From,To,Dir ), player( P ), From != To.
del( clear( To ),Ti ) :- move( P,From,To,Dir,Ti ), movedir( From,To,Dir ), player( P ), From != To.
at( P,To,Ti ) :- move( P,From,To,Dir,Ti ), movedir( From,To,Dir ), player( P ), From != To.
clear( From,Ti ) :- move( P,From,To,Dir,Ti ), movedir( From,To,Dir ), player( P ), From != To.

% push-to-goal/7, effects
del( at( P,Ppos ),Ti ) :- pushtogoal( P,S,Ppos,From,To,Dir,Ti ), 
                          movedir( Ppos,From,Dir ), movedir( From,To,Dir ), isgoal( To ), player( P ), stone( S ), Ppos != To, Ppos != From, From != To.
del( at( S,From ),Ti ) :- pushtogoal( P,S,Ppos,From,To,Dir,Ti ), 
                          movedir( Ppos,From,Dir ), movedir( From,To,Dir ), isgoal( To ), player( P ), stone( S ), Ppos != To, Ppos != From, From != To.
del( clear( To ),Ti ) :- pushtogoal( P,S,Ppos,From,To,Dir,Ti ), 
                         movedir( Ppos,From,Dir ), movedir( From,To,Dir ), isgoal( To ), player( P ), stone( S ), Ppos != To, Ppos != From, From != To.
at( P,From,Ti ) :- pushtogoal( P,S,Ppos,From,To,Dir,Ti ), 
                   movedir( Ppos,From,Dir ), movedir( From,To,Dir ), isgoal( To ), player( P ), stone( S ), Ppos != To, Ppos != From, From != To.
at( S,To,Ti ) :- pushtogoal( P,S,Ppos,From,To,Dir,Ti ), 
                 movedir( Ppos,From,Dir ), movedir( From,To,Dir ), isgoal( To ), player( P ), stone( S ), Ppos != To, Ppos != From, From != To.
clear( Ppos,Ti ) :- pushtogoal( P,S,Ppos,From,To,Dir,Ti ), 
                    movedir( Ppos,From,Dir ), movedir( From,To,Dir ), isgoal( To ), player( P ), stone( S ), Ppos != To, Ppos != From, From != To.
atgoal( S,Ti ) :- pushtogoal( P,S,Ppos,From,To,Dir,Ti ), 
                  stone( S ), movedir( Ppos,From,Dir ), movedir( From,To,Dir ), isgoal( To ), player( P ), stone( S ), Ppos != To, Ppos != From, From != To.
% <<<<<  EFFECTS APPLY
% 

% 
% 
% INERTIA  >>>>>
clear( L,Ti ) :- clear( L,Ti-1 ), not del( clear( L ),Ti  ), step(Ti).
atgoal( S,Ti ) :- atgoal( S,Ti-1 ), not del( atgoal( S ),Ti ), stone( S ), step(Ti).
at( T,L,Ti ) :- at( T,L,Ti-1 ), not del( at( T,L ) ,Ti  ), step(Ti).
% <<<<<  INERTIA
% 

% 
% 
% PRECONDITIONS HOLD  >>>>>

% push-to-nongoal/6, preconditions
 :- pushtonongoal( P,S,Ppos,From,To,Dir,Ti ), not preconditions_png( P,S,Ppos,From,To,Dir,Ti ), movedir( Ppos,From,Dir ), movedir( From,To,Dir ), isnongoal( To ), player( P ), stone( S ), Ppos != To, Ppos != From, From != To.
preconditions_png( P,S,Ppos,From,To,Dir,Ti ) :- at( P,Ppos,Ti-1 ), at( S,From,Ti-1 ), clear( To,Ti-1 ), movedir( Ppos,From,Dir ), movedir( From,To,Dir ), isnongoal( To ), movedir( Ppos,From,Dir ), movedir( From,To,Dir ), isnongoal( To ), player( P ), stone( S ), Ppos != To, Ppos != From, From != To, step(Ti).

% move/4, preconditions
 :- move( P,From,To,Dir,Ti ), not preconditions_m( P,From,To,Dir,Ti ), movedir( From,To,Dir ), player( P ), From != To.
preconditions_m( P,From,To,Dir,Ti ) :- at( P,From,Ti-1 ), clear( To,Ti-1 ), movedir( From,To,Dir ), movedir( From,To,Dir ), player( P ), From != To, step(Ti).

% push-to-goal/6, preconditions
 :- pushtogoal( P,S,Ppos,From,To,Dir,Ti ), not preconditions_pg( P,S,Ppos,From,To,Dir,Ti ), movedir( Ppos,From,Dir ), movedir( From,To,Dir ), isgoal( To ), player( P ), stone( S ), Ppos != To, Ppos != From, From != To, step(Ti).
preconditions_pg( P,S,Ppos,From,To,Dir,Ti ) :- at( P,Ppos,Ti-1 ), at( S,From,Ti-1 ), clear( To,Ti-1 ), movedir( Ppos,From,Dir ), movedir( From,To,Dir ), isgoal( To ), movedir( Ppos,From,Dir ), movedir( From,To,Dir ), isgoal( To ), player( P ), stone( S ), Ppos != To, Ppos != From, From != To, step(Ti).

% <<<<<  PRECONDITIONS HOLD
% 
%
% Goal Reached check 
%
goalreached :- step(T), N = #count{ X,T : atgoal(X,T) , goal(X) }, N = #count{ X : goal(X) }.
:- not goalreached.

% Gringo directives to show / hide particular literals
%#hide.
%#show pushtonongoal/7.
%#show move/5.
%#show pushtogoal/7.

"""
output = """
{at(player_01,pos_2_2), at(player_01,pos_2_2,0), at(player_01,pos_2_2,1), at(player_01,pos_2_2,2), at(player_01,pos_3_2,3), at(stone_01,pos_3_2), at(stone_01,pos_3_2,0), at(stone_01,pos_3_2,1), at(stone_01,pos_3_2,2), at(stone_01,pos_4_2,3), atgoal(stone_01,3), clear(pos_2_2,3), clear(pos_4_2), clear(pos_4_2,0), clear(pos_4_2,1), clear(pos_4_2,2), del(at(player_01,pos_2_2),3), del(at(stone_01,pos_3_2),3), del(clear(pos_4_2),3), goal(stone_01), goalreached, isgoal(pos_4_2), isnongoal(pos_1_1), isnongoal(pos_1_2), isnongoal(pos_1_3), isnongoal(pos_2_1), isnongoal(pos_2_2), isnongoal(pos_2_3), isnongoal(pos_3_1), isnongoal(pos_3_2), isnongoal(pos_3_3), isnongoal(pos_4_1), isnongoal(pos_4_3), isnongoal(pos_5_1), isnongoal(pos_5_2), isnongoal(pos_5_3), movedir(pos_2_2,pos_3_2,dir_right), movedir(pos_3_2,pos_2_2,dir_left), movedir(pos_3_2,pos_4_2,dir_right), movedir(pos_4_2,pos_3_2,dir_left), noop(1), noop(2), player(player_01), preconditions_pg(player_01,stone_01,pos_2_2,pos_3_2,pos_4_2,dir_right,1), preconditions_pg(player_01,stone_01,pos_2_2,pos_3_2,pos_4_2,dir_right,2), preconditions_pg(player_01,stone_01,pos_2_2,pos_3_2,pos_4_2,dir_right,3), pushtogoal(player_01,stone_01,pos_2_2,pos_3_2,pos_4_2,dir_right,3), step(1), step(2), step(3), stone(stone_01)}
{at(player_01,pos_2_2), at(player_01,pos_2_2,0), at(player_01,pos_2_2,1), at(player_01,pos_3_2,2), at(player_01,pos_3_2,3), at(stone_01,pos_3_2), at(stone_01,pos_3_2,0), at(stone_01,pos_3_2,1), at(stone_01,pos_4_2,2), at(stone_01,pos_4_2,3), atgoal(stone_01,2), atgoal(stone_01,3), clear(pos_2_2,2), clear(pos_2_2,3), clear(pos_4_2), clear(pos_4_2,0), clear(pos_4_2,1), del(at(player_01,pos_2_2),2), del(at(stone_01,pos_3_2),2), del(clear(pos_4_2),2), goal(stone_01), goalreached, isgoal(pos_4_2), isnongoal(pos_1_1), isnongoal(pos_1_2), isnongoal(pos_1_3), isnongoal(pos_2_1), isnongoal(pos_2_2), isnongoal(pos_2_3), isnongoal(pos_3_1), isnongoal(pos_3_2), isnongoal(pos_3_3), isnongoal(pos_4_1), isnongoal(pos_4_3), isnongoal(pos_5_1), isnongoal(pos_5_2), isnongoal(pos_5_3), movedir(pos_2_2,pos_3_2,dir_right), movedir(pos_3_2,pos_2_2,dir_left), movedir(pos_3_2,pos_4_2,dir_right), movedir(pos_4_2,pos_3_2,dir_left), noop(1), noop(3), player(player_01), preconditions_m(player_01,pos_3_2,pos_2_2,dir_left,3), preconditions_pg(player_01,stone_01,pos_2_2,pos_3_2,pos_4_2,dir_right,1), preconditions_pg(player_01,stone_01,pos_2_2,pos_3_2,pos_4_2,dir_right,2), pushtogoal(player_01,stone_01,pos_2_2,pos_3_2,pos_4_2,dir_right,2), step(1), step(2), step(3), stone(stone_01)}
{at(player_01,pos_2_2), at(player_01,pos_2_2,0), at(player_01,pos_2_2,1), at(player_01,pos_2_2,3), at(player_01,pos_3_2,2), at(stone_01,pos_3_2), at(stone_01,pos_3_2,0), at(stone_01,pos_3_2,1), at(stone_01,pos_4_2,2), at(stone_01,pos_4_2,3), atgoal(stone_01,2), atgoal(stone_01,3), clear(pos_2_2,2), clear(pos_3_2,3), clear(pos_4_2), clear(pos_4_2,0), clear(pos_4_2,1), del(at(player_01,pos_2_2),2), del(at(player_01,pos_3_2),3), del(at(stone_01,pos_3_2),2), del(clear(pos_2_2),3), del(clear(pos_4_2),2), goal(stone_01), goalreached, isgoal(pos_4_2), isnongoal(pos_1_1), isnongoal(pos_1_2), isnongoal(pos_1_3), isnongoal(pos_2_1), isnongoal(pos_2_2), isnongoal(pos_2_3), isnongoal(pos_3_1), isnongoal(pos_3_2), isnongoal(pos_3_3), isnongoal(pos_4_1), isnongoal(pos_4_3), isnongoal(pos_5_1), isnongoal(pos_5_2), isnongoal(pos_5_3), move(player_01,pos_3_2,pos_2_2,dir_left,3), movedir(pos_2_2,pos_3_2,dir_right), movedir(pos_3_2,pos_2_2,dir_left), movedir(pos_3_2,pos_4_2,dir_right), movedir(pos_4_2,pos_3_2,dir_left), noop(1), player(player_01), preconditions_m(player_01,pos_3_2,pos_2_2,dir_left,3), preconditions_pg(player_01,stone_01,pos_2_2,pos_3_2,pos_4_2,dir_right,1), preconditions_pg(player_01,stone_01,pos_2_2,pos_3_2,pos_4_2,dir_right,2), pushtogoal(player_01,stone_01,pos_2_2,pos_3_2,pos_4_2,dir_right,2), step(1), step(2), step(3), stone(stone_01)}
{at(player_01,pos_2_2), at(player_01,pos_2_2,0), at(player_01,pos_3_2,1), at(player_01,pos_3_2,2), at(player_01,pos_3_2,3), at(stone_01,pos_3_2), at(stone_01,pos_3_2,0), at(stone_01,pos_4_2,1), at(stone_01,pos_4_2,2), at(stone_01,pos_4_2,3), atgoal(stone_01,1), atgoal(stone_01,2), atgoal(stone_01,3), clear(pos_2_2,1), clear(pos_2_2,2), clear(pos_2_2,3), clear(pos_4_2), clear(pos_4_2,0), del(at(player_01,pos_2_2),1), del(at(stone_01,pos_3_2),1), del(clear(pos_4_2),1), goal(stone_01), goalreached, isgoal(pos_4_2), isnongoal(pos_1_1), isnongoal(pos_1_2), isnongoal(pos_1_3), isnongoal(pos_2_1), isnongoal(pos_2_2), isnongoal(pos_2_3), isnongoal(pos_3_1), isnongoal(pos_3_2), isnongoal(pos_3_3), isnongoal(pos_4_1), isnongoal(pos_4_3), isnongoal(pos_5_1), isnongoal(pos_5_2), isnongoal(pos_5_3), movedir(pos_2_2,pos_3_2,dir_right), movedir(pos_3_2,pos_2_2,dir_left), movedir(pos_3_2,pos_4_2,dir_right), movedir(pos_4_2,pos_3_2,dir_left), noop(2), noop(3), player(player_01), preconditions_m(player_01,pos_3_2,pos_2_2,dir_left,2), preconditions_m(player_01,pos_3_2,pos_2_2,dir_left,3), preconditions_pg(player_01,stone_01,pos_2_2,pos_3_2,pos_4_2,dir_right,1), pushtogoal(player_01,stone_01,pos_2_2,pos_3_2,pos_4_2,dir_right,1), step(1), step(2), step(3), stone(stone_01)}
{at(player_01,pos_2_2), at(player_01,pos_2_2,0), at(player_01,pos_2_2,3), at(player_01,pos_3_2,1), at(player_01,pos_3_2,2), at(stone_01,pos_3_2), at(stone_01,pos_3_2,0), at(stone_01,pos_4_2,1), at(stone_01,pos_4_2,2), at(stone_01,pos_4_2,3), atgoal(stone_01,1), atgoal(stone_01,2), atgoal(stone_01,3), clear(pos_2_2,1), clear(pos_2_2,2), clear(pos_3_2,3), clear(pos_4_2), clear(pos_4_2,0), del(at(player_01,pos_2_2),1), del(at(player_01,pos_3_2),3), del(at(stone_01,pos_3_2),1), del(clear(pos_2_2),3), del(clear(pos_4_2),1), goal(stone_01), goalreached, isgoal(pos_4_2), isnongoal(pos_1_1), isnongoal(pos_1_2), isnongoal(pos_1_3), isnongoal(pos_2_1), isnongoal(pos_2_2), isnongoal(pos_2_3), isnongoal(pos_3_1), isnongoal(pos_3_2), isnongoal(pos_3_3), isnongoal(pos_4_1), isnongoal(pos_4_3), isnongoal(pos_5_1), isnongoal(pos_5_2), isnongoal(pos_5_3), move(player_01,pos_3_2,pos_2_2,dir_left,3), movedir(pos_2_2,pos_3_2,dir_right), movedir(pos_3_2,pos_2_2,dir_left), movedir(pos_3_2,pos_4_2,dir_right), movedir(pos_4_2,pos_3_2,dir_left), noop(2), player(player_01), preconditions_m(player_01,pos_3_2,pos_2_2,dir_left,2), preconditions_m(player_01,pos_3_2,pos_2_2,dir_left,3), preconditions_pg(player_01,stone_01,pos_2_2,pos_3_2,pos_4_2,dir_right,1), pushtogoal(player_01,stone_01,pos_2_2,pos_3_2,pos_4_2,dir_right,1), step(1), step(2), step(3), stone(stone_01)}
{at(player_01,pos_2_2), at(player_01,pos_2_2,0), at(player_01,pos_2_2,2), at(player_01,pos_2_2,3), at(player_01,pos_3_2,1), at(stone_01,pos_3_2), at(stone_01,pos_3_2,0), at(stone_01,pos_4_2,1), at(stone_01,pos_4_2,2), at(stone_01,pos_4_2,3), atgoal(stone_01,1), atgoal(stone_01,2), atgoal(stone_01,3), clear(pos_2_2,1), clear(pos_3_2,2), clear(pos_3_2,3), clear(pos_4_2), clear(pos_4_2,0), del(at(player_01,pos_2_2),1), del(at(player_01,pos_3_2),2), del(at(stone_01,pos_3_2),1), del(clear(pos_2_2),2), del(clear(pos_4_2),1), goal(stone_01), goalreached, isgoal(pos_4_2), isnongoal(pos_1_1), isnongoal(pos_1_2), isnongoal(pos_1_3), isnongoal(pos_2_1), isnongoal(pos_2_2), isnongoal(pos_2_3), isnongoal(pos_3_1), isnongoal(pos_3_2), isnongoal(pos_3_3), isnongoal(pos_4_1), isnongoal(pos_4_3), isnongoal(pos_5_1), isnongoal(pos_5_2), isnongoal(pos_5_3), move(player_01,pos_3_2,pos_2_2,dir_left,2), movedir(pos_2_2,pos_3_2,dir_right), movedir(pos_3_2,pos_2_2,dir_left), movedir(pos_3_2,pos_4_2,dir_right), movedir(pos_4_2,pos_3_2,dir_left), noop(3), player(player_01), preconditions_m(player_01,pos_2_2,pos_3_2,dir_right,3), preconditions_m(player_01,pos_3_2,pos_2_2,dir_left,2), preconditions_pg(player_01,stone_01,pos_2_2,pos_3_2,pos_4_2,dir_right,1), pushtogoal(player_01,stone_01,pos_2_2,pos_3_2,pos_4_2,dir_right,1), step(1), step(2), step(3), stone(stone_01)}
{at(player_01,pos_2_2), at(player_01,pos_2_2,0), at(player_01,pos_2_2,2), at(player_01,pos_3_2,1), at(player_01,pos_3_2,3), at(stone_01,pos_3_2), at(stone_01,pos_3_2,0), at(stone_01,pos_4_2,1), at(stone_01,pos_4_2,2), at(stone_01,pos_4_2,3), atgoal(stone_01,1), atgoal(stone_01,2), atgoal(stone_01,3), clear(pos_2_2,1), clear(pos_2_2,3), clear(pos_3_2,2), clear(pos_4_2), clear(pos_4_2,0), del(at(player_01,pos_2_2),1), del(at(player_01,pos_2_2),3), del(at(player_01,pos_3_2),2), del(at(stone_01,pos_3_2),1), del(clear(pos_2_2),2), del(clear(pos_3_2),3), del(clear(pos_4_2),1), goal(stone_01), goalreached, isgoal(pos_4_2), isnongoal(pos_1_1), isnongoal(pos_1_2), isnongoal(pos_1_3), isnongoal(pos_2_1), isnongoal(pos_2_2), isnongoal(pos_2_3), isnongoal(pos_3_1), isnongoal(pos_3_2), isnongoal(pos_3_3), isnongoal(pos_4_1), isnongoal(pos_4_3), isnongoal(pos_5_1), isnongoal(pos_5_2), isnongoal(pos_5_3), move(player_01,pos_2_2,pos_3_2,dir_right,3), move(player_01,pos_3_2,pos_2_2,dir_left,2), movedir(pos_2_2,pos_3_2,dir_right), movedir(pos_3_2,pos_2_2,dir_left), movedir(pos_3_2,pos_4_2,dir_right), movedir(pos_4_2,pos_3_2,dir_left), player(player_01), preconditions_m(player_01,pos_2_2,pos_3_2,dir_right,3), preconditions_m(player_01,pos_3_2,pos_2_2,dir_left,2), preconditions_pg(player_01,stone_01,pos_2_2,pos_3_2,pos_4_2,dir_right,1), pushtogoal(player_01,stone_01,pos_2_2,pos_3_2,pos_4_2,dir_right,1), step(1), step(2), step(3), stone(stone_01)}
"""
