input = """
edge(a,b,1).
edge(a,c,3).
edge(c,b,2).
edge(b,d,3).
edge(b,c,1).
edge(c,d,3).

town(T) :- edge(T,_,_).
town(T) :- edge(_,T,_).
"""
output = """
{edge(a,b,1), edge(a,c,3), edge(b,c,1), edge(b,d,3), edge(c,b,2), edge(c,d,3), town(a), town(b), town(c), town(d)}
"""
