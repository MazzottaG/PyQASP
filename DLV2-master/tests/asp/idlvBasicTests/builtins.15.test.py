input = """
plusxxy(X,X,Y) :- X=X+Y. %+(X,X,Y), #int(X).
plusxyx(X,Y,X) :- X=Y+X. %+(X,Y,X), #int(X), #int(Y).
plusyxx(Y,X,X) :- Y=X+X. %+(Y,X,X), #int(Y), #int(X).
plusxxx(X,X,X) :- X=X+X. %+(X,X,X), #int(X).

timesxxy(X,X,Y) :- X=X*Y. %*(X,X,Y), #int(X).
timesxyx(X,Y,X) :- X=Y*X. %*(X,Y,X), #int(Y), #int(X).
timesyxx(Y,X,X) :- Y=X*X. %*(Y,X,X), #int(Y), #int(X).
timesxxx(X,X,X) :- X=X*X. %*(X,X,X), #int(X).

"""
output = """
"""
