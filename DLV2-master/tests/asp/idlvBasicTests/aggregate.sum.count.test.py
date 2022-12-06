input = """
edge(a,b). edge(b,c). edge(c,a).

num_edges(N) :- N = #sum { 1,X,Y : edge(X,Y) }.
"""
output = """
{edge(a,b), edge(b,c), edge(c,a), num_edges(3)}
"""
