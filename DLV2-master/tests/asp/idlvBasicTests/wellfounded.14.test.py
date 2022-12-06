input = """
a :- b.
b :- a.
:- not a.

"""
output = """
INCOHERENT
"""
