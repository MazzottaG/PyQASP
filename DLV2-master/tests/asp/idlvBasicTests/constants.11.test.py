input = """
%#const a = a.
p(a).
a :- p(a).
"""
output = """
{a, p(a)}
"""
