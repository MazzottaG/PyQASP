input = """
%Non ground rules, constraints and weak constraints with the aggregate
%function min in the body. Guards are integer.  

a(1).
b(1).
p(1).
q(1).
c(1,2).
c(1,3).

%----  #min{nonGroundAtoms} op int ----(at the end)

okay01(M, N) :- p(M),q(N), #min{V : a(M),b(N),c(M,V)} = 2.           
  
okay02(M, N) :- p(M),q(N), #min{V : a(M),b(N),c(M,V)} < 3.           
 
okay03(M, N) :- p(M),q(N), #min{V : a(M),b(N),c(M,V)} <= 2.          

okay04(M, N) :- p(M),q(N), #min{V : a(M),b(N),c(M,V)} > 1.           
    
okay05(M, N) :- p(M),q(N), #min{V : a(M),b(N),c(M,V)} >= 2.          

:- p(M),q(N), #min{V : a(M),b(N),c(M,V)} = 3.
           

:- p(M),q(N), #min{V : a(M),b(N),c(M,V)} < 2.           
 
   
:- p(M),q(N), #min{V : a(M),b(N),c(M,V)} <= 1.          
          

:- p(M),q(N), #min{V : a(M),b(N),c(M,V)} > 2.           
 

:- p(M),q(N), #min{V : a(M),b(N),c(M,V)} >= 3.


%----  #min{nonGroundAtoms} op int ----(at the beginning)

okay06(M, N) :- #min{V : a(M),b(N),c(M,V)} = 2, p(M),q(N).           

okay07(M, N) :- #min{V : a(M),b(N),c(M,V)} < 3, p(M),q(N).           

okay08(M, N) :- #min{V : a(M),b(N),c(M,V)} <= 2, p(M),q(N).          

okay09(M, N) :- #min{V : a(M),b(N),c(M,V)} > 1, p(M),q(N).           

okay10(M, N) :- #min{V : a(M),b(N),c(M,V)} >= 2, p(M),q(N).    

:- #min{V : a(M),b(N),c(M,V)} = 1, p(M),q(N).  
         

:- #min{V : a(M),b(N),c(M,V)} < 2, p(M),q(N).
           

:- #min{V : a(M),b(N),c(M,V)} <= 1, p(M),q(N). 
          

:- #min{V : a(M),b(N),c(M,V)} > 2, p(M),q(N).  
          

:- #min{V : a(M),b(N),c(M,V)} >= 3, p(M),q(N). 
     

%----  #min{nonGroundAtoms} op int ----(in the middle)

okay11(M, N) :- p(M), #min{V : a(M),b(N),c(M,V)} = 2, q(N).           

okay12(M, N) :- p(M), #min{V : a(M),b(N),c(M,V)} < 3, q(N).           

okay13(M, N) :- p(M), #min{V : a(M),b(N),c(M,V)} <= 2, q(N).          

okay14(M, N) :- p(M), #min{V : a(M),b(N),c(M,V)} > 1, q(N).           

okay15(M, N) :- p(M),#min{V : a(M),b(N),c(M,V)} >= 2, q(N).    

:- p(M), #min{V : a(M),b(N),c(M,V)} = 1, q(N).  
         

:- p(M), #min{V : a(M),b(N),c(M,V)} < 2, q(N).
           

:- p(M), #min{V : a(M),b(N),c(M,V)} <= 1, q(N). 
          

:- p(M), #min{V : a(M),b(N),c(M,V)} > 2, q(N).  
          

:- p(M), #min{V : a(M),b(N),c(M,V)} >= 3, q(N). 

 
%----  int op #min{nonGroundAtoms}----(at the end) 

okay16(M, N) :- p(M),q(N), 2 = #min{V : a(M),b(N),c(M,V)}.           

okay17(M, N) :- p(M),q(N), 1 < #min{V : a(M),b(N),c(M,V)}.           

okay18(M, N) :- p(M),q(N), 2 <= #min{V : a(M),b(N),c(M,V)}.           

okay19(M, N) :- p(M),q(N), 3  > #min{V : a(M),b(N),c(M,V)}.           

okay20(M, N) :- p(M),q(N), 2 >= #min{V : a(M),b(N),c(M,V)}.     

:- p(M),q(N), 1 = #min{V : a(M),b(N),c(M,V)}.           
  
 
:- p(M),q(N), 2 < #min{V : a(M),b(N),c(M,V)}.           


:- p(M),q(N), 3 <= #min{V : a(M),b(N),c(M,V)}.           


:- p(M),q(N), 2 > #min{V : a(M),b(N),c(M,V)}.           
           

:- p(M),q(N), 1 >= #min{V : a(M),b(N),c(M,V)}.          
          

%----  int op #min{nonGroundAtoms}---- (at the beginning)

okay21(M, N) :- 2 = #min{V : a(M),b(N),c(M,V)}, p(M),q(N).          

okay22(M, N) :- 1 < #min{V : a(M),b(N),c(M,V)}, p(M),q(N).         

okay23(M, N) :- 2 <= #min{V : a(M),b(N),c(M,V)}, p(M),q(N).         

okay24(M, N) :- 3 > #min{V : a(M),b(N),c(M,V)}, p(M),q(N).          

okay25(M, N) :- 2 >= #min{V : a(M),b(N),c(M,V)}, p(M),q(N).         

:- 1 = #min{V : a(M),b(N),c(M,V)}, p(M),q(N).          
   

:- 2 < #min{V : a(M),b(N),c(M,V)}, p(M),q(N).         
         

:- 3 <= #min{V : a(M),b(N),c(M,V)}, p(M),q(N). 
         

:- 2 > #min{V : a(M),b(N),c(M,V)}, p(M),q(N).          
          

:- 1 >= #min{V : a(M),b(N),c(M,V)}, p(M),q(N).     


    
%----  int op #min{nonGroundAtoms}---- (in the middle)

okay26(M, N) :- p(M), 2 = #min{V : a(M),b(N),c(M,V)}, q(N).          

okay27(M, N) :- p(M), 1 < #min{V : a(M),b(N),c(M,V)}, q(N).         

okay28(M, N) :- p(M), 2 <= #min{V : a(M),b(N),c(M,V)}, q(N).         

okay29(M, N) :- p(M), 3 > #min{V : a(M),b(N),c(M,V)}, q(N).          

okay30(M, N) :- p(M), 2 >= #min{V : a(M),b(N),c(M,V)}, q(N).         

:- p(M), 1 = #min{V : a(M),b(N),c(M,V)}, q(N).          
   

:- p(M), 2 < #min{V : a(M),b(N),c(M,V)}, q(N).         
         

:- p(M), 3 <= #min{V : a(M),b(N),c(M,V)}, q(N). 
         

:- p(M), 2 > #min{V : a(M),b(N),c(M,V)}, q(N).          
          

:- p(M), 1 >= #min{V : a(M),b(N),c(M,V)}, q(N).     


%---- int < #min{nonGroundAtoms} < int ----


okay31(M,N) :- p(M),q(N), 1 < #min{V : a(M),b(N),c(M,V)} < 3.      

okay32(M,N) :- 1 < #min{V : a(M),b(N),c(M,V)} < 3, p(M),q(N).

okay33(M,N) :- p(M), 1 < #min{V : a(M),b(N),c(M,V)} < 3, q(N).   


:- p(M),q(N), 2 < #min{V : a(M),b(N),c(M,V)} < 3.      

:- 2 < #min{V : a(M),b(N),c(M,V)} < 3, p(M),q(N).   
  
:- p(M), 2 < #min{V : a(M),b(N),c(M,V)} < 3, q(N).


      



     

%---- int < #min{} <= int ----


okay34(M, N) :- p(M),q(N), 1 < #min{V : a(M),b(N),c(M,V)} <= 2. 

okay35(M, N) :- 1 < #min{V : a(M),b(N),c(M,V)} <= 2, p(M),q(N). 

okay36(M, N) :- p(M), 1 < #min{V : a(M),b(N),c(M,V)} <= 2, q(N).


:- p(M),q(N), 2 < #min{V : a(M),b(N),c(M,V)} <= 3.     

:- 2 < #min{V : a(M),b(N),c(M,V)} <= 3, p(M),q(N).

:- p(M), 2 < #min{V : a(M),b(N),c(M,V)} <= 3, q(N).  


     





%---- int <= #min{nonGroundAtoms} < int ----

okay37(M, N) :- p(M),q(N), 2 <= #min{V : a(M),b(N),c(M,V)} < 3.     

okay38(M, N) :- 2 <= #min{V : a(M),b(N),c(M,V)} < 3, p(M),q(N).     

okay39(M, N) :- p(M), 2 <= #min{V : a(M),b(N),c(M,V)} < 3, q(N).


:- p(M),q(N), 3 <= #min{V : a(M),b(N),c(M,V)} < 4.     

:- 3 <= #min{V : a(M),b(N),c(M,V)} < 4, p(M),q(N).  

:- p(M), 3 <= #min{V : a(M),b(N),c(M,V)} < 4, q(N).


     




   
%---- int <= #min{nonGroundAtoms} <= int ----


okay40(M, N) :- p(M),q(N), 2 <= #min{V : a(M),b(N),c(M,V)} <= 3.    

okay41(M, N) :- 2 <= #min{V : a(M),b(N),c(M,V)} <= 3, p(M),q(N).    

okay42(M, N) :- p(M), 2 <= #min{V : a(M),b(N),c(M,V)} <= 3, q(N).


:- p(M),q(N), 3 <= #min{V : a(M),b(N),c(M,V)} <= 4.    

:- 3 <= #min{V : a(M),b(N),c(M,V)} <= 4, p(M),q(N).

:- p(M), 3 <= #min{V : a(M),b(N),c(M,V)} <= 4, q(N).   



    



    

"""
output = """
{a(1), b(1), c(1,2), c(1,3), okay01(1,1), okay02(1,1), okay03(1,1), okay04(1,1), okay05(1,1), okay06(1,1), okay07(1,1), okay08(1,1), okay09(1,1), okay10(1,1), okay11(1,1), okay12(1,1), okay13(1,1), okay14(1,1), okay15(1,1), okay16(1,1), okay17(1,1), okay18(1,1), okay19(1,1), okay20(1,1), okay21(1,1), okay22(1,1), okay23(1,1), okay24(1,1), okay25(1,1), okay26(1,1), okay27(1,1), okay28(1,1), okay29(1,1), okay30(1,1), okay31(1,1), okay32(1,1), okay33(1,1), okay34(1,1), okay35(1,1), okay36(1,1), okay37(1,1), okay38(1,1), okay39(1,1), okay40(1,1), okay41(1,1), okay42(1,1), p(1), q(1)}
"""
