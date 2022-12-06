input = """
b:-#max{f(X): a(X)}<3.
c:-#min{f(X): a(X)}>2.
d:-1<#count{f(X): a(X)}<1.
f:-1<#sum{f(X): a(X)}<4.
a(1..10) | b.
"""
output = """
{a(1), a(10), a(2), a(3), a(4), a(5), a(6), a(7), a(8), a(9), c}
{b, c}
"""
