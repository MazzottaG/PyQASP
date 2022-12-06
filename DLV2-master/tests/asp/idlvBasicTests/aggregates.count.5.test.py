input = """
%Non-safe rules with the aggregate count.

a(1).
b(1).
c(1,1).
c(1,2).
p(1).
q(1).

%----  non-safe atoms in #count{nonGroundAtoms} op int ----
non_safe01(M, N) :- #count{V:a(M),b(N),c(M,V)} = 2.           
  
non_safe02(M, N) :- #count{V:a(M),b(N),c(M,V)} < 3.           
 
non_safe03(M, N) :- #count{V:a(M),b(N),c(M,V)} <= 2.          

non_safe04(M, N) :- #count{V:a(M),b(N),c(M,V)} > 1.           
    
non_safe05(M, N) :- #count{V:a(M),b(N),c(M,V)} >= 2.          

:- #count{V:a(M),b(N),c(M,V)} = 3.

:- #count{V:a(M),b(N),c(M,V)} < 2.           
  
   
:- #count{V:a(M),b(N),c(M,V)} <= 1.          
           

:- #count{V:a(M),b(N),c(M,V)} > 2.           
  

:- #count{V:a(M),b(N),c(M,V)} >= 3.
 

%----  non-safe atoms in #count{nonGroundAtoms} op int ----(at the end)

non_safe06(M, N) :- p(M), #count{V:a(M),b(N),c(M,V)} = 2.           
  
non_safe07(M, N) :- p(M), #count{V:a(M),b(N),c(M,V)} < 3.           
 
non_safe08(M, N) :- p(M), #count{V:a(M),b(N),c(M,V)} <= 2.          

non_safe09(M, N) :- p(M), #count{V:a(M),b(N),c(M,V)} > 1.           
    
non_safe10(M, N) :- p(M), #count{V:a(M),b(N),c(M,V)} >= 2.          

:- p(M), #count{V:a(M),b(N),c(M,V)} = 3.
            

:- p(M), #count{V:a(M),b(N),c(M,V)} < 2.           
  
   
:- p(M), #count{V:a(M),b(N),c(M,V)} <= 1.          
           

:- p(M), #count{V:a(M),b(N),c(M,V)} > 2.           
  

:- p(M), #count{V:a(M),b(N),c(M,V)} >= 3.
 

%----   non-safe #count{nonGroundAtoms} op int ----(at the beginning)

non_safe11(M, N) :- #count{V:a(M),b(N),c(M,V)} = 2, p(M).           

non_safe12(M, N) :- #count{V:a(M),b(N),c(M,V)} < 3, p(M).           

non_safe13(M, N) :- #count{V:a(M),b(N),c(M,V)} <= 2, p(M).          

non_safe14(M, N) :- #count{V:a(M),b(N),c(M,V)} > 1, p(M).           

non_safe15(M, N) :- #count{V:a(M),b(N),c(M,V)} >= 2, p(M).    

:- #count{V:a(M),b(N),c(M,V)} = 3, p(M).  
          

:- #count{V:a(M),b(N),c(M,V)} < 1, p(M).
            

:- #count{V:a(M),b(N),c(M,V)} <= 1, p(M). 
           

:- #count{V:a(M),b(N),c(M,V)} > 2, p(M). 
           

:- #count{V:a(M),b(N),c(M,V)} >= 3, p(M). 
      


%----  non-safe int op #count{nonGroundAtoms}----

non_safe16(M, N) :- 2 = #count{V:a(M),b(N),c(M,V)}.           

non_safe17(M, N) :- 1 < #count{V:a(M),b(N),c(M,V)}.           

non_safe18(M, N) :- 1 <= #count{V:a(M),b(N),c(M,V)}.           

non_safe19(M, N) :- 3  > #count{V:a(M),b(N),c(M,V)}.           

non_safe20(M, N) :- 3 >= #count{V:a(M),b(N),c(M,V)}.     

:- 1 = #count{V:a(M),b(N),c(M,V)}.           
   
 
:- 3 < #count{V:a(M),b(N),c(M,V)}.           
 

:- 3 <= #count{V:a(M),b(N),c(M,V)}.           
 

:- 1 > #count{V:a(M),b(N),c(M,V)}.           
            

:- 1 >= #count{V:a(M),b(N),c(M,V)}.          
      
 
%----  non-safe int op #count{nonGroundAtoms}----(at the end) 

non_safe21(M, N) :- q(N), 2 = #count{V:a(M),b(N),c(M,V)}.           

non_safe22(M, N) :- q(N), 1 < #count{V:a(M),b(N),c(M,V)}.           

non_safe23(M, N) :- q(N), 1 <= #count{V:a(M),b(N),c(M,V)}.           

non_safe24(M, N) :- q(N), 3  > #count{V:a(M),b(N),c(M,V)}.           

non_safe25(M, N) :- q(N), 3 >= #count{V:a(M),b(N),c(M,V)}.     

:- q(N), 1 = #count{V:a(M),b(N),c(M,V)}.           
   
 
:- q(N), 3 < #count{V:a(M),b(N),c(M,V)}.           
 

:- q(N), 3 <= #count{V:a(M),b(N),c(M,V)}.           
 

:- q(N), 1 > #count{V:a(M),b(N),c(M,V)}.           
            

:- q(N), 1 >= #count{V:a(M),b(N),c(M,V)}.          
           

%----  non-safe int op #count{nonGroundAtoms}---- (at the beginning)

non_safe26(M, N) :- 2 = #count{V:a(M),b(N),c(M,V)}, q(N).        

non_safe27(M, N) :- 1 < #count{V:a(M),b(N),c(M,V)}, q(N).         

non_safe28(M, N) :- 1 <= #count{V:a(M),b(N),c(M,V)}, q(N).         

non_safe29(M, N) :- 3 > #count{V:a(M),b(N),c(M,V)}, q(N).          

non_safe30(M, N) :- 2 >= #count{V:a(M),b(N),c(M,V)}, q(N).         

:- 1 = #count{V:a(M),b(N),c(M,V)}, q(N).          
    

:- 3 < #count{V:a(M),b(N),c(M,V)}, q(N).         
          

:- 3 <= #count{V:a(M),b(N),c(M,V)}, q(N). 
          

:- 1 > #count{V:a(M),b(N),c(M,V)}, q(N).          
           

:- 1 >= #count{V:a(M),b(N),c(M,V)}, q(N).     
 


%---- non-safe int < #count{nonGroundAtoms} < int ----


non_safe31(M,N) :- 1 < #count{V:a(M),b(N),c(M,V)} < 3.      

non_safe32(M,N) :- 1< #count{V:a(M),b(N),c(M,V)} < 3, q(N).   

non_safe33(M,N) :- p(N), 1< #count{V:a(M),b(N),c(M,V)} < 3.   


:- 2 < #count{V:a(M),b(N),c(M,V)} < 3.   
  
:- 2 < #count{V:a(M),b(N),c(M,V)} < 3, q(N).


 

      


%---- non-safe int < #count{} <= int ----


non_safe34(M, N) :- 1 < #count{V:a(M),b(N),c(M,V)} <= 2.     

non_safe35(M, N) :- q(N), 1 < #count{V:a(M),b(N),c(M,V)} <= 2. 

non_safe36(M, N) :- 1 < #count{V:a(M),b(N),c(M,V)} <= 2, p(M).


:- q(N), 2 < #count{V:a(M),b(N),c(M,V)} <= 3.

:- 2 < #count{V:a(M),b(N),c(M,V)} <= 3, p(M).  


 

 


%---- non-safe int <= #count{nonGroundAtoms} < int ----

non_safe37(M, N) :- 2 <= #count{V:a(M),b(N),c(M,V)} < 3.    

non_safe38(M, N) :- q(N), 2 <= #count{V:a(M),b(N),c(M,V)} < 3.     

non_safe39(M, N) :- 2 <= #count{V:a(M),b(N),c(M,V)} < 3, q(N).


:- 3 <= #count{V:a(M),b(N),c(M,V)} < 4.     

:- p(M), 3 <= #count{V:a(M),b(N),c(M,V)} < 4.  

:- 3 <= #count{V:a(M),b(N),c(M,V)} < 4, q(N).


      

 

 
   
%---- non-safe int <= #count{nonGroundAtoms} <= int ----


non_safe40(M, N) :- 2 <= #count{V:a(M),b(N),c(M,V)} <= 3.    

non_safe41(M, N) :- p(M), 2 <= #count{V:a(M),b(N),c(M,V)} <= 3.    

non_safe42(M, N) :- 2 <= #count{V:a(M),b(N),c(M,V)} <= 3, q(N).


:- 3 <= #count{V:a(M),b(N),c(M,V)} <= 4.    

:- 3 <= #count{V:a(M),b(N),c(M,V)} <= 4, q(N).

:- p(M), 3 <= #count{V:a(M),b(N),c(M,V)} <= 4.   



    

 

    

"""
output = """
"""
