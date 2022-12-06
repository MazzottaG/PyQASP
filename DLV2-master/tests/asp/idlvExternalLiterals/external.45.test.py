input="""
list([1,2,3,2]).
list([[d,f,g]|[1,2,4]]).
list([a,b,[a,b]]).
list(s(1)).
list(2).
list("ciao").
last(X,D):-list(X),&last(X;D).
"""
output="""
{last([1,2,3,2],2), last([a,b,[a,b]],[a,b]), last([[d,f,g],1,2,4],4), list([1,2,3,2]), list(2), list([a,b,[a,b]]), list("ciao"), list([[d,f,g],1,2,4]), list(s(1))}
"""
