input = """
:- a, c.

a | b.
a | a.

fact1.

:- fact1, not c.
"""
output = """
INCOHERENT
"""
