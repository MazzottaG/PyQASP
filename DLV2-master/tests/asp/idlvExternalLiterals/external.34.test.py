input="""
a(1..3).
sumRange(N):-#sum{Z:a(X),a(Y),&int(X,Y;Z)}=N.
sumRange2(X,Y,N):-a(X),a(Y),#sum{Z:&int(X,Y;Z)}=N.
"""
output="""
{a(1), a(2), a(3), sumRange(6), sumRange2(1,1,1), sumRange2(1,2,3), sumRange2(1,3,6), sumRange2(2,1,0), sumRange2(2,2,2), sumRange2(2,3,5), sumRange2(3,1,0), sumRange2(3,2,0), sumRange2(3,3,3)}
"""
