input = """
b :- not a.
a :- not b.

%:- m.
m | n.

"""
output = """
{b, m}
{a, m}
{b, n}
{a, n}
"""
