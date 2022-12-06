input="""
a(Z):-&int(1,3;Z).
e(Z):-&int(2,5;Z).

b(Z):-e(Z),a(Y), &int(X+Y,Y;Z),a(X).
v(Z):-a(Z),&int(X,Y;Z+1),e(X),e(Y).

f(Z):-a(X),e(Y),&int(X,Y;Z),not e(Z).
"""
output="""
{a(1), a(2), a(3), e(2), e(3), e(4), e(5), f(1), v(1), v(2), v(3)}
"""
