%@forall
{p1(1);p1(2);p1(3);p1(4);p1(5)}.
:-#count{X:p1(X)}!=2.
%@exists
{p2(1);p2(2);p2(3);p2(4)}.
:-#count{X:p2(X)}<2.
b(X,Y)|nB(X,Y):-p1(X),p2(Y).
:-#count{X,Y:b(X,Y)}<2.
%@exists
dp3(1..10).
{dp3(X):dp3(X)}.
:-#count{X:p3(X)}<1.
:-b(X,Y),not p3(X+Y).
%@constraint
da(1..10).
{a(X):da(X)}.
:-#count{X:a(X)}<1.
:-a(X),p3(X).
