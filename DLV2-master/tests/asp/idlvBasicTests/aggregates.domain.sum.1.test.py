input = """
f(one).

true :- #sum{ X : f(X) } >= 0.
"""
output = """
{f(one), true}
"""
