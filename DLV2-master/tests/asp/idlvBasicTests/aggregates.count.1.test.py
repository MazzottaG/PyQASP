input = """
% Different kind of aggregates with ground guards defined 
% in the body of rules, constraints and weak constraints.
% Atoms named "okay" have to be true in the computed stable models,
% atoms named "ouch" have to be false.

p.
d(1).
d(2).
d(3).


%---- #count{...} op integer ----(only)

okay1 :- #count{V : d(V)} = 3.              
 
ouch1 :- #count{V : d(V)} = 2.              
ouch1 :- #count{V : d(V)} = 4. 


okay2 :- #count{V : d(V)} < 4.              

ouch2 :- #count{V : d(V)} < 3.              

 
okay3 :- #count{V : d(V)} <= 3.             

ouch3 :- #count{V : d(V)} <= 2.             


okay4 :- #count{V : d(V)} > 2.              

ouch4 :- #count{V : d(V)} > 3.              


okay5 :- #count{V : d(V)} >= 3.             

ouch5 :- #count{V : d(V)} >= 4.             

%----  #count{...} op integer ----(at the end)

okay6 :- p, #count{V : d(V)} = 3.           

ouch6 :- p, #count{V : d(V)} = 2.
ouch6 :- p, #count{V : d(V)} = 4.           


okay7 :- p, #count{V : d(V)} < 4.           

ouch7 :- p, #count{V : d(V)} < 3.           


okay8 :- p, #count{V : d(V)} <= 3.          

ouch8 :- p, #count{V : d(V)} <= 2.          


okay9 :- p, #count{V : d(V)} > 2.           

ouch9 :- p, #count{V : d(V)} > 3.           


okay10 :- p, #count{V : d(V)} >= 3.          

ouch10 :- p, #count{V : d(V)} >= 4.          

%----  #count{...} op integer ----(at the beginning)

okay11 :- #count{V : d(V)} = 3, p.           

ouch11 :- #count{V : d(V)} = 2, p.           
ouch11 :- #count{V : d(V)} = 4, p.


okay12 :- #count{V : d(V)} < 4, p.           

ouch12 :- #count{V : d(V)} < 3, p.           


okay13 :- #count{V : d(V)} <= 3, p.          

ouch13 :- #count{V : d(V)} <= 2, p.          


okay14 :- #count{V : d(V)} > 2, p.           

ouch14 :- #count{V : d(V)} > 3, p.           


okay15 :- #count{V : d(V)} >= 3, p.          

ouch15 :- #count{V : d(V)} >= 4, p.          

%---- integer op #count{...} ----(only)

okay16 :- 3 = #count{V : d(V)}.              

ouch16 :- 2 = #count{V : d(V)}.              
ouch16 :- 4 = #count{V : d(V)}. 


okay17 :- 2 < #count{V : d(V)}.              

ouch17 :- 3 < #count{V : d(V)}.              


okay18 :- 3 <= #count{V : d(V)}.             

ouch18 :- 4 <= #count{V : d(V)}.             


okay19 :- 4 > #count{V : d(V)}.              

ouch19 :- 3 > #count{V : d(V)}.              


okay20 :- 3 >= #count{V : d(V)}.             

ouch20 :- 2 >= #count{V : d(V)}.             

%----  integer op #count{...}----(at the end) 

okay21 :- p, 3 = #count{V : d(V)}.           

ouch21 :- p, 2 = #count{V : d(V)}.           
ouch21 :- p, 4 = #count{V : d(V)}.


okay22 :- p, 2 < #count{V : d(V)}.           

ouch22 :- p, 3 < #count{V : d(V)}.           


okay23 :- p, 3 <= #count{V : d(V)}.           

ouch23 :- p, 4 <= #count{V : d(V)}.          


okay24 :- p, 4 > #count{V : d(V)}.           

ouch24 :- p, 3 > #count{V : d(V)}.           


okay25 :- p, 3 >= #count{V : d(V)}.          

ouch25 :- p, 2 >= #count{V : d(V)}.          

%----  integer op #count{...}---- (at the beginning)

okay26 :- 3 = #count{V : d(V)}, p.          

ouch26 :- 2 = #count{V : d(V)}, p.          
ouch26 :- 4 = #count{V : d(V)}, p. 


okay27 :- 2 < #count{V : d(V)}, p.         

ouch27 :- 3 < #count{V : d(V)}, p.          


okay28 :- 3 <= #count{V : d(V)}, p.         

ouch28 :- 4 <= #count{V : d(V)}, p.         


okay29 :- 4 > #count{V : d(V)}, p.          

ouch29 :- 3 > #count{V : d(V)}, p.          


okay30 :- 3 >= #count{V : d(V)}, p.         

ouch30 :- 2 >= #count{V : d(V)}, p.         
   
%---- integer < #count{...} < integer ----(only)

okay31 :- 2 < #count{V : d(V)} < 4.         

ouch31 :- 2 < #count{V : d(V)} < 3.          
ouch31 :- 3 < #count{V : d(V)} < 4.  

%---- integer < #count{...} < integer ----(at the end)

okay32 :- p, 2 < #count{V : d(V)} < 4.      

ouch32 :- p, 2 < #count{V : d(V)} < 3.      
ouch32 :- p, 3 < #count{V : d(V)} < 4.      

%---- integer < #count{...} < integer ----(at the beginning)

okay33 :- 2 < #count{V : d(V)} < 4, p.     

ouch33 :- 2 < #count{V : d(V)} < 3, p.      
ouch33 :- 3 < #count{V : d(V)} < 4, p.

%---- integer < #count{...} <= integer ----(only)

okay34 :- 2 < #count{V : d(V)} <= 3.        

ouch34 :- 3 < #count{V : d(V)} <= 4.        

%---- integer < #count{...} <= integer ----(at the end)

okay35 :- p, 2 < #count{V : d(V)} <= 3.     

ouch35 :- p, 3 < #count{V : d(V)} <= 4.     

%---- integer < #count{...} <= integer ----(at the beginning)

okay36 :- 2 < #count{V : d(V)} <= 3, p.     

ouch36 :- 3 < #count{V : d(V)} <= 4, p.     

%---- integer <= #count{...} < integer ----(at the end)

okay37 :- p, 3 <= #count{V : d(V)} < 4.     

ouch37 :- p, 2 <= #count{V : d(V)} < 3.     
ouch37 :- p, 4 <= #count{V : d(V)} < 5.    

%---- integer <= #count{...} < integer ----(at the beginning)

okay37 :- 3 <= #count{V : d(V)} < 4, p.     

ouch37 :- 2 <= #count{V : d(V)} < 3, p.  
ouch37 :- 4 <= #count{V : d(V)} < 5, p.     

%---- integer <= #count{...} <= integer ----(only)

okay38 :- 2 <= #count{V : d(V)} <= 3.      
okay39 :- 3 <= #count{V : d(V)} <= 4.  

ouch39 :- 1 <= #count{V : d(V)} <= 2.     
ouch39 :- 4 <= #count{V : d(V)} <= 5.       


okay40 :- 3 <= #count{V : d(V)} <= 3.       

ouch40 :- 2 <= #count{V : d(V)} <= 2.   
ouch40 :- 4 <= #count{V : d(V)} <= 4.       

%---- integer <= #count{...} <= integer ----(at the end)

okay41 :- p, 2 <= #count{V : d(V)} <= 3.    
okay41 :- p, 3 <= #count{V : d(V)} <= 4.  

ouch42 :- p, 1 <= #count{V : d(V)} <= 2.    
ouch42 :- p, 4 <= #count{V : d(V)} <= 5.    


okay43 :- p, 3 <= #count{V : d(V)} <= 3.    

ouch43 :- p, 2 <= #count{V : d(V)} <= 2.   
ouch43 :- p, 4 <= #count{V : d(V)} <= 4.   

%---- integer <= #count{...} <= integer ----(at the beginning)

okay44 :- 2 <= #count{V : d(V)} <= 3, p.    
okay45 :- 3 <= #count{V : d(V)} <= 4, p.    

ouch45 :- 1 <= #count{V : d(V)} <= 2, p.
ouch45 :- 4 <= #count{V : d(V)} <= 5, p.    


okay46 :- 3 <= #count{V : d(V)} <= 3, p.    

ouch46 :- 2 <= #count{V : d(V)} <= 2, p.    
ouch46 :- 4 <= #count{V : d(V)} <= 4, p.    

%---count in constraints----------------------------

:- #count{V : d(V)} = 2.
:- #count{V : d(V)} < 3.
:- 4 <= #count{V : d(V)}.
:- #count{V : d(V)} > 3.
:- #count{V : d(V)} >= 4.

:- p, #count{V : d(V)} = 2.
:- p, #count{V : d(V)} < 3.
:- p, 4 <= #count{V : d(V)}.
:- p, #count{V : d(V)} > 3.
:- p, #count{V : d(V)} >= 4.

:- #count{V : d(V)} = 2, p.
:- #count{V : d(V)} < 3, p.
:- 4 <= #count{V : d(V)}, p.
:- #count{V : d(V)} > 3, p.
:- #count{V : d(V)} >= 4, p.

:- 2 = #count{V : d(V)}.
:- 3 < #count{V : d(V)}.
:- 4 <= #count{V : d(V)}.
:- 3 > #count{V : d(V)}. 
:- 2 >= #count{V : d(V)}.

:- p, 2 = #count{V : d(V)}.
:- p, 3 < #count{V : d(V)}.
:- p, 4 <= #count{V : d(V)}.
:- p, 3 > #count{V : d(V)}. 
:- p, 2 >= #count{V : d(V)}.

:- 2 = #count{V : d(V)}, p.
:- 3 < #count{V : d(V)}, p.
:- 4 <= #count{V : d(V)}, p.
:- 3 > #count{V : d(V)}, p. 
:- 2 >= #count{V : d(V)}, p.


:- 1 <= #count{V : d(V)} <= 2, p.
:- p, 2 <= #count{V : d(V)} <= 2.
:- 2 <= #count{V : d(V)} <= 2.

:- 1 < #count{V : d(V)} < 2, p.
:- p, 1 < #count{V : d(V)} < 2.
:- 1 < #count{V : d(V)} < 2.

:- 1 < #count{V : d(V)} <= 2, p.
:- p, 1 < #count{V : d(V)} <= 2.
:- 1 < #count{V : d(V)} <= 2.

:- 1 <= #count{V : d(V)} < 2, p.
:- p, 1 <= #count{V : d(V)} < 2.
:- 1 <= #count{V : d(V)} < 2.
"""
output = """
{d(1), d(2), d(3), okay1, okay10, okay11, okay12, okay13, okay14, okay15, okay16, okay17, okay18, okay19, okay2, okay20, okay21, okay22, okay23, okay24, okay25, okay26, okay27, okay28, okay29, okay3, okay30, okay31, okay32, okay33, okay34, okay35, okay36, okay37, okay38, okay39, okay4, okay40, okay41, okay43, okay44, okay45, okay46, okay5, okay6, okay7, okay8, okay9, p}
"""
