input = """
%#const bsize = 10.
p(bsize) :-a.
a.
"""
output = """
{a, p(bsize)}
"""
