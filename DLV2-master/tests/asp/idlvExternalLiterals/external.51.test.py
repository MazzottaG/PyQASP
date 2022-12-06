input="""
list([1,2,3,6,[1,3,5]]).
list([[d,f,g]|[1,2,4]]).
list([1,2,3]).
list([3,2,[a,[b,a],b],1]).
list(s(1)).
list(2).
list("ciao").
rev(X,L):-list(X),&reverse_r(X;L).
"""
output="""
{list([1,2,3]), list([1,2,3,6,[1,3,5]]), list(2), list([3,2,[a,[b,a],b],1]), list("ciao"), list([[d,f,g],1,2,4]), list(s(1)), rev([1,2,3],[3,2,1]), rev([1,2,3,6,[1,3,5]],[[5,3,1],6,3,2,1]), rev([3,2,[a,[b,a],b],1],[1,[b,[a,b],a],2,3]), rev([[d,f,g],1,2,4],[4,2,1,[g,f,d]])}
"""
