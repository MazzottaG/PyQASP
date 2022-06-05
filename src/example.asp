%@forall
b(1).
b(2).
a(X) | -a(X) :- b(X).
%@exists
c :- a(1).
%@constraint
:- c.

