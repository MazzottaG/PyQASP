input = """
% Aggregates defined into the body of rules, constraints and weak constraints.
% No model is computed as, at least the last strong constraint is always violated.

a(1).
b.
c.
d.
p(1,2).
p(2,1).p(2,2).p(2,3).p(2,4).
p(3,2).p(3,3).p(3,4).
p(4,2).p(4,3).p(4,4).p(4,5).
p(5,4).

%----  #count{...} op var ----(at the end)

okay1(M, N) :- p(M, N), #count{V : a(V), b, c} = N.           
  
okay2(M, N) :- p(M, N), #count{V : a(V), b, c} < N.           
 
okay3(M, N) :- p(M, N), #count{V : a(V), b, c} <= N.          

okay4(M, N) :- p(M, N), #count{V : a(V), b, c} > N.           
    
okay5(M, N) :- p(M, N), #count{V : a(V), b, c} >= N.          

:- p(M, N), #count{V : a(V), b, c} = N.           

:- p(M, N), #count{V : a(V), b, c} < N.           
   
:- p(M, N), #count{V : a(V), b, c} <= N.          

:- p(M, N), #count{V : a(V), b, c} > N.           

:- p(M, N), #count{V : a(V), b, c} >= N.

%----  #count{...} op var ----(at the beginning)

okay6(M, N) :- #count{V : a(V), b, c} = N, p(M, N).           

okay7(M, N) :- #count{V : a(V), b, c} < N, p(M, N).           

okay8(M, N) :- #count{V : a(V), b, c} <= N, p(M, N).          

okay9(M, N) :- #count{V : a(V), b, c} > N, p(M, N).           

okay10(M, N) :- #count{V : a(V), b, c} >= N, p(M, N).    

:- #count{V : a(V), b, c} = N, p(M, N).  

:- #count{V : a(V), b, c} < N, p(M, N).      

:- #count{V : a(V), b, c} <= N, p(M, N). 

:- #count{V : a(V), b, c} > N, p(M, N).  

:- #count{V : a(V), b, c} >= N, p(M, N). 
 
%----  var op #count{...}----(at the end) 

okay11(M, N) :- p(M, N), N = #count{V : a(V), b, c}.           

okay12(M, N) :- p(M, N), N < #count{V : a(V), b, c}.           

okay13(M, N) :- p(M, N), N <= #count{V : a(V), b, c}.           

okay14(M, N) :- p(M, N), N  > #count{V : a(V), b, c}.           

okay15(M, N) :- p(M, N), N >= #count{V : a(V), b, c}.     

:- p(M, N), N = #count{V : a(V), b, c}.           
 
:- p(M, N), N < #count{V : a(V), b, c}.           

:- p(M, N), N <= #count{V : a(V), b, c}.           

:- p(M, N), N > #count{V : a(V), b, c}.           

:- p(M, N), N >= #count{V : a(V), b, c}.          

%----  var op #count{...}---- (at the beginning)

okay16(M, N) :- N = #count{V : a(V), b, c}, p(M, N).          

okay17(M, N) :- N < #count{V : a(V), b, c}, p(M, N).         

okay18(M, N) :- N <= #count{V : a(V), b, c}, p(M, N).         

okay19(M, N) :- N > #count{V : a(V), b, c}, p(M, N).          

okay20(M, N) :- N >= #count{V : a(V), b, c}, p(M, N).         

:- N = #count{V : a(V), b, c}, p(M, N).          

:- N < #count{V : a(V), b, c}, p(M, N).         

:- N <= #count{V : a(V), b, c}, p(M, N). 

:- N > #count{V : a(V), b, c}, p(M, N).          

:- N >= #count{V : a(V), b, c}, p(M, N).     

%---- var < #count{...} < var ----


okay21(M, N) :- p(M, N), M < #count{V : a(V), b, c} < N.      

okay22(M, N) :- M < #count{V : a(V), b, c} < N, p(M, N). 
    


:- p(M, N), M < #count{V : a(V), b, c} < N.      

:- M < #count{V : a(V), b, c} < N, p(M, N).   
  


%---- var < #count{...} <= var ----


okay23(M, N) :- p(M, N), M < #count{V : a(V), b, c} <= N.     

okay24(M, N) :- M < #count{V : a(V), b, c} <= N, p(M, N). 



:- p(M, N), M < #count{V : a(V), b, c} <= N.     

:- M < #count{V : a(V), b, c} <= N, p(M, N).
    
  
%---- var <= #count{...} < var ----


okay25(M, N) :- p(M, N), M <= #count{V : a(V), b, c} < N.     

okay26(M, N) :- M <= #count{V : a(V), b, c} < N, p(M, N).     



:- p(M, N), M <= #count{V : a(V), b, c} < N.     

:- M <= #count{V : a(V), b, c} < N, p(M, N).  


   
%---- var <= #count{...} <= var ----


okay27(M, N) :- p(M, N), M <= #count{V : a(V), b, c} <= N.    

okay28(M, N) :- M <= #count{V : a(V), b, c} <= N, p(M, N).    



:- p(M, N), M <= #count{V : a(V), b, c} <= N.    

% The following constraint is always violated.
:- M <= #count{V : a(V), b, c} <= N, p(M, N).    

"""
output = """
INCOHERENT
"""
