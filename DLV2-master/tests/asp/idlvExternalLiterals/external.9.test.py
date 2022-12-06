input="""
a(X,Y):-b(X),&sum(X,Y;4),c(Y).
c(D):-&sum(X,2;Z),b(X),&abs(Z;D).
c(Z):-b(X),&abs(X;Y),&sum(Y,X;Z).
b(-7).b(-1).b(20).b(5).
"""
output="""
{a(-1,5), b(-1), b(-7), b(20), b(5), c(0), c(1), c(10), c(22), c(40), c(5), c(7)}
"""
