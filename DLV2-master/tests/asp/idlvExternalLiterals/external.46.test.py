input="""
list([1,2,3]).
list([[d,f,g]|[1,2,4]]).
list(s(1)).
list(2).
list("ciao").
l(0..3).
lastb(X,D,S):-list(X),l(D),l(S),&length(X;D+S).
"""
output="""
{l(0), l(1), l(2), l(3), lastb([1,2,3],0,3), lastb([1,2,3],1,2), lastb([1,2,3],2,1), lastb([1,2,3],3,0), lastb([[d,f,g],1,2,4],1,3), lastb([[d,f,g],1,2,4],2,2), lastb([[d,f,g],1,2,4],3,1), list("ciao"), list(2), list([1,2,3]), list([[d,f,g],1,2,4]), list(s(1))}
"""
