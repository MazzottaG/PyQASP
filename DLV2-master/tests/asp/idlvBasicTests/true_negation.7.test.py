input = """
% true negation of terms does not make sense.

-g(a).
okay(a).
f(1,"zwei").
:- not g(a).
:- not okay(a).
:- not f(1,"zwei").
"""
output = """
INCOHERENT
"""
