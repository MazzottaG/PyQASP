input = """
%#const a = 10.

p(10).
a :- p(a).
"""
output = """
{p(10)}
"""
