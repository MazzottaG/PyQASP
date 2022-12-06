input = """
%Non ground rules, constraints and weak constraints with the aggregate
%function sum in the body. Guards are non ground, too.

a(3).
a(4).
b(1).
b(4).
c(4,1).
c(4,2).
c(3,1).
c(3,2).
p(3).
q(1).
%v(3).
t(4).


%----  #sum{nonGroundAtoms} op var ----(at the end)

okay01(M, N) :- p(M),q(N), #sum{V:a(M),b(N),c(M,V)} = M.           
  
okay02(M, N) :- p(M),t(N), #sum{V:a(M),b(N),c(M,V)} < N.           
 
okay03(M, N) :- p(M),q(N), #sum{V:a(M),b(N),c(M,V)} <= M.          

okay04(M, N) :- p(M),q(N), #sum{V:a(M),b(N),c(M,V)} > N.           
    
okay05(M, N) :- p(M),q(N), #sum{V:a(M),b(N),c(M,V)} >= N.          

:- p(M),q(N), #sum{V:a(M),b(N),c(M,V)} = N.
           

:- p(M),q(N), #sum{V:a(M),b(N),c(M,V)} < N.           
 
   
:- p(M),q(N), #sum{V:a(M),b(N),c(M,V)} <= N.          
          

:- p(M),q(N), #sum{V:a(M),b(N),c(M,V)} > M.           
 

:- p(M),t(N), #sum{V:a(M),b(N),c(V,M)} >= N.


%----  #sum{nonGroundAtoms} op var ----(at the beginning)

okay06(M, N) :- #sum{V:a(M),b(N),c(M,V)} = M, p(M),q(N).           

okay07(M, N) :- #sum{V:a(M),b(N),c(V,M)} < M, p(M),q(N).           

okay08(M, N) :- #sum{V:a(M),b(N),c(M,V)} <= M, p(M),q(N).          

okay09(M, N) :- #sum{V:a(M),b(N),c(M,V)} > N, p(M),q(N).           

okay10(M, N) :- #sum{V:a(M),b(N),c(M,V)} >= N, p(M),q(N).    

:- #sum{V:a(M),b(N),c(M,V)} = N, p(M),q(N).  
         

:- #sum{V:a(M),b(N),c(M,V)} < N, p(M),q(N).
           

:- #sum{V:a(M),b(N),c(M,V)} <= N, p(M),q(N). 
          

:- #sum{V:a(M),b(N),c(M,V)} > M, p(M),q(N).  
          

:- #sum{V:a(M),b(N),c(V,M)} >= N, p(M),t(N). 
     

%----  #sum{nonGroundAtoms} op var ----(in the middle)

okay11(M, N) :- p(M), #sum{V:a(M),b(N),c(M,V)} = M, q(N).           

okay12(M, N) :- p(M), #sum{V:a(M),b(N),c(V,M)} < M, q(N).           

okay13(M, N) :- p(M), #sum{V:a(M),b(N),c(M,V)} <= M, q(N).          

okay14(M, N) :- p(M), #sum{V:a(M),b(N),c(M,V)} > N, q(N).           

okay15(M, N) :- p(M),#sum{V:a(M),b(N),c(M,V)} >= N, q(N).    

:- p(M), #sum{V:a(M),b(N),c(M,V)} = N, q(N).  
         

:- p(M), #sum{V:a(M),b(N),c(M,V)} < N, q(N).
           

:- p(M), #sum{V:a(M),b(N),c(M,V)} <= N, q(N). 
          

:- p(M), #sum{V:a(M),b(N),c(M,V)} > M, q(N).  
          

:- p(M), #sum{V:a(M),b(N),c(V,M)} >= M, q(N). 

 
%----  var op #sum{nonGroundAtoms}----(at the end) 

okay16(M, N) :- p(M),q(N), M = #sum{V:a(M),b(N),c(M,V)}.           

okay17(M, N) :- p(M),q(N), N < #sum{V:a(M),b(N),c(M,V)}.           

okay18(M, N) :- p(M),q(N), M <= #sum{V:a(M),b(N),c(M,V)}.           

okay19(M, N) :- p(M),t(N), N  > #sum{V:a(M),b(N),c(M,V)}.           

okay20(M, N) :- p(M),q(N), M >= #sum{V:a(M),b(N),c(M,V)}.     

:- p(M),q(N), N = #sum{V:a(M),b(N),c(M,V)}.           
  
 
:- p(M),q(N), M < #sum{V:a(M),b(N),c(M,V)}.           


:- p(M),q(N), M <= #sum{V:a(M),b(N),c(V,M)}.           


:- p(M),q(N), N > #sum{V:a(M),b(N),c(M,V)}.           
           

:- p(M),q(N), N >= #sum{V:a(M),b(N),c(M,V)}.          
          

%----  var op #sum{nonGroundAtoms}---- (at the beginning)

okay21(M, N) :- M = #sum{V:a(M),b(N),c(M,V)}, p(M),q(N).          

okay22(M, N) :- N < #sum{V:a(M),b(N),c(M,V)}, p(M),q(N).         

okay23(M, N) :- M <= #sum{V:a(M),b(N),c(M,V)}, p(M),q(N).         

okay24(M, N) :- N > #sum{V:a(M),b(N),c(M,V)}, p(M),t(N).          

okay25(M, N) :- M >= #sum{V:a(M),b(N),c(M,V)}, p(M),q(N).         

:- N = #sum{V:a(M),b(N),c(M,V)}, p(M),q(N).          
   

:- M < #sum{V:a(M),b(N),c(M,V)}, p(M),q(N).         
         

:- M <= #sum{V:a(M),b(N),c(V,M)}, p(M),q(N). 
         

:- N > #sum{V:a(M),b(N),c(M,V)}, p(M),q(N).          
          

:- N >= #sum{V:a(M),b(N),c(M,V)}, p(M),q(N).     


    
%----  var op #sum{nonGroundAtoms}---- (in the middle)

okay26(M, N) :- p(M), M = #sum{V:a(M),b(N),c(M,V)}, q(N).          

okay27(M, N) :- p(M), N < #sum{V:a(M),b(N),c(M,V)}, q(N).         

okay28(M, N) :- p(M), M <= #sum{V:a(M),b(N),c(M,V)}, q(N).         

okay29(M, N) :- p(M), N > #sum{V:a(M),b(N),c(M,V)}, t(N).          

okay30(M, N) :- p(M), M >= #sum{V:a(M),b(N),c(M,V)}, q(N).         

:- p(M), N = #sum{V:a(M),b(N),c(M,V)}, q(N).          
   

:- p(M), M < #sum{V:a(M),b(N),c(M,V)}, q(N).         
         

:- p(M), M <= #sum{V:a(M),b(N),c(V,M)}, q(N). 
         

:- p(M), N >#sum{V:a(M),b(N),c(M,V)}, q(N).          
          

:- p(M), N >= #sum{V:a(M),b(N),c(M,V)}, q(N).     


%---- var < #sum{nonGroundAtoms} < var ----


okay31(M,N) :- t(M),q(N), N < #sum{V:a(M),b(N),c(M,V)} < M.    

okay32(M,N) :- N < #sum{V:a(M),b(N),c(M,V)} < M, t(M),q(N).

okay33(M,N) :- t(M), N < #sum{V:a(M),b(N),c(M,V)} < M, q(N).   


:- p(M),q(N), N < #sum{V:a(M),b(N),c(M,V)} < M.      

:- N < #sum{V:a(M),b(N),c(M,V)} < M, p(M),q(N).   
  
:- p(M), N < #sum{V:a(M),b(N),c(M,V)} < M, q(N).


      



     

%---- var < #sum{} <= var ----


okay34(M, N) :- p(M),q(N), N < #sum{V:a(M),b(N),c(M,V)} <= M.     

okay35(M, N) :- N < #sum{V:a(M),b(N),c(M,V)} <= M, p(M),q(N). 

okay36(M, N) :- p(M), N < #sum{V:a(M),b(N),c(M,V)} <= M, q(N).


:- p(M),q(N), N < #sum{V:a(M),b(N),c(V,M)} <= M.     

:- N < #sum{V:a(M),b(N),c(V,M)} <= M, p(M),q(N).

:- p(M), N < #sum{V:a(M),b(N),c(V,M)} <= M, q(N).  


     





%---- var <= #sum{nonGroundAtoms} < var ----

okay37(M, N) :- t(M),q(N), N <= #sum{V:a(M),b(N),c(M,V)} < M.     

okay38(M, N) :- N <= #sum{V:a(M),b(N),c(M,V)} < M, t(M),q(N).     

okay39(M, N) :- t(M), N <= #sum{V:a(M),b(N),c(M,V)} < M, q(N).


:- p(M),q(N), N <= #sum{V:a(M),b(N),c(M,V)} < M.     

:- N <= #sum{V:a(M),b(N),c(M,V)} < M, p(M),q(N).  

:- p(M), N <= #sum{V:a(M),b(N),c(M,V)} < M, q(N).


     




   
%---- var <= #sum{nonGroundAtoms} <= var ----


okay40(M, N) :- p(M),q(N), N <= #sum{V:a(M),b(N),c(M,V)} <= M.    

okay41(M, N) :- M <= #sum{V:a(M),b(N),c(M,V)} <= M, p(M),q(N).    

okay42(M, N) :- p(M), M <= #sum{V:a(M),b(N),c(M,V)} <= M, q(N).


:- v(M),t(N), M <= #sum{V:a(M),b(N),c(M,V)} <= N.  

:- M <= #sum{V:a(M),b(N),c(M,V)} <= N, v(M),t(N).

:- v(M), M <= #sum{V:a(M),b(N),c(M,V)} <= N, t(N).   



    



    




"""
output = """
{a(3), a(4), b(1), b(4), c(3,1), c(3,2), c(4,1), c(4,2), okay01(3,1), okay02(3,4), okay03(3,1), okay04(3,1), okay05(3,1), okay06(3,1), okay07(3,1), okay08(3,1), okay09(3,1), okay10(3,1), okay11(3,1), okay12(3,1), okay13(3,1), okay14(3,1), okay15(3,1), okay16(3,1), okay17(3,1), okay18(3,1), okay19(3,4), okay20(3,1), okay21(3,1), okay22(3,1), okay23(3,1), okay24(3,4), okay25(3,1), okay26(3,1), okay27(3,1), okay28(3,1), okay29(3,4), okay30(3,1), okay31(4,1), okay32(4,1), okay33(4,1), okay34(3,1), okay35(3,1), okay36(3,1), okay37(4,1), okay38(4,1), okay39(4,1), okay40(3,1), okay41(3,1), okay42(3,1), p(3), q(1), t(4)}
"""
