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
q(1,4).
r(1,2).

%----  #sum{...} op var ----(at the end)

okay1(M, N) :- p(M, N), #sum{V : a(V), b, c} = N.           

okay2(M, N) :- p(M, N), #sum{V : a(V), b, c} < N.           
 
okay3(M, N) :- p(M, N), #sum{V : a(V), b, c} <= N.          

okay4(M, N) :- p(M, N), #sum{V : a(V), b, c} > N.           
    
okay5(M, N) :- p(M, N), #sum{V : a(V), b, c} >= N.          

:- p(M, N), #sum{V : a(V), b, c} = M.           
           

:- p(M, N), #sum{V : a(V), b, c} < M.           
 
   
:- p(M, N), #sum{V : a(V), b, c} <= M.          
          

:- q(M, N), #sum{V : a(V), b, c} > N.           
 

:- q(M, N), #sum{V : a(V), b, c} >= N.


%----  #sum{...} op var ----(at the beginning)

okay6(M, N) :- #sum{V : a(V), b, c} = N, p(M, N).           

okay7(M, N) :- #sum{V : a(V), b, c} < N, p(M, N).           

okay8(M, N) :- #sum{V : a(V), b, c} <= N, p(M, N).          

okay9(M, N) :- #sum{V : a(V), b, c} > N, p(M, N).           

okay10(M, N) :- #sum{V : a(V), b, c} >= N, p(M, N).    

:- #sum{V : a(V), b, c} = N, q(M, N).  
         

:- #sum{V : a(V), b, c} < M, p(M, N).      
           

:- #sum{V : a(V), b, c} <= M, p(M, N). 
          

:- #sum{V : a(V), b, c} > N, q(M, N).  
          

:- #sum{V : a(V), b, c} >= N, q(M, N). 
     
 
%----  var op #sum{...}----(at the end) 

okay11(M, N) :- p(M, N), N = #sum{V : a(V), b, c}.           

okay12(M, N) :- p(M, N), N < #sum{V : a(V), b, c}.           

okay13(M, N) :- p(M, N), N <= #sum{V : a(V), b, c}.           

okay14(M, N) :- p(M, N), N  > #sum{V : a(V), b, c}.           

okay15(M, N) :- p(M, N), N >= #sum{V : a(V), b, c}.     

:- p(M, N), M = #sum{V : a(V), b, c}.           
  
 
:- q(M, N), N < #sum{V : a(V), b, c}.           


:- q(M, N), N <= #sum{V : a(V), b, c}.           


:- p(M, N), M > #sum{V : a(V), b, c}.           
           

:- p(M, N), M >= #sum{V : a(V), b, c}.          
          

%----  var op #sum{...}---- (at the beginning)

okay16(M, N) :- N = #sum{V : a(V), b, c}, p(M, N).          

okay17(M, N) :- N < #sum{V : a(V), b, c}, p(M, N).         

okay18(M, N) :- N <= #sum{V : a(V), b, c}, p(M, N).         

okay19(M, N) :- N > #sum{V : a(V), b, c}, p(M, N).          

okay20(M, N) :- N >= #sum{V : a(V), b, c}, p(M, N).         

:- M = #sum{V : a(V), b, c}, p(M, N).          
   

:- N < #sum{V : a(V), b, c}, q(M, N).         
         

:- N <= #sum{V : a(V), b, c}, q(M, N). 
         

:- M > #sum{V : a(V), b, c}, p(M, N).          
          

:- M >= #sum{V : a(V), b, c}, p(M, N).     
     

%---- var < #sum{...} < var ----


okay21(M, N) :- p(M, N), M < #sum{V : a(V), b, c} < N.      

okay22(M, N) :- M < #sum{V : a(V), b, c} < N, p(M, N). 
    


:- r(M, N), M < #sum{V : a(V), b, c} < N.      

:- M < #sum{V : a(V), b, c} < N, r(M, N).   
  


      

     

%---- var < #sum{...} <= var ----


okay23(M, N) :- p(M, N), M < #sum{V : a(V), b, c} <= N.     

okay24(M, N) :- M < #sum{V : a(V), b, c} <= N, p(M, N). 



:- r(M, N), M < #sum{V : a(V), b, c} <= N.     

:- M < #sum{V : a(V), b, c} <= N, r(M, N).
    


     


    
%---- var <= #sum{...} < var ----


okay25(M, N) :- p(M, N), M <= #sum{V : a(V), b, c} < N.     

okay26(M, N) :- M <= #sum{V : a(V), b, c} < N, p(M, N).     



:- r(M, N), M <= #sum{V : a(V), b, c} < N.     

:- M <= #sum{V : a(V), b, c} < N, r(M, N).  



     

  
   
%---- var <= #sum{...} <= var ----


okay27(M, N) :- p(M, N), M <= #sum{V : a(V), b, c} <= N.    

okay28(M, N) :- M <= #sum{V : a(V), b, c} <= N, p(M, N).    



:- r(M, N), M <= #sum{V : a(V), b, c} <= N.    

% The following constraint is always violated.
:- M <= #sum{V : a(V), b, c} <= N, p(M, N).    



    

    
"""
output = """
INCOHERENT
"""
