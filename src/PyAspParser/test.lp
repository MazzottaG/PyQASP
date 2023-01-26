assign(X,C)| notAssign(X,C):-node(X),colour(C).

:-node(X), #count{C:chosenColour(X,C)} = 0.

:-link(X,Y),chosenColour(X,C),chosenColour(Y,C),X!=Y.


1<={chosenColour(X,C):colour(C)}<=1 :- node(X).