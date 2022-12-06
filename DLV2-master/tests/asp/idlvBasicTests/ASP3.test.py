input = """
%instance

edge(a,b). edge(b,c). edge(c,a).

%encoding

% nodes and values
node(X) :- edge(X,Y).
node(Y) :- edge(X,Y).
num_edges(N) :- N = #sum { 1,X,Y : edge(X,Y) }.
num(0).
num(N) :- num(N1), N=N1+1, num_edges(E), N<=E.

% assignment to nodes
{ value(X,N) : num(N) } = 1 :- node(X).

% assignment to edges
{ edge_value(edge(X,Y),N) : num(N), N>0 } = 1 :- edge(X,Y).

% relates node values with edge values
:- not edge_value(edge(X,Y),M-N), edge(X,Y), value(X,M), value(Y,N), N < M.
:- not edge_value(edge(X,Y),N-M), edge(X,Y), value(X,M), value(Y,N), N > M.

% alldifferent values
:- value(X,N), value(Y,N), num(N), X<Y.
:- edge_value(X,N), edge_value(Y,N), num(N), X<Y.

"""
output = """
{edge(a,b), edge(b,c), edge(c,a), edge_value(edge(a,b),2), edge_value(edge(b,c),1), edge_value(edge(c,a),3), node(a), node(b), node(c), num(0), num(1), num(2), num(3), num_edges(3), value(a,3), value(b,1), value(c,0)}
{edge(a,b), edge(b,c), edge(c,a), edge_value(edge(a,b),1), edge_value(edge(b,c),2), edge_value(edge(c,a),3), node(a), node(b), node(c), num(0), num(1), num(2), num(3), num_edges(3), value(a,3), value(b,2), value(c,0)}
{edge(a,b), edge(b,c), edge(c,a), edge_value(edge(a,b),3), edge_value(edge(b,c),2), edge_value(edge(c,a),1), node(a), node(b), node(c), num(0), num(1), num(2), num(3), num_edges(3), value(a,0), value(b,3), value(c,1)}
{edge(a,b), edge(b,c), edge(c,a), edge_value(edge(a,b),1), edge_value(edge(b,c),2), edge_value(edge(c,a),3), node(a), node(b), node(c), num(0), num(1), num(2), num(3), num_edges(3), value(a,0), value(b,1), value(c,3)}
{edge(a,b), edge(b,c), edge(c,a), edge_value(edge(a,b),2), edge_value(edge(b,c),1), edge_value(edge(c,a),3), node(a), node(b), node(c), num(0), num(1), num(2), num(3), num_edges(3), value(a,0), value(b,2), value(c,3)}
{edge(a,b), edge(b,c), edge(c,a), edge_value(edge(a,b),3), edge_value(edge(b,c),1), edge_value(edge(c,a),2), node(a), node(b), node(c), num(0), num(1), num(2), num(3), num_edges(3), value(a,0), value(b,3), value(c,2)}
{edge(a,b), edge(b,c), edge(c,a), edge_value(edge(a,b),1), edge_value(edge(b,c),3), edge_value(edge(c,a),2), node(a), node(b), node(c), num(0), num(1), num(2), num(3), num_edges(3), value(a,2), value(b,3), value(c,0)}
{edge(a,b), edge(b,c), edge(c,a), edge_value(edge(a,b),2), edge_value(edge(b,c),3), edge_value(edge(c,a),1), node(a), node(b), node(c), num(0), num(1), num(2), num(3), num_edges(3), value(a,1), value(b,3), value(c,0)}
{edge(a,b), edge(b,c), edge(c,a), edge_value(edge(a,b),2), edge_value(edge(b,c),3), edge_value(edge(c,a),1), node(a), node(b), node(c), num(0), num(1), num(2), num(3), num_edges(3), value(a,2), value(b,0), value(c,3)}
{edge(a,b), edge(b,c), edge(c,a), edge_value(edge(a,b),3), edge_value(edge(b,c),1), edge_value(edge(c,a),2), node(a), node(b), node(c), num(0), num(1), num(2), num(3), num_edges(3), value(a,3), value(b,0), value(c,1)}
{edge(a,b), edge(b,c), edge(c,a), edge_value(edge(a,b),1), edge_value(edge(b,c),3), edge_value(edge(c,a),2), node(a), node(b), node(c), num(0), num(1), num(2), num(3), num_edges(3), value(a,1), value(b,0), value(c,3)}
{edge(a,b), edge(b,c), edge(c,a), edge_value(edge(a,b),3), edge_value(edge(b,c),2), edge_value(edge(c,a),1), node(a), node(b), node(c), num(0), num(1), num(2), num(3), num_edges(3), value(a,3), value(b,0), value(c,2)}
"""
