input = """
% unbound variables should be allowed in the
% requires part of action/fluent declarations:
town(a).
town(b).
edge(a,b,2).
"""
output = """
{edge(a,b,2), town(a), town(b)}
"""
