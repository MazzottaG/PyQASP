input="""
a(X):-e(Z),d(E),&int(Z,E;X),&sum(X,Z;F),&int(Z,E;F).
e(-2..2). d(1..4).
"""
output="""
{a(0), a(1), a(2), a(3), a(4), d(1), d(2), d(3), d(4), e(-1), e(-2), e(0), e(1), e(2)}
"""
