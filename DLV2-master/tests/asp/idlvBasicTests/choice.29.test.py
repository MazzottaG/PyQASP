input = """
:- not b.
a | b :- a.
"""
output = """
INCOHERENT
"""
