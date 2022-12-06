input="""
a("&%££$").a(d01).e("lk").
b(Z):-not &append_str(X,Y;Z),a(X),a(Y),e(Z).
b(Z):-&append_str(X,Y;Z),a(X),a(Y).
c(G):-b(Y),not &sub_str(6,8,Y;G),a(P),&to_qstr(P;G).
f(X):-&int(1,3;X).
nomember(X,S):-f(X),not &member_str(X,S;),a(S).
"""
output="""
{a("&%££$"), a(d01), b("&%££$&%££$"), b("&%££$d01"), b("d01&%££$"), b("lk"), b(d01d01), c("&%££$"), c("d01"), e("lk"), f(1), f(2), f(3), nomember(1,"&%££$"), nomember(2,"&%££$"), nomember(2,d01), nomember(3,"&%££$"), nomember(3,d01)}
"""
