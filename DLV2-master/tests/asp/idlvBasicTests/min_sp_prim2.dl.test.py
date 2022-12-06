input = """
% Computes the minimum spanning tree by a weighted graph by using Prim
% algorithm.
% Version with weakconstraints with weights as variables.
root(a).
node(a). node(b). node(c). node(d). node(e).
edge(a,b,4). edge(a,c,3). edge(c,b,2). edge(c,d,3). edge(b,e,4). edge(d,e,5).

redundantEdge(X,Y,C) :- edge(X,Y,C), edge(X,Y,C1), C>C1.

in_tree(X,Y,C) | out_tree(X,Y) :-
	edge(X,Y,C),
	not redundantEdge(X,Y,C),
	reached(X).

:- root(X), in_tree(_,X,C).

:- in_tree(X,Y,_), in_tree(Z,Y,_), X != Z.

reached(X):- root(X).
reached(Y):- in_tree(X,Y,C).
:-node(X), not reached(X).

%:- in_tree(X,Y,C). [C:1]
"""
output = """
{edge(a,b,4), edge(a,c,3), edge(b,e,4), edge(c,b,2), edge(c,d,3), edge(d,e,5), in_tree(a,b,4), in_tree(a,c,3), in_tree(c,d,3), in_tree(d,e,5), node(a), node(b), node(c), node(d), node(e), out_tree(b,e), out_tree(c,b), reached(a), reached(b), reached(c), reached(d), reached(e), root(a)}
{edge(a,b,4), edge(a,c,3), edge(b,e,4), edge(c,b,2), edge(c,d,3), edge(d,e,5), in_tree(a,c,3), in_tree(c,b,2), in_tree(c,d,3), in_tree(d,e,5), node(a), node(b), node(c), node(d), node(e), out_tree(a,b), out_tree(b,e), reached(a), reached(b), reached(c), reached(d), reached(e), root(a)}
{edge(a,b,4), edge(a,c,3), edge(b,e,4), edge(c,b,2), edge(c,d,3), edge(d,e,5), in_tree(a,b,4), in_tree(a,c,3), in_tree(b,e,4), in_tree(c,d,3), node(a), node(b), node(c), node(d), node(e), out_tree(c,b), out_tree(d,e), reached(a), reached(b), reached(c), reached(d), reached(e), root(a)}
{edge(a,b,4), edge(a,c,3), edge(b,e,4), edge(c,b,2), edge(c,d,3), edge(d,e,5), in_tree(a,c,3), in_tree(b,e,4), in_tree(c,b,2), in_tree(c,d,3), node(a), node(b), node(c), node(d), node(e), out_tree(a,b), out_tree(d,e), reached(a), reached(b), reached(c), reached(d), reached(e), root(a)}
"""
