{a(X)}:-X=1..2.

b(X):-a(X),not c(X).
c(X):-a(X),not b(X).

