input="""
list([1,2,3]).
list([[d,f,g]|[1,2,4]]).
list([1,[1,[1,[1,2],2],2],2]).
list(s(1)).
list(2).
list("ciao").
flat(X,Y):-list(X),&flatten(X;Y).
"""
output="""
{flat([1,2,3],[1,2,3]), flat([1,[1,[1,[1,2],2],2],2],[1,1,1,1,2,2,2,2]), flat([[d,f,g],1,2,4],[d,f,g,1,2,4]), list("ciao"), list(2), list([1,2,3]), list([1,[1,[1,[1,2],2],2],2]), list([[d,f,g],1,2,4]), list(s(1))}
"""
