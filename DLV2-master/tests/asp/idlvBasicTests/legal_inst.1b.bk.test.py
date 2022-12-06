input = """
% if more than one body matches the
% requires part, this shouldn't change anything:

town(a).
town(b).
edge(a,b,2).
edge(a,b,4).
"""
output = """
{edge(a,b,2), edge(a,b,4), town(a), town(b)}
"""
