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

serr1(X,Z) :- 2 <= #min{V : a(V), b(V,X)}, 1 > #min{U: e(U), f(X,U)}.

serr2(X,Y) :- p(X,Y), 1 <= #min{V : a(V), b(X,Z)} <= 5, #min{U : a(Z)} > 3.

%serr3(X,Z) :- #min{V: e(V), f(Y,Z)} > 3, 4 <= #min{V : c(V), d(Y)}, p(X,Z).

%--- non-safe occurrences: non propositional guards var op #min{...} (only) ---

serr4(X,Y) :- #min{V : a(V)} = X.              
               
serr5(X,Y) :- #min{V : a(V)} < X.              

serr6(X,Y) :- #min{V : a(V)} <= X.             

serr7(X,Y) :- #min{V : a(V)} > X.              

serr8(X,Y) :- #min{V : a(V)} >= X.

:- #min{V : a(V)} = X.              
 
            
:- #min{V : a(V)} < X.              


:- #min{V : a(V)} <= X.             


:- #min{V : a(V)} > X.              


:- #min{V : a(V)} >= X.            


%--- non-safe occurrences: non propositional guards, #min{...} op var ----(at the end)

%serr9(Y,Z) :- p(Y,Z), q(Y), #min{V : a(V)} = X.           

serr10(Y,Z) :- p(Y,Z), q(Y), #min{V : a(V)} < X.           

serr11(Y,Z) :- p(Y,Z), q(Y), #min{V : a(V)} <= X.          

serr12(Y,Z) :- p(Y,Z), q(Y), #min{V : a(V)} > X.           

serr13(Y,Z) :- p(Y,Z), q(Y), #min{V : a(V)} >= X.          

:- p(Y,Z), q(Y), #min{V : a(V)} = X.           
           

:- p(Y,Z), q(Y), #min{V : a(V)} < X.           
 
   
:- p(Y,Z), q(Y), #min{V : a(V)} <= X.          
          

:- p(Y,Z), q(Y), #min{V : a(V)} > X.           
 

:- p(Y,Z), q(Y), #min{V : a(V)} >= X.


%---- non-safe occurrences: non propositional upr guards, #min{...} op var ----(at the beginning)

%serr14(Y,Z) :- #min{V : a(V)} =  X, p(Y,Z), q(Y).           

serr15(Y,Z) :- #min{V : a(V)} <  X, p(Y,Z), q(Y).           

serr16(Y,Z) :- #min{V : a(V)} <= X, p(Y,Z), q(Y).          

serr17(Y,Z) :- #min{V : a(V)} >  X, p(Y,Z), q(Y).           

serr18(Y,Z) :- #min{V : a(V)} >= X, p(Y,Z), q(Y).    

:- #min{V : a(V)} = X, p(Y,Z), q(Y).  
         

:- #min{V : a(V)} < X, p(Y,Z), q(Y).
           

:- #min{V : a(V)} <= X, p(Y,Z), q(Y). 
          

:- #min{V : a(V)} > X, q(Y).  
          

:- #min{V : a(V)} >= X, q(Y). 


%----non-safe occurrences: var < #min{...} < var ----

serr19(M,N) :- M < #min{V : a(V)} < N.         

serr20(M,Y) :- p(M,Y), q(M), M < #min{V : a(V)} < N.      

serr21(M,Y) :- p(M,Y), M < #min{V : a(V)} < N, q(M). 

serr22(M,Y) :- p(M,Y), q(M), M < #min{V : a(V)} < N.      

serr23(N,Y) :- M < #min{V : a(V)} < N, p(N,Y), q(N).

:- M < #min{V : a(V)} < N.         

:- q(M), M < #min{V : a(V)} < N.      

:- M < #min{V : a(V)} < N, q(M).   
  

         

      




:- q(M), M < #min{V : a(V)} < N.      

:- M < #min{V : a(V)} < N, q(N).   
  
      

  

%----non-safe occurrences: var < #min{...} <= var ----

serr24 :- M < #min{V : a(V)} <= N.        

serr25 :- q(N), M < #min{V : a(V)} <= N.     

serr27 :- M < #min{V : a(V)} <= N, q(N). 

serr28 :- q(M), M < #min{V : a(V)} <= N.     

serr29 :- M < #min{V : a(V)} <= N, q(N).

:- M < #min{V : a(V)} <= N.        

:- q(N), M < #min{V : a(V)} <= N.     

:- M < #min{V : a(V)} <= N, q(N).
    

        

     




:- q(M), M < #min{V : a(V)} <= N.     

:- M < #min{V : a(V)} <= N, q(N).
    
     




%----non-safe occurrences: var <= #max{...} < var ----

auch30(M,N) :- M <= #min{V : a(V)} < N.       

auch31(N,Y)  :- p(N,Y), q(N), M <= #min{V : a(V)} < N.     

auch31(Y,N)  :- M <= #min{V : a(V)} < N, q(N), p(N,Y).   

auch33(M,Y)  :- p(M,Y), q(M), M <= #min{V : a(V)} < N.     

auch34(Y,N)  :- M <= #min{V : a(V)} < N, q(N), p(N,Y).

:- M <= #min{V : a(V)} < N.       

:- q(N), M <= #min{V : a(V)} < N.     

:- M <= #min{V : a(V)} < N, q(N).  


       

     



:- q(M), M <= #min{V : a(V)} < N.     

:- M <= #min{V : a(V)} < N, q(N).  

     


   
%--- non-safe occurrences: both variables and integer as guards with non propositional atoms ---

serr35(X,Y) :- X <= #min{U : a(X), b(X,Y)}, 1 < #min {V : e(V), f(V,Y)}.

serr36(X,Y) :- p(X,Y), 1 <= #min{U : a(Z), b(X,Y)} <= 5, #min{V : a(Z)} < Z.

serr37(X,Y) :- #min{U : a(X), e(Z), f(Z,Y)} >= X, 3 <= #min{V : a(X), b(X,Y), c(X), d(X)}, p(X,Y).

serr38(X,Y) :- p(X,Y), Z <= #min{V : a(V), b(X,Z)} <= 5, #min{U : a(Z)} > X.
"""
output = """
"""
