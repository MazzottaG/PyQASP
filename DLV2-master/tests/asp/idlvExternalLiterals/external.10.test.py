input="""
c(Z):-b(X),b(Y),not &abs(Z;8),&sum(Y,X;Z).
b(-7).b(-1).b(1).

"""
output="""
{b(-7), b(-1), b(1), c(0), c(-14), c(-2), c(2), c(-6)}
"""
