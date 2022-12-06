input = """
ok:- #count{V:b(V)}=X,  not p(X).

b(1).
p(2).
"""
output = """
{b(1), ok, p(2)}
"""
