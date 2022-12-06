input = """
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%               DLV META-INTERPRETER
%
% for preferred answer sets of prop. prioritized LP (P,<)
% using the graph algorithm of (Brewka & Eiter 1999).
% 
% INPUT: Rules
%  
%   r:  A <- B1,...,Bm, not C1,..., not Cn
%
% are represented by facts: 
% 
%  rule(r). head(A,r). pbl(B1,r). ... pbl(Bm,r).
%                      nbl(C1,r). ... nbl(Cn,r).
%
% Classical negation is represented by facts
%
%   compl(L,L').
%
% for complementary classical literals L and L'.
%
% Rule preferences r < r' are specified as facts
%
%   pr(r,r'). 
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Represent answer set:

  in_AS(X) :- head(X,Y), not pos_body_false(Y),
                         not neg_body_false(Y). 

  pos_body_false(Y) :- pbl(X,Y), not in_AS(X). 
  neg_body_false(Y) :- nbl(X,Y), in_AS(X). 

% Eliminate opposite literals (explicitly specified).

  :- compl(X,Y), in_AS(X), in_AS(Y).


% GRAPH ALGORITHM 

% label 'g' nodes and 'z' nodes (other labels are uninteresting): 

 g(R) :- rule(R), not neg_body_false(R), not pos_body_false(R). 

 z(R) :- rule(R), not pos_body_false(R), head(X,R), not in_AS(X).

% use rules ids as time stamps. 

 time(T) :- rule(T). 

% initial step of the algorithm: consider global source nodes. Only
% non-z nodes can be removed

 nosource0(R) :- pr(R1,R). 

 remove(R,T) :- rule(R), not nosource0(R), not z(R), time(T). 

% other steps in the algorithm: remove non-z nodes and, under some
% conditions, also z-nodes

 nosource(R,T) :- pr(R1,R), time(T), not remove(R1,T). 

 remove(R,T1) :- rule(R), not nosource(R,T), time(T), time(T1),
                 T < T1, not z(R), not remove(R,T).

 remove(R,T1) :- rule(R), not nosource(R,T), time(T), time(T1),
                 T < T1, z(R), nbl(X,R), s(X,T).

% add the head if a removed generating rule to the set T  

 s(X,T) :- remove(R,T), g(R), head(X,R). 

% check whether all rules are removed. 

 removed(R) :- remove(R,T). 

 :- rule(R), not removed(R). 


"""
output = """
{}
"""
