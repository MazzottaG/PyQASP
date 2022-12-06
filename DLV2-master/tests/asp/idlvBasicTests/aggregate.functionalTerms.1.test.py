input = """
b(Z):-#max{f(X): a(X)}=Z.
c(Z):-#min{f(X): a(X)}=Z.
d(Z):-#count{f(X): a(X)}=Z.
f(Z):-#sum{f(X): a(X)}=Z.
a(1..10).
"""
output = """
{a(1), a(10), a(2), a(3), a(4), a(5), a(6), a(7), a(8), a(9), b(f(10)), c(f(1)), d(10), f(0)}
"""
