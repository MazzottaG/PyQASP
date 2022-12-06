input = """
b(1) | b(2).
b(1) | b(3).
b(1) | b(4).
x | y.
:- x, #count{X:b(X)}=1.
:- #count{X:b(X)}=1, y.
"""
output = """
{b(2), b(3), b(4), x}
{b(2), b(3), b(4), y}
"""
