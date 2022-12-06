input = """
:- not b.
b :- a, not a.
a | c.
"""
output = """
INCOHERENT
"""
