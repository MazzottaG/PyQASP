input = """
u_bottle(Z,Z1,Z2) | d :- bottle(Z,Z1,Z2).
a(X,Y) :- bottle(B,X,Y), X<#max{Z:u_bottle(Z,B,Z2)}<Y.
bottle(1..2,1..2,1..2).

"""
output = """
{bottle(1,1,1), bottle(1,1,2), bottle(1,2,1), bottle(1,2,2), bottle(2,1,1), bottle(2,1,2), bottle(2,2,1), bottle(2,2,2), u_bottle(1,1,1), u_bottle(1,1,2), u_bottle(1,2,1), u_bottle(1,2,2), u_bottle(2,1,1), u_bottle(2,1,2), u_bottle(2,2,1), u_bottle(2,2,2)}
{bottle(1,1,1), bottle(1,1,2), bottle(1,2,1), bottle(1,2,2), bottle(2,1,1), bottle(2,1,2), bottle(2,2,1), bottle(2,2,2), d}
"""
