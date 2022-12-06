input = """
a(f(1)). 
a(f(f(2))).

a(X) | b(X) :- a(f(X)).
:- b(f(X)).
"""
output = """
{a(1), a(f(1)), a(f(2)), a(f(f(2))), b(2)}
{a(1), a(2), a(f(1)), a(f(2)), a(f(f(2)))}
{a(f(1)), a(f(2)), a(f(f(2))), b(1), b(2)}
{a(2), a(f(1)), a(f(2)), a(f(f(2))), b(1)}
"""
