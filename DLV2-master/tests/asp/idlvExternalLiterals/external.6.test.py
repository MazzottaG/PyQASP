input="""
a(1..3).
a(c).
a(e).
a(d).
e(2..4).
d(X,Y):- a(X),a(Y),&int(X,Y;Z),not &sum(Z,Z;G), e(G).
f(X,Y):-not &int(W,G;Z),&sum(X,Y;Z),e(X),e(Y), a(W),a(G).
"""
output="""
{a(1), a(2), a(3), a(c), a(d), a(e), d(1,1), d(1,2), d(1,3), d(2,2), d(2,3), d(3,3), e(2), e(3), e(4), f(2,2), f(2,3), f(2,4), f(3,2), f(3,3), f(3,4), f(4,2), f(4,3), f(4,4)}
"""
