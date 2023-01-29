input = """
num(1..3).
a(f(X,Y,Z)) :- num(X),num(Y),num(Z).
b(f(X,Y,Z)) :- num(X),num(Y),num(Z).

ok1(X,Y) :- a(X), b(Y),X>Y.
%ok2(X,Y) :- a(X), b(Y),X<Y.
%ok3(X,Y) :- a(X), b(Y),X>=Y.
%ok3(X,Y) :- a(X), b(Y),X<=Y.
%ok4(X,Y) :- a(X), b(Y),X!=Y.
%ok5(X,Y) :- a(X), b(Y),X=Y.
"""
output = """
{a(f(1,1,1)), a(f(1,1,2)), a(f(1,1,3)), a(f(1,2,1)), a(f(1,2,2)), a(f(1,2,3)), a(f(1,3,1)), a(f(1,3,2)), a(f(1,3,3)), a(f(2,1,1)), a(f(2,1,2)), a(f(2,1,3)), a(f(2,2,1)), a(f(2,2,2)), a(f(2,2,3)), a(f(2,3,1)), a(f(2,3,2)), a(f(2,3,3)), a(f(3,1,1)), a(f(3,1,2)), a(f(3,1,3)), a(f(3,2,1)), a(f(3,2,2)), a(f(3,2,3)), a(f(3,3,1)), a(f(3,3,2)), a(f(3,3,3)), b(f(1,1,1)), b(f(1,1,2)), b(f(1,1,3)), b(f(1,2,1)), b(f(1,2,2)), b(f(1,2,3)), b(f(1,3,1)), b(f(1,3,2)), b(f(1,3,3)), b(f(2,1,1)), b(f(2,1,2)), b(f(2,1,3)), b(f(2,2,1)), b(f(2,2,2)), b(f(2,2,3)), b(f(2,3,1)), b(f(2,3,2)), b(f(2,3,3)), b(f(3,1,1)), b(f(3,1,2)), b(f(3,1,3)), b(f(3,2,1)), b(f(3,2,2)), b(f(3,2,3)), b(f(3,3,1)), b(f(3,3,2)), b(f(3,3,3)), num(1), num(2), num(3), ok1(f(1,1,2),f(1,1,1)), ok1(f(1,1,3),f(1,1,1)), ok1(f(1,1,3),f(1,1,2)), ok1(f(1,2,1),f(1,1,1)), ok1(f(1,2,1),f(1,1,2)), ok1(f(1,2,1),f(1,1,3)), ok1(f(1,2,2),f(1,1,1)), ok1(f(1,2,2),f(1,1,2)), ok1(f(1,2,2),f(1,1,3)), ok1(f(1,2,2),f(1,2,1)), ok1(f(1,2,3),f(1,1,1)), ok1(f(1,2,3),f(1,1,2)), ok1(f(1,2,3),f(1,1,3)), ok1(f(1,2,3),f(1,2,1)), ok1(f(1,2,3),f(1,2,2)), ok1(f(1,3,1),f(1,1,1)), ok1(f(1,3,1),f(1,1,2)), ok1(f(1,3,1),f(1,1,3)), ok1(f(1,3,1),f(1,2,1)), ok1(f(1,3,1),f(1,2,2)), ok1(f(1,3,1),f(1,2,3)), ok1(f(1,3,2),f(1,1,1)), ok1(f(1,3,2),f(1,1,2)), ok1(f(1,3,2),f(1,1,3)), ok1(f(1,3,2),f(1,2,1)), ok1(f(1,3,2),f(1,2,2)), ok1(f(1,3,2),f(1,2,3)), ok1(f(1,3,2),f(1,3,1)), ok1(f(1,3,3),f(1,1,1)), ok1(f(1,3,3),f(1,1,2)), ok1(f(1,3,3),f(1,1,3)), ok1(f(1,3,3),f(1,2,1)), ok1(f(1,3,3),f(1,2,2)), ok1(f(1,3,3),f(1,2,3)), ok1(f(1,3,3),f(1,3,1)), ok1(f(1,3,3),f(1,3,2)), ok1(f(2,1,1),f(1,1,1)), ok1(f(2,1,1),f(1,1,2)), ok1(f(2,1,1),f(1,1,3)), ok1(f(2,1,1),f(1,2,1)), ok1(f(2,1,1),f(1,2,2)), ok1(f(2,1,1),f(1,2,3)), ok1(f(2,1,1),f(1,3,1)), ok1(f(2,1,1),f(1,3,2)), ok1(f(2,1,1),f(1,3,3)), ok1(f(2,1,2),f(1,1,1)), ok1(f(2,1,2),f(1,1,2)), ok1(f(2,1,2),f(1,1,3)), ok1(f(2,1,2),f(1,2,1)), ok1(f(2,1,2),f(1,2,2)), ok1(f(2,1,2),f(1,2,3)), ok1(f(2,1,2),f(1,3,1)), ok1(f(2,1,2),f(1,3,2)), ok1(f(2,1,2),f(1,3,3)), ok1(f(2,1,2),f(2,1,1)), ok1(f(2,1,3),f(1,1,1)), ok1(f(2,1,3),f(1,1,2)), ok1(f(2,1,3),f(1,1,3)), ok1(f(2,1,3),f(1,2,1)), ok1(f(2,1,3),f(1,2,2)), ok1(f(2,1,3),f(1,2,3)), ok1(f(2,1,3),f(1,3,1)), ok1(f(2,1,3),f(1,3,2)), ok1(f(2,1,3),f(1,3,3)), ok1(f(2,1,3),f(2,1,1)), ok1(f(2,1,3),f(2,1,2)), ok1(f(2,2,1),f(1,1,1)), ok1(f(2,2,1),f(1,1,2)), ok1(f(2,2,1),f(1,1,3)), ok1(f(2,2,1),f(1,2,1)), ok1(f(2,2,1),f(1,2,2)), ok1(f(2,2,1),f(1,2,3)), ok1(f(2,2,1),f(1,3,1)), ok1(f(2,2,1),f(1,3,2)), ok1(f(2,2,1),f(1,3,3)), ok1(f(2,2,1),f(2,1,1)), ok1(f(2,2,1),f(2,1,2)), ok1(f(2,2,1),f(2,1,3)), ok1(f(2,2,2),f(1,1,1)), ok1(f(2,2,2),f(1,1,2)), ok1(f(2,2,2),f(1,1,3)), ok1(f(2,2,2),f(1,2,1)), ok1(f(2,2,2),f(1,2,2)), ok1(f(2,2,2),f(1,2,3)), ok1(f(2,2,2),f(1,3,1)), ok1(f(2,2,2),f(1,3,2)), ok1(f(2,2,2),f(1,3,3)), ok1(f(2,2,2),f(2,1,1)), ok1(f(2,2,2),f(2,1,2)), ok1(f(2,2,2),f(2,1,3)), ok1(f(2,2,2),f(2,2,1)), ok1(f(2,2,3),f(1,1,1)), ok1(f(2,2,3),f(1,1,2)), ok1(f(2,2,3),f(1,1,3)), ok1(f(2,2,3),f(1,2,1)), ok1(f(2,2,3),f(1,2,2)), ok1(f(2,2,3),f(1,2,3)), ok1(f(2,2,3),f(1,3,1)), ok1(f(2,2,3),f(1,3,2)), ok1(f(2,2,3),f(1,3,3)), ok1(f(2,2,3),f(2,1,1)), ok1(f(2,2,3),f(2,1,2)), ok1(f(2,2,3),f(2,1,3)), ok1(f(2,2,3),f(2,2,1)), ok1(f(2,2,3),f(2,2,2)), ok1(f(2,3,1),f(1,1,1)), ok1(f(2,3,1),f(1,1,2)), ok1(f(2,3,1),f(1,1,3)), ok1(f(2,3,1),f(1,2,1)), ok1(f(2,3,1),f(1,2,2)), ok1(f(2,3,1),f(1,2,3)), ok1(f(2,3,1),f(1,3,1)), ok1(f(2,3,1),f(1,3,2)), ok1(f(2,3,1),f(1,3,3)), ok1(f(2,3,1),f(2,1,1)), ok1(f(2,3,1),f(2,1,2)), ok1(f(2,3,1),f(2,1,3)), ok1(f(2,3,1),f(2,2,1)), ok1(f(2,3,1),f(2,2,2)), ok1(f(2,3,1),f(2,2,3)), ok1(f(2,3,2),f(1,1,1)), ok1(f(2,3,2),f(1,1,2)), ok1(f(2,3,2),f(1,1,3)), ok1(f(2,3,2),f(1,2,1)), ok1(f(2,3,2),f(1,2,2)), ok1(f(2,3,2),f(1,2,3)), ok1(f(2,3,2),f(1,3,1)), ok1(f(2,3,2),f(1,3,2)), ok1(f(2,3,2),f(1,3,3)), ok1(f(2,3,2),f(2,1,1)), ok1(f(2,3,2),f(2,1,2)), ok1(f(2,3,2),f(2,1,3)), ok1(f(2,3,2),f(2,2,1)), ok1(f(2,3,2),f(2,2,2)), ok1(f(2,3,2),f(2,2,3)), ok1(f(2,3,2),f(2,3,1)), ok1(f(2,3,3),f(1,1,1)), ok1(f(2,3,3),f(1,1,2)), ok1(f(2,3,3),f(1,1,3)), ok1(f(2,3,3),f(1,2,1)), ok1(f(2,3,3),f(1,2,2)), ok1(f(2,3,3),f(1,2,3)), ok1(f(2,3,3),f(1,3,1)), ok1(f(2,3,3),f(1,3,2)), ok1(f(2,3,3),f(1,3,3)), ok1(f(2,3,3),f(2,1,1)), ok1(f(2,3,3),f(2,1,2)), ok1(f(2,3,3),f(2,1,3)), ok1(f(2,3,3),f(2,2,1)), ok1(f(2,3,3),f(2,2,2)), ok1(f(2,3,3),f(2,2,3)), ok1(f(2,3,3),f(2,3,1)), ok1(f(2,3,3),f(2,3,2)), ok1(f(3,1,1),f(1,1,1)), ok1(f(3,1,1),f(1,1,2)), ok1(f(3,1,1),f(1,1,3)), ok1(f(3,1,1),f(1,2,1)), ok1(f(3,1,1),f(1,2,2)), ok1(f(3,1,1),f(1,2,3)), ok1(f(3,1,1),f(1,3,1)), ok1(f(3,1,1),f(1,3,2)), ok1(f(3,1,1),f(1,3,3)), ok1(f(3,1,1),f(2,1,1)), ok1(f(3,1,1),f(2,1,2)), ok1(f(3,1,1),f(2,1,3)), ok1(f(3,1,1),f(2,2,1)), ok1(f(3,1,1),f(2,2,2)), ok1(f(3,1,1),f(2,2,3)), ok1(f(3,1,1),f(2,3,1)), ok1(f(3,1,1),f(2,3,2)), ok1(f(3,1,1),f(2,3,3)), ok1(f(3,1,2),f(1,1,1)), ok1(f(3,1,2),f(1,1,2)), ok1(f(3,1,2),f(1,1,3)), ok1(f(3,1,2),f(1,2,1)), ok1(f(3,1,2),f(1,2,2)), ok1(f(3,1,2),f(1,2,3)), ok1(f(3,1,2),f(1,3,1)), ok1(f(3,1,2),f(1,3,2)), ok1(f(3,1,2),f(1,3,3)), ok1(f(3,1,2),f(2,1,1)), ok1(f(3,1,2),f(2,1,2)), ok1(f(3,1,2),f(2,1,3)), ok1(f(3,1,2),f(2,2,1)), ok1(f(3,1,2),f(2,2,2)), ok1(f(3,1,2),f(2,2,3)), ok1(f(3,1,2),f(2,3,1)), ok1(f(3,1,2),f(2,3,2)), ok1(f(3,1,2),f(2,3,3)), ok1(f(3,1,2),f(3,1,1)), ok1(f(3,1,3),f(1,1,1)), ok1(f(3,1,3),f(1,1,2)), ok1(f(3,1,3),f(1,1,3)), ok1(f(3,1,3),f(1,2,1)), ok1(f(3,1,3),f(1,2,2)), ok1(f(3,1,3),f(1,2,3)), ok1(f(3,1,3),f(1,3,1)), ok1(f(3,1,3),f(1,3,2)), ok1(f(3,1,3),f(1,3,3)), ok1(f(3,1,3),f(2,1,1)), ok1(f(3,1,3),f(2,1,2)), ok1(f(3,1,3),f(2,1,3)), ok1(f(3,1,3),f(2,2,1)), ok1(f(3,1,3),f(2,2,2)), ok1(f(3,1,3),f(2,2,3)), ok1(f(3,1,3),f(2,3,1)), ok1(f(3,1,3),f(2,3,2)), ok1(f(3,1,3),f(2,3,3)), ok1(f(3,1,3),f(3,1,1)), ok1(f(3,1,3),f(3,1,2)), ok1(f(3,2,1),f(1,1,1)), ok1(f(3,2,1),f(1,1,2)), ok1(f(3,2,1),f(1,1,3)), ok1(f(3,2,1),f(1,2,1)), ok1(f(3,2,1),f(1,2,2)), ok1(f(3,2,1),f(1,2,3)), ok1(f(3,2,1),f(1,3,1)), ok1(f(3,2,1),f(1,3,2)), ok1(f(3,2,1),f(1,3,3)), ok1(f(3,2,1),f(2,1,1)), ok1(f(3,2,1),f(2,1,2)), ok1(f(3,2,1),f(2,1,3)), ok1(f(3,2,1),f(2,2,1)), ok1(f(3,2,1),f(2,2,2)), ok1(f(3,2,1),f(2,2,3)), ok1(f(3,2,1),f(2,3,1)), ok1(f(3,2,1),f(2,3,2)), ok1(f(3,2,1),f(2,3,3)), ok1(f(3,2,1),f(3,1,1)), ok1(f(3,2,1),f(3,1,2)), ok1(f(3,2,1),f(3,1,3)), ok1(f(3,2,2),f(1,1,1)), ok1(f(3,2,2),f(1,1,2)), ok1(f(3,2,2),f(1,1,3)), ok1(f(3,2,2),f(1,2,1)), ok1(f(3,2,2),f(1,2,2)), ok1(f(3,2,2),f(1,2,3)), ok1(f(3,2,2),f(1,3,1)), ok1(f(3,2,2),f(1,3,2)), ok1(f(3,2,2),f(1,3,3)), ok1(f(3,2,2),f(2,1,1)), ok1(f(3,2,2),f(2,1,2)), ok1(f(3,2,2),f(2,1,3)), ok1(f(3,2,2),f(2,2,1)), ok1(f(3,2,2),f(2,2,2)), ok1(f(3,2,2),f(2,2,3)), ok1(f(3,2,2),f(2,3,1)), ok1(f(3,2,2),f(2,3,2)), ok1(f(3,2,2),f(2,3,3)), ok1(f(3,2,2),f(3,1,1)), ok1(f(3,2,2),f(3,1,2)), ok1(f(3,2,2),f(3,1,3)), ok1(f(3,2,2),f(3,2,1)), ok1(f(3,2,3),f(1,1,1)), ok1(f(3,2,3),f(1,1,2)), ok1(f(3,2,3),f(1,1,3)), ok1(f(3,2,3),f(1,2,1)), ok1(f(3,2,3),f(1,2,2)), ok1(f(3,2,3),f(1,2,3)), ok1(f(3,2,3),f(1,3,1)), ok1(f(3,2,3),f(1,3,2)), ok1(f(3,2,3),f(1,3,3)), ok1(f(3,2,3),f(2,1,1)), ok1(f(3,2,3),f(2,1,2)), ok1(f(3,2,3),f(2,1,3)), ok1(f(3,2,3),f(2,2,1)), ok1(f(3,2,3),f(2,2,2)), ok1(f(3,2,3),f(2,2,3)), ok1(f(3,2,3),f(2,3,1)), ok1(f(3,2,3),f(2,3,2)), ok1(f(3,2,3),f(2,3,3)), ok1(f(3,2,3),f(3,1,1)), ok1(f(3,2,3),f(3,1,2)), ok1(f(3,2,3),f(3,1,3)), ok1(f(3,2,3),f(3,2,1)), ok1(f(3,2,3),f(3,2,2)), ok1(f(3,3,1),f(1,1,1)), ok1(f(3,3,1),f(1,1,2)), ok1(f(3,3,1),f(1,1,3)), ok1(f(3,3,1),f(1,2,1)), ok1(f(3,3,1),f(1,2,2)), ok1(f(3,3,1),f(1,2,3)), ok1(f(3,3,1),f(1,3,1)), ok1(f(3,3,1),f(1,3,2)), ok1(f(3,3,1),f(1,3,3)), ok1(f(3,3,1),f(2,1,1)), ok1(f(3,3,1),f(2,1,2)), ok1(f(3,3,1),f(2,1,3)), ok1(f(3,3,1),f(2,2,1)), ok1(f(3,3,1),f(2,2,2)), ok1(f(3,3,1),f(2,2,3)), ok1(f(3,3,1),f(2,3,1)), ok1(f(3,3,1),f(2,3,2)), ok1(f(3,3,1),f(2,3,3)), ok1(f(3,3,1),f(3,1,1)), ok1(f(3,3,1),f(3,1,2)), ok1(f(3,3,1),f(3,1,3)), ok1(f(3,3,1),f(3,2,1)), ok1(f(3,3,1),f(3,2,2)), ok1(f(3,3,1),f(3,2,3)), ok1(f(3,3,2),f(1,1,1)), ok1(f(3,3,2),f(1,1,2)), ok1(f(3,3,2),f(1,1,3)), ok1(f(3,3,2),f(1,2,1)), ok1(f(3,3,2),f(1,2,2)), ok1(f(3,3,2),f(1,2,3)), ok1(f(3,3,2),f(1,3,1)), ok1(f(3,3,2),f(1,3,2)), ok1(f(3,3,2),f(1,3,3)), ok1(f(3,3,2),f(2,1,1)), ok1(f(3,3,2),f(2,1,2)), ok1(f(3,3,2),f(2,1,3)), ok1(f(3,3,2),f(2,2,1)), ok1(f(3,3,2),f(2,2,2)), ok1(f(3,3,2),f(2,2,3)), ok1(f(3,3,2),f(2,3,1)), ok1(f(3,3,2),f(2,3,2)), ok1(f(3,3,2),f(2,3,3)), ok1(f(3,3,2),f(3,1,1)), ok1(f(3,3,2),f(3,1,2)), ok1(f(3,3,2),f(3,1,3)), ok1(f(3,3,2),f(3,2,1)), ok1(f(3,3,2),f(3,2,2)), ok1(f(3,3,2),f(3,2,3)), ok1(f(3,3,2),f(3,3,1)), ok1(f(3,3,3),f(1,1,1)), ok1(f(3,3,3),f(1,1,2)), ok1(f(3,3,3),f(1,1,3)), ok1(f(3,3,3),f(1,2,1)), ok1(f(3,3,3),f(1,2,2)), ok1(f(3,3,3),f(1,2,3)), ok1(f(3,3,3),f(1,3,1)), ok1(f(3,3,3),f(1,3,2)), ok1(f(3,3,3),f(1,3,3)), ok1(f(3,3,3),f(2,1,1)), ok1(f(3,3,3),f(2,1,2)), ok1(f(3,3,3),f(2,1,3)), ok1(f(3,3,3),f(2,2,1)), ok1(f(3,3,3),f(2,2,2)), ok1(f(3,3,3),f(2,2,3)), ok1(f(3,3,3),f(2,3,1)), ok1(f(3,3,3),f(2,3,2)), ok1(f(3,3,3),f(2,3,3)), ok1(f(3,3,3),f(3,1,1)), ok1(f(3,3,3),f(3,1,2)), ok1(f(3,3,3),f(3,1,3)), ok1(f(3,3,3),f(3,2,1)), ok1(f(3,3,3),f(3,2,2)), ok1(f(3,3,3),f(3,2,3)), ok1(f(3,3,3),f(3,3,1)), ok1(f(3,3,3),f(3,3,2))}
"""