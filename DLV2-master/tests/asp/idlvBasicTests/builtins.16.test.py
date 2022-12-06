input = """
pxx4(X,X) :- X=X+4. %, #int(X).
pxx_(X,X) :- X=X+Y. %#int(Y), +(X,X,Y), #int(X).
px4x(X,X) :- X=4+X. %, #int(X).
px_x(X,X) :- X=Y+X. %#int(Y), +(X,Y,X), #int(X).
p4xx(X,X) :- 4=X+X. %+(4,X,X), #int(X).
p_xx(X,X) :- Y=X+X. %#int(Y), +(Y,X,X), #int(X).

"""
output = """
"""
