input = """
x | y.
:- x.
a :- x.
a :- not y.
a :- b.
b :- a.
t :- not a, not b, not x.
"""
output = """
{t, y}
"""
