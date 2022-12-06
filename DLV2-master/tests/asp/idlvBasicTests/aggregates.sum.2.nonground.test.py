input = """
a(1).
a(2).
a(3).
b(2).
b(3).
%c(1).
%c(2).
c(3).
c(4).
d.
e(1).
e(2).
e(3).
e(4).
e(5).
%p(1,3).
%p(2,4).
p(1,6).
%p(1,7).

%----  #sum{...} op var ----(at the end)

okay1(M, N) :- p(M, N), #sum{V : a(V)} = N.           
  
okay2(M, N) :- p(M, N), #sum{V : b(V)} < N.           
 
okay3(M, N) :- p(M, N), #sum{V : a(V)} <= N.          

okay4(M, N) :- p(M, N), #sum{V : c(V)} > N.           
    
okay5(M, N) :- p(M, N), #sum{V : a(V)} >= N.          

:- p(M, N), #sum{V : a(V)} = M.           
           

:- p(M, N), #sum{V : a(V)} < M.           
 
   
:- p(M, N), #sum{V : a(V)} <= M.          
          

:- p(M, N), #sum{V : a(V)} > N.           
 

:- p(M, N), #sum{V : b(V)} >= N.


%----  #sum{...} op var ----(at the beginning)

okay6(M, N) :- #sum{V : a(V)} = N, p(M, N).           

okay7(M, N) :- #sum{V : b(V)} < N, p(M, N).           

okay8(M, N) :- #sum{V : a(V)} <= N, p(M, N).          

okay9(M, N) :- #sum{V : c(V)} > M, p(M, N).           

okay10(M, N) :- #sum{V : a(V)} >= N, p(M, N).    

:- #sum{V : b(V)} = N, p(M, N).  
         

:- #sum{V : a(V)} < M, p(M, N).
           

:- #sum{V : a(V)} <= M, p(M, N). 
          

:- #sum{V : a(V)} > N, p(M, N).  
          

:- #sum{V : b(V)} >= N, p(M, N). 
     
 
%----  var op #sum{...}----(at the end) 


okay11(M, N) :- p(M, N), N = #sum{V : a(V)}.           

okay12(M, N) :- p(M, N), M < #sum{V : a(V)}.           

okay13(M, N) :- p(M, N), N <= #sum{V : a(V)}.           

okay14(M, N) :- p(M, N), N  > #sum{V : b(V)}.           

okay15(M, N) :- p(M, N), N >= #sum{V : a(V)}.     


:- p(M, N), N = #sum{V : b(V)}.           
  
 
:- p(M, N), N < #sum{V : a(V)}.           


:- p(M, N), N <= #sum{V : b(V)}.           


:- p(M, N), M > #sum{V : a(V)}.           
           

:- p(M, N), M >= #sum{V : a(V)}.          
          

%----  var op #sum{...}---- (at the beginning)

okay16(M, N) :- N = #sum{V : a(V)}, p(M, N).          

okay17(M, N) :- M < #sum{V : a(V)}, p(M, N).         

okay18(M, N) :- N <= #sum{V : a(V)}, p(M, N).         

okay19(M, N) :- N > #sum{V : b(V)}, p(M, N).          

okay20(M, N) :- N >= #sum{V : a(V)}, p(M, N).         

:- M = #sum{V : a(V)}, p(M, N).          
   

:- N < #sum{V : a(V)}, p(M, N).         
         

:- N <= #sum{V : b(V)}, p(M, N). 
         

:- M > #sum{V : a(V)}, p(M, N).          
          

:- M >= #sum{V : a(V)}, p(M, N).     
     

%---- var < #sum{...} < var ----


okay21(M, N) :- p(M, N), M < #sum{V : b(V)} < N.      

okay22(M, N) :- M < #sum{V : b(V)} < N, p(M, N). 
    


:- p(M, N), M < #sum{V : a(V)} < N.      

:- M < #sum{V : a(V), d} < N, p(M, N).   
  


      

     

%---- var < #sum{...} <= var ----


okay23(M, N) :- p(M, N), M < #sum{V : a(V)} <= N.     

okay24(M, N) :- M < #sum{V : a(V)} <= N, p(M, N). 



:- p(M, N), M < #sum{V : c(V)} <= N.     

:- M < #sum{V : c(V)} <= N, p(M, N).


     



%---- var <= #sum{...} < var ----


okay25(M, N) :- p(M, N), M <= #sum{V : b(V)} < N.     

okay26(M, N) :- M <= #sum{V : b(V)} < N, p(M, N).     



:- p(M, N), M <= #sum{V : c(V), d} < N.     

:- M <= #sum{V : c(V), d} < N, p(M, N).  



     

  
   
%---- var <= #sum{...} <= var ----


okay27(M, N) :- p(M, N), M <= #sum{V : a(V)} <= N.    

okay28(M, N) :- M <= #sum{V : a(V)} <= N, p(M, N).    



:- p(M, N), M <= #sum{V : c(V)} <= N.    

:- M <= #sum{V : c(V), d} <= N, p(M, N).    



    


"""
output = """
{a(1), a(2), a(3), b(2), b(3), c(3), c(4), d, e(1), e(2), e(3), e(4), e(5), okay1(1,6), okay10(1,6), okay11(1,6), okay12(1,6), okay13(1,6), okay14(1,6), okay15(1,6), okay16(1,6), okay17(1,6), okay18(1,6), okay19(1,6), okay2(1,6), okay20(1,6), okay21(1,6), okay22(1,6), okay23(1,6), okay24(1,6), okay25(1,6), okay26(1,6), okay27(1,6), okay28(1,6), okay3(1,6), okay4(1,6), okay5(1,6), okay6(1,6), okay7(1,6), okay8(1,6), okay9(1,6), p(1,6)}
"""
