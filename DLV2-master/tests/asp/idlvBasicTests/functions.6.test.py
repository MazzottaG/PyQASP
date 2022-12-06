input = """
b(f(2)).
b(f(3)).

a(f(X)) :- b(X).
"""
output = """
{a(f(f(2))), a(f(f(3))), b(f(2)), b(f(3))}
"""
