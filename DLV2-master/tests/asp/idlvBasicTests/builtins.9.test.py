input = """
-p(1) | f.
p(X) :- not -p(X), number(X).

number(X) :- X!=0. %#int(X), X!=0.
:- f.

"""
output = """
"""
