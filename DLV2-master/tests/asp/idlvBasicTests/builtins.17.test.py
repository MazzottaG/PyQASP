input = """
minusxxy(X,X,Y) :- X=X-Y. %-(X,X,Y), #int(X).
minusxyx(X,Y,X) :- X=Y-X. %-(X,Y,X), #int(X), #int(Y).
minusyxx(Y,X,X) :- Y=X-X. %-(Y,X,X), #int(Y), #int(X).
minusxxx(X,X,X) :- X=X-X. %-(X,X,X), #int(X).

divxxy(X,X,Y) :- X=X/Y. %/(X,X,Y), #int(X).
divxyx(X,Y,X) :- X=Y/X. %/(X,Y,X), #int(X), #int(Y).
divyxx(Y,X,X) :- Y=X/X. %/(Y,X,X), #int(Y), #int(X).
divxxx(X,X,X) :- X=X/X. %/(X,X,X), #int(X).

div00(0,X,Z) :- Z=0/X. %#int(X), Z=0/X.
div01(X,0,Z) :- Z=X/0. %#int(X), Z=X/0.
div02(X,Z,0) :- 0=X/Z. %#int(X), #int(Z), 0=X/Z.
div03(Z,X,0) :- 0=Z/X. %#int(X), #int(Z), 0=Z/X.
div04(Z,0,X) :- X=Z/0. %#int(Z), X=Z/0.
div05(0,Z,X) :- X=0/Z. %#int(Z), X=0/Z.

%modxxy(X,X,Y) :- #mod(X,X,Y), #int(X).
%modxyx(X,Y,X) :- #mod(X,Y,X), #int(X), #int(Y).
%modyxx(Y,X,X) :- #mod(Y,X,X), #int(Y), #int(X).
%modxxx(X,X,X) :- #mod(X,X,X), #int(X).

%mod00(0,X,Z) :- #int(X), #mod(0,X,Z).
%mod01(X,0,Z) :- #int(X), #mod(X,0,Z).
%modxyz(X,Y,Z) :- #int(X), #int(Y), #mod(X,Y,Z).
"""
output = """
"""
