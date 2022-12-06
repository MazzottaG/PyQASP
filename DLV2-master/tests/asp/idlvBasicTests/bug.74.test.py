input = """
difference(a,b,0).
difference(c,d,2).
%:- difference(_,_,D).  [D:]

"""
output = """
{difference(a,b,0), difference(c,d,2)}
"""
