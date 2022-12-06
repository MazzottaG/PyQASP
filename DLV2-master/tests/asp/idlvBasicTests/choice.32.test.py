input = """
a | b.
:- b.

a | x | y :- x.
x | y :- y.

:- not x, not y.
"""
output = """
INCOHERENT
"""
