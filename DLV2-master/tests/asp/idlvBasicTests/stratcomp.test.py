input = """
% Strategic Companies
%
% As we want to produce X, Y or Z must be strategic.

 strategic(Y) | strategic(Z) :- produced_by(X,Y,Z).

% W is strategic, if it is controlled by strategic 
% companies X, Y, and Z  

 strategic(W) :- controlled_by(W,X,Y,Z), 
                 strategic(X), strategic(Y), strategic(Z). 

% Handle special 0 symbol 

 :- strategic(0). 
 strategic(X) :- strategic(X).
 strategic(0) :- true. 
 true. 
"""
output = """
INCOHERENT
"""
