input = """
a(D) :- edge(X,Y,X2,Y2), D =  (X2-X) * (Y2-Y) , not inv(X,Y,X2,Y2).
edge(50,90,50,98).
edge(50,98,50,90).
edge(50,90,49,93).
"""
output = """
{a(-3), a(0), edge(50,90,49,93), edge(50,90,50,98), edge(50,98,50,90)}
"""
