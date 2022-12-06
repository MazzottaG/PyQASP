input = """
a(a).
b(1).
b(2).

cap(1).

all(X,Y) | nall(X,Y) :- a(X), b(Y).



"""
output = """
{a(a), all(a,1), b(1), b(2), cap(1), nall(a,2)}
{a(a), all(a,1), all(a,2), b(1), b(2), cap(1)}
{a(a), b(1), b(2), cap(1), nall(a,1), nall(a,2)}
{a(a), all(a,2), b(1), b(2), cap(1), nall(a,1)}
"""
