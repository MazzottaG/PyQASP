input = """
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%               DLV META-INTERPRETER
%
% for preferred answer sets of prop. prioritized LP (P,<). 
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

% For full prioritization: refine pr to a total ordering.

 pr(X,Y) | pr(Y,X) :- rule(X), rule(Y), X != Y. 

 pr(X,Z) :- pr(X,Y), pr(Y,Z). 

         :- pr(X,X). 

% Check dual reduct: Build sets S_i, use rule ids as indices i.
%
% lit(X,r): the literal x occurs in the set S_r

   lit(X,Y) :- head(X,Y), not pos_body_false(Y), 
               not defeat_local(Y), not in_AS(X). 

   lit(X,Y) :- head(X,Y), not pos_body_false(Y), 
               not defeat_local(Y), not defeat_global(Y). 

   defeat_local(Y)  :- nbl(X,Y), lit(X,Y1), pr(Y1,Y).  

   defeat_global(Y) :- nbl(X,Y), in_AS(X). 

% include literal into CP(.)

   in_CP(X) :- lit(X,Y).

   :- in_CP(X), not in_AS(X). 

"""
output = """
{}
"""
