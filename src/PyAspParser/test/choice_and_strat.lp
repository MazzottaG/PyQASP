{a(X)}:-X=1..2.

b(X):-a(X),c(X).
c(X):-a(X),b(X).

