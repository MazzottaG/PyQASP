input = """
% Simplification of bug.82, showing that also excess models could be produced
% due to the same bug.

%%
% Guess depots for highway (at restaurant locations)
%%

mcDepot(H,K) | -mcDepot(H,K) :- mcBurger(H,K). 

%%
% Check required number of depots
%%

:- nDepots(H,N), not #count{K : mcDepot(H,K)} = N.

%%
% Optimize local distance per highway
%%

%%
% Auxiliary predicates
%%

% For each highway, compute all distances between restaurants and depots
dist(H,K,K,0) :- mcBurger(H,K), mcDepot(H,K).
dist(H,I,J,D) :- mcBurger(H,I), mcDepot(H,J), I>J, I=D+J.
dist(H,I,J,D) :- mcBurger(H,I), mcDepot(H,J), J>I, J=D+I.

% For each highway and any restaurant, determine local distance (to next depot)
loc_dist(H,I,D) :- dist(H,I,J,D), #min{S : dist(H,I,K,S)} = D.



%%
% Input for Highway A1
%%

highway(1).
km(1,25).
nDepots(1,2).

%%
% Restaurants along A1
%%

mcBurger(1, 4).
mcBurger(1,12).
mcBurger(1,21).

%%
% Just for Testing
%%

mcDepot(1,12).


"""
output = """
{highway(1), km(1,25), nDepots(1,2), mcBurger(1,4), mcBurger(1,12), mcBurger(1,21), mcDepot(1,12), mcDepot(1,4), -mcDepot(1,21), dist(1,4,12,8), dist(1,12,4,8), dist(1,21,12,9), dist(1,21,4,17), dist(1,4,4,0), dist(1,12,12,0), loc_dist(1,21,9), loc_dist(1,4,0), loc_dist(1,12,0)}
{highway(1), km(1,25), nDepots(1,2), mcBurger(1,4), mcBurger(1,12), mcBurger(1,21), mcDepot(1,12), -mcDepot(1,4), mcDepot(1,21), dist(1,4,12,8), dist(1,4,21,17), dist(1,12,21,9), dist(1,21,12,9), dist(1,12,12,0), dist(1,21,21,0), loc_dist(1,4,8), loc_dist(1,12,0), loc_dist(1,21,0)}
"""
