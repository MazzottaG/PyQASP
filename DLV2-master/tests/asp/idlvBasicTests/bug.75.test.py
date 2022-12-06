input = """
edb(0). edb(1).
a(X) | na(X) :- edb(X).
%:- a(X). [X:X]

"""
output = """
{a(0), edb(0), edb(1), na(1)}
{a(0), a(1), edb(0), edb(1)}
{edb(0), edb(1), na(0), na(1)}
{a(1), edb(0), edb(1), na(0)}
"""
