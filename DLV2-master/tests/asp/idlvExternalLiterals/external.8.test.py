input="""
a(Z)|nota(Z):-&int(1,3;Z).
e(Z):-&int(2,5;Z).

b(Z):-e(Z),a(Y), &int(X+Y,Y;Z),a(X).
v(Z):-a(Z),&int(X,Y;Z+1),e(X),e(Y).

:-a(X),e(Y),&int(X,Y;Z),e(Z).
"""
output="""
{e(2), e(3), e(4), e(5), nota(1), nota(2), nota(3)}
"""
