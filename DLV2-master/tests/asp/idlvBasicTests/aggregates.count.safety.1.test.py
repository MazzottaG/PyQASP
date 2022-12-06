input = """
ouch(V) :- #count{V: not a(V)} = 1.
:- #count{V: not a(V)} = 1.
"""
output = """
"""
