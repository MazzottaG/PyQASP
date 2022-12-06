input = """
node(a).
node(b).
node(c).
edge(b,c).
in_tree(X,Y) | out_tree(X,Y) :- edge(X,Y).
:- node(X), not #count{X:in_tree(X,Y)} = 1.


"""
output = """
INCOHERENT
"""
