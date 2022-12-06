input="""
list([1,2,3]).
list([[d,f,g]|[1,2,4]]).
list([d,f,g]).
list(s(1)).
list(1).
list(2).
list(3).
list(4).
list("ciao").
l(0..4).
membNT(X,J,Y):-list(X),l(J),list(Y),&memberNth(X,J;Y).
"""
output="""
{l(0), l(1), l(2), l(3), l(4), list([d,f,g]), list([1,2,3]), list(1), list(2), list(3), list(4), list("ciao"), list([[d,f,g],1,2,4]), list(s(1)), membNT([1,2,3],1,1), membNT([1,2,3],2,2), membNT([1,2,3],3,3), membNT([[d,f,g],1,2,4],1,[d,f,g]), membNT([[d,f,g],1,2,4],2,1), membNT([[d,f,g],1,2,4],3,2), membNT([[d,f,g],1,2,4],4,4)}
"""
