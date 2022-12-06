input = """
q("1","2","3").
q("3","te,s,t!","new").
q(1,2,"Work?").

p(f(f(X,Y),Z)) :- q(X,Y,Z).
r(X,Y,Z) :- p(f(f(X,Y),Z)).
"""
output = """
{p(f(f("1","2"),"3")), p(f(f("3","te,s,t!"),"new")), p(f(f(1,2),"Work?")), q("1","2","3"), q("3","te,s,t!","new"), q(1,2,"Work?"), r("1","2","3"), r("3","te,s,t!","new"), r(1,2,"Work?")}
"""
