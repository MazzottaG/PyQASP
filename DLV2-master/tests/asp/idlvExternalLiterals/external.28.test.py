input="""
g(Z):-a(X),a(Y),not &sum(X,Y;Z+2),e(Z).
a(1..2). e(1..4).
"""
output="""
{a(1), a(2), e(1), e(2), e(3), e(4), g(1), g(2), g(3), g(4)}
"""
