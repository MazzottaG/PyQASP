input = """
s(1,1,1).
s(2,1,2).
one(1).
ok1 :- one(One), #count{ S: s(S,One,X) } = 2.
ok2 :- one(One), #count{ S: s(S,X,One) } = 1.
"""
output = """
{ok1, ok2, one(1), s(1,1,1), s(2,1,2)}
"""
