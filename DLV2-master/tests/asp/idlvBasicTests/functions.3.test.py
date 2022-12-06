input = """
ok(X) :- p(f(X)), q(f(X)).
q(X) :- p(X).

p(f(a)).
p(f(b,a)).
p(g(c)).
"""
output = """
{ok(a), p(f(a)), p(f(b,a)), p(g(c)), q(f(a)), q(f(b,a)), q(g(c))}
"""
