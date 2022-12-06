input = """
a(1).
a(2).
a(3).
b(1).
b(2).
c(1).
c(2).
c(3).
c(4).
d.
e(1).
e(2).
e(3).
e(4).
e(5).
p(1,3).
p(2,4).

%----  #count{...} op var ----(at the end)

okay1(M, N) :- p(M, N), #count{V : a(V)} = N.           
  
okay2(M, N) :- p(M, N), #count{V : a(V)} < N.           
 
okay3(M, N) :- p(M, N), #count{V : a(V)} <= N.          

okay4(M, N) :- p(M, N), #count{V : e(V)} > N.           
    
okay5(M, N) :- p(M, N), #count{V : c(V)} >= N.          

:- p(M, N), #count{V : a(V)} = M.           

:- p(M, N), #count{V : a(V)} < M.           
   
:- p(M, N), #count{V : a(V)} <= M.          

:- p(M, N), #count{V : a(V)} > N.           

:- p(M, N), #count{V : b(V)} >= N.

%----  #count{...} op var ----(at the beginning)

okay6(M, N) :- #count{V : a(V)} = N, p(M, N).           

okay7(M, N) :- #count{V : a(V)} < N, p(M, N).           

okay8(M, N) :- #count{V : a(V)} <= N, p(M, N).          

okay9(M, N) :- #count{V : a(V)} > M, p(M, N).           

okay10(M, N) :- #count{V : a(V)} >= N, p(M, N).    

:- #count{V : b(V)} = N, p(M, N).  

:- #count{V : a(V)} < M, p(M, N).

:- #count{V : a(V)} <= M, p(M, N). 

:- #count{V : a(V)} > N, p(M, N).  

:- #count{V : b(V)} >= N, p(M, N). 
 
%----  var op #count{...}----(at the end) 


okay11(M, N) :- p(M, N), N = #count{V : a(V)}.           

okay12(M, N) :- p(M, N), M < #count{V : a(V)}.           

okay13(M, N) :- p(M, N), N <= #count{V : a(V)}.           

okay14(M, N) :- p(M, N), N  > #count{V : a(V)}.           

okay15(M, N) :- p(M, N), N >= #count{V : a(V)}.     


:- p(M, N), N = #count{V : b(V)}.           

 
:- p(M, N), N < #count{V : a(V)}.           

:- p(M, N), N <= #count{V : b(V)}.           

:- p(M, N), M > #count{V : a(V)}.           

:- p(M, N), M >= #count{V : a(V)}.          

%----  var op #count{...}---- (at the beginning)

okay16(M, N) :- N = #count{V : a(V)}, p(M, N).          

okay17(M, N) :- M < #count{V : a(V)}, p(M, N).         

okay18(M, N) :- N <= #count{V : a(V)}, p(M, N).         

okay19(M, N) :- N > #count{V : a(V)}, p(M, N).          

okay20(M, N) :- N >= #count{V : a(V)}, p(M, N).         

:- M = #count{V : a(V)}, p(M, N).          

:- N < #count{V : a(V)}, p(M, N).         

:- N <= #count{V : b(V)}, p(M, N). 

:- M > #count{V : a(V)}, p(M, N).          

:- M >= #count{V : a(V)}, p(M, N).     

%---- var < #count{...} < var ----


okay21(M, N) :- p(M, N), M < #count{V : a(V)} < N.      

okay22(M, N) :- M < #count{V : a(V)} < N, p(M, N). 
    


:- p(M, N), M < #count{V : e(V)} < N.      

:- M < #count{V : e(V), d} < N, p(M, N).   
  



%---- var < #count{...} <= var ----


okay23(M, N) :- p(M, N), M < #count{V : a(V)} <= N.     

okay24(M, N) :- M < #count{V : a(V)} <= N, p(M, N). 



:- p(M, N), M < #count{V : e(V)} <= N.     

:- M < #count{V : e(V)} <= N, p(M, N).



%---- var <= #count{...} < var ----


okay25(M, N) :- p(M, N), M <= #count{V : a(V)} < N.     

okay26(M, N) :- M <= #count{V : b(V)} < N, p(M, N).     



:- p(M, N), M <= #count{V : e(V), d} < N.     

:- M <= #count{V : e(V), d} < N, p(M, N).  



   
%---- var <= #count{...} <= var ----


okay27(M, N) :- p(M, N), M <= #count{V : a(V)} <= N.    

okay28(M, N) :- M <= #count{V : a(V)} <= N, p(M, N).    



:- p(M, N), M <= #count{V : e(V)} <= N.    

:- M <= #count{V : e(V), d} <= N, p(M, N).    



"""
output = """
{a(1), a(2), a(3), b(1), b(2), c(1), c(2), c(3), c(4), d, e(1), e(2), e(3), e(4), e(5), okay1(1,3), okay10(1,3), okay11(1,3), okay12(1,3), okay12(2,4), okay13(1,3), okay14(2,4), okay15(1,3), okay15(2,4), okay16(1,3), okay17(1,3), okay17(2,4), okay18(1,3), okay19(2,4), okay2(2,4), okay20(1,3), okay20(2,4), okay21(2,4), okay22(2,4), okay23(1,3), okay23(2,4), okay24(1,3), okay24(2,4), okay25(2,4), okay26(1,3), okay26(2,4), okay27(1,3), okay27(2,4), okay28(1,3), okay28(2,4), okay3(1,3), okay3(2,4), okay4(1,3), okay4(2,4), okay5(1,3), okay5(2,4), okay6(1,3), okay7(2,4), okay8(1,3), okay8(2,4), okay9(1,3), okay9(2,4), p(1,3), p(2,4)}
"""
