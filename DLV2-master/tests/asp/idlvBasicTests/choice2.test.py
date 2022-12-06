input = """
{a;b}:-c.
c.
"""
output = """
{c}
{b, c}
{a, c}
{a, b, c}
"""
