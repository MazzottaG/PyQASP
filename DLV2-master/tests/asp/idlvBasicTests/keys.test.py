input = """

 d :- a. 
 b :- c,d. 
 c :- b,e. 

 ok :- a, b, c, d, e, f. 
"""
output = """
{}
"""
