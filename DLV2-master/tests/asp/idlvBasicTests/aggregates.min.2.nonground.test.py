input = """
a(3).
a(4).
a(5).
b(2).
b(3).
c(4).
c(5).
d.
p(1,3).


%----  #min{...} op var ----(at the end)

okay1(M, N) :- p(M, N), #min{V : a(V)} = N.           
  
okay2(M, N) :- p(M, N), #min{V : b(V)} < N.           
 
okay3(M, N) :- p(M, N), #min{V : a(V)} <= N.          

okay4(M, N) :- p(M, N), #min{V : c(V)} > N.           
    
okay5(M, N) :- p(M, N), #min{V : a(V)} >= N.          

:- p(M, N), #min{V : a(V)} = M.           
           

:- p(M, N), #min{V : a(V)} < M.           
 
   
:- p(M, N), #min{V : a(V)} <= M.          
          

:- p(M, N), #min{V : a(V)} > N.           
 

:- p(M, N), #min{V : b(V)} >= N.


%----  #min{...} op var ----(at the beginning)

okay6(M, N) :- #min{V : a(V)} = N, p(M, N).           

okay7(M, N) :- #min{V : b(V)} < N, p(M, N).           

okay8(M, N) :- #min{V : a(V)} <= N, p(M, N).          

okay9(M, N) :- #min{V : c(V)} > M, p(M, N).           

okay10(M, N) :- #min{V : a(V)} >= N, p(M, N).    

:- #min{V : b(V)} = N, p(M, N).  
         

:- #min{V : a(V)} < M, p(M, N).
           

:- #min{V : a(V)} <= M, p(M, N). 
          

:- #min{V : a(V)} > N, p(M, N).  
          

:- #min{V : b(V)} >= N, p(M, N). 
     
 
%----  var op #min{...}----(at the end) 


okay11(M, N) :- p(M, N), N = #min{V : a(V)}.           

okay12(M, N) :- p(M, N), M < #min{V : a(V)}.           

okay13(M, N) :- p(M, N), N <= #min{V : a(V)}.           

okay14(M, N) :- p(M, N), N  > #min{V : b(V)}.           

okay15(M, N) :- p(M, N), N >= #min{V : a(V)}.     


:- p(M, N), N = #min{V : b(V)}.           
  
 
:- p(M, N), N < #min{V : a(V)}.           


:- p(M, N), N <= #min{V : b(V)}.           


:- p(M, N), M > #min{V : a(V)}.           
           

:- p(M, N), M >= #min{V : a(V)}.          
          

%----  var op #min{...}---- (at the beginning)

okay16(M, N) :- N = #min{V : a(V)}, p(M, N).          

okay17(M, N) :- M < #min{V : a(V)}, p(M, N).         

okay18(M, N) :- N <= #min{V : a(V)}, p(M, N).         

okay19(M, N) :- N > #min{V : b(V)}, p(M, N).          

okay20(M, N) :- N >= #min{V : a(V)}, p(M, N).         

:- M = #min{V : a(V)}, p(M, N).          
   

:- N < #min{V : a(V)}, p(M, N).         
         

:- N <= #min{V : b(V)}, p(M, N). 
         

:- M > #min{V : a(V)}, p(M, N).          
          

:- M >= #min{V : a(V)}, p(M, N).     
     

%---- var < #min{...} < var ----


okay21(M, N) :- p(M, N), M < #min{V : b(V)} < N.      

okay22(M, N) :- M < #min{V : b(V)} < N, p(M, N). 
    


:- p(M, N), M < #min{V : a(V)} < N.      

:- M < #min{V : a(V), d} < N, p(M, N).   
  


      

     

%---- var < #min{...} <= var ----


okay23(M, N) :- p(M, N), M < #min{V : a(V)} <= N.     

okay24(M, N) :- M < #min{V : a(V)} <= N, p(M, N). 



:- p(M, N), M < #min{V : c(V)} <= N.     

:- M < #min{V : c(V)} <= N, p(M, N).


     



%---- var <= #min{...} < var ----


okay25(M, N) :- p(M, N), M <= #min{V : b(V)} < N.     

okay26(M, N) :- M <= #min{V : b(V)} < N, p(M, N).     



:- p(M, N), M <= #min{V : c(V), d} < N.     

:- M <= #min{V : c(V), d} < N, p(M, N).  



     

  
   
%---- var <= #min{...} <= var ----


okay27(M, N) :- p(M, N), M <= #min{V : a(V)} <= N.    

okay28(M, N) :- M <= #min{V : a(V)} <= N, p(M, N).    



:- p(M, N), M <= #min{V : c(V)} <= N.    

:- M <= #min{V : c(V), d} <= N, p(M, N).    



    


"""
output = """
{a(3), a(4), a(5), b(2), b(3), c(4), c(5), d, okay1(1,3), okay10(1,3), okay11(1,3), okay12(1,3), okay13(1,3), okay14(1,3), okay15(1,3), okay16(1,3), okay17(1,3), okay18(1,3), okay19(1,3), okay2(1,3), okay20(1,3), okay21(1,3), okay22(1,3), okay23(1,3), okay24(1,3), okay25(1,3), okay26(1,3), okay27(1,3), okay28(1,3), okay3(1,3), okay4(1,3), okay5(1,3), okay6(1,3), okay7(1,3), okay8(1,3), okay9(1,3), p(1,3)}
"""
