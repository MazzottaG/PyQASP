input="""
list([1,2,3,6,[1,3,5]]).
list([[d,f,g]|[1,2,4]]).
list([1,2,3]).
list([1,3,5]).
list([3]).
list(s(1)).
list(2).
list("ciao").
l(5).
tail(X,L):-list(X),&tail(X;L).
"""
output="""
{list([1,2,3]), list([1,2,3,6,[1,3,5]]), list([1,3,5]), list(2), list([3]), list("ciao"), list([[d,f,g],1,2,4]), list(s(1)), l(5), tail([1,2,3],[2,3]), tail([1,2,3,6,[1,3,5]],[2,3,6,[1,3,5]]), tail([1,3,5],[3,5]), tail([3],[]), tail([[d,f,g],1,2,4],[1,2,4])}
"""
