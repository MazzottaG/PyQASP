input = """
ok(X) :- p(f(f(X))), q(f(X)).

q(X) :- p(X).

p(f(X)) :- t(X).

p(f(a)).
t(f(a)).

"""
output = """
{ok(a), p(f(a)), p(f(f(a))), q(f(a)), q(f(f(a))), t(f(a))}
"""
