input = """

% map date to weekday (mon =1, ..., sun = 7);
% The tour starts on Monday. 

 weekday(1,1).
 weekday(D,W) :- D=D1+1, W=W1+1, weekday(D1,W1), W1 < 7.
 weekday(D,1) :- D=D1+1, weekday(D1,7).

% connections with default costs (capitols of the Austrian federal states).

 conn(brg,ibk,2).
 conn(ibk,sbg,2).
 conn(ibk,wie,5).
 conn(ibk,kla,3).
 conn(sbg,kla,2).
 conn(sbg,gra,2).
 conn(sbg,lin,1).
 conn(sbg,wie,3).
 conn(kla,gra,2).
 conn(lin,stp,1).
 conn(lin,wie,2).
 conn(lin,gra,2).
 conn(gra,wie,2).
 conn(gra,eis,1).
 conn(stp,wie,1).
 conn(eis,wie,1).
 conn(stp,eis,2).

conn(B,A,C) :- conn(A,B,C).

 city(T) :- conn(T,_,_).

% costing: use default cost, if there are no extra costs 
%

 cost(A,B,W,C) :- conn(A,B,C), 0 < W, W <= 7, %#int(W), 0 < W, W <= 7, 
                  not ecost(A,B,W).  

  ecost(A,B,W) :- ex_cost(A,B,W,C).
 cost(A,B,W,C) :- ex_cost(A,B,W,C).

 ex_cost(A,B,W,C) :- ex_cost(B,A,W,C). 

% Some example of an extra cost.

  ex_cost(stp,eis,2,10).

% Nicer would be, in an unstratified program, 
% to have no "ex_cost" but only cost, and define
% 
% ecost(A,B,W) :- cost(A,B,W,C), dcost(A,B,C1), C != C1.
% ecost(A,B,W) :- ecost(B,A,W). 





"""
output = """
"""
