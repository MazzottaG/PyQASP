%@forall
{p1(1..5)}.
:-#count{X:p1(X)}!=2.
%@exists
{p2(1..4)}.
:-#count{X:p2(X)}<2.
b(X,Y)|nB(X,Y):-p1(X),p2(Y).
:-#count{X,Y:b(X,Y)}<2.
%@exists
{p3(1..10)}.
:-#count{X:p3(X)}<1.
:-b(X,Y),not p3(X+Y).
%@constraint
{a(1..10)}.
:-#count{X:a(X)}<1.
:-a(X),p3(X).
