assign(X,C)| notAssign(X,C):-node(X),colour(C).

:-node(X), #count{C:assign(X,C)} = 0.

:-link(X,Y),assign(X,C),assign(Y,C),X!=Y.
