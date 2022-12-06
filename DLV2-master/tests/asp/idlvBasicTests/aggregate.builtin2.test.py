input = """
a(X,N) :- a(X),b(Y), #count{1:X=Y;1:X=Y+1}>1.
a(1..3).
b(2..4).
"""
output = """
"""
