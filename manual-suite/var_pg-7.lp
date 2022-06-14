%@exists
a(1).a(2).a(3).
b(1).b(2).b(3).
1{match(X,Y):a(X),b(Y)}1.
%@forall
{c(1..4)}.
%@constraint
:-match(X,Y),W=X+Y,W>=Z,c(Z).