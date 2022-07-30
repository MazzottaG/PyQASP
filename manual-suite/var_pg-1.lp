%@forall
{p1(1);p1(2);p1(3);p1(4);p1(5);p1(6)}.
:-#count{X:p1(X)}<1.
%@forall
{p2(1);p2(2);p2(3)}.
:-#count{X:p2(X)}<1.
%@exists
{p3(1);p3(2);p3(3);p3(4);p3(5)}.
:-#count{X:p3(X)}<2.
a(X)|nA(X):-p3(X),p3(Y),not p1(X+Y).
:-p2(X), #count{Y:a(Y)} < X.
%@exists
{p4(7);p4(8);p4(9);p4(10)}.
:-#count{X:p4(X)}<1.
:-a(X),p4(X).
%@constraint
aa(X):-a(X).
{aa(1);aa(2);aa(3);aa(4);aa(5)}.
:-#count{X:aa(X)}<1.
:-p4(X),aa(X).
:-aa(X),aa(Y),Z=X+Y,Z>10.
