input = """
riga(1..5).
colonna(1..5).

cella(X,Y) :-riga(X), colonna(Y).

maggiore(1,4,1,5).
maggiore(1,3,2,3).
maggiore(2,4,2,3).
maggiore(3,2,2,2).
maggiore(2,3,3,3).
maggiore(3,4,2,4).
maggiore(4,1,4,2).
maggiore(4,5,4,4).
maggiore(5,2,5,3).
maggiore(5,3,4,3).
maggiore(4,4,5,4).

contiene(2,5,2).

contiene(X,Y,Z)|nonContiene(X,Y,Z) :- cella(X,Y), riga(Z).

:- contiene(X,Y,Z), contiene(X,Y2,Z), Y != Y2.
:- contiene(X,Y,Z), contiene(X2,Y,Z), X != X2.

:- contiene(X,Y,Z), contiene(X2,Y2,Z2), maggiore(X,Y,X2,Y2), Z < Z2.

:- cella(X,Y), not #count{Z: contiene(X,Y,Z)} = 1.
"""
output = """
{riga(1), riga(2), riga(3), riga(4), riga(5), colonna(1), colonna(2), colonna(3), colonna(4), colonna(5), maggiore(1,4,1,5), maggiore(1,3,2,3), maggiore(2,4,2,3), maggiore(3,2,2,2), maggiore(2,3,3,3), maggiore(3,4,2,4), maggiore(4,1,4,2), maggiore(4,5,4,4), maggiore(5,2,5,3), maggiore(5,3,4,3), maggiore(4,4,5,4), contiene(2,5,2), cella(1,1), cella(1,2), cella(1,3), cella(1,4), cella(1,5), cella(2,1), cella(2,2), cella(2,3), cella(2,4), cella(2,5), cella(3,1), cella(3,2), cella(3,3), cella(3,4), cella(3,5), cella(4,1), cella(4,2), cella(4,3), cella(4,4), cella(4,5), cella(5,1), cella(5,2), cella(5,3), cella(5,4), cella(5,5), nonContiene(1,1,1), nonContiene(1,2,1), nonContiene(1,3,1), nonContiene(1,4,1), contiene(1,5,1), nonContiene(2,1,1), contiene(2,2,1), nonContiene(2,3,1), nonContiene(2,4,1), nonContiene(2,5,1), contiene(3,1,1), nonContiene(3,2,1), nonContiene(3,3,1), nonContiene(3,4,1), nonContiene(3,5,1), nonContiene(4,1,1), nonContiene(4,2,1), contiene(4,3,1), nonContiene(4,4,1), nonContiene(4,5,1), nonContiene(5,1,1), nonContiene(5,2,1), nonContiene(5,3,1), contiene(5,4,1), nonContiene(5,5,1), nonContiene(1,1,2), nonContiene(1,2,2), nonContiene(1,3,2), contiene(1,4,2), nonContiene(1,5,2), nonContiene(2,1,2), nonContiene(2,2,2), nonContiene(2,3,2), nonContiene(2,4,2), nonContiene(3,1,2), nonContiene(3,2,2), contiene(3,3,2), nonContiene(3,4,2), nonContiene(3,5,2), nonContiene(4,1,2), contiene(4,2,2), nonContiene(4,3,2), nonContiene(4,4,2), nonContiene(4,5,2), contiene(5,1,2), nonContiene(5,2,2), nonContiene(5,3,2), nonContiene(5,4,2), nonContiene(5,5,2), contiene(1,1,3), nonContiene(1,2,3), nonContiene(1,3,3), nonContiene(1,4,3), nonContiene(1,5,3), nonContiene(2,1,3), nonContiene(2,2,3), contiene(2,3,3), nonContiene(2,4,3), nonContiene(2,5,3), nonContiene(3,1,3), contiene(3,2,3), nonContiene(3,3,3), nonContiene(3,4,3), nonContiene(3,5,3), nonContiene(4,1,3), nonContiene(4,2,3), nonContiene(4,3,3), contiene(4,4,3), nonContiene(4,5,3), nonContiene(5,1,3), nonContiene(5,2,3), nonContiene(5,3,3), nonContiene(5,4,3), contiene(5,5,3), nonContiene(1,1,4), contiene(1,2,4), nonContiene(1,3,4), nonContiene(1,4,4), nonContiene(1,5,4), nonContiene(2,1,4), nonContiene(2,2,4), nonContiene(2,3,4), contiene(2,4,4), nonContiene(2,5,4), nonContiene(3,1,4), nonContiene(3,2,4), nonContiene(3,3,4), nonContiene(3,4,4), contiene(3,5,4), contiene(4,1,4), nonContiene(4,2,4), nonContiene(4,3,4), nonContiene(4,4,4), nonContiene(4,5,4), nonContiene(5,1,4), nonContiene(5,2,4), contiene(5,3,4), nonContiene(5,4,4), nonContiene(5,5,4), nonContiene(1,1,5), nonContiene(1,2,5), contiene(1,3,5), nonContiene(1,4,5), nonContiene(1,5,5), contiene(2,1,5), nonContiene(2,2,5), nonContiene(2,3,5), nonContiene(2,4,5), nonContiene(2,5,5), nonContiene(3,1,5), nonContiene(3,2,5), nonContiene(3,3,5), contiene(3,4,5), nonContiene(3,5,5), nonContiene(4,1,5), nonContiene(4,2,5), nonContiene(4,3,5), nonContiene(4,4,5), contiene(4,5,5), nonContiene(5,1,5), contiene(5,2,5), nonContiene(5,3,5), nonContiene(5,4,5), nonContiene(5,5,5)}
"""
