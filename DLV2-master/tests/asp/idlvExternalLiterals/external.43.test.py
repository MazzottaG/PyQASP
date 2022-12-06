input="""
list([1,2,3]).
list([[d,f,g]|[1,2,4]]).
list(s(1)).
list(2).
list("ciao").
l(0..3).
l([a,b]).
insLast(X,K,Y):-list(X),l(K),&insLast(X,K;Y).
%insLast(X,Y):-list(X),&insLast(X,[a,b];Y). ->> ok
"""
output="""
{insLast([1,2,3],0,[1,2,3,0]), insLast([1,2,3],1,[1,2,3,1]), insLast([1,2,3],2,[1,2,3,2]), insLast([1,2,3],3,[1,2,3,3]), insLast([1,2,3],[a,b],[1,2,3,[a,b]]), insLast([[d,f,g],1,2,4],0,[[d,f,g],1,2,4,0]), insLast([[d,f,g],1,2,4],1,[[d,f,g],1,2,4,1]), insLast([[d,f,g],1,2,4],2,[[d,f,g],1,2,4,2]), insLast([[d,f,g],1,2,4],3,[[d,f,g],1,2,4,3]), insLast([[d,f,g],1,2,4],[a,b],[[d,f,g],1,2,4,[a,b]]), l(0), l(1), l(2), l(3), list([1,2,3]), list(2), list("ciao"), list([[d,f,g],1,2,4]), list(s(1)), l([a,b])}
"""
