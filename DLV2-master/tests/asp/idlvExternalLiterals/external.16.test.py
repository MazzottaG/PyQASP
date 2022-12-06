input="""
a(Z):-&int(1,3;Z).
e(Z):-&int(2,5;Z).
e("2").e("3").e("4").
a("2").a("3").
b("21").b("22").b("23").

g(X):-&append_str(Z,Y;X),e(Z),a(Y),b(X).
"""
output="""
{a("2"), a("3"), a(1), a(2), a(3), b("21"), b("22"), b("23"), e("2"), e("3"), e("4"), e(2), e(3), e(4), e(5), g("21"), g("22"), g("23")}
"""
