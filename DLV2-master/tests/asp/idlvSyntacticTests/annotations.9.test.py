input = """
b(1..5) | f.
c(1..5) | d.
%@rule_partial_order(@before={b(Y)},@after={#count{X:b(X),c(X)}<55}).
%@rule_partial_order(@before={b(Y)},@after={#count{X:b(X),c(X)}<5}).
:-#count{X:b(X),c(X)}<3,#count{X:b(X),c(X)}<5,b(Y),c(Y).

"""
output = """
{f, c(5), c(4), c(3), c(2), c(1)}
{f, d}
{b(5), b(4), b(3), b(2), b(1), d}
{b(5), b(4), b(3), b(2), b(1), c(5), c(4), c(3), c(2), c(1)}
"""
