input = """
true.
a | b :- true, 1 < 2.
:- a.
"""
output = """
{b, true}
"""
