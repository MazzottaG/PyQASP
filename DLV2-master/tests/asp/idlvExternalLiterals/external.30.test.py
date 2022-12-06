input="""
l(Z):- a(X),a(Y),&sum(X,Y;Z+2),e(Z).
y(Z):- a(X),a(Y),e(Z),&sum(X,Y;Z+2).
a(1..5). e(1..2).
"""
output="""
{a(1), a(2), a(3), a(4), a(5), e(1), e(2), l(1), l(2), y(1), y(2)}
"""
