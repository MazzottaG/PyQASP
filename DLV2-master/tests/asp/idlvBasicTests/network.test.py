input = """
connected(a,b).
connected(a,f).
connected(b,c).
connected(b,d).
connected(c,e).
connected(d,e).
connected(f,e).

node(X) :- connected(X,Y).
node(Y) :- connected(X,Y).

reaches(X,X) :- node(X), not offline(X).
reaches(X,Z) :- reaches(X,Y), connected(Y,Z), not offline(Z).
"""
output = """
{connected(a,b), connected(a,f), connected(b,c), connected(b,d), connected(c,e), connected(d,e), connected(f,e), node(a), node(b), node(c), node(d), node(e), node(f), reaches(a,a), reaches(a,b), reaches(a,c), reaches(a,d), reaches(a,e), reaches(a,f), reaches(b,b), reaches(b,c), reaches(b,d), reaches(b,e), reaches(c,c), reaches(c,e), reaches(d,d), reaches(d,e), reaches(e,e), reaches(f,e), reaches(f,f)}
"""