input="""
a(F):-e(Z),d(E),&int(Z,E;X),&abs(X+Z;F),&int(Z,E;F).
e(-2..2). d(4..7).
"""
output="""
{a(0), a(1), a(2), a(3), a(4), a(5), a(6), a(7), d(4), d(5), d(6), d(7), e(-1), e(-2), e(0), e(1), e(2)}
"""