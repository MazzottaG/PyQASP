{path(X,Y):arc(X,Y)}.

%  exactly one outgoing edge from each node
:-node(X),#count{Y:path(X,Y)}!=1.

% exactly one incoming edge in each node
:-node(Y),#count{X:path(X,Y)}!=1.

% reachability rules
r(X,Y):-path(X,Y).
r(X,Z):-r(X,Y),path(Y,Z).

reached(Y):-r(X,Y).
:-node(X),not reached(X).