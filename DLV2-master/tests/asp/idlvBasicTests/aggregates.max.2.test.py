input = """
% Aggregates defined into the body of rules, constraints and weak constraints.
% No model is computed as, at least the last strong constraint is always violated.

a(1).
a(2).
b.
c.
d.
p(1,2).
p(1,3).
p(1,4).
q(1,3).
r(1,2).
s(2,4).
t(3,4).

%----  #max{...} op var ----(at the end)
okay1(M, N) :- p(M, N), #max{V : a(V), b, c} = N.           

okay2(M, N) :- p(M, N), #max{V : a(V), b, c} < N.           
 
okay3(M, N) :- p(M, N), #max{V : a(V), b, c} <= N.          

okay4(M, N) :- p(M, N), #max{V : a(V), b, c} > M.           
    
okay5(M, N) :- p(M, N), #max{V : a(V), b, c} >= N.          

:- p(M, N), #max{V : a(V), b, c} = M.           
           

:- p(M, N), #max{V : a(V), b, c} < M.           
 
   
:- p(M, N), #max{V : a(V), b, c} <= M.          
          

:- q(M, N), #max{V : a(V), b, c} > N.           
 

:- q(M, N), #max{V : a(V), b, c} >= N.


%----  #max{...} op var ----(at the beginning)

okay6(M, N) :- #max{V : a(V), b, c} = N, p(M, N).           

okay7(M, N) :- #max{V : a(V), b, c} < N, p(M, N).           

okay8(M, N) :- #max{V : a(V), b, c} <= N, p(M, N).          

okay9(M, N) :- #max{V : a(V), b, c} > M, p(M, N).           

okay10(M, N) :- #max{V : a(V), b, c} >= N, p(M, N).    

:- #max{V : a(V), b, c} = N, q(M, N).  
         

:- #max{V : a(V), b, c} < M, p(M, N).      
           

:- #max{V : a(V), b, c} <= M, p(M, N). 
          

:- #max{V : a(V), b, c} > N, q(M, N).  
          

:- #max{V : a(V), b, c} >= N, q(M, N). 
     
 
%----  var op #max{...}----(at the end) 

okay11(M, N) :- p(M, N), N = #max{V : a(V), b, c}.           

okay12(M, N) :- p(M, N), M < #max{V : a(V), b, c}.           

okay13(M, N) :- p(M, N), N <= #max{V : a(V), b, c}.           

okay14(M, N) :- p(M, N), N  > #max{V : a(V), b, c}.           

okay15(M, N) :- p(M, N), N >= #max{V : a(V), b, c}.     

:- p(M, N), M = #max{V : a(V), b, c}.           
  
 
:- q(M, N), N < #max{V : a(V), b, c}.           


:- q(M, N), N <= #max{V : a(V), b, c}.           


:- p(M, N), M > #max{V : a(V), b, c}.           
           

:- p(M, N), M >= #max{V : a(V), b, c}.          
          

%----  var op #max{...}---- (at the beginning)

okay16(M, N) :- N = #max{V : a(V), b, c}, p(M, N).          

okay17(M, N) :- M < #max{V : a(V), b, c}, p(M, N).         

okay18(M, N) :- N <= #max{V : a(V), b, c}, p(M, N).         

okay19(M, N) :- N > #max{V : a(V), b, c}, p(M, N).          

okay20(M, N) :- N >= #max{V : a(V), b, c}, p(M, N).         

:- M = #max{V : a(V), b, c}, p(M, N).          
   

:- N < #max{V : a(V), b, c}, q(M, N).         
         

:- N <= #max{V : a(V), b, c}, q(M, N). 
         

:- M > #max{V : a(V), b, c}, p(M, N).          
          

:- M >= #max{V : a(V), b, c}, p(M, N).     
     

%---- var < #max{...} < var ----


okay21(M, N) :- p(M, N), M < #max{V : a(V), b, c} < N.      

okay22(M, N) :- M < #max{V : a(V), b, c} < N, p(M, N). 
    


:- r(M, N), M < #max{V : a(V), b, c} < N.      

:- M < #max{V : a(V), b, c} < N, r(M, N).   
  


      

     

%---- var < #max{...} <= var ----


okay23(M, N) :- q(M, N), M < #max{V : a(V), b, c} <= N.     

okay24(M, N) :- M < #max{V : a(V), b, c} <= N, q(M, N). 



:- s(M, N), M < #max{V : a(V), b, c} <= N.     

:- M < #max{V : a(V), b, c} <= N, s(M, N).
    


     


    
%---- var <= #max{...} < var ----


okay25(M, N) :- p(M, N), M <= #max{V : a(V), b, c} < N.     

okay26(M, N) :- M <= #max{V : a(V), b, c} < N, p(M, N).     



:- r(M, N), M <= #max{V : a(V), b, c} < N.     

:- M <= #max{V : a(V), b, c} < N, r(M, N).  



     

  
   
%---- var <= #max{...} <= var ----


okay27(M, N) :- s(M, N), M <= #max{V : a(V), b, c} <= N.    

okay28(M, N) :- M <= #max{V : a(V), b, c} <= N, s(M, N).    



:- t(M, N), M <= #max{V : a(V), b, c} <= N.    

% The following constraint is always violated.
:- M <= #max{V : a(V), b, c} <= N, p(M, N).    



    

    
"""
output = """
INCOHERENT
"""
