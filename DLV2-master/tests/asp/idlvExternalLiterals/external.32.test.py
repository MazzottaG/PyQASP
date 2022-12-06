input="""
a(1..6).
countEven(N):-#count{X:a(X),&mod(X,2;0)}=N.
isEven(I,B):-#count{2:&mod(I,2;0)}=B,a(I).
isEvenBin(I,B):-a(I),#count{Y:&mod(I,2;Y),Y=0}=B.


"""
output="""
{a(1), a(2), a(3), a(4), a(5), a(6), countEven(3), isEven(1,0), isEven(3,0), isEven(5,0), isEven(2,1), isEven(4,1), isEven(6,1), isEvenBin(1,0), isEvenBin(3,0), isEvenBin(5,0), isEvenBin(2,1), isEvenBin(4,1), isEvenBin(6,1)}
"""
