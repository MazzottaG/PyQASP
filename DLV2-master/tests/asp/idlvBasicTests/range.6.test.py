input = """


%g(2..4,b).
%g(X,b) :- #int(X), 2 <= X, X <= 4.
%h(3..5, const, 2..8).
%h(X,b,Y) :- 3 <= X, X <= 5, %#int(X), 3 <= X, X <= 5,
%            2 <= Y, Y <= 8. %#int(Y), 2 <= Y, Y <= 8.

intersect1(X) :- g(X,_), h(X,_,_).
intersect2(X) :- g(X,_), h(_,_,X).
"""
output = """
{}
"""
