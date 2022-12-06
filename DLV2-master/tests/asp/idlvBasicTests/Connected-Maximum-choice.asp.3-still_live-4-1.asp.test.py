input = """
% a cell may live, except for the ones at the border
{ lives(X,Y) } <= 1 :- value(X), value(Y), X>0, Y>0, size(N), X<=N, Y<=N.
size(3).
value(0).
value(1).
value(2).
value(3).
value(4).
"""
output = """
{size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,2), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,2), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,2), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,2), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,2), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,2), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,2), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,2), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,2), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,2), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,2), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,2), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,2), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,2), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,2), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,2), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,2), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,2), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,2), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,2), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,2), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,2), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,2), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,2), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,2), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,2), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,2), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,2), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,2), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,2), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,2), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,2), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,2), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,2), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,2), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,2), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,2), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,2), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,2), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,2), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,2), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,2), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,2), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,2), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,2), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,2), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,2), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,2), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,2), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,2), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,2), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,2), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,2), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,2), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,2), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,2), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,2), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,2), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,2), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,2), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,2), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,2), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,2), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,2), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,2), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,2), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,2), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,2), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,2), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,2), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,2), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,2), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,2), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,2), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,2), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,2), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,2), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,2), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,2), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,2), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,2), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,2), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,2), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,2), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,2), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,2), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,2), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,2), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,2), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,2), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,2), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,2), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,2), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,2), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,2), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,2), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,2), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,2), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,2), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,2), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,2), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,2), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,2), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,2), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,2), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,2), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,2), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,2), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,2), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,2), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,2), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,2), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,2), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,2), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,2), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,2), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,2), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,2), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,2), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,2), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,2), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,2), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,2), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,2), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,2), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,2), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,2), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,2), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,2), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,2), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,2), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,2), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,2), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,2), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,2), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,2), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,2), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,2), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,2), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,2), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,2), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,2), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,2), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,2), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,2), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,2), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,2), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,2), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,2), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,2), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,2), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,2), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,2), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,2), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,2), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,2), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,2), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,2), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(2,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,2), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,2), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,2), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,2), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,2), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,2), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,2), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,2), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,2), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,1), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,2), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,2), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,2), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,2), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
{lives(1,1), lives(1,2), lives(1,3), lives(2,1), lives(2,2), lives(2,3), lives(3,1), lives(3,2), lives(3,3), size(3), value(0), value(1), value(2), value(3), value(4)}
"""
