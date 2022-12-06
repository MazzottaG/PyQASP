input="""
a(1..6).
countOdd(N):-#count{X:a(X),not &mod(X,2;0)}=N.
isOdd(I,B):-#count{2:not &mod(I,2;0)}=B,a(I).
isOddBin(I,B):-a(I),#count{Y:not &mod(I,2;Y),Y=0}=B.
"""
output="""
{a(1), a(2), a(3), a(4), a(5), a(6), countOdd(3), isOdd(1,1), isOdd(3,1), isOdd(5,1), isOdd(2,0), isOdd(4,0), isOdd(6,0), isOddBin(1,1), isOddBin(3,1), isOddBin(5,1), isOddBin(2,0), isOddBin(4,0), isOddBin(6,0)}
"""
