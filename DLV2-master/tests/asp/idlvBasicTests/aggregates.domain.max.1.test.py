input = """
f(one).

true :- #max{ X : f(X) } >= 0.
"""
output = """
{f(one), true}
"""
