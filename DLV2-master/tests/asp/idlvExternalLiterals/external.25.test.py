input="""
b(Z):-a(X),a(Y),&sum(X,Y;Z).
c(X):-a(X),a(Y),&sum(X,Y;2).
d(Z):-a(X),a(Y),&sum(X,Y;Z),e(Z).
f(Z):-a(Y),&sum(2,Y;Z).
a(1..3).e(6..10).
"""
output="""
{a(1), a(2), a(3), b(2), b(3), b(4), b(5), b(6), c(1), d(6), e(10), e(6), e(7), e(8), e(9), f(3), f(4), f(5)}
"""
