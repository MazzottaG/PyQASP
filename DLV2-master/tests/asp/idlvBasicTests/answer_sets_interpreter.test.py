input = """
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% dlv-Interpreter for preferred answer sets of propositional programs. 
% 
% Store rules
%  
%     r:  A <- B1,...,Bm, not C1,..., not Cn
%
% by facts: 
% 
%  rule(r). head(A,r). bpl(B1,r). ... bpl(Bm,r). nbl(C1,r). ... nbl(Cn,r).
%
% classical negation must be emulated, state facts opp(L,L') for
% opposite classical literals 
%
% Specify preference  r < r' through fact  pr(r,r'). 
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Apply rules of the program: 

  in_AS(X) :- head(X,Y), not pos_body_false(Y), not neg_body_false(Y). 

% pbl - positive body literal; nbl - negative body literal 

  pos_body_false(Y) :- pbl(X,Y), not in_AS(X). 
  neg_body_false(Y) :- nbl(X,Y), in_AS(X). 

% Eliminate opposite literals (explicitly specified).

  :- opp(X,Y), in_AS(X), in_AS(Y).


"""
output = """
{}
"""
