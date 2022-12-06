input="""
list([1,2,3,6,[1,3,5]]).
list([[d,f,g]|[1,2,4]]).
list([1,2,3]).
list([1,2]).
list([6,[1,3,5]]).
%list([1,3,5]).
list(s(1)).
list(2).
list("ciao").
sub(X,L):-list(X),list(L),&subList(X,L;).



%errore-> member(X,Y):-list(Y),list(X),&member(X,Y;). se X=[1,3,5] funziona
%member(Y):-list(Y),list(X),&member([1,3,5],Y;). non funziona
"""
output="""
{list("ciao"), list(2), list([1,2,3,6,[1,3,5]]), list([1,2,3]), list([1,2]), list([6,[1,3,5]]), list([[d,f,g],1,2,4]), list(s(1)), sub([1,2,3,6,[1,3,5]],[1,2,3,6,[1,3,5]]), sub([1,2,3],[1,2,3,6,[1,3,5]]), sub([1,2,3],[1,2,3]), sub([1,2],[1,2,3,6,[1,3,5]]), sub([1,2],[1,2,3]), sub([1,2],[1,2]), sub([1,2],[[d,f,g],1,2,4]), sub([6,[1,3,5]],[1,2,3,6,[1,3,5]]), sub([6,[1,3,5]],[6,[1,3,5]]), sub([[d,f,g],1,2,4],[[d,f,g],1,2,4])}
"""
