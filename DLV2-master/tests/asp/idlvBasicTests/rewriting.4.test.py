input = """
a(c).
b(X) :- a(X), not b(X).

"""
output = """
INCOHERENT
"""
