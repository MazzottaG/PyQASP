input = """
% Different kind of aggregates with ground guards defined 
% in the body of rules, constraints and weak constraints.
% Atoms named "okay" have to be true in the computed stable models,
% atoms named "ouch" have to be false.

p.
d(1).
d(2).
d(3).


%---- #sum{...} op integer ----(only)

okay1 :- #sum{V : d(V)} = 6.              
 
ouch1 :- #sum{V : d(V)} = 4.              
ouch1 :- #sum{V : d(V)} = 5. 


okay2 :- #sum{V : d(V)} < 7.              

ouch2 :- #sum{V : d(V)} < 6.              

 
okay3 :- #sum{V : d(V)} <= 6.             

ouch3 :- #sum{V : d(V)} <= 5.             


okay4 :- #sum{V : d(V)} > 5.              

ouch4 :- #sum{V : d(V)} > 6.              


okay5 :- #sum{V : d(V)} >= 6.             

ouch5 :- #sum{V : d(V)} >= 7.             

%----  #sum{...} op integer ----(at the end)

okay6 :- p, #sum{V : d(V)} = 6.           

ouch6 :- p, #sum{V : d(V)} = 5.
ouch6 :- p, #sum{V : d(V)} = 7.           


okay7 :- p, #sum{V : d(V)} < 7.           

ouch7 :- p, #sum{V : d(V)} < 6.           


okay8 :- p, #sum{V : d(V)} <= 6.          

ouch8 :- p, #sum{V : d(V)} <= 5.          


okay9 :- p, #sum{V : d(V)} > 5.           

ouch9 :- p, #sum{V : d(V)} > 6.           


okay10 :- p, #sum{V : d(V)} >= 6.          

ouch10 :- p, #sum{V : d(V)} >= 7.          

%----  #sum{...} op integer ----(at the beginning)

okay11 :- #sum{V : d(V)} = 6, p.           

ouch11 :- #sum{V : d(V)} = 5, p.           
ouch11 :- #sum{V : d(V)} = 7, p.


okay12 :- #sum{V : d(V)} < 7, p.           

ouch12 :- #sum{V : d(V)} < 6, p.           


okay13 :- #sum{V : d(V)} <= 6, p.          

ouch13 :- #sum{V : d(V)} <= 5, p.          


okay14 :- #sum{V : d(V)} > 5, p.           

ouch14 :- #sum{V : d(V)} > 6, p.           


okay15 :- #sum{V : d(V)} >= 6, p.          

ouch15 :- #sum{V : d(V)} >= 7, p.          

%---- integer op #sum{...} ----(only)

okay16 :- 6 = #sum{V : d(V)}.              

ouch16 :- 5 = #sum{V : d(V)}.              
ouch16 :- 7 = #sum{V : d(V)}. 


okay17 :- 5 < #sum{V : d(V)}.              

ouch17 :- 6 < #sum{V : d(V)}.              


okay18 :- 6 <= #sum{V : d(V)}.             

ouch18 :- 7 <= #sum{V : d(V)}.             


okay19 :- 7 > #sum{V : d(V)}.              

ouch19 :- 6 > #sum{V : d(V)}.              


okay20 :- 6 >= #sum{V : d(V)}.             

ouch20 :- 5 >= #sum{V : d(V)}.             

%----  integer op #sum{...}----(at the end) 

okay21 :- p, 6 = #sum{V : d(V)}.           

ouch21 :- p, 5 = #sum{V : d(V)}.           
ouch21 :- p, 7 = #sum{V : d(V)}.


okay22 :- p, 5 < #sum{V : d(V)}.           

ouch22 :- p, 6 < #sum{V : d(V)}.           


okay23 :- p, 6 <= #sum{V : d(V)}.           

ouch23 :- p, 7 <= #sum{V : d(V)}.          


okay24 :- p, 7 > #sum{V : d(V)}.           

ouch24 :- p, 6 > #sum{V : d(V)}.           


okay25 :- p, 6 >= #sum{V : d(V)}.          

ouch25 :- p, 5 >= #sum{V : d(V)}.          

%----  integer op #sum{...}---- (at the beginning)

okay26 :- 6 = #sum{V : d(V)}, p.          

ouch26 :- 5 = #sum{V : d(V)}, p.          
ouch26 :- 7 = #sum{V : d(V)}, p. 


okay27 :- 5 < #sum{V : d(V)}, p.         

ouch27 :- 6 < #sum{V : d(V)}, p.          


okay28 :- 6 <= #sum{V : d(V)}, p.         

ouch28 :- 7 <= #sum{V : d(V)}, p.         


okay29 :- 7 > #sum{V : d(V)}, p.          

ouch29 :- 6 > #sum{V : d(V)}, p.          


okay30 :- 6 >= #sum{V : d(V)}, p.         

ouch30 :- 5 >= #sum{V : d(V)}, p.         
   
%---- integer < #sum{...} < integer ----(only)

okay31 :- 5 < #sum{V : d(V)} < 7.         

ouch31 :- 5 < #sum{V : d(V)} < 6.          
ouch31 :- 6 < #sum{V : d(V)} < 7.  

%---- integer < #sum{...} < integer ----(at the end)

okay32 :- p, 5 < #sum{V : d(V)} < 7.      

ouch32 :- p, 5 < #sum{V : d(V)} < 6.      
ouch32 :- p, 6 < #sum{V : d(V)} < 7.      

%---- integer < #sum{...} < integer ----(at the beginning)

okay33 :- 5 < #sum{V : d(V)} < 7, p.     

ouch33 :- 5 < #sum{V : d(V)} < 6, p.      
ouch33 :- 6 < #sum{V : d(V)} < 7, p.

%---- integer < #sum{...} <= integer ----(only)

okay34 :- 5 < #sum{V : d(V)} <= 6.        

ouch34 :- 6 < #sum{V : d(V)} <= 7.        

%---- integer < #sum{...} <= integer ----(at the end)

okay35 :- p, 5 < #sum{V : d(V)} <= 6.     

ouch35 :- p, 6 < #sum{V : d(V)} <= 7.     

%---- integer < #sum{...} <= integer ----(at the beginning)

okay36 :- 5 < #sum{V : d(V)} <= 6, p.     

ouch36 :- 6 < #sum{V : d(V)} <= 7, p.     

%---- integer <= #sum{...} < integer ----(at the end)

okay37 :- p, 6 <= #sum{V : d(V)} < 7.     

ouch37 :- p, 5 <= #sum{V : d(V)} < 6.     
ouch37 :- p, 7 <= #sum{V : d(V)} < 8.    

%---- integer <= #sum{...} < integer ----(at the beginning)

okay37 :- 6 <= #sum{V : d(V)} < 7, p.     

ouch37 :- 5 <= #sum{V : d(V)} < 6, p.  
ouch37 :- 7 <= #sum{V : d(V)} < 8, p.     

%---- integer <= #sum{...} <= integer ----(only)

okay38 :- 5 <= #sum{V : d(V)} <= 6.      
okay39 :- 6 <= #sum{V : d(V)} <= 7.  

ouch39 :- 4 <= #sum{V : d(V)} <= 5.     
ouch39 :- 7 <= #sum{V : d(V)} <= 8.       


okay40 :- 6 <= #sum{V : d(V)} <= 6.       

ouch40 :- 5 <= #sum{V : d(V)} <= 5.   
ouch40 :- 7 <= #sum{V : d(V)} <= 7.       

%---- integer <= #sum{...} <= integer ----(at the end)

okay41 :- p, 5 <= #sum{V : d(V)} <= 6.    
okay42:- p, 6 <= #sum{V : d(V)} <= 7.  

ouch42 :- p, 4 <= #sum{V : d(V)} <= 5.    
ouch42 :- p, 7 <= #sum{V : d(V)} <= 8.    


okay43 :- p, 6 <= #sum{V : d(V)} <= 6.    

ouch43 :- p, 5 <= #sum{V : d(V)} <= 5.   
ouch43 :- p, 7 <= #sum{V : d(V)} <= 7.   

%---- integer <= #sum{...} <= integer ----(at the beginning)

okay44 :- 5 <= #sum{V : d(V)} <= 6, p.    
okay45 :- 6 <= #sum{V : d(V)} <= 7, p.    

ouch45 :- 4 <= #sum{V : d(V)} <= 5, p.
ouch45 :- 7 <= #sum{V : d(V)} <= 8, p.    


okay46 :- 6 <= #sum{V : d(V)} <= 6, p.    

ouch46 :- 4 <= #sum{V : d(V)} <= 5, p.    
ouch46 :- 7 <= #sum{V : d(V)} <= 8, p.    

%---sum in constraints----------------------------

:- #sum{V : d(V)} = 5.
:- #sum{V : d(V)} < 6.
:- 7 <= #sum{V : d(V)}.
:- #sum{V : d(V)} > 6.
:- #sum{V : d(V)} >= 7.

:- p, #sum{V : d(V)} = 5.
:- p, #sum{V : d(V)} < 5.
:- p, 7 <= #sum{V : d(V)}.
:- p, #sum{V : d(V)} > 6.
:- p, #sum{V : d(V)} >= 7.

:- #sum{V : d(V)} = 5, p.
:- #sum{V : d(V)} < 6, p.
:- 7 <= #sum{V : d(V)}, p.
:- #sum{V : d(V)} > 6, p.
:- #sum{V : d(V)} >= 7, p.

:- 5 = #sum{V : d(V)}.
:- 6 < #sum{V : d(V)}.
:- 7 <= #sum{V : d(V)}.
:- 6 > #sum{V : d(V)}. 
:- 5 >= #sum{V : d(V)}.

:- p, 5 = #sum{V : d(V)}.
:- p, 6 < #sum{V : d(V)}.
:- p, 7 <= #sum{V : d(V)}.
:- p, 6 > #sum{V : d(V)}. 
:- p, 5 >= #sum{V : d(V)}.

:- 5 = #sum{V : d(V)}, p.
:- 6 < #sum{V : d(V)}, p.
:- 7 <= #sum{V : d(V)}, p.
:- 6 > #sum{V : d(V)}, p. 
:- 5 >= #sum{V : d(V)}, p.


:- 4 <= #sum{V : d(V)} <= 5, p.
:- p, 4 <= #sum{V : d(V)} <= 5.
:- 4 <= #sum{V : d(V)} <= 5.

:- 4 < #sum{V : d(V)} < 5, p.
:- p, 4 < #sum{V : d(V)} < 5.
:- 4 < #sum{V : d(V)} < 5.

:- 4 < #sum{V : d(V)} <= 5, p.
:- p, 4 < #sum{V : d(V)} <= 5.
:- 4 < #sum{V : d(V)} <= 5.

:- 4 <= #sum{V : d(V)} < 5, p.
:- p, 4 <= #sum{V : d(V)} < 5.
:- 4 <= #sum{V : d(V)} < 5.

%---sum in weak constraints-------------------------






















 





 





 


















"""
output = """
{d(1), d(2), d(3), okay1, okay10, okay11, okay12, okay13, okay14, okay15, okay16, okay17, okay18, okay19, okay2, okay20, okay21, okay22, okay23, okay24, okay25, okay26, okay27, okay28, okay29, okay3, okay30, okay31, okay32, okay33, okay34, okay35, okay36, okay37, okay38, okay39, okay4, okay40, okay41, okay42, okay43, okay44, okay45, okay46, okay5, okay6, okay7, okay8, okay9, p}
"""
