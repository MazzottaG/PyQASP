input = """
%#const pippo = "pippo".
p(pippo).
a(X) :- p(X).
"""
output = """
{a(pippo), p(pippo)}
"""
