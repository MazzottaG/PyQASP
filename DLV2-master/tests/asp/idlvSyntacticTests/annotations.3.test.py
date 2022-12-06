input = """
%@rule_atom_indexed(@atom=f(Y,Z),@arguments={2}).
a(X,Y,Z):-b(X),not c(X,Y),f(Y,Z),Z<X+Y.
b(1..5).
c(6..10,1..3).
f(1..5,1..10).

"""
output = """
{b(1), b(2), b(3), b(4), b(5), c(6,1), c(6,2), c(6,3), c(7,1), c(7,2), c(7,3), c(8,1), c(8,2), c(8,3), c(9,1), c(9,2), c(9,3), c(10,1), c(10,2), c(10,3), f(1,1), f(1,2), f(1,3), f(1,4), f(1,5), f(1,6), f(1,7), f(1,8), f(1,9), f(1,10), f(2,1), f(2,2), f(2,3), f(2,4), f(2,5), f(2,6), f(2,7), f(2,8), f(2,9), f(2,10), f(3,1), f(3,2), f(3,3), f(3,4), f(3,5), f(3,6), f(3,7), f(3,8), f(3,9), f(3,10), f(4,1), f(4,2), f(4,3), f(4,4), f(4,5), f(4,6), f(4,7), f(4,8), f(4,9), f(4,10), f(5,1), f(5,2), f(5,3), f(5,4), f(5,5), f(5,6), f(5,7), f(5,8), f(5,9), f(5,10), a(1,1,1), a(1,2,1), a(1,2,2), a(1,3,1), a(1,3,2), a(1,3,3), a(1,4,1), a(1,4,2), a(1,4,3), a(1,4,4), a(1,5,1), a(1,5,2), a(1,5,3), a(1,5,4), a(1,5,5), a(2,1,1), a(2,1,2), a(2,2,1), a(2,2,2), a(2,2,3), a(2,3,1), a(2,3,2), a(2,3,3), a(2,3,4), a(2,4,1), a(2,4,2), a(2,4,3), a(2,4,4), a(2,4,5), a(2,5,1), a(2,5,2), a(2,5,3), a(2,5,4), a(2,5,5), a(2,5,6), a(3,1,1), a(3,1,2), a(3,1,3), a(3,2,1), a(3,2,2), a(3,2,3), a(3,2,4), a(3,3,1), a(3,3,2), a(3,3,3), a(3,3,4), a(3,3,5), a(3,4,1), a(3,4,2), a(3,4,3), a(3,4,4), a(3,4,5), a(3,4,6), a(3,5,1), a(3,5,2), a(3,5,3), a(3,5,4), a(3,5,5), a(3,5,6), a(3,5,7), a(4,1,1), a(4,1,2), a(4,1,3), a(4,1,4), a(4,2,1), a(4,2,2), a(4,2,3), a(4,2,4), a(4,2,5), a(4,3,1), a(4,3,2), a(4,3,3), a(4,3,4), a(4,3,5), a(4,3,6), a(4,4,1), a(4,4,2), a(4,4,3), a(4,4,4), a(4,4,5), a(4,4,6), a(4,4,7), a(4,5,1), a(4,5,2), a(4,5,3), a(4,5,4), a(4,5,5), a(4,5,6), a(4,5,7), a(4,5,8), a(5,1,1), a(5,1,2), a(5,1,3), a(5,1,4), a(5,1,5), a(5,2,1), a(5,2,2), a(5,2,3), a(5,2,4), a(5,2,5), a(5,2,6), a(5,3,1), a(5,3,2), a(5,3,3), a(5,3,4), a(5,3,5), a(5,3,6), a(5,3,7), a(5,4,1), a(5,4,2), a(5,4,3), a(5,4,4), a(5,4,5), a(5,4,6), a(5,4,7), a(5,4,8), a(5,5,1), a(5,5,2), a(5,5,3), a(5,5,4), a(5,5,5), a(5,5,6), a(5,5,7), a(5,5,8), a(5,5,9)}
"""
