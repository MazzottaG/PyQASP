input = """
%#const a = "pippo".
p(a).
p(pippo).
a(X) :- p(X).
"""
output = """
{a(a), a(pippo), p(a), p(pippo)}
"""
