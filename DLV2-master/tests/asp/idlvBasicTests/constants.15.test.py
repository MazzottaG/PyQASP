input = """
%#const "pippo" = pippo.
p("pippo").

%#const X = 3.
p(X). 
a(X) :- p(X).

%#const c = C.
f(c).
g(X) :- f(X).

"""
output = """
"""
