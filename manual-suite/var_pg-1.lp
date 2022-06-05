%@forall
{p1(1..6)}.
:-#count{X:p1(X)}<1.
%@forall
{p2(1..3)}.
:-#count{X:p2(X)}<1.
%@exists
{p3(1..5)}.
:-#count{X:p3(X)}<2.
a(X)|nA(X):-p3(X),p3(Y),not p1(X+Y).
:-p2(X), #count{Y:a(Y)} < X.
%@exists
{p4(7..10)}.
:-#count{X:p4(X)}<1.
:-a(X),p4(X).
%@constraint
{a(1..5)}.
:-#count{X:a(X)}<1.
:-p4(X),a(X).
:-a(X),a(Y),Z=X+Y,Z>10.
