input="""
a(1..6).
div(U,D):-&mod(U,D;0),a(U),a(D).
dis(X):-a(X),not &mod(X,2;0).
par(X):-a(X),not &mod(X,2;Y),Y=1.
"""
output="""
{a(1), a(2), a(3), a(4), a(5), a(6), dis(1), dis(3), dis(5), div(1,1), div(2,1), div(2,2), div(3,1), div(3,3), div(4,1), div(4,2), div(4,4), div(5,1), div(5,5), div(6,1), div(6,2), div(6,3), div(6,6), par(2), par(4), par(6)}
"""
