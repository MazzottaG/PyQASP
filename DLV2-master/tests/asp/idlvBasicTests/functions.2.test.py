input = """
ok(X) :- p(X), q(X).
q(X) :- p(X).

p(f(a)).
"""
output = """
{ok(f(a)), p(f(a)), q(f(a))}
"""
