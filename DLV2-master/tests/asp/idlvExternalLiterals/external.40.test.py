input="""
list([1,2,3]).
list([[d,f,g]|[1,2,4]]).
list([1]).
list(s(1)).
list(2).
list("ciao").
l(0..4).
listDelNth(X,J,Y):-list(X),l(J),&delNth(X,J;Y).
"""
output="""
{l(0), l(1), l(2), l(3), l(4), list([1]), list([1,2,3]), list(2), list("ciao"), listDelNth([1],1,[]), listDelNth([1,2,3],1,[2,3]), listDelNth([1,2,3],2,[1,3]), listDelNth([1,2,3],3,[1,2]), listDelNth([[d,f,g],1,2,4],1,[1,2,4]), listDelNth([[d,f,g],1,2,4],2,[[d,f,g],2,4]), listDelNth([[d,f,g],1,2,4],3,[[d,f,g],1,4]), listDelNth([[d,f,g],1,2,4],4,[[d,f,g],1,2]), list([[d,f,g],1,2,4]), list(s(1))}
"""
