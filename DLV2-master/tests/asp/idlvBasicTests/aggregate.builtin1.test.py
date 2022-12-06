input = """
y(X) :- a(X), #count{1:X=Y}=1.
a(1..3).
"""
output = """
{a(1), a(2), a(3), y(1), y(2), y(3)}
"""
