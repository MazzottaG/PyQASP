input = """
:- not a.
:- not b.
a :- c.
c :- a.
b :- d.
d :- b.
e | f.
:- f, not e.
a :- not e.
b :- not e.
"""
output = """
INCOHERENT
"""
