input = """
a(1).
a(2).
a(3).
a(-1).
a(-3).
b(1).
b(2).
b(-5).
%c(1).
%c(2).
c(3).
c(4).
c(-1).
c(-20).
c(-2).
d.
e(1).
e(2).
e(3).
e(4).
e(5).
e(-4).
e(-8).
e(-1254).
p(1,3).
p(2,4).
%p(1,6).
%p(1,7).
p(-4,4).
p(-100,100).
p(-5,8).
%----  #max{...} op var ----(at the end)

okay1(M, N) :- p(M, N), #max{V : a(V)} = N.           
  
okay2(M, N) :- p(M, N), #max{V : b(V)} < N.           
 
okay3(M, N) :- p(M, N), #max{V : a(V)} <= N.          

okay4(M, N) :- p(M, N), #max{V : c(V)} > N.           
    
okay5(M, N) :- p(M, N), #max{V : a(V)} >= N.          




%----  #max{...} op var ----(at the beginning)

okay6(M, N) :- #max{V : a(V)} = N, p(M, N).           

okay7(M, N) :- #max{V : b(V)} < N, p(M, N).           

okay8(M, N) :- #max{V : a(V)} <= N, p(M, N).          

okay9(M, N) :- #max{V : c(V)} > M, p(M, N).           

okay10(M, N) :- #max{V : a(V)} >= N, p(M, N).    


     
 
%----  var op #max{...}----(at the end) 


okay11(M, N) :- p(M, N), N = #max{V : a(V)}.           

okay12(M, N) :- p(M, N), M < #max{V : a(V)}.           

okay13(M, N) :- p(M, N), N <= #max{V : a(V)}.           

okay14(M, N) :- p(M, N), N  > #max{V : b(V)}.           

okay15(M, N) :- p(M, N), N >= #max{V : a(V)}.     


      
          

%----  var op #max{...}---- (at the beginning)

okay16(M, N) :- N = #max{V : a(V)}, p(M, N).          

okay17(M, N) :- M < #max{V : a(V)}, p(M, N).         

okay18(M, N) :- N <= #max{V : a(V)}, p(M, N).         

okay19(M, N) :- N > #max{V : b(V)}, p(M, N).          

okay20(M, N) :- N >= #max{V : a(V)}, p(M, N).         


     

%---- var < #max{...} < var ----


okay21(M, N) :- p(M, N), M < #max{V : b(V)} < N.      

okay22(M, N) :- M < #max{V : b(V)} < N, p(M, N). 
    




      

     

%---- var < #max{...} <= var ----


okay23(M, N) :- p(M, N), M < #max{V : a(V)} <= N.     

okay24(M, N) :- M < #max{V : a(V)} <= N, p(M, N). 




     



%---- var <= #max{...} < var ----


okay25(M, N) :- p(M, N), M <= #max{V : b(V)} < N.     

okay26(M, N) :- M <= #max{V : b(V)} < N, p(M, N).     





     

  
   
%---- var <= #max{...} <= var ----


okay27(M, N) :- p(M, N), M <= #max{V : a(V)} <= N.    

okay28(M, N) :- M <= #max{V : a(V)} <= N, p(M, N).    





    


"""
output = """
{a(-1), a(-3), a(1), a(2), a(3), b(-5), b(1), b(2), c(-1), c(-2), c(-20), c(3), c(4), d, e(-1254), e(-4), e(-8), e(1), e(2), e(3), e(4), e(5), okay1(1,3), okay10(1,3), okay11(1,3), okay12(-100,100), okay12(-4,4), okay12(-5,8), okay12(1,3), okay12(2,4), okay13(1,3), okay14(-100,100), okay14(-4,4), okay14(-5,8), okay14(1,3), okay14(2,4), okay15(-100,100), okay15(-4,4), okay15(-5,8), okay15(1,3), okay15(2,4), okay16(1,3), okay17(-100,100), okay17(-4,4), okay17(-5,8), okay17(1,3), okay17(2,4), okay18(1,3), okay19(-100,100), okay19(-4,4), okay19(-5,8), okay19(1,3), okay19(2,4), okay2(-100,100), okay2(-4,4), okay2(-5,8), okay2(1,3), okay2(2,4), okay20(-100,100), okay20(-4,4), okay20(-5,8), okay20(1,3), okay20(2,4), okay21(-100,100), okay21(-4,4), okay21(-5,8), okay21(1,3), okay22(-100,100), okay22(-4,4), okay22(-5,8), okay22(1,3), okay23(-100,100), okay23(-4,4), okay23(-5,8), okay23(1,3), okay23(2,4), okay24(-100,100), okay24(-4,4), okay24(-5,8), okay24(1,3), okay24(2,4), okay25(-100,100), okay25(-4,4), okay25(-5,8), okay25(1,3), okay25(2,4), okay26(-100,100), okay26(-4,4), okay26(-5,8), okay26(1,3), okay26(2,4), okay27(-100,100), okay27(-4,4), okay27(-5,8), okay27(1,3), okay27(2,4), okay28(-100,100), okay28(-4,4), okay28(-5,8), okay28(1,3), okay28(2,4), okay3(-100,100), okay3(-4,4), okay3(-5,8), okay3(1,3), okay3(2,4), okay4(1,3), okay5(1,3), okay6(1,3), okay7(-100,100), okay7(-4,4), okay7(-5,8), okay7(1,3), okay7(2,4), okay8(-100,100), okay8(-4,4), okay8(-5,8), okay8(1,3), okay8(2,4), okay9(-100,100), okay9(-4,4), okay9(-5,8), okay9(1,3), okay9(2,4), p(-100,100), p(-4,4), p(-5,8), p(1,3), p(2,4)}
"""