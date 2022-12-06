input = """
num(1..2).
a(f(X,Y)) :- num(X),num(Y).
b(f(X,Y)) :- num(X),num(Y).

ok1(X,Y) :- a(X), b(Y),X>Y.
ok2(X,Y) :- a(X), b(Y),X<Y.
ok3(X,Y) :- a(X), b(Y),X>=Y.
ok3(X,Y) :- a(X), b(Y),X<=Y.
ok4(X,Y) :- a(X), b(Y),X!=Y.
ok5(X,Y) :- a(X), b(Y),X=Y.
"""
output = """
{a(f(1,1)), a(f(1,2)), a(f(2,1)), a(f(2,2)), b(f(1,1)), b(f(1,2)), b(f(2,1)), b(f(2,2)), num(1), num(2), ok1(f(1,2),f(1,1)), ok1(f(2,1),f(1,1)), ok1(f(2,1),f(1,2)), ok1(f(2,2),f(1,1)), ok1(f(2,2),f(1,2)), ok1(f(2,2),f(2,1)), ok2(f(1,1),f(1,2)), ok2(f(1,1),f(2,1)), ok2(f(1,1),f(2,2)), ok2(f(1,2),f(2,1)), ok2(f(1,2),f(2,2)), ok2(f(2,1),f(2,2)), ok3(f(1,1),f(1,1)), ok3(f(1,1),f(1,2)), ok3(f(1,1),f(2,1)), ok3(f(1,1),f(2,2)), ok3(f(1,2),f(1,1)), ok3(f(1,2),f(1,2)), ok3(f(1,2),f(2,1)), ok3(f(1,2),f(2,2)), ok3(f(2,1),f(1,1)), ok3(f(2,1),f(1,2)), ok3(f(2,1),f(2,1)), ok3(f(2,1),f(2,2)), ok3(f(2,2),f(1,1)), ok3(f(2,2),f(1,2)), ok3(f(2,2),f(2,1)), ok3(f(2,2),f(2,2)), ok4(f(1,1),f(1,2)), ok4(f(1,1),f(2,1)), ok4(f(1,1),f(2,2)), ok4(f(1,2),f(1,1)), ok4(f(1,2),f(2,1)), ok4(f(1,2),f(2,2)), ok4(f(2,1),f(1,1)), ok4(f(2,1),f(1,2)), ok4(f(2,1),f(2,2)), ok4(f(2,2),f(1,1)), ok4(f(2,2),f(1,2)), ok4(f(2,2),f(2,1)), ok5(f(1,1),f(1,1)), ok5(f(1,2),f(1,2)), ok5(f(2,1),f(2,1)), ok5(f(2,2),f(2,2))}
"""
