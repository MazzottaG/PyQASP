input = """
a(X) | b(Y) :- v (X,Y) .
 v(v ,v) | w(v,w) | v(w, v ).
"""
output = """
{w(v,w)}
{a(v), v(v,v)}
{b(v), v(v,v)}
{a(w), v(w,v)}
{b(v), v(w,v)}
"""
