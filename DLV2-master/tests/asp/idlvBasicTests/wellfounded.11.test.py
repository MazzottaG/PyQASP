input = """
t(Z) :- t0(Z).
t(Z) :- g(X,Y,Z), t(X).
t(Z) :- g(X,Y,Z), not t(Y).

t0(1).
g(1,2,3).
g(2,5,4).
g(2,4,5).
g(5,3,6).

"""
output = """
{g(1,2,3), g(2,4,5), g(2,5,4), g(5,3,6), t(1), t(3), t(4), t0(1)}
{g(1,2,3), g(2,4,5), g(2,5,4), g(5,3,6), t(1), t(3), t(5), t(6), t0(1)}
"""
