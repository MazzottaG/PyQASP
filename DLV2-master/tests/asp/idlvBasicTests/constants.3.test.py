input = """
%#const a=1.

p(a) :- X=a. %#int(X), X=a.
"""
output = """
{p(a)}
"""
