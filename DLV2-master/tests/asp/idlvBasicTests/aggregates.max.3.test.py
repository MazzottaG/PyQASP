input = """
%All the following rules are syntactically wrong.
%"serr" stands for s(yntax) err(or).  
a(1).
b(1,1).
b(1,2).
c(3).
d(3).
e(1).
f(1,1).
p(1,1).
p(1,2).
q(1).
q(2).

%--- multiple non-safe occurrences: non propositional Conj ---

serr1(X,Z) :- 2 <= #max{V : a(V), b(V,X)}, 1 > #max{U: e(U), f(X,U)}.

serr2(X,Y) :- p(X,Y), 1 <= #max{V : a(V), b(X,Z)} <= 5, #max{U : a(Z)} > 3.

%serr3(X,Z) :- #max{V: e(V), f(Y,Z)} > 3, 4 <= #max{V : c(V), d(Y)}, p(X,Z).

%--- non-safe occurrences: non propositional guards var op #max{...} (only) ---

serr4(X,Y) :- #max{V : a(V)} = X.              
               
serr5(X,Y) :- #max{V : a(V)} < X.              

serr6(X,Y) :- #max{V : a(V)} <= X.             

serr7(X,Y) :- #max{V : a(V)} > X.              

serr8(X,Y) :- #max{V : a(V)} >= X.

:- #max{V : a(V)} = X.              
 
            
:- #max{V : a(V)} < X.              


:- #max{V : a(V)} <= X.             


:- #max{V : a(V)} > X.              


:- #max{V : a(V)} >= X.            


%--- non-safe occurrences: non propositional guards, #max{...} op var ----(at the end)

%serr9(Y,Z) :- p(Y,Z), q(Y), #max{V : a(V)} = X.           

serr10(Y,Z) :- p(Y,Z), q(Y), #max{V : a(V)} < X.           

serr11(Y,Z) :- p(Y,Z), q(Y), #max{V : a(V)} <= X.          

serr12(Y,Z) :- p(Y,Z), q(Y), #max{V : a(V)} > X.           

serr13(Y,Z) :- p(Y,Z), q(Y), #max{V : a(V)} >= X.          

:- p(Y,Z), q(Y), #max{V : a(V)} = X.           
           

:- p(Y,Z), q(Y), #max{V : a(V)} < X.           
 
   
:- p(Y,Z), q(Y), #max{V : a(V)} <= X.          
          

:- p(Y,Z), q(Y), #max{V : a(V)} > X.           
 

:- p(Y,Z), q(Y), #max{V : a(V)} >= X.


%---- non-safe occurrences: non propositional upr guards, #max{...} op var ----(at the beginning)

%serr14(Y,Z) :- #max{V : a(V)} =  X, p(Y,Z), q(Y).           

serr15(Y,Z) :- #max{V : a(V)} <  X, p(Y,Z), q(Y).           

serr16(Y,Z) :- #max{V : a(V)} <= X, p(Y,Z), q(Y).          

serr17(Y,Z) :- #max{V : a(V)} >  X, p(Y,Z), q(Y).           

serr18(Y,Z) :- #max{V : a(V)} >= X, p(Y,Z), q(Y).    

:- #max{V : a(V)} = X, p(Y,Z), q(Y).  
         

:- #max{V : a(V)} < X, p(Y,Z), q(Y).
           

:- #max{V : a(V)} <= X, p(Y,Z), q(Y). 
          

:- #max{V : a(V)} > X, q(Y).  
          

:- #max{V : a(V)} >= X, q(Y). 


%----non-safe occurrences: var < #max{...} < var ----

serr19(M,N) :- M < #max{V : a(V)} < N.         

serr20(M,Y) :- p(M,Y), q(M), M < #max{V : a(V)} < N.      

serr21(M,Y) :- p(M,Y), M < #max{V : a(V)} < N, q(M). 

serr22(M,Y) :- p(M,Y), q(M), M < #max{V : a(V)} < N.      

serr23(N,Y) :- M < #max{V : a(V)} < N, p(N,Y), q(N).

:- M < #max{V : a(V)} < N.         

:- q(M), M < #max{V : a(V)} < N.      

:- M < #max{V : a(V)} < N, q(M).   
  

         

      




:- q(M), M < #max{V : a(V)} < N.      

:- M < #max{V : a(V)} < N, q(N).   
  
      

  

%----non-safe occurrences: var < #max{...} <= var ----

serr24 :- M < #max{V : a(V)} <= N.        

serr25 :- q(N), M < #max{V : a(V)} <= N.     

serr27 :- M < #max{V : a(V)} <= N, q(N). 

serr28 :- q(M), M < #max{V : a(V)} <= N.     

serr29 :- M < #max{V : a(V)} <= N, q(N).

:- M < #max{V : a(V)} <= N.        

:- q(N), M < #max{V : a(V)} <= N.     

:- M < #max{V : a(V)} <= N, q(N).
    

        

     




:- q(M), M < #max{V : a(V)} <= N.     

:- M < #max{V : a(V)} <= N, q(N).
    
     




%----non-safe occurrences: var <= #max{...} < var ----

auch30(M,N) :- M <= #max{V : a(V)} < N.       

auch31(N,Y)  :- p(N,Y), q(N), M <= #max{V : a(V)} < N.     

auch31(Y,N)  :- M <= #max{V : a(V)} < N, q(N), p(N,Y).   

auch33(M,Y)  :- p(M,Y), q(M), M <= #max{V : a(V)} < N.     

auch34(Y,N)  :- M <= #max{V : a(V)} < N, q(N), p(N,Y).

:- M <= #max{V : a(V)} < N.       

:- q(N), M <= #max{V : a(V)} < N.     

:- M <= #max{V : a(V)} < N, q(N).  


       

     



:- q(M), M <= #max{V : a(V)} < N.     

:- M <= #max{V : a(V)} < N, q(N).  

     


   
%--- non-safe occurrences: both variables and integer as guards with non propositional atoms ---

serr35(X,Y) :- X <= #max{U : a(X), b(X,Y)}, 1 < #max {V : e(V), f(V,Y)}.

serr36(X,Y) :- p(X,Y), 1 <= #max{U : a(Z), b(X,Y)} <= 5, #max{V : a(Z)} < Z.

serr37(X,Y) :- #max{U : a(X), e(Z), f(Z,Y)} >= X, 3 <= #max{V : a(X), b(X,Y), c(X), d(X)}, p(X,Y).

serr38(X,Y) :- p(X,Y), Z <= #max{V : a(V), b(X,Z)} <= 5, #max{U : a(Z)} > X.
"""
output = """
"""
