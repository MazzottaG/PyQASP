input = """
reach(X,Y):-edge(X,Y).
reach(X,Z):-reach(X,Y),edge(Y,Z).

edge(1,2).
edge(2,3).
edge(2,4).
edge(3,5).
edge(5,6).
edge(5,7).
"""
output = """
{edge(1,2), edge(2,3), edge(2,4), edge(3,5), edge(5,6), edge(5,7), reach(1,2), reach(1,3), reach(1,4), reach(1,5), reach(1,6), reach(1,7), reach(2,3), reach(2,4), reach(2,5), reach(2,6), reach(2,7), reach(3,5), reach(3,6), reach(3,7), reach(5,6), reach(5,7)}
"""
