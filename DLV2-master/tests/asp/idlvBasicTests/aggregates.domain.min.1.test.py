input = """
f(one).

true :- #min{ X : f(X) } >= 0.
"""
output = """
{f(one), true}
"""
