input = """
a(a) | b.

nok :- 1 < #count{A:a(A)}.

"""
output = """
{a(a)}
{b}
"""
