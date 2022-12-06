input = """
married(X,Y) :- husband_of(X,Y).
married(Y,X) :- husband_of(X,Y).
married(X,Y) :- wife_of(X,Y).
married(Y,X) :- wife_of(X,Y).

parent(X,Y) :- father_of(X,Y).
parent(X,Y) :- mother_of(X,Y).
parent(X,Y) :- married(X,Z), father_of(Z,Y).
parent(X,Y) :- married(X,Z), mother_of(Z,Y).

ancestor(X,Y) :- parent(X,Y).
ancestor(X,Y) :- ancestor(X,U), ancestor(U,Y).
"""
output = """
{}
"""
