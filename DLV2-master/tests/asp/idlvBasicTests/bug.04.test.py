input = """
:- a, b, c.

a | b.

a :- b.

fact1.

fact2.

:- fact1, not c, fact2.
"""
output = """
INCOHERENT
"""
