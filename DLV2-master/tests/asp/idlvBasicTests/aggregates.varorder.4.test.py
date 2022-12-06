input = """
int(1..5).

ship(1). ship(2).

coords(X,Y) :- int(X), int(Y), X > 0, Y > 0, X <= 2, Y <= 2.

ship_at(S,X,Y) | ship_at_not(S,X,Y) :- ship(S), coords(X,Y).

:- ship(S), not #count{X,Y:ship_at(S,X,Y)} = 1.

:- rowsum(R,Su), not #sum{ S : ship_at(S,R,Y) } = Su.
:- colsum(C,Su), not #sum{ S : ship_at(S,X,C) } = Su.

rowsum(2,2).
colsum(1,3).

"""
output = """
{colsum(1,3), coords(1,1), coords(1,2), coords(2,1), coords(2,2), int(1), int(2), int(3), int(4), int(5), rowsum(2,2), ship(1), ship(2), ship_at(1,1,1), ship_at(2,2,1), ship_at_not(1,1,2), ship_at_not(1,2,1), ship_at_not(1,2,2), ship_at_not(2,1,1), ship_at_not(2,1,2), ship_at_not(2,2,2)}
"""
