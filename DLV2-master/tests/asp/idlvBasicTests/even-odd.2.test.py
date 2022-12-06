input = """
true.
even(0) :- true.
even(X) :- even(Y), Y=X+2.

odd(X) :- not even(X). %#int(X), not even(X)."""
output = """
"""
