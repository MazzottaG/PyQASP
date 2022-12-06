input = """
% This would (have) trigger(ed) inferences from dynamic constraints.

x | y.

a :- not x.
b :- not x.

c :- b.
a :- not c.
"""
output = """
{a, b, c, y}
{a, x}
"""
