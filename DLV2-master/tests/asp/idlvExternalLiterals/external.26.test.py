input="""
i(Z):-a(X),e(Y),&sum(X*2,Y;Z).
b(Y,Z):-i(X),&sum(X/2,Y;Z),e(Y),a(Z).
a(1..3).e(6..8).
"""
output="""
{a(1), a(2), a(3), e(6), e(7), e(8), i(10), i(11), i(12), i(13), i(14), i(8), i(9)}
"""