input = """
:-#max{f(X): a(X)}<3.
:-#min{f(X): a(X)}>2.
:-1<#count{f(X): a(X)}<1.
:-1<#sum{f(X): a(X)}<4.
a(a) | b.
a(b) | b.
a(c) | b.
a(d) | b.
"""
output = """
INCOHERENT
"""
