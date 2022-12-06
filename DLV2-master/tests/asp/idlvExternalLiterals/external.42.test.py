input="""
list([1,2,3]).
list([2]).
list([[d,f,g]|[1,2,4]]).
list(s(1)).
list(2).
list("ciao").
head(X,Y):-list(X),&head(X;Y).
"""
output="""
{head([1,2,3],1), head([2],2), head([[d,f,g],1,2,4],[d,f,g]), list("ciao"), list(2), list([1,2,3]), list([2]), list([[d,f,g],1,2,4]), list(s(1))}
"""
