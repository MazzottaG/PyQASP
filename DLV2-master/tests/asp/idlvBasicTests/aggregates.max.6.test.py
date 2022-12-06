input = """
%Non ground rules, constraints and weak constraints with the aggregate
%function max in the body. Guards are non ground, too.

a(2).
a(3).
b(1).
c(3,1).
c(3,2).
c(2,1).
c(2,2).
p(2).
q(1).
v(3).
t(3).    

%----  #max{nonGroundAtoms} op var ----(at the end)

okay01(M, N) :- p(M),q(N), #max{V:a(M),b(N),c(M,V)} = M.           
  
okay02(M, N) :- p(M),t(N), #max{V:a(M),c(M,V)} < N.           
 
okay03(M, N) :- p(M),q(N), #max{V:a(M),b(N),c(M,V)} <= M.          

okay04(M, N) :- p(M),q(N), #max{V:a(M),b(N),c(M,V)} > N.           
    
okay05(M, N) :- p(M),q(N), #max{V:a(M),b(N),c(M,V)} >= M.          

:- p(M),q(N), #max{V:a(M),b(N),c(M,V)} = N.
           

:- p(M),q(N), #max{V:a(M),b(N),c(M,V)} < N.           
 
   
:- p(M),q(N), #max{V:a(M),b(N),c(M,V)} <= N.          
          

:- p(M),q(N), #max{V:a(M),b(N),c(M,V)} > M.           
 

:- p(M),t(N), #max{V:a(M),b(N),c(V,M)} >= N.


%----  #max{nonGroundAtoms} op var ----(at the beginning)

okay06(M, N) :- #max{V:a(M),b(N),c(M,V)} = M, p(M),q(N).           

okay07(M, N) :- #max{V:a(M),c(M,V)} < N, p(M),t(N).           

okay08(M, N) :- #max{V:a(M),b(N),c(M,V)} <= M, p(M),q(N).          

okay09(M, N) :- #max{V:a(M),b(N),c(M,V)} > N, p(M),q(N).           

okay10(M, N) :- #max{V:a(M),b(N),c(M,V)} >= N, p(M),q(N).    

:- #max{V:a(M),b(N),c(M,V)} = N, p(M),q(N).  
         

:- #max{V:a(M),b(N),c(M,V)} < N, p(M),q(N).
           

:- #max{V:a(M),b(N),c(M,V)} <= N, p(M),q(N). 
          

:- #max{V:a(M),b(N),c(M,V)} > M, p(M),q(N).  
          

:- #max{V:a(M),b(N),c(V,M)} >= N, p(M),t(N). 
     

%----  #max{nonGroundAtoms} op var ----(in the middle)

okay11(M, N) :- p(M), #max{V:a(M),b(N),c(M,V)} = M, q(N).           

okay12(M, N) :- p(M), #max{V:a(M),c(M,V)} < N, t(N).           

okay13(M, N) :- p(M), #max{V:a(M),b(N),c(M,V)} <= M, q(N).          

okay14(M, N) :- p(M), #max{V:a(M),b(N),c(M,V)} > N, q(N).           

okay15(M, N) :- p(M),#max{V:a(M),b(N),c(M,V)} >= N, q(N).    

:- p(M), #max{V:a(M),b(N),c(M,V)} = N, q(N).  
         

:- p(M), #max{V:a(M),b(N),c(M,V)} < N, q(N).
           

:- p(M), #max{V:a(M),b(N),c(M,V)} <= N, q(N). 
          

:- p(M), #max{V:a(M),b(N),c(M,V)} > M, q(N).  
          

:- p(M), #max{V:a(M),b(N),c(V,M)} >= M, t(N). 

 
%----  var op #max{nonGroundAtoms}----(at the end) 

okay16(M, N) :- p(M),q(N), M = #max{V:a(M),b(N),c(M,V)}.           

okay17(M, N) :- p(M),q(N), N < #max{V:a(M),b(N),c(M,V)}.           

okay18(M, N) :- p(M),q(N), M <= #max{V:a(M),b(N),c(M,V)}.           

okay19(M, N) :- p(M),t(N), N  > #max{V:a(M),c(M,V)}.           

okay20(M, N) :- p(M),q(N), M >= #max{V:a(M),b(N),c(M,V)}.     

:- p(M),q(N), N = #max{V:a(M),b(N),c(M,V)}.           
  
 
:- p(M),q(N), M < #max{V:a(M),b(N),c(M,V)}.           


:- p(M),t(N), M <= #max{V:a(M),b(N),c(V,M)}.           


:- p(M),q(N), N > #max{V:a(M),b(N),c(M,V)}.           
           

:- p(M),q(N), N >= #max{V:a(M),b(N),c(M,V)}.          
          

%----  var op #max{nonGroundAtoms}---- (at the beginning)

okay21(M, N) :- M = #max{V:a(M),b(N),c(M,V)}, p(M),q(N).          

okay22(M, N) :- N < #max{V:a(M),b(N),c(M,V)}, p(M),q(N).         

okay23(M, N) :- M <= #max{V:a(M),b(N),c(M,V)}, p(M),q(N).         

okay24(M, N) :- N > #max{V:a(M),c(M,V)}, p(M),t(N).          

okay25(M, N) :- M >= #max{V:a(M),b(N),c(M,V)}, p(M),q(N).         

:- N = #max{V:a(M),b(N),c(M,V)}, p(M),q(N).          
   

:- M < #max{V:a(M),b(N),c(M,V)}, p(M),q(N).         
         

:- N <= #max{V:a(M),b(N),c(V,M)}, p(M),t(N). 
         

:- N > #max{V:a(M),b(N),c(M,V)}, p(M),q(N).          
          

:- N >= #max{V:a(M),b(N),c(M,V)}, p(M),q(N).     


    
%----  var op #max{nonGroundAtoms}---- (in the middle)

okay26(M, N) :- p(M), M = #max{V:a(M),b(N),c(M,V)}, q(N).          

okay27(M, N) :- p(M), N < #max{V:a(M),b(N),c(M,V)}, q(N).         

okay28(M, N) :- p(M), M <= #max{V:a(M),b(N),c(M,V)}, q(N).         

okay29(M, N) :- p(M), N > #max{V:a(M),c(M,V)}, t(N).          

okay30(M, N) :- p(M), M >= #max{V:a(M),b(N),c(M,V)}, q(N).         

:- p(M), N = #max{V:a(M),b(N),c(M,V)}, q(N).          
   

:- p(M), M < #max{V:a(M),b(N),c(M,V)}, q(N).         
         

:- p(M), N <= #max{V:a(M),b(N),c(V,M)}, t(N). 
         

:- p(M), N >#max{V:a(M),b(N),c(M,V)}, q(N).          
          

:- p(M), N >= #max{V:a(M),b(N),c(M,V)}, q(N).     


%---- var < #max{nonGroundAtoms} < var ----


okay31(M,N) :- t(M),q(N), N < #max{V:a(M),b(N),c(M,V)} < M.    

okay32(M,N) :- N < #max{V:a(M),b(N),c(M,V)} < M, t(M),q(N).

okay33(M,N) :- t(M), N < #max{V:a(M),b(N),c(M,V)} < M, q(N).   


:- p(M),q(N), N < #max{V:a(M),b(N),c(M,V)} < M.      

:- N < #max{V:a(M),b(N),c(M,V)} < M, p(M),q(N).   
  
:- p(M), N < #max{V:a(M),b(N),c(M,V)} < M, q(N).


      



     

%---- var < #max{} <= var ----


okay34(M, N) :- p(M),q(N), N < #max{V:a(M),b(N),c(M,V)} <= M.     

okay35(M, N) :- N < #max{V:a(M),b(N),c(M,V)} <= M, p(M),q(N). 

okay36(M, N) :- p(M), N < #max{V:a(M),b(N),c(M,V)} <= M, q(N).


:- p(M),q(N), N < #max{V:a(M),b(N),c(V,M)} <= M.     

:- N < #max{V:a(M),b(N),c(V,M)} <= M, p(M),q(N).

:- p(M), N < #max{V:a(M),b(N),c(V,M)} <= M, q(N).  


     





%---- var <= #max{nonGroundAtoms} < var ----

okay37(M, N) :- t(M),q(N), N <= #max{V:a(M),b(N),c(M,V)} < M.     

okay38(M, N) :- N <= #max{V:a(M),b(N),c(M,V)} < M, t(M),q(N).     

okay39(M, N) :- t(M), N <= #max{V:a(M),b(N),c(M,V)} < M, q(N).


:- p(M),q(N), N <= #max{V:a(M),b(N),c(M,V)} < M.     

:- N <= #max{V:a(M),b(N),c(M,V)} < M, p(M),q(N).  

:- p(M), N <= #max{V:a(M),b(N),c(M,V)} < M, q(N).


     




   
%---- var <= #max{nonGroundAtoms} <= var ----


okay40(M, N) :- p(M),q(N), N <= #max{V:a(M),b(N),c(M,V)} <= M.    

okay41(M, N) :- M <= #max{V:a(M),b(N),c(M,V)} <= M, p(M),q(N).    

okay42(M, N) :- p(M), M <= #max{V:a(M),b(N),c(M,V)} <= M, q(N).


:- v(M),t(N), M <= #max{V:a(M),b(N),c(M,V)} <= N.  

:- M <= #max{V:a(M),b(N),c(M,V)} <= N, v(M),t(N).

:- v(M), M <= #max{V:a(M),b(N),c(M,V)} <= N, t(N).   



    



    




"""
output = """
{a(2), a(3), b(1), c(2,1), c(2,2), c(3,1), c(3,2), okay01(2,1), okay02(2,3), okay03(2,1), okay04(2,1), okay05(2,1), okay06(2,1), okay07(2,3), okay08(2,1), okay09(2,1), okay10(2,1), okay11(2,1), okay12(2,3), okay13(2,1), okay14(2,1), okay15(2,1), okay16(2,1), okay17(2,1), okay18(2,1), okay19(2,3), okay20(2,1), okay21(2,1), okay22(2,1), okay23(2,1), okay24(2,3), okay25(2,1), okay26(2,1), okay27(2,1), okay28(2,1), okay29(2,3), okay30(2,1), okay31(3,1), okay32(3,1), okay33(3,1), okay34(2,1), okay35(2,1), okay36(2,1), okay37(3,1), okay38(3,1), okay39(3,1), okay40(2,1), okay41(2,1), okay42(2,1), p(2), q(1), t(3), v(3)}
"""
