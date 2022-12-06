input = """
%#const pippo = pippo.
%#const pippo = 2. 
p(pippo).
a(X) :- p(X).
"""
output = """
{a(pippo), p(pippo)}
"""
