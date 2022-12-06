input = """
b(Z):-#max{f(X),g(X): a(X),a(Y)}=Z.
c(Z):-#min{f(X),g(X): a(X),a(Y)}=Z.
d(Z):-#count{f(X),g(X): a(X),a(Y)}=Z.
f(Z):-#sum{f(X),g(X): a(X),a(Y)}=Z.
a(1..10) | b.
"""
output = """
{b, b(#inf), c(#sup), d(0), f(0)}
{a(1), a(10), a(2), a(3), a(4), a(5), a(6), a(7), a(8), a(9), b(f(10)), c(f(1)), d(10), f(0)}
"""
