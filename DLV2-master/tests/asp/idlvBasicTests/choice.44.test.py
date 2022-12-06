input = """
:- not a, x.
:- not e, y.

x | y.


l | a | e :- l.
a | e | l :- a.

nota :- y, not a.
a :- r, y, not nota.
note :- x, not e.
e :- r, x, not note.

r :- x, y.
"""
output = """
INCOHERENT
"""
