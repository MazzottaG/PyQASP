input = """
a(1) | a(2).
b(1) | b(2).
okay :- #count{V:a(V),b(V)}=1.
"""
output = """
{a(2), b(2), okay}
{a(1), b(1), okay}
{a(1), b(2)}
{a(2), b(1)}
"""
