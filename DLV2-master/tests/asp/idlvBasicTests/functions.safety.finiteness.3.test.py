input = """
p(f(X)) :- p(f(X)).
p(f(a)).
"""
output = """
{p(f(a))}
"""
