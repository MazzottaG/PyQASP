input = """
f(1). 
f(2).
f(3).

g(A,B,C,D,E) :- f(A), f(B), f(C), f(D), f(E).

a("2b") | a("not2b").
"""
output = """
{a("2b"), f(1), f(2), f(3), g(1,1,1,1,1), g(1,1,1,1,2), g(1,1,1,1,3), g(1,1,1,2,1), g(1,1,1,2,2), g(1,1,1,2,3), g(1,1,1,3,1), g(1,1,1,3,2), g(1,1,1,3,3), g(1,1,2,1,1), g(1,1,2,1,2), g(1,1,2,1,3), g(1,1,2,2,1), g(1,1,2,2,2), g(1,1,2,2,3), g(1,1,2,3,1), g(1,1,2,3,2), g(1,1,2,3,3), g(1,1,3,1,1), g(1,1,3,1,2), g(1,1,3,1,3), g(1,1,3,2,1), g(1,1,3,2,2), g(1,1,3,2,3), g(1,1,3,3,1), g(1,1,3,3,2), g(1,1,3,3,3), g(1,2,1,1,1), g(1,2,1,1,2), g(1,2,1,1,3), g(1,2,1,2,1), g(1,2,1,2,2), g(1,2,1,2,3), g(1,2,1,3,1), g(1,2,1,3,2), g(1,2,1,3,3), g(1,2,2,1,1), g(1,2,2,1,2), g(1,2,2,1,3), g(1,2,2,2,1), g(1,2,2,2,2), g(1,2,2,2,3), g(1,2,2,3,1), g(1,2,2,3,2), g(1,2,2,3,3), g(1,2,3,1,1), g(1,2,3,1,2), g(1,2,3,1,3), g(1,2,3,2,1), g(1,2,3,2,2), g(1,2,3,2,3), g(1,2,3,3,1), g(1,2,3,3,2), g(1,2,3,3,3), g(1,3,1,1,1), g(1,3,1,1,2), g(1,3,1,1,3), g(1,3,1,2,1), g(1,3,1,2,2), g(1,3,1,2,3), g(1,3,1,3,1), g(1,3,1,3,2), g(1,3,1,3,3), g(1,3,2,1,1), g(1,3,2,1,2), g(1,3,2,1,3), g(1,3,2,2,1), g(1,3,2,2,2), g(1,3,2,2,3), g(1,3,2,3,1), g(1,3,2,3,2), g(1,3,2,3,3), g(1,3,3,1,1), g(1,3,3,1,2), g(1,3,3,1,3), g(1,3,3,2,1), g(1,3,3,2,2), g(1,3,3,2,3), g(1,3,3,3,1), g(1,3,3,3,2), g(1,3,3,3,3), g(2,1,1,1,1), g(2,1,1,1,2), g(2,1,1,1,3), g(2,1,1,2,1), g(2,1,1,2,2), g(2,1,1,2,3), g(2,1,1,3,1), g(2,1,1,3,2), g(2,1,1,3,3), g(2,1,2,1,1), g(2,1,2,1,2), g(2,1,2,1,3), g(2,1,2,2,1), g(2,1,2,2,2), g(2,1,2,2,3), g(2,1,2,3,1), g(2,1,2,3,2), g(2,1,2,3,3), g(2,1,3,1,1), g(2,1,3,1,2), g(2,1,3,1,3), g(2,1,3,2,1), g(2,1,3,2,2), g(2,1,3,2,3), g(2,1,3,3,1), g(2,1,3,3,2), g(2,1,3,3,3), g(2,2,1,1,1), g(2,2,1,1,2), g(2,2,1,1,3), g(2,2,1,2,1), g(2,2,1,2,2), g(2,2,1,2,3), g(2,2,1,3,1), g(2,2,1,3,2), g(2,2,1,3,3), g(2,2,2,1,1), g(2,2,2,1,2), g(2,2,2,1,3), g(2,2,2,2,1), g(2,2,2,2,2), g(2,2,2,2,3), g(2,2,2,3,1), g(2,2,2,3,2), g(2,2,2,3,3), g(2,2,3,1,1), g(2,2,3,1,2), g(2,2,3,1,3), g(2,2,3,2,1), g(2,2,3,2,2), g(2,2,3,2,3), g(2,2,3,3,1), g(2,2,3,3,2), g(2,2,3,3,3), g(2,3,1,1,1), g(2,3,1,1,2), g(2,3,1,1,3), g(2,3,1,2,1), g(2,3,1,2,2), g(2,3,1,2,3), g(2,3,1,3,1), g(2,3,1,3,2), g(2,3,1,3,3), g(2,3,2,1,1), g(2,3,2,1,2), g(2,3,2,1,3), g(2,3,2,2,1), g(2,3,2,2,2), g(2,3,2,2,3), g(2,3,2,3,1), g(2,3,2,3,2), g(2,3,2,3,3), g(2,3,3,1,1), g(2,3,3,1,2), g(2,3,3,1,3), g(2,3,3,2,1), g(2,3,3,2,2), g(2,3,3,2,3), g(2,3,3,3,1), g(2,3,3,3,2), g(2,3,3,3,3), g(3,1,1,1,1), g(3,1,1,1,2), g(3,1,1,1,3), g(3,1,1,2,1), g(3,1,1,2,2), g(3,1,1,2,3), g(3,1,1,3,1), g(3,1,1,3,2), g(3,1,1,3,3), g(3,1,2,1,1), g(3,1,2,1,2), g(3,1,2,1,3), g(3,1,2,2,1), g(3,1,2,2,2), g(3,1,2,2,3), g(3,1,2,3,1), g(3,1,2,3,2), g(3,1,2,3,3), g(3,1,3,1,1), g(3,1,3,1,2), g(3,1,3,1,3), g(3,1,3,2,1), g(3,1,3,2,2), g(3,1,3,2,3), g(3,1,3,3,1), g(3,1,3,3,2), g(3,1,3,3,3), g(3,2,1,1,1), g(3,2,1,1,2), g(3,2,1,1,3), g(3,2,1,2,1), g(3,2,1,2,2), g(3,2,1,2,3), g(3,2,1,3,1), g(3,2,1,3,2), g(3,2,1,3,3), g(3,2,2,1,1), g(3,2,2,1,2), g(3,2,2,1,3), g(3,2,2,2,1), g(3,2,2,2,2), g(3,2,2,2,3), g(3,2,2,3,1), g(3,2,2,3,2), g(3,2,2,3,3), g(3,2,3,1,1), g(3,2,3,1,2), g(3,2,3,1,3), g(3,2,3,2,1), g(3,2,3,2,2), g(3,2,3,2,3), g(3,2,3,3,1), g(3,2,3,3,2), g(3,2,3,3,3), g(3,3,1,1,1), g(3,3,1,1,2), g(3,3,1,1,3), g(3,3,1,2,1), g(3,3,1,2,2), g(3,3,1,2,3), g(3,3,1,3,1), g(3,3,1,3,2), g(3,3,1,3,3), g(3,3,2,1,1), g(3,3,2,1,2), g(3,3,2,1,3), g(3,3,2,2,1), g(3,3,2,2,2), g(3,3,2,2,3), g(3,3,2,3,1), g(3,3,2,3,2), g(3,3,2,3,3), g(3,3,3,1,1), g(3,3,3,1,2), g(3,3,3,1,3), g(3,3,3,2,1), g(3,3,3,2,2), g(3,3,3,2,3), g(3,3,3,3,1), g(3,3,3,3,2), g(3,3,3,3,3)}
{a("not2b"), f(1), f(2), f(3), g(1,1,1,1,1), g(1,1,1,1,2), g(1,1,1,1,3), g(1,1,1,2,1), g(1,1,1,2,2), g(1,1,1,2,3), g(1,1,1,3,1), g(1,1,1,3,2), g(1,1,1,3,3), g(1,1,2,1,1), g(1,1,2,1,2), g(1,1,2,1,3), g(1,1,2,2,1), g(1,1,2,2,2), g(1,1,2,2,3), g(1,1,2,3,1), g(1,1,2,3,2), g(1,1,2,3,3), g(1,1,3,1,1), g(1,1,3,1,2), g(1,1,3,1,3), g(1,1,3,2,1), g(1,1,3,2,2), g(1,1,3,2,3), g(1,1,3,3,1), g(1,1,3,3,2), g(1,1,3,3,3), g(1,2,1,1,1), g(1,2,1,1,2), g(1,2,1,1,3), g(1,2,1,2,1), g(1,2,1,2,2), g(1,2,1,2,3), g(1,2,1,3,1), g(1,2,1,3,2), g(1,2,1,3,3), g(1,2,2,1,1), g(1,2,2,1,2), g(1,2,2,1,3), g(1,2,2,2,1), g(1,2,2,2,2), g(1,2,2,2,3), g(1,2,2,3,1), g(1,2,2,3,2), g(1,2,2,3,3), g(1,2,3,1,1), g(1,2,3,1,2), g(1,2,3,1,3), g(1,2,3,2,1), g(1,2,3,2,2), g(1,2,3,2,3), g(1,2,3,3,1), g(1,2,3,3,2), g(1,2,3,3,3), g(1,3,1,1,1), g(1,3,1,1,2), g(1,3,1,1,3), g(1,3,1,2,1), g(1,3,1,2,2), g(1,3,1,2,3), g(1,3,1,3,1), g(1,3,1,3,2), g(1,3,1,3,3), g(1,3,2,1,1), g(1,3,2,1,2), g(1,3,2,1,3), g(1,3,2,2,1), g(1,3,2,2,2), g(1,3,2,2,3), g(1,3,2,3,1), g(1,3,2,3,2), g(1,3,2,3,3), g(1,3,3,1,1), g(1,3,3,1,2), g(1,3,3,1,3), g(1,3,3,2,1), g(1,3,3,2,2), g(1,3,3,2,3), g(1,3,3,3,1), g(1,3,3,3,2), g(1,3,3,3,3), g(2,1,1,1,1), g(2,1,1,1,2), g(2,1,1,1,3), g(2,1,1,2,1), g(2,1,1,2,2), g(2,1,1,2,3), g(2,1,1,3,1), g(2,1,1,3,2), g(2,1,1,3,3), g(2,1,2,1,1), g(2,1,2,1,2), g(2,1,2,1,3), g(2,1,2,2,1), g(2,1,2,2,2), g(2,1,2,2,3), g(2,1,2,3,1), g(2,1,2,3,2), g(2,1,2,3,3), g(2,1,3,1,1), g(2,1,3,1,2), g(2,1,3,1,3), g(2,1,3,2,1), g(2,1,3,2,2), g(2,1,3,2,3), g(2,1,3,3,1), g(2,1,3,3,2), g(2,1,3,3,3), g(2,2,1,1,1), g(2,2,1,1,2), g(2,2,1,1,3), g(2,2,1,2,1), g(2,2,1,2,2), g(2,2,1,2,3), g(2,2,1,3,1), g(2,2,1,3,2), g(2,2,1,3,3), g(2,2,2,1,1), g(2,2,2,1,2), g(2,2,2,1,3), g(2,2,2,2,1), g(2,2,2,2,2), g(2,2,2,2,3), g(2,2,2,3,1), g(2,2,2,3,2), g(2,2,2,3,3), g(2,2,3,1,1), g(2,2,3,1,2), g(2,2,3,1,3), g(2,2,3,2,1), g(2,2,3,2,2), g(2,2,3,2,3), g(2,2,3,3,1), g(2,2,3,3,2), g(2,2,3,3,3), g(2,3,1,1,1), g(2,3,1,1,2), g(2,3,1,1,3), g(2,3,1,2,1), g(2,3,1,2,2), g(2,3,1,2,3), g(2,3,1,3,1), g(2,3,1,3,2), g(2,3,1,3,3), g(2,3,2,1,1), g(2,3,2,1,2), g(2,3,2,1,3), g(2,3,2,2,1), g(2,3,2,2,2), g(2,3,2,2,3), g(2,3,2,3,1), g(2,3,2,3,2), g(2,3,2,3,3), g(2,3,3,1,1), g(2,3,3,1,2), g(2,3,3,1,3), g(2,3,3,2,1), g(2,3,3,2,2), g(2,3,3,2,3), g(2,3,3,3,1), g(2,3,3,3,2), g(2,3,3,3,3), g(3,1,1,1,1), g(3,1,1,1,2), g(3,1,1,1,3), g(3,1,1,2,1), g(3,1,1,2,2), g(3,1,1,2,3), g(3,1,1,3,1), g(3,1,1,3,2), g(3,1,1,3,3), g(3,1,2,1,1), g(3,1,2,1,2), g(3,1,2,1,3), g(3,1,2,2,1), g(3,1,2,2,2), g(3,1,2,2,3), g(3,1,2,3,1), g(3,1,2,3,2), g(3,1,2,3,3), g(3,1,3,1,1), g(3,1,3,1,2), g(3,1,3,1,3), g(3,1,3,2,1), g(3,1,3,2,2), g(3,1,3,2,3), g(3,1,3,3,1), g(3,1,3,3,2), g(3,1,3,3,3), g(3,2,1,1,1), g(3,2,1,1,2), g(3,2,1,1,3), g(3,2,1,2,1), g(3,2,1,2,2), g(3,2,1,2,3), g(3,2,1,3,1), g(3,2,1,3,2), g(3,2,1,3,3), g(3,2,2,1,1), g(3,2,2,1,2), g(3,2,2,1,3), g(3,2,2,2,1), g(3,2,2,2,2), g(3,2,2,2,3), g(3,2,2,3,1), g(3,2,2,3,2), g(3,2,2,3,3), g(3,2,3,1,1), g(3,2,3,1,2), g(3,2,3,1,3), g(3,2,3,2,1), g(3,2,3,2,2), g(3,2,3,2,3), g(3,2,3,3,1), g(3,2,3,3,2), g(3,2,3,3,3), g(3,3,1,1,1), g(3,3,1,1,2), g(3,3,1,1,3), g(3,3,1,2,1), g(3,3,1,2,2), g(3,3,1,2,3), g(3,3,1,3,1), g(3,3,1,3,2), g(3,3,1,3,3), g(3,3,2,1,1), g(3,3,2,1,2), g(3,3,2,1,3), g(3,3,2,2,1), g(3,3,2,2,2), g(3,3,2,2,3), g(3,3,2,3,1), g(3,3,2,3,2), g(3,3,2,3,3), g(3,3,3,1,1), g(3,3,3,1,2), g(3,3,3,1,3), g(3,3,3,2,1), g(3,3,3,2,2), g(3,3,3,2,3), g(3,3,3,3,1), g(3,3,3,3,2), g(3,3,3,3,3)}
"""